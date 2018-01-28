
// TestLoadLibraryDlg.h : ͷ�ļ�
//

#pragma once
#include "MyEditBrowseCtrl.h"
#include "afxeditbrowsectrl.h"

// CTestLoadLibraryDlg �Ի���
class CTestLoadLibraryDlg : public CDialogEx
{
// ����
public:
	CTestLoadLibraryDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTLOADLIBRARY_DIALOG };
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
	afx_msg void OnBnClickedButtonLoad();
	CString m_sDllPath;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CMyEditBrowseCtrl m_editBrowse;
};
