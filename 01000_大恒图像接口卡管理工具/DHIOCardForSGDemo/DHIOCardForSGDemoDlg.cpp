
// DHIOCardForSGDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DHIOCardForSGDemo.h"
#include "DHIOCardForSGDemoDlg.h"
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


// CDHIOCardForSGDemoDlg �Ի���



CDHIOCardForSGDemoDlg::CDHIOCardForSGDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DHIOCARDFORSGDEMO_DIALOG, pParent)
	, m_iCurIOCardType(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDHIOCardForSGDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_IOCardSN, m_comboIOcardSN);
	DDX_Radio(pDX, IDC_RADIO1, m_iCurIOCardType);
}

BEGIN_MESSAGE_MAP(CDHIOCardForSGDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OpenCard1, &CDHIOCardForSGDemoDlg::OnBnClickedButtonOpencard1)
	ON_CBN_SELCHANGE(IDC_COMBO_IOCardSN, &CDHIOCardForSGDemoDlg::OnCbnSelchangeComboIocardsn)
	ON_BN_CLICKED(IDC_BUTTON_OpenIOCardCfgFile, &CDHIOCardForSGDemoDlg::OnBnClickedButtonOpeniocardcfgfile)
	ON_CONTROL_RANGE(BN_CLICKED ,IDC_RADIO1, IDC_RADIO6, &CDHIOCardForSGDemoDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_BUTTON_OpenConnectionFile, &CDHIOCardForSGDemoDlg::OnBnClickedButtonOpenconnectionfile)
	ON_BN_CLICKED(IDC_BUTTON_OpenManageToolCfgFile, &CDHIOCardForSGDemoDlg::OnBnClickedButtonOpenmanagetoolcfgfile)
	ON_BN_CLICKED(IDC_BUTTON_ReInit, &CDHIOCardForSGDemoDlg::OnBnClickedButtonReinit)
END_MESSAGE_MAP()


// CDHIOCardForSGDemoDlg ��Ϣ�������

BOOL CDHIOCardForSGDemoDlg::OnInitDialog()
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

	InitWindow();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDHIOCardForSGDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDHIOCardForSGDemoDlg::OnPaint()
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
HCURSOR CDHIOCardForSGDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDHIOCardForSGDemoDlg::OnBnClickedButtonOpencard1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int iIOCardSN = m_comboIOcardSN.GetCurSel();
	m_pIOCard[iIOCardSN].reset();
	switch (m_p_eIOCardType[iIOCardSN])
	{
	case e_IOCardType::e_IOCardType_NONE:
		return;
	case e_IOCardType::e_IOCardType_SG:
		m_pIOCard[iIOCardSN] = make_shared<CDHIOCardForSG>();
		break;
	case e_IOCardType::e_IOCardType_PIOB:
		return;
	case e_IOCardType::e_IOCardType_PIOC:
		m_pIOCard[iIOCardSN] = make_shared<CDHIOCardForPIO24C>();
		break;
	case e_IOCardType::e_IOCardType_PCI_6356:
		m_pIOCard[iIOCardSN] = make_shared<CDHIOCardForPCI6356>();
		break;
	case e_IOCardType::e_IOCardType_PCIE_6356:
		break;
	case e_IOCardType::e_IOCardType_RS232_XZXNYN:
		m_pIOCard[iIOCardSN] = make_shared<CDHIOCardForRS232_XZXNYN>();
		break;
	default:
		return;
	}
	if (nullptr == m_pIOCard[iIOCardSN])
		return;
	//AfxMessageBox(m_p_sPathFileOfInit[iIOCardSN]);
	BOOL bTemp = PathFileExists(m_p_sPathFileOfInit[iIOCardSN]);
	if (FALSE == PathFileExists(m_p_sPathFileOfInit[iIOCardSN]))
	{
		AfxMessageBox(_T("��ʧ��, �ӿڿ������ļ������ڣ�"));
		return;
	}
	CINICfg iniTemp(m_p_sPathFileOfInit[iIOCardSN]);
// 	int iIOCardTypeTemp = iniTemp.GetIntEx(_T("Critical Config"), _T("�ӿڿ�����"), 0, _T("0���޿���1ģ��ӿڿ���2��B����3��C����4��PCI6356����5��PCIE_6356���� 6��RS232_XZXNYN���ڼ̵���"));
// 	if (m_p_eIOCardType[iIOCardSN] != (e_IOCardType)iIOCardTypeTemp)
// 	{
// 		if (IDOK == AfxMessageBox(_T("�ӿڿ������ļ����Ͳ���,�Ƿ�������ļ��޸ģ�"), MB_OKCANCEL))
// 		{
// 			OnBnClickedButtonOpeniocardcfgfile();
// 		}
// 		return;
// 	}

	CIOCardInitInfo initInfo;
	int iSrcSize = m_p_sPathFileOfInit[iIOCardSN].GetLength() * sizeof(TCHAR);
	memcpy_s(initInfo.lpszPathFileOfInit, MAX_PATH*sizeof(TCHAR), m_p_sPathFileOfInit[iIOCardSN], m_p_sPathFileOfInit[iIOCardSN].GetLength() * sizeof(TCHAR));
	memcpy_s(initInfo.lpszPathFileOfConnection, MAX_PATH * sizeof(TCHAR), m_p_sPathFileOfConnect[iIOCardSN], m_p_sPathFileOfConnect[iIOCardSN].GetLength() * sizeof(TCHAR));
	if (!m_pIOCard[iIOCardSN]->InitCard(initInfo))
	{
		AfxMessageBox(_T("��ʼ���ӿڿ�ʧ��\n")+m_pIOCard[iIOCardSN]->GetLastError());
	}
	if (!m_pIOCard[iIOCardSN]->OpenCard())
	{
		AfxMessageBox(_T("�򿪽ӿڿ�ʧ��\n")+m_pIOCard[iIOCardSN]->GetLastError());
	}
	m_pIOCard[iIOCardSN]->OpenTools(this);
}


void CDHIOCardForSGDemoDlg::OnCbnSelchangeComboIocardsn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if (m_iIOCardNum > 0)
	{
		m_iLastIOCardSN = m_comboIOcardSN.GetCurSel();
		m_iCurIOCardType = (int)m_p_eIOCardType[m_iLastIOCardSN]-1;
	}
	UpdateData(FALSE);
}


void CDHIOCardForSGDemoDlg::OnBnClickedButtonOpeniocardcfgfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	int iIOCardSN = m_comboIOcardSN.GetCurSel();
	if (0 < m_iIOCardNum)
	{
		if (FALSE == PathFileExists(m_p_sPathFileOfInit[iIOCardSN]))
		{
			AfxMessageBox(_T("�ӿڿ������ļ������ڣ�"));
			return;
		}
		CString sCmd = _T("notepad.exe ") + m_p_sPathFileOfInit[iIOCardSN];
		//WinExec(sCmd, SW_SHOW);
		//system(m_p_sPathFileOfInit[iIOCardSN]);
		ShellExecute(NULL, _T("open"), m_p_sPathFileOfInit[iIOCardSN], NULL, NULL, SW_SHOWNORMAL);
	}
}


void CDHIOCardForSGDemoDlg::OnBnClickedRadio1(UINT uID)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	for (int i = 0; i < m_iIOCardNum; ++i)
	{
		if ((int)m_p_eIOCardType[i] == m_iCurIOCardType + 1)
		{
			m_comboIOcardSN.SetCurSel(i);
			UpdateData(FALSE);
			return;
		}
	}
	AfxMessageBox(_T("�����ļ��޴����ͽӿڿ���"));
	m_iCurIOCardType = (int)m_p_eIOCardType[m_iLastIOCardSN]-1;
	UpdateData(FALSE);
}


void CDHIOCardForSGDemoDlg::OnBnClickedButtonOpenconnectionfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	int iIOCardSN = m_comboIOcardSN.GetCurSel();
	if (0 < m_iIOCardNum)
	{
		ShellExecute(NULL, _T("open"), m_p_sPathFileOfConnect[iIOCardSN], NULL, NULL, SW_SHOWNORMAL);
	}
}


void CDHIOCardForSGDemoDlg::OnBnClickedButtonOpenmanagetoolcfgfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sPathFileManageTool = CExFn::GetCurrentAppPath() + _T("\\Config\\DHIOCard\\DHIOCardManageTools.ini");
	if (TRUE == PathFileExists(sPathFileManageTool))
	{
		ShellExecute(NULL, _T("open"), sPathFileManageTool, NULL, NULL, SW_SHOWNORMAL);
	}
}


void CDHIOCardForSGDemoDlg::OnBnClickedButtonReinit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	InitWindow();
	AfxMessageBox(_T("����ɹ���"));
	
}


bool CDHIOCardForSGDemoDlg::InitWindow()
{
	m_comboIOcardSN.ResetContent();
	m_iIOCardNum = 0;
	fill(m_arr_bEnable.begin(), m_arr_bEnable.end(), false);


	m_iniIOCardManageCfg.Init(CExFn::GetCurrentAppPath() + _T("\\Config\\DHIOCard\\DHIOCardManageTools.ini"));
	m_iIOCardNum = m_iniIOCardManageCfg.GetIntEx(_T("Critical Config"), _T("�ӿڿ���Ŀ"), 0);
	m_pIOCard = make_unique<shared_ptr<CDHIOCard>[]>(m_iIOCardNum);
	m_p_eIOCardType = make_unique<e_IOCardType[]>(m_iIOCardNum);
	m_p_sPathFileOfInit = make_unique<CString[]>(m_iIOCardNum);
	m_p_sPathFileOfConnect = make_unique<CString[]>(m_iIOCardNum);
	m_iCurIOCardType = (int)e_IOCardType::e_IOCardType_NONE;
	m_iLastIOCardSN = 0;
	for (int i = 0; i < m_iIOCardNum; ++i)
	{
		CString sTemp{ _T("") };
		sTemp.Format(_T("�ӿڿ�%d����"), i);
		m_p_eIOCardType[i] = (e_IOCardType)m_iniIOCardManageCfg.GetIntEx(sTemp, _T("�ӿڿ�����"), 0, _T("0���޿���1ģ��ӿڿ���2��B����3��C����4��PCI6356����5��PCIE_6356���� 6��RS232_XZXNYN���ڼ̵���"));
		m_p_sPathFileOfInit[i] = m_iniIOCardManageCfg.GetStringEx(sTemp, _T("��ʼ���ļ�·����"), m_p_sPathFileOfInit[i], _T("NULL"));
		m_p_sPathFileOfConnect[i] = m_iniIOCardManageCfg.GetStringEx(sTemp, _T("�ӿڿ���ϵͳ�ӿ������ļ�"), m_p_sPathFileOfConnect[i], _T("NULL"));
		m_p_sPathFileOfInit[i] = CExFn::GetCurrentAppPath() + _T("\\") + m_p_sPathFileOfInit[i];
		m_p_sPathFileOfConnect[i] = CExFn::GetCurrentAppPath() + _T("\\") + m_p_sPathFileOfConnect[i];
		if (0 <= (int)m_p_eIOCardType[i] - 1)
			m_arr_bEnable[(int)m_p_eIOCardType[i] - 1] = true;
		if (0 == i)
		{
			m_iCurIOCardType = (int)m_p_eIOCardType[0] - 1;
		}
		CString sIOCardName{ _T("") };
		switch (m_p_eIOCardType[i])
		{
		case e_IOCardType::e_IOCardType_NONE:
			sIOCardName = _T("�޿�");
			break;
		case e_IOCardType::e_IOCardType_SG:
			sIOCardName = _T("ģ��ӿڿ�");
			break;
		case e_IOCardType::e_IOCardType_PIOB:
			sIOCardName = _T("B��");
			break;
		case e_IOCardType::e_IOCardType_PIOC:
			sIOCardName = _T("C��");
			break;
		case e_IOCardType::e_IOCardType_PCI_6356:
			sIOCardName = _T("PCI6356��");
			break;
		case e_IOCardType::e_IOCardType_PCIE_6356:
			sIOCardName = _T("PCIE6356��");
			break;
		case e_IOCardType::e_IOCardType_RS232_XZXNYN:
			sIOCardName = _T("RS232���ڼ̵���");
			break;
		default:
			break;
		}

		sTemp.Format(_T("�ӿڿ�%d  [%s]"), i, sIOCardName);
		m_comboIOcardSN.AddString(sTemp);
	}
	m_comboIOcardSN.SetCurSel(0);

	for (int i = 0; i < 6; ++i)
	{
		if (!m_arr_bEnable[i])
		{
			GetDlgItem(IDC_RADIO1 + i)->EnableWindow(FALSE);
		}
	}
	UpdateData(FALSE);
	return true;
}
