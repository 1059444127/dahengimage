#pragma once


// CDlgBtnCtrlGroup �Ի���

class CDlgBtnCtrlGroup : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgBtnCtrlGroup)

public:
	CDlgBtnCtrlGroup(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgBtnCtrlGroup();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BtnCtrlGroup };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
