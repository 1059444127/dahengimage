#if !defined(AFX_DLGCAMERASETTING_H__DDDBCBCE_FFAD_4798_929E_D0250F4F19AA__INCLUDED_)
#define AFX_DLGCAMERASETTING_H__DDDBCBCE_FFAD_4798_929E_D0250F4F19AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCameraSetting.h : header file
//
#include "JAICamDH.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCameraSetting dialog

class CDlgCameraSetting : public CDialog
{
// Construction
public:
	CDlgCameraSetting(CWnd* pParent = NULL);   // standard constructor

	int m_iMinExp;				//��С�ع�
	int m_iMaxExp;				//����ع�
	int m_iMinGain;				//��С����
	int m_iMaxGain;				//�������
	int m_iTriggerMode;			//����ģʽ 0���ⴥ�� 1���ڴ���

	CJAICamDH* m_pGrabber;		//�ɼ�����
	CString m_sConfigFile;		//���������ļ�


	void SetOwner(CJAICamDH* pGrabber,CString strConfigFile); //����ָ�� ��domodel()֮ǰ����



// Dialog Data
	//{{AFX_DATA(CDlgCameraSetting)
	enum { IDD = IDD_CAMERA_SETTING };
	CSliderCtrl	m_SliderGain;
	CSliderCtrl	m_SliderExp;
	CStatic	m_MinGain;
	CStatic	m_MaxGain;
	CStatic	m_MinExp;
	CStatic	m_MaxExp;
	CComboBox	m_ComboTrig;
	int		m_iExposureTime;
	int		m_iGain;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCameraSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCameraSetting)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCustomdrawSliderExp(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSliderGain(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCAMERASETTING_H__DDDBCBCE_FFAD_4798_929E_D0250F4F19AA__INCLUDED_)
