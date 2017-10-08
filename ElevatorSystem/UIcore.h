#pragma once
#include "UI.h"

template<typename T>
class CNullableObject
{
public:
    CNullableObject() : null(true) {}
    CNullableObject(const T& t) : null(false), obj(t) {}

    operator bool() const { return !null; }
    operator T() const { return obj; }
    bool nullable() const { return null; }
    const T& value() const { return obj; }
    void clear(){ null = true; }
    CNullableObject& operator = (const T& t) { null = false; obj = t; return *this; }
    static CNullableObject& Null() { static CNullableObject o; return o; }

private:
    bool null;
    T obj;
};

template<typename T>
class CTimerObject
{
public:
    CTimerObject() : run(false) {}
    CTimerObject(const T& t, const T& z) : run(true), obj(t), origin(t), zero(z) { ASSERT(t > z); }

    bool signal()
    {
        if (!run)
            return false;
        --obj;
        if (obj > zero)
            return false;
        obj = origin;
        return true;
    }

    CTimerObject& operator = (const CTimerObject& o)
    {
        obj = o.obj; origin = o.origin; zero = o.zero; run = o.run; return *this;
    }

private:
    T obj, origin, zero; bool run;
};

template<typename T>
class CAutoMachineCycleObject
{
public:
    CAutoMachineCycleObject() : run(false) {}
    CAutoMachineCycleObject(const T& t, const T& z) : run(true), obj(t), zero(z) { ASSERT(t >= z); }
    CAutoMachineCycleObject(const CAutoMachineCycleObject& o, const T& delta) : run(o.run), obj(o.obj + delta), zero(o.zero) {}

    bool signal()
    {
        if (!run)
            return false;
        obj--;
        if (obj > zero)
            return false;
        run = false;
        return true;
    }

    CAutoMachineCycleObject& operator = (const CAutoMachineCycleObject& o)
    {
        obj = o.obj; zero = o.zero; run = o.run; return *this;
    }

private:
    T obj, zero; bool run;
};

class UIESDataStruct
{
public:
    typedef int DataType;

    struct Data
    {
        DataType CLK_PER_META_PROC; // 元步骤的时钟周期
        DataType NPLS_CLK_TRANSTION; // 匀速的时钟周期基数
        DataType WAIT_CLK; // 等待的时钟周期基数
        DataType LEVELS; // 楼层数
        DataType STARTS; // 开始的楼层，可以为地下层
        DataType ORIGINS; // 初始楼层

        Data() :
            CLK_PER_META_PROC(UICFG_META_CLK),
            NPLS_CLK_TRANSTION(UICFG_NPLS),
            WAIT_CLK(UICFG_WAIT),
            LEVELS(UICFG_LEVEL),
            STARTS(UICFG_START),
            ORIGINS(UICFG_ORIGIN)
        {}
    };

private:
    Data data;

public:
    UIESDataStruct();

    enum _DataStructType
    {
        META_CLK = 0x10,
        NPLS,
        WAIT,
        LEVEL,
        START,
        ORIGIN
    };

    enum { INVALID_VALUE = INT_MAX };

    LRESULT set(_DataStructType, DataType);
    LRESULT get(_DataStructType) const;
    void reset();

private:
    bool checked(_DataStructType, DataType);
};

class UIESStateStruct
{
public:
    UIESStateStruct();
    ~UIESStateStruct();

    typedef UIESDataStruct::Data Data;
    typedef UIESDataStruct::DataType DataType;

    enum PrefetchType
    {
        Up,
        Down,
        Internal
    };

    enum State
    {
        Move,
        Wait,
        Stop,
        Accelerate,
        Decelerate
    };

    struct WorkState
    {
        State state;
        DataType level;
        DataType step;
        bool direction;

        WorkState() :
            state(Stop),
            level(UICFG_ORIGIN),
            step(0),
            direction(true)
        {}
    };
    
    void reset(const Data&);
    void prefetch(PrefetchType, UINT); // 中断
    bool work(bool& levelChanged, bool& stateChanged); // 主循环
    void handle(); // 中断处理

    LPCTSTR getStateName(UINT) const;
    UINT getState() const { return stat.state; }
    DataType getLevel() const { return stat.level; }
    void disable(std::function<void(DataType)>);

private:
    void resetInternal();
    void handleInternal(DataType);
    void insertIns(bool, DataType);
    bool fetchInternal(bool direction, DataType local, DataType& level);
    bool fetchInternalDestroy();
    void internalCheckLevel();

    void internalChangeStateToWait();
    void internalChangeStateToStop();

    struct PrefetchInstruction
    {
        PrefetchType type;
        DataType level;

        PrefetchInstruction() = default;
        PrefetchInstruction(PrefetchType t, UINT l)
            : type(t), level(l) {}
    };

    struct CacheLevelStruct
    {
        DataType level;
        bool direction;

        CacheLevelStruct() = default;
        CacheLevelStruct(DataType l, bool d)
            : level(l), direction(d) {}
    };

    std::vector<PrefetchInstruction> cache;
    std::set<DataType> up_ins;
    std::set<DataType> down_ins;
    std::set<DataType> clear_queue;
    Data data;
    WorkState stat;
    CNullableObject<CacheLevelStruct> cacheLevel;
    CTimerObject<DataType> timer;
    CAutoMachineCycleObject<DataType> auto_machine_cycle;
    DataType levelOld;
    DataType levelCycle;
};