// Login.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ThreeTierRights.h"
#include "ThreeTierRightsInterior.h"
#include "DlgLogin.h"
#include "afxdialogex.h"
#include "resource.h"



// CLogin �Ի���

IMPLEMENT_DYNAMIC(CDlgLogin, CDialogEx)

CDlgLogin::CDlgLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_Login, pParent)
	, m_sUsername(_T(""))
	, m_sPassword(_T(""))
{

}

CDlgLogin::~CDlgLogin()
{
}

void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_UserName, m_sUsername);
	DDX_Text(pDX, IDC_EDIT_Password, m_sPassword);
}


BEGIN_MESSAGE_MAP(CDlgLogin, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgLogin::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_ShowPsw, &CDlgLogin::OnBnClickedCheckShowpsw)
	ON_BN_CLICKED(IDC_CHECK_ShowUserName, &CDlgLogin::OnBnClickedCheckShowusername)
END_MESSAGE_MAP()


// CLogin ��Ϣ�������


void CDlgLogin::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	CThreeTierRightsInterior * pThreeTierRightsInterior = (CThreeTierRightsInterior*)m_pThreeTierRightsInterior;
	if (nullptr != pThreeTierRightsInterior)
	{
		auto itera = pThreeTierRightsInterior->m_mapUserInfo.find(m_sUsername);
		if (itera == pThreeTierRightsInterior->m_mapUserInfo.end())
		{
			MessageBox(tr("�û��������ڣ����������룡"), tr("����"), MB_OK | MB_ICONWARNING);
			GetDlgItem(IDC_EDIT_UserName)->SetFocus();
			return;
		}
		else if (m_sPassword != itera->second.sPassword)
		{		
			MessageBox(tr("���벻��ȷ�����������룡"), tr("����"), MB_OK | MB_ICONWARNING);
			GetDlgItem(IDC_EDIT_Password)->SetFocus();
			return;
		}
		else if (!itera->second.bEnable)
		{
			MessageBox(tr("�û������ã�"), tr("����"), MB_OK | MB_ICONWARNING);
			GetDlgItem(IDC_EDIT_Password)->SetFocus();
			return;
		}
		else
		{
			pThreeTierRightsInterior->m_iCurUserType = itera->second.eUserType;
		}

	}
	
	CDialogEx::OnOK();
}


BOOL CDlgLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CThreeTierRightsInterior * pThreeTierRightsInterior = (CThreeTierRightsInterior*)m_pThreeTierRightsInterior;
	if (NULL != pThreeTierRightsInterior)
		m_sUsername = pThreeTierRightsInterior->m_sCurUsername;
	(GetDlgItem(IDC_EDIT_UserName))->SetFocus();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


bool CDlgLogin::SetCallPointer(void* pThreeTierRightsInterior)
{
	m_pThreeTierRightsInterior = pThreeTierRightsInterior;
	return false;
}


void CDlgLogin::OnBnClickedCheckShowpsw()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	((CEdit*)GetDlgItem(IDC_EDIT_Password))->SetPasswordChar(TRUE == ((CButton*)GetDlgItem(IDC_CHECK_ShowPsw))->GetCheck() ? 0 : _T('*'));
	(CEdit*)GetDlgItem(IDC_EDIT_Password)->SetFocus();
	UpdateData(FALSE);
}


void CDlgLogin::OnBnClickedCheckShowusername()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	((CEdit*)GetDlgItem(IDC_EDIT_UserName))->SetPasswordChar(TRUE == ((CButton*)GetDlgItem(IDC_CHECK_ShowUserName))->GetCheck() ? 0 : _T('*'));
	(CEdit*)GetDlgItem(IDC_EDIT_UserName)->SetFocus();
	UpdateData(FALSE);
}
