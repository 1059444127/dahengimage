
// ThreeTierRightsDemoDlg.h : ͷ�ļ�
//

#pragma once
#include "ThreeTierRights.h"

// CThreeTierRightsDemoDlg �Ի���
class CThreeTierRightsDemoDlg : public CDialogEx
{
// ����
public:
	CThreeTierRightsDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_THREETIERRIGHTSDEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonUsermanage();
	afx_msg void OnBnClickedButtonOperator();
	afx_msg void OnBnClickedButtonDebugger();

private:
	CThreeTierRights m_threeTierRights;
public:
	afx_msg void OnBnClickedButtonAdmin();
	afx_msg void OnBnClickedButtonAdmin2();
	afx_msg void OnBnClickedButtonDebugger2();
	afx_msg void OnBnClickedButtonOperator2();
	afx_msg void OnBnClickedButtonUsermanage2();
	afx_msg void OnBnClickedButtonSuperadmin();
};
