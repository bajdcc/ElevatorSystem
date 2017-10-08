#include "stdafx.h"
#include "UIcore.h"

UIESDataStruct::UIESDataStruct()
{

}

LRESULT UIESDataStruct::set(_DataStructType type, DataType value)
{
    if (!checked(type, value)) return E_FAIL;

    switch (type)
    {
    case META_CLK:
        data.CLK_PER_META_PROC = value; break;
    case NPLS:
        data.NPLS_CLK_TRANSTION = value; break;
    case WAIT:
        data.WAIT_CLK = value; break;
    case LEVEL:
        data.LEVELS = value; break;
    case START:
        data.STARTS = value; break;
    case ORIGIN:
        data.ORIGINS = value; break;
    }

    return S_OK;
}

LRESULT UIESDataStruct::get(_DataStructType type) const
{
    switch (type)
    {
    case META_CLK:
        return data.CLK_PER_META_PROC;
    case NPLS:
        return data.NPLS_CLK_TRANSTION;
    case WAIT:
        return data.WAIT_CLK;
    case LEVEL:
        return data.LEVELS;
    case START:
        return data.STARTS;
    case ORIGIN:
        return data.ORIGINS;
    }

    return INVALID_VALUE;
}

bool UIESDataStruct::checked(_DataStructType type, DataType value)
{
    switch (type)
    {
    case META_CLK:
        return value > 0;
    case NPLS:
        return value >= 0;
    case WAIT:
        return value > 0;
    case LEVEL:
        return value > 1;
    case START:
        return true;
    case ORIGIN:
        return data.STARTS <= value && value < data.STARTS + data.LEVELS;
    }
    return false;
}

void UIESDataStruct::reset()
{
    data.CLK_PER_META_PROC = UICFG_META_CLK;
    data.NPLS_CLK_TRANSTION = UICFG_NPLS;
    data.WAIT_CLK = UICFG_WAIT;
    data.LEVELS = UICFG_LEVEL;
    data.STARTS = UICFG_START;
    data.ORIGINS = UICFG_ORIGIN;
}

//////////////////////////////////////////////////////////////////////////

UIESStateStruct::UIESStateStruct()
{

}

UIESStateStruct::~UIESStateStruct()
{

}

LPCTSTR UIESStateStruct::getStateName(UINT sta) const
{
    switch (sta)
    {
    case UIESStateStruct::Move:
        return _T("运行");
    case UIESStateStruct::Wait:
        return _T("等待");
    case UIESStateStruct::Stop:
        return _T("停止");
    case UIESStateStruct::Accelerate:
        return _T("加速");
    case UIESStateStruct::Decelerate:
        return _T("减速");
    default:
        return _T("未知");
    }
}

void UIESStateStruct::reset(const Data& d)
{
    data = d;
    resetInternal();
}

void UIESStateStruct::prefetch(PrefetchType type, UINT level)
{
    cache.push_back(PrefetchInstruction(type, level));
}

void UIESStateStruct::handle()
{
    std::for_each(cache.begin(), cache.end(), [&](PrefetchInstruction ins)
    {
        ASSERT(data.ORIGINS <= ins.level && ins.level < data.STARTS + data.LEVELS);

        switch (ins.type)
        {
        case Up:
            ASSERT(ins.level != data.STARTS + data.LEVELS - 1);
            up_ins.insert(ins.level);
            break;
        case Down:
            ASSERT(ins.level != data.STARTS);
            down_ins.insert(ins.level);
            break;
        case Internal:
            handleInternal(ins.level);
            break;
        }
    });
    cache.clear();
}

void UIESStateStruct::resetInternal()
{
    cache.clear();
    up_ins.clear();
    down_ins.clear();
    clear_queue.clear();
    stat.direction = true;
    stat.level = data.ORIGINS;
    levelOld = stat.level;
    levelCycle = 2 + 4 * data.NPLS_CLK_TRANSTION;
    timer = decltype(timer)(data.CLK_PER_META_PROC, 0);
    internalChangeStateToStop();
}

void UIESStateStruct::handleInternal(DataType level)
{
    if (stat.level == level) insertIns(stat.direction, level);
    else insertIns(level > stat.level, level);
}

void UIESStateStruct::insertIns(bool direction, int level)
{
    if (direction) up_ins.insert(level);
    else down_ins.insert(level);
}

void UIESStateStruct::internalCheckLevel()
{
    if (stat.step > 2 * data.NPLS_CLK_TRANSTION + 1)
    {
        do 
        {
            stat.step -= levelCycle;
            stat.level++;
        } while (stat.step > 2 * data.NPLS_CLK_TRANSTION + 1);
    }
    else if (stat.step < -2 * data.NPLS_CLK_TRANSTION - 1)
    {
        do 
        {
            stat.step += levelCycle;
            stat.level--;
        } while (stat.step < -2 * data.NPLS_CLK_TRANSTION - 1);
    }
}

bool UIESStateStruct::work(bool& levelChanged, bool& stateChanged)
{
    // 核心算法
    if (!timer.signal())
        return false;

    auto lvl_old = levelOld;
    std::tuple<decltype(stat.level), decltype(stat.state)> old(stat.level, stat.state);

    static bool targetChanged = false;
    bool emptyQueue = !fetchInternal(stat.direction, stat.level, levelOld);
    targetChanged = targetChanged || levelOld != lvl_old;

    if (emptyQueue) stat.direction = !stat.direction;

    switch (stat.state)
    {
    case UIESStateStruct::Move:
        stat.step += stat.direction ? 2 : -2;
        if (auto_machine_cycle.signal())
        {
            stat.state = Decelerate;
        }
        else
        {
            if ((targetChanged) && (stat.step != 0) &&
                ((stat.direction && levelOld < lvl_old) || (!stat.direction && levelOld > lvl_old)))
            {
                DataType delay = (2 * data.NPLS_CLK_TRANSTION + 2) * (levelOld - lvl_old);
                auto_machine_cycle = decltype(auto_machine_cycle)(auto_machine_cycle, delay);
                targetChanged = false;
            }
            internalCheckLevel();
        }
        break;
    case UIESStateStruct::Wait:
        if (auto_machine_cycle.signal())
        {
            if (stat.level == levelOld)
            {
                fetchInternalDestroy();
                if (emptyQueue)
                {
                    internalChangeStateToStop();
                }
                else
                {                    
                    internalChangeStateToWait();
                }
            }
            else
            {
                stat.state = Accelerate;
            }
        }
        break;
    case UIESStateStruct::Stop:
        if (emptyQueue);
        else if (stat.level == levelOld)
            internalChangeStateToWait();
        else
            stat.state = Accelerate;
        break;
    case UIESStateStruct::Accelerate:
        if (true)
        {
            ASSERT(stat.step == 0);
            stat.step += stat.direction ? 1 : -1;
            stat.state = Move;
            DataType delta = abs(stat.level - levelOld);
            DataType delay = 2 * (data.NPLS_CLK_TRANSTION + 1) * delta - 2;
            if (delta > 1) delay -= delta - 1;
            auto_machine_cycle = decltype(auto_machine_cycle)(delay, 0);
        }
        break;
    case UIESStateStruct::Decelerate:
        ASSERT((stat.direction && stat.step == -1) || (!stat.direction && stat.step == 1));
        internalChangeStateToWait();
        break;
    }

    levelChanged = std::get<0>(old) != stat.level;
    stateChanged = std::get<1>(old) != stat.state;
    return true;
}

void UIESStateStruct::internalChangeStateToWait()
{
    stat.step = 0;
    stat.state = Wait;
    auto_machine_cycle = decltype(auto_machine_cycle)(data.WAIT_CLK, 1);
}

void UIESStateStruct::internalChangeStateToStop()
{
    stat.step = 0;
    stat.state = Stop;
    auto_machine_cycle = decltype(auto_machine_cycle)();
}

bool UIESStateStruct::fetchInternal(bool direction, DataType local, DataType& level)
{
    if (cacheLevel)
    {
        level = cacheLevel.value().level;
        return true;
    }

    if (direction)
    {
        auto found = std::find_if(up_ins.begin(), up_ins.end(), [=](int l) { return l >= local; });
        if (found == up_ins.end())
        {
            auto found2 = std::find_if(down_ins.rbegin(), down_ins.rend(), [=](int l) { return l >= local; });
            if (found2 == down_ins.rend() || *found2 == level)
                return false;
            level = *found2;
        }
        else
            level = *found;
        cacheLevel = CacheLevelStruct(level, direction);
    }
    else
    {
        auto found = std::find_if(down_ins.rbegin(), down_ins.rend(), [=](int l) { return l <= local; });
        if (found == down_ins.rend())
        {
            auto found2 = std::find_if(up_ins.begin(), up_ins.end(), [=](int l) { return l <= local; });
            if (found2 == up_ins.end() || *found2 == level)
                return false;
            level = *found2;
        }
        else
            level = *found;
        cacheLevel = CacheLevelStruct(level, direction);
    }

    return true;
}

bool UIESStateStruct::fetchInternalDestroy()
{
    if (!cacheLevel)
        return false;

    auto cls = cacheLevel.value();
    if (cls.direction)
        up_ins.erase(cls.level);
    else
        down_ins.erase(cls.level);
    cacheLevel.clear();
    clear_queue.insert(cls.level);

    return true;
}

void UIESStateStruct::disable(std::function<void(DataType)> func)
{
    std::for_each(clear_queue.begin(), clear_queue.end(), func);
    clear_queue.clear();
}

