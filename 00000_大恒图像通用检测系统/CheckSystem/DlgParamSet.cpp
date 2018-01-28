// DlgParamSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CheckSystem.h"
#include "DlgParamSet.h"
#include "afxdialogex.h"
#include "resource.h"
#include "MainFrm.h"
#include "CheckSystemDoc.h"
#include "CheckSystemView.h"


// CDlgParamSet �Ի���

IMPLEMENT_DYNAMIC(CDlgParamSet, CDialogEx)

CDlgParamSet::CDlgParamSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ParamSet, pParent)
{

}

CDlgParamSet::~CDlgParamSet()
{
}

void CDlgParamSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgParamSet, CDialogEx)
	ON_MESSAGE(DHPG_MSG_PROPERTY_CHANGED, OnDHPropGrid_DataChanged)
	ON_MESSAGE(DHPG_MSG_BTN_CLICKED, OnDHPropGrid_ButtonClicked)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDlgParamSet::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDOK, &CDlgParamSet::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgParamSet ��Ϣ�������


void CDlgParamSet::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: �ڴ˴������Ϣ����������
}


void CDlgParamSet::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CRect rect;
	GetClientRect(rect);
	CBrush brush;
	brush.CreateSolidBrush(RGB(255, 255, 255));
	dc.FillRect(&rect, &brush);
	brush.DeleteObject();

	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(rect);
	ScreenToClient(rect);
	m_gridParamSet.MoveWindow(rect);

}


bool CDlgParamSet::InstallPropertyGrid()
{
	CRect rect;
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(rect);
	ScreenToClient(rect);
	m_gridParamSet.InstallControl(this, rect, "SystemParam", 1, 1,true);
	m_gridParamSet.DisplayCtrl(SW_SHOW);
	UpdateData(FALSE);
	m_gridParamSet.ReadModel();
	return false;
}


BOOL CDlgParamSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InstallPropertyGrid();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCheckSystemView *pView = (CCheckSystemView *)pFrame->GetActiveView();
	m_p_bEnableDebugCam = make_unique<bool[]>(pFrame->m_iGrabberNum);
	m_gridParamSet.EnableCtrl(0, 0, 3, pFrame->m_threeTierRights.VerifyRights(e_UserType_SuperAdmin));//**[1/16/2018 ConanSteve]**:	���Ĵ�ͼ����ʽ��ֻ�г�������Ա���޸� 
// 	for (int i = 0; i < IDC_BUTTON_END - IDC_BUTTON_BEGIN; ++i)
// 	{
// 		bool bShow = false;
// 		switch (i+IDC_BUTTON_BEGIN)
// 		{
// 		case IDC_BUTTON_RECORD://**[1/16/2018 ConanSteve]**:	�ٷְٲ���ʾ��ť 
// 			bShow = false;
// 			break;
// 		default:
// 			bShow = pFrame->m_threeTierRights.VerifyRights(pView->m_pdlgRunning->GetDlgItem(IDC_BUTTON_BEGIN + i));
// 			break;
// 		} 
// 		m_gridParamSet.EnableCtrl(1, 0, i, bShow);
// 	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

LRESULT CDlgParamSet::OnDHPropGrid_ButtonClicked(WPARAM wParam, LPARAM lParam)
{
	P_DHPROGMSGSTRUCT2 pDMS = (P_DHPROGMSGSTRUCT2)lParam;
	e_Grid1DllID nDllIndex1 = (e_Grid1DllID)pDMS->nDllIndex;
	int nGroupIndex2 = pDMS->nGroupIndex;
	int n3ID = pDMS->nID;
	TCHAR chGroupName[256], chValue[256];
#ifdef UNICODE
	_tcscpy_s(chGroupName, CStringConvert::ANSI_UCS2(pDMS->chGroupName).c_str());
	_tcscpy_s(chValue, CStringConvert::ANSI_UCS2(pDMS->chValue).c_str());
#else
	_tcscpy_s(chGroupName, pDMS->chGroupName);
	_tcscpy_s(chValue, pDMS->chValue);
#endif
	//UpdateData(TRUE);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCheckSystemView * pView = (CCheckSystemView *)pFrame->GetActiveView();
	e_Grid2SysCfg e2SysCfgType = (e_Grid2SysCfg)nGroupIndex2;
	switch (nDllIndex1)
	{
	case e_Grid1DllID::SystemConfig:
	{
		e_Grid2SysCfg e2SysCfgType = (e_Grid2SysCfg)nGroupIndex2;
		switch (e2SysCfgType)
		{
		case e_Grid2SysCfg::SaveImage:
		{
			switch (n3ID)
			{
			case 0://**[11/23/2017 ConanSteve]**:�����ʷͼ��
			{
				if (IDYES == MessageBox(tr("ȷ�������ʷͼ����"), _T("��������"), MB_YESNO|MB_ICONQUESTION))
				{
					CString sPathOfSaveImg{_T("")};
					sPathOfSaveImg.Format(_T("%s\\SaveImages"), pFrame->m_sPathOfVirtualDisk);
					CExFn::DeleteDirectory(sPathOfSaveImg, _T("*.*"), true);
				}
			}
				break;
			default:
				break;
			}
			break;
		}
		default:
			break;
		}
		break;
	}
	case e_Grid1DllID::RunControl:
	{
		e_Grid2RunCtrl e2RunCtrl = (e_Grid2RunCtrl)nGroupIndex2;
		switch (e2RunCtrl)
		{
		case e_Grid2RunCtrl::RunCtrl:
		{     
			int iButtonID = n3ID + IDC_BUTTON_RESET_SYSTEM; //**[12/27/2017 ConanSteve]**:  �õ���ť�ؼ���ID
			pView->m_pdlgRunning->OnButtonClick(iButtonID);
			break;
		}
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
	//UpdateData(FALSE);
	return true;
}

LRESULT CDlgParamSet::OnDHPropGrid_DataChanged(WPARAM wParam, LPARAM lParam)
{
	//**[11/30/2017 ConanSteve]**:  �˺���ֻ������ParamList���޸Ĳ���Ҫ������Ч����ز���
	//**[11/23/2017 ConanSteve]**:  ���������������ֱ����˱����������������ֱ���1����2����3��
	P_DHPROGMSGSTRUCT2 pDMS = (P_DHPROGMSGSTRUCT2)lParam;
	e_Grid1DllID nDllIndex1 = (e_Grid1DllID)pDMS->nDllIndex;
	int n3ID = pDMS->nID;
	int n2GroupIndex = pDMS->nGroupIndex;
	TCHAR chGroupName[256], chValue[256];
#ifdef UNICODE
	_tcscpy_s(chGroupName, CStringConvert::ANSI_UCS2(pDMS->chGroupName).c_str());
	_tcscpy_s(chValue, CStringConvert::ANSI_UCS2(pDMS->chValue).c_str());
#else
	_tcscpy_s(chGroupName, pDMS->chGroupName);
	_tcscpy_s(chValue, pDMS->chValue);
#endif
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	switch (nDllIndex1)
	{
	case e_Grid1DllID::SystemConfig:
	{
		e_Grid2SysCfg e2SysCfg = (e_Grid2SysCfg)n2GroupIndex;
		switch (e2SysCfg)
		{
		case e_Grid2SysCfg::SaveImage:
		{
			if (1 == n3ID)//**[11/23/2017 ConanSteve]**:  ��ͼ����
			{
				
				
			}
			break;
		default:
			break;
		}
		}
		break;
	}
	default:
		break;
	}
	//UpdateData(FALSE);
	return true;
}

BOOL CDlgParamSet::UpdateData(BOOL bSaveAndValidate)
{
	//TRUE: �����д�����
	//FALSE:����д�����
	//������
	//m_pPropGridCtrl->UpdatePara(0, 1, 1, DHPropGridData(&m_sModel_b.fMinScore), bSaveAndValidate);

	//m_pPropGridCtrlModel->UpdatePara(0, 1, 0, DHPropGridData(&m_nTestArea), bSaveAndValidate);//01
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	e_Grid1DllID iDllID1{ e_Grid1DllID::SystemConfig };
	int iGroupID2{ 0 };
	/*************************************<ϵͳ�����������>************************************[12/14/2017 ConanSteve]*/

	int k = 1;
	iDllID1 = e_Grid1DllID::SystemConfig;
	{
		iGroupID2 = 0;//**[9/13/2017 ConanSteve]**:  ��ͼ���
		m_gridParamSet.UpdatePara((int)iDllID1, iGroupID2, k++, DHPropGridData(&pFrame->m_iSaveImgType), bSaveAndValidate);
		m_gridParamSet.UpdatePara((int)iDllID1, iGroupID2, k++, DHPropGridData(&pFrame->m_bEnableCircleSaveImg), bSaveAndValidate);
		m_gridParamSet.UpdatePara((int)iDllID1, iGroupID2, k++, DHPropGridData(&pFrame->m_iSaveImgObj), bSaveAndValidate);
		m_gridParamSet.UpdatePara((int)iDllID1, iGroupID2, k++, DHPropGridData(&pFrame->m_iMaxNumberOfSaveImg), bSaveAndValidate);
		for (int i=0; i < pFrame->m_iCheckGroupNum; ++i)
		{
			m_gridParamSet.UpdatePara((int)iDllID1, iGroupID2, k++, DHPropGridData(&pFrame->m_arr_bEnableSaveImgOfEachCheckGroup[i]), bSaveAndValidate);
		}
	}
	/*************************************<���п���>************************************[12/14/2017 ConanSteve]*/
	iDllID1 = e_Grid1DllID::RunControl;
	iGroupID2 = 0;
	{
		//**[12/21/2017 ConanSteve]**:  ��������
		m_gridParamSet.UpdatePara((int)iDllID1, iGroupID2, 3, DHPropGridData(&pFrame->m_bEnableLog), bSaveAndValidate);//**[1/17/2018 ConanSteve]**:  ��־��¼
		//**[12/14/2017 ConanSteve]**:  �㷨��������
		++iGroupID2;
		k = 0;	
		m_gridParamSet.UpdatePara((int)iDllID1, iGroupID2, k++, DHPropGridData(&pFrame->m_iAlgDisplayMode), bSaveAndValidate);//**[12/14/2017 ConanSteve]**:  �㷨��ʾ��Ʒͼ������
		m_gridParamSet.UpdatePara((int)iDllID1, iGroupID2, k++, DHPropGridData(&pFrame->m_iDrawImgType), bSaveAndValidate);
		++iGroupID2;//**[1/10/2018 ConanSteve]**:  ������
		for (int i = 0; i < pFrame->m_iCheckGroupNum; ++i)
		{
			m_gridParamSet.UpdatePara((int)iDllID1, iGroupID2, i, DHPropGridData(&pFrame->m_arr_bEnableExecCheckOfEachGroup[i]), bSaveAndValidate);
		}

		++iGroupID2;//**[1/10/2018 ConanSteve]**:  �޷Ͽ���
		k = 0;
		m_gridParamSet.UpdatePara((int)iDllID1, iGroupID2, k++, DHPropGridData(&pFrame->m_bEnableEmptyKick), bSaveAndValidate);
		for (int i = 0; i < pFrame->m_iKickQueueNum; ++i)
		{
			m_gridParamSet.UpdatePara((int)iDllID1, iGroupID2, k++, DHPropGridData(&pFrame->m_arr_bEnableKickOfKickQueue[i]), bSaveAndValidate);
		}
	}
	return 0;
}

void CDlgParamSet::OnBnClickedButtonApply()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//**[11/30/2017 ConanSteve]**:  �˺���������ParamList���޸ĵ��ǲ���������Ч�Ĳ���
	int i = 0;
	UpdateData(TRUE);
	/*************************************<��ͼ���>************************************[11/23/2017 ConanSteve]*/
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (e_SaveImgType_NO != pFrame->m_iSaveImgType)
	{
		for (int i = 0; i < pFrame->m_iGrabberNum; ++i)
		{
			CString sPathOfSaveImg{ _T("") }, sNameOfImgTypeFolder{ _T("") };
			sPathOfSaveImg.Format(_T("%s\\SaveImages"), pFrame->m_sPathOfVirtualDisk);
			SYSTEMTIME sysTime;
			GetLocalTime(&sysTime);
			switch (pFrame->m_iSaveImgType)
			{
			case e_SaveImgType_NO:
				break;
			case e_SaveImgType_Consecutive:
			{
				sNameOfImgTypeFolder = _T("GrabImages");
			}
			break;
			case e_SaveImgType_Error:
			{
				sNameOfImgTypeFolder = _T("ErrorImages");
			}
			break;
			case e_SaveImgType_ProductExist:
			{
				sNameOfImgTypeFolder = _T("ModelImages");
			}
			break;
			default:
			{
				sNameOfImgTypeFolder = _T("GrabImages");
			}
			break;
			}
			_stprintf_s(pFrame->m_arr_lpszPathOfSaveImg[i],_T("%s\\%s\\%04d-%02d-%02d\\Camera%d"),
				sPathOfSaveImg,
				sNameOfImgTypeFolder,
				sysTime.wYear, sysTime.wMonth, sysTime.wDay,
				i + 1);
			if (FALSE == PathFileExists(pFrame->m_arr_lpszPathOfSaveImg[i]))
			{
				CExFn::CreateMultiDirectory(pFrame->m_arr_lpszPathOfSaveImg[i]);
				if (FALSE == PathFileExists(pFrame->m_arr_lpszPathOfSaveImg[i]))
				{
					CString sErrorInfo{ _T("") };
					sErrorInfo.Format(_T("������ͼ��%s��·��ʧ��"), pFrame->m_arr_lpszPathOfSaveImg[i]);
					pFrame->m_fileErrorInfo.Write(sErrorInfo);
				}
			}
			s_AlgModelInput sIn;
			sIn.iCheckSN = i;
			sIn.iOperSig = e_AlgModelInputOper_SaveImage;
			sIn.iReserve1 = pFrame->m_iSaveImgType;
			sIn.iReserve2 = pFrame->m_iMaxNumberOfSaveImg;
			sIn.iReserve3 = pFrame->m_iSaveImgObj;
			sIn.iReserve4 = 0;

			s_Status sStatus;
			if (pFrame->m_bEnableInitMultiObjectAndCheck)
			{
				sStatus = pFrame->m_multiObject.Set(sIn);
			}
			//**[11/23/2017 ConanSteve]**:  ����ϵͳÿ��λ��ͼ����
			pFrame->m_p_iCurNumOfSaveImgOfEachGrab[i] = 0;
		}
	}
	/*************************************<RS232���>************************************[11/23/2017 ConanSteve]*/
	m_gridParamSet.WriteModel();
}


void CDlgParamSet::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedButtonApply();
	CDialogEx::OnOK();
}
