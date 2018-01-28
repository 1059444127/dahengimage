#pragma once

#include "MainFrm.h"
#include "DlgHisErrorInfo.h"
#include "StaticFader.h"
#include "xShadeButton.h"
#include "afxwin.h"
#include "DlgSwitchModel.h"
// CDlgRunning dialog

class CDlgRunning : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRunning)

public:
	CDlgRunning(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgRunning();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Running };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonClick(UINT nID);
	virtual BOOL OnInitDialog();
/*************************************<�Զ����Ա>************************************[9/7/2017 ConanSteve]*/
private:
	CCriticalSection m_csShowBitmapInfoSection;//**[9/5/2017 ConanSteve]**:  ��ͼ�ٽ���
public:
	CMainFrame * m_pFrame{NULL};
	unique_ptr<CStatic[]> m_pStaticCamImg{ nullptr };
	unique_ptr<CStatic[]> m_pStaticAlgImg{ nullptr };
	//**[8/24/2017 ConanSteve]**:  camera number
	int m_iGrabberNum{ 0 };
	//**[8/24/2017 ConanSteve]**:  rectangle position of original images from camera
	unique_ptr<CRect[]> m_p_rectCamImg{ NULL };
	//**[8/24/2017 ConanSteve]**:  rectangle position of algorithm images 
	unique_ptr<CRect[]> m_p_rectAlgImg{ NULL };
	shared_ptr<CDlgHisErrorInfo> m_pdlgHisErrorInfo;
	shared_ptr<CDlgSwitchModel> m_pDlgSwitchModel;
	/*************************************<���ư�ť>************************************[9/20/2017 ConanSteve]*/
	shared_ptr<CButton> m_pBtnRecord;//**[9/20/2017 ConanSteve]**:  ��־��¼��ť
	shared_ptr<CButton> m_pBtnDefectStatistic;//**[9/20/2017 ConanSteve]**:  ȱ��ͳ�ư�ť

	CxShadeButton m_btnSwitchModel;//**[12/8/2017 ConanSteve]**:  �л�ģ��
	CxShadeButton m_btnOpenIOCardTools;//**[12/8/2017 ConanSteve]**:  �򿪽ӿڿ�����
	CxShadeButton m_btnRecord;//**[12/8/2017 ConanSteve]**:  ����־��¼



	//**[12/1/2017 ConanSteve]**:  ��Ϣ��ʾ
	CStatic m_staticCheckCntLabel;//**[12/1/2017 ConanSteve]**:  �������
	CStatic m_staticErrorCntLabel;//**[12/1/2017 ConanSteve]**:  ȱ������
	CStatic m_staticGoodCntLabel;//**[12/1/2017 ConanSteve]**:  ��Ʒ����
	CStatic m_staticGoodRateLabel;//**[12/1/2017 ConanSteve]**:  �ϸ���
	CListCtrl m_listctrlRealtimeRunInfo;//**[12/8/2017 ConanSteve]**:  ʵʱ������Ϣ��������ʾ���б����ʹ�����Ϣ


	const int m_ci_MaxCustomBtnNum{ 10 };
	int m_iCustomLabelNum{ 5 };//**[12/27/2017 ConanSteve]**:  ͨ�ð�ť����
	array<CxShadeButton, 20> m_arr_btnCustomLabel;//**[12/27/2017 ConanSteve]**:  ͨ�ð�ť��ǩ����
	int m_iCustomLabelStyle = 0;//**[12/27/2017 ConanSteve]**:  ��Ӱ���
	int m_iCustomLabelFontSize = 20;
	int m_iCustomLabelFontWeight = 400;
	int m_iCustomLabelFontTextColor = RGB(0x0, 0x72, 0xc6);
	bool m_bEnableCustomLabelBtnStyle{ true };
	CString m_sCustomLabelFontName = _T("����");
	vector<CxShadeButton*> m_vec_pCustomLabel;
	CStatic m_sStaticGroupOfLabel;

	int m_iAssemblyLineNum{ 1 };//**[1/24/2018 ConanSteve]**:  ��ˮ�߸���
	array<CxShadeButton, 20> m_arr_labelPerAssembly;//**[1/24/2018 ConanSteve]**:  ÿ����ˮ�ߵı�ǩ


	int m_iCustomBtnNum{ 6 };
	array<CxShadeButton, 100> m_arr_btnCustomBtn;//**[12/27/2017 ConanSteve]**:  ͨ�ð�ť����
	int m_iCustomBtnStyle = 0;//**[12/27/2017 ConanSteve]**:  ��Ӱ���
	int m_iCustomBtnFontSize = 20;
	int m_iCustomBtnFontWeight = 400;
	int m_iCustomBtnFontTextColor = RGB(0x0, 0x72, 0xc6);
	CString m_sCustomBtnFontName = _T("����");
	vector<CxShadeButton*> m_vec_pCustomBtn;
	CStatic m_sStaticGroupOfBtn;


	CLayout m_layout;
	bool m_bNeedAddCtrl = true;//**[1/16/2018 ConanSteve]**: �Ƿ���Ӱ�ť

private:
	void DisplayCameraImage(CDC *pDC, const int iCamGroup, int iIndex, e_ShowModeFlip eShowMode);
	void ShowDIBImage(CDC* pDC, CRect Rect, e_ShowModeFlip ShowModeFlip, e_ShowModeStretch ShowModeStretch,
		const BYTE* pImageBuff, long lImageWidth, long lImageHeight, long lImageBitCount, int iCameraSN);
public:
	void DisplayImage(const int iCamGroup, int iIndex);
	CStatic m_staticTest;
	CStatic m_staticCompanyLogo;
	CStatic m_staticTime;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	bool ResizeLayout();
};
