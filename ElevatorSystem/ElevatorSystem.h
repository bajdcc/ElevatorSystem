
// ccElevatorSystem.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CElevatorSystemApp: 
// �йش����ʵ�֣������ ccElevatorSystem.cpp
//

class CElevatorSystemApp : public CWinApp
{
public:
	CElevatorSystemApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
    virtual int ExitInstance();
};

extern CElevatorSystemApp theApp;