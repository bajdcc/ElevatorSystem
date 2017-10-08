#pragma once
#include "defaultsettings.h"
#include "api.h"
#include "id.h"
#include <afx.h>
#include <afxdialogex.h>
#include <Gdiplus.h>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <functional>

using namespace Gdiplus;

class UIManager;
class UIControl;
class UIContainer;

typedef std::shared_ptr<UIControl>(CALLBACK* UI_FIND_CALL)(std::shared_ptr<UIControl>, void*);
LPCTSTR debugGetMessageName(UINT message);

#define UICreateObject(Classname) createObject<Classname>(RUNTIME_CLASS(Classname))

class UIManager : public CObject
{
    DECLARE_DYNCREATE(UIManager)
public:
    UIManager();
    ~UIManager();

    void setDialog(CDialogEx* dlg) { m_dlg = dlg; }
    void init();
    bool registerControl(std::shared_ptr<UIControl> pCtrl);
    bool unregisterControl(std::shared_ptr<UIControl> pCtrl);
    std::shared_ptr<UIContainer> getLayout() const { return m_layout; }

    void onSize(UINT nType, int cx, int cy);
    void onPaint(HDC hDC, CRect rect);
    void onLButtonDown(UINT nFlags, CPoint point);
    void onLButtonUp(UINT nFlags, CPoint point);
    bool onSetCursor(UINT nHitTest, UINT message);
    void onMouseMove(UINT nFlags, CPoint point);
    void onMouseLeave();
    void onMouseHover(UINT nFlags, CPoint point);
    void onDestroy();
    LRESULT onCommand(WPARAM wParam, LPARAM lParam);
    LRESULT onNcHitTest(LRESULT nHitTest, CPoint point);
    void onMove(int x, int y);
    void onKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    void onKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
    void onTimer(UINT_PTR nIDEvent);
    void doTimer(WORD wId, UINT nElapse);

    static std::shared_ptr<UIControl> CALLBACK __FindControlFromPoint(std::shared_ptr<UIControl>, void*);
    static std::shared_ptr<UIControl> CALLBACK __FindControlFromId(std::shared_ptr<UIControl>, void*);
    std::shared_ptr<UIControl> FindControl(Point pt);
    std::shared_ptr<UIControl> FindControl(WORD id, WORD direct);

    void invalidate(UIControl*);
    void doDialogBkMask(Graphics& graphics);
    void doDialog(LPCTSTR pstrTitle, LPCTSTR pstrBody);
    void endDialog();

    void postMessage(UIControl*, UINT nCmd, WORD wDirect = UIDFT_CONTROL_DIRECT_ID, WORD wParam = 0);
    LRESULT sendMessage(UIControl*, UINT msg, WPARAM wp = 0, LPARAM lp = 0, UINT flag = 0, Point pt = Point(), bool recursion = false);
    LRESULT sendMessage(WORD, UINT msg, WPARAM wp = 0, LPARAM lp = 0, UINT flag = 0, Point pt = Point(), bool recursion = false);

    template<typename T>
    void postMessage(const std::shared_ptr<T>& ctrl, UINT nCmd, WORD wDirect = UIDFT_CONTROL_DIRECT_ID, WORD wParam = 0)
    {
        postMessage(ctrl.get(), nCmd, wDirect, wParam);
    }
    template<typename T>
    LRESULT sendMessage(const std::shared_ptr<T>& ctrl, UINT msg, WPARAM wp = 0, LPARAM lp = 0, UINT flag = 0, Point pt = Point(), bool recursion = false)
    {
        return sendMessage(ctrl.get(), msg, wp, lp, flag, pt, recursion);
    }

    template<typename T>
    std::shared_ptr<T> createObject(CRuntimeClass* pClass)
    {
        ASSERT(pClass->IsDerivedFrom(T::GetThisClass()));
        return std::shared_ptr<T>(static_cast<T*>(pClass->CreateObject()));
    }

    enum _WindowAction
    {
        None,
        Minimize,
        Close,
        SetTimer,
        KillTimer,
        Redraw,
        SetCursor,
        SetFocus
    };
    enum { GlobalTimerId = UID_X_TIMER };

    UINT_PTR doWindow(_WindowAction, UINT_PTR data = 0, UINT param = 0);

protected:
    template<typename T = UIControl> std::shared_ptr<T> getControl(WORD id);

    enum { CommandMessageId = WM_GUI_CMD };
    void postCommandMessage(UIControl*, UINT nCmd, WORD wDirect = UIDFT_CONTROL_DIRECT_ID, WORD wParam = 0);
    void postCommandMessage(WORD, UINT nCmd, WORD wDirect = UIDFT_CONTROL_DIRECT_ID, WORD wParam = 0);
    LRESULT sendCommandMessage(UIControl*, UINT msg, WPARAM wp = 0, LPARAM lp = 0, UINT flag = 0, Point pt = Point(), bool recursion = false);
    LRESULT sendCommandMessage(WORD, UINT msg, WPARAM wp = 0, LPARAM lp = 0, UINT flag = 0, Point pt = Point(), bool recursion = false);
    
    template<typename T>
    void postCommandMessage(const std::shared_ptr<T>& ctrl, UINT nCmd, WORD wDirect = UIDFT_CONTROL_DIRECT_ID, WORD wParam = 0)
    {
        postCommandMessage(ctrl.get(), nCmd, wDirect, wParam);
    }
    template<typename T>
    LRESULT sendCommandMessage(const std::shared_ptr<T>& ctrl, UINT msg, WPARAM wp = 0, LPARAM lp = 0, UINT flag = 0, Point pt = Point(), bool recursion = false)
    {
        return sendCommandMessage(ctrl.get(), msg, wp, lp, flag, pt, recursion);
    }

private:
    void unloadControlLink(UIControl*);

#ifdef _DEBUG
    bool filterMessage(UINT);
#endif // _DEBUG

    struct EventObject
    {
        UIControl*  m_click;
        UIControl*  m_focus;
        UIControl*  m_hover;
        UIControl*  m_key;
        bool m_mouseTracking;
        bool m_mouseCapture;
        bool m_dialog;

        EventObject() : m_click(NULL)
            , m_focus(NULL)
            , m_hover(NULL)
            , m_key(NULL)
            , m_mouseTracking(false)
            , m_mouseCapture(false)
            , m_dialog(false)
        {}
    };

    EventObject m_event;
    CDialogEx* m_dlg;
    CRect m_client;
    std::shared_ptr<UIContainer> m_layout;
    CDC m_bufferedDC;
    CBitmap m_bufferedBMP;
    CDC m_maskDC;
    CBitmap m_maskBMP;
    std::map<WORD, std::shared_ptr<CObject>> m_register;
};

class UIControl : public CObject, public std::enable_shared_from_this<UIControl>
{
    DECLARE_DYNCREATE(UIControl)
public:
    UIControl();
    virtual ~UIControl();

    virtual void setManager(UIManager* pManager) { m_manager = pManager; }
    virtual UIManager* getManager() const { return m_manager; }
    virtual void setParent(UIControl* pParent) { m_parent = pParent; }
    virtual UIControl* getParent() const { return m_parent; }
    virtual void setRect(Rect rect) { m_client = rect; }
    virtual Rect getRect() const { return m_client; }
    virtual void setColor(Color color) { m_color = color; }
    virtual Color getColor() const { return m_color; }
    virtual void setName(LPCTSTR name) { m_name = name; }
    virtual LPCTSTR getName() const { return m_name; }
    virtual void setId(WORD id) { m_id = id; }
    virtual WORD getId() const { return m_id; }
    virtual void setDirectId(WORD id) { m_directId = id; }
    virtual WORD getDirectId() const { return m_directId; }
    virtual WORD getDID() const { return m_directId == UIDFT_CONTROL_DIRECT_ID ? m_id : m_directId; }
    void setTransparent(bool transparent) { m_transparent = transparent; }
    bool getTransparent() const { return m_transparent; }
    void setHitTest(LRESULT hittest) { m_hittest = hittest; }
    LRESULT getHitTest() const { return m_hittest; }
    void setData(WORD data) { m_data = data; }
    WORD getData() const { return m_data; }

    virtual bool hasAttr(DWORD mask) const { return (m_state & mask) != 0; }
    virtual void setAttr(DWORD add, DWORD remove = 0) { m_state |= add; m_state &= ~remove; }

    virtual void onPaint(Graphics& graphics, Rect rect);
    virtual void onPaintBkg(Graphics& graphics, Rect rect);
    virtual LRESULT onEvent(UINT msg, WPARAM wp, LPARAM lp, UINT flag, Point pt);
    virtual void onCommand(DWORD id, UINT message);
    virtual void onRecursion(UINT message);
    virtual void onCreate();
    virtual void onDestroy();

    void doTimer(UINT elapse = 0);
    virtual std::shared_ptr<UIControl> FindControl(UI_FIND_CALL proc, void* data, UINT flags);

    void invalidate();
    Color getTrueColor(Color) const;

    virtual void* getHashCode() const { return (void*)m_id; }
    void blink();

protected:
    bool isReadyForPaint(Rect& rect);

protected:
    UIControl* m_parent;
    UIManager* m_manager;
    Rect m_client;
    Color m_color;
    CString m_name;
    WORD m_id;
    WORD m_directId;
    DWORD m_state;
    bool m_transparent;
    LRESULT m_hittest;
    WORD m_data;
};

class UITimer : public UIControl
{
    DECLARE_DYNCREATE(UITimer)
public:
    UITimer();
    virtual ~UITimer();
    virtual void onCommand(DWORD id, UINT message);
    void onTimer(WORD id, WORD elapse);
    void onDispatch(WORD id);

protected:
    std::map<WORD, std::tuple<UINT_PTR, UINT>> m_timerId;
};

class UIContainer : public UIControl
{
    DECLARE_DYNCREATE(UIContainer)
public:
    UIContainer();
    virtual ~UIContainer();

    virtual void setManager(UIManager* pManager);

    bool addControl(std::shared_ptr<UIControl> ctrl);
    bool removeControl(std::shared_ptr<UIControl> ctrl);

    virtual void onPaint(Graphics& graphics, Rect rect);
    virtual void onPaintChildren(Graphics& graphics, Rect rect);

    virtual std::shared_ptr<UIControl> FindControl(UI_FIND_CALL proc, void* data, UINT flags);
    virtual std::shared_ptr<UIControl> FindControlById(WORD id);
    virtual std::shared_ptr<UIControl> FindControlLv1(WORD id);
    virtual void onRecursion(UINT message);
    virtual void onCommand(DWORD id, UINT message);

protected:
    virtual void init(WORD wParam = 0);
    virtual void destroy();
    virtual std::shared_ptr<UIControl> FindControlInternal(WORD id);

private:
    std::set<std::shared_ptr<UIControl>> m_ctrls;
};

class UIDialog : public UIContainer
{
    DECLARE_DYNCREATE(UIDialog)
public:
    UIDialog();
    virtual ~UIDialog();

    virtual void onPaint(Graphics& graphics, Rect rect);
    virtual void onPaintFrame(Graphics& graphics, Rect rect);

protected:
    Color m_frameColor;
    REAL m_frameWidth;
};

class UILabel : public UIControl
{
    DECLARE_DYNCREATE(UILabel)
public:
    UILabel();
    virtual ~UILabel();

    void setFontText(LPCTSTR text) { m_font.m_text = text; }
    LPCTSTR getFontText() const { return m_font.m_text; }
    int getFontLength() const { return m_font.m_text.GetLength(); }
    void setFontFamily(LPCTSTR family) { m_font.m_family = family; }
    LPCTSTR getFontFamily() const { return m_font.m_family; }
    void setFontColor(Color color) { m_font.m_color = color; }
    Color getFontColor() const { return m_font.m_color; }
    void setFontRect(RectF rect) { m_font.m_rect = rect; }
    RectF getFontRect() const { return m_font.m_rect; }
    void setFontSize(REAL size) { m_font.m_size = size; }
    REAL getFontSize() const { return m_font.m_size; }
    void setFontUnit(Unit unit) { m_font.m_unit = unit; }
    Unit setFontUnit() const { return m_font.m_unit; }
    void setFontStyle(FontStyle style) { m_font.m_style = style; }
    FontStyle getFontStyle() const { return m_font.m_style; }
    void setFontFormat(INT format) { m_font.m_format = format; }
    INT getFontFormat() const { return m_font.m_format; }
    void setFontHorizenAlignment(StringAlignment alignment) { m_font.m_horizen = alignment; }
    StringAlignment getFontHorizenAlignment() const { return m_font.m_horizen; }
    void setFontVerticalAlignment(StringAlignment alignment) { m_font.m_vertical = alignment; }
    StringAlignment getFontVerticalAlignment() const { return m_font.m_vertical; }
    void setFontTrimming(StringTrimming trimming) { m_font.m_trimming = trimming; }
    StringTrimming getFontTrimming() const { return m_font.m_trimming; }

    virtual void setRectEx(Rect rect);
    virtual void onPaint(Graphics& graphics, Rect rect);
    virtual void onPaintText(Graphics& graphics, Rect rect);
    virtual LRESULT onEvent(UINT msg, WPARAM wp, LPARAM lp, UINT flag, Point pt);

protected:
    struct FontAttr
    {
        CString m_text;
        CString m_family;
        Color m_color;
        RectF m_rect;
        REAL m_size;
        Unit m_unit;
        FontStyle m_style;
        INT m_format;
        StringAlignment m_horizen;
        StringAlignment m_vertical;
        StringTrimming m_trimming;

        FontAttr() : m_family(UIDFT_FONT_FAMILY)
            , m_size(UIDFT_FONT_SIZE)
            , m_color(UIDFT_FONT_COLOR)
            , m_unit(UIDFT_FONT_UNIT)
            , m_style(UIDFT_FONT_STYLE)
            , m_format(UIDFT_FONT_FORMAT)
            , m_horizen(UIDFT_FONT_H_ALIGN)
            , m_vertical(UIDFT_FONT_V_ALIGN)
            , m_trimming(UIDFT_FONT_TRIMMING)
        {
        }
    };

    FontAttr m_font;

public:
    static const FontAttr& defaultFontAttr() { static FontAttr attr; return attr; }
};

class UIButton : public UILabel
{
    DECLARE_DYNCREATE(UIButton)
public:
    UIButton();
    virtual ~UIButton();

    enum ButtonColorFlags
    {
        NORMAL_FONT = 1,
        HOT_FONT,
        FOCUSED_FONT,
        PRESSED_FONT,
        NORMAL_BK,
        HOT_BK,
        FOCUSED_BK,
        PRESSED_BK,
        FRAME,
        NORMAL_FRAME,
        HOT_FRAME,
        FOCUSED_FRAME,
        PRESSED_FRAME,
    };

    void setButtonType(UINT type) { m_buttonType = type; }
    UINT getButtonType() const { return m_buttonType; }
    void setButtonColor(ButtonColorFlags flags, Color color);
    Color getButtonColor(ButtonColorFlags flags) const;
    void setButtonFrameWidth(REAL width) { m_button.m_frameWidth = width; }
    REAL getButtonFrameWidth() const { return m_button.m_frameWidth; }

    virtual void onPaint(Graphics& graphics, Rect rect);
    virtual void onPaintBkg(Graphics& graphics, Rect rect);
    virtual void onPaintFrame(Graphics& graphics, Rect rect);

protected:
    void internalCheckState();

    virtual void activate();
    virtual LRESULT onEvent(UINT msg, WPARAM wp, LPARAM lp, UINT flag, Point pt);

protected:
    struct ButtonAttr
    {
        Color m_normalFontColor;
        Color m_hotFontColor;
        Color m_focusedFontColor;
        Color m_pressedFontColor;
        Color m_normalBkColor;
        Color m_hotBkColor;
        Color m_focusedBkColor;
        Color m_pressedBkColor;
        Color m_frameColor;
        Color m_normalFrameColor;
        Color m_hotFrameColor;
        Color m_focusedFrameColor;
        Color m_pressedFrameColor;

        REAL m_frameWidth;

        ButtonAttr() : m_normalFontColor(UIDFT_BUTTON_NORMAL_FONT_COLOR)
            , m_hotFontColor(UIDFT_BUTTON_HOT_FONT_COLOR)
            , m_focusedFontColor(UIDFT_BUTTON_FOCUSED_FONT_COLOR)
            , m_pressedFontColor(UIDFT_BUTTON_PRESSED_FONT_COLOR)
            , m_normalBkColor(UIDFT_BUTTON_NORMAL_BK_COLOR)
            , m_hotBkColor(UIDFT_BUTTON_HOT_BK_COLOR)
            , m_focusedBkColor(UIDFT_BUTTON_FOCUSED_BK_COLOR)
            , m_pressedBkColor(UIDFT_BUTTON_PRESSED_BK_COLOR)
            , m_frameColor(UIDFT_BUTTON_NORMAL_FRAME_COLOR)
            , m_normalFrameColor(UIDFT_BUTTON_NORMAL_FRAME_COLOR)
            , m_hotFrameColor(UIDFT_BUTTON_HOT_FRAME_COLOR)
            , m_focusedFrameColor(UIDFT_BUTTON_FOCUSED_FRAME_COLOR)
            , m_pressedFrameColor(UIDFT_BUTTON_PRESSED_FRAME_COLOR)
            , m_frameWidth(UIDFT_BUTTON_FRAME_WIDTH)
        {
        }
    };

    ButtonAttr m_button;
    UINT m_buttonType;

public:
    static const ButtonAttr& defaultButtonAttr() { static ButtonAttr attr; return attr; }
};

