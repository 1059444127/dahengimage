#if !defined(AFX_DLGSYSTEMCONFIG_H__45E0B736_7F42_4AD4_94C9_1FCA76C65604__INCLUDED_)
#define AFX_DLGSYSTEMCONFIG_H__45E0B736_7F42_4AD4_94C9_1FCA76C65604__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSystemConfig.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSystemConfig dialog

class CDlgSystemConfig : public CDialog
{
	// Construction
public:
	CDlgSystemConfig(CWnd* pParent = NULL);   // standard constructor

	int		m_iShowMode;	//��ʾģʽ(0:����ʾ,1:��ʾ����,2:��ʾ��ƿ,3:��ʾ��Ʒ,4:��ʾ��Ʒ,5:��ʾ��ƿ)
	int		m_iKickMode;	//�޷�ģʽ 0Ϊȫ�ߣ�1Ϊȫ���ޣ�2Ϊ�����ߣ�3Ϊ����
	int     m_iShowDevice;	//0Ϊϵͳͼ�� 1Ϊ�㷨ͼ��
	BOOL m_bOpenAlgCheck[6]{ TRUE };
	BOOL m_bOpenObjStatusWnd{ FALSE };

							// Dialog Data
							//{{AFX_DATA(CDlgSystemConfig)
	enum { IDD = IDD_DLG_SYSTEM_CONFIG };
	double	m_dBlue;
	double	m_dGreen;
	double	m_dRed;
	BOOL	m_bKick1;
	BOOL	m_bBayer;
	int		m_iMaxSaveImageCount;
	int		m_iShowTime;
	BOOL	m_bKick2;
	BOOL	m_bBayer2;
	double	m_dBlue2;
	double	m_dGreen2;
	double	m_dRed2;
	double	m_dRed3;
	double	m_dGreen3;
	double	m_dBlue3;
	BOOL	m_bKick3;
	BOOL	m_bBayer3;
	BOOL	m_bBayer4;
	BOOL	m_bBayer5;
	BOOL	m_bBayer6;
	BOOL	m_bKick4;
	BOOL	m_bKick5;
	BOOL	m_bKick6;
	double	m_dBlue4;
	double	m_dBlue5;
	double	m_dBlue6;
	double	m_dGreen4;
	double	m_dGreen5;
	double	m_dGreen6;
	double	m_dRed4;
	double	m_dRed5;
	double	m_dRed6;
	BOOL	m_bSaveImageSys;
	//}}AFX_DATA



	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSystemConfig)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
														//}}AFX_VIRTUAL

														// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSystemConfig)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	// 	afx_msg void OnRadioNone();
	// 	afx_msg void OnRadioAll();
	// 	afx_msg void OnRadioError();
	// 	afx_msg void OnRadioExist();
	afx_msg void OnRadioKickAll();
	afx_msg void OnRadioKickNo();
	afx_msg void OnRadioKickDiff();
	afx_msg void OnRadioKickNormal();
	afx_msg void OnRadioSystem();
	afx_msg void OnRadioAlg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_iMaxNumOfConstantKick{10};//**[9/27/2017 ConanSteve]**:  ���������������
	afx_msg void OnBnClickedCheck6(UINT uID);
	afx_msg void OnBnClickedCheckStopMachnie0();
	afx_msg void OnBnClickedCheckStopMachnie1();
	int m_iLabelQueLen;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSYSTEMCONFIG_H__45E0B736_7F42_4AD4_94C9_1FCA76C65604__INCLUDED_)
