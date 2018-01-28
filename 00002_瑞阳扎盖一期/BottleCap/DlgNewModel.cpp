// DlgNewModel.cpp : implementation file
//

#include "stdafx.h"
#include "BottleCap.h"
#include "DlgNewModel.h"
#include "MainFrm.h"
#include "BottleCapVIEW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainFrame*			g_pFrame; //ȫ��ָ��
extern CBottleCapView*     g_pView;

/////////////////////////////////////////////////////////////////////////////
// CDlgNewModel dialog


CDlgNewModel::CDlgNewModel(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNewModel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNewModel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgNewModel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNewModel)
	DDX_Control(pDX, IDC_COMBO_SPEC, m_cmbSpec);
	DDX_Control(pDX, IDC_COMBO_PERCENT, m_cmbPercent);
	DDX_Control(pDX, IDC_COMBO_NAME, m_cmbName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNewModel, CDialog)
	//{{AFX_MSG_MAP(CDlgNewModel)
	ON_BN_CLICKED(IDC_RADIO_BODY_SHAPE_1, OnRadioBodyShape1)
	ON_BN_CLICKED(IDC_RADIO_BODY_SHAPE_2, OnRadioBodyShape2)
	ON_BN_CLICKED(IDC_RADIO_HEAD_SHAPE_1, OnRadioHeadShape1)
	ON_BN_CLICKED(IDC_RADIO_HEAD_SHAPE_2, OnRadioHeadShape2)
	ON_CBN_SELCHANGE(IDC_COMBO_NAME, OnSelchangeComboName)
	ON_CBN_SELCHANGE(IDC_COMBO_PERCENT, OnSelchangeComboPercent)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEC, OnSelchangeComboSpec)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNewModel message handlers


void CDlgNewModel::OnOK() 
{
	// TODO: Add extra validation here
	
	int i1=0,i2=0;
	int j1=0,j2=0;

	// ��ȡƿ����״
	i1 = ((CButton*)GetDlgItem(IDC_RADIO_BODY_SHAPE_1))->GetCheck();
	i2 = ((CButton*)GetDlgItem(IDC_RADIO_BODY_SHAPE_2))->GetCheck();


	// ��ȡƿͷ��״
	j1 = ((CButton*)GetDlgItem(IDC_RADIO_HEAD_SHAPE_1))->GetCheck();
	j2 = ((CButton*)GetDlgItem(IDC_RADIO_HEAD_SHAPE_2))->GetCheck();


	if(0==i1 && 0==i2)
	{
		AfxMessageBox("��ѡ��ƿ������");
		return;
	}

	if(0==j1 && 0==j2)
	{
		AfxMessageBox("��ѡ��ƿͷ����");
		return;
	}


	// �鿴�Ƿ�����������
	int i=0,j=0;
	BOOL bChange=TRUE;
	CString strTemp1,strTemp2;

	// ��Ʒ����
	m_iLabelType = (m_iNameIndex+1)*100+(m_iSpecIndex+1)*10+(m_iPercentIndex+1);

	// ��ȡ��ǩ����
	if(0==m_iNameIndex)
	{
		m_sModelName.Format("%s","������ע��Һ");
	}
	else if(1==m_iNameIndex)
	{
		m_sModelName.Format("%s","�Ȼ���ע��Һ");
	}
	else if(2==m_iNameIndex)
	{
		m_sModelName.Format("%s","�������ָ�ע��Һ");
	}
	else if(3==m_iNameIndex)
	{
		m_sModelName.Format("%s","�������Ȼ���ע��Һ");
	}
	else if(4==m_iNameIndex)
	{
		m_sModelName.Format("%s","��¶��ע��Һ");
	}


	//��ȡ������
	if(0==m_iSpecIndex)
	{
		m_sModelName+="50";
	}
	else if(1==m_iSpecIndex)
	{
		m_sModelName+="250";
	}
	else if(2==m_iSpecIndex)
	{
		m_sModelName+="500";
	}
	else if(3==m_iSpecIndex)
	{
		m_sModelName+="1000";
	}
	else if(4==m_iSpecIndex)
	{
		m_sModelName+="150";
	}

	strTemp2 = "�ްٷֱ�";
	m_cmbPercent.AddString(strTemp2);
	strTemp2 = "0.9%";
	m_cmbPercent.AddString(strTemp2);
	strTemp2 = "5%";
	m_cmbPercent.AddString(strTemp2);
	strTemp2 = "10%";
	m_cmbPercent.AddString(strTemp2);

	// ��Ӱٷֱ�
	if(0==m_iPercentIndex)
	{
		m_sModelName+="";
	}
	else if(1==m_iPercentIndex)
	{
		m_sModelName+="09";
	}
	else if(2==m_iPercentIndex)
	{
		m_sModelName+="5";
	}
	else if(3==m_iPercentIndex)
	{
		m_sModelName+="10";
	}

	// ƿ������
	if(100==m_iHeadShape)
	{
		m_sModelName+="����";
	}
	else if(101==m_iHeadShape)
	{
		m_sModelName+="˫��";
	}

	//ƿ������ ��ƿ�Ϳ�����
	if(100==m_iBodyShape)
	{
		m_sModelName+="��ƿ";
	}
	else if(101==m_iBodyShape)
	{
		m_sModelName+="������";
	}


	CDialog::OnOK();
}

void CDlgNewModel::OnRadioBodyShape1() 
{
	// TODO: Add your control notification handler code here
	m_iBodyShape = 100;
}

void CDlgNewModel::OnRadioBodyShape2() 
{
	// TODO: Add your control notification handler code here
	m_iBodyShape = 101;
}

void CDlgNewModel::OnRadioHeadShape1() 
{
	// TODO: Add your control notification handler code here
	m_iHeadShape = 100;
}

void CDlgNewModel::OnRadioHeadShape2() 
{
	// TODO: Add your control notification handler code here
	m_iHeadShape = 101;
}

BOOL CDlgNewModel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int i=0,j=0;

	CString strTitle,strTemp1,strTemp2;
// 	strTitle.Format("�½�ģ��--%s",m_sModelName);
	strTitle.Format("�½�ģ��");
	SetWindowText(strTitle);

	// ���ز�Ʒ����
	m_cmbName.ResetContent();
	strTemp2 = "������ע��Һ";
	m_cmbName.AddString(strTemp2);
	strTemp2 = "�Ȼ���ע��Һ";
	m_cmbName.AddString(strTemp2);
	strTemp2 = "�������ָ�ע��Һ";
	m_cmbName.AddString(strTemp2);
	strTemp2 = "�������Ȼ���ע��Һ";
	m_cmbName.AddString(strTemp2);
	strTemp2 = "��¶��ע��Һ";
	m_cmbName.AddString(strTemp2);


	// ���ز�Ʒ�������
	m_cmbSpec.ResetContent();
	strTemp2 = "50ml";
	m_cmbSpec.AddString(strTemp2);
	strTemp2 = "150ml";
	m_cmbSpec.AddString(strTemp2);
	strTemp2 = "250ml";
	m_cmbSpec.AddString(strTemp2);
	strTemp2 = "500ml";
	m_cmbSpec.AddString(strTemp2);
	strTemp2 = "1000ml";
	m_cmbSpec.AddString(strTemp2);

	

	// ���ذٷֱ���Ϣ
	m_cmbPercent.ResetContent();

	strTemp2 = "�ްٷֱ�";
	m_cmbPercent.AddString(strTemp2);
	strTemp2 = "0.9%";
	m_cmbPercent.AddString(strTemp2);
	strTemp2 = "5%";
	m_cmbPercent.AddString(strTemp2);
	strTemp2 = "10%";
	m_cmbPercent.AddString(strTemp2);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// ����
void CDlgNewModel::OnSelchangeComboName() 
{
	// TODO: Add your control notification handler code here
	m_iNameIndex = m_cmbName.GetCurSel();
}

// �ٷֱ�
void CDlgNewModel::OnSelchangeComboPercent() 
{
	// TODO: Add your control notification handler code here
	m_iPercentIndex = m_cmbPercent.GetCurSel();
}

// ���
void CDlgNewModel::OnSelchangeComboSpec() 
{
	// TODO: Add your control notification handler code here

	int iSpec=0;
	iSpec = m_cmbSpec.GetCurSel();

	if(0==iSpec)
	{
		m_iSpecIndex = 0;	//50ml
	}
	else if(1==iSpec)
	{
		m_iSpecIndex = 4;  //150ml
	}
	else if(2==iSpec)
	{
		m_iSpecIndex = 1;   //250ml
	}
	else if(3==iSpec)
	{
		m_iSpecIndex = 2;  //500ml
	}
	else if(4==iSpec)
	{
		m_iSpecIndex = 3;  //1000ml
	}
	
}
