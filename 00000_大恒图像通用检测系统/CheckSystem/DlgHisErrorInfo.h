#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgHisErrorInfo �Ի���

class CDlgHisErrorInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHisErrorInfo)

public:
	CDlgHisErrorInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgHisErrorInfo();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HisErrorInfo };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


public:
	int m_iIndexOfCheckGroupSelected{ 0 };
	CComboBox m_comboCheckGroup;
	CListCtrl m_listctrlDefectStatistics;
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	bool UpdateDefectList();
	afx_msg void OnCbnSelchangeComboCheckgourp();
};
