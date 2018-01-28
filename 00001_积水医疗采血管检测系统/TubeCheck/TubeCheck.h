// TubeCheck.h : main header file for the TubeCheck application
//

#if !defined(AFX_TubeCheck_H__8C465693_791E_477D_93F8_B3B1E8BC899F__INCLUDED_)
#define AFX_TubeCheck_H__8C465693_791E_477D_93F8_B3B1E8BC899F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "ProgramLicense.h"
#include "Rockey4_ND_32.h"
#include "SysDef.h"
#include "..\\..\\IncPublic\\MultiObjectDef.h"
#include "..\\..\\IncPublic\\ImageDef.h"
#include "..\\..\\IncPublic\\IDS.h"
#include "..\\..\\IncPublic\\xcgui.h"
#include "..\\..\\IncPublic\\ExtenInfo.h"
#include "..\\..\\IncPublic\\ExFn.h"

/////////////////////////////////////////////////////////////////////////////
// CTubeCheckApp:
// See TubeCheck.cpp for the implementation of this class
//

class CTubeCheckApp : public CWinApp
{
public:
	CTubeCheckApp();


public:
	
	// ������Licence 
	CProgramLicense m_ProgramLicense;

	CString	 m_strWorkPath;										// ����·��
	CString	 m_strCheckSystemConfigPath;						// �����ļ�·��
	
	int m_iLicenseExpDate;										// License��������ʱ��

	// ��ȡ����·��
	CString GetCurrentAppPath();


	// �жϼ��ܹ��Ƿ����
	BOOL RockeyIsExist();

	// �жϼ��ܹ��Ƿ����,�����Ӳ��
	BOOL RockeyIsExist_HD();
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTubeCheckApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTubeCheckApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TubeCheck_H__8C465693_791E_477D_93F8_B3B1E8BC899F__INCLUDED_)
