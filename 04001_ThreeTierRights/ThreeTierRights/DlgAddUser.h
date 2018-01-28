#pragma once
#include "afxwin.h"


// CDlgAddUser �Ի���

class CDlgAddUser : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAddUser)

public:
	CDlgAddUser(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAddUser();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_AddUser };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_iUserType;
	CString m_sUsername;
	CString m_sPassword1;
	CString m_sPassword2;
	CComboBox m_comboUserType;
	void* m_pThreeTierRightsInterior{nullptr};

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
