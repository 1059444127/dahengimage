#pragma once


// CLogin �Ի���

class CDlgLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLogin)

public:
	CDlgLogin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLogin();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Login };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_sUsername;
	CString m_sPassword;
	CString  m_sUserType{ _T("") };
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	bool SetCallPointer(void* pThreeTierRightsInterior);
private:
	void* m_pThreeTierRightsInterior{nullptr};
public:
	afx_msg void OnBnClickedCheckShowpsw();
	afx_msg void OnBnClickedCheckShowusername();
};
