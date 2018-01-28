#pragma once


// CDlgIOCard �Ի���
#include "DHIOCardForSG.h"
#include "..\\inc\\xShadeButton.h"
#include "afxcmn.h"

#define ID_EVENT_UPDATA_LIST WM_USER+10//**[12/9/2017 ConanSteve]**:  ˢ�½�������

class CDlgIOCard : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgIOCard)

public:
	CDlgIOCard(CWnd* pParent = NULL, CDHIOCard* pIOCard = NULL);   // ��׼���캯��
	virtual ~CDlgIOCard();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOnoff();

private:
	int m_iQueueNum{0};
	bool m_bOpenCardSuccess{ false };
	bool	m_bReadRegister{ false };//**[12/13/2017 ConanSteve]**:  �Ƿ��ڶ�ȡ�Ĵ���
	CBitmap m_bmpON, m_bmpOFF;
	HICON m_iconON, m_iconOFF;
public:
	CDHIOCard* m_pIOCard{ nullptr };
	virtual BOOL OnInitDialog();
	CListCtrl m_listctrlQueueInfo;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonReadcnt();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonOut0(UINT uID);
	afx_msg void OnBnClickedButtonIn0(UINT uID);
	afx_msg void OnBnClickedButtonOpencfgfile();
	afx_msg void OnBnClickedCheckActive0(UINT uID);
	bool InitWindow();
	afx_msg void OnBnClickedButtonOpenconnectionfile();
};
