#include "stdafx.h"
#include "UI.h"

IMPLEMENT_DYNCREATE(UIControl, CObject)

UIControl::UIControl()
: m_manager(nullptr)
, m_parent(nullptr)
, m_state(UIDFT_CONTROL_STATE)
, m_name(UIDFT_CONTROL_NAME)
, m_id(UIDFT_CONTROL_ID)
, m_directId(UIDFT_CONTROL_DIRECT_ID)
, m_transparent(UIDFT_CONTROL_TRANSPARENT)
, m_color(UIDFT_CONTROL_COLOR)
, m_hittest(UIDFT_CONTROL_HITTEST)
{
    
}

UIControl::~UIControl()
{

}

bool UIControl::isReadyForPaint(Rect& rect)
{
    return rect.Intersect(m_client) == TRUE && hasAttr(STATE_VISIBLE);
}

void UIControl::doTimer(UINT elapse /*= 0*/)
{
    ASSERT(m_manager);
    m_manager->doTimer(m_id, elapse);
}

void UIControl::onPaint(Graphics& graphics, Rect rect)
{
    if (!isReadyForPaint(rect)) return;
    if (m_transparent) return;

    onPaintBkg(graphics, rect);
}

void UIControl::onPaintBkg(Graphics& graphics, Rect rect)
{
    SolidBrush brush(m_color);
    graphics.FillRectangle(&brush, rect);
}

std::shared_ptr<UIControl> UIControl::FindControl(UI_FIND_CALL proc, void* data, UINT flags)
{
    if ((flags & UIFIND_VISIBLE) != 0 && !hasAttr(STATE_VISIBLE)) return nullptr;
    if ((flags & UIFIND_ENABLED) != 0 && !hasAttr(STATE_ENABLED)) return nullptr;
    if ((flags & UIFIND_HITTEST) != 0) return proc(shared_from_this(), data);
    return nullptr;
}

LRESULT UIControl::onEvent(UINT msg, WPARAM wp, LPARAM lp, UINT flag, Point pt)
{
    switch (msg)
    {
    case WM_NCHITTEST:
        return m_hittest;
    case WM_COMMAND:
        onCommand(wp, lp);
        return 0;
    default:
        break;
    }
    return 0;
}

void UIControl::onCommand(DWORD id, UINT message)
{
    if (m_directId != UIDFT_CONTROL_DIRECT_ID)
    {
        m_manager->postMessage(this, message);
    }
}

void UIControl::onRecursion(UINT message)
{
    switch (message)
    {
    case WM_CREATE:
        onCreate();
        break;
    case WM_DESTROY:
        onDestroy();
        break;
    default:
        break;
    }
}

void UIControl::onCreate()
{

}

void UIControl::onDestroy()
{

}

void UIControl::invalidate()
{
    m_manager->invalidate(this);
}

Color UIControl::getTrueColor(Color color) const
{
    return hasAttr(STATE_ENABLED) ? color : Color(128, color.GetR(), color.GetG(), color.GetB());
}

void UIControl::blink()
{
    m_state ^= STATE_VISIBLE;
    invalidate();
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(UILabel, UIControl)

UILabel::UILabel()
{

}

UILabel::~UILabel()
{

}

void UILabel::setRectEx(Rect rect)
{
    UIControl::setRect(rect);
    m_font.m_rect.X = (REAL)rect.X;
    m_font.m_rect.Y = (REAL)rect.Y;
    m_font.m_rect.Width = (REAL)rect.Width;
    m_font.m_rect.Height = (REAL)rect.Height;
}

void UILabel::onPaint(Graphics& graphics, Rect rect)
{
    if (!isReadyForPaint(rect)) return;

    UIControl::onPaint(graphics, rect);
    onPaintText(graphics, rect);
}

void UILabel::onPaintText(Graphics& graphics, Rect rect)
{
    StringFormat stringFormat;
    stringFormat.SetTrimming(m_font.m_trimming);
    stringFormat.SetAlignment(m_font.m_horizen);
    stringFormat.SetLineAlignment(m_font.m_vertical);

    Gdiplus::Font font(m_font.m_family, m_font.m_size, m_font.m_style, m_font.m_unit);
    SolidBrush brush(getTrueColor(m_font.m_color));
    graphics.DrawString(m_font.m_text, -1, &font, m_font.m_rect, &stringFormat, &brush);
}

LRESULT UILabel::onEvent(UINT msg, WPARAM wp, LPARAM lp, UINT flag, Point pt)
{
    switch (msg)
    {
    case WM_SETTEXT:
        setFontText((LPCTSTR)lp);
        invalidate();
        return getFontLength();
    case WM_GETTEXT:
        ASSERT(lp);
        *(CString*)(lp) = getFontText();
        return getFontLength();
    case WM_GETTEXTLENGTH:
        return getFontLength();
    }

    return UIControl::onEvent(msg, wp, lp, flag, pt);
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(UIButton, UILabel)

UIButton::UIButton()
: m_buttonType(UIDFT_BUTTON_TYPE)
{
    m_color = UIDFT_BUTTON_COLOR;
    setAttr(STATE_CURSOR | STATE_HOTTRACK);
}

UIButton::~UIButton()
{

}

void UIButton::setButtonColor(ButtonColorFlags flags, Color color)
{
    switch (flags)
    {
    case NORMAL_FONT:
        m_button.m_normalFontColor = color; break;
    case HOT_FONT:
        m_button.m_hotFontColor = color; break;
    case FOCUSED_FONT:
        m_button.m_focusedFontColor = color; break;
    case PRESSED_FONT:
        m_button.m_pressedFontColor = color; break;
    case NORMAL_BK:
        m_button.m_normalBkColor = color; break;
    case HOT_BK:
        m_button.m_hotBkColor = color; break;
    case FOCUSED_BK:
        m_button.m_focusedBkColor = color; break;
    case PRESSED_BK:
        m_button.m_pressedBkColor = color; break;
    case FRAME:
        m_button.m_frameColor = color; break;
    case NORMAL_FRAME:
        m_button.m_normalFrameColor = color; break;
    case HOT_FRAME:
        m_button.m_hotFrameColor = color; break;
    case FOCUSED_FRAME:
        m_button.m_focusedFrameColor = color; break;
    case PRESSED_FRAME:
        m_button.m_pressedFrameColor = color; break;
    }
}

Color UIButton::getButtonColor(ButtonColorFlags flags) const
{
    switch (flags)
    {
    case NORMAL_FONT:
        return m_button.m_normalFontColor;
    case HOT_FONT:
        return m_button.m_hotFontColor;
    case FOCUSED_FONT:
        return m_button.m_focusedFontColor;
    case PRESSED_FONT:
        return m_button.m_pressedFontColor;
    case NORMAL_BK:
        return m_button.m_normalBkColor;
    case HOT_BK:
        return m_button.m_hotBkColor;
    case FOCUSED_BK:
        return m_button.m_focusedBkColor;
    case PRESSED_BK:
        return m_button.m_pressedBkColor;
    case FRAME:
        return m_button.m_frameColor;
    case NORMAL_FRAME:
        return m_button.m_normalFrameColor;
    case HOT_FRAME:
        return m_button.m_hotFrameColor;
    case FOCUSED_FRAME:
        return m_button.m_focusedFrameColor;
    case PRESSED_FRAME:
        return m_button.m_pressedFrameColor;
    }
    return Color();
}

void UIButton::internalCheckState()
{
    if (hasAttr(STATE_PRESSED))
    {
        m_color = m_button.m_pressedBkColor;
        m_font.m_color = m_button.m_pressedFontColor;
        m_button.m_frameColor = m_button.m_pressedFrameColor;
    }
    else if (hasAttr(STATE_HOT))
    {
        m_color = m_button.m_hotBkColor;
        m_font.m_color = m_button.m_hotFontColor;
        m_button.m_frameColor = m_button.m_hotFrameColor;
    }
    else if (hasAttr(STATE_FOCUSED))
    {
        m_color = m_button.m_focusedBkColor;
        m_font.m_color = m_button.m_focusedFontColor;
        m_button.m_frameColor = m_button.m_focusedFrameColor;
    }
    else
    {
        m_color = getTrueColor(m_button.m_normalBkColor);
        m_font.m_color = getTrueColor(m_button.m_normalFontColor);
        m_button.m_frameColor = getTrueColor(m_button.m_normalFrameColor);
    }
}

void UIButton::onPaint(Graphics& graphics, Rect rect)
{
    if (!isReadyForPaint(rect)) return;

    internalCheckState();
    onPaintBkg(graphics, rect);
    onPaintFrame(graphics, rect);
    UILabel::onPaintText(graphics, rect);
}

void UIButton::onPaintFrame(Graphics& graphics, Rect rect)
{
    Pen pen(m_button.m_frameColor, m_button.m_frameWidth);
    switch (m_buttonType)
    {
    case UITYPE_BUTTON_RECTANGLE:
        graphics.DrawRectangle(&pen, rect);
        break;
    case UITYPE_BUTTON_CIRCLE:
        graphics.DrawEllipse(&pen, rect);
        break;
    default:
        break;
    }
}

void UIButton::onPaintBkg(Graphics& graphics, Rect rect)
{
    SolidBrush brush(m_color);
    switch (m_buttonType)
    {
    case UITYPE_BUTTON_RECTANGLE:
        graphics.FillRectangle(&brush, rect);
        break;
    case UITYPE_BUTTON_CIRCLE:
        graphics.FillEllipse(&brush, rect);
        break;
    default:
        break;
    }
}

void UIButton::activate()
{
    m_manager->postMessage(this, WM_LBUTTONDOWN, m_directId, m_data);
}

LRESULT UIButton::onEvent(UINT msg, WPARAM wp, LPARAM lp, UINT flag, Point pt)
{
    switch (msg)
    {
    case WM_SETCURSOR:
        SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
        return 0;
    case WM_SETFOCUS:
        if (hasAttr(STATE_ENABLED) && hasAttr(STATE_SETFOCUS))
        {
            setAttr(STATE_FOCUSED); invalidate();
        }
        break;
    case WM_KILLFOCUS:
        if (hasAttr(STATE_ENABLED) && hasAttr(STATE_SETFOCUS))
        {
            setAttr(0, STATE_FOCUSED); invalidate();
        }
        break;
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
        if (hasAttr(STATE_ENABLED))
        {
            setAttr(STATE_PRESSED | STATE_CAPTURED); invalidate();
            if (!hasAttr(STATE_NOACTIVATE))
                activate();
        }
        break;
    case WM_LBUTTONUP:
        if (hasAttr(STATE_ENABLED) && hasAttr(STATE_CAPTURED))
        {
            setAttr(0, STATE_PRESSED | STATE_CAPTURED); invalidate();
        }
        break;
    case WM_MOUSEENTER:
        if (hasAttr(STATE_ENABLED) && hasAttr(STATE_HOTTRACK))
        {
            setAttr(STATE_HOT); invalidate();
        }
        break;
    case WM_MOUSELEAVE:
        if (hasAttr(STATE_ENABLED) && hasAttr(STATE_HOTTRACK))
        {
            setAttr(0, STATE_HOT | STATE_PRESSED); invalidate();
        }
        break;
    default:
        break;
    }

    return UILabel::onEvent(msg, wp, lp, flag, pt);
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(UITimer, UIControl)

UITimer::UITimer()
{
    setAttr(0, STATE_VISIBLE);
    setName(_T("全局定时器"));
}

UITimer::~UITimer()
{

}

void UITimer::onCommand(DWORD id, UINT message)
{
    WORD wId = LOWORD(id);
    WORD wRedirect = HIWORD(id);
    WORD wMsg = LOWORD(message);
    WORD wParam = HIWORD(message);

    switch (wMsg)
    {
    case WM_TIMER:
        onDispatch(wId);
        break;
    case WM_TIMECHANGE:
        onTimer(wId, wParam);
        break;
    default:
        break;
    }
}

void UITimer::onTimer(WORD id, WORD elapse)
{
    auto it = m_timerId.find(id);
    if (m_timerId.empty() || it == m_timerId.end())
    {
        auto timer = m_manager->doWindow(UIManager::SetTimer, id, elapse);
        m_timerId.insert(std::map<WORD, std::tuple<UINT_PTR, UINT>>::value_type
            (id, std::tuple<UINT_PTR, UINT>(timer, elapse)));
    }
    else
    {
        m_timerId.erase(it);
        m_manager->doWindow(UIManager::KillTimer, std::get<0>(it->second));
    }
}

void UITimer::onDispatch(WORD id)
{
    auto it = m_timerId.find(id);
    if (!m_timerId.empty() && it != m_timerId.end())
        m_manager->sendMessage(id, WM_TIMER, m_id, ::GetTickCount(), std::get<1>(it->second));
}
