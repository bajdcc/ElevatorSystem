#pragma once
#include "UI.h"
#include "UIcore.h"

class UIESCaption : public UIContainer
{
    DECLARE_DYNCREATE(UIESCaption)
public:
    UIESCaption();
    virtual ~UIESCaption();
    virtual void onCommand(DWORD id, UINT message);

    enum _Caption_ID
    {
        Root,
        Title,
        About,
        Minimize,
        Close
    };

protected:
    virtual void init(WORD wParam = 0);
};

class UIESAboutDialog : public UIDialog
{
    DECLARE_DYNCREATE(UIESAboutDialog)
public:
    UIESAboutDialog();
    virtual ~UIESAboutDialog();
    virtual void onCommand(DWORD id, UINT message);

    enum _AboutDialog_ID
    {
        Root,
        Title,
        Body,
        Close
    };

protected:
    virtual void init(WORD wParam = 0);
};

class UIESInsideButton : public UIButton
{
    DECLARE_DYNCREATE(UIESInsideButton)
public:
    UIESInsideButton();
    virtual ~UIESInsideButton();
    virtual LRESULT onEvent(UINT msg, WPARAM wp, LPARAM lp, UINT flag, Point pt);
    virtual void activate();

private:
    bool m_activated;
};

class UIESOutsideButton : public UIContainer
{
    DECLARE_DYNCREATE(UIESOutsideButton)
public:
    UIESOutsideButton();
    virtual ~UIESOutsideButton();
    virtual LRESULT onEvent(UINT msg, WPARAM wp, LPARAM lp, UINT flag, Point pt);

    enum _OutsideButton_ID
    {
        Root,
        Display,
        Up,
        Down,
    };

protected:
    virtual void init(WORD wParam = 0);
};

class UIESInsideButtonPanel : public UIDialog
{
    DECLARE_DYNCREATE(UIESInsideButtonPanel)
public:
    UIESInsideButtonPanel();
    virtual ~UIESInsideButtonPanel();
    virtual void onCommand(DWORD id, UINT message);

    enum _InsideButton_ID
    {
        Root,
        Display,
        Array
    };

    enum
    {
        Default_Width = UICFG_IS_W,
        Default_Height = UICFG_IS_H,
    };

protected:
    virtual void init(WORD wParam = 0);
};

class UIESOutsideButtonPanel : public UIDialog
{
    DECLARE_DYNCREATE(UIESOutsideButtonPanel)
public:
    UIESOutsideButtonPanel();
    virtual ~UIESOutsideButtonPanel();
    virtual void onCommand(DWORD id, UINT message);

    enum _OutsideButton_ID
    {
        Root,
        Display,
        Array
    };

    enum
    {
        Default_Width = UICFG_OS_W,
        Default_Height = UICFG_OS_H,
    };

protected:
    virtual void init(WORD wParam = 0);
};

class UIESLevelPanel : public UIDialog
{
    DECLARE_DYNCREATE(UIESLevelPanel)
public:
    UIESLevelPanel();
    virtual ~UIESLevelPanel();
    virtual void onCommand(DWORD id, UINT message);

    enum _Level_ID
    {
        Root,
        Array
    };

    enum
    {
        Default_Width = UICFG_LL_W,
        Default_Height = UICFG_LL_H,
    };

protected:
    virtual void init(WORD wParam = 0);
    void focus(CNullableObject<int>);
    void strong(int);
    void normal(int);
    std::shared_ptr<UILabel> member(int);
    bool valid(int) const;

    CNullableObject<int> focusLevel;
};

class UIESStatePanel : public UIDialog
{
    DECLARE_DYNCREATE(UIESStatePanel)
public:
    UIESStatePanel();
    virtual ~UIESStatePanel();
    virtual void onCommand(DWORD id, UINT message);

protected:
    virtual void init(WORD wParam = 0);
    void show(WORD);

    enum _State_ID
    {
        Root,
        Display
    };
};

class UIESControlDispatcher : public UIControl
{
    DECLARE_DYNCREATE(UIESControlDispatcher)
public:
    UIESControlDispatcher();
    virtual ~UIESControlDispatcher();
    virtual LRESULT onEvent(UINT msg, WPARAM wp, LPARAM lp, UINT flag, Point pt);
    virtual void onCommand(DWORD id, UINT message);

    enum
    {
        In_Up = WM_APP,
        In_Down,
        In_Internal,
        Out_Up,
        Out_Down,
        Out_Internal,
        Out_Level,
        Out_State
    };

protected:
    void onTimer();
    virtual void onCreate();
    virtual void onDestroy();
    virtual void initControls();

protected:
    enum { ControlCenterElapse = UICFG_X_ELAPSE };
};

class UIESControlCenter : public UIDialog
{
    DECLARE_DYNCREATE(UIESControlCenter)
public:
    UIESControlCenter();
    virtual ~UIESControlCenter();
    virtual void onCommand(DWORD id, UINT message);
    virtual void init(WORD wParam = 0);
    virtual LRESULT onEvent(UINT msg, WPARAM wp, LPARAM lp, UINT flag, Point pt);

    typedef UIESDataStruct::Data DataStructType;
    typedef UIESDataStruct::DataType DataType;

protected:
    void timer();
    void dispatch(bool levelChanged, bool stateChanged);
    void disable(WORD level);
    int reset();

    enum _ControlCenter_ID
    {
        Root,
        Data
    };

    UIESStateStruct stat;
};

class UIESDataCenter : public UIControl
{
    DECLARE_DYNCREATE(UIESDataCenter)
    typedef UIESDataStruct::_DataStructType DataStructType;
public:
    UIESDataCenter();
    virtual ~UIESDataCenter();
    virtual LRESULT onEvent(UINT msg, WPARAM wp, LPARAM lp, UINT flag, Point pt);

protected:
    UIESDataStruct data;
};

class UIESTimerCenter : public UIDialog
{
    DECLARE_DYNCREATE(UIESTimerCenter)
public:
    UIESTimerCenter();
    virtual ~UIESTimerCenter();
    virtual LRESULT onEvent(UINT msg, WPARAM wp, LPARAM lp, UINT flag, Point pt);

    enum _TimerCenter_ID
    {
        Root,
        Display,
        LTick,
        RTick
    };

protected:
    void onTimer();
    virtual void onCreate();
    virtual void onDestroy();
    virtual void init(WORD wParam = 0);
    enum { TimerCenterElapse = UICFG_NORMAL_TIMER_ELAPSE };
};