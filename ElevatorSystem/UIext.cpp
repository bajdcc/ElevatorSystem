#include "stdafx.h"
#include "resource.h"
#include "UIext.h"

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(UIESCaption, UIContainer)

UIESCaption::UIESCaption()
{
    this->setName(_T("标题栏布局"));
    this->setColor(Color::White);
    this->setHitTest(HTCAPTION);
}

UIESCaption::~UIESCaption()
{

}

void UIESCaption::init(WORD wParam /*= 0*/)
{
    CString caption; caption.LoadString(IDS_APP_TITLE);

    auto rt1(m_client);
    rt1.X += 15;
    rt1.Width = 500;

    auto sys_caption = m_manager->UICreateObject(UILabel);
    sys_caption->setName(_T("标题"));
    sys_caption->setRectEx(rt1);
    sys_caption->setFontColor(Color(104, 33, 122));
    sys_caption->setFontText(caption);
    sys_caption->setFontHorizenAlignment(StringAlignmentNear);
    sys_caption->setFontStyle(FontStyleBold);
    sys_caption->setTransparent(true);
    sys_caption->setId(m_id + Title);
    sys_caption->setDirectId(getDID());
    sys_caption->setHitTest(HTCAPTION);
    sys_caption->setAttr(STATE_NOID);
    addControl(sys_caption);

    auto rt2(m_client);
    rt2.X = rt2.X + rt2.Width - 40;
    rt2.Width = 40;
    rt2.Inflate(Point(-6, -9));
    rt2.X -= 70;

    auto sys_about = m_manager->UICreateObject(UIButton);
    sys_about->setName(_T("关于"));
    sys_about->setRectEx(rt2);
    sys_about->setFontText(_T("?"));
    sys_about->setFontFamily(_T("Times New Roman"));
    sys_about->setFontStyle(FontStyleBold);
    sys_about->setId(m_id + About);
    sys_about->setDirectId(getDID());
    addControl(sys_about);

    rt2.X += 35;

    auto sys_minimize = m_manager->UICreateObject(UIButton);
    sys_minimize->setName(_T("最小化"));
    sys_minimize->setRectEx(rt2);
    sys_minimize->setFontText(_T("__"));
    sys_minimize->setFontFamily(_T("Times New Roman"));
    sys_minimize->setFontStyle(FontStyleBold);
    sys_minimize->setId(m_id + Minimize);
    sys_minimize->setDirectId(getDID());
    addControl(sys_minimize);

    rt2.X += 35;

    auto sys_close = m_manager->UICreateObject(UIButton);
    sys_close->setName(_T("关闭"));
    sys_close->setRectEx(rt2);
    sys_close->setFontText(_T("X"));
    sys_close->setFontFamily(_T("Times New Roman"));
    sys_close->setFontStyle(FontStyleBold);
    sys_close->setId(m_id + Close);
    sys_close->setDirectId(getDID());
    addControl(sys_close);
}

void UIESCaption::onCommand(DWORD id, UINT message)
{
    WORD wID = LOWORD(id);
    WORD wRedirect = HIWORD(id);
    WORD wMsg = LOWORD(message);
    WORD wParam = HIWORD(message);

    if (wRedirect != UIDFT_CONTROL_DIRECT_ID)
    {
        switch (wMsg)
        {
        case WM_LBUTTONDOWN:
            switch (wID)
            {
            case UID_SYS_LAYOUT + About:
                if (true)
                {
                    CString body; body.LoadString(IDS_ABOUT);
                    m_manager->doDialog(_T("关于本程序"), body.GetBuffer());
                }
                break;
            case UID_SYS_LAYOUT + Minimize:
                m_manager->doWindow(UIManager::Minimize);
                break;
            case UID_SYS_LAYOUT + Close:
                m_manager->doWindow(UIManager::Close);
            default:
                break;
            }
            break;
        default:
            break;
        }
    }

    UIContainer::onCommand(id, message);
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(UIESAboutDialog, UIDialog)

UIESAboutDialog::UIESAboutDialog()
{
    this->setName(_T("对话框"));
    this->setHitTest(HTCAPTION);
    this->setDirectId(getDID());
}

UIESAboutDialog::~UIESAboutDialog()
{

}

void UIESAboutDialog::init(WORD wParam /*= 0*/)
{
    auto rt1(m_client);
    rt1.Inflate(-1, -1);
    rt1.Height = 50;

    auto title = m_manager->UICreateObject(UILabel);
    title->setName(_T("对话框标题"));
    title->setRectEx(rt1);
    title->setFontColor(Color::OrangeRed);
    title->setFontFamily(_T("楷体"));
    title->setFontStyle(FontStyleBold);
    title->setFontSize(16);
    title->setId(m_id + Title);
    title->setDirectId(getDID());
    title->setHitTest(HTCAPTION);
    addControl(title);

    Rect rt2(m_client);
    rt2.Height -= 40;
    rt2.Y += 40;
    rt2.Inflate(-30, -20);
    rt2.Height -= 40;

    auto body = m_manager->UICreateObject(UILabel);
    body->setName(_T("对话框正文"));
    body->setRectEx(rt2);
    body->setFontHorizenAlignment(StringAlignmentNear);
    body->setFontVerticalAlignment(StringAlignmentNear);
    body->setFontColor(Color::Purple);
    body->setId(m_id + Body);
    body->setDirectId(getDID());
    body->setHitTest(HTCAPTION);
    addControl(body);

    Rect rt3(m_client.GetRight() - 100, m_client.GetBottom() - 40, 100, 40);
    rt3.Inflate(Point(-6, -6));
    auto close = m_manager->UICreateObject(UIButton);
    close->setName(_T("对话框关闭"));
    close->setRectEx(rt3);
    close->setFontFamily(_T("Microsoft YaHei"));
    close->setFontText(_T("关闭"));
    close->setId(m_id + Close);
    close->setDirectId(getDID());
    addControl(close);
}

void UIESAboutDialog::onCommand(DWORD id, UINT message)
{
    WORD wID = LOWORD(id);
    WORD wRedirect = HIWORD(id);
    WORD wMsg = LOWORD(message);
    WORD wParam = HIWORD(message);

    if (wRedirect != UIDFT_CONTROL_DIRECT_ID)
    {
        switch (wMsg)
        {
        case WM_LBUTTONDOWN:
            switch (wID)
            {
            case UID_X_DIALOG + Close:
                m_manager->endDialog();
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }

    UIDialog::onCommand(id, message);
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(UIESInsideButton, UIButton)

UIESInsideButton::UIESInsideButton()
: m_activated(false)
{
    setFontSize(9);
    setAttr(STATE_NOID | STATE_SETFOCUS, STATE_HOTTRACK);
    m_buttonType = UITYPE_BUTTON_CIRCLE;
    m_button.m_focusedBkColor = UIDFT_BUTTON_PRESSED_BK_COLOR;
    m_button.m_focusedFontColor = UIDFT_BUTTON_PRESSED_FONT_COLOR;
}

UIESInsideButton::~UIESInsideButton()
{

}

LRESULT UIESInsideButton::onEvent(UINT msg, WPARAM wp, LPARAM lp, UINT flag, Point pt)
{
    switch (msg)
    {
    case WM_KILLFOCUS:
        if (m_activated)
            return 0;
        break;
    case UIMSG_Restore:
        m_activated = false;
        m_manager->sendMessage(this, WM_KILLFOCUS);
        return 0;
    case UIMSG_Disable:
        setAttr(0, STATE_ENABLED); invalidate();
        return 0;
    default:
        break;
    }

    return UIButton::onEvent(msg, wp, lp, flag, pt);
}

void UIESInsideButton::activate()
{
    if (!m_activated)
    {
        m_activated = true;
        UIButton::activate();
    }
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(UIESOutsideButton, UIContainer)

UIESOutsideButton::UIESOutsideButton()
{
    setAttr(STATE_NOID);
}

UIESOutsideButton::~UIESOutsideButton()
{

}

void UIESOutsideButton::init(WORD wParam /*= 0*/)
{
    int invalid_value = m_manager->sendMessage(UID_X_CONTROL_DISPATCHER, UIMSG_InvalidValue);
    int start = m_manager->sendMessage(UID_X_CONTROL_DISPATCHER, UIMSG_GetValue, 0, 0, UIESDataCenter::DataStructType::START);
    ASSERT(start != invalid_value);

    CString szName;
    int lv = (int)wParam;
    int lv2 = lv + start - 1;

    auto rt(m_client);
    rt.Width = 30;
    szName.Format(_T("电梯按钮外组#%d_显示"), lv);
    CString level; level.Format(_T("%d"), lv2);

    auto text = m_manager->UICreateObject(UILabel);
    text->setName(szName);
    text->setRectEx(rt);
    text->setFontText(level);
    text->setFontStyle(FontStyleBold);
    text->setFontHorizenAlignment(StringAlignmentFar);
    text->setId(m_id + Display);
    text->setDirectId(getDID());
    text->setAttr(STATE_NOID);
    addControl(text);

    auto rt1(m_client);
    auto width = (m_client.Width - rt.Width) / 2;
    rt1.X = rt.GetRight();
    rt1.Width = width;
    rt1.Inflate(-6, -6);
    szName.Format(_T("电梯按钮外组#%d_上"), lv);

    auto btn1 = m_manager->UICreateObject(UIESInsideButton);
    btn1->setName(szName);
    btn1->setRectEx(rt1);
    btn1->setFontText(_T("▲"));
    btn1->setId(m_id + Up);
    btn1->setData(lv);
    btn1->setDirectId(getDID());
    addControl(btn1);

    rt1.Offset(width, 0);
    szName.Format(_T("电梯按钮外组#%d_下"), lv);

    auto btn2 = m_manager->UICreateObject(UIESInsideButton);
    btn2->setName(szName);
    btn2->setRectEx(rt1);
    btn2->setFontText(_T("▼"));
    btn2->setId(m_id + Down);
    btn2->setData(lv);
    btn2->setDirectId(getDID());
    addControl(btn2);
}

LRESULT UIESOutsideButton::onEvent(UINT msg, WPARAM wp, LPARAM lp, UINT flag, Point pt)
{
    switch (msg)
    {
    case UIMSG_Disable:
        switch (flag)
        {
        case UIESControlDispatcher::Out_Up:
            m_manager->sendMessage(FindControlInternal(Up), msg); return 0;
        case UIESControlDispatcher::Out_Down:
            m_manager->sendMessage(FindControlInternal(Down), msg); return 0;
        }
    case UIMSG_Restore:
        m_manager->sendMessage(FindControlInternal(Up), UIMSG_Restore);
        m_manager->sendMessage(FindControlInternal(Down), UIMSG_Restore);
        return 0;
    }

    return UIContainer::onEvent(msg, wp, lp, flag, pt);
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(UIESInsideButtonPanel, UIDialog)

UIESInsideButtonPanel::UIESInsideButtonPanel()
{
    this->setName(_T("电梯按钮内组控制板"));
}

UIESInsideButtonPanel::~UIESInsideButtonPanel()
{

}

void UIESInsideButtonPanel::init(WORD wParam /*= 0*/)
{
    int cnt = (int)wParam + 1;
    m_client.Width = Default_Width;
    m_client.Height = Default_Height * ((cnt / 2) + 1);
    auto rt(m_client);
    rt.Height /= (cnt / 2) + 1;
    auto h = rt.Height;

    auto text = m_manager->UICreateObject(UILabel);
    text->setName(_T("电梯按钮内组控制板显示"));
    text->setRectEx(rt);
    text->setColor(Color::White);
    text->setFontText(_T("内层控制"));
    text->setFontStyle(FontStyleBold);
    text->setFontColor(Color::DarkSlateBlue);
    text->setId(m_id + Display);
    text->setAttr(STATE_NOID);
    addControl(text);
    rt.Offset(0, h);

    WORD wArray = Array;
    CString szName, lv;
    rt.Width /= 2;
    rt.X += rt.Width / 2;
    rt.Y += rt.Height / 2;
    rt.Width = rt.Height = 0;
    auto r = h / 2 - 2;
    rt.Inflate(r, r);
    auto rt1(rt);

    int invalid_value = m_manager->sendMessage(UID_X_CONTROL_DISPATCHER, UIMSG_InvalidValue);
    int start = m_manager->sendMessage(UID_X_CONTROL_DISPATCHER, UIMSG_GetValue, 0, 0, UIESDataCenter::DataStructType::START);
    ASSERT(start != invalid_value);

    for (int i = 1, j = start; i < cnt; i++, wArray++, j++)
    {
        szName.Format(_T("电梯按钮内组#%d"), i);
        lv.Format(_T("%d"), j);
        auto btn = m_manager->UICreateObject(UIESInsideButton);
        btn->setName(szName);
        btn->setRectEx(rt1);
        btn->setFontText(lv);
        btn->setFontStyle(FontStyleBold);
        btn->setId(m_id + wArray);
        btn->setData(i);
        btn->setDirectId(getDID());
        addControl(btn);
        rt1.Offset(0, h);
        if (i == cnt / 2)
        {
            rt1 = rt;
            rt1.Offset(m_client.Width / 2, 0);
        }
    }
}

void UIESInsideButtonPanel::onCommand(DWORD id, UINT message)
{
    WORD wID = LOWORD(id);
    WORD wRedirect = HIWORD(id);
    WORD wMsg = LOWORD(message);
    WORD wParam = HIWORD(message);

    switch (wMsg)
    {
    case WM_LBUTTONDOWN:
        m_manager->postMessage(this, UIESControlDispatcher::In_Internal, UID_X_CONTROL_DISPATCHER, wParam);
        break;
    case UIESControlDispatcher::Out_Internal:
        m_manager->postMessage(FindControlInternal(Array + wParam - 1), UIMSG_Restore);
        return;
    case UIMSG_Restore:
        m_manager->sendMessage(FindControlInternal(Array + wParam - 1), UIMSG_Restore);
        return;
    }

    UIDialog::onCommand(id, message);
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(UIESOutsideButtonPanel, UIDialog)

UIESOutsideButtonPanel::UIESOutsideButtonPanel()
{
    this->setName(_T("电梯按钮外组控制板"));
}

UIESOutsideButtonPanel::~UIESOutsideButtonPanel()
{

}

void UIESOutsideButtonPanel::init(WORD wParam /*= 0*/)
{
    int cnt = (int)wParam + 1;
    m_client.Width = Default_Width;
    m_client.Height = Default_Height * cnt;
    auto rt(m_client);
    rt.Height /= cnt;
    auto h = rt.Height;

    auto text = m_manager->UICreateObject(UILabel);
    text->setName(_T("电梯按钮外组控制板显示"));
    text->setRectEx(rt);
    text->setColor(Color::White);
    text->setFontText(_T("外层控制"));
    text->setFontStyle(FontStyleBold);
    text->setFontColor(Color::DarkSlateGray);
    text->setId(m_id + Display);
    text->setAttr(STATE_NOID);
    addControl(text);

    rt.Offset(0, h);
    WORD wArray = Array;
    CString szName;

    for (int i = 1; i < cnt; i++, wArray++)
    {
        szName.Format(_T("电梯按钮外组#%d"), i);
        auto btn = m_manager->UICreateObject(UIESOutsideButton);
        btn->setName(szName);
        btn->setId(m_id + wArray);
        btn->setData(i);
        btn->setRect(rt);
        btn->setDirectId(getDID());
        addControl(btn);
        m_manager->postMessage(this, UIMSG_Init, btn->getId(), i);
        rt.Offset(0, h);
    }
    m_manager->postMessage(this, UIMSG_Delay, 0, wParam);
}

void UIESOutsideButtonPanel::onCommand(DWORD id, UINT message)
{
    WORD wID = LOWORD(id);
    WORD wRedirect = HIWORD(id);
    WORD wMsg = LOWORD(message);
    WORD wParam = HIWORD(message);

    switch (wMsg)
    {
    case WM_LBUTTONDOWN:
        if (true)
        {
            WORD wMiss = wID - wParam - wRedirect - UIESOutsideButtonPanel::Array + 1;
            switch (wMiss)
            {
            case UIESOutsideButton::Up:
                m_manager->postMessage(this, UIESControlDispatcher::In_Up, UID_X_CONTROL_DISPATCHER, wParam);
                break;
            case UIESOutsideButton::Down:
                m_manager->postMessage(this, UIESControlDispatcher::In_Down, UID_X_CONTROL_DISPATCHER, wParam);
                break;
            }
        }
        break;
    case UIMSG_Delay:
        m_manager->sendMessage(FindControlInternal(Array + wParam - 1).get(), UIMSG_Disable, 0, 0, UIESControlDispatcher::Out_Up);
        m_manager->sendMessage(FindControlInternal(Array).get(), UIMSG_Disable, 0, 0, UIESControlDispatcher::Out_Down);
        return;
    case UIMSG_Restore:
        m_manager->sendMessage(FindControlInternal(Array + wParam - 1).get(), UIMSG_Restore);
        return;
    }

    UIDialog::onCommand(id, message);
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(UIESLevelPanel, UIDialog)

UIESLevelPanel::UIESLevelPanel()
{
    this->setName(_T("电梯楼层显示板"));
}

UIESLevelPanel::~UIESLevelPanel()
{

}

void UIESLevelPanel::init(WORD wParam /*= 0*/)
{
    int cnt = (int)wParam;
    m_client.Width = Default_Width * cnt;
    m_client.Height = Default_Height;
    auto rt(m_client);
    rt.Width = Default_Width;

    WORD wArray = Array;
    CString szName, lv;

    int invalid_value = m_manager->sendMessage(UID_X_CONTROL_DISPATCHER, UIMSG_InvalidValue);
    int start = m_manager->sendMessage(UID_X_CONTROL_DISPATCHER, UIMSG_GetValue, 0, 0, UIESDataCenter::DataStructType::START);
    ASSERT(start != invalid_value);

    for (int i = 1, j = start; i <= cnt; i++, wArray++, j++)
    {
        szName.Format(_T("电梯楼层组#%d"), i);
        lv.Format(_T("%d"), j);
        auto text = m_manager->UICreateObject(UILabel);
        text->setName(szName);
        text->setRectEx(rt);
        text->setFontText(lv);
        text->setFontStyle(FontStyleBold);
        text->setId(m_id + wArray);
        text->setData(i);
        text->setAttr(STATE_NOID);
        text->setDirectId(getDID());
        addControl(text);
        rt.Offset(Default_Width, 0);
    }
}

void UIESLevelPanel::onCommand(DWORD id, UINT message)
{
    WORD wID = LOWORD(id);
    WORD wRedirect = HIWORD(id);
    WORD wMsg = LOWORD(message);
    WORD wParam = HIWORD(message);

    switch (wMsg)
    {
    case UIESControlDispatcher::Out_Level:
        focus(CNullableObject<int>(wParam));
        break;
    case UIMSG_Reset:
        focus(CNullableObject<int>::Null());
        break;
    default:
        break;
    }

    UIDialog::onCommand(id, message);
}

void UIESLevelPanel::focus(CNullableObject<int> o)
{
    if (o && focusLevel && o.value() == focusLevel.value())
        return;
    if (o && valid(o.value()))
    {
        strong(o.value());
        if (focusLevel)
            normal(focusLevel.value());
        focusLevel = o;
    }
    else
        focusLevel.clear();
    invalidate();
}

void UIESLevelPanel::strong(int value)
{
    auto label = member(value); ASSERT(label);
    label->setFontColor(Color::Red);
    label->setFontSize(20);
}

void UIESLevelPanel::normal(int value)
{
    auto label = member(value); ASSERT(label);
    label->setFontColor(UILabel::defaultFontAttr().m_color);
    label->setFontSize(UILabel::defaultFontAttr().m_size);
}

std::shared_ptr<UILabel> UIESLevelPanel::member(int value)
{
    return std::dynamic_pointer_cast<UILabel>(FindControlInternal(value));
}

bool UIESLevelPanel::valid(int value) const
{
    return value > 0 && value <= m_data;
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(UIESStatePanel, UIDialog)

UIESStatePanel::UIESStatePanel()
{
    this->setName(_T("电梯状态显示板"));
}

UIESStatePanel::~UIESStatePanel()
{

}

void UIESStatePanel::init(WORD wParam /*= 0*/)
{
    auto rt(m_client); rt.Inflate(-3, -3);
    auto text = m_manager->UICreateObject(UILabel);
    text->setName(_T("电梯状态显示"));
    text->setRectEx(rt);
    text->setFontStyle(FontStyleBold);
    text->setId(m_id + Display);
    text->setAttr(STATE_NOID);
    text->setDirectId(getDID());
    addControl(text);
}

void UIESStatePanel::onCommand(DWORD id, UINT message)
{
    WORD wID = LOWORD(id);
    WORD wRedirect = HIWORD(id);
    WORD wMsg = LOWORD(message);
    WORD wParam = HIWORD(message);

    switch (wMsg)
    {
    case UIESControlDispatcher::Out_State:
        show(wParam);
        break;
    default:
        break;
    }

    UIDialog::onCommand(id, message);
}

void UIESStatePanel::show(WORD state)
{
    m_manager->sendMessage(FindControlInternal(Display), WM_SETTEXT, 0,
        m_manager->sendMessage(UID_X_CONTROL_DISPATCHER, WM_GETTEXT, 0, 0, state));
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(UIESControlDispatcher, UIControl)

UIESControlDispatcher::UIESControlDispatcher()
{
    setAttr(0, STATE_VISIBLE);
    setName(_T("电梯控制台消息派遣中心"));
}

UIESControlDispatcher::~UIESControlDispatcher()
{
}

void UIESControlDispatcher::onTimer()
{
    m_manager->postMessage(this, WM_TIMER, UID_X_CONTROL_CENTER);
}

void UIESControlDispatcher::onCreate()
{
    doTimer(ControlCenterElapse);
    initControls();
}

void UIESControlDispatcher::onDestroy()
{
    doTimer();
}

void UIESControlDispatcher::initControls()
{
    m_manager->postMessage(this, UIMSG_Init, UID_SYS_LAYOUT);
    m_manager->postMessage(this, UIMSG_Init, UID_X_DIALOG);
    m_manager->postMessage(this, UIMSG_Init, UID_X_CONTROL_CENTER);
    m_manager->postMessage(this, UIMSG_Init, m_id);
}

LRESULT UIESControlDispatcher::onEvent(UINT msg, WPARAM wp, LPARAM lp, UINT flag, Point pt)
{
    switch (msg)
    {
    case WM_TIMER:
        onTimer();
        break;
    case WM_GETTEXT:
        return m_manager->sendMessage(UID_X_CONTROL_CENTER, msg, wp, lp, flag, pt);
    case UIMSG_SetValue:
    case UIMSG_GetValue:
    case UIMSG_InvalidValue:
    case UIMSG_Reset:
        return m_manager->sendMessage(UID_X_CONTROL_CENTER, msg, wp, lp, flag, pt);
    default:
        break;
    }

    return UIControl::onEvent(msg, wp, lp, flag, pt);
}

void UIESControlDispatcher::onCommand(DWORD id, UINT message)
{
    WORD wID = LOWORD(id);
    WORD wRedirect = HIWORD(id);
    WORD wMsg = LOWORD(message);
    WORD wParam = HIWORD(message);

    switch (wMsg)
    {
    case UIMSG_Init:
        m_manager->doWindow(UIManager::Redraw);
        return;
    case UIMSG_Restore:
        m_manager->postMessage(this, UIMSG_Restore, UID_OUTSIDE_PANEL, wParam);
        m_manager->postMessage(this, UIMSG_Restore, UID_INSIDE_PANEL, wParam);
        m_manager->doWindow(UIManager::SetFocus);
        return;
    case In_Up:
        m_manager->postMessage(this, In_Up, UID_X_CONTROL_CENTER, wParam);
        return;
    case In_Down:
        m_manager->postMessage(this, In_Down, UID_X_CONTROL_CENTER, wParam);
        return;
    case In_Internal:
        m_manager->postMessage(this, In_Internal, UID_X_CONTROL_CENTER, wParam);
        return;
    case Out_Up:
        m_manager->postMessage(this, Out_Up, UID_OUTSIDE_PANEL, wParam);
        return;
    case Out_Down:
        m_manager->postMessage(this, Out_Down, UID_OUTSIDE_PANEL, wParam);
        return;
    case Out_Internal:
        m_manager->postMessage(this, Out_Internal, UID_INSIDE_PANEL, wParam);
        return;
    case Out_Level:
        m_manager->postMessage(this, Out_Level, UID_LEVEL_PANEL, wParam);
        return;
    case Out_State:
        m_manager->postMessage(this, Out_State, UID_STATE_PANEL, wParam);
        return;
    default:
        break;
    }
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(UIESControlCenter, UIDialog)

UIESControlCenter::UIESControlCenter()
{
    setAttr(0, STATE_VISIBLE);
    setName(_T("电梯控制台主机"));
}

UIESControlCenter::~UIESControlCenter()
{

}

void UIESControlCenter::onCommand(DWORD id, UINT message)
{
    WORD wID = LOWORD(id);
    WORD wRedirect = HIWORD(id);
    WORD wMsg = LOWORD(message);
    WORD wParam = HIWORD(message);

    switch (wMsg)
    {
    case WM_TIMER:
        timer();
        break;
    case UIESControlDispatcher::In_Up:
        stat.prefetch(UIESStateStruct::Up, wParam);
        break;
    case UIESControlDispatcher::In_Down:
        stat.prefetch(UIESStateStruct::Down, wParam);
        break;
    case UIESControlDispatcher::In_Internal:
        stat.prefetch(UIESStateStruct::Internal, wParam);
        break;
    default:
        break;
    }

    UIDialog::onCommand(id, message);
}

void UIESControlCenter::init(WORD wParam /*= 0*/)
{
    auto data = m_manager->UICreateObject(UIESDataCenter);
    data->setId(m_id + Data);
    addControl(data);

    m_manager->sendMessage(this, UIMSG_Reset);
    WORD level = (WORD)m_manager->sendMessage(this, UIMSG_Init);
    m_manager->postMessage(this, UIMSG_Init, UID_TIMER);
    m_manager->postMessage(this, UIMSG_Init, UID_LEVEL_PANEL, level);
    m_manager->postMessage(this, UIMSG_Init, UID_STATE_PANEL);
    m_manager->postMessage(this, UIMSG_Init, UID_OUTSIDE_PANEL, level);
    m_manager->postMessage(this, UIMSG_Init, UID_INSIDE_PANEL, level);
    m_manager->postMessage(this, UIMSG_Init, UID_X_CONTROL_DISPATCHER);
}

LRESULT UIESControlCenter::onEvent(UINT msg, WPARAM wp, LPARAM lp, UINT flag, Point pt)
{
    switch (msg)
    {
    case UIMSG_SetValue:
    case UIMSG_GetValue:
    case UIMSG_InvalidValue:
    case UIMSG_Reset:
        return m_manager->sendMessage(FindControlInternal(Data).get(), msg, wp, lp, flag, pt);
    case UIMSG_Init:
        return reset();
    case WM_GETTEXT:
        return (LRESULT)stat.getStateName((UIESStateStruct::State)flag);
    default:
        break;
    }

    return UIDialog::onEvent(msg, wp, lp, flag, pt);
}

void UIESControlCenter::timer()
{
    bool levelChanged = false, stateChanged = false;
    bool turn = stat.work(levelChanged, stateChanged);
    stat.disable(std::bind1st(std::mem_fun(&UIESControlCenter::disable), this));
    stat.handle();
    if (turn) dispatch(levelChanged, stateChanged);
}

void UIESControlCenter::dispatch(bool levelChanged, bool stateChanged)
{
    if (levelChanged) m_manager->postMessage(this, UIESControlDispatcher::Out_Level, UID_X_CONTROL_DISPATCHER, stat.getLevel());
    if (stateChanged) m_manager->postMessage(this, UIESControlDispatcher::Out_State, UID_X_CONTROL_DISPATCHER, stat.getState());
}

int UIESControlCenter::reset()
{
    auto data = FindControlInternal(Data); ASSERT(data);
    DataStructType ds;
    ds.CLK_PER_META_PROC = m_manager->sendMessage(data.get(), UIMSG_GetValue, 0, 0, UIESDataCenter::DataStructType::META_CLK);
    ds.NPLS_CLK_TRANSTION = m_manager->sendMessage(data.get(), UIMSG_GetValue, 0, 0, UIESDataCenter::DataStructType::NPLS);
    ds.WAIT_CLK = m_manager->sendMessage(data.get(), UIMSG_GetValue, 0, 0, UIESDataCenter::DataStructType::WAIT);
    ds.LEVELS = m_manager->sendMessage(data.get(), UIMSG_GetValue, 0, 0, UIESDataCenter::DataStructType::LEVEL);
    ds.STARTS = m_manager->sendMessage(data.get(), UIMSG_GetValue, 0, 0, UIESDataCenter::DataStructType::START);
    ds.ORIGINS = m_manager->sendMessage(data.get(), UIMSG_GetValue, 0, 0, UIESDataCenter::DataStructType::ORIGIN);
    stat.reset(ds);
    dispatch(true, true);
    return ds.LEVELS;
}

void UIESControlCenter::disable(WORD level)
{
    m_manager->postMessage(this, UIMSG_Restore, UID_X_CONTROL_DISPATCHER, level);
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(UIESDataCenter, UIControl)
UIESDataCenter::UIESDataCenter()
{
    setAttr(0, STATE_VISIBLE);
    setName(_T("电梯控制台数据中心"));
}

UIESDataCenter::~UIESDataCenter()
{

}

LRESULT UIESDataCenter::onEvent(UINT msg, WPARAM wp, LPARAM lp, UINT flag, Point pt)
{
    switch (msg)
    {
    case UIMSG_SetValue:
        return data.set((DataStructType)flag, lp);
    case UIMSG_GetValue:
        return data.get((DataStructType)flag);
    case UIMSG_InvalidValue:
        return data.INVALID_VALUE;
    case UIMSG_Reset:
        data.reset();
        return S_OK;
    default:
        break;
    }

    return UIControl::onEvent(msg, wp, lp, flag, pt);
}


//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(UIESTimerCenter, UIDialog)

UIESTimerCenter::UIESTimerCenter()
{
    setName(_T("实时时间"));
}

UIESTimerCenter::~UIESTimerCenter()
{

}

void UIESTimerCenter::onCreate()
{
    doTimer(TimerCenterElapse);
}

void UIESTimerCenter::onDestroy()
{
    doTimer();
}

void UIESTimerCenter::init(WORD wParam /*= 0*/)
{
    const int tick_d = 20;

    auto rt(m_client);
    rt.Inflate(-tick_d * 2, -4);

    auto text = m_manager->UICreateObject(UILabel);
    text->setName(_T("时间显示板"));
    text->setRectEx(rt);
    text->setFontStyle(FontStyleBold);
    text->setFontColor(Color::Red);
    text->setId(m_id + Display);
    text->setAttr(STATE_NOID);
    addControl(text);

    auto rt1(rt);
    rt1.X -= tick_d;
    rt1.Width = tick_d;

    auto ltext = m_manager->UICreateObject(UILabel);
    ltext->setName(_T("时间闪烁1"));
    ltext->setRectEx(rt1);
    ltext->setFontText(_T("●"));
    ltext->setFontStyle(FontStyleBold);
    ltext->setFontVerticalAlignment(StringAlignmentFar);
    ltext->setFontColor(Color::Red);
    ltext->setId(m_id + LTick);
    ltext->setAttr(STATE_NOID, STATE_VISIBLE);
    addControl(ltext);

    auto rt2(rt);
    rt2.X = rt2.GetRight();
    rt2.Width = tick_d;

    auto rtext = m_manager->UICreateObject(UILabel);
    rtext->setName(_T("时间闪烁2"));
    rtext->setRectEx(rt2);
    rtext->setFontText(_T("●"));
    rtext->setFontStyle(FontStyleBold);
    rtext->setFontVerticalAlignment(StringAlignmentFar);
    rtext->setFontColor(Color::Red);
    rtext->setId(m_id + RTick);
    rtext->setAttr(STATE_NOID, STATE_VISIBLE);
    addControl(rtext);
}

LRESULT UIESTimerCenter::onEvent(UINT msg, WPARAM wp, LPARAM lp, UINT flag, Point pt)
{
    switch (msg)
    {
    case WM_TIMER:
        onTimer();
        break;
    default:
        break;
    }

    return UIDialog::onEvent(msg, wp, lp, flag, pt);
}

void UIESTimerCenter::onTimer()
{
    CTime time = CTime::GetCurrentTime();
    CString display = time.Format(_T("%Y-%m-%d %H:%M:%S"));
    auto ctrl = FindControlInternal(Display); ASSERT(ctrl);
    m_manager->sendMessage(ctrl, WM_SETTEXT, (WPARAM)display.GetLength(), (LPARAM)display.GetBuffer());
    auto ctrl1 = FindControlInternal(LTick); ASSERT(ctrl1); ctrl1->blink();
    auto ctrl2 = FindControlInternal(RTick); ASSERT(ctrl2); ctrl2->blink();
}
