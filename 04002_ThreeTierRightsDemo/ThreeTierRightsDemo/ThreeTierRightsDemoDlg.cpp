
// ThreeTierRightsDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ThreeTierRightsDemo.h"
#include "ThreeTierRightsDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CThreeTierRightsDemoDlg �Ի���



CThreeTierRightsDemoDlg::CThreeTierRightsDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_THREETIERRIGHTSDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CThreeTierRightsDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CThreeTierRightsDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Login, &CThreeTierRightsDemoDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_UserManage, &CThreeTierRightsDemoDlg::OnBnClickedButtonUsermanage)
	ON_BN_CLICKED(IDC_BUTTON_Operator, &CThreeTierRightsDemoDlg::OnBnClickedButtonOperator)
	ON_BN_CLICKED(IDC_BUTTON_Debugger, &CThreeTierRightsDemoDlg::OnBnClickedButtonDebugger)
	ON_BN_CLICKED(IDC_BUTTON_Admin, &CThreeTierRightsDemoDlg::OnBnClickedButtonAdmin)
	ON_BN_CLICKED(IDC_BUTTON_Admin2, &CThreeTierRightsDemoDlg::OnBnClickedButtonAdmin2)
	ON_BN_CLICKED(IDC_BUTTON_Debugger2, &CThreeTierRightsDemoDlg::OnBnClickedButtonDebugger2)
	ON_BN_CLICKED(IDC_BUTTON_Operator2, &CThreeTierRightsDemoDlg::OnBnClickedButtonOperator2)
	ON_BN_CLICKED(IDC_BUTTON_UserManage2, &CThreeTierRightsDemoDlg::OnBnClickedButtonUsermanage2)
	ON_BN_CLICKED(IDC_BUTTON_SuperAdmin, &CThreeTierRightsDemoDlg::OnBnClickedButtonSuperadmin)
END_MESSAGE_MAP()


// CThreeTierRightsDemoDlg ��Ϣ�������

BOOL CThreeTierRightsDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_threeTierRights.AddCtrl(GetDlgItem(IDC_BUTTON_Operator2));
	m_threeTierRights.AddCtrl(GetDlgItem(IDC_BUTTON_Debugger2));
	m_threeTierRights.AddCtrl(GetDlgItem(IDC_BUTTON_Admin2));
	m_threeTierRights.AddCtrl(GetDlgItem(IDC_BUTTON_UserManage2));
	m_threeTierRights.AddCtrl(GetDlgItem(IDC_BUTTON_SuperAdmin));
	OnBnClickedButtonLogin();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CThreeTierRightsDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CThreeTierRightsDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CThreeTierRightsDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CThreeTierRightsDemoDlg::OnBnClickedButtonLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_threeTierRights.Login();
	GetDlgItem(IDC_BUTTON_Operator2)->ShowWindow(m_threeTierRights.VerifyRights(GetDlgItem(IDC_BUTTON_Operator2)));
	GetDlgItem(IDC_BUTTON_Debugger2)->ShowWindow(m_threeTierRights.VerifyRights(GetDlgItem(IDC_BUTTON_Debugger2)));
	GetDlgItem(IDC_BUTTON_Admin2)->ShowWindow(m_threeTierRights.VerifyRights(GetDlgItem(IDC_BUTTON_Admin2)));
	GetDlgItem(IDC_BUTTON_UserManage2)->ShowWindow(m_threeTierRights.VerifyRights(GetDlgItem(IDC_BUTTON_UserManage2)));
	GetDlgItem(IDC_BUTTON_SuperAdmin)->ShowWindow(m_threeTierRights.VerifyRights(GetDlgItem(IDC_BUTTON_SuperAdmin)));
	UpdateData(FALSE);
}


void CThreeTierRightsDemoDlg::OnBnClickedButtonUsermanage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_threeTierRights.ManageUsers();
}


void CThreeTierRightsDemoDlg::OnBnClickedButtonOperator()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_threeTierRights.VerifyRights(e_UserType_Operator, true))
		AfxMessageBox(_T("Execute Succeed!"));
}


void CThreeTierRightsDemoDlg::OnBnClickedButtonDebugger()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_threeTierRights.VerifyRights(e_UserType_Debugger, true))
		AfxMessageBox(_T("Execute Succeed!"));
}


void CThreeTierRightsDemoDlg::OnBnClickedButtonAdmin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_threeTierRights.VerifyRights(e_UserType_Admin, true))
		AfxMessageBox(_T("Execute Succeed!"));
}


void CThreeTierRightsDemoDlg::OnBnClickedButtonAdmin2()
{
	// TODO: Add your control notification handler code here
}


void CThreeTierRightsDemoDlg::OnBnClickedButtonDebugger2()
{
	// TODO: Add your control notification handler code here
}


void CThreeTierRightsDemoDlg::OnBnClickedButtonOperator2()
{
	// TODO: Add your control notification handler code here
}


void CThreeTierRightsDemoDlg::OnBnClickedButtonUsermanage2()
{
	// TODO: Add your control notification handler code here
}


void CThreeTierRightsDemoDlg::OnBnClickedButtonSuperadmin()
{
	// TODO: Add your control notification handler code here
}
