#if !defined(AFX_TESTCIPHERDLG_H__20CDFF02_A306_11D5_9302_E6DCFE97F30D__INCLUDED_)
#define AFX_TESTCIPHERDLG_H__20CDFF02_A306_11D5_9302_E6DCFE97F30D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

// DlgTestCipher.h : header file
//
/********************************************************************
	created:	2001/09/06
	created:	6:9:2001   21:53
	filename: 	d:\liufenglong\moneycode\dlgtestcipher.h
	file path:	d:\liufenglong\moneycode
	file base:	dlgtestcipher
	file ext:	h
	author:		lfl
	
	purpose:	�����޸Ĳ���������
*********************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CDlgTestCipher dialog

class CDlgTestCipher : public CDialog
{
// Construction
public:
	int		m_itimes;		//����������Ĵ���,��������,�Ի����Զ�ȡ��
	BOOL	m_bOK;			//����׼ȷ
	CDlgTestCipher(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTestCipher)
	enum { IDD = IDD_DLG_TESTCIPHER };
	CString	m_cipher;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTestCipher)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTestCipher)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTCIPHERDLG_H__20CDFF02_A306_11D5_9302_E6DCFE97F30D__INCLUDED_)
