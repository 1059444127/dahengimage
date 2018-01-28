// BottleCapView.h : interface of the CBottleCapView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BottleCapVIEW_H__2CE4C44C_E38A_4C4D_A1CD_244FE6789ECE__INCLUDED_)
#define AFX_BottleCapVIEW_H__2CE4C44C_E38A_4C4D_A1CD_244FE6789ECE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "resource.h"       // main symbols
#include "MainFrm.h"
#include "StaticFader.h"
#include "BottleCapDoc.h"
#include "xShadeButton.h"
#include "DlgShowImage.h"
#include "DlgPage1.h"
#include "DlgPage2.h"
#include "DlgAlarm.h"
#include "DlgDefect.h"
#include "DlgAlarmPos.h"
#include "DlgAlarmStopCount.h"
#include "DlgUserAdm.h"

class CBottleCapView : public CFormView
{
protected: // create from serialization only
	CBottleCapView();
	DECLARE_DYNCREATE(CBottleCapView)

public:
	//{{AFX_DATA(CBottleCapView)
	enum { IDD = IDD_BottleCap_FORM };
	CStaticFader	m_UserData;
	CStaticFader	m_UserLabel;
	CStaticFader	m_SpeedData;
	CStaticFader	m_SpeedLabel;
	CxShadeButton	m_BtnZero;
	CxShadeButton	m_BtnDefectStatistic;
	CTabCtrl	m_Tab1;
	CxShadeButton	m_BtnModel;
	CxShadeButton	m_BtnParam;
	CxShadeButton	m_BtnQuit;
	CxShadeButton	m_BtnRecord;
	CxShadeButton	m_BtnStatistic;
	CxShadeButton	m_BtnStart;
	CStaticFader	m_TotalGoodRatioData;
	CStaticFader	m_TotalGoodRatioLabel;
	CStaticFader	m_TotalGoodLabel;
	CStaticFader	m_TotalGoodData;
	CStaticFader	m_ModelNameLabel;
	CStaticFader	m_ModelNameData;
	CStaticFader	m_TotalData;
	CStaticFader	m_TotalLabel;
	//}}AFX_DATA

// Attributes
public:
	CBottleCapDoc* GetDocument();

public:
	int m_iIndex;					//�б������Ŀ����
	int m_iSaveImageType;			//����ͼ������ 0Ϊ������1Ϊ����
	int m_iIsCircle;				//�Ƿ�ѭ������ͼ�� 0Ϊ��ѭ��
	int m_iSaveImageCount;			//����ͼ������
	int m_iLabelFontHeight;			//��ǩ�����С

	BOOL m_bAlarm_StopCount[ALLELEMENT_COUNT]; //�Ƿ�ֹͣ��������
	BOOL m_bLabelAlarm[ALLELEMENT_COUNT];	// ��ǩ��ⱨ��
	BOOL m_bQuitAlarmThread;		// �˳������߳�

	CRect  m_rectShowImage[CAMERA_COUNT];								//ͼ��ѹ����ʾ��		
	CCriticalSection m_csShowBitmapInfoSection;

	CFont m_fontTitle;				//��������
	CFont m_fontTime;				//ʱ������
	CFont m_fontSystem;				//ϵͳ��������
	CFont m_fontAdv;				//�߼�ģʽ����

	CDlgShowImage* m_pShowImage;	//��ʾͼ��
	CDlgAlarm* m_pdlgAlarm;			//��ʾ������Ϣ
	CDlgAlarmPos* m_pdlgAlarmPos;	//��ǩλ�ñ�����Ϣ
	CDlgDefect* m_pdlgDefect;		//��ʾȱ��ͳ����Ϣ

	CDlgAlarmStopCount* m_pdlgAlarmStop;	//ֹͣ��������

	
	CDlgPage1 m_dlgPage1;			//�鿴����ͼ������ҳ1

	// ��ʾ������Ϣ�߳�
	CWinThread* m_pShowWarmingInfoThread;	
	static DWORD ShowWarmingInfoThread(LPVOID lpParam);
	DWORD ShowWarmingInfoThreadFunc();

	// ���ֹͣ����
	CWinThread* m_pMonitorStopCountThread;	
	static DWORD MonitorStopCountThread(LPVOID lpParam);
	DWORD MonitorStopCountThreadFunc();



// Operations
public:
	//��Ӽ�¼���б�ؼ�
	//iCamGroup ��ʾ��λ�� iImageSN ͼ���  iCircleSN �ɼ����(�󴥷�����֡�������iImageSN��ͬ)
	void AddRecordToListCtrl(int iCamGroup, int iImageSN, int iCircleSN, CString strSaveTime);

	// ���ع�Դ��Ϣ�Ͳ�Ʒ��Ϣ
	BOOL LoadLightAndProductInfo(CString strModelName,CString sOldModelName);

	//�ⴥ���������ʾͼ��
	void DisplayCameraImage(CDC* pDC, const int iCameraSN, int iIndex, e_ShowModeFlip eShowMode);
	void DisplayImage(const int iCamGroup, int iIndex);

	void ShowDIBImage(CDC* pDC, CRect Rect, e_ShowModeFlip ShowModeFlip, 
		e_ShowModeStretch ShowModeStretch,const BYTE* pImageBuff,
		long lImageWidth, long lImageHeight, long lImageBitCount,int iCameraSN);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBottleCapView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:

	// ��ʼ������
	BOOL InitWindowRect();
	virtual ~CBottleCapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBottleCapView)
	afx_msg void OnBtnQuit();
	afx_msg void OnBtnParam();
	afx_msg void OnBtnMouldAdm();
	afx_msg void OnBtnStatistic();
	afx_msg void OnBtnStart();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnRecord();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSaveImage();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnIocard();
	afx_msg void OnSystemConfig();
	afx_msg void OnAdvMode();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnZero();
	afx_msg void OnBtnDefectStatistics();
	afx_msg void OnStopSaveImage();
	afx_msg void OnUserAdm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedStopSaveImage();
	afx_msg void OnStnClickedMp();
	afx_msg void OnStnDblclickMp();
};

#ifndef _DEBUG  // debug version in BottleCapView.cpp
inline CBottleCapDoc* CBottleCapView::GetDocument()
   { return (CBottleCapDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BottleCapVIEW_H__2CE4C44C_E38A_4C4D_A1CD_244FE6789ECE__INCLUDED_)
