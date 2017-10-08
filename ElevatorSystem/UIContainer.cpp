#include "stdafx.h"
#include "UI.h"

IMPLEMENT_DYNCREATE(UIContainer, UIControl)

UIContainer::UIContainer()
{
}

UIContainer::~UIContainer()
{
    destroy();
}

void UIContainer::setManager(UIManager* pManager)
{
    UIControl::setManager(pManager);
    std::for_each(m_ctrls.begin(), m_ctrls.end(), [&](std::shared_ptr<UIControl> x)
    {
        x->setManager(pManager);
    });
}

void UIContainer::init(WORD wParam /*= 0*/)
{

}

bool UIContainer::addControl(std::shared_ptr<UIControl> ctrl)
{
    ASSERT(ctrl);
    if (m_ctrls.find(ctrl) == m_ctrls.end())
    {
        m_ctrls.insert(ctrl);
        ctrl->setParent(this);
        ctrl->setManager(getManager());
        return getManager()->registerControl(ctrl);
    }
    TRACE("add control failed: 0x%08X <%s> [%d] '%S'\n", ctrl, ctrl->GetRuntimeClass()->m_lpszClassName, ctrl->getId(), ctrl->getName());
    return false;
}

bool UIContainer::removeControl(std::shared_ptr<UIControl> ctrl)
{
    ASSERT(ctrl);
    if (!m_ctrls.empty())
    {
        auto found = m_ctrls.find(ctrl);
        if (found != m_ctrls.end())
        {
            m_ctrls.erase(found);
            return getManager()->unregisterControl(ctrl);
        }
    }
    TRACE("remove control failed: 0x%08X <%s> [%d] '%S'\n", ctrl, ctrl->GetRuntimeClass()->m_lpszClassName, ctrl->getId(), ctrl->getName());
    return false;
}

void UIContainer::destroy()
{
    std::for_each(m_ctrls.begin(), m_ctrls.end(), [&](std::shared_ptr<UIControl> x)
    {
        getManager()->unregisterControl(x);
    });
    m_ctrls.clear();
}

void UIContainer::onPaint(Graphics& graphics, Rect rect)
{
    if (!isReadyForPaint(rect)) return;

    UIControl::onPaint(graphics, rect);
    onPaintChildren(graphics, rect);
}

void UIContainer::onPaintChildren(Graphics& graphics, Rect rect)
{
    std::for_each(m_ctrls.rbegin(), m_ctrls.rend(), [&](std::shared_ptr<UIControl> x)
    {
        x->onPaint(graphics, rect);
    });
}

std::shared_ptr<UIControl> UIContainer::FindControl(UI_FIND_CALL proc, void* data, UINT flags)
{
    if (!hasAttr(STATE_VISIBLE))
        return nullptr;

    if ((flags & UIFIND_ME_FIRST) != 0) {
        auto ctrl = UIControl::FindControl(proc, data, flags);
        if (ctrl) return ctrl;
    }

    if ((flags & UIFIND_TOP_FIRST) != 0) // 正序
    {
        for (auto it = m_ctrls.begin(); it != m_ctrls.end(); it++)
        {
            auto ctrl = it->get()->FindControl(proc, data, flags);
            if (ctrl)
                return ctrl;
        }
    }
    else // 逆序
    {
        for (auto it = m_ctrls.rbegin(); it != m_ctrls.rend(); it++)
        {
            auto ctrl = it->get()->FindControl(proc, data, flags);
            if (ctrl)
                return ctrl;
        }
    }

    if ((flags & UIFIND_ME_FIRST) == 0)
    {
        auto ctrl = UIControl::FindControl(proc, data, flags);
        if (ctrl) return ctrl;
    }

    return nullptr;
}

std::shared_ptr<UIControl> UIContainer::FindControlById(WORD id)
{
    return FindControl(UIManager::__FindControlFromId, (void*)(id), UIFIND_HITTEST | UIFIND_TOP_FIRST);
}

std::shared_ptr<UIControl> UIContainer::FindControlLv1(WORD id)
{
    auto ctrl = std::find_if(m_ctrls.begin(), m_ctrls.end(), [&](std::shared_ptr<UIControl> x)
    {
        return x->getId() == id;
    });
    return ctrl == m_ctrls.end() ? nullptr : *ctrl;
}

std::shared_ptr<UIControl> UIContainer::FindControlInternal(WORD id)
{
    return FindControlLv1(m_id + id);
}

void UIContainer::onRecursion(UINT message)
{
    std::for_each(m_ctrls.begin(), m_ctrls.end(), [&](std::shared_ptr<UIControl> x)
    {
        x->onRecursion(message);
    });

    UIControl::onRecursion(message);
}

void UIContainer::onCommand(DWORD id, UINT message)
{
    WORD wID = LOWORD(id);
    WORD wRedirect = HIWORD(id);
    WORD wMsg = LOWORD(message);
    WORD wParam = HIWORD(message);

    switch (wMsg)
    {
    case UIMSG_Init:
        destroy();
        setData(wParam);
        init(wParam);
        break;
    default:
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(UIDialog, UIContainer)

UIDialog::UIDialog()
: m_frameColor(UIDFT_DIALOG_FRAME_COLOR)
, m_frameWidth(UIDFT_DIALOG_FRAME_WIDTH)
{

}

UIDialog::~UIDialog()
{

}

void UIDialog::onPaint(Graphics& graphics, Rect rect)
{
    if (!isReadyForPaint(rect)) return;

    if (hasAttr(STATE_MASK))
        m_manager->doDialogBkMask(graphics);
    onPaintBkg(graphics, rect);
    onPaintFrame(graphics, rect);
    onPaintChildren(graphics, rect);
}

void UIDialog::onPaintFrame(Graphics& graphics, Rect rect)
{
    rect.Inflate(1, 1);
    if (m_client.Contains(rect))
        return;
    Pen pen(m_frameColor, m_frameWidth);
    graphics.DrawRectangle(&pen, rect);
}
