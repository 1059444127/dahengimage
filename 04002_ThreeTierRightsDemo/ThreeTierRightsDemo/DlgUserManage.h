#pragma once


// CDlgUserManage �Ի���

class CDlgUserManage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgUserManage)

public:
	CDlgUserManage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgUserManage();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UserManage };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
