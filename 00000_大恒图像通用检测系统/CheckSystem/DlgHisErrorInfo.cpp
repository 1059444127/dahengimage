// DlgHisErrorInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CheckSystem.h"
#include "MainFrm.h"
#include "DlgHisErrorInfo.h"
#include "afxdialogex.h"
#include "ExFn.h"


// CDlgHisErrorInfo �Ի���

IMPLEMENT_DYNAMIC(CDlgHisErrorInfo, CDialogEx)

CDlgHisErrorInfo::CDlgHisErrorInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_HisErrorInfo, pParent)
{

}

CDlgHisErrorInfo::~CDlgHisErrorInfo()
{
}

void CDlgHisErrorInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CheckGourp, m_comboCheckGroup);
	DDX_Control(pDX, IDC_LIST_DefectStatistics, m_listctrlDefectStatistics);
}


BEGIN_MESSAGE_MAP(CDlgHisErrorInfo, CDialogEx)
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_COMBO_CheckGourp, &CDlgHisErrorInfo::OnCbnSelchangeComboCheckgourp)
END_MESSAGE_MAP()


// CDlgHisErrorInfo ��Ϣ�������


BOOL CDlgHisErrorInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CINICfg iniAlgCfg(CExFn::GetCurrentAppPath() + _T("\\Config\\AlgConfig.ini"));
	CString sModuleName{ _T("") };
	sModuleName = iniAlgCfg.GetStringEx(_T("Demo Controls"), _T("DefaultModel"), sModuleName);
	CINICfg iniCheckSysCfg(CExFn::GetCurrentAppPath() + _T("\\ModelInfo\\") + sModuleName +_T("\\CheckSystemConfig.ini"));
	int iCheckGroupNum = iniCheckSysCfg.GetIntEx(_T("Inspection Parameters"), _T("CheckNum"));


	m_comboCheckGroup.AddString(tr("ȫ��λ"));
	for (int i = 0; i < iCheckGroupNum; ++i)
	{
		CString sGroupName{ _T("") };
		sGroupName.Format(tr("��λ%d"), i + 1);
		m_comboCheckGroup.AddString(sGroupName);
	}
	m_comboCheckGroup.SetCurSel(0);
	CLayout layout(CExFn::GetCurrentAppPath()+_T("\\Config\\LayoutRunDlg.xml"), CRect(0,0,0,0));
	int iWidthOfDefectDesc = layout.GetAttrib(_T("HistoryErrorInfo"), _T("widthOfDefectDesc"), 100);//**[12/4/2017 ConanSteve]**:  ȱ���������п��
	int iWidthOfDefectCnt = layout.GetAttrib(_T("HistoryErrorInfo"), _T("widthOfDefectCount"), 60);//**[12/4/2017 ConanSteve]**:  ȱ�ݼ������п��
	m_listctrlDefectStatistics.InsertColumn(0, tr("ȱ������"), LVCFMT_LEFT, iWidthOfDefectDesc);
	m_listctrlDefectStatistics.InsertColumn(1, tr("ȱ�ݼ���"), LVCFMT_LEFT, iWidthOfDefectCnt);
	m_listctrlDefectStatistics.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//UpdateDefectList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


int CDlgHisErrorInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	
	return 0;
}


bool CDlgHisErrorInfo::UpdateDefectList()
{
	UpdateData();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	m_listctrlDefectStatistics.DeleteAllItems();
	m_iIndexOfCheckGroupSelected = m_comboCheckGroup.GetCurSel();
	if(0== m_iIndexOfCheckGroupSelected)
	{
		for (int iCheckGroupSN = 0; iCheckGroupSN < pFrame->m_iCheckGroupNum; ++iCheckGroupSN)
		{
			map<int, s_DefectInfo> mapDefect = pFrame->m_arr_mapDefectStatistics[iCheckGroupSN];//**[12/1/2017 ConanSteve]**:  ���Ǹ�map
			int i = 0;
			for (auto itera = mapDefect.begin(); itera != mapDefect.end(); ++itera)
			{
				m_listctrlDefectStatistics.InsertItem(i, _T(""));
				m_listctrlDefectStatistics.SetItemText(i, 0, itera->second.sDesc);
				CString sCnt{ _T("") };
				sCnt.Format(_T("%d"), itera->second.iCnt);
				m_listctrlDefectStatistics.SetItemText(i, 1, sCnt);
				++i;
			}
		}
	}
	else if(m_iIndexOfCheckGroupSelected>0 &&m_iIndexOfCheckGroupSelected<= pFrame->m_iCheckGroupNum)
	{
		map<int, s_DefectInfo> mapDefect = pFrame->m_arr_mapDefectStatistics[m_iIndexOfCheckGroupSelected - 1];//**[12/1/2017 ConanSteve]**:  ���Ǹ�map
		int i = 0;
		for (auto itera = mapDefect.begin(); itera != mapDefect.end(); ++itera)
		{
			m_listctrlDefectStatistics.InsertItem(i, _T(""));
			m_listctrlDefectStatistics.SetItemText(i, 0, itera->second.sDesc);
			CString sCnt{ _T("") };
			sCnt.Format(_T("%d"), itera->second.iCnt);
			m_listctrlDefectStatistics.SetItemText(i, 1, sCnt);
			++i;
		}
	}
	UpdateData(FALSE);
	return false;
}


void CDlgHisErrorInfo::OnCbnSelchangeComboCheckgourp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateDefectList();
}
