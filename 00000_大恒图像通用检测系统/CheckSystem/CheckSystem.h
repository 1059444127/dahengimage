
// CheckSystem.h : main header file for the CheckSystem application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
 #pragma comment(lib, "xcgui.lib")
 #include "xcgui.h"
#include "resourceOfXC.h"
#include "ExFn.h"
#include "Markup.h"
#include "xShadeButton.h"
#include "memory"
#include "SystemMacro.h"
#include "resourceOfPerson.h"
#include "ParamDefine.h"
#include "DxImageProc.h"
#include "alg_struct.h"
#include "Rockey4_ND_32.h"
#include "ProgramLicense.h"
#pragma comment(lib, "..\\Lib\\DHProgramLicense.lib")

#include "vector"
#include "array"


using namespace std;
//#include "CommonFiles\CmnHdr.h"

#define MaxLenOfBuf 1024

#define  tr(quote) _T(quote)
using namespace std;
//#include "Common.h"
// CCheckSystemApp:
// See CheckSystem.cpp for the implementation of this class
//

class CCheckSystemApp : public CWinApp
{
public:
	CCheckSystemApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
public:
	// ������Licence 
	CProgramLicense m_ProgramLicense;

	CString	 m_strWorkPath;										// ����·��
	CString	 m_strCheckSystemConfigPath;						// �����ļ�·��

	int m_iLicenseExpDate;										// License��������ʱ��

																// ��ȡ����·��

	// �жϼ��ܹ��Ƿ����
	BOOL RockeyIsExist();

	// �жϼ��ܹ��Ƿ����,�����Ӳ��
	BOOL RockeyIsExist_HD();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCheckSystemApp theApp;
