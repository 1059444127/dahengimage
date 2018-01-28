// DlgIOCardForRS232_XZXNYN.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DHIOCardForRS232_XZXNYNInterior.h"
#include "DlgIOCardForRS232_XZXNYN.h"
#include "afxdialogex.h"
#include "resource.h"


// CDlgIOCardForRS232_XZXNYN �Ի���

IMPLEMENT_DYNAMIC(CDlgIOCardForRS232_XZXNYN, CDialogEx)

CDlgIOCardForRS232_XZXNYN::CDlgIOCardForRS232_XZXNYN(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_RS232_XZXNYN, pParent)
{

}

CDlgIOCardForRS232_XZXNYN::~CDlgIOCardForRS232_XZXNYN()
{
}

void CDlgIOCardForRS232_XZXNYN::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COM, m_comboComPort);
	DDX_Control(pDX, IDC_COMBO_BoardId, m_comboBoardId);
}


BEGIN_MESSAGE_MAP(CDlgIOCardForRS232_XZXNYN, CDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_Ctrl1, IDC_BUTTON_Ctrl8, &CDlgIOCardForRS232_XZXNYN::OnBnClickedButtonCtrl1)
	ON_BN_CLICKED(IDC_BUTTON_Switch, &CDlgIOCardForRS232_XZXNYN::OnBnClickedButtonSwitch)
END_MESSAGE_MAP()


// CDlgIOCardForRS232_XZXNYN ��Ϣ�������




BOOL CDlgIOCardForRS232_XZXNYN::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString sTemp{ _T("") };
	for (int i = 0; i < 16; ++i)
	{
		sTemp.Format(_T("COM%d"), i + 1);
		m_comboComPort.AddString(sTemp);
	}
	for (int i = 0; i < 16; ++i)
	{
		sTemp.Format(_T("ģ��%d"), i + 1);
		m_comboBoardId.AddString(sTemp);
	}
	InitWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


bool CDlgIOCardForRS232_XZXNYN::InitWindow()
{
	CDHIOCardForRS232_XZXNYNInterior* pIOCard = (CDHIOCardForRS232_XZXNYNInterior*)m_pIOcard;
	if (nullptr != pIOCard)
	{
		m_comboComPort.SetCurSel(pIOCard->m_iPort - 1);
		m_comboBoardId.SetCurSel(pIOCard->m_iBoardId - 1);
		m_comboComPort.EnableWindow(!pIOCard->m_bOpenCardSuccess);
		m_comboBoardId.EnableWindow(!pIOCard->m_bOpenCardSuccess);
		GetDlgItem(IDC_EDIT_FrameDelay)->EnableWindow(!pIOCard->m_bOpenCardSuccess);
		SetDlgItemInt(IDC_EDIT_FrameDelay, pIOCard->m_iFrameDelayOfKick);
// 		for (int i = 0; i < c_CoilNum; ++i)
// 		{
// 			GetDlgItem(IDC_BUTTON_Ctrl1 + i)->EnableWindow(!pIOCard->m_bOpenCardSuccess);
// 		}
		if (pIOCard->m_bOpenCardSuccess)
		{
			SetDlgItemText(IDC_BUTTON_Switch, _T("�رմ���"));
		}
		else
		{
			SetDlgItemText(IDC_BUTTON_Switch, _T("�򿪴���"));
		}
	}
	UpdateData(FALSE);
	return false;
}


bool CDlgIOCardForRS232_XZXNYN::SetIOCardPoint(CDHIOCard* pIOCard)
{
	m_pIOcard = pIOCard;
	return true;
}


void CDlgIOCardForRS232_XZXNYN::OnBnClickedButtonCtrl1(UINT uID)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iCtrlId = uID - IDC_BUTTON_Ctrl1;
	CDHIOCardForRS232_XZXNYNInterior* pIOCard = (CDHIOCardForRS232_XZXNYNInterior*)m_pIOcard;
	if (0 <= iCtrlId&&iCtrlId < c_CoilNum &&nullptr != pIOCard)
	{
		pIOCard->SwitchState(iCtrlId);
	}
}


void CDlgIOCardForRS232_XZXNYN::OnBnClickedButtonSwitch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	CDHIOCardForRS232_XZXNYNInterior* pIOCard = (CDHIOCardForRS232_XZXNYNInterior*)m_pIOcard;
	if (nullptr != pIOCard)
	{
		if (!pIOCard->m_bOpenCardSuccess)
		{
			pIOCard->m_iPort = m_comboComPort.GetCurSel() + 1;
			pIOCard->m_iBoardId = m_comboBoardId.GetCurSel() + 1;
			pIOCard->m_iFrameDelayOfKick = GetDlgItemInt(IDC_EDIT_FrameDelay);
			pIOCard->CloseCard();
			if (pIOCard->OpenCard())
			{
				AfxMessageBox(_T("�򿪴��ڳɹ���"));
			}
			else
			{
				AfxMessageBox(_T("�򿪴���ʧ�ܣ�"));
			}
		}
		else
		{
			pIOCard->CloseCard();
		}
	}
	InitWindow();
}
