
// DHIOCardForSGDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDHIOCardForSGDemoApp: 
// �йش����ʵ�֣������ DHIOCardForSGDemo.cpp
//

class CDHIOCardForSGDemoApp : public CWinApp
{
public:
	CDHIOCardForSGDemoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDHIOCardForSGDemoApp theApp;
