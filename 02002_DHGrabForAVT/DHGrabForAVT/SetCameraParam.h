#if !defined(AFX_SETCAMERAPARAM_H__81FEB935_2DA9_4AE2_8D02_5CCF886B3247__INCLUDED_)
#define AFX_SETCAMERAPARAM_H__81FEB935_2DA9_4AE2_8D02_5CCF886B3247__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetCameraParam.h : header file
//
#include "AVTCamDH.h"
#include "ExFn.h"
/////////////////////////////////////////////////////////////////////////////
// CSetCameraParam dialog

class CSetCameraParam : public CDialog
{
// Construction
public:
	CString m_strLanType;		 // �����ļ����� [4/8/2014 SJC]
	void SetDiscriptionInfo(CString strLanType);//�����������͸���������Ϣ

	UINT   m_iMaxWhiteBalCB;     // ���İ�ƽ��CB���� 
	UINT   m_iMaxWhiteBalCR;     // ���İ�ƽ��CR����
	UINT   m_iMaxBrightness;     // �������
	UINT   m_iMaxGain;           // �������
	UINT   m_iMaxShutter;        // ����ع�
	UINT   m_iMinBrightness;     // ��С����
	UINT   m_iMinGain;           // ��С����
	UINT   m_iMinShutter;        // ��С�ع�
	CSetCameraParam(CWnd* pParent = NULL);   // standard constructor
	void SetOwner(CAVTCamDH *pGrabber, const CString &strProfileName);
	void EnableShow(BOOL bEnable);

// Dialog Data
	//{{AFX_DATA(CSetCameraParam)
	enum { IDD = IDD_SET_CAMERA_DIALOG };
	CComboBox	m_TriggerMode;
	UINT		m_nTriggerMode;
	UINT		m_nBrightness;
	UINT		m_nGain;
	UINT		m_nShutter;
	int			m_nBrightnessSlider;
	int			m_nGainSlider;
	int			m_nShutterSlider;
	UINT	m_nAOILeft;
	UINT	m_nAOITop;
	UINT	m_nAOIWidth;
	UINT	m_nAOIHeight;
	UINT	m_nCB;
	UINT	m_nCR;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetCameraParam)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	DWORD           m_OldFrameCnt; 

	// Generated message map functions
	//{{AFX_MSG(CSetCameraParam)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditBrightness();
	afx_msg void OnChangeEditGain();
	afx_msg void OnChangeEditShutter();
	virtual void OnOK();
	afx_msg void OnSelchangeComboTriggerMode();
	afx_msg void OnClose();
	afx_msg void OnBtnApply();
	afx_msg void OnButtonAoiApply();
	afx_msg void OnButtonFrametest();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonLogin();
	afx_msg void OnButtonAoiApplywb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CAVTCamDH *m_pGrabber;
	CString m_strProfileName;

	UINT m_nOldBrightness;      //�޸�ǰ������
	UINT m_nOldGain;            //�޸�ǰ������
	UINT m_nOldShutter;         //�޸�ǰ��Shutter
	UINT m_nOldTriggerMode;     //�޸�ǰ�Ĵ���ģʽ

	UINT m_iOldCB;  //�޸�ǰ�İ�ƽ��UBֵ
	UINT m_iOldCR;  //�޸�ǰ�İ�ƽ��VRֵ

	// ����SLIDER�ؼ��������Сֵ�ͻ�������
	void SetSliderRange();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETCAMERAPARAM_H__81FEB935_2DA9_4AE2_8D02_5CCF886B3247__INCLUDED_)
