#include "stdafx.h"
#include "UI.h"

using namespace Gdiplus;

UIManager& globalGetManagerInstance(void)
{
    static UIManager layout;
    return layout;
}

ULONG_PTR gdiplus_token;

void globalSetDialog(CDialogEx* pDlg)
{
    ASSERT(pDlg);
    globalGetManagerInstance().setDialog(pDlg);
}

void globalStartupGdiPlus()
{
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplus_token, &gdiplusStartupInput, NULL);
}

void globalShutdownGdiPlus()
{
    GdiplusShutdown(gdiplus_token);
}

void globalDialogInit()
{
    globalGetManagerInstance().init();
}

void globalDialogOnSize(UINT nType, int cx, int cy)
{
    globalGetManagerInstance().onSize(nType, cx, cy);
}

void globalDialogOnPaint(HDC hDC, RECT rect)
{
    globalGetManagerInstance().onPaint(hDC, rect);
}

void globalDialogOnLButtonDown(UINT nFlags, POINT point)
{
    globalGetManagerInstance().onLButtonDown(nFlags, point);
}

void globalDialogOnLButtonUp(UINT nFlags, POINT point)
{
    globalGetManagerInstance().onLButtonUp(nFlags, point);
}

bool globalDialogOnSetCursor(UINT nHitTest, UINT message)
{
    return globalGetManagerInstance().onSetCursor(nHitTest, message);
}

void globalDialogOnMouseMove(UINT nFlags, POINT point)
{
    globalGetManagerInstance().onMouseMove(nFlags, point);
}

void globalDialogOnMouseLeave()
{
    globalGetManagerInstance().onMouseLeave();
}

void globalDialogOnMouseHover(UINT nFlags, POINT point)
{
    globalGetManagerInstance().onMouseHover(nFlags, point);
}

void globalDialogOnDestroy()
{
    globalGetManagerInstance().onDestroy();
}

LRESULT globalDialogOnCommand(WPARAM wParam, LPARAM lParam)
{
    return globalGetManagerInstance().onCommand(wParam, lParam);
}

LRESULT globalDialogOnNcHitTest(LRESULT nHitTest, POINT point)
{
    return globalGetManagerInstance().onNcHitTest(nHitTest, point);
}

void globalDialogOnMove(int x, int y)
{
    return globalGetManagerInstance().onMove(x, y);
}

void globalDialogOnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    return globalGetManagerInstance().onKeyDown(nChar, nRepCnt, nFlags);
}

void globalDialogOnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    return globalGetManagerInstance().onKeyUp(nChar, nRepCnt, nFlags);
}

void globalDialogOnTimer(UINT_PTR nIDEvent)
{
    return globalGetManagerInstance().onTimer(nIDEvent);
}
