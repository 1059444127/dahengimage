
// ThreeTierRightsDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CThreeTierRightsDemoApp: 
// �йش����ʵ�֣������ ThreeTierRightsDemo.cpp
//

class CThreeTierRightsDemoApp : public CWinApp
{
public:
	CThreeTierRightsDemoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CThreeTierRightsDemoApp theApp;
