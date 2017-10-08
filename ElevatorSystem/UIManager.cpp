#include "stdafx.h"
#include "resource.h"
#include "UI.h"
#include "UIext.h"
#include <locale.h>

IMPLEMENT_DYNCREATE(UIManager, CObject)

UIManager::UIManager() : m_dlg(nullptr)
{
}

UIManager::~UIManager()
{
    TRACE("<%s> destroy\n", GetRuntimeClass()->m_lpszClassName);
    m_layout.reset();
}

void UIManager::init()
{
    _tsetlocale(LC_CTYPE, _T("chs"));

    TRACE("<%s> init\n", GetRuntimeClass()->m_lpszClassName);

    CString caption; caption.LoadString(IDS_APP_TITLE);

    m_dlg->SetWindowPos(NULL, 0, 0, UI_WINDOW_W, UI_WINDOW_H, SWP_NOACTIVATE);
    m_dlg->SetWindowText(caption);
    m_dlg->CenterWindow();
    m_dlg->GetClientRect(m_client);

    {
        auto cx = GetSystemMetrics(SM_CXSCREEN);
        auto cy = GetSystemMetrics(SM_CYSCREEN);
        CDC* pDC = m_dlg->GetDC();
        m_bufferedDC.CreateCompatibleDC(pDC);
        m_bufferedBMP.CreateCompatibleBitmap(pDC, cx, cy);
        m_bufferedDC.SelectObject(m_bufferedBMP);
        m_maskDC.CreateCompatibleDC(pDC);
        m_maskBMP.CreateCompatibleBitmap(pDC, cx, cy);
        m_maskDC.SelectObject(m_maskBMP);
        pDC->Detach();
        ASSERT(m_bufferedDC.m_hDC);
        ASSERT(m_bufferedBMP.m_hObject);
        ASSERT(m_maskDC.m_hDC);
        ASSERT(m_maskBMP.m_hObject);
    }

    m_layout = UICreateObject(UIContainer);
    m_layout->setRect(Rect(m_client.left, m_client.top, m_client.Width(), m_client.Height()));
    m_layout->setManager(this);

    auto rect = m_layout->getRect();

    {
        auto mask = UICreateObject(UIContainer);
        mask->setName(_T("全局掩码"));
        mask->setRect(rect);
        mask->setTransparent(true);
        mask->setId(UID_X_MASK);
        mask->setAttr(0, STATE_VISIBLE);
        mask->setHitTest(HTERROR);
        m_layout->addControl(mask);

        auto rt(rect);
        rt.Inflate(-150, -100);

        auto dialog = UICreateObject(UIESAboutDialog);
        dialog->setId(UID_X_DIALOG);
        dialog->setRect(rt);
        dialog->setAttr(STATE_MASK);
        mask->addControl(dialog);
    }

    auto root = UICreateObject(UIContainer);
    root->setName(_T("顶级布局"));
    root->setRect(rect);
    root->setId(UID_TOP_LAYOUT);
    m_layout->addControl(root);

    {
        auto rt(rect);
        rt.Height = 40;

        auto sys = UICreateObject(UIESCaption);
        sys->setId(UID_SYS_LAYOUT);
        sys->setRect(rt);
        root->addControl(sys);
    }

    {
        Rect rt(50, 50, 0, 0);
        auto level = UICreateObject(UIESLevelPanel);
        level->setId(UID_LEVEL_PANEL);
        level->setRect(rt);
        root->addControl(level);
    }

    {
        Rect rt(rect.GetRight() - 100, 50, 60, 40);
        auto state = UICreateObject(UIESStatePanel);
        state->setId(UID_STATE_PANEL);
        state->setRect(rt);
        root->addControl(state);
    }

    {
        Rect rt(50, 200, 0, 0);
        auto outBtn = UICreateObject(UIESOutsideButtonPanel);
        outBtn->setId(UID_OUTSIDE_PANEL);
        outBtn->setRect(rt);
        root->addControl(outBtn);
    }

    {
        Rect rt(rect.GetRight() - 50 - 110, 200, 0, 0);
        auto inBtn = UICreateObject(UIESInsideButtonPanel);
        inBtn->setId(UID_INSIDE_PANEL);
        inBtn->setRect(rt);
        root->addControl(inBtn);
    }

    {
        Rect rt(rect.GetRight() - 280, rect.GetBottom() - 45, 270, 35);
        auto timer = UICreateObject(UIESTimerCenter);
        timer->setId(UID_TIMER);
        timer->setRect(rt);
        root->addControl(timer);
    }

    {
        auto ctrlCenter = UICreateObject(UIESControlDispatcher);
        ctrlCenter->setId(UID_X_CONTROL_DISPATCHER);
        root->addControl(ctrlCenter);
    }

    {
        auto ctrlDispatcher = UICreateObject(UIESControlCenter);
        ctrlDispatcher->setId(UID_X_CONTROL_CENTER);
        root->addControl(ctrlDispatcher);
    }

    {
        auto timer = UICreateObject(UITimer);
        timer->setId(UID_X_TIMER);
        root->addControl(timer);
    }

    sendCommandMessage(m_layout.get(), WM_CREATE, 0, 0, 0, Point(), true);
}

bool UIManager::registerControl(std::shared_ptr<UIControl> ctrl)
{
    if (ctrl->hasAttr(STATE_NOID))
        return false;
    if (m_register.find(ctrl->getId()) != m_register.end())
    {
        return false;
    }
    TRACE("register ctrl: 0x%08X <%s> [%d] '%S'\n", ctrl.get(), ctrl->GetRuntimeClass()->m_lpszClassName, ctrl->getId(), ctrl->getName());
    m_register.insert(std::make_pair(ctrl->getId(), ctrl));
    return true;
}

bool UIManager::unregisterControl(std::shared_ptr<UIControl> ctrl)
{
    unloadControlLink(ctrl.get());
    if (ctrl->hasAttr(STATE_NOID))
        return false;
    if (m_register.find(ctrl->getId()) != m_register.end())
    {
        TRACE("unregister ctrl: 0x%08X <%s> [%d] '%S'\n", ctrl.get(), ctrl->GetRuntimeClass()->m_lpszClassName, ctrl->getId(), ctrl->getName());
        m_register.erase(ctrl->getId());
        return true;
    }
    return false;
}

void UIManager::doTimer(WORD wId, UINT nElapse)
{
    postCommandMessage(wId, WM_TIMECHANGE, GlobalTimerId, nElapse);
}

std::shared_ptr<UIControl> CALLBACK UIManager::__FindControlFromPoint(std::shared_ptr<UIControl> ctrl, void* data)
{
    return ctrl->getRect().Contains(*static_cast<Point*>(data)) ? ctrl : nullptr;
}

std::shared_ptr<UIControl> CALLBACK UIManager::__FindControlFromId(std::shared_ptr<UIControl> ctrl, void* data)
{
    return ctrl->getId() == (WORD)(data) ? ctrl : nullptr;
}

std::shared_ptr<UIControl> UIManager::FindControl(Point pt)
{
    return m_layout->FindControl(__FindControlFromPoint, &pt, UIFIND_VISIBLE | UIFIND_HITTEST | UIFIND_TOP_FIRST);
}

std::shared_ptr<UIControl> UIManager::FindControl(WORD id, WORD direct)
{
    if (direct != UIDFT_CONTROL_DIRECT_ID)
    {
        auto ctrl = getControl(direct);
        if (ctrl) return ctrl;
        else
        {
            auto ctrl = getControl(id);
            if (ctrl && ctrl->IsKindOf(RUNTIME_CLASS(UIContainer)))
                return std::dynamic_pointer_cast<UIContainer>(ctrl)->FindControlLv1(direct);
        }
        return nullptr;
    }
    return getControl(id);
}

template<typename T /*= UIControl*/>
std::shared_ptr<T> UIManager::getControl(WORD id)
{
    auto o = m_register.find(id);
    if (o != m_register.end())
    {
        ASSERT(o->second->IsKindOf(RUNTIME_CLASS(T)));
        return std::dynamic_pointer_cast<T>(o->second);
    }
    return nullptr;
}

void UIManager::invalidate(UIControl* ctrl)
{
    if (m_event.m_dialog)
    {
        m_dlg->Invalidate();
        return;
    }
    auto rect = ctrl->getRect();
    CRect rt(rect.X, rect.Y, rect.X + rect.Width + 1, rect.Y + rect.Height + 1);
    m_dlg->InvalidateRect(rt);
}

void UIManager::unloadControlLink(UIControl* ctrl)
{
    if (m_event.m_click == ctrl) { m_event.m_click = NULL; m_event.m_mouseCapture = false; ReleaseCapture(); }
    if (m_event.m_focus == ctrl) { m_event.m_focus = NULL; }
    if (m_event.m_hover == ctrl) { m_event.m_hover = NULL; m_event.m_mouseTracking = false; }
    if (m_event.m_key == ctrl) m_event.m_key = NULL;
}

#ifdef _DEBUG
bool UIManager::filterMessage(UINT uMsg)
{
    switch (uMsg)
    {
    case WM_SETCURSOR:
    case WM_MOUSEMOVE:
    case WM_NCHITTEST:
        return true;
    }
    return false;
}
#endif // _DEBUG

void UIManager::postMessage(UIControl* ctrl, UINT nCmd, WORD wDirect /*= UIDFT_CONTROL_DIRECT_ID*/, WORD wParam /*= 0*/)
{
    postCommandMessage(ctrl, nCmd, wDirect, wParam);
}

LRESULT UIManager::sendMessage(UIControl* ctrl, UINT msg, WPARAM wp /*= 0*/, LPARAM lp /*= 0*/, UINT flag /*= 0*/, Point pt /*= Point()*/, bool recursion /*= false*/)
{
    return sendCommandMessage(ctrl, msg, wp, lp, flag, pt, recursion);
}

LRESULT UIManager::sendMessage(WORD id, UINT msg, WPARAM wp /*= 0*/, LPARAM lp /*= 0*/, UINT flag /*= 0*/, Point pt /*= Point()*/, bool recursion /*= false*/)
{
    return sendCommandMessage(id, msg, wp, lp, flag, pt, recursion);
}

void UIManager::postCommandMessage(UIControl* ctrl, UINT nCmd, WORD wDirect /*= UIDFT_CONTROL_DIRECT_ID*/, WORD wParam /*= 0*/)
{
    WORD wDirectId = wDirect == UIDFT_CONTROL_DIRECT_ID ? ctrl->getDirectId() : wDirect;
    WORD wId = ctrl->getId();
    TRACE("command message: 0x%08X <%s> [%d] '%S' - MSG:0x%X Re:[%d] P:[0x%X]\n", ctrl,
        ctrl->GetRuntimeClass()->m_lpszClassName, wId, ctrl->getName(), nCmd, wDirectId, wParam);
    m_dlg->PostMessage(CommandMessageId, MAKELONG(wId, wDirectId), MAKELONG(nCmd, wParam));
}

void UIManager::postCommandMessage(WORD id, UINT nCmd, WORD wDirect /*= UIDFT_CONTROL_DIRECT_ID*/, WORD wParam /*= 0*/)
{
    auto ctrl = getControl(id); ASSERT(ctrl);
    postCommandMessage(ctrl, nCmd, wDirect, wParam);
}

LRESULT UIManager::sendCommandMessage(UIControl* ctrl, UINT msg, WPARAM wp /*= 0*/, LPARAM lp /*= 0*/, UINT flag /*= 0*/, Point pt /*= Point()*/, bool recursion /*= false*/)
{
    ASSERT(ctrl);
#ifdef _DEBUG
    if (!filterMessage(msg))
    {
        TRACE("control message: 0x%08X <%s> [%d] '%S' - %S[0x%x] {W:0x%X,L:0x%X,U:%d,PT:{%d,%d}}\n", ctrl,
            ctrl->GetRuntimeClass()->m_lpszClassName, ctrl->getId(), ctrl->getName(),
            debugGetMessageName(msg), msg, wp, lp, flag, pt.X, pt.Y);
    }
#endif // _DEBUG
    if (recursion)
    {
        ctrl->onRecursion(msg);
        return 0;
    }
    return ctrl->onEvent(msg, wp, lp, flag, pt);
}

LRESULT UIManager::sendCommandMessage(WORD id, UINT msg, WPARAM wp /*= 0*/, LPARAM lp /*= 0*/, UINT flag /*= 0*/, Point pt /*= Point()*/, bool recursion /*= false*/)
{
    auto ctrl = getControl(id);
    if (!ctrl) return 0;
    return sendCommandMessage(ctrl, msg, wp, lp, flag, pt, recursion);
}

void UIManager::doDialogBkMask(Graphics& graphics)
{
    m_maskDC.BitBlt(m_client.left, m_client.top, m_client.Width(), m_client.Height(), NULL, 0, 0, BLACKNESS);
    static BLENDFUNCTION bf;
    bf.SourceConstantAlpha = 80;

    CDC* pDC = CDC::FromHandle(graphics.GetHDC());
    pDC->AlphaBlend(m_client.left, m_client.top, m_client.Width(), m_client.Height(), &m_maskDC,
        m_client.left, m_client.top, m_client.Width(), m_client.Height(), bf);
    graphics.ReleaseHDC(pDC->m_hDC);
}

void UIManager::doDialog(LPCTSTR pstrTitle, LPCTSTR pstrBody)
{
    if (!getControl(UID_X_DIALOG)) return;
    getControl<UILabel>(UID_X_DIALOG + UIESAboutDialog::Title)->setFontText(pstrTitle);
    getControl<UILabel>(UID_X_DIALOG + UIESAboutDialog::Body)->setFontText(pstrBody);
    getControl(UID_X_MASK)->setAttr(STATE_VISIBLE);
    m_event.m_dialog = true;
    m_dlg->Invalidate();
}

void UIManager::endDialog()
{
    getControl(UID_X_MASK)->setAttr(0, STATE_VISIBLE);
    m_dlg->Invalidate();
    m_event.m_dialog = false;
}

//////////////////////////////////////////////////////////////////////////

void UIManager::onSize(UINT nType, int cx, int cy)
{
    m_dlg->Invalidate();
}

void UIManager::onPaint(HDC hDC, CRect rect)
{
    CDC* pDC = CDC::FromHandle(hDC);
    m_bufferedDC.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), pDC, rect.left, rect.top, BLACKNESS);
    Graphics graphics(m_bufferedDC.m_hDC);
    Rect rt(rect.left, rect.top, rect.Width(), rect.Height());
    m_layout->onPaint(graphics, rt);
    pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &m_bufferedDC, rect.left, rect.top, SRCCOPY);
}

void UIManager::onLButtonDown(UINT nFlags, CPoint point)
{
    m_dlg->SetFocus();
    Point pt(point.x, point.y);
    auto ctrl = FindControl(pt);
    if (!ctrl)
    {
        if (m_event.m_focus != NULL)
        {
            sendCommandMessage(m_event.m_focus, WM_KILLFOCUS, 0, 0, nFlags, pt);
            m_event.m_focus = NULL;
        }
        return;
    }
    if (m_event.m_focus != ctrl.get())
    {
        if (m_event.m_focus != NULL)
            sendCommandMessage(m_event.m_focus, WM_KILLFOCUS, 0, 0, nFlags, pt);
        m_event.m_focus = ctrl.get();
        sendCommandMessage(m_event.m_focus, WM_SETFOCUS, 0, 0, nFlags, pt);
    }
    m_event.m_click = ctrl.get();
    sendCommandMessage(ctrl, WM_LBUTTONDOWN, 0, 0, nFlags, pt);
    m_dlg->SetCapture();
    m_event.m_mouseCapture = true;
}

void UIManager::onLButtonUp(UINT nFlags, CPoint point)
{
    if (m_event.m_click == NULL)
        return;
    Point pt(point.x, point.y);
    sendCommandMessage(m_event.m_click, WM_LBUTTONUP, 0, 0, nFlags, pt);
    ReleaseCapture();
    m_event.m_mouseCapture = false;
}

bool UIManager::onSetCursor(UINT nHitTest, UINT message)
{
    if (m_event.m_mouseCapture)
        return false;
    CPoint point;
    GetCursorPos(&point);
    m_dlg->ScreenToClient(&point);
    Point pt(point.x, point.y);
    auto ctrl = FindControl(pt);
    if (!ctrl || !(ctrl->hasAttr(STATE_CURSOR)))
        return false;
    sendCommandMessage(ctrl, WM_SETCURSOR, message, 0, nHitTest, pt);
    return true;
}

void UIManager::onMouseMove(UINT nFlags, CPoint point)
{
    if (!m_event.m_mouseTracking) {
        TRACKMOUSEEVENT tme = { 0 };
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_HOVER | TME_LEAVE;
        tme.hwndTrack = m_dlg->m_hWnd;
        tme.dwHoverTime = 1000UL; // one second
        TrackMouseEvent(&tme);
        m_event.m_mouseTracking = TRUE;
    }

    Point pt(point.x, point.y);
    auto ctrl = FindControl(pt);

    if (ctrl.get() != m_event.m_hover)
    {
        if (m_event.m_hover != NULL)
        {
            sendCommandMessage(m_event.m_hover, WM_MOUSELEAVE, 0, 0, 0, pt);
            m_event.m_hover = NULL;
        }
        if (ctrl)
        {
            sendCommandMessage(ctrl, WM_MOUSEENTER, 0, 0, 0, pt);
            m_event.m_hover = ctrl.get();
        }
    }

    if (ctrl) sendCommandMessage(ctrl, WM_MOUSEMOVE, 0, 0, 0, pt);
}

void UIManager::onMouseLeave()
{
    if (m_event.m_mouseTracking)
    {
        m_event.m_mouseTracking = false;
        CPoint point;
        GetCursorPos(&point);
        m_dlg->ScreenToClient(&point);
        Point pt(point.x, point.y);
        auto ctrl = FindControl(pt);
        if (ctrl)
            sendCommandMessage(ctrl, WM_MOUSELEAVE, 0, 0, 0, pt);
        if (m_event.m_hover != NULL)
        {
            sendCommandMessage(m_event.m_hover, WM_MOUSELEAVE, 0, 0, 0, pt);
            m_event.m_hover = NULL;
        }
    }
}

void UIManager::onMouseHover(UINT nFlags, CPoint point)
{
    m_event.m_mouseTracking = FALSE;
    Point pt(point.x, point.y);
    auto ctrl = FindControl(pt);

    if (!ctrl) return;

    if (m_event.m_hover != NULL)
        sendCommandMessage(m_event.m_hover, WM_MOUSEHOVER, 0, 0, 0, pt);
}

void UIManager::onDestroy()
{
    CPoint point;
    GetCursorPos(&point);
    m_dlg->ScreenToClient(&point);
    Point pt(point.x, point.y);

    if (m_event.m_hover != NULL)
    {
        sendCommandMessage(m_event.m_hover, WM_MOUSELEAVE, 0, 0, 0, pt);
        m_event.m_hover = NULL;
    }

    if (m_event.m_click != NULL)
    {
        sendCommandMessage(m_event.m_click, WM_LBUTTONUP, 0, 0, 0, pt);
        m_event.m_click = NULL;
    }

    sendCommandMessage(m_layout.get(), WM_DESTROY, 0, 0, 0, Point(), true);
}

LRESULT UIManager::onNcHitTest(LRESULT nHitTest, CPoint point)
{
    if (nHitTest == HTCLIENT)
    {
        m_dlg->ScreenToClient(&point);
        Point pt(point.x, point.y);
        auto ctrl = FindControl(pt);
        if (ctrl)
        {
            LRESULT lr = sendCommandMessage(ctrl, WM_NCHITTEST, 0, 0, 0, pt);
            return lr == 0 ? HTCLIENT : lr;
        }
    }
    return HTCLIENT;
}

void UIManager::onMove(int x, int y)
{
    m_dlg->Invalidate();
}

void UIManager::onKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (m_event.m_focus == NULL)
        return;

    sendCommandMessage(m_event.m_focus, WM_KEYDOWN, nChar, nRepCnt, nFlags);
    m_event.m_key = m_event.m_focus;
}

void UIManager::onKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (m_event.m_key == NULL)
        return;

    sendCommandMessage(m_event.m_focus, WM_KEYUP, nChar, nRepCnt, nFlags);
    m_event.m_key = NULL;
}

void UIManager::onTimer(UINT_PTR nIDEvent)
{
    postCommandMessage(nIDEvent, WM_TIMER, GlobalTimerId);
}

LRESULT UIManager::onCommand(WPARAM wParam, LPARAM lParam)
{
    WORD wID = LOWORD(wParam);
    WORD wRedirect = HIWORD(wParam);

    auto ctrl = FindControl(wID, wRedirect);
    if (ctrl) return sendCommandMessage(ctrl, WM_COMMAND, wParam, lParam);

    TRACE("command message failed: Id[%d] Re[%d]\n", wID, wRedirect);
    return 0;
}

UINT_PTR UIManager::doWindow(_WindowAction action, UINT_PTR data /*= 0*/, UINT param /*= 0*/)
{
    switch (action)
    {
    case Minimize:
        return m_dlg->ShowWindow(SW_SHOWMINIMIZED);
    case Close:
        return m_dlg->PostMessage(WM_CLOSE);
    case SetTimer:
        return m_dlg->SetTimer(data, param, NULL);
    case KillTimer:
        return m_dlg->KillTimer(data);
    case Redraw:
        m_dlg->Invalidate(); return 0;
    case SetCursor:
        m_dlg->PostMessage(WM_SETCURSOR, (WPARAM)m_dlg->GetSafeHwnd(),
            (LPARAM)MAKELONG(WM_MOUSEMOVE, HTNOWHERE)); return 0;
    case SetFocus:
        m_dlg->PostMessage(WM_LBUTTONDOWN); m_dlg->PostMessage(WM_LBUTTONUP); m_dlg->SetFocus(); return 0;
    default:
        break;
    }
    return 0;
}
