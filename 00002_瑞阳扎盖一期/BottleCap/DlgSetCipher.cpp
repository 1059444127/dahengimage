// SetCipherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSetCipher.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetCipher dialog


CDlgSetCipher::CDlgSetCipher(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetCipher::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetCipher)
	m_new1 = _T("");
	m_new2 = _T("");
	m_old = _T("");
	//}}AFX_DATA_INIT
}


void CDlgSetCipher::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetCipher)
	DDX_Text(pDX, IDC_CIPHER_NEW1, m_new1);
	DDV_MaxChars(pDX, m_new1, 6);
	DDX_Text(pDX, IDC_CIPHER_NEW2, m_new2);
	DDV_MaxChars(pDX, m_new2, 6);
	DDX_Text(pDX, IDC_CIPHER_OLD, m_old);
	DDV_MaxChars(pDX, m_old, 6);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetCipher, CDialog)
	//{{AFX_MSG_MAP(CDlgSetCipher)
	ON_BN_CLICKED(IDC_BTNSETCIPHER, OnBtnsetcipher)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetCipher message handlers
/********************************************************************
	created:	2001/09/06
	created:	6:9:2001   21:07
	author:		lfl
	Param:          
	Return:         
	
	purpose:	�޸��û�����,���������ϵͳ��һЩ�߼�
	           ���ö��趨��,����ֻ֤Ҫ����Ȩ�޵��û������޸���Щ
			   ��������
	modify:        
*********************************************************************/

void CDlgSetCipher::OnBtnsetcipher() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	char chTemp[6];
	CString strOldCipher;
	CString str;
	CString strCipher;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
 	strCipher.Format("%s",pFrame->m_sCipherPath1);

	GetPrivateProfileString("cipher","1","daheng",chTemp,7,strCipher);
	strOldCipher=chTemp;
	if(m_old.GetLength()!=6 || m_old!=strOldCipher)
	{
		AfxMessageBox("�����������,����������!!");
		return;
// 		goto END_ERR;
	}
	if(m_new2.GetLength()!=6 || m_new1.GetLength()!=6)
	{
		AfxMessageBox("�����������6λ!!");
// 		goto END_ERR;
		return;
	}
	if(m_new2==m_old || m_new1==m_old)
	{
		AfxMessageBox("���������������ͬ!!");
		goto END_ERR;
	}
	if (m_new2!=m_new1) 
	{
		AfxMessageBox("�������������ͬ!!");
		goto END_ERR;
	}
//	WritePrivateProfileString("cipher","1",m_new2,"cipher.cip");
	WritePrivateProfileString("cipher","1",m_new2,strCipher);
	
	str.Format("�����޸ĳɹ�!\n������Ϊ%s",m_new2);
	AfxMessageBox(str);
END_ERR:
	m_new2 = "";
	m_new1 = "";
	m_old = "";
	UpdateData(false);

	CDialog::OnOK();
}
