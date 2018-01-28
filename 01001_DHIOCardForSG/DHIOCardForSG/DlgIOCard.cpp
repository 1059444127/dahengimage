// DlgIOCard.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DHIOCardForSG.h"
#include "DHIOCardForSGInterior.h"
#include "DlgIOCard.h"
#include "afxdialogex.h"
#include "resource.h"
#include "ExFn.h"

// CDlgIOCard �Ի���

IMPLEMENT_DYNAMIC(CDlgIOCard, CDialogEx)

CDlgIOCard::CDlgIOCard(CWnd* pParent /*=NULL*/, CDHIOCard* pIOCard)
	: CDialogEx(IDD_DIALOG_SG, pParent)
{
	m_pIOCard = pIOCard;
	CDHIOCardForSGInterior* pTempIOCard = (CDHIOCardForSGInterior*)pIOCard;
	m_iQueueNum = pTempIOCard->m_iQueueNum;
}

CDlgIOCard::~CDlgIOCard()
{
}

void CDlgIOCard::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_QueueInfo, m_listctrlQueueInfo);
}


BEGIN_MESSAGE_MAP(CDlgIOCard, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ONOFF, &CDlgIOCard::OnBnClickedButtonOnoff)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_ReadCnt, &CDlgIOCard::OnBnClickedButtonReadcnt)
	ON_BN_CLICKED(IDC_BUTTON_Reset, &CDlgIOCard::OnBnClickedButtonReset)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_Out0, IDC_BUTTON_Out23, &CDlgIOCard::OnBnClickedButtonOut0)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_In0, IDC_BUTTON_In11,&CDlgIOCard::OnBnClickedButtonIn0)
	ON_BN_CLICKED(IDC_BUTTON_OpenCfgFile, &CDlgIOCard::OnBnClickedButtonOpencfgfile)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK_Active0, IDC_CHECK_Active11, &CDlgIOCard::OnBnClickedCheckActive0)
	ON_BN_CLICKED(IDC_BUTTON_OpenConnectionFile, &CDlgIOCard::OnBnClickedButtonOpenconnectionfile)
END_MESSAGE_MAP()


// CDlgIOCard ��Ϣ�������


void CDlgIOCard::OnBnClickedButtonOnoff()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_bOpenCardSuccess = !m_bOpenCardSuccess;
	if (m_bOpenCardSuccess)
	{	
		if(nullptr != m_pIOCard)
			m_pIOCard->Enable(true);//**[12/8/2017 ConanSteve]**:  ʹ�ӿڿ�����
		SetDlgItemText(IDC_BUTTON_ONOFF, _T("ֹͣ����"));
	}
	else
	{
		if (nullptr != m_pIOCard)
			m_pIOCard->Enable(false);//**[12/8/2017 ConanSteve]**:  ʹ�ӿڿ�ֹͣ����
		SetDlgItemText(IDC_BUTTON_ONOFF, _T("��ʼ����"));
	}
	UpdateData(FALSE);
}


BOOL CDlgIOCard::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_bmpON.LoadBitmap(IDB_BITMAP_ON);
	m_bmpOFF.LoadBitmap(IDB_BITMAP_OFF);

	m_listctrlQueueInfo.InsertColumn(0, _T("���б��"), LVCFMT_LEFT, 100);
	m_listctrlQueueInfo.InsertColumn(1, _T("��������(ms)"), LVCFMT_LEFT, 100);
	m_listctrlQueueInfo.InsertColumn(2, _T("����˿�"), LVCFMT_LEFT, 100);
	m_listctrlQueueInfo.InsertColumn(3, _T("��������"), LVCFMT_LEFT, 100);
	InitWindow();

	/*************************************<>************************************[12/16/2017 ConanSteve]*/
// 	BITMAP bmp;
// 	m_bmpON.GetBitmap(&bmp);
// 
// 	HBITMAP hbmMask = ::CreateCompatibleBitmap(::GetDC(NULL),
// 		bmp.bmWidth, bmp.bmHeight);
// 
// 	ICONINFO ii = { 0 };
// 	ii.fIcon = TRUE;
// 	ii.hbmColor = m_bmpON;
// 	ii.hbmMask = hbmMask;
// 	HICON hIcon = ::CreateIconIndirect(&ii);//һ��������Ҫ��ע����DestroyIcon�����ͷ�ռ�õ��ڴ漰��Դ
// 	::DeleteObject(hbmMask);
	/*************************************<>************************************[12/16/2017 ConanSteve]*/
	for (int i = 0; i < 24; ++i)
	{
		//((CButton*)GetDlgItem(IDC_BUTTON_Out0 + i))->SetIcon(hIcon);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgIOCard::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent)
	{
	case ID_EVENT_UPDATA_LIST:
	{
		UpdateData(TRUE);
		CDHIOCardForSGInterior *pIOCard = (CDHIOCardForSGInterior *)m_pIOCard;
		for (int i = 0; i < m_iQueueNum; ++i)
		{
			m_listctrlQueueInfo.SetItemText(i, 3, CExFn::Int2CS(pIOCard->m_arr_iTriggerCnt[i]));
		}
		for (int i = 0; i < MAX_INPORT_NUM; ++i)
		{
			CString sTemp{ _T("") };
			sTemp.Format(_T("%06d"), pIOCard->m_arr_iInportCnt[i]);
			SetDlgItemText(IDC_STATIC_In0 + i, sTemp);
		}
		for (int i = 0; i < MAX_OUTPORT_NUM; ++i)
		{
			CString sTemp{ _T("") };
			sTemp.Format(_T("%06d"), pIOCard->m_arr_iOutportCnt[i]);
			SetDlgItemText(IDC_STATIC_Out0 + i, sTemp);
		}
		UpdateData(FALSE);
	}
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CDlgIOCard::OnBnClickedButtonReadcnt()
{
	m_bReadRegister = !m_bReadRegister;
	if (m_bReadRegister)
	{
		SetTimer(ID_EVENT_UPDATA_LIST, 500, NULL);
		SetDlgItemText(IDC_BUTTON_ReadCnt, _T("ֹͣ��ȡ����"));
	}
	else
	{
		KillTimer(ID_EVENT_UPDATA_LIST); 
		SetDlgItemText(IDC_BUTTON_ReadCnt, _T("��ʼ��ȡ����"));
		//GetDlgItem(IDC_BUTTON_ReadCnt)->EnableWindow(TRUE);
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CDlgIOCard::OnBnClickedButtonReset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDHIOCardForSGInterior *pIOCard = (CDHIOCardForSGInterior *)m_pIOCard;
	//**[12/17/2017 ConanSteve]**:  �������źŵƹر�
	if (nullptr != pIOCard)
	{
		pIOCard->ResetIOCard();
	}
	InitWindow();
}


void CDlgIOCard::OnBnClickedButtonOut0(UINT uID)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDHIOCardForSGInterior *pIOCard = (CDHIOCardForSGInterior *)m_pIOCard;
	if (nullptr != m_pIOCard)
	{
		int iOutport = uID - IDC_BUTTON_Out0;
		if (0 <= iOutport &&iOutport < 24)
		{
			pIOCard->m_arr_bOutportStatus[iOutport] = !pIOCard->m_arr_bOutportStatus[iOutport];
			if (pIOCard->m_arr_bOutportStatus[iOutport])//**[12/16/2017 ConanSteve]**:  ��
			{
				SetEvent(pIOCard->m_arr_OutputEventHandle[iOutport]);
				pIOCard->m_arr_iOutportCnt[iOutport]++;
				ResetEvent(pIOCard->m_arr_OutputEventHandle[iOutport]);
				((CStatic*)GetDlgItem(IDC_STATIC_BmpOut0 + iOutport))->SetBitmap(m_bmpON);
				//((CMFCButton*)GetDlgItem(uID))->SetFaceColor(RGB(255, 0, 0));
			}
			else//**[12/16/2017 ConanSteve]**:  �ر�
			{
				((CStatic*)GetDlgItem(IDC_STATIC_BmpOut0 + iOutport))->SetBitmap(m_bmpOFF);
				//((CMFCButton*)GetDlgItem(uID))->SetFaceColor(RGB(0, 0, 0));
			}
		}
	}
	
}


void CDlgIOCard::OnBnClickedButtonIn0(UINT uID)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDHIOCardForSGInterior *pIOCard = (CDHIOCardForSGInterior *)m_pIOCard;
	int iInportId = uID - IDC_BUTTON_In0;
	if (0 <= iInportId &&iInportId < 12 && nullptr != pIOCard)
	{
		pIOCard->TriggerInport(iInportId);
	}
}




void CDlgIOCard::OnBnClickedCheckActive0(UINT uID)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDHIOCardForSGInterior *pIOCard = (CDHIOCardForSGInterior *)m_pIOCard;
	int iInportId = uID - IDC_CHECK_Active0;
	if (nullptr != pIOCard)
	{
		pIOCard->m_arr_bEnableInternalActive[iInportId] = TRUE == ((CButton*)GetDlgItem(uID))->GetCheck();
	}
}


bool CDlgIOCard::InitWindow()
{

	CDHIOCardForSGInterior* pIOCard = (CDHIOCardForSGInterior*)m_pIOCard;
	m_listctrlQueueInfo.DeleteAllItems();
	m_listctrlQueueInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	for (int i = 0; i < pIOCard->m_iQueueNum; ++i)
	{
		m_listctrlQueueInfo.InsertItem(i, _T(""));
		m_listctrlQueueInfo.SetItemText(i, 0, CExFn::Int2CS(i));
		m_listctrlQueueInfo.SetItemText(i, 1, CExFn::Int2CS(pIOCard->m_arr_iActiveInterval[i]));
		m_listctrlQueueInfo.SetItemText(i, 2, _T("In") + CExFn::Int2CS(pIOCard->m_arr_iQueueInport[i]));
		m_listctrlQueueInfo.SetItemText(i, 3, _T("0"));
	}
	for (int i = 0; i < MAX_INPORT_NUM; ++i)
	{
		((CButton*)GetDlgItem(IDC_CHECK_Active0 + i))->SetCheck(pIOCard->m_arr_bEnableInternalActive[i] ? TRUE : FALSE);
	}
	for (int i = 0; i < MAX_OUTPORT_NUM; ++i)
	{
		((CStatic*)GetDlgItem(IDC_STATIC_BmpOut0 + i))->SetBitmap(m_bmpOFF);

	}
	UpdateData(FALSE);
	return false;
}

void CDlgIOCard::OnBnClickedButtonOpencfgfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDHIOCardForSGInterior *pIOCard = (CDHIOCardForSGInterior *)m_pIOCard;
	if (nullptr != pIOCard)
	{
		ShellExecute(NULL, _T("open"), pIOCard->m_InitInfo.lpszPathFileOfInit, NULL, NULL, SW_SHOWNORMAL);
	}
}

void CDlgIOCard::OnBnClickedButtonOpenconnectionfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDHIOCardForSGInterior *pIOCard = (CDHIOCardForSGInterior *)m_pIOCard;
	if (nullptr != pIOCard)
	{
		ShellExecute(NULL, _T("open"), pIOCard->m_InitInfo.lpszPathFileOfConnection, NULL, NULL, SW_SHOWNORMAL);
	}
}
