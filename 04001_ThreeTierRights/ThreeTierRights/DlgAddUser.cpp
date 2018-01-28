// DlgAddUser.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ThreeTierRights.h"
#include "DlgAddUser.h"
#include "ThreeTierRightsInterior.h"
#include "afxdialogex.h"
#include "resource.h"
#include "ExFn.h"


// CDlgAddUser �Ի���

IMPLEMENT_DYNAMIC(CDlgAddUser, CDialogEx)

CDlgAddUser::CDlgAddUser(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_AddUser, pParent)
	, m_iUserType(0)
	, m_sUsername(_T(""))
	, m_sPassword1(_T(""))
	, m_sPassword2(_T(""))
{

}

CDlgAddUser::~CDlgAddUser()
{
}

void CDlgAddUser::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_UserType, m_iUserType);
	DDX_Text(pDX, IDC_EDIT_UserName, m_sUsername);
	DDX_Text(pDX, IDC_EDIT_Password1, m_sPassword1);
	DDX_Text(pDX, IDC_EDIT_Password2, m_sPassword2);
	DDX_Control(pDX, IDC_COMBO_UserType, m_comboUserType);
}


BEGIN_MESSAGE_MAP(CDlgAddUser, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgAddUser::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgAddUser::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgAddUser ��Ϣ�������


BOOL CDlgAddUser::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_comboUserType.Clear();
	m_comboUserType.AddString(tr("����Ա"));
	m_comboUserType.AddString(tr("����Ա"));
	m_comboUserType.AddString(tr("����Ա"));
	m_comboUserType.SetCurSel(0);
	GetDlgItem(IDC_EDIT_UserName)->SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgAddUser::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if (_T("") == m_sUsername)
	{
		MessageBox(tr("�û�������Ϊ�գ����������룡"), tr("����"), MB_OK|MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_UserName)->SetFocus();
		return;
	}
	//int iRet = _ttoi(m_sUsername.GetAt(0));
// 	if (0 < iRet)
// 	{
// 		AfxMessageBox(tr("�û�����һ���ַ�����Ϊ���֣����������룡"));
// 		GetDlgItem(IDC_EDIT_UserName)->SetFocus();
// 		return;
// 	}
	CThreeTierRightsInterior* pThreeTierRightsInterior = (CThreeTierRightsInterior*)m_pThreeTierRightsInterior;//**[11/15/2017 ConanSteve]**:  ����û����Ƿ�Ϊ��
	if (nullptr == pThreeTierRightsInterior)
	{
		CDialogEx::OnOK();
		return;
	}
	auto itera_find = pThreeTierRightsInterior->m_mapUserInfo.find(m_sUsername);
	if (pThreeTierRightsInterior->m_mapUserInfo.end() != itera_find)//**[11/15/2017 ConanSteve]**:  �û����Ѵ���
	{
		MessageBox(tr("��ǰ�û����Ѿ����ڣ������������û�����"), tr("����"), MB_OK | MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_UserName)->SetFocus();
		return;
	}
	/*************************************<��������Ƿ��ظ�>************************************[11/15/2017 ConanSteve]*/
// 	if (_T("") == m_sPassword1)
// 	{
// 		MessageBox(tr("����1����Ϊ�գ����������룡"), tr("����"), MB_OK | MB_ICONWARNING);
// 		GetDlgItem(IDC_EDIT_Password1)->SetFocus();
// 		return;
// 	}
// 	if (_T("") == m_sPassword2)
// 	{
// 		MessageBox(tr("����2����Ϊ�գ����������룡"), tr("����"), MB_OK | MB_ICONWARNING);
// 		GetDlgItem(IDC_EDIT_Password2)->SetFocus();
// 		return;
// 	}
	if (m_sPassword1 != m_sPassword2)
	{
		MessageBox(tr("���벻��ͬ�����������룡"), tr("����"), MB_OK | MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_Password2)->SetFocus();
		return;
	}

	pThreeTierRightsInterior->m_mapUserInfo.insert(make_pair(m_sUsername, CUserInfo(m_sUsername, m_sPassword1, e_UserType(m_iUserType + 1))));
// 	pThreeTierRightsInterior->m_xml.AddChildElem(m_sUsername);
// 	pThreeTierRightsInterior->m_xml.AddChildAttrib(_T("password"), m_sPassword1);
// 	pThreeTierRightsInterior->m_xml.AddChildAttrib(_T("userType"), m_iUserType + 1);
	MessageBox(tr("����û����ɹ���"), tr("֪ͨ"), MB_OK | MB_ICONINFORMATION);
	CDialogEx::OnOK();
}


void CDlgAddUser::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
