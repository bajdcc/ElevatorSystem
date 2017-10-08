
// ccElevatorSystemDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ElevatorSystem.h"
#include "ElevatorSystemDlg.h"
#include "afxdialogex.h"
#include <Gdiplus.h>


// CElevatorSystemDlg 对话框



CElevatorSystemDlg::CElevatorSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CElevatorSystemDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    globalSetDialog(this);
}

void CElevatorSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CElevatorSystemDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_SETCURSOR()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
    ON_WM_MOUSEHOVER()
    ON_WM_CLOSE()
    ON_WM_DESTROY()    
    ON_WM_NCHITTEST()
    ON_WM_NCLBUTTONDOWN()
    ON_WM_NCLBUTTONUP()
    ON_WM_NCMOUSELEAVE()
    ON_WM_NCMOUSEMOVE()
    ON_WM_NCMOUSEHOVER()
    ON_WM_MOVE()
    ON_WM_KEYDOWN()
    ON_WM_KEYUP()
    ON_WM_TIMER()
    ON_MESSAGE(WM_GUI_CMD, OnGuiCommand)
END_MESSAGE_MAP()


// CElevatorSystemDlg 消息处理程序

BOOL CElevatorSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
    globalDialogInit();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CElevatorSystemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
        CPaintDC dc(this);
        globalDialogOnPaint(dc.m_hDC, dc.m_ps.rcPaint);
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CElevatorSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CElevatorSystemDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
    globalDialogOnSize(nType, cx, cy);
}


BOOL CElevatorSystemDlg::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}


void CElevatorSystemDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    globalDialogOnLButtonDown(nFlags, point);
    CDialogEx::OnLButtonDown(nFlags, point);
}


void CElevatorSystemDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    globalDialogOnLButtonUp(nFlags, point);
    CDialogEx::OnLButtonUp(nFlags, point);
}


BOOL CElevatorSystemDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (globalDialogOnSetCursor(nHitTest, message))
        return true;
    return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}


void CElevatorSystemDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    globalDialogOnMouseMove(nFlags, point);
    CDialogEx::OnMouseMove(nFlags, point);
}


void CElevatorSystemDlg::OnMouseLeave()
{
    globalDialogOnMouseLeave();
    CDialogEx::OnMouseLeave();
}


void CElevatorSystemDlg::OnMouseHover(UINT nFlags, CPoint point)
{
    globalDialogOnMouseHover(nFlags, point);
    CDialogEx::OnMouseHover(nFlags, point);
}


void CElevatorSystemDlg::OnClose()
{
    CDialogEx::OnClose();
}


void CElevatorSystemDlg::OnDestroy()
{
    CDialogEx::OnDestroy();
    globalDialogOnDestroy();
}


LRESULT CElevatorSystemDlg::OnGuiCommand(WPARAM wParam, LPARAM lParam)
{
    return globalDialogOnCommand(wParam, lParam);
}


LRESULT CElevatorSystemDlg::OnNcHitTest(CPoint point)
{
    return globalDialogOnNcHitTest(CDialogEx::OnNcHitTest(point), point);
}


void CElevatorSystemDlg::OnMove(int x, int y)
{
    CDialogEx::OnMove(x, y);
    globalDialogOnMove(x, y);
}


void CElevatorSystemDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    globalDialogOnKeyDown(nChar, nRepCnt, nFlags);
    CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CElevatorSystemDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    globalDialogOnKeyUp(nChar, nRepCnt, nFlags);
    CDialogEx::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CElevatorSystemDlg::OnTimer(UINT_PTR nIDEvent)
{
    globalDialogOnTimer(nIDEvent);
    CDialogEx::OnTimer(nIDEvent);
}
