
// TestLoadLibrary.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestLoadLibraryApp: 
// �йش����ʵ�֣������ TestLoadLibrary.cpp
//

class CTestLoadLibraryApp : public CWinApp
{
public:
	CTestLoadLibraryApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestLoadLibraryApp theApp;
