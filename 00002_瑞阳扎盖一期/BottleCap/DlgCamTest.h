#if !defined(AFX_DLGCAMTEST_H__3649FA70_66EA_43F0_9F8B_B84699B86562__INCLUDED_)
#define AFX_DLGCAMTEST_H__3649FA70_66EA_43F0_9F8B_B84699B86562__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCamTest.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCamTest dialog

class CDlgCamTest : public CDialog
{
// Construction
public:
	CDlgCamTest(CWnd* pParent = NULL);   // standard constructor

	int m_iCamLevel;			//�������ߵ͵�ƽ 0Ϊ�͵�ƽ��1Ϊ�ߵ�ƽ
	CString m_strAlgIniPath;			//�㷨�ļ�·��

	void RefreshCombo(CString strModelName);

// Dialog Data
	//{{AFX_DATA(CDlgCamTest)
	enum { IDD = IDD_DLG_CAM_TEST };
	int		m_iCameraSN;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCamTest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCamTest)
	afx_msg void OnBtnStart();
	afx_msg void OnBtnStop();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCAMTEST_H__3649FA70_66EA_43F0_9F8B_B84699B86562__INCLUDED_)
