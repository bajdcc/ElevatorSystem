#pragma once
#include <afx.h>
#include <afxdialogex.h>

#define WM_GUI_CMD      ( WM_USER + 0x10 )

void globalSetDialog(CDialogEx* pDlg);
void globalStartupGdiPlus();
void globalShutdownGdiPlus();
void globalDialogInit();
void globalDialogOnSize(UINT nType, int cx, int cy);
void globalDialogOnPaint(HDC hDC, RECT rect);
void globalDialogOnLButtonDown(UINT nFlags, POINT point);
void globalDialogOnLButtonUp(UINT nFlags, POINT point);
bool globalDialogOnSetCursor(UINT nHitTest, UINT message);
void globalDialogOnMouseMove(UINT nFlags, POINT point);
void globalDialogOnMouseLeave();
void globalDialogOnMouseHover(UINT nFlags, POINT point);
void globalDialogOnDestroy();
LRESULT globalDialogOnCommand(WPARAM wParam, LPARAM lParam);
LRESULT globalDialogOnNcHitTest(LRESULT nHitTest, POINT point);
void globalDialogOnMove(int x, int y);
void globalDialogOnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
void globalDialogOnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
void globalDialogOnTimer(UINT_PTR nIDEvent);

