// MainFrm.cpp : implementation of the CMainFrame class
//
#include "stdafx.h"
#include "TubeCheck.h"
#include "MainFrm.h"
#include "TubeCheckDoc.h"
#include "TubeCheckView.h"
#include <io.h>
#include <MATH.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainFrame*		g_pFrame; //ȫ��ָ��
extern CTubeCheckDoc*		g_pDoc;       
extern CTubeCheckView*	g_pView;
#define ID_MSG_ALARM 62000

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_COPYDATA()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_INDICATOR_GRAB,
	ID_INDICATOR_CHECK,
	ID_INDICATOR_ERROR,
	ID_INDICATOR_OPERATION,
// 	ID_SEPARATOR,           // status line indicator
// 	ID_INDICATOR_CAPS,
// 	ID_INDICATOR_NUM,
// 	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	CoInitialize(NULL);

	LARGE_INTEGER iLarge;
	QueryPerformanceFrequency(&iLarge);
	m_dFreq=(double)iLarge.QuadPart;
	fill(m_arr_iAlgStopMachineCtrl.begin(), m_arr_iAlgStopMachineCtrl.end(), 0);
	int i=0,j=0,k=0;
	fill(m_arr_iCurNumOfConstantKick.begin(), m_arr_iCurNumOfConstantKick.end(), 0);
	fill(m_arrKickTable2.begin(), m_arrKickTable2.end(), s_5CheckInfo());

	for(i=0;i<c_AllElementCount;i++)
	{
		m_iRC_ThreadBad[i]=0;						// ����߳��и������ź�ʱ��¼
		m_iRC_ThreadGood[i]=0;						// ����߳��и������ź�ʱ��¼
		m_iRC_Good[i]=0;							// д���źż�¼
		m_iRC_AddKickImage[i]=0;					// ��¼����ͼ���
		m_iRC_KickImage[i]=0;						// ��¼�޷�ͼ���
		m_iRC_ImageNum[i]=0;						// ��¼�ظ���¼
		m_iRC_Image[i]=0;							// ��¼�������̵߳�ͼ���
		m_iRC_BeforeD[i]=0;							// ��¼�㷨����֮ǰ��ͼ���
		m_iRC_Detect[i]=0;							// ��¼�㷨�����ͼ��ż����ʱ��
		m_iRC_Kick[i]=0;							// ��¼�޷�ͼ���
		m_iRC_Call[i]=0;							// ��¼�ص���ͼ��ż��ص����
		m_iRC_DetectTime[i]=0;						// ��¼�㷨���ʱ��
		m_iRC_RecordImage[i]=0;						// ��¼�߳���ͼ��ű仯ʱ��ʱ��
		m_iRC_SetEvent[i]=0;						// �������߳�
		m_iRC_WriteImage[i]=0;						// ��ȡͼ������[2014-12-5 zhy]
		m_iRC_StopMachine[i]=0;
		m_iRC_RecordKick[i]=0;

		m_iAbnormalImage[i]=0;
		m_iStopCountDiff[i]=0;
		m_iInspectionObj[i]=0;
		m_iSaveCameraImageCount[i]=0;	
		m_iSaveCameraImageCount_Setting[i]=0;		
		m_iSaveCircleCameraImageCount[i]=0;						

		m_iKickInterval[i]=0;									
		m_iGrabCountSingleCircle[i]=0;				
		m_iGrabCircleCount[i]=0;						
		m_iImageKickThread[i]=0;						
		m_iImageCheckThread[i]=0;

		m_iCheckErrorCount[i]=0;
		m_iImageCountOverFlowNumber[i]=0;		
		m_iFlowOverImageCountInPE[i]=0;				
		m_iKickFlowNumber[i]=0;						

		m_iHardImageCount[i]=0;
		m_iLastImageCount[i]=0;	
		m_iLastHardImageCount[i]=0;
		m_iImageCountInPE[i]=0;						
		m_iLastImageCountInPE[i]=0;		
		m_iHardKickCount[i]=0;						
		m_iLastKickCount[i]=0;							
		m_iLastImageID[i]=0;							
		m_iDefectTypePerWS[i]=0;
		m_iLabelErrorCount[i]=0;

		for(j=0;j<256;j++)
		{
			m_iCheckResultTable[i][j]=-1;
			m_bTubeCheckResult[i][j] = FALSE;
			m_bCheckResultOffset[i][j] = FALSE;
		}

		for(j=0;j<MAX_DEFECT_TYPE_COUNT;j++)
		{
			m_iDefectCount[i][j]=0;
		}

		m_bIsKick[i]=TRUE;							// ��·�Ƿ��߷�
		m_bSaveCameraImage[i]=FALSE;				// �Ƿ񱣴�ͼ��
		m_bDisplayImage[i]=TRUE;					// ������λ�Ƿ���ʾͼ��
		m_bDisplayAlgImage[i]=TRUE;					// ������λ�Ƿ���ʾ�㷨ͼ��
		m_bIsCheck[i]=TRUE;							// �Ƿ���
		m_bLabelAlarm[i]=FALSE;
		m_bAlarm_StopCount[i]=FALSE;				// ֹͣ��������

		m_dAlgCheckTime[i]=0.0;						// �㷨���ʱ��

		/*m_pImageAddr[i]=NULL;	*/			// ���ͼ������

	}

	for(i=0;i<CAMERA_COUNT;i++)
	{
		m_iCamSN[i]=0;
		m_iPicCount[i]=0;

		m_iOriginSaveImageNum[i]=0;
		m_bIsSimulateIniExist[i]=FALSE;				// ģ���ͼ�������ж�ģ��ɼ�ʱ�����ļ��Ƿ����
		m_bGrabFlip[i]=FALSE;						// �Ƿ���з�ת

		m_pbRGBImage[i]=NULL;						// ��ͼ�����任ͼ�񻺳���

		for(j=0;j<256;j++)
		{
			for(k=0;k<PIC_COUNT;k++)
			{
				m_bWriteImage[i][j][k]=FALSE;
			}
		}
	}

	for(i=0;i<c_AllElementCount;i++)
	{
		m_iRC_PE[i]=0;								// ��¼���
		m_iFrameCount[i]=0;
		m_iFrameCountOverFlowNumber[i]=0;
		m_iLastFrameCount[i]=0;
	}

	for(i=0;i<256;i++)
	{
		m_pLutR[i] = min((int)(i*0),255);
		m_pLutG[i] = min((int)(i*0),255);
		m_pLutB[i] = min((int)(i*0),255);
	}
	
	m_iSaveImage_Abnormal=0;
	m_iAlarmCardNum = 1;
	m_iAlarmOutPort = 3;
	m_iCheckErrorCountTotal=0;										// ÿһ�ο��쿪ʼ�����ڵļ������
	m_iCheckGoodCountTotal=0;										// ÿһ�ο��쿪ʼ�����ڵĺ�Ʒ����
	m_iCheckCountTotal=0;											// ÿһ�ο��쿪ʼ�����ڵĻ�Ʒ����
	m_iMaxCameraImageWidth=0;									// �ɼ�ͼ����
	m_iMaxCameraImageHeight=0;									// �ɼ�ͼ��߶�
	m_iMaxCameraImageSize=0;									// �ɼ�ͼ���С
	m_iMaxCameraImagePixelSize=0;								// �ɼ�ͼ�����ش�С
	m_iImageShowMode=0;											// ��ʾͼ��ģʽ 0:����ʾ; 1:��ʾ����; 2:����ʾ����
	m_iIntervalDisplayTime=0;									// ͼ����ʾ���ʱ��
	m_iMaxSaveImageCount=0;										// ÿ����λ����ͼ����
	m_iPreAllocateSaveInfoElementCount=0;						// Ԥ���䱣��ͼ����ϢԪ�ظ�����������ϢԪ�ء�һ�����ڲɼ��ĸ���
	m_iMaxGrabCountSingleCircle=0;								// ȫ����λ��һ�����ڵĲɼ�ͼ������ֵ
	m_iLastCheckSpeedCount=0;

	m_bAdvMode=FALSE;
	m_bReadModel=FALSE;											// �ж϶�дģ���Ƿ�ɹ�
	m_bStartRecord=FALSE;										// ���ƿ�ʼ��¼
	m_bIsDetecting=FALSE;										// �ж��Ƿ������ڼ��״̬
	m_bIOCardValid=FALSE;										// �ӿڿ��Ƿ���Ч
	m_bIOCardInitSuccess=FALSE;									// �ӿڿ��Ƿ��ʼ���ɹ�
	m_bCalAlg=TRUE;												// �Ƿ�����㷨���
	m_bSoftResetIOCard=TRUE;									// �Ƿ������λ�ӿڿ�
	m_bPauseSaveImageThread=FALSE;								// �Ƿ���ͣ����ͼ���߳�
	m_bKillSaveImageThread=FALSE;								// �Ƿ�ֹͣ����ͼ���߳�
	m_bCloseCount=FALSE;										// ���ڿ����Ƿ��ȡIO���е�ͼ�����
	m_bWaitDemoMould=FALSE;
	m_bStopReadKickImageNo=FALSE;
	m_bOpenAlgCheck = TRUE;
	
	m_dwCheckBadRatio=0.0;										// ��Ʒ��

	m_bFirstCheck=TRUE;
	m_iKickMode=3;
	m_iCamTestMode=0;
	m_iEmptyCount=0;
	m_iKickOffset5=0;

	// ��ʼֵ��Ϊ��Ʒ
	memset(&m_sOut5[0],0,sizeof(LabelCheck::s_AlgInspResult)*256);
	for (int i = 0; i < 6; ++i)
	{
		m_bOpenCameraAlgCheck[i] = TRUE;
	}
	m_iSaveImageSys=0;
	m_bHaveStartSaveThread = FALSE;

}

CMainFrame::~CMainFrame()
{
	int i = 0;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	//����״̬��
	m_wndStatusBar.SetPaneInfo(0, ID_INDICATOR_GRAB, SBPS_NORMAL, 1300);  //�軪��������
	m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_CHECK, SBPS_NORMAL, 300);
	m_wndStatusBar.SetPaneInfo(2, ID_INDICATOR_ERROR, SBPS_NORMAL, 300);
	//m_wndStatusBar.SetPaneInfo(3, ID_INDICATOR_OPERATION, SBPS_NORMAL, 200);
	for (int i = 0; i < 4; ++i)
	{
		m_wndStatusBar.SetPaneText(i, TEXT("Ready"), true);
	}


	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable

	// ���ر������Ͳ˵���
	ModifyStyle(WS_CAPTION,0);
	SetMenu(NULL);


	////////////////////////////////////////////////////////////////////////
	//���"��¼���ļ�·�����ļ�"���ļ�·��
	m_sWorkPath = GetCurrentAppPath();						// ����·��
	m_sWorkPath.Delete(m_sWorkPath.GetLength()-1, 1);		// ɾ������·�������Ǹ�'\'

	//��������ļ����ļ����Ƿ����
	if (!FindFilePathFunc()) 
	{
		AfxMessageBox("ϵͳ�����ļ�������!");
		return -1;
	}

	try
	{
		// ��ȡϵͳ�����ļ�
		if(!ReadSystemConfig(m_sSysConfigFileName))
		{
			AfxMessageBox("��ȡϵͳ�����ļ�ʧ��!");
			return -1;
		}
	}
	catch (...)
	{
		AfxMessageBox("��ȡϵͳ�����ļ��쳣!");
		return -1;
	}


#ifdef USE_IO_CARD
	
	//��ʼ������	
	try
	{	
		// ��ʼ���ӿڿ�
		if(m_bIOCardValid)
		{	
			if(!InitIOCard())
			{
				AfxMessageBox("��ʼ���ӿڿ�ʧ��!");
				return -1;
			}
			else
				m_bIOCardInitSuccess = TRUE;              // �ӿڿ��Ƿ��ʼ���ɹ�
		}
	}
	catch (...)
	{
		AfxMessageBox("��ʼ���ӿڿ��쳣!");
		return -1;
	}
	 	
 #endif

	try
	{
		// ��ʼ���ɼ�����
		if (!InitGrabber())
		{
			AfxMessageBox("��ʼ�����ʧ��!");
			return -1;
		}
	}
	catch (...)
	{
		AfxMessageBox("��ʼ������쳣!");
		return -1;
	}
	
	try
	{
		// ��ʼ���ڴ�������
		if (!InitDataFunc())
		{
			AfxMessageBox("��ʼ��������ʧ��!");
			return -1;
		}
	}
	catch (...)
	{
		AfxMessageBox("��ʼ���ڴ��������쳣!");
		return -1;
	}
	//////////////////////////////////////////////////////////////////////////

	//�����߳�
	if (!StartAllThread())
	{
		AfxMessageBox("��ʼ���߳�ʧ��!");
		return -1;
	}

	//[2012-5-9 by zhy]
	if(!LoadAllImage())
	{
		AfxMessageBox("����M��ͼ��ʧ��!");
		return -1;
	}

	// �������ݿ�[2012-11-27]
	if(!ADOConnectDB())
	{
		AfxMessageBox("�������ݿ�ʧ��!");
		return FALSE;
	}

	// �鿴�ܵ����ݱ��Ƿ����
	if(!ExistTable("All_Check_Info"))
	{
		// ������
		if(!CreateTable("All_Check_Info"))
		{
			AfxMessageBox("�������ݱ� All_Check_Info ʧ��!");
			return FALSE;
		}
	}

	CString strTemp,strMsg;
	strTemp.Format("%s","Defect_Statistic_Info");
	if(!ExistTable(strTemp))
	{
		if(!CreateTable(strTemp))
		{
			strMsg.Format("�������ݱ� %s ʧ��!",strTemp);
			AfxMessageBox(strMsg);
			return FALSE;
		}
	}
	
	SetWindowText(m_sWindowTitle);

	//�����ɼ���ʱ��
	SetTimer(StartGrabTimer, 100, NULL);
	
	SetTimer(IntervalDisplayTimerOne,m_iIntervalDisplayTime,NULL);    //���1�����ʾʱ��
	PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,SystemStarted);



	/************************************************************************/
	/*                       [5/18/2017 ConanSteve]                         */
#ifdef DEBUG
	if (g_bCSRecord)
	{
		CString csKickFilePath{ TEXT("") };
		SYSTEMTIME sysTime{ 0 };
		GetLocalTime(&sysTime);
		csKickFilePath.Format(_T("\\LogFiles\\%04d%02d%02d_%02d%02d%02dInformation.txt"), sysTime.wYear, sysTime.wMonth, sysTime.wDay,
			sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
		csKickFilePath = m_sWorkPath + csKickFilePath;
		m_myFileTickResult.Open(csKickFilePath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite);
	}
#endif

	SetTimer(CheckSpeedRealTime, m_iCheckSpeedInterval, NULL);// ����ٶ�ʵʱ��ʱ��������
	SetTimer(IO24BSTATUS, 200, NULL);
	SetTimer(DisplayCount, 100, NULL);
	SetTimer(SaveCount, 60000, NULL);
	SetTimer(ShowDataInfo, 300, NULL);
#ifdef SPOT
	m_bIsDetecting = TRUE;
	m_IOCard[e_IOCard_2].setCardOutput(m_iPortOfStopMachine, 1);
#endif
	
	/************************************************************************/
	m_wsResPath = CStringConvert::ANSI_UCS2(m_sWorkPath.GetBuffer()) + L"/res/xml/";
	bool bRet{ false }, bRet1{ false }, bRet2{ false }, bRet3{ false }, bRet4{false};

	if (!PathFileExistsW(m_wsResPath.c_str()))
		return 0;
	wstring wsResFilePath = m_wsResPath + L"resource.res";
	if (!PathFileExistsW(wsResFilePath.c_str()))
		return 0;
	bRet = XC_LoadResource(wsResFilePath.c_str());
	wsResFilePath = m_wsResPath + L"resource1.res";
	if (!PathFileExistsW(wsResFilePath.c_str()))
		return 0;
	bRet1 = XC_LoadResource(wsResFilePath.c_str());
	wsResFilePath = m_wsResPath + L"resource2.res";
	if (!PathFileExistsW(wsResFilePath.c_str()))
		return 0;
	bRet2 = XC_LoadResource(wsResFilePath.c_str());
	wsResFilePath = m_wsResPath + L"style1.css";
	if (!PathFileExistsW(wsResFilePath.c_str()))
		return 0;
	bRet3 = XC_LoadStyle(wsResFilePath.c_str());
	wsResFilePath = m_wsResPath + L"style2.css";
	if (!PathFileExistsW(wsResFilePath.c_str()))
		return 0;
	bRet4 = XC_LoadStyle(wsResFilePath.c_str());
	if (!bRet || !bRet1 | !bRet2||!bRet3||!bRet4)
		return 0;
	m_bOpenXCWnd = true;
	/************************************************************************/
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
	{
	cs.style|=WS_MAXIMIZE;
	cs.style=WS_MAXIMIZEBOX&WS_MINIMIZEBOX&WS_SYSMENU;

	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	// �������

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::QuitPrecedure()
{
	if (m_bIsDetecting)
	{
		if(AfxMessageBox("ϵͳ���ڼ�⣬ȷ��Ҫֹͣ�����?",MB_YESNO) !=IDYES)
		{
			return;
		}
		else
		{
			StopCheck();
		}
	}

	//ShowWindow(SW_HIDE);
	QuitCheckProgram();	
			
	CMainFrame::OnClose();
}

void CMainFrame::InitInspectionDll()
{
	int i{ 0 };
	CString strPath;
	strPath.Format("%s\\Config\\AlgConfig.ini",m_sWorkPath);
	s_CheckInitParam sInit;
	LabelCheck::s_AlgSingleInit sAlgInit[c_AllElementCount];
	ZeroMemory(sAlgInit,sizeof(LabelCheck::s_AlgSingleInit)*c_AllElementCount);
	CDC* pDC[c_AllElementCount];
	pDC[0]=(CDC*)(g_pView->GetDlgItem(IDC_ALG1)->GetSafeHwnd());
	pDC[1]=(CDC*)(g_pView->GetDlgItem(IDC_ALG2)->GetSafeHwnd());
	pDC[2]=(CDC*)(g_pView->GetDlgItem(IDC_ALG3)->GetSafeHwnd());
	pDC[3]=(CDC*)(g_pView->GetDlgItem(IDC_ALG4)->GetSafeHwnd());
	pDC[4]=(CDC*)(g_pView->GetDlgItem(IDC_ALG5)->GetSafeHwnd());
	pDC[5]=(CDC*)(g_pView->GetDlgItem(IDC_ALG6)->GetSafeHwnd());
	
	//*[7/3/2017 ConanSteve]*:  ����ϵͳͼ����ʾ�����Ϣ
	s_DrawSysPic arrSysPicInfo[c_AllElementCount];
	arrSysPicInfo[0].pDC = (CDC*)(g_pView->GetDlgItem(IDC_PIC1)->GetSafeHwnd());
	arrSysPicInfo[1].pDC = (CDC*)(g_pView->GetDlgItem(IDC_PIC2)->GetSafeHwnd());
	arrSysPicInfo[2].pDC = (CDC*)(g_pView->GetDlgItem(IDC_PIC3)->GetSafeHwnd());
	arrSysPicInfo[3].pDC = (CDC*)(g_pView->GetDlgItem(IDC_PIC4)->GetSafeHwnd());
	arrSysPicInfo[4].pDC = (CDC*)(g_pView->GetDlgItem(IDC_PIC5)->GetSafeHwnd());
	arrSysPicInfo[5].pDC = (CDC*)(g_pView->GetDlgItem(IDC_PIC6)->GetSafeHwnd());
	arrSysPicInfo[0].hWnd = (g_pView->GetDlgItem(IDC_PIC1))->m_hWnd;
	arrSysPicInfo[1].hWnd = (g_pView->GetDlgItem(IDC_PIC2))->m_hWnd;
	arrSysPicInfo[2].hWnd = (g_pView->GetDlgItem(IDC_PIC3))->m_hWnd;
	arrSysPicInfo[3].hWnd = (g_pView->GetDlgItem(IDC_PIC4))->m_hWnd;
	arrSysPicInfo[4].hWnd = (g_pView->GetDlgItem(IDC_PIC5))->m_hWnd;
	arrSysPicInfo[5].hWnd = (g_pView->GetDlgItem(IDC_PIC6))->m_hWnd;
	for (int i = 0; i < c_AllElementCount; ++i)
	{
		arrSysPicInfo[i].rect = g_pView->m_rectShowImage[i];
	}
	/*****************************************************************************************/
	for(int i=0;i<c_AllElementCount;i++)
	{
// 		if(3==i)
// 			sAlgInit[i].nBayer =4 ;
// 		else
			sAlgInit[i].nBayer = m_BayerTransform[i].m_bAreaCamBayer;

		sAlgInit[i].nCameraIndex=i;
		sAlgInit[i].nHeight=m_sCameraImageInfo[i].m_iCameraImageHeight;
		sAlgInit[i].nWidth=m_sCameraImageInfo[i].m_iCameraImageWidth;
// 		if (3 == i)
// 			sAlgInit->nRevert = 0;
// 		else
// 			sAlgInit[i].nRevert = 2;
		sAlgInit[i].nRevert = m_bGrabFlip[i];
		sAlgInit[i].pDC=pDC;
	}

	int pAddr[c_AllElementCount];
	for( i=0;i<c_AllElementCount;i++)
	{
		pAddr[i]=(int)(&sAlgInit[i]);
	}
	sInit.pParam = (void*)pAddr;
	sInit.KickCallbackFunc=GlobalKickCallbackFunc;
	sInit.StatusCallbackFunc=GlobalStatusCallbackFunc;

#ifdef __SGGrabber_
	sInit.iCallType=2;
#else
	sInit.iCallType=0;
#endif

	sInit.pFilePath = strPath.GetBuffer(0);
	sInit.pThis=this;
	sInit.iReserve1=e_MachineType_Liquor;		//0��ʾ���� 1��ʾҩҺ
	

	s_Status sStatus;
	ZeroMemory(&sStatus,sizeof(sStatus));
	sStatus = m_InspectionObj.Init(sInit);


	CString strModelPath;
	strModelPath.Format("%s\\ModelInfo\\%s",m_sWorkPath,m_sModelName);

	// ��Ҫ������
	s_Status sRet;
	ZeroMemory(&sRet,sizeof(s_Status));

	s_AlgModelInput sAlgModel;
	ZeroMemory(&sAlgModel,sizeof(s_AlgModelInput));

	LabelCheck::s_DemoAlgSetParam sAlgSet;
	ZeroMemory(&sAlgSet,sizeof(LabelCheck::s_DemoAlgSetParam));

	int* pModelParam[c_AllElementCount];

	for(i=0;i<c_AllElementCount;i++)
	{
		sAlgSet.bTopWin=TRUE;
		sAlgSet.pParent=NULL;
		sAlgSet.iCheckSN=i;
		memcpy(sAlgSet.chModelName,m_sModelName.GetBuffer(0),m_sModelName.GetLength());
		
		sAlgModel.iCheckSN=i;
		sAlgModel.iOperSig=0;
		sAlgModel.pParam=&sAlgSet;

		m_InspectionObj.Set(sAlgModel);
	}
}

// ��ȡ��ִ�г������ڵ�·��
CString CMainFrame::GetCurrentAppPath()
{
	char chFile[MAX_PATH], chDrive[MAX_PATH], chDir[_MAX_DIR], chFname[_MAX_FNAME], chExt[_MAX_EXT];
	CString AppPath;
	GetModuleFileName(NULL, chFile, sizeof(chFile));
	_splitpath(chFile, chDrive, chDir, chFname, chExt);
	strcat(chDrive, chDir);
	AppPath = chDrive;
	return AppPath;
}

// ����ļ�·���Ƿ����, û������
BOOL CMainFrame::FindFilePathFunc()
{
	// �����������
	CFileFind searcher;		// ��������·�����ļ��Ƿ���ڵĶ���	
	CString strPath;
	CString strVirtualDiskPath;

	CTime CurrTime;
	CurrTime = CTime::GetCurrentTime();	

	// ѯ���㷨demo�����ļ����Ƿ����
	strVirtualDiskPath.Format("%s\\Config\\AlgConfig.ini",m_sWorkPath);
	m_iniAlgConfig.Init(CExFn::GetCurrentAppPath() + _T("\\Config\\AlgConfig.ini"));
	m_iMaxNumOfConstantKick = m_iniAlgConfig.GetIntEx(_T("SystemConfiguration"), _T("MaxNumberOfConstantKick"), 10, _T("�������������ֵ"));
	m_iPortOfAlarm = m_iniAlgConfig.GetIntEx(_T("SystemConfiguration"), _T("PortOfAlarm"), 3, _T("�ӿڿ�д�����źŵ�Out�˿�"));
	m_iPortOfStopMachine = m_iniAlgConfig.GetIntEx(_T("SystemConfiguration"), _T("PortOfStopMachine"), 4, _T("�ӿڿ�дͣ���ͺŵ�Out�˿�"));
	m_iPortOfNormalRunning = m_iniAlgConfig.GetIntEx(_T("SystemConfiguration"), _T("PortOfNormalRunning"), 5, _T("�ӿڿ�д�������е�Out�˿�"));
	m_iLastCheckCntTotle = m_iniAlgConfig.GetIntEx(_T("ͳ����Ϣ"), _T("�ϴμ������"), 0);
	m_iLastGoodCntTotle = m_iniAlgConfig.GetIntEx(_T("ͳ����Ϣ"), _T("�ϴκϸ�Ʒ��"), 0);
	if(!inIsPathExist(strVirtualDiskPath))
	{
// 		GetPrivateProfileString(TEXT("Demo Controls"), TEXT("Language"), TEXT(""), m_sLanguage.GetBuffer(256), 256, m_sVirtualDisk);
// 		m_sLanguage.ReleaseBuffer();
// 		INIT_STRING_TABLE(m_sLanguage);
		//g_pView->m_SpeedLabel.Display(tr("����ٶ�:"), "", 0, FALSE);
		CFile file;
		BOOL bRet=file.Open(strVirtualDiskPath,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		if(bRet==TRUE)
		{
			WritePrivateProfileString("DEMO Controls","VirtualDisk","M",strVirtualDiskPath);
			WritePrivateProfileString("DEMO Controls","DisableCreate","1",strVirtualDiskPath);
		}


		GetPrivateProfileString("DEMO Controls","VirtualDisk","M",m_sVirtualDisk.GetBuffer(255),255,strVirtualDiskPath);

		// д������ť������[2014-11-7 by zhy]
		CString str1;
		GetPrivateProfileString("DEMO Controls","ImgBtn1Text","",str1.GetBuffer(255),255,strVirtualDiskPath);
		if(str1.IsEmpty())
		{
			WritePrivateProfileString("DEMO Controls","ImgBtn1Text","��λ������ͼ��",strVirtualDiskPath);
		}
		
		GetPrivateProfileString("DEMO Controls","ImgBtn2Text","",str1.GetBuffer(255),255,strVirtualDiskPath);
		if(str1.IsEmpty())
		{
			WritePrivateProfileString("DEMO Controls","ImgBtn2Text","��λ������ͼ��",strVirtualDiskPath);
		}
		
		GetPrivateProfileString("DEMO Controls","ImgBtn3Text","",str1.GetBuffer(255),255,strVirtualDiskPath);
		if(str1.IsEmpty())
		{
			WritePrivateProfileString("DEMO Controls","ImgBtn3Text","��λ��ģ��ͼ��",strVirtualDiskPath);
		}
		
		
		// д������ͼ���ļ���·��[2014-11-7 by zhy]
		
		str1.Format("%s:\\SaveImages\\GrabImages",m_sVirtualDisk);
		WritePrivateProfileString("DEMO Controls","ImgBtn1Path",str1,strVirtualDiskPath);
		
		str1.Format("%s:\\SaveImages\\ErrorImages",m_sVirtualDisk);
		WritePrivateProfileString("DEMO Controls","ImgBtn2Path",str1,strVirtualDiskPath);
		
		str1.Format("%s:\\SaveImages\\ModelImages",m_sVirtualDisk);
		WritePrivateProfileString("DEMO Controls","ImgBtn3Path",str1,strVirtualDiskPath);


		// �㷨��������ͼ��
		str1.Format("%s:\\SaveImages\\GrabImages\\%04d-%02d-%02d\\Camera1",m_sVirtualDisk,CurrTime.GetYear(),
			CurrTime.GetMonth(),CurrTime.GetDay());
		WritePrivateProfileString("DEMO Controls","ImgAlgPath_1",str1,strVirtualDiskPath);
	}
	else
	{
// 		GetPrivateProfileString(TEXT("Demo Controls"), TEXT("Language"), TEXT(""), m_sLanguage.GetBuffer(256), 256, m_sVirtualDisk);
// 		m_sLanguage.ReleaseBuffer();
// 		INIT_STRING_TABLE(m_sLanguage);
		
		//g_pView->m_SpeedLabel.Display(tr("����ٶ�:"), "", 0, FALSE);
		WritePrivateProfileString("DEMO Controls","DisableCreate","1",strVirtualDiskPath);
		GetPrivateProfileString("DEMO Controls","VirtualDisk","M",m_sVirtualDisk.GetBuffer(255),255,strVirtualDiskPath);
		WritePrivateProfileString("DEMO Controls","VirtualDisk",m_sVirtualDisk,strVirtualDiskPath);


		// д������ť������[2014-11-7 by zhy]
		CString str1;
		GetPrivateProfileString("DEMO Controls","ImgBtn1Text","",str1.GetBuffer(255),255,strVirtualDiskPath);
		if(str1.IsEmpty())
		{
			WritePrivateProfileString("DEMO Controls","ImgBtn1Text","��λ������ͼ��",strVirtualDiskPath);
		}

		GetPrivateProfileString("DEMO Controls","ImgBtn2Text","",str1.GetBuffer(255),255,strVirtualDiskPath);
		if(str1.IsEmpty())
		{
			WritePrivateProfileString("DEMO Controls","ImgBtn2Text","��λ������ͼ��",strVirtualDiskPath);
		}

		GetPrivateProfileString("DEMO Controls","ImgBtn3Text","",str1.GetBuffer(255),255,strVirtualDiskPath);
		if(str1.IsEmpty())
		{
			WritePrivateProfileString("DEMO Controls","ImgBtn3Text","��λ��ģ��ͼ��",strVirtualDiskPath);
		}


		// д������ͼ���ļ���·��[2014-11-7 by zhy]

		str1.Format("%s:\\SaveImages\\GrabImages",m_sVirtualDisk);
		WritePrivateProfileString("DEMO Controls","ImgBtn1Path",str1,strVirtualDiskPath);

		str1.Format("%s:\\SaveImages\\ErrorImages",m_sVirtualDisk);
		WritePrivateProfileString("DEMO Controls","ImgBtn2Path",str1,strVirtualDiskPath);

		str1.Format("%s:\\SaveImages\\ModelImages",m_sVirtualDisk);
		WritePrivateProfileString("DEMO Controls","ImgBtn3Path",str1,strVirtualDiskPath);

		// �㷨��������ͼ��
		str1.Format("%s:\\SaveImages\\GrabImages\\%04d-%02d-%02d\\Camera1",m_sVirtualDisk,CurrTime.GetYear(),
			CurrTime.GetMonth(),CurrTime.GetDay());
		WritePrivateProfileString("DEMO Controls","ImgAlgPath_1",str1,strVirtualDiskPath);	
	}

	/************************************************************************/
	/*                       [6/26/2017 ConanSteve]                         */
	GetPrivateProfileString(TEXT("Demo Controls"), TEXT("DefaultModel"), TEXT(""), m_sModelName.GetBuffer(256), 256, strVirtualDiskPath);
	m_sModelName.ReleaseBuffer();
	LoadCamConfig(m_sModelName);
	m_sModelPath.Format("%s\\ModelInfo\\%s", m_sWorkPath, m_sModelName);
	/************************************************************************/

	//��־�ļ�Ŀ¼[2013-8-21 by zhy]
	CString strMPath;
	strMPath.Format("%s:\\LogFiles\\GrabLog",m_sVirtualDisk);
	if(!inIsPathExist(strMPath))
	{
		inCreateMultiLevelPath(strMPath);
	}

	int i=0;
	CString strName,strBasic;
	strBasic.Format("%s:\\LogFiles\\GrabLog",m_sVirtualDisk);

	for(i=0;i<c_AllElementCount;i++)
	{
		// ��¼�����Ϣ
		strName.Format("%s\\Guangdian%d.txt",strBasic,i+1);
		m_filePE[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
	}

	for(i=0;i<c_AllElementCount;i++)
	{
		// ��¼�㷨���ʱ��[2013-8-21 by zhy]
		strName.Format("%s\\Suanfashijian%d.txt",strBasic,i+1);
		m_fileDetectTime[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// ��¼����ص�ʱ��[2013-8-21 by zhy]
		strName.Format("%s\\Huidiao%d.txt",strBasic,i+1);
		m_fileCall[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// ����߳����㷨��ⷵ��FALSEʱ��ͼ����Ϣ[2013-8-21 by zhy]
		strName.Format("%s\\Thread_Bad%d.txt",strBasic,i+1);
		m_fileThreadBad[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// ����߳����㷨��ⷵ��TRUEʱ��ͼ����Ϣ[2013-8-21 by zhy]
		strName.Format("%s\\Thread_Good%d.txt",strBasic,i+1);
		m_fileThreadGood[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// �޷��߳��еõ���Ʒʱ��ͼ����Ϣ
		strName.Format("%s\\Kick_Good%d.txt",strBasic,i+1);
		m_fileGood[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// �����޷ϲ�Ʒ����Ϣ
		strName.Format("%s\\Tifei%d.txt",strBasic,i+1);
		m_fileKickImage[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// ���в��޲�Ʒ����Ϣ
		strName.Format("%s\\Buti%d.txt",strBasic,i+1);
		m_fileAddKickImage[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// �ص������м�¼ͼ����Ϣ
		strName.Format("%s\\Call_Mid%d.txt",strBasic,i+1);
		m_fileImageNum[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// �������м�¼ͼ����Ϣ
		strName.Format("%s\\CheckThread_Begin%d.txt",strBasic,i+1);
		m_fileImage[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// �����㷨���ʱͼ����Ϣ
		strName.Format("%s\\Jianceqian%d.txt",strBasic,i+1);
		m_fileBeforeD[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// ��¼�㷨�����
		strName.Format("%s\\Baocuo%d.txt",strBasic,i+1);
		m_fileDetect[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// ��¼�������͸��ӿڿ���ͼ��ż�ʱ��
		strName.Format("%s\\Kick_Bad%d.txt",strBasic,i+1);
		//m_arr_fileKick[i].Open(strName, &m_bStartRecord, CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		
		// ����һ���߳��м�¼�ӿڿ�ͼ��żĴ����е�ͼ���
		strName.Format("%s\\Tuxiangjicunqi%d.txt",strBasic,i+1);
		m_fileRecordImage[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// ��¼�¼������ź���Ϣ
		strName.Format("%s\\Call_SetEvent%d.txt",strBasic,i+1);
		m_fileSetEvent[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// ��¼��ȡͼ��������Ϣ
		strName.Format("%s\\Call_WriteImage%d.txt",strBasic,i+1);
		m_fileWriteImage[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// ͣ������
		strName.Format("%s\\StopMachine%d.txt",strBasic,i+1);
		m_fileStopMachine[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);

		strName.Format("%s\\RecordKick%d.txt",strBasic,i+1);
		m_fileRecordKick[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
	}
	
	// ������������ļ��Ƿ����
	m_sCipherPath1.Format("%s\\Config\\cipher1.cip",m_sWorkPath);

	//���ϵͳ�����ļ��Ƿ����
#ifdef __SGGrabber_                                // ģ��ɼ�
	m_sSysConfigFileName.Format("%s\\Config\\CheckSystemConfig-sim.ini", m_sWorkPath);
#else
	m_sSysConfigFileName.Format("%s\\Config\\CheckSystemConfig.ini", m_sWorkPath);
#endif
	if(!searcher.FindFile(m_sSysConfigFileName))
	{
		CString strExceptionDes("");
		strExceptionDes.Format("��������:CheckSystemConfig.ini�ļ�����ָ��·����!");
		m_LogManager.MyWriteLogFile("������ȡ",strExceptionDes,OperationLog);
		AfxMessageBox(strExceptionDes);
	}
		
	//���ģ��ɼ������ļ��Ƿ����  [4/27/2010 GZ]
	for (i=0;i<CAMERA_COUNT;i++)
	{
		CString strTmppp,strConfigName;
		strTmppp.Format("SimulateGrabberConfig%d.ini",i+1);
		strConfigName.Format("%s\\Config\\%s", m_sWorkPath,strTmppp);
		if(!searcher.FindFile(strConfigName))
		{
			m_bIsSimulateIniExist[i]=FALSE;
		}
		else
		{
			CString strTmpPath("");
			GetPrivateProfileString("��ͼ����","ͼ���ļ������ļ���","",m_sSimulateGrabPath[i].GetBuffer(255),255,strConfigName);
			if (m_sSimulateGrabPath[i]!="")
			{
				CString strTmpPath;
				strTmpPath.Format("%s\\*.bmp",m_sSimulateGrabPath[i]);
				if (!searcher.FindFile(strTmpPath))
				{
					m_bIsSimulateIniExist[i]=FALSE;
				}            
				else 
					m_bIsSimulateIniExist[i]=TRUE;
			}
			
		}		
	}
	
	//��鱣��ͼ���ļ����Ƿ����
	m_sImageFilePath.Format("%s\\SaveImages", m_sWorkPath);
	if(!searcher.FindFile(m_sImageFilePath))
	{
		if(CreateDirectory(m_sImageFilePath, NULL) == FALSE)
		{
			AfxMessageBox("����·��ʧ��, ��ȷ�ϱ��ر���ͼ���ļ����Ƿ�������ȷ!!!");
			return FALSE;
		}
	}

	CString strInterestFilePath;
	strInterestFilePath.Format("%s\\InterestImages",m_sWorkPath);
	if(!searcher.FindFile(strInterestFilePath))
	{
		if(CreateDirectory(strInterestFilePath, NULL) == FALSE)
		{
			AfxMessageBox("����·��ʧ��, ��ȷ�ϱ��ر������Ȥͼ���ļ����Ƿ�������ȷ!!!");
			return FALSE;
		}
	}
	
	//�����־�ļ����Ƿ����
	m_sLogFilePath.Format("%s\\LogFiles", m_sWorkPath);
	if(!searcher.FindFile(m_sLogFilePath))
	{
		if(CreateDirectory(m_sLogFilePath, NULL) == FALSE)
		{
			AfxMessageBox("����·��ʧ��, ��ȷ�ϱ��ؼ�¼��־�ļ����Ƿ�������ȷ!!!");
			return FALSE;
		}
	}
	
	//����㷨�����ļ����Ƿ����
	m_sAlgIniFilePath.Format("%s\\ModelInfo", m_sWorkPath);
	if(!searcher.FindFile(m_sAlgIniFilePath))
	{
		if(CreateDirectory(m_sAlgIniFilePath, NULL) == FALSE)
		{
			AfxMessageBox("����·��ʧ��, ��ȷ�ϱ����㷨�����ļ����Ƿ�������ȷ!!!");
			return FALSE;
		}
	}

	
	//����ͼ����ص��ļ���
	strPath.Format("%s\\GrabImages", m_sImageFilePath); //�ɼ�ͼ���ļ���
	if(!searcher.FindFile(strPath))
	{
		if(CreateDirectory(strPath, NULL) == FALSE)
		{
			AfxMessageBox("�����ɼ�ͼ���ļ���·��ʧ��!!!");
			return FALSE;
		}
	}
	
	//��������ͼ���ļ���
	strPath.Format("%s\\ErrorImages", m_sImageFilePath); //����ͼ���ļ���
	if(!searcher.FindFile(strPath))
	{
		if(CreateDirectory(strPath, NULL) == FALSE)
		{
			AfxMessageBox("��������ͼ���ļ���·��ʧ��!!!");
			return FALSE;
		}
	}
	
	//�����쳣ͼ���ļ���
	strPath.Format("%s\\AbnormityImages", m_sImageFilePath); //�쳣ͼ���ļ���
	if(!searcher.FindFile(strPath))
	{
		if(CreateDirectory(strPath, NULL) == FALSE)
		{
			AfxMessageBox("�����쳣ͼ���ļ���·��ʧ��!!!");
			return FALSE;
		}
	}
	
	//������ȷͼ���ļ���
	strPath.Format("%s\\CorrectImages", m_sImageFilePath); //��ȷͼ���ļ���
	if(!searcher.FindFile(strPath))
	{
		if(CreateDirectory(strPath, NULL) == FALSE)
		{
			AfxMessageBox("������ȷͼ���ļ���·��ʧ��!!!");
			return FALSE;
		}
	}
	// �쳣ͼ�񱣴��ļ���
	CString strAbnormalPath,strAbnormalDes,strDate;
	strDate.Format("%04d-%02d-%02d",CurrTime.GetYear(),CurrTime.GetMonth(),CurrTime.GetDay());
	
	for(i=0;i<CAMERA_COUNT;i++)
	{
		strAbnormalPath.Format("%s:\\SaveImages\\AbnormityImages\\%s\\Camera%d",m_sVirtualDisk,strDate,i+1);
		if (!inIsPathExist(strAbnormalPath))
		{
			if(!inCreateMultiLevelPath(strAbnormalPath))
			{
				strAbnormalDes.Format("����·�� %s ʧ�ܣ�",strAbnormalPath);
				AfxMessageBox(strAbnormalDes);
				break;
				return FALSE;
			}
		}
	}
	
	// Ϊ�㷨�������ñ�������ͼ��ʹ�õ��ļ���
	
	CString strAlgImages,strDes;
	
	for(i=0;i<CAMERA_COUNT;i++)
	{
		strAlgImages.Format("%s:\\SaveImages\\AlgImages\\Camera%d",m_sVirtualDisk,i+1);
		if (!inIsPathExist(strAlgImages))
		{
			if(!inCreateMultiLevelPath(strAlgImages))
			{
				strDes.Format("����·�� %s ʧ�ܣ�",strAlgImages);
				AfxMessageBox(strDes);
				break;
				return FALSE;
			}
		}
	}


	//������־��ص��ļ���
	strPath.Format("%s\\OperationLog", m_sLogFilePath); //������־�ļ���
	if(!searcher.FindFile(strPath))
	{
		if(CreateDirectory(strPath, NULL) == FALSE)
		{
			AfxMessageBox("����������־�ļ���·��ʧ��!!!");
			return FALSE;
		}
	}
	
	strPath.Format("%s\\AbnormityLog", m_sLogFilePath); //�쳣��־�ļ���
	if(!searcher.FindFile(strPath))
	{
		if(CreateDirectory(strPath, NULL) == FALSE)
		{
			AfxMessageBox("�����쳣��־�ļ���·��ʧ��!!!");
			return FALSE;
		}
	}
	
	strPath.Format("%s\\CheckLog", m_sLogFilePath); //�����־�ļ���
	if(!searcher.FindFile(strPath))
	{
		if(CreateDirectory(strPath, NULL) == FALSE)
		{
			AfxMessageBox("���������־�ļ���·��ʧ��!!!");
			return FALSE;
		}
	}
	m_sCheckLogFilePath=strPath;
	
	// ����ļ����µ������ļ���
	if(!searcher.FindFile(strPath))
	{
		if(CreateDirectory(strPath, NULL) == FALSE)
		{
			AfxMessageBox("���������־�ļ���-�����ļ��е�·��ʧ��!!!");
			return FALSE;
		}
	}

	//�����޸���־�ļ���
	strPath.Format("%s\\MendParamLog", m_sLogFilePath); 
	if(!searcher.FindFile(strPath))
	{
		if(CreateDirectory(strPath, NULL) == FALSE)
		{
			AfxMessageBox("���������޸���־�ļ���·��ʧ��!!!");
			return FALSE;
		}
	}
	
	//ͳ�Ƽ��ʱ���ļ��� [2012-3-14 by zhy]
	strPath.Format("%s\\GrabLog", m_sLogFilePath); 
	if(!searcher.FindFile((strPath)))
	{
		if(CreateDirectory(strPath, NULL) == FALSE)
		{
			AfxMessageBox("������¼���ʱ����־�ļ���·��ʧ��!!!");
			return FALSE;
		}
	}


	// ���������ļ�[2014-12-2 zhy]
	strDes.Format("%s\\DataFiles\\%04d-%02d-%02d",m_sWorkPath,CurrTime.GetYear(),CurrTime.GetMonth(),CurrTime.GetDay());
	if(!inIsPathExist(strDes))
	{
		if(!inCreateMultiLevelPath(strDes))
		{
			strDes.Format("����·�� %s ʧ�ܣ�",strDes);
			AfxMessageBox(strDes);
			return FALSE;
		}
	}
	
	m_sDataFilePath = strDes;

	return TRUE;
}

// �ļ����� �������ƣ�FO_COPY�������У�FO_CUT����ɾ����FO_DELETE��
BOOL CMainFrame::OP_Directory(LPCSTR lpsrcPath, LPCSTR lpdstPath, int type, int flag)
{
	SHFILEOPSTRUCT FileOp;   
	
	FileOp.hwnd   =   NULL;   
	FileOp.wFunc  =   type;   
	FileOp.pFrom  =   lpsrcPath;   
	FileOp.pTo    =   lpdstPath;   
	FileOp.fFlags =   flag;//FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR|FOF_NOERRORUI|FOF_SILENT;   
	FileOp.hNameMappings  =  NULL;   
	FileOp.lpszProgressTitle  =  NULL;   
	
	int iRet=-1;
	iRet = SHFileOperation(&FileOp);
	
	if(0==iRet)
		return TRUE;
	else
		return FALSE;
}

BOOL CMainFrame::inCreateMultiLevelPath(const CString &strPath)
{
	//����һ�����Ŀ¼��������ھͲ�����
	//(strPath  �Ϸ�: "D:\\abc\\me" �� "me\\you" �� ""; 
	//			���Ϸ�: "D:\\abc\\me\\" �� "me\\you\\" �� "\\")
	CString tmpPath;// = strPath;
	tmpPath.Format("%s",strPath);
	CList<CString,CString&> FolderStrList;
	int nStrLen = 0;
	
	for(;;)
	{
		nStrLen = tmpPath.GetLength();
		if(nStrLen <= 0)
		{
			break;//��ͷ���ļ���
		}		
		
		BOOL bPathExist = FALSE;//��·���Ƿ��Ѿ�����
		
		if(tmpPath.Right(1) == ":")
		{
			bPathExist = inIsPathExist(tmpPath + "\\");
		}
		else
		{
			bPathExist = inIsPathExist(tmpPath);
		}
		
		if(bPathExist)//���·������
		{
			break;//���Ѵ��ڵ�·���½��ļ���
		}
		
		//���·��������		
		//ÿ�ν�����һ���ļ��������
		
		int nPos = 0;
		nPos = tmpPath.ReverseFind('\\');//�Ӻ���ǰȷ���ļ�����
		if(nPos == -1)//�ѵ����һ���ļ�����
		{
			nPos = 0;//����һ���ļ��������
		}
		
		//���ļ��������
		CString tmpStr = tmpPath.Right(nStrLen - nPos);
		FolderStrList.AddHead(tmpStr);
		tmpPath.Delete(nPos, nStrLen - nPos);		
	}	
	
	//�����ļ���������˳����
	while(FolderStrList.IsEmpty() == FALSE)
	{
		tmpPath += FolderStrList.RemoveHead();
		BOOL bRet = FALSE;
		bRet = CreateDirectory(tmpPath, NULL);
		if(bRet == FALSE)
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

BOOL CMainFrame::inIsPathExist(const CString &strPath)
{
#ifdef _UNICODE
#define IsDirectoryExist(path) ::_waccess(path,0) == 0
	//_waccess�ĵڶ���������ʾ���жϴ����ԣ�����0��ʾ���ڣ�����-1��ʾ������
#else
#define IsDirectoryExist(path) ::_access(path,0) == 0//_access��MBCS�汾
#endif
	
	if(IsDirectoryExist(strPath))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	
}

// ��ȡϵͳ�����ļ�
BOOL CMainFrame::ReadSystemConfig(const CString &strLoadFile)
{
	CString  strTemp;
	UINT ExitCode = 0;
	BOOL bReturn = TRUE;
	m_sAlgConfigFilePath = m_sWorkPath + TEXT("\\Config\\AlgConfig.ini");

	//��ȡ���ڱ���  //[2012-1-18 by zhy]
	GetPrivateProfileString("ϵͳ����", "���ڱ���", "���ͼ���ǩ�������ϵͳ���V1.0", m_sWindowTitle.GetBuffer(255), 255, strLoadFile);
	
	//��ȡ��һ��ģ������
	GetPrivateProfileString("ϵͳ����","��һ��ʹ���㷨ģ������","",strTemp.GetBuffer(255),255,strLoadFile);     //  [6/17/2010 ZXF]

	// ������������ļ�[2017-3-24]
// 	LoadCamConfig(strTemp);
// 
// 	m_sModelName.Format("%s",strTemp);
// 	m_sModelPath.Format("%s\\ModelInfo\\%s",m_sWorkPath,strTemp);    

	m_iLabelErrorCount[Camera1]			= GetPrivateProfileInt("ϵͳ����", "��ǩλ�������������", 5, strLoadFile);			//�����������
	m_bIOCardValid						= GetPrivateProfileInt("ϵͳ����", "�ӿڿ��Ƿ���Ч", 0, strLoadFile);			//�ӿڿ��Ƿ���Ч
	m_iImageShowMode					= GetPrivateProfileInt("ϵͳ����", "��ʾȫ��ͼ��", 1, strLoadFile);				//�ж�ͼ����ʾģʽ
	m_bOpenAlgCheck						= GetPrivateProfileInt("ϵͳ����", "�����㷨���", 1, strLoadFile);				//�Ƿ����㷨��� 
	m_iMaxSaveImageCount				= GetPrivateProfileInt("ϵͳ����", "����ͼ������", 100, strLoadFile);			//����ͼ����������
	m_iPreAllocateSaveInfoElementCount	= GetPrivateProfileInt("ϵͳ����", "Ԥ���䱣��ͼ��Ԫ�ظ���", 5, strLoadFile); 
	m_iIntervalDisplayTime				= GetPrivateProfileInt("ϵͳ����", "ͼ����ʾ���ʱ��", 0, strLoadFile);			//ͼ����ʾ���ʱ��
	m_iCheckSpeedInterval				= GetPrivateProfileInt("ϵͳ����", "����ٶ�ʱ����", 2000, strLoadFile);
	m_iSaveImage_Abnormal				= GetPrivateProfileInt("ϵͳ����", "��������쳣ͼ������", 10, strLoadFile);
	m_iRecord_Set						= GetPrivateProfileInt("ϵͳ����", "���������־��¼����", 20000, strLoadFile);
	m_iErrorSignal						= GetPrivateProfileInt("ϵͳ����", "�����źż��", 100, strLoadFile);
	m_bOpenImageSave					= GetPrivateProfileInt("ϵͳ����", "����ͼ�񱣴�", 0, strLoadFile);				//�Ƿ����㷨��� 
	m_iKickOffset5				= GetPrivateProfileInt("ϵͳ����", "����λ�õ����6�Ĺ��Ӹ���", 4, strLoadFile);				//����λ�õ����6�Ĺ��Ӹ���
	m_iShowDevice			= GetPrivateProfileInt("ϵͳ����", "��ʾ�豸", 0, strLoadFile);				//0Ϊϵͳͼ�� 1Ϊ�㷨ͼ��
	m_iSaveImageSys    = GetPrivateProfileInt("ϵͳ����", "ϵͳ�Ƿ񱣴�ͼ��", 0, strLoadFile);				//0Ϊ��1Ϊ��

	/************************************************************************/
	//added by zhangfan[2017/04/19]
	m_iSaveImageType = GetPrivateProfileInt(_T("SystemConfiguration"), _T("SaveImageType"), 0, m_sSysConfigFileName);/** ����ͼ������              [6/26/2017 ConanSteve]**/
	m_iIsCircle = GetPrivateProfileInt(_T("SystemConfiguration"), _T("WhetherCirleSaveImage"), 0, m_sSysConfigFileName);
	m_iSaveSeqCnt = GetPrivateProfileInt(_T("SystemConfiguration"), _T("NumberOfSequentialImageSaved"), 10, m_sSysConfigFileName);
	m_iSaveErrCnt = GetPrivateProfileInt(_T("SystemConfiguration"), _T("NumberOfErrorImageSaved"), 10, m_sSysConfigFileName);
	m_iSaveExistCnt = GetPrivateProfileInt(TEXT("SystemConfiguration"), TEXT("NumberOfExistenceImageSaved"), 10, m_sSysConfigFileName);
	for (int i = 0; i < c_AllElementCount; ++i)
	{
		CString csKey{ _T("") };
		csKey.Format(_T("WhetherSaveImageWithWorkspace%d"), i + 1);
		m_bSaveCameraImage[i] = GetPrivateProfileInt("SystemConfiguration", csKey, 0, m_sSysConfigFileName);
	}
	m_iLabelQueLen = GetPrivateProfileInt(_T("SystemConfiguration"), _T("LabelQueueLength"), 5, m_sSysConfigFileName);
	m_iDisBtwn5And6 = GetPrivateProfileInt(_T("SystemConfiguration"), _T("DistanceBetween5And6"), 5, m_sSysConfigFileName);
	m_iSaveImgObj = GetPrivateProfileInt(TEXT("SystemConfiguration"), TEXT("SaveImageObjection"), 0, m_sSysConfigFileName);
	m_iSaveImgNamingRegulation = GetPrivateProfileInt(TEXT("SystemConfiguration"), TEXT("SaveImageNamingRegulation"), 0, m_sSysConfigFileName);
	/************************************************************************/
	
	m_iSysDetectMode = GetPrivateProfileInt(TEXT("SystemConfiguration"), TEXT("SystemDetectMode"), e_SysDetectMode_Multi, m_sAlgConfigFilePath);
	m_iSysDataEleLen = e_SysDetectMode_Multi == m_iSysDetectMode ? 1 : 0;
	m_iObjDetectMode = GetPrivateProfileInt(TEXT("SystemConfiguration"), TEXT("ObjectDetectMode"), e_ObjDetectMode_1, m_sAlgConfigFilePath);
	/************************************************************************/
	/************************************************************************/
	/*                       [5/17/2017 ConanSteve]                         */
	for (int i = 0; i < c_AllElementCount; ++i)
	{
		CString sCameraCfgFilePath{ TEXT("") };
		sCameraCfgFilePath.Format(TEXT("CameraParm%d.ini"), i + 1);
		sCameraCfgFilePath = m_sWorkPath + TEXT("\\Config\\") + sCameraCfgFilePath;
		m_iExposureTime[i] = GetPrivateProfileInt(TEXT("Camera"), TEXT("ExposureTime"), 100, sCameraCfgFilePath);
	}
	m_iSG_ElapseTimeToRead = GetPrivateProfileInt(TEXT("SystemConfiguration"), TEXT("TimeElpaseToReadImg"), 50, m_sSysConfigFileName);
	/************************************************************************/
	if (m_iIntervalDisplayTime < 0)
	{

		strTemp.Format("��ȡ�����ļ�-ϵͳ����-ͼ����ʾ���ʱ��-%d����!",m_iIntervalDisplayTime);
		MessageBox(strTemp);
		return FALSE;
	}
	
	CString  strSession;
	int i=0,j=0;
	for (i=0;i<c_AllElementCount;i++)
	{
		
		strSession.Format("��λ%dֹͣ�����ж�����",i+1);
		m_iStopCountDiff[i]=GetPrivateProfileInt("ϵͳ����", strSession, 3, strLoadFile);//[2013-2-21]

		strSession.Format("��λ%d�㷨�������",i+1);
		m_iInspectionObj[i]=GetPrivateProfileInt("ϵͳ����", strSession, 3, strLoadFile);//[2013-2-21]

		strSession.Format("��λ%d�㷨����ʱ��",i+1);
		m_iKickInterval[i]=GetPrivateProfileInt("ϵͳ����", strSession, 120, strLoadFile);//[2013-2-21]

		strSession.Format("���%d�Ƿ���",i+1);
		m_bIsCheck[i] = GetPrivateProfileInt("ϵͳ����", strSession, 0, strLoadFile); //�Ƿ���

		strSession.Format("���%dһ�����ڲɼ�ͼ�����",i+1);
		m_iGrabCountSingleCircle[i] = GetPrivateProfileInt("ϵͳ����", strSession, 1, strLoadFile);//һ�����ڲɼ�ͼ�����

		if (i==0)// ȡ 3����������ڲɼ����������ֵ [4/19/2010 GZ]
		{
			m_iMaxGrabCountSingleCircle=m_iGrabCountSingleCircle[i];
		}
		else
		{
			if (m_iGrabCountSingleCircle[i]>m_iMaxGrabCountSingleCircle)
			{
				m_iMaxGrabCountSingleCircle=m_iGrabCountSingleCircle[i];
			}
		}
	}

	//Bayer�任��ر���
	for (i=0;i<CAMERA_COUNT;i++)
	{
		// �������ͼ���Ƿ����Bayerת��
		strSession.Format("���%dBayer�任����",i+1);
		m_BayerTransform[i].m_bAreaCamBayer = GetPrivateProfileInt(strSession, "�Ƿ����Bayer�任", -1, strLoadFile);
		
		GetPrivateProfileString(strSession,"Bayerת��Red����","",strTemp.GetBuffer(255),255,m_sSysConfigFileName);
		m_BayerTransform[i].m_dBayerRedGain = atof(strTemp);
		
		GetPrivateProfileString(strSession,"Bayerת��Green����","",strTemp.GetBuffer(255),255,m_sSysConfigFileName);
		m_BayerTransform[i].m_dBayerGreenGain = atof(strTemp);
		
		GetPrivateProfileString(strSession,"Bayerת��Blue����","",strTemp.GetBuffer(255),255,m_sSysConfigFileName);
		m_BayerTransform[i].m_dBayerBlueGain = atof(strTemp);

// #ifdef __SGGrabber_
// 		m_BayerTransform[i].m_bAreaCamBayer = FALSE;
// #endif

		if (m_BayerTransform[i].m_bAreaCamBayer)
		{
			SetLutTable(m_BayerTransform[i].m_dBayerRedGain,m_BayerTransform[i].m_dBayerGreenGain,
						m_BayerTransform[i].m_dBayerBlueGain);
		}
		
		strSession.Format("���%d�Ƿ�ת",i+1);
		m_bGrabFlip[i] = GetPrivateProfileInt("�ɼ�ͼ��ת", strSession, 0, strLoadFile);	
		
		strSession.Format("���%d��תģʽ",i+1);
		int iFlip = GetPrivateProfileInt("�ɼ�ͼ��ת", strSession, 0, strLoadFile);	
		if (iFlip == 0)
		{
			m_eShowModeFlip[i] = NoFlip;         //�޷�ת
		}
		else if (iFlip == 1)
		{
			m_eShowModeFlip[i] = FlipVertical; //��ֱ��ת
		}
		else if (iFlip == 2)
		{
			m_eShowModeFlip[i] = FlipHorizintal;   //ˮƽ��ת
		}
	}

	// ��ʼ��д��ȱ����Ϣ
	// ���λ����Ϣ
	CString strInfo;
	strInfo.Format("%d",10);
	WritePrivateProfileString("��λ1ȱ����Ϣ","ȱ�ݸ���",strInfo,strLoadFile);

	strInfo.Format("%s","�޷��ҵ�ƿͷ����������");
	WritePrivateProfileString("��λ1ȱ����Ϣ","����1",strInfo,strLoadFile);

	strInfo.Format("%s","ƿ�ӷ���");
	WritePrivateProfileString("��λ1ȱ����Ϣ","����2",strInfo,strLoadFile);

	strInfo.Format("%s","�ޱ�ǩ");
	WritePrivateProfileString("��λ1ȱ����Ϣ","����3",strInfo,strLoadFile);

	strInfo.Format("%s","��ǩλ��ƫ��");
	WritePrivateProfileString("��λ1ȱ����Ϣ","����4",strInfo,strLoadFile);

	strInfo.Format("%s","��ǩλ��ƫ��");
	WritePrivateProfileString("��λ1ȱ����Ϣ","����5",strInfo,strLoadFile);

	strInfo.Format("%s","��ǩλ��������б");
	WritePrivateProfileString("��λ1ȱ����Ϣ","����6",strInfo,strLoadFile);

	strInfo.Format("%s","��ǩλ��������б");
	WritePrivateProfileString("��λ1ȱ����Ϣ","����7",strInfo,strLoadFile);
	
	strInfo.Format("%s","");
	WritePrivateProfileString("��λ1ȱ����Ϣ","����8",strInfo,strLoadFile);
	
	strInfo.Format("%s","");
	WritePrivateProfileString("��λ1ȱ����Ϣ","����9",strInfo,strLoadFile);
	
	strInfo.Format("%s","�ж����ǩ");
	WritePrivateProfileString("��λ1ȱ����Ϣ","����10",strInfo,strLoadFile);

	// ��λ2ȱ����Ϣ
	strInfo.Format("%d",8);
	WritePrivateProfileString("��λ2ȱ����Ϣ","ȱ�ݸ���",strInfo,strLoadFile);
	
	strInfo.Format("%s","��ǩ��������");
	WritePrivateProfileString("��λ2ȱ����Ϣ","����1",strInfo,strLoadFile);
	
	strInfo.Format("%s","�޷���λ����ǩ����");
	WritePrivateProfileString("��λ2ȱ����Ϣ","����2",strInfo,strLoadFile);
	
	strInfo.Format("%s","�޷�ƥ�䵽ģ��");
	WritePrivateProfileString("��λ2ȱ����Ϣ","����3",strInfo,strLoadFile);
	
	strInfo.Format("%s","�޷���ȷ��ȡ�����");
	WritePrivateProfileString("��λ2ȱ����Ϣ","����4",strInfo,strLoadFile);
	
	strInfo.Format("%s","���ڲ���ȱ�ٵ�");
	WritePrivateProfileString("��λ2ȱ����Ϣ","����5",strInfo,strLoadFile);
	
	strInfo.Format("%s","���ڲ���ȱ�ٺ��");
	WritePrivateProfileString("��λ2ȱ����Ϣ","����6",strInfo,strLoadFile);
	
	strInfo.Format("%s","�������������");
	WritePrivateProfileString("��λ2ȱ����Ϣ","����7",strInfo,strLoadFile);
	
	strInfo.Format("%s","��������ģ��");
	WritePrivateProfileString("��λ2ȱ����Ϣ","����8",strInfo,strLoadFile);
	
	// ����ȱ����Ϣ
	for(i=0;i<c_AllElementCount;i++)
	{
		CString str1,str2;
		str1.Format("��λ%dȱ����Ϣ",i+1);
		str2.Format("%s","ȱ�ݸ���");
		m_iDefectTypePerWS[i]=GetPrivateProfileInt(str1, str2, 10, strLoadFile);	
		
		for(j=0;j<m_iDefectTypePerWS[i];j++)
		{
			str2.Format("����%d",j+1);
			GetPrivateProfileString(str1,str2,"",m_sDefectInfo[i][j].GetBuffer(255),255,strLoadFile);
		}
	}
	
	// �㷨Demo·�� [2014-7-1 by zhy]
	m_sAlgConfigFilePath.Format("%s\\set.ini", m_sWorkPath);	
	// Demo����
	GetPrivateProfileString("ϵͳ����","DemoName","",m_sDemoName.GetBuffer(255),255,m_sAlgConfigFilePath);
	//WritePrivateProfileString("ϵͳ����","AppName",m_sWindowTitle,m_sAlgConfigFilePath);
	WritePrivateProfileString("ϵͳ����", "AppName", _T("���ͼ���Ѫ�ܼ��ϵͳ"), m_sAlgConfigFilePath);

	// �����ź���ʹ�õĽӿڿ���ź�����˿ں�
	m_iAlarmCardNum = GetPrivateProfileInt("�ӿڿ�����", "�����ź���ʹ�õĽӿڿ����", 1, strLoadFile);
	if(m_iAlarmCardNum<1 || m_iAlarmCardNum>IOCARD_COUNT)
	{
		AfxMessageBox("�����ź���ʹ�ýӿڿ����ô�����鿴�����ļ�!");
		return FALSE;
	}

	m_iAlarmOutPort = GetPrivateProfileInt("�ӿڿ�����", "�����ź���ʹ�õ�����˿ں�", 7, strLoadFile);
	if(m_iAlarmOutPort<0 || m_iAlarmOutPort>7)
	{
		AfxMessageBox("�����ź���ʹ�ýӿڿ�����˿����ô�����鿴�����ļ�!");
		return FALSE;
	}

	// ���ʹ�õ�������
	for(i=0;i<CAMERA_COUNT;i++)
	{
		if(i<Camera6)
		{		
			strInfo.Format("���%d��ʹ�õĽӿڿ����",i+1);
			m_iCamCardNum[i]=GetPrivateProfileInt("�ӿڿ�����", strInfo, 1, strLoadFile);
		}
		else
		{
			strInfo.Format("���%d��ʹ�õĽӿڿ����",i+1);
			m_iCamCardNum[i]=GetPrivateProfileInt("�ӿڿ�����", strInfo, 2, strLoadFile);
		}

		strInfo.Format("���%d��ʹ�õ�����˿ں�",i+1);

		if(Camera1==i)
		{
			m_iCamOutPort[i]=GetPrivateProfileInt("�ӿڿ�����", strInfo, 1, strLoadFile);
		}
		else if(Camera2==i)
		{
			m_iCamOutPort[i]=GetPrivateProfileInt("�ӿڿ�����", strInfo, 1, strLoadFile);
		}
		else if(Camera3==i)
		{
			m_iCamOutPort[i]=GetPrivateProfileInt("�ӿڿ�����", strInfo, 1, strLoadFile);
		}
		else if(Camera4==i)
		{
			m_iCamOutPort[i]=GetPrivateProfileInt("�ӿڿ�����", strInfo, 1, strLoadFile);
		}
		else if(Camera5==i)
		{
			m_iCamOutPort[i]=GetPrivateProfileInt("�ӿڿ�����", strInfo, 2, strLoadFile);
		}
		else if(Camera6==i)
		{
			m_iCamOutPort[i]=GetPrivateProfileInt("�ӿڿ�����", strInfo, 1, strLoadFile);
		}
	}

	// ��ȡ�ڶ�·������X��ʱ֡����
	CString strPIO24B[IOCARD_COUNT];
	strPIO24B[Camera1].Format("%s\\PIO24B_reg_init.txt",m_sWorkPath);
	strPIO24B[Camera2].Format("%s\\PIO24B_reg_init_2.txt",m_sWorkPath);

	CTime curTime = CTime::GetCurrentTime();

	for(i=0;i<c_AllElementCount;i++)
	{
		m_sSaveImagePath[i].Format("%s:\\%s\\%s\\%04d-%02d-%02d\\Camera%d",
			m_sVirtualDisk,
			"SaveImages","GrabImages",
			curTime.GetYear(),
			curTime.GetMonth(),
			curTime.GetDay(),
			i+1);

		if(!inIsPathExist(m_sSaveImagePath[i]))
		{
			if(!inCreateMultiLevelPath(m_sSaveImagePath[i]))
			{
				CString str;
				str.Format("����·��ʧ��:%s",m_sSaveImagePath[i]);
				AfxMessageBox(str);
				return FALSE;
			}
		}
	}

	// ��ȡ����ͼ����
	m_iPicCountPerCam[Camera1] = GetPrivateProfileInt("PIO24B", "44", 12, strPIO24B[Camera1]);		//��ʱ����x�Ĵ�������
	m_iPicCountPerCam[Camera2] = GetPrivateProfileInt("PIO24B", "44", 12, strPIO24B[Camera1]);		//��ʱ����0�Ĵ�������
	m_iPicCountPerCam[Camera3] = GetPrivateProfileInt("PIO24B", "44", 12, strPIO24B[Camera1]);		//��ʱ����0�Ĵ�������
	m_iPicCountPerCam[Camera4] = GetPrivateProfileInt("PIO24B", "44", 12, strPIO24B[Camera1]);		//��ʱ����0�Ĵ�������
	m_iPicCountPerCam[Camera5] = GetPrivateProfileInt("PIO24B", "68", 0, strPIO24B[Camera1]);		//��ʱ����0�Ĵ�������
	m_iPicCountPerCam[Camera6] = GetPrivateProfileInt("PIO24B", "44", 12, strPIO24B[Camera2]);		 //��ʱ����x�Ĵ�������

	m_iPicCountPerCam[Camera1]++;
	m_iPicCountPerCam[Camera2]++;
	m_iPicCountPerCam[Camera3]++;
	m_iPicCountPerCam[Camera4]++;
	m_iPicCountPerCam[Camera5]++;
	m_iPicCountPerCam[Camera6]++;

	return TRUE;
}

// ����ϵͳ�����ļ�
BOOL CMainFrame::SaveSystemConfig(const CString &strLoadFile)
{
	int i{ 0 };
	CString strValue;
	/************************************************************************/
	m_iniAlgConfig.WriteInt(_T("SystemConfiguration"), _T("MaxNumberOfConstantKick"), m_iMaxNumOfConstantKick, _T("�������������ֵ"));
	//added by zhangfan[2017/04/19]
	CString csKey{ _T("") }, csValue{_T("")};
	csValue.Format(_T("%d	//�㷨����ͼ������"), m_iSaveImageType);
	WritePrivateProfileString(_T("SystemConfiguration"), _T("SaveImageType"), csValue, m_sSysConfigFileName);
	csValue.Format(_T("%d	//�Ƿ�ѭ����ͼ"), m_iIsCircle);
	WritePrivateProfileString("SystemConfiguration", _T("WhetherCirleSaveImage"), csValue, m_sSysConfigFileName);
	csValue.Format(_T("%d	//˳���ͼ������"), m_iSaveSeqCnt);
	WritePrivateProfileString(_T("SystemConfiguration"), _T("NumberOfSequentialImageSaved"), csValue, m_sSysConfigFileName);
	csValue.Format(_T("%d"), m_iSaveErrCnt);
	WritePrivateProfileString(_T("SystemConfiguration"), _T("NumberOfErrorImageSaved"), csValue, m_sSysConfigFileName);
	csValue.Format(_T("%d"), m_iSaveExistCnt);
	WritePrivateProfileString(_T("SystemConfiguration"), _T("NumberOfExistenceImageSaved"), csValue, m_sSysConfigFileName);
	for (int i = 0; i < c_AllElementCount; ++i)
	{
		csKey.Format(_T("WhetherSaveImageWithWorkspace%d"), i + 1);
		csValue.Format(_T("%d"), m_bSaveCameraImage[i] ? 1 : 0);
		WritePrivateProfileString("SystemConfiguration", csKey, csValue, m_sSysConfigFileName);
	}
	csValue.Format(_T("%d"), m_iLabelQueLen);
	WritePrivateProfileString(_T("SystemConfiguration"), _T("LabelQueueLength"), csValue, m_sSysConfigFileName);
	csValue.Format(_T("%d"), m_iDisBtwn5And6);
	WritePrivateProfileString(_T("SystemConfiguration"), _T("DistanceBetween5And6"), csValue, m_sSysConfigFileName);
	csValue.Format(TEXT("%d"), m_iSaveImgObj);
	WritePrivateProfileString(TEXT("SystemConfiguration"), TEXT("SaveImageObjection"), csValue, m_sSysConfigFileName);
	csValue.Format(TEXT("%d"), m_iSaveImgNamingRegulation);
	WritePrivateProfileString(TEXT("SystemConfiguration"), TEXT("SaveImageNamingRegulation"), csValue, m_sSysConfigFileName);
	/************************************************************************/
	/************************************************************************/
	/*                       [5/17/2017 ConanSteve]                         */
	for (int i = 0; i < c_AllElementCount; ++i)
	{
		CString sCameraCfgFilePath{ TEXT("") };
		sCameraCfgFilePath.Format(TEXT("CameraParm%d.ini"), i + 1);
		sCameraCfgFilePath = m_sWorkPath + TEXT("\\Config\\") + sCameraCfgFilePath;
		CString sValue{ TEXT("") };
		sValue.Format(TEXT("%d"), m_iExposureTime[i]);
		WritePrivateProfileString(TEXT("Camera"), TEXT("ExposureTime"), sValue, sCameraCfgFilePath);
	}
	CString sTimeElpaseToRead{ TEXT("") };
	sTimeElpaseToRead.Format(TEXT("%d"), m_iSG_ElapseTimeToRead);
	WritePrivateProfileString(TEXT("SystemConfiguration"), TEXT("TimeElpaseToReadImg"), sTimeElpaseToRead, m_sSysConfigFileName);
	/************************************************************************/
	strValue.Format("%d", m_iIntervalDisplayTime);	
	WritePrivateProfileString("ϵͳ����", "ͼ����ʾ���ʱ��", strValue, strLoadFile);

	strValue.Format("%d", m_iImageShowMode);
	WritePrivateProfileString("ϵͳ����", "��ʾȫ��ͼ��", strValue, strLoadFile);

	strValue.Format("%d", m_bOpenAlgCheck);
	WritePrivateProfileString("ϵͳ����", "�����㷨���", strValue, strLoadFile);

	strValue.Format("%d", m_bOpenImageSave);
	WritePrivateProfileString("ϵͳ����", "����ͼ�񱣴�", strValue, strLoadFile);


	// [2014-6-10 by zhy]	
	strValue.Format("%d",m_iMaxSaveImageCount);
	WritePrivateProfileString("ϵͳ����", "����ͼ������", strValue, strLoadFile);

	strValue.Format("%d", m_iPreAllocateSaveInfoElementCount);	
	WritePrivateProfileString("ϵͳ����", "Ԥ���䱣��ͼ��Ԫ�ظ���", strValue, strLoadFile);

	strValue.Format("%d", m_iLabelErrorCount[Camera1]);	
	WritePrivateProfileString("ϵͳ����", "��ǩλ�������������", strValue, strLoadFile);

	strValue.Format("%d",m_iCheckSpeedInterval);
	WritePrivateProfileString("ϵͳ����", "����ٶ�ʱ����", strValue, strLoadFile);

	strValue.Format("%d",m_iSaveImage_Abnormal);
	WritePrivateProfileString("ϵͳ����", "��������쳣ͼ������", strValue, strLoadFile);

	strValue.Format("%d",m_iRecord_Set);
	WritePrivateProfileString("ϵͳ����", "���������־��¼����", strValue, strLoadFile);

	strValue.Format("%d",m_iErrorSignal);
	WritePrivateProfileString("ϵͳ����", "�����źż��",strValue, strLoadFile);

	strValue.Format("%d",m_iLicenseExpDate);
	WritePrivateProfileString("ϵͳ����", "License����", strValue, strLoadFile);

	strValue.Format("%d		//����λ�õ����6�Ĺ��Ӹ���",m_iKickOffset5);
	WritePrivateProfileString("ϵͳ����", "����λ�õ����6�Ĺ��Ӹ���", strValue, strLoadFile);

	strValue.Format("%d		//��ʾ�豸 0Ϊϵͳͼ�� 1Ϊ�㷨ͼ��",m_iShowDevice);
	WritePrivateProfileString("ϵͳ����", "��ʾ�豸", strValue, strLoadFile);

	strValue.Format("%d		//ϵͳ�Ƿ񱣴�ͼ�� 0Ϊ��1Ϊ��",m_iSaveImageSys);
	WritePrivateProfileString("ϵͳ����", "ϵͳ�Ƿ񱣴�ͼ��", strValue, strLoadFile);

	strValue.Format("%d",m_iAlarmCardNum);
	WritePrivateProfileString("�ӿڿ�����","�����ź���ʹ�õĽӿڿ����",strValue, strLoadFile);
	
	strValue.Format("%d",m_iAlarmOutPort);
	WritePrivateProfileString("�ӿڿ�����","�����ź���ʹ�õ�����˿ں�",strValue, strLoadFile);

	CString  strSession;	
	for (int i=0;i<c_AllElementCount;i++)
	{
		strSession.Format("��λ%dֹͣ�����ж�����",i+1);
		strValue.Format("%d",m_iStopCountDiff[i]);
		WritePrivateProfileString("ϵͳ����", strSession, strValue, strLoadFile);

		strSession.Format("��λ%d�㷨�������",i+1);
		strValue.Format("%d",m_iInspectionObj[i]);
		WritePrivateProfileString("ϵͳ����", strSession, strValue, strLoadFile);

		strSession.Format("��λ%d�㷨����ʱ��",i+1);
		strValue.Format("%d", m_iKickInterval[i]);	
		WritePrivateProfileString("ϵͳ����", strSession, strValue, strLoadFile);

		strSession.Format("���%d�Ƿ���",i+1);
		strValue.Format("%d", m_bIsCheck[i]);	
		WritePrivateProfileString("ϵͳ����", strSession, strValue, strLoadFile);
		
		strSession.Format("���%dһ�����ڲɼ�ͼ�����",i+1);
		strValue.Format("%d", m_iGrabCountSingleCircle[i]);	
		WritePrivateProfileString("ϵͳ����", strSession, strValue, strLoadFile);	
	}

	//Bayer�任��ر���
	for (i=0;i<CAMERA_COUNT;i++)
	{		
		strSession.Format("���%dBayer�任����",i+1);		
		strValue.Format("%d", m_BayerTransform[i].m_bAreaCamBayer);	
		//WritePrivateProfileString(strSession, "�Ƿ����Bayer�任", strValue, strLoadFile);
		
		strValue.Format("%f", m_BayerTransform[i].m_dBayerRedGain);	
		WritePrivateProfileString(strSession, "Bayerת��Red����", strValue, strLoadFile);
		
		strValue.Format("%f", m_BayerTransform[i].m_dBayerGreenGain);	
		WritePrivateProfileString(strSession, "Bayerת��Green����", strValue, strLoadFile);
		
		strValue.Format("%f", m_BayerTransform[i].m_dBayerBlueGain);	
		WritePrivateProfileString(strSession, "Bayerת��Blue����", strValue, strLoadFile);
		
		strSession.Format("���%d�Ƿ�ת",i+1);
		strValue.Format("%d",m_bGrabFlip[i]);
		WritePrivateProfileString("�ɼ�ͼ��ת", strSession, strValue, strLoadFile);

		strSession.Format("���%d��תģʽ",i+1);
		strValue.Format("%d",m_eShowModeFlip[i]);
		WritePrivateProfileString("�ɼ�ͼ��ת", strSession, strValue, strLoadFile);		
	}

	// ���ʹ�õ�������
	for(i=0;i<CAMERA_COUNT;i++)
	{
		strSession.Format("���%d��ʹ�õĽӿڿ����",i+1);
		strValue.Format("%d",m_iCamCardNum[i]);
		WritePrivateProfileString("�ӿڿ�����", strSession, strValue,strLoadFile);
		
		strSession.Format("���%d��ʹ�õ�����˿ں�",i+1);
		strValue.Format("%d",m_iCamOutPort[i]);
		WritePrivateProfileString("�ӿڿ�����", strSession, strValue,strLoadFile);
	}
	return TRUE;
}

//����Bayer����
BOOL CMainFrame::SetLutTable(double dRatioR,double dRatioG,double dRatioB) 
{
	if(dRatioR <=0)
		return FALSE;
	if(dRatioG <=0)
		return FALSE;
	if(dRatioB <=0)
		return FALSE;
	
	for(int i=0;i<256;i++)
	{
		m_pLutR[i] = min((int)(i*dRatioR),255);
		m_pLutG[i] = min((int)(i*dRatioG),255);
		m_pLutB[i] = min((int)(i*dRatioB),255);
	}
	return TRUE;
}

// ��ʼ���ӿڿ�
inline BOOL CMainFrame::InitIOCard()
{	
	try{
		for(int i=0;i<IOCARD_COUNT;i++)
		{
			BOOL  bRet = FALSE;

			if(Camera1==i)
			{
				bRet= m_IOCard[i].openCard(0);
				
				if(bRet==FALSE)
				{
					AfxMessageBox("Open IOCard Failed!");
					return FALSE;
				}
				
				m_IOCard[i].hardReset();
				m_IOCard[i].initRegisters("PIO24B_reg_init.txt", "PIO24B");
				m_IOCard[i].softReset();	
			}	
			else if(Camera2==i)
			{
				bRet= m_IOCard[i].openCard(1);
				
				if(bRet==FALSE)
				{
					AfxMessageBox("Open IOCard Failed!");
					return FALSE;
				}
				
				m_IOCard[i].hardReset();
				bRet=m_IOCard[i].initRegisters("PIO24B_reg_init_2.txt", "PIO24B");
				m_IOCard[i].softReset();	
			}
		}
	}
	catch (...)
	{
		CString strExceptionDes("");
		strExceptionDes.Format("InitIOCard:�ӿڿ���ʼ���쳣!");
		AfxMessageBox(strExceptionDes);
		return FALSE;
	}
	
	return TRUE;
}

// �ͷŽӿڿ�
inline BOOL CMainFrame::ReleaseIOCard()
{
	
#ifdef USE_IO_CARD
	if(m_bIOCardValid && m_bIOCardInitSuccess)
	{
		try
		{
			for(int i=0;i<IOCARD_COUNT;i++)
			{
				m_IOCard[i].enable(false);	      // �ر�ʹ�ܽӿڿ� [1/18/2012 dh]
				m_IOCard[i].closeCard();
			}
		}
		catch (...)
		{
			CString  strExceptionDes;  
			strExceptionDes.Format("StopCheck-���ýӿڿ��쳣!");
			m_LogManager.MyWriteLogFile("�ӿڿ�����",strExceptionDes,AbnormityLog);
		}
	}
#endif
	
	return TRUE;
}

// ��ʼ���ɼ�����
inline BOOL CMainFrame::InitGrabber()
{
	BOOL bRet = FALSE;
	int iDeviceSN  = 0;		     // ����豸���	
	
	CString str;				 // �ַ���ͨ�ñ���
	CString strCameraDeviceName; // �������
	CString strDeviceMark;       // �豸��ʶ
	CString strDeviceInitFile;	 // �豸��ʼ���ļ���
	CString strSession;         // �ֶ�����

	for (int i=0;i<CAMERA_COUNT;i++)
	{
		
		strSession.Format("�豸%c���",'A'+i);  
		iDeviceSN = GetPrivateProfileInt("�ɼ�����", strSession, -1, m_sSysConfigFileName);//�豸���
		
		//У��������
		if (iDeviceSN > CAMERA_COUNT)
		{
			CString strExceptionDes("");
			strExceptionDes.Format("InitGrabber:���%d���%d��������������!",i+1,iDeviceSN);
			AfxMessageBox(strExceptionDes);
			return FALSE;
		}

		////////////////////
		// ͨ�ð汾
		strSession.Format("�豸%c����",'A'+i);
		GetPrivateProfileString("�ɼ�����", strSession, "", strCameraDeviceName.GetBuffer(255), 255, m_sSysConfigFileName);
		
		//  [4/21/2010 GZ]
		strSession.Format("�豸%c��ʶ",'A'+i);
		GetPrivateProfileString("�ɼ�����", strSession, "", strDeviceMark.GetBuffer(255), 255, m_sSysConfigFileName);
	
		strSession.Format("�豸%c��ʼ���ļ���",'A'+i);
		GetPrivateProfileString("�ɼ�����", strSession, "", str.GetBuffer(255), 255, m_sSysConfigFileName);
		strDeviceInitFile.Format("%s\\config\\%s", m_sWorkPath, str);
		
		// ��������·�����ļ��Ƿ���ڵĶ���
		CFileFind searcher;		
		if(!searcher.FindFile(strDeviceInitFile))
		{
			CString strExceptionDes("");
			strExceptionDes.Format("��������:���%d�����ļ�%s�ļ�����ָ��·����",i+1,str);
			AfxMessageBox(strExceptionDes);
			return FALSE;
		}
		else
		{
			m_sCamConfigPath[i]=strDeviceInitFile;
			m_sCamConfigName[i]=str;
		}
		
		// ��ʼ���ɼ�����
		s_GBINITSTRUCT AreaGraber;
		AreaGraber.iGrabberTypeSN = iDeviceSN;
		AreaGraber.nGrabberSN = iDeviceSN;						// �豸���
#ifdef __RealGrabber_
		AreaGraber.nGrabberSN = iDeviceSN;						// �豸���
		sprintf(AreaGraber.strDeviceMark, strDeviceMark.GetBuffer(strDeviceMark.GetLength()));	// �豸����
#endif
		
		AreaGraber.Context	  = this;							// thisָ��
		sprintf(AreaGraber.strDeviceName, strCameraDeviceName.GetBuffer(strCameraDeviceName.GetLength()));	// �豸����
		sprintf(AreaGraber.strGrabberFile, strDeviceInitFile);	// �豸�ĳ�ʼ���ļ���
		
		AreaGraber.CallBackFunc = GlobalAreaGrabOverCallback1;	// �ص�����
		
		// ��ʼ��ͼ��
		
		bRet = m_CameraGrabber[i].Init(&AreaGraber);
	
		if (!bRet)
		{
			// ��ӳ�ʼ���쳣�������ִ�����Ϣ
			s_GBERRORINFO GrabberInfo;
			m_CameraGrabber[i].GetLastErrorInfo(&GrabberInfo);
			CString strExceptionDes;
			strExceptionDes.Format("���%d��ʼ��ʧ��!",i+1);
			strExceptionDes += GrabberInfo.strErrorDescription;
		 	AfxMessageBox(strExceptionDes);
			return FALSE;
		}
		else
		{
			CString sFilePath{ TEXT("") };
			sFilePath.Format(TEXT("%s"), AreaGraber.strGrabberFile);
			int pos = sFilePath.ReverseFind(TEXT('\\'));
			sFilePath = sFilePath.Right(sFilePath.GetLength() - 1 - pos);
			m_hEventSGActive[i] = CreateEvent(NULL, FALSE, FALSE, sFilePath);
			m_hEventComplete[i] = CreateEvent(NULL, FALSE, FALSE, sFilePath + TEXT("Complete"));
			TRACE("���%d�ɼ������ʼ���ɹ�!\n",i+1);
		}
//		bRet = m_CameraGrabber[i].StopGrab();
		//��ȡ�ɼ���ͼ���ȡ��߶ȡ�λ��
		m_CameraGrabber[i].GetParamInt(GBImageWidth, m_sCameraImageInfo[i].m_iCameraImageWidth);			// �������ͼ����
		m_CameraGrabber[i].GetParamInt(GBImageHeight, m_sCameraImageInfo[i].m_iCameraImageHeight);			// �������ͼ��߶�
		m_CameraGrabber[i].GetParamInt(GBImageBufferSize, m_sCameraImageInfo[i].m_iCameraImageSize);		// �������ͼ���С
		m_CameraGrabber[i].GetParamInt(GBImagePixelSize, m_sCameraImageInfo[i].m_iCameraImagePixelSize);	// �������ͼ�����ش�С
#ifdef __RealGrabber_
		if(m_BayerTransform[i].m_bAreaCamBayer)
		{
			m_sCameraImageInfo[i].m_iCameraImagePixelSize = m_sCameraImageInfo[i].m_iCameraImagePixelSize;  // ��ɫ���
			m_sCameraImageInfo[i].m_iCameraImageSize = m_sCameraImageInfo[i].m_iCameraImageSize;
		}
#endif
	
		if (m_sCameraImageInfo[i].m_iCameraImageWidth > m_iMaxCameraImageWidth)
		{
			m_iMaxCameraImageWidth = m_sCameraImageInfo[i].m_iCameraImageWidth;
		}
		
		if (m_sCameraImageInfo[i].m_iCameraImageHeight > m_iMaxCameraImageHeight)
		{
			m_iMaxCameraImageHeight = m_sCameraImageInfo[i].m_iCameraImageHeight;
		}
		
		if (m_sCameraImageInfo[i].m_iCameraImageSize > m_iMaxCameraImageSize)
		{
			m_iMaxCameraImageSize = m_sCameraImageInfo[i].m_iCameraImageSize;
		}
		
		if (m_sCameraImageInfo[i].m_iCameraImagePixelSize > m_iMaxCameraImagePixelSize)
		{
			m_iMaxCameraImagePixelSize = m_sCameraImageInfo[i].m_iCameraImagePixelSize;
		}

		//int iWidth=m_sCameraImageInfo[i].m_iCameraImageWidth*m_iPicCountPerCam[i];
		int iWidth = m_sCameraImageInfo[i].m_iCameraImageWidth;
		int iHeight= m_sCameraImageInfo[i].m_iCameraImageHeight;

		//�����ͼ���Сд��AlgCofig����
		CString strPath;
		strPath.Format("%s\\Config\\AlgConfig.ini",m_sWorkPath);

		CString strApp,strValue;
		strApp.Format("Camera%d Parameters",i+1);
		strValue.Format("%d				//���ͼ����",iWidth*m_iPicCountPerCam[i]);
		WritePrivateProfileString(strApp,"Width",strValue,strPath);

		strValue.Format(TEXT("%d           //ͼ������"), m_iPicCountPerCam[i]);
		WritePrivateProfileString(strApp, TEXT("PictureNumberPerCamera"), strValue, strPath);
	
		strValue.Format("%d				//���ͼ��߶�",iHeight);
		WritePrivateProfileString(strApp,"Height",strValue,strPath);

		//�������������ǩ��Ŀ��Ϊ1
		strValue.Format("%d				//ÿ���㷨��ⷵ�ؽ������",1);
		WritePrivateProfileString(strApp,"CheckOutNum",strValue,strPath);
		
		//�����������ṹ���С
		strValue.Format("%d				//�㷨���ʱ�ṹ��s_CheckInParam�� void* pParam�Ĵ�С",sizeof(LabelCheck::s_AlgInspInPara));
		WritePrivateProfileString(strApp,"CheckInSize",strValue,strPath);
		
		//����������ṹ���С
		strValue.Format("%d				//�㷨���ʱ�ṹ��s_CheckOutParam�� void* pParam�Ĵ�С",sizeof(LabelCheck::s_AlgInspResult));
		WritePrivateProfileString(strApp,"CheckOutSize",strValue,strPath);
		
	}	
	//////////////////////////////////////////////////////////////////////////
	return TRUE;
}

// �ͷŲɼ�����
inline BOOL CMainFrame::ReleaseGrabber()
{
	try
	{
		for (int i = 0; i < CAMERA_COUNT; i++)
		{
			m_CameraGrabber[i].Close();
			TRACE("�ر����%d\r\n", i + 1);
		}
	}
	catch (...)
	{
		TRACE(TEXT("Close Camera Fail!"));
	}


	return TRUE;
}

// ��ʼ���ڴ�������
inline BOOL CMainFrame::InitDataFunc()
{
	/************************************************************************/
	/*                       [6/29/2017 ConanSteve]                         */
	//m_pImgDataTemp.reset(new boost::shared_array<char>[CAMERA_COUNT]);
	/************************************************************************/
	
	int i = 0;
	int j = 0;
	int k = 0;
	int iImageSize=0;

	/************************************************************************/
	/*                [5/8/2017 ConanSteve]                     */
	for (int i = 0; i < m_iLabelQueLen; ++i)
	{
		s_5CheckInfo s5CheckInfo;
		//s5CheckInfo.bKick = true;
		m_sLabelRst.push(s5CheckInfo);
	}
	/************************************************************************/
	for (int i = 0; i < c_AllElementCount; ++i)
	{
		m_sElement[i] = new s_ElementInfo[m_iSysDataEleLen];
	}
	/************************************************************************/
	

	// ����洢Bayer�任�ڴ�
// 	for (i=0;i<CAMERA_COUNT;i++)
// 	{
// 		int iWidth=0,iHeight=0,iByte=0;
// 		iWidth = m_sCameraImageInfo[i].m_iCameraImageWidth;
// 		iHeight = m_sCameraImageInfo[i].m_iCameraImageHeight;
// 		iByte = m_sCameraImageInfo[i].m_iCameraImagePixelSize;
// 		m_pbRGBImage[i] = new BYTE[iWidth*iHeight*iByte];
// 	}

	
	// Ϊ�㷨ģ�����÷����ڴ�
	for(i=0;i<CAMERA_COUNT;i++)
	{	
		try
		{
// 
// 			m_pAlgElement[i] = new BYTE[m_iMaxCameraImageWidth*m_iMaxCameraImageHeight*m_sCameraImageInfo[i].m_iCameraImagePixelSize*m_iPicCountPerCam[i]];
// 			memset(m_pAlgElement[i], 0, m_iMaxCameraImageWidth*m_iMaxCameraImageHeight*m_sCameraImageInfo[i].m_iCameraImagePixelSize*m_iPicCountPerCam[i]);
		}
		catch (...)
		{
			CString  strExceptionDes;
			strExceptionDes.Format("InitDataFunc-���%d�����㷨ģ�����������ڴ����!",i+1);
			m_LogManager.MyWriteLogFile("��ʼ��",strExceptionDes,AbnormityLog);
			return FALSE;
		}
	}
	
	try
	{	
		for(i=0; i<c_AllElementCount; i++)
		{
			iImageSize = m_sCameraImageInfo[i].m_iCameraImageSize;
			int iTotal = iImageSize*m_iPicCountPerCam[i]*m_iSysDataEleLen;
			/*m_pImageAddr[i] = NULL;*/
			//m_pImageAddr[i] = shared_ptr<int>(new BYTE[iImageSize*m_iPicCountPerCam[i]*m_iSysDataEleLen], default_delete<BYTE[]>());
			m_pImageAddr[i] = shared_ptr<BYTE>(new BYTE[iImageSize*m_iPicCountPerCam[i] * m_iSysDataEleLen], [](BYTE*p) {
				delete[] p;
			});
			ZeroMemory(m_pImageAddr[i].get(),iImageSize*m_iPicCountPerCam[i]* m_iSysDataEleLen);
		}
	}
	catch (...)
	{
		CString  strExceptionDes;
		strExceptionDes.Format("InitDataFunc-����������Ϣ���пռ����!");
		m_LogManager.MyWriteLogFile("��ʼ��",strExceptionDes,AbnormityLog);
		return FALSE;
	}

// ��ʼ��BITMAP��Ϣ
	for(i=0;i<CAMERA_COUNT;i++)
	{
		m_pBitmapInfo[i] = NULL;
		m_pBitmapInfo[i] = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 256*sizeof(RGBQUAD)];;
		m_pBitmapInfo[i]->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		m_pBitmapInfo[i]->bmiHeader.biWidth = m_sCameraImageInfo[i].m_iCameraImageWidth;
		m_pBitmapInfo[i]->bmiHeader.biHeight = m_sCameraImageInfo[i].m_iCameraImageHeight;
		m_pBitmapInfo[i]->bmiHeader.biPlanes=1;
		m_pBitmapInfo[i]->bmiHeader.biBitCount = m_sCameraImageInfo[i].m_iCameraImagePixelSize*8;
		m_pBitmapInfo[i]->bmiHeader.biCompression = BI_RGB;
		m_pBitmapInfo[i]->bmiHeader.biSizeImage = 0;
		m_pBitmapInfo[i]->bmiHeader.biXPelsPerMeter =0;
		m_pBitmapInfo[i]->bmiHeader.biYPelsPerMeter = 0;
		m_pBitmapInfo[i]->bmiHeader.biClrUsed = 0;
		m_pBitmapInfo[i]->bmiHeader.biClrImportant = 0;

		//(2003-4-2)
		//����8λλͼ��˵������Ҫ��ɫ�塣����256ɫ��λͼ����ɫ������ĸ���λ256��
		for( int k = 0; k < 256; k++)
		{
			m_pBitmapInfo[i]->bmiColors[k].rgbBlue = (BYTE)k;
			m_pBitmapInfo[i]->bmiColors[k].rgbGreen = (BYTE)k;
			m_pBitmapInfo[i]->bmiColors[k].rgbRed = (BYTE)k;
			m_pBitmapInfo[i]->bmiColors[k].rgbReserved = 0;
		}
	}

	try
	{
		for(i=0; i<c_AllElementCount; i++)
		{
			m_hImageSaveStart[i] = CreateEvent(NULL,FALSE,FALSE,NULL);
			if(ERROR_ALREADY_EXISTS == GetLastError())
			{
				CString strExceptionDes("");
				strExceptionDes.Format("%s","����Ѿ�����--CreateEvent!");
				AfxMessageBox(strExceptionDes);
				return FALSE;
			}
			else if(NULL == m_hImageSaveStart[i])
			{
				CString strExceptionDes("");
				strExceptionDes.Format("%s","CreateEvent Fail!");
				AfxMessageBox(strExceptionDes);
				return FALSE;
			}
		}
	}
	catch (...)
	{
		CString strExceptionDes("");
		strExceptionDes.Format("%s","CreateEvent �쳣!");
		AfxMessageBox(strExceptionDes);
		return FALSE;
	}

	return TRUE;
}

// �ͷ��ڴ�������
inline BOOL CMainFrame::ReleaseDataFunc()

{
#ifdef DEBUG
	if (g_bCSRecord)
		m_myFileTickResult.Close();
#endif
	int i = 0;
	int j = 0;
	int k = 0;
	int  iAllInfoElementCount=0;
	CString strinfo;
	/************************************************************************/
	for (int i = 0; i < c_AllElementCount; ++i)
	{
		delete []m_sElement[i];
	}



	TRACE("-----�ͷ� RGB------\r\n");




	TRACE("-----�ͷ� Alg------\r\n");

	for(i=0;i<c_AllElementCount;i++)
	{
		if(i==Camera2)
		{
			int ii=0;
		}

// 		if(m_pImageAddr[i]!=NULL)
// 		{
// 			delete m_pImageAddr[i];
// 			m_pImageAddr[i]=NULL;
// 		}

		TRACE("-----�ͷ� 1------\r\n");
	}
	TRACE("-----�ͷ� Addr------\r\n");


	// �ͷ�ͼ����Ϣ
	for(i=0;i<CAMERA_COUNT;i++)
	{
		if(m_pBitmapInfo[i]!=NULL)
		{
			delete m_pBitmapInfo[i];
			m_pBitmapInfo[i]=NULL;
		}
	}

	TRACE("-----�ͷ� Bitmap------\r\n");

	return TRUE;
}

//����ص���1��λ�����
void CMainFrame::GlobalAreaGrabOverCallback1 (const s_GBSIGNALINFO* SigInfo)
{
	if (SigInfo && SigInfo->Context)
    {
        CMainFrame* pMF = (CMainFrame*) SigInfo->Context;
		pMF->AreaGrabOverCallback1(SigInfo);
    }
}
void CMainFrame::AreaGrabOverCallback1(const s_GBSIGNALINFO* SigInfo)
{
	int nGrabComplete=0;
	int iGrabSN=0;
	iGrabSN=SigInfo->nGrabberSN;

 	if(!m_bIsDetecting)
 	{
 		return;
 	}

#ifdef __RealGrabber_
	iGrabSN=SigInfo->nGrabberSN;
#endif
	//�����Ų��� ��Ҫ���д���
	if(iGrabSN<Camera1)
	{
		CString  strExceptionDes;
		strExceptionDes.Format("AreaGrabOverCallback-�����ų���!",SigInfo->nGrabberSN+1);
		m_LogManager.MyWriteLogFile("�ɼ��ص�",strExceptionDes,AbnormityLog);
		return; 
	}

//ʵ�ʲɼ�ʹ��
#ifdef __RealGrabber_  
	try
	{
		// ��ñ���������  
		if(m_bIOCardValid)
		{
			//������Ŀʵ���ź�������  �����������ź�(����ĿΪ����ź�)
			ReadMachineSignal(iGrabSN);
		}

		bool bRet{ false };
		if (e_SysDetectMode_Multi == m_iSysDetectMode)
			bRet = CameraGrabFuncSingle(iGrabSN, iGrabSN);
		else if (e_SysDetectMode_Single == m_iSysDetectMode)
			bRet = CameraGrabFuncSingleEx(iGrabSN, iGrabSN);
		if (!bRet)
		{
			CString  strExceptionDes;
			strExceptionDes.Format("CameraGrabFuncSingle-���%d�ɼ�����FALSE!",SigInfo->nGrabberSN+1);
			m_LogManager.MyWriteLogFile("�ɼ��ص�",strExceptionDes,OperationLog);
			return;
		}

	}
	catch (...)
	{
		//**************************************����Ҫ��һ������**********************//
		TRACE("------------------�ɼ��ص�����!!!-----------------------\n");
		CString  strExceptionDes;
		strExceptionDes.Format("AreaGrabOverCallback-�ɼ��ص�����!");
		m_LogManager.MyWriteLogFile("�ɼ��ص�",strExceptionDes,AbnormityLog);
	}

#else
	//ģ��ɼ�

	try
	{	
		//����Ĳɼ�����
		bool bRet{ false };
		if (e_SysDetectMode_Multi == m_iSysDetectMode)
			bRet = CameraGrabFuncSingle(iGrabSN, iGrabSN);
		else if(e_SysDetectMode_Single == m_iSysDetectMode)
			bRet = CameraGrabFuncSingleEx(iGrabSN, iGrabSN);
		if (!bRet)
		{
			CString  strExceptionDes;
			strExceptionDes.Format("CameraGrabFuncSingle-���%d�ɼ�����FALSE!",SigInfo->nGrabberSN+1);
			m_LogManager.MyWriteLogFile(TEXT("�ɼ��ص�"),strExceptionDes,OperationLog);
			return;
		}

	}
	catch (...)
	{
		TRACE("-----------ģ��ɼ�����!--------------\n");
		CString  strExceptionDes;
		strExceptionDes.Format("AreaGrabOverCallback-ģ��ɼ�����!");
		m_LogManager.MyWriteLogFile("�ɼ��ص�",strExceptionDes,AbnormityLog);
	}
	
#endif
}

// ���������߳�                       
inline BOOL CMainFrame::StartAllThread()
{
	// ��ȡCPU�������Ϣ [2/17/2008 Dufeng]
	DWORD dwProcessAffinityMask = 0;
	DWORD dwSystemAffinityMask = 0;
	::GetProcessAffinityMask(GetCurrentProcess(), &dwProcessAffinityMask, &dwSystemAffinityMask) ;	

	CWinThread* pTemp = NULL;

	// �ȴ�����DEMO
	pTemp=NULL;
	pTemp = AfxBeginThread((AFX_THREADPROC)WaitDemoMould,this);
	if(NULL==pTemp)
	{
		return FALSE;
	}

	// ��ȡͼ���
	pTemp=NULL;
	pTemp = AfxBeginThread((AFX_THREADPROC)GetIOCardImageNumThread,this);
	if(NULL==pTemp)
	{
		return FALSE;
	}

	// �������ϵͳͼ��������ͼ�߳�
	pTemp = NULL;

	return TRUE;
}

// ɱ�������߳�
inline BOOL CMainFrame::KillAllThread()
{	
	DWORD  wlc = 0;

	m_bWaitDemoMould=TRUE;
	m_bStopReadKickImageNo=TRUE;
	m_bKillSaveImageThread=TRUE;

	g_pView->m_bQuitAlarmThread = TRUE;

	// ��ȡ�����߳�
	m_bCloseCount=TRUE;
	wlc = WaitForSingleObject(m_evtGetIOCardImageNum,100);
	if(wlc==WAIT_OBJECT_0)
	{
		TRACE("...��ȡ�����߳��˳��ɹ�!\n");
	}


	wlc = WaitForSingleObject(m_evtKillWait,100);
	if(wlc==WAIT_OBJECT_0)
	{
		TRACE("...�ȴ�Demo�߳��˳��ɹ�!\n");
	}
	
	return TRUE;
}

DWORD CMainFrame::WaitDemoMould(LPVOID lpParam)
{
	CMainFrame* This = (CMainFrame*)lpParam;
	DWORD dRet = This->WaitDemoMouldFunc();	
	return dRet;
}

DWORD CMainFrame::WaitDemoMouldFunc()
{
	DWORD dRet=0;
	
	while(FALSE==m_bWaitDemoMould)
	{
		dRet = WaitForSingleObject(m_evtTrigDemoMould,10);	
		if(WAIT_OBJECT_0==dRet)
		{
			CTubeCheckView* pView = (CTubeCheckView*) g_pView;
			pView->LoadLightAndProductInfo(m_sModelName,m_sOldModelName);
		}
		
		Sleep(10);
		
	}
	
	m_evtKillWait.SetEvent();

	return 0;
}

BOOL  CMainFrame::ThreadDisplayImage( int iCamGroup, int iIndex) //��ʾͼ��
{

	if (g_pView != NULL)
	{	
		g_pView->DisplayImage(iCamGroup, iIndex);
	}

	return TRUE;
}


int CMainFrame::SaveAbnormityImageFunc(const char* pPath,int iWidth, int iHeight, int iByte, unsigned char* pAddr)
{
	// �����ļ���
	int iImageSize;
	iImageSize = iWidth*iHeight*iByte;

	CString str1,str2,strSavePath1;
	str1.Format("%s",pPath);
	if(FALSE==inIsPathExist(str1))
	{
		if(FALSE==inCreateMultiLevelPath(str1))
		{
			str2.Format("%s �ļ��д���ʧ��!",str1);
			return 0;
		}
	}
	
	for(int i=0;i<m_iPicCountPerCam[Camera1];i++)
	{
		strSavePath1.Format("%s\\%d.bmp",str1,i+1);
		savebmp(strSavePath1,iWidth,iHeight,iByte,pAddr+iImageSize);
	}
	
	return 1;
}

int CMainFrame::savebmp(const char *lpszPathName,int W,int H, int nByte, unsigned char *lpDest)
{
	DWORD dwSize = W*H*nByte;
	BITMAPFILEHEADER bmFH{0};
	int nPaletteEntries = 256;
	
	if(nByte<3)
	{
		nPaletteEntries = 256;
	}
	else
	{
		nPaletteEntries = 0;
	}
	
	bmFH.bfType = 19778;
	//	bmFH.bfType = "BM";
	
	bmFH.bfSize = (DWORD) ( sizeof(BITMAPFILEHEADER) + 
		sizeof(BITMAPINFOHEADER) +
		nPaletteEntries * sizeof(RGBQUAD)) +
		dwSize;
	bmFH.bfReserved1 = bmFH.bfReserved2 = 0;
	bmFH.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + 
		sizeof(BITMAPINFOHEADER) + 
		nPaletteEntries * sizeof(RGBQUAD);
	
	BITMAPINFO *FileBMP;
	char bmpBuf[2048];
	FileBMP = (BITMAPINFO *)bmpBuf;
	FileBMP->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	FileBMP->bmiHeader.biWidth = W;
	FileBMP->bmiHeader.biHeight = H;
	FileBMP->bmiHeader.biPlanes = 1;
	FileBMP->bmiHeader.biBitCount = nByte*8;
	FileBMP->bmiHeader.biCompression = BI_RGB;
	FileBMP->bmiHeader.biSizeImage = 0;
	FileBMP->bmiHeader.biXPelsPerMeter = 0;
	FileBMP->bmiHeader.biYPelsPerMeter = 0;
	FileBMP->bmiHeader.biClrUsed = 0;
	FileBMP->bmiHeader.biClrImportant = 0;
	
	//(2003-4-2)
	//����8λλͼ��˵������Ҫ��ɫ�塣����256ɫ��λͼ����ɫ������ĸ���λ256��
	for( int k = 0; k < 256; k++)
	{
		FileBMP->bmiColors[k].rgbBlue = (BYTE)k;
		FileBMP->bmiColors[k].rgbGreen = (BYTE)k;
		FileBMP->bmiColors[k].rgbRed = (BYTE)k;
		FileBMP->bmiColors[k].rgbReserved = 0;
	}
	
	
	CFile ImageFile;
	DWORD dwOffset = 0;
	
	if( ImageFile.Open(lpszPathName, CFile::modeCreate | CFile::modeReadWrite) )
	{
		ImageFile.Write( &bmFH, sizeof(BITMAPFILEHEADER) ); 
		ImageFile.Write( &FileBMP->bmiHeader, sizeof(BITMAPINFOHEADER) );
		ImageFile.Write( &FileBMP->bmiColors[0], sizeof(RGBQUAD) * nPaletteEntries);
		ImageFile.Write( lpDest, dwSize );	
		ImageFile.Close();
		return 1;
	}
	else
		return -1;
	
}

inline BOOL CMainFrame::LoadAllImage()
{
	CFileFind fd;
	CString strSrcRootPath;
	strSrcRootPath.Format("%s:\\SaveImages",m_sVirtualDisk);
	BOOL bFind = fd.FindFile(strSrcRootPath+"\\*.*");
	CString strSrcPath,strCameraName;
	CString strImageType;
		
	while(bFind)
	{
		//����Ŀ¼�������ļ�������
		bFind=fd.FindNextFile();
		if(!fd.IsDots())//AbnormityImages/ErrorImages/GrabImages
		{
			CFileFind fd1;
			strSrcPath.Format("%s\\%s",strSrcRootPath,fd.GetFileName());
			BOOL bFind1=fd1.FindFile(strSrcPath+"\\*.*");
			while (bFind1)
			{
				bFind1=fd1.FindNextFile();
				if (!fd1.IsDots())//2011��4��14��
				{
					CFileFind fd2;
					strSrcPath.Format("%s\\%s\\%s",strSrcRootPath,fd.GetFileName(),fd1.GetFileName());

					BOOL bFind2=fd2.FindFile(strSrcPath+"\\*.*");
					while (bFind2)
					{
						bFind2=fd2.FindNextFile();
						if (!fd2.IsDots())//Camera1
						{
							CFileFind fd3;
							strSrcPath.Format("%s\\%s\\%s\\%s",strSrcRootPath,fd.GetFileName(),fd1.GetFileName(),fd2.GetFileName());
							strCameraName.Format("%s",fd2.GetFileName());
							BOOL bFind3=fd3.FindFile(strSrcPath+"\\*.*");
							while (bFind3)
							{
								bFind3=fd3.FindNextFile();
								if (!fd3.IsDots()&&!fd3.IsDirectory())//.bmp
								{
									CString strDstRootPath;
									CString strDstPath;
									CString strErr;

									CString strBmpFilePath;
									strBmpFilePath.Format("%s",fd3.GetFilePath());
									strImageType.Format("%s",fd.GetFileName());

									strBmpFilePath+='\0';
									OP_Directory(strBmpFilePath,NULL,FO_DELETE,FOF_NOCONFIRMATION | FOF_SIMPLEPROGRESS |FOF_NOERRORUI);
								}//.bmp ��If������
							}   
						}// Camera��If������
					}
				}// ���ڵ�If������
			}
		}
	}

	// ��������Ի�����������ƶ��ļ��������

	return TRUE;
}

void CMainFrame::SoftResetIOCard(CString strModelName)
{
	int iParam1=0,iParam2=0;
	int iNewParam1=0,iNewParam2=0;

	m_bSoftResetIOCard = TRUE;
	CTubeCheckView* pView = (CTubeCheckView*)GetActiveView();

	KillTimer(ID_START_CHECK_TIMER);
	KillTimer(ID_UPDATE_DEFECT_INFO_TIMER);
	if (m_bIOCardValid && m_bIOCardInitSuccess) //�ӿڿ���Ч�ҳ�ʼ���ɹ�
	{
		try
		{
			for (auto &a : m_IOCard)
			{
				a.softReset();
			}
		}
		catch (...)
		{
			CString  strExceptionDes;
			strExceptionDes.Format("StartCheck-���ýӿڿ��쳣!");
			m_LogManager.MyWriteLogFile("�ӿڿ�����",strExceptionDes,AbnormityLog);
		}
	}
	
	
	int i=0,j=0;
	for(i=0;i<c_AllElementCount;i++)
	{
		m_iGrabCircleCount[i]=0;   //���ɼ������ָ�Ϊ0
		m_iCheckErrorCount[i]=0;
		m_iImageCountOverFlowNumber[i]=0;		
		m_iFlowOverImageCountInPE[i]=0;				
		m_iKickFlowNumber[i]=0;						
		
		m_iLastImageCount[i]=0;	
		m_iLastHardImageCount[i]=0;
		m_iLastImageCountInPE[i]=0;	
		m_iLastKickCount[i]=0;							
		m_iLastImageID[i]=0;	
		m_iCheckTubeCnt[i] = 0;
		m_iCheckErrorCount[i] = 0;

		for(j=0;j<MAX_DEFECT_TYPE_COUNT;j++)
		{
			m_iDefectCount[i][j]=0;
		}
	}

	for(i=0;i<c_AllElementCount;i++)
	{
		m_iRC_PE[i]=0;								// ��¼���
		m_iFrameCount[i]=0;
		m_iFrameCountOverFlowNumber[i]=0;
		m_iLastFrameCount[i]=0;
	}

	for(i=0;i<CAMERA_COUNT;i++)
	{
		m_iCamSN[i]=0;
		m_iPicCount[i]=0;
	}

	m_iLastGoodCntTotle = 0;
	m_iLastCheckCntTotle = 0;
	m_iCheckCountTotal=0;										// ���ݿ������õļ������
	m_iCheckErrorCountTotal=0;
	m_iCheckGoodCountTotal=0;
	m_iEmptyCount=0;

	pView->m_TotalData.Display("0","",0,FALSE);					//�ϸ�Ʒ������
	pView->m_TotalGoodData.Display("0","",0,FALSE);				//���ϸ�Ʒ��������
	pView->m_TotalGoodRatioData.Display("0.0%","",0,FALSE);		//�ϸ��ʹ���


	// �������κ����³�ʼ�����ݿ�
	InitDataBase();
}

// �������ݿ�
BOOL CMainFrame::ADOConnectDB()
{
	CString strContext;
	HRESULT hr;
	_bstr_t connStr;
	strContext.Format("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s\\MP.mdb;Persist Security Info=False",m_sWorkPath);
	connStr=strContext;
	
	try
	{
		(m_Connect).CreateInstance(__uuidof(Connection));
		hr = m_Connect->Open(connStr,"","",adModeUnknown);
	}
	catch (_com_error e)
	{
		AfxMessageBox((char*)e.Description());
	}
	
	m_RecordSet.CreateInstance(__uuidof(Recordset));
	
	return TRUE;
}

//�ж����ݿ��е����ݱ��Ƿ���� 
BOOL CMainFrame::ExistTable(CString strTable)
{
	_variant_t RecordsAffected;
	_bstr_t connStr;
	CString  strSQL;

	// ��ȡϵͳ����ϵͳ���в�ѯ�Ƿ��в��ҵı�
	strSQL.Format("select * from MSysObjects where Name='%s'",strTable);
	connStr = strSQL;
	
	(m_RecordSet).CreateInstance(__uuidof(Recordset));
	
	try
	{
		(m_RecordSet)->Open(strSQL.GetBuffer(strSQL.GetLength()),
			_variant_t((IDispatch*)(m_Connect),true),
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch(_com_error* e)
	{
		AfxMessageBox(e->ErrorMessage());
		return 0;
	}
	
	if(!m_RecordSet->ADOEOF)
	{
		(m_RecordSet)->MoveFirst();
	}
	else
	{
		m_RecordSet->Close();
		return 0;
	}

	long lCount=0;

	while(!m_RecordSet->ADOEOF)
	{
		m_RecordSet->MoveNext();
		lCount++;
	}

	if(lCount<=0)
	{
		return FALSE;
	}
	
	m_RecordSet->Close();

	return TRUE;
}

BOOL CMainFrame::CreateTable(CString strTable)
{
	_bstr_t sSql;

	CString strSQL;

	if(strTable=="All_Check_Info")
	{
		strSQL.Format("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
			"Create table ",
			strTable,
			"( ID int IDENTITY (1, 1) NOT NULL PRIMARY KEY,",
			"StartTime datetime,",
			"StopTime datetime,",
			"ModelName char(255),",
			"TotalCount char(255),",
			"TotalGood char(255),",
			"TotalBad char(255),",
			"TotalBadRatio char(255),",
			"Total1 char(255),",
			"Good1 char(255),",
			"Bad1 char(255),",
			"BadRatio1 char(255),",
			"Total2 char(255),",
			"Good2 char(255),",
			"Bad2 char(255),",
			"BadRatio2 char(255))");
	}
	else
	{
		strSQL.Format("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
			"Create table ",
			strTable,
			"( ID int IDENTITY (1, 1) NOT NULL PRIMARY KEY,",
			"StartTime datetime,",
			"StopTime datetime,",
			"ModelName char(255),",
			"TotalCount int,",
			"TotalGood int,",
			"TotalBad int,",
			"Total1 int,",
			"Bad1 int,",
			"Total2 int,",
			"Bad2 int,",
			"Type1 int,",
			"Type2 int,",
			"Type3 int,",
			"Type4 int,",
			"Type5 int,",
			"Type6 int,",
			"Type7 int,",
			"Type8 int,",
			"Type9 int,",
			"Type10 int,",
			"Type11 int,",
			"Type12 int,",
			"Type13 int,",
			"Type14 int,",
			"Type15 int,",
			"Type16 int,",
			"Type17 int,",
			"Type18 int,",
			"Type19 int,",
			"Type20 int,",
			"Type21 int,",
			"Type22 int,",
			"Type23 int,",
			"Type24 int,",
			"Type25 int,",
			"Type26 int,",
			"Type27 int,",
			"Type28 int,",
			"Type29 int,",
			"Type30 int,",
			"Type31 int,",
			"Type32 int,",
			"Type33 int,",
			"Type34 int,",
			"Type35 int,",
			"Type36 int,",
			"Type37 int,",
			"Type38 int,",
			"Type39 int,",
			"Type40 int,",
			"Type41 int,",
			"Type42 int,",
			"Type43 int,",
			"Type44 int,",
			"Type45 int,",
			"Type46 int,",
			"Type47 int,",
			"Type48 int,",
			"Type49 int,",
			"Type50 int,",
			"Type51 int,",
			"Type52 int,",
			"Type53 int,",
			"Type54 int,",
			"Type55 int,",
			"Type56 int,",
			"Type57 int,",
			"Type58 int,",
			"Type59 int,",
			"Type60 int,",
			"Type61 int,",
			"Type62 int,",
			"Type63 int,",
			"Type64 int,",
			"Type65 int,",
			"Type66 int,",
			"Type67 int,",
			"Type68 int,",
			"Type69 int,",
			"Type70 int,",
			"Type71 int,",
			"Type72 int,",
			"Type73 int,",
			"Type74 int,",
			"Type75 int,",
			"Type76 int,",
			"Type77 int,",
			"Type78 int,",
			"Type79 int,",
			"Type80 int)");
	}


	try
	{
		sSql = strSQL;		
		_CommandPtr rCommand;
		rCommand.CreateInstance(__uuidof(Command));
		
		_variant_t vNULL;
		vNULL.vt = VT_ERROR;
		vNULL.scode = DISP_E_PARAMNOTFOUND;
		rCommand->ActiveConnection = m_Connect;
		rCommand->CommandText = sSql;
		rCommand->Execute(&vNULL,&vNULL,adCmdText);

	}
	catch (_com_error* e)
	{
		AfxMessageBox(e->ErrorMessage());
		return FALSE;
	}

	return TRUE;
}


void CMainFrame::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (nHitTest==HTCAPTION)
	{
		return;
	}

	CFrameWnd::OnNcLButtonDown(nHitTest, point);
}

void CMainFrame::InitDataBase()
{
	m_bSoftResetIOCard = FALSE;

	// ��ʼ���ܵļ����Ϣ��
	CTime CurrTime = CTime::GetCurrentTime();
	m_sStartTime.Format("%d-%d-%d %d:%d:%d",CurrTime.GetYear(),CurrTime.GetMonth(),CurrTime.GetDay(),
		CurrTime.GetHour(),CurrTime.GetMinute(),CurrTime.GetSecond());
	
	//All_Check_Info��
	CString strTotalCount,strTotalGood,strTotalBad, strTotalBadRatio;
	strTotalCount.Format("%d",m_iCheckCountTotal);
	strTotalBad.Format("%d",m_iCheckErrorCountTotal);
	strTotalGood.Format("%d",m_iCheckGoodCountTotal);
	
	if(m_iCheckErrorCountTotal<=0||m_iCheckCountTotal<=0)
	{
		strTotalBadRatio.Format("%0.3f%%",0.0);
	}
	else
	{
		strTotalBadRatio.Format("%0.3f%%",((float)100*m_iCheckErrorCountTotal)/((float)m_iCheckCountTotal));
	}
	

	s_CheckResultInfo info;
	info.strStartTime=m_sStartTime;
	info.strStopTime=m_sStartTime;
	info.strModelName=m_sModelName;
	info.strTotalCount= strTotalCount;
	info.strTotalGood = strTotalGood;
	info.strTotalBad=strTotalBad;
	info.strTotalBadRatio=strTotalBadRatio;
	info.strTotal1="0";
	info.strGood1="0";
	info.strBad1="0";
	info.strBadRatio1="0";
	info.strTotal2="0";
	info.strGood2="0";
	info.strBad2="0";
	info.strBadRatio2="0";

	InitTableData("All_Check_Info");
	InsertTableData(info,"All_Check_Info");
	SetTimer(ID_START_CHECK_TIMER, 60000, NULL);

	// ȱ����Ϣͳ�Ʊ�
	int i=0,j=0;
	CString strTemp;
	strTemp.Format("%s","Defect_Statistic_Info");
	s_CheckDefectInfo iDefectInfo;
	iDefectInfo.sStartTime=m_sStartTime;
	iDefectInfo.sStopTime=m_sStartTime;
	iDefectInfo.sModelName=m_sModelName;
	iDefectInfo.iTotalCount=0;
	iDefectInfo.iTotalGood=0;
	iDefectInfo.iTotalBad=0;
	iDefectInfo.iTotal1=0;
	iDefectInfo.iBad1=0;
	iDefectInfo.iTotal2=0;
	iDefectInfo.iBad2=0;

	for(i=0;i<c_AllElementCount;i++)
	{
		for(j=0;j<MAX_DEFECT_TYPE_COUNT;j++)
		{
			iDefectInfo.iDefectCount[i*MAX_DEFECT_TYPE_COUNT+j]=0;
		}
	}


	InitTableData(strTemp);
//	InsertDefectTableData(iDefectInfo,strTemp);
	SetTimer(ID_UPDATE_DEFECT_INFO_TIMER, 1000, NULL);
}

void CMainFrame::InitTableData(CString strTableName)
{
	int nTemp=0;
	CString strTemp(""), strSQL("");
	strSQL.Format("Select * from %s", strTableName);
	(m_RecordSet).CreateInstance(__uuidof(Recordset));

	try
	{
		(m_RecordSet)->Open(strSQL.GetBuffer(strSQL.GetLength()),
			_variant_t((IDispatch*)(m_Connect),true),
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch(_com_error* e)
	{
		AfxMessageBox(e->ErrorMessage());
		return;
	}

	if(!m_RecordSet->ADOEOF)
	{
		(m_RecordSet)->MoveFirst();
	}
	else
	{
		m_RecordSet->Close();
		return;
	}

	_variant_t var;
	while(!(m_RecordSet)->ADOEOF)
	{
		var=m_RecordSet->GetCollect("TotalCount");
		strTemp=(LPCSTR)_bstr_t(var);
		nTemp=atoi(strTemp);
		if(nTemp==0)
		{
			(m_RecordSet)->Delete(adAffectCurrent);
			(m_RecordSet)->Update();
		}
		m_RecordSet->MoveNext();
	}

	m_RecordSet->Close();
	
}

void CMainFrame::InsertTableData(s_CheckResultInfo info, CString strTableName)
{
	CString strSQL("");
	strSQL.Format("Select * from %s", strTableName);
	(m_RecordSet).CreateInstance(__uuidof(Recordset));

	try
	{
		(m_RecordSet)->Open(strSQL.GetBuffer(strSQL.GetLength()),
			_variant_t((IDispatch*)(m_Connect),true),
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch (_com_error* e)
	{
		AfxMessageBox(e->ErrorMessage());
		return;
	}



	if((m_RecordSet)->ADOEOF)
	{
		(m_RecordSet)->AddNew();
		(m_RecordSet)->PutCollect("StartTime",_variant_t(info.strStartTime));
		(m_RecordSet)->PutCollect("StopTime",_variant_t(info.strStopTime));
		(m_RecordSet)->PutCollect("ModelName", _variant_t(info.strModelName));
		(m_RecordSet)->PutCollect("TotalCount", _variant_t(info.strTotalCount));
		(m_RecordSet)->PutCollect("TotalGood", _variant_t(info.strTotalGood));
		(m_RecordSet)->PutCollect("TotalBad", _variant_t(info.strTotalBad));
		(m_RecordSet)->PutCollect("TotalBadRatio", _variant_t(info.strTotalBadRatio));

		(m_RecordSet)->PutCollect("Total1", _variant_t(info.strTotal1));
		(m_RecordSet)->PutCollect("Good1", _variant_t(info.strGood1));
		(m_RecordSet)->PutCollect("Bad1", _variant_t(info.strBad1));
		(m_RecordSet)->PutCollect("BadRatio1", _variant_t(info.strBadRatio1));


		(m_RecordSet)->PutCollect("Total2", _variant_t(info.strTotal2));
		(m_RecordSet)->PutCollect("Good2", _variant_t(info.strGood2));
		(m_RecordSet)->PutCollect("Bad2", _variant_t(info.strBad2));
		(m_RecordSet)->PutCollect("BadRatio2", _variant_t(info.strBadRatio2));
		(m_RecordSet)->Update();


	}
	else
	{
		while(!(m_RecordSet)->ADOEOF)
		{

			(m_RecordSet)->MoveNext();

			if((m_RecordSet)->ADOEOF)
			{

				(m_RecordSet)->AddNew();

				(m_RecordSet)->PutCollect("StartTime",_variant_t(info.strStartTime));

				(m_RecordSet)->PutCollect("StopTime",_variant_t(info.strStopTime));
	
				(m_RecordSet)->PutCollect("ModelName", _variant_t(info.strModelName));
	
				(m_RecordSet)->PutCollect("TotalCount", _variant_t(info.strTotalCount));

				(m_RecordSet)->PutCollect("TotalGood", _variant_t(info.strTotalGood));

				(m_RecordSet)->PutCollect("TotalBad", _variant_t(info.strTotalBad));

				(m_RecordSet)->PutCollect("TotalBadRatio", _variant_t(info.strTotalBadRatio));

				(m_RecordSet)->PutCollect("Total1", _variant_t(info.strTotal1));

				(m_RecordSet)->PutCollect("Good1", _variant_t(info.strGood1));

				(m_RecordSet)->PutCollect("Bad1", _variant_t(info.strBad1));

				(m_RecordSet)->PutCollect("BadRatio1", _variant_t(info.strBadRatio1));

				(m_RecordSet)->PutCollect("Total2", _variant_t(info.strTotal2));

				(m_RecordSet)->PutCollect("Good2", _variant_t(info.strGood2));

				(m_RecordSet)->PutCollect("Bad2", _variant_t(info.strBad2));

				(m_RecordSet)->PutCollect("BadRatio2", _variant_t(info.strBadRatio2));

				(m_RecordSet)->Update();

				break;
			}
		}
	}

	(m_RecordSet)->Close();

}

void CMainFrame::UpdateTableData(s_CheckResultInfo info, CString strTableName)
{
	CString strSQL("");
	strSQL.Format("Select * from %s", strTableName);
	(m_RecordSet).CreateInstance(__uuidof(Recordset));

	try
	{
		(m_RecordSet)->Open(strSQL.GetBuffer(strSQL.GetLength()),
			_variant_t((IDispatch*)(m_Connect),true),
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch (_com_error * e)
	{
		AfxMessageBox(e->ErrorMessage());
		return;
	}

	if((m_RecordSet)->ADOEOF)
	{
		(m_RecordSet)->PutCollect("StartTime",_variant_t(info.strStartTime));
		(m_RecordSet)->PutCollect("StopTime",_variant_t(info.strStopTime));
		(m_RecordSet)->PutCollect("ModelName", _variant_t(info.strModelName));
		(m_RecordSet)->PutCollect("TotalCount", _variant_t(info.strTotalCount));
		(m_RecordSet)->PutCollect("TotalGood", _variant_t(info.strTotalGood));
		(m_RecordSet)->PutCollect("TotalBad", _variant_t(info.strTotalBad));
		(m_RecordSet)->PutCollect("TotalBadRatio", _variant_t(info.strTotalBadRatio));
		(m_RecordSet)->PutCollect("Total1", _variant_t(info.strTotal1));
		(m_RecordSet)->PutCollect("Good1", _variant_t(info.strGood1));
		(m_RecordSet)->PutCollect("Bad1", _variant_t(info.strBad1));
		(m_RecordSet)->PutCollect("BadRatio1", _variant_t(info.strBadRatio1));
		(m_RecordSet)->PutCollect("Total2", _variant_t(info.strTotal2));
		(m_RecordSet)->PutCollect("Good2", _variant_t(info.strGood2));
		(m_RecordSet)->PutCollect("Bad2", _variant_t(info.strBad2));
		(m_RecordSet)->PutCollect("BadRatio2", _variant_t(info.strBadRatio2));
		(m_RecordSet)->Update();
	}
	else
	{
		while(!(m_RecordSet)->ADOEOF)
		{
			(m_RecordSet)->MoveNext();
			if((m_RecordSet)->ADOEOF)
			{
				(m_RecordSet)->MovePrevious();
				(m_RecordSet)->PutCollect("StartTime",_variant_t(info.strStartTime));
				(m_RecordSet)->PutCollect("StopTime",_variant_t(info.strStopTime));
				(m_RecordSet)->PutCollect("ModelName", _variant_t(info.strModelName));
				(m_RecordSet)->PutCollect("TotalCount", _variant_t(info.strTotalCount));
				(m_RecordSet)->PutCollect("TotalGood", _variant_t(info.strTotalGood));
				(m_RecordSet)->PutCollect("TotalBad", _variant_t(info.strTotalBad));
				(m_RecordSet)->PutCollect("TotalBadRatio", _variant_t(info.strTotalBadRatio));
				(m_RecordSet)->PutCollect("Total1", _variant_t(info.strTotal1));
				(m_RecordSet)->PutCollect("Good1", _variant_t(info.strGood1));
				(m_RecordSet)->PutCollect("Bad1", _variant_t(info.strBad1));
				(m_RecordSet)->PutCollect("BadRatio1", _variant_t(info.strBadRatio1));
				(m_RecordSet)->PutCollect("Total2", _variant_t(info.strTotal2));
				(m_RecordSet)->PutCollect("Good2", _variant_t(info.strGood2));
				(m_RecordSet)->PutCollect("Bad2", _variant_t(info.strBad2));
				(m_RecordSet)->PutCollect("BadRatio2", _variant_t(info.strBadRatio2));
				(m_RecordSet)->Update();
				break;
			}
		}
	}

	(m_RecordSet)->Close();
}

BOOL CMainFrame::StartCheck()
{
	fill(m_arr_iCurNumOfConstantKick.begin(), m_arr_iCurNumOfConstantKick.end(), 0);
	m_bCheckPushed = true;
//	SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);
	
	if (g_pView!=NULL)
	{
		g_pView->SetDlgItemText(IDC_BTN_START,"ֹͣ���");
		g_pView->m_BtnStart.SetIcon(IDI_ICON_STOP,BS_LEFT);
	}
	int i=0,j=0;

	for(i=0;i<c_AllElementCount;i++)
	{
		/*m_iCheckImageCount[i]=0;*/
		m_bDisplayAlgImage[i]=TRUE;
		m_bDisplayImage[i] = TRUE;
#ifdef __SGGrabber_
		m_iCheckErrorCount[i]=0;
		m_iGrabCircleCount[i]=0;
#endif	
	}

// 	for(i=0;i<CAMERA_COUNT;i++)
// 	{
// 		m_iPicCount[i]=0;
// 	}

	PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,Checking);

	int iParam1=0,iParam2=0;
	int iNewParam1=0,iNewParam2=0;
	
#ifdef USE_IO_CARD
	if (m_bIOCardValid && m_bIOCardInitSuccess) //�ӿڿ���Ч�ҳ�ʼ���ɹ�
	{
		try
		{
			for (int i = 0; i < IOCARD_COUNT; i++)
			{
				m_IOCard[i].enable(true); //[2012-2-1 by zhy]

			}
		}
		catch (...)
		{
			CString  strExceptionDes;
			strExceptionDes.Format("StartCheck-���ýӿڿ��쳣!");
			m_LogManager.MyWriteLogFile("�ӿڿ�����", strExceptionDes, AbnormityLog);
		}
	}
#endif

	if (TRUE == m_bSoftResetIOCard)
	{
		InitDataBase();
	}

	m_bIsDetecting = TRUE;

#ifdef __SGGrabber_
	//SetTimer(TIMER_SG_READ_IMG, m_iSG_ElapseTimeToRead, NULL);
#endif

	if (m_bIOCardValid)
	{
		fill(m_arr_iCurNumOfConstantKick.begin(), m_arr_iCurNumOfConstantKick.end(), 0);
		m_IOCard[e_IOCard_2].setCardOutput(m_iPortOfNormalRunning, 1);
		m_IOCard[e_IOCard_2].setCardOutput(m_iPortOfAlarm, 0);
		m_IOCard[e_IOCard_2].setCardOutput(m_iPortOfStopMachine, 0);
	}
	
// 	for (int i = 0; i < CAMERA_COUNT; ++i)
// 	{
// 		StartCameraGrab(i);
// 	}
	return TRUE;
}

BOOL CMainFrame::StopCheck()
{
// 	for (int i = 0; i < CAMERA_COUNT; ++i)
// 	{
// 		StopCameraGrab(i);
// 	}
	m_bCheckPushed = false;
	if (m_bIOCardValid)
	{
		m_IOCard[e_IOCard_2].setCardOutput(m_iPortOfStopMachine, 1);
		m_IOCard[e_IOCard_2].setCardOutput(m_iPortOfNormalRunning, 0);
		Sleep(100);
	}
	
	int i=0;
	if (g_pView!=NULL)
	{
		g_pView->SetDlgItemText(IDC_BTN_START,"��ʼ���");
		g_pView->m_BtnStart.SetIcon(IDI_ICON_START,BS_LEFT);
	}

	
	for(i=0;i<c_AllElementCount;i++)
	{
		m_bDisplayImage[i] = FALSE;
		m_bDisplayAlgImage[i]=FALSE;
	}
	
	
	PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,StopChecked);


#ifdef USE_IO_CARD
	StopMachine();
//	Sleep(1000);
// 	if(m_bIOCardValid && m_bIOCardInitSuccess)
// 	{
// 		try
// 		{
// 			for(int i=0;i<IOCARD_COUNT;i++)
// 			{
// 				//��ֹ�ӿڿ�
// 				m_IOCard[i].enable(false); //[2012-2-1 by zhy]
// 			}
// 		}
// 		catch (...)
// 		{
// 			CString  strExceptionDes;
// 			strExceptionDes.Format("StopCheck-���ýӿڿ��쳣!");
// 			m_LogManager.MyWriteLogFile("�ӿڿ�����",strExceptionDes,AbnormityLog);
// 		}
// 	}
#endif
	
	//  [12/3/2009 GZ]
	
	m_bIsDetecting = FALSE;
#ifdef __SGGrabber_
	KillTimer(TIMER_SG_READ_IMG);
#endif
	//SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);
	return TRUE;
}

// ��ȡ�����ź�
inline BOOL CMainFrame::ReadMachineSignal(int iGrabSn)
{
	static int iHardImageCount2=0;
	int nImageNum1=0, nImageNum3=0;
	
	try
	{
		if(Camera1==iGrabSn)
		{
			nImageNum1 = m_IOCard[Camera1].readCounter(28)%256; //��ȡͼ����� ͼ��ż���x����Ӧ��1·����Ĳ�ͼ�ص���
		}
		else if(Camera2==iGrabSn)
		{
			nImageNum1	= m_IOCard[Camera1].readCounter(28)%256; //��ȡͼ����� ͼ��ż���x����Ӧ��2·����Ĳ�ͼ�ص���
		}	 
		else if(Camera3==iGrabSn)
		{
			nImageNum1	= m_IOCard[Camera1].readCounter(28)%256; //��ȡͼ����� ͼ��ż���x����Ӧ��3·����Ĳ�ͼ�ص���
		}	 
		else if(Camera4==iGrabSn)
		{
			nImageNum1	= m_IOCard[Camera1].readCounter(28)%256; //��ȡͼ����� ͼ��ż���x����Ӧ��4·����Ĳ�ͼ�ص���
		}	 
		else if(Camera5==iGrabSn)
		{
			nImageNum1	= m_IOCard[Camera1].readCounter(29)%256; //��ȡͼ����� ͼ��ż���0����Ӧ��5·����Ĳ�ͼ�ص���
		}
		else if(Camera6==iGrabSn)
		{
			nImageNum1	= m_IOCard[Camera2].readCounter(28)%256; //��ȡͼ����� ͼ��ż���x����Ӧ��6·����Ĳ�ͼ�ص���
		}


		// ͼ���
		
		//��һ�������ͼ����֡����
		if (m_iLastImageCount[iGrabSn] > 200 && nImageNum1 <= 20)
		{
			m_iImageCountOverFlowNumber[iGrabSn] +=1;
		}

		m_iHardImageCount[iGrabSn] = nImageNum1 + 256*m_iImageCountOverFlowNumber[iGrabSn];
	
		if(nImageNum1!=m_iLastImageCount[iGrabSn])
		{
			m_iLastImageCount[iGrabSn] = nImageNum1;
		}

		return TRUE;
	}
	catch (...)
	{
		CString  strExceptionDes;
		strExceptionDes.Format(TEXT("ReadMachineSignal-��ȡ�����ź��쳣!"));
		m_LogManager.MyWriteLogFile("�ӿڿ�����",strExceptionDes,AbnormityLog);
		return FALSE;
	}

}

//**[7/19/2017 ConanSteve]**:  ���� �����������ӵĺ��� [4/30/2010 GZ]
BOOL  CMainFrame::CameraGrabFuncSingle(int iCameraSN,
									   int iAllElementSN)//**[7/19/2017 ConanSteve]**:  ��������Ĳɼ�����
{
	
	if (iCameraSN < 0)
	{
		CString  strExceptionDes;
		strExceptionDes.Format(TEXT("CameraGrabFuncSingle-������%d����!"),iCameraSN);
		m_LogManager.MyWriteLogFile(TEXT("�ɼ��ص�"),strExceptionDes,AbnormityLog);
		return FALSE;
	}
	
	BOOL bRet = FALSE;
	int iGrabCircle=0,iPicSN=0;
	bRet = JudgeGrabCircleNew(iCameraSN,iAllElementSN,iGrabCircle,iPicSN);

	try
	{
		/*if(bRet)*/
		{	
			CString str1{TEXT("")};
			/** Write Logs              [5/5/2017 ConanSteve]**/
			if(TRUE==m_bStartRecord)
			{
				str1.Format("==Image: %06d=Grab: %06d=Pic:%d=%s==\r\n",
					m_iHardImageCount[iAllElementSN], iGrabCircle, iPicSN, GetCurTime());
				m_iRC_ImageNum[iAllElementSN]++;
				if(m_iRC_ImageNum[iAllElementSN]>m_iRecord_Set)
				{
					m_iRC_ImageNum[iAllElementSN]=0;
					m_fileImageNum[iAllElementSN].SeekToBegin();
				}
				m_fileImageNum[iAllElementSN].Write(str1.GetBuffer(0),str1.GetLength());
			}
			
			// ����ϢԪ��ûʹ����ʼʹ��
			m_sElement[iAllElementSN][iGrabCircle%m_iSysDataEleLen].bIsCheck = m_bIsCheck[iAllElementSN];
			m_sElement[iAllElementSN][iGrabCircle%m_iSysDataEleLen].bHaveError=FALSE;
			m_sElement[iAllElementSN][iGrabCircle%m_iSysDataEleLen].iStationSN=iAllElementSN;
			m_sElement[iAllElementSN][iGrabCircle%m_iSysDataEleLen].iCircleSN = iGrabCircle;
#ifdef __SGGrabber_
			m_sElement[iAllElementSN][iGrabCircle%m_iSysDataEleLen].iImageSN = iGrabCircle;	
#else
			m_sElement[iAllElementSN][iGrabCircle%m_iSysDataEleLen].iImageSN = m_iHardImageCount[iAllElementSN];
#endif

			// ��д��ͼ�����ݡ��ɼ���Ϣ���ӿڿ���Ϣ��				
			int nReturn = 0;
			BYTE* pImageBuffer = NULL;
			int iImageWidth=0, iImageHeight=0, iImageByte=0,iImageSize=0;
			iImageWidth=m_sCameraImageInfo[iCameraSN].m_iCameraImageWidth;
			iImageHeight=m_sCameraImageInfo[iCameraSN].m_iCameraImageHeight;
			iImageByte=m_sCameraImageInfo[iCameraSN].m_iCameraImagePixelSize;
			iImageSize=m_sCameraImageInfo[iCameraSN].m_iCameraImageSize;
			
			// �õ�ͼ���ڴ��ַ
			bRet = m_CameraGrabber[iCameraSN].GetParamInt(GBImageBufferAddr, nReturn);		
			ASSERT(bRet == TRUE);
			
			pImageBuffer = (BYTE*)nReturn;
			ASSERT(pImageBuffer != NULL);	

			if(e_CamMode_Normal==m_iCamTestMode)
			{
				if(iPicSN>=m_iPicCountPerCam[iAllElementSN])
				{
					return FALSE;
				}
			}

			m_sElement[iAllElementSN][iGrabCircle%m_iSysDataEleLen].iIsUsing=1;

			/** Test mode              [5/5/2017 ConanSteve]**/
			if(e_CamMode_Test==m_iCamTestMode)
			{
				if (pImageBuffer != NULL)
				{
					
// 					if(m_BayerTransform[iCameraSN].m_bAreaCamBayer)
// 					{
// 						ConvertBayer2Rgb(m_pbRGBImage[iCameraSN],
// 							pImageBuffer,
// 							iImageWidth,
// 							iImageHeight,
// 							BAYER2RGB_NEIGHBOUR,
// 							&m_pLutR[0],
// 							&m_pLutG[0],
// 							&m_pLutB[0],
// 							FALSE,
// 							BAYER_RG);
// 
// 						int iOffset = iImageSize*m_iPicCountPerCam[iAllElementSN]*(iGrabCircle%m_iSysDataEleLen);
// 						
// 						// ���ڴ�дͼ������
// 						memcpy(m_pImageAddr[iAllElementSN].get()+iOffset,m_pbRGBImage[iCameraSN],iImageSize);
// 					}
// 					else
					{
						int iOffset = iImageSize*m_iPicCountPerCam[iAllElementSN]*(iGrabCircle%m_iSysDataEleLen);
						memcpy(m_pImageAddr[iAllElementSN].get()+iOffset, pImageBuffer,
							iImageSize);
					}								
					
					// �Բɼ���ͼ�����ˮƽ��ֱ����
// 					if (m_bGrabFlip[iCameraSN])
// 					{
// 						int iOffset = iImageSize*m_iPicCountPerCam[iAllElementSN]*(iGrabCircle%m_iSysDataEleLen);
// 						MirrorSingleImage(m_pImageAddr[iAllElementSN].get() +iOffset,
// 							iImageWidth,iImageHeight,iImageByte*8,m_eShowModeFlip[iCameraSN]);
// 					}
				}

//				ThreadDisplayImage(iAllElementSN,iGrabCircle%m_iSysDataEleLen);
			}
			/** normal acquisition mode              [5/5/2017 ConanSteve]**/
			else
			{
				if (pImageBuffer != NULL)
				{
// 					if(m_BayerTransform[iCameraSN].m_bAreaCamBayer)
// 					{
// 						ConvertBayer2Rgb(m_pbRGBImage[iCameraSN],
// 							pImageBuffer,
// 							iImageWidth,
// 							iImageHeight,
// 							BAYER2RGB_NEIGHBOUR,
// 							&m_pLutR[0],
// 							&m_pLutG[0],
// 							&m_pLutB[0],
// 							FALSE,
// 							BAYER_RG);
// 						
// 						int iOffset = iImageSize*m_iPicCountPerCam[iAllElementSN]*(iGrabCircle%m_iSysDataEleLen);
// 
// 						// ���ڴ�дͼ������
// 						memcpy(m_pImageAddr[iAllElementSN].get() +iOffset+iPicSN*iImageSize,
// 							m_pbRGBImage[iCameraSN],
// 							iImageSize);
// 						
// 					}
// 					else
					{
						int iOffset = iImageSize*m_iPicCountPerCam[iAllElementSN]*(iGrabCircle%m_iSysDataEleLen);

						if(TRUE==m_bStartRecord)
						{
							m_iRC_WriteImage[iAllElementSN]++;
							if(m_iRC_WriteImage[iAllElementSN]>m_iRecord_Set)
							{
								m_iRC_WriteImage[iAllElementSN]=0;
								m_fileWriteImage[iAllElementSN].SeekToBegin();
							}
							
							str1.Format("==Image:%d==Grab:%d==pic:%d=%s==%d=%d=%d=\r\n",m_iHardImageCount[iAllElementSN],
								iGrabCircle,iPicSN,GetCurTime(),(int)m_pImageAddr[iAllElementSN].get(),iOffset,iImageSize);
							m_fileWriteImage[iAllElementSN].Write(str1.GetBuffer(0),str1.GetLength());
						}

						memcpy(m_pImageAddr[iAllElementSN].get()+iOffset+iPicSN*iImageSize, pImageBuffer,
							iImageSize);
					}	
					
					
					// �Բɼ���ͼ�����ˮƽ��ֱ����
// 					if (m_bGrabFlip[iCameraSN])
// 					{
// 						int iOffset = iImageSize*m_iPicCountPerCam[iAllElementSN]*(iGrabCircle%m_iSysDataEleLen);
// 
// 						MirrorSingleImage(m_pImageAddr[iAllElementSN].get() +iOffset+iPicSN*iImageSize,
// 							iImageWidth,iImageHeight,iImageByte*8,m_eShowModeFlip[iCameraSN]);
// 					}
					
					m_bWriteImage[iAllElementSN][iGrabCircle%256][iPicSN]=TRUE;
										
					// saveImage
					int iTotal=0;
					for(int i=0;i<m_iPicCountPerCam[iAllElementSN];i++)
					{
						if(m_bWriteImage[iAllElementSN][iGrabCircle%256][i])
						{
							iTotal++;
						}
					}
					/** Camera has collected all specified images needed              [5/5/2017 ConanSteve]**/
					if(iTotal==m_iPicCountPerCam[iAllElementSN])
					{
						if(TRUE==m_bStartRecord)
						{
							m_iRC_SetEvent[iAllElementSN]++;
							if(m_iRC_SetEvent[iAllElementSN]>m_iRecord_Set)
							{
								m_iRC_SetEvent[iAllElementSN]=0;
								m_fileSetEvent[iAllElementSN].SeekToBegin();
							}

							str1.Format("==Camera:%d==Image:%d==%s==\r\n",iAllElementSN,m_iHardImageCount[iAllElementSN],GetCurTime());

							m_fileSetEvent[iAllElementSN].Write(str1.GetBuffer(0),str1.GetLength());
						}

						for(int i=0;i<PIC_COUNT;i++)
						{
							m_bWriteImage[iAllElementSN][iGrabCircle%256][i]=FALSE;
						}
						/** Use function of system saving image              [5/5/2017 ConanSteve]**/
						if(1==m_iSaveImageSys)
						{
							m_sElement[iAllElementSN][iGrabCircle%m_iSysDataEleLen].iIsUsing=2;
							m_sElement[iAllElementSN][iGrabCircle%m_iSysDataEleLen].iIsDetecting=0;

							//SetEvent(m_hImageSaveStart[iAllElementSN]);
						}
						
						CString strSavePath;
						CTime curTime = CTime::GetCurrentTime();
						
						s_CheckInParam sIn;

						int iOffset = iImageSize*m_iPicCountPerCam[iAllElementSN]*(iGrabCircle%m_iSysDataEleLen);

						LabelCheck::s_AlgInspInPara sParam;
						sParam.bDsp=m_iImageShowMode;
						sParam.bRunCheck=TRUE;
						sParam.iCircleIndex=m_iHardImageCount[iAllElementSN];
						sParam.iCount=m_iPicCountPerCam[iAllElementSN];
						sParam.nCamIndex=iAllElementSN;
						sParam.nChannel=m_sCameraImageInfo[iAllElementSN].m_iCameraImagePixelSize;
						sParam.nDisplayMode=m_iImageShowMode;
						sParam.nInSig=0;
						sParam.nSaveImageMode = m_bSaveCameraImage[iAllElementSN] ? m_iSaveImageType : -1;
						sParam.nHeight = m_sCameraImageInfo[iAllElementSN].m_iCameraImageHeight;
						sParam.nWidth=m_sCameraImageInfo[iAllElementSN].m_iCameraImageWidth;
						sParam.pcImageData=(char*)(m_pImageAddr[iAllElementSN].get() +iOffset);
						ZeroMemory(sParam.chSaveImagePath, sizeof(sParam.chSaveImagePath));
						memcpy(sParam.chSaveImagePath,m_sSaveImagePath[iAllElementSN].GetBuffer(0),m_sSaveImagePath[iAllElementSN].GetLength());


						sParam.bDisplaySys = 0 == m_iShowDevice ? true : false;//*[7/5/2017 ConanSteve]*:  0��ʾϵͳͼ��1��ʾ�㷨ͼ��
						sParam.bDetectAlg = m_bOpenCameraAlgCheck[iAllElementSN];
						sParam.nDispFirstorLast = 2;
#ifdef SPOT
						sParam.iRunMode = 0;
#else
						sParam.iRunMode = 1;
#endif // SPOT
						
						sIn.iCheckSN=iAllElementSN;
						sIn.iImageSN=m_iHardImageCount[iAllElementSN];
						sIn.pImageData=(char*)m_pImageAddr[iAllElementSN].get() +iOffset;
						sIn.pParam=&sParam;

						sIn.iReserve1 = e_SysDetectMode_Multi;


						//*[7/7/2017 ConanSteve]*:  ��չ����;
// 						m_arrMultiObjSaveImgPara[iAllElementSN].SetImgNum(m_iPicCountPerCam[iAllElementSN]);
// 						m_arrMultiObjSaveImgPara[iAllElementSN].SetImgWidth(m_sCameraImageInfo[iAllElementSN].m_iCameraImageWidth);
// 						m_arrMultiObjSaveImgPara[iAllElementSN].SetImgHeight(m_sCameraImageInfo[iAllElementSN].m_iCameraImageHeight) ;
// 						m_arrMultiObjSaveImgPara[iAllElementSN].SetImgChannel(m_sCameraImageInfo[iAllElementSN].m_iCameraImagePixelSize);
// 						m_arrMultiObjSaveImgPara[iAllElementSN].SetIsCirCleSaveImg(1 == m_iIsCircle ? true : false);//*[7/10/2017 ConanSteve]*:  1��ʾѭ����ͼ��0��ʾ��ѭ����ͼ
// 						m_arrMultiObjSaveImgPara[iAllElementSN].SetSaveImgFolderPath(m_sSaveImagePath[iAllElementSN].GetBuffer(0), m_sSaveImagePath[iAllElementSN].GetLength());
						m_arr_ExtenPara[iAllElementSN].push_back(COptionalFunc(e_Fn_CheckInParam_SmallImageNumOfEachGrab).SetInt(m_iPicCountPerCam[iAllElementSN]));
						m_arr_ExtenPara[iAllElementSN].push_back(COptionalFunc(e_Fn_CheckInParam_ImageWidth).SetInt(m_sCameraImageInfo[iAllElementSN].m_iCameraImageWidth));
						m_arr_ExtenPara[iAllElementSN].push_back(COptionalFunc(e_Fn_CheckInParam_ImageHeight).SetInt(m_sCameraImageInfo[iAllElementSN].m_iCameraImageHeight));
						m_arr_ExtenPara[iAllElementSN].push_back(COptionalFunc(e_Fn_CheckInParam_ImageChannel).SetInt(m_sCameraImageInfo[iAllElementSN].m_iCameraImagePixelSize));
						m_arr_ExtenPara[iAllElementSN].push_back(COptionalFunc(e_Fn_CheckInParam_WhetherCicleSaveImage).SetBool(1 == m_iIsCircle ? true : false));
						m_arr_ExtenPara[iAllElementSN].push_back(COptionalFunc(e_Fn_CheckInParam_SaveImagePath).SetArrayTCHAR(m_sSaveImagePath[iAllElementSN].GetBuffer(0)));

						COptionalFunc opFn = m_arr_ExtenPara[iAllElementSN].GetFuncStruct(e_Fn_CheckInParam_SaveImagePath);
						sIn.iReserve4 = (int)&m_arr_ExtenPara[iAllElementSN];
						
						if(TRUE==m_bOpenAlgCheck)
						{
							m_InspectionObj.Detect(sIn);
						}
						m_iCheckImageCount[iAllElementSN]++;
					}
				}
				else
				{
					return FALSE;
				}
			}

			m_sElement[iAllElementSN][iGrabCircle%m_iSysDataEleLen].iIsUsing=0;
		}
	}
	catch (...)
	{
		TRACE("----------------�ɼ��ص�:���%d���������ɼ�����!---------------\n",iCameraSN+1);
		CString  strExceptionDes;
		strExceptionDes.Format("CameraGrabFuncSingle-���%d���������ɼ�����=%d=%d=%d=!",iCameraSN+1,
			m_iHardImageCount[iAllElementSN],iGrabCircle,iPicSN);
		m_LogManager.MyWriteLogFile("�ɼ��ص�",strExceptionDes,AbnormityLog);
	}	

	return TRUE;
}

inline BOOL  CMainFrame::JudgeGrabCircleNew(int  iCameraSN,
											int  iAllElementSN,	
											int& iGrabCircle,
											int& iPicSN) 
{	
	CString strCamera1;
	LARGE_INTEGER iLarge1;
	LONGLONG qt1;
	
	m_iPicCount[iCameraSN]++;
	/** д��־              [5/5/2017 ConanSteve]**/
	if(TRUE==m_bStartRecord)
	{
		QueryPerformanceCounter(&iLarge1);
		qt1=iLarge1.QuadPart;
		double diffTest=0,dd=0.0;
		
		dd=abs(qt1-m_lCameraTimeInfo[iAllElementSN]);
		diffTest=1000*dd/m_dFreq;
		m_lCameraTimeInfo[iAllElementSN]=qt1;
		
		
		strCamera1.Format("==�����:%d==ͼ���:%06d==���:%0.0f==�ɼ�:%d==Last:%d==%s==\r\n",
			iAllElementSN+1,m_iHardImageCount[iAllElementSN],diffTest,m_iGrabCircleCount[iAllElementSN],
			m_iLastHardImageCount[iAllElementSN],GetCurTime());
		
		m_iRC_Call[iAllElementSN]++;
		if(m_iRC_Call[iAllElementSN]>m_iRecord_Set)
		{
			m_iRC_Call[iAllElementSN]=0;
			m_fileCall[iAllElementSN].SeekToBegin();
		}
		m_fileCall[iAllElementSN].Write(strCamera1.GetBuffer(0),strCamera1.GetLength());
	}


#ifdef __RealGrabber_
	if(m_iHardImageCount[iAllElementSN]!=m_iLastHardImageCount[iAllElementSN])
	{
		m_iCamSN[iCameraSN]=0;
		iPicSN=0;

		m_iGrabCircleCount[iAllElementSN]++;		
		if (m_iGrabCircleCount[iAllElementSN] < 0)
		{
			m_iGrabCircleCount[iAllElementSN]=1;
		}
		iGrabCircle = m_iGrabCircleCount[iAllElementSN];
		m_iLastHardImageCount[iAllElementSN] = m_iHardImageCount[iAllElementSN];
	}
	else
	{
		m_iCamSN[iCameraSN]++;
		iPicSN=m_iCamSN[iCameraSN];
		iGrabCircle = m_iGrabCircleCount[iAllElementSN];
		return FALSE;
	}
#else

	if((m_iPicCount[iCameraSN]-1)%m_iPicCountPerCam[iCameraSN]==0)
	{

		m_iGrabCircleCount[iAllElementSN]++;

 		iGrabCircle=m_iGrabCircleCount[iAllElementSN];
 		m_iCamSN[iCameraSN]=(m_iPicCount[iCameraSN]-1)%m_iPicCountPerCam[iCameraSN];
		iPicSN=m_iCamSN[iCameraSN];
	}
	else
	{
		iGrabCircle=m_iGrabCircleCount[iAllElementSN];
		m_iCamSN[iCameraSN]=(m_iPicCount[iCameraSN]-1)%m_iPicCountPerCam[iCameraSN];
		iPicSN=m_iCamSN[iCameraSN];
	}
#endif	


#ifdef __SGGrabber_ 
	m_iHardImageCount[iAllElementSN] = m_iGrabCircleCount[iAllElementSN];
	m_SpendTime[iAllElementSN][m_iHardImageCount[iAllElementSN]%256].Start();
	m_iCheckResultTable[iAllElementSN][m_iHardImageCount[iAllElementSN]%256] = -1;
	m_bTubeCheckResult[iAllElementSN][m_iHardImageCount[iAllElementSN]%256] = FALSE;
	m_iImageKickThread[iAllElementSN] = m_iHardImageCount[iAllElementSN];
#endif

	return TRUE;

}

//��ͼ�����ݽ��о���
int CMainFrame::MirrorSingleImage(BYTE* pImageBuff, int lImageWidth, int lImageHeight, int lImageBitCount, e_ShowModeFlip  eShowMode) //ֻ��ˮƽ�ʹ�ֱ����
{
	if (pImageBuff == NULL)
	{
		return ImageBufferError;
	}
	if (lImageWidth<0||lImageHeight<0||lImageBitCount<0)
	{
		return ImageWHBITError;
	}
	
	if (lImageBitCount==8)
	{	
		lImageWidth=WIDTHBYTES(lImageWidth*8);
		if (eShowMode==FlipHorizintal) //ˮƽ��ת
		{
			BYTE  tem;
			for (int i=0;i<lImageHeight;i++)
			{
				for (int j=0;j<lImageWidth/2;j++)
				{
					tem=*(pImageBuff+i*lImageWidth+j);
					*(pImageBuff+i*lImageWidth+j)=*(pImageBuff+i*lImageWidth+lImageWidth-1-j);
					*(pImageBuff+i*lImageWidth+lImageWidth-1-j)=tem;
				}
			}
			return TRUE;
		}
		if (eShowMode==FlipVertical) //��ֱ��ת
		{
			BYTE  tem;
			for (int i=0;i<lImageHeight/2;i++)
			{
				for (int j=0;j<lImageWidth;j++)
				{
					tem=*(pImageBuff+i*lImageWidth+j);
					*(pImageBuff+i*lImageWidth+j)=*(pImageBuff+(lImageHeight-1-i)*lImageWidth+j);
					*(pImageBuff+(lImageHeight-1-i)*lImageWidth+j)=tem;
				}
			}
			return TRUE;
		}
	}
	if (lImageBitCount==24)//24λ����
	{
		lImageWidth=WIDTHBYTES(lImageWidth*8);
		
		if (eShowMode==FlipHorizintal) //ˮƽ��ת
		{
			BYTE  tem1,tem2,tem3;
			for (int i=0;i<lImageHeight;i++)
			{
				for (int j=0;j<lImageWidth/2;j++)
				{
					tem1=*(pImageBuff+i*lImageWidth*3+3*j);
					tem2=*(pImageBuff+i*lImageWidth*3+3*j+1);
					tem3=*(pImageBuff+i*lImageWidth*3+3*j+2);
					*(pImageBuff+i*lImageWidth*3+3*j)=*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j));
					*(pImageBuff+i*lImageWidth*3+3*j+1)=*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j)+1);
					*(pImageBuff+i*lImageWidth*3+3*j+2)=*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j)+2);
					*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j))=tem1;
					*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j)+1)=tem2;
					*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j)+2)=tem3;
				}
			}
			return TRUE;
		}
		if (eShowMode==FlipVertical) //��ֱ��ת
		{
			BYTE  tem1,tem2,tem3;
			for (int i=0;i<lImageHeight/2;i++)
			{
				for (int j=0;j<lImageWidth;j++)
				{
					tem1=*(pImageBuff+i*lImageWidth*3+3*j);
					tem2=*(pImageBuff+i*lImageWidth*3+3*j+1);
					tem3=*(pImageBuff+i*lImageWidth*3+3*j+2);
					*(pImageBuff+i*lImageWidth*3+3*j)=*(pImageBuff+(lImageHeight-1-i)*lImageWidth*3+3*j);
					*(pImageBuff+i*lImageWidth*3+3*j+1)=*(pImageBuff+(lImageHeight-1-i)*lImageWidth*3+3*j+1);
					*(pImageBuff+i*lImageWidth*3+3*j+2)=*(pImageBuff+(lImageHeight-1-i)*lImageWidth*3+3*j+2);
					*(pImageBuff+(lImageHeight-1-i)*lImageWidth*3+3*j)=tem1;
					*(pImageBuff+(lImageHeight-1-i)*lImageWidth*3+3*j+1)=tem2;
					*(pImageBuff+(lImageHeight-1-i)*lImageWidth*3+3*j+2)=tem3;
				}
			}
			return TRUE;
		}
	}	
	return TRUE;
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (TIMER_SG_READ_IMG == nIDEvent)
	{
		int k{ 0 };
		for (int i = 0; i < c_AllElementCount; ++i)
		{
			SetEvent(m_hEventSGActive[i]);
			DWORD dwRet=WaitForSingleObject(m_hEventComplete[i], 100);
			if (WAIT_TIMEOUT == dwRet)
				break;
		}
		int d{ 0 };
	}
	else if (nIDEvent == StartGrabTimer)
	{
		for (int i=0;i<CAMERA_COUNT;i++)
		{
			if (!StartCameraGrab(i))//��ʼ�ɼ�
			{
				KillTimer(StartGrabTimer);
				return;
			} 
		}
		KillTimer(StartGrabTimer);
	}
	else if (nIDEvent == IntervalDisplayTimerOne) // ���ڿ������1��ͼ����ʾ���
	{
		m_bDisplayImage[Camera1] = TRUE;
		m_bDisplayAlgImage[Camera1]=TRUE;
	}
	else if (nIDEvent == IntervalDisplayTimerTwo) // ���ڿ������2��ͼ����ʾ���
	{
		m_bDisplayImage[Camera2] = TRUE;
		m_bDisplayAlgImage[Camera2]=TRUE;
	}
	else if(nIDEvent == ID_START_CHECK_TIMER)
	{
		CString strStopTime;
		CTime CurrTime = CTime::GetCurrentTime();
		strStopTime.Format("%d-%d-%d %d:%d:%d",CurrTime.GetYear(),CurrTime.GetMonth(),CurrTime.GetDay(),
			CurrTime.GetHour(),CurrTime.GetMinute(),CurrTime.GetSecond());
		
		CString strCheckTotal,strBadTotal,strGoodTotal, strBadTotalRatio;
		CString strTotal[c_AllElementCount],strGood[c_AllElementCount];
		CString strBad[c_AllElementCount],strBadRatio[c_AllElementCount];

		strCheckTotal.Format("%d",m_iCheckCountTotal);
		strBadTotal.Format("%d",m_iCheckErrorCountTotal);
		strGoodTotal.Format("%d",m_iCheckGoodCountTotal);
		
		int iGood=0;
		float fRatio=0.0;
		
		for(int i=0;i<c_AllElementCount;i++)
		{
			strTotal[i].Format("%d",m_iFrameCount[i]);
			strBad[i].Format("%d",m_iHardKickCount[i]);
			
			iGood = m_iFrameCount[i]-m_iHardKickCount[i];
			if(iGood<=0)
			{
				strGood[i].Format("%d",0);
			}
			else
			{
				strGood[i].Format("%d",iGood);
			}
			
			if(m_iFrameCount[i]<=0 || m_iHardKickCount[i]<=0)
			{
				strBadRatio[i].Format("%0.3f%%",0.0);
			}
			else
			{
				fRatio = ((float)100*m_iHardKickCount[i])/m_iFrameCount[i];
				strBadRatio[i].Format("%0.3f%%",fRatio);
			}
		}
		
		if(m_iCheckErrorCountTotal<=0 || m_iCheckCountTotal<=0)
		{
			strBadTotalRatio.Format("%0.3f%%",0.0);
		}
		else
		{
			strBadTotalRatio.Format("%0.3f%%",((float)100*m_iCheckErrorCountTotal)/((float)m_iCheckCountTotal));
		}
		
		s_CheckResultInfo info;
		info.strStartTime=m_sStartTime;
		info.strStopTime=strStopTime;
		info.strModelName=m_sModelName;
		info.strTotalCount= strCheckTotal;
		info.strTotalGood = strGoodTotal;
		info.strTotalBad=strBadTotal;
		info.strTotalBadRatio=strBadTotalRatio;
		info.strTotal1 = strTotal[Camera1];
		info.strGood1 = strGood[Camera1];
		info.strBad1 = strBad[Camera1];
		info.strBadRatio1 = strBadRatio[Camera1];
// 		info.strTotal2 = strTotal[Camera2];
// 		info.strGood2 = strGood[Camera2];
// 		info.strBad2 = strBad[Camera2];
// 		info.strBadRatio2 = strBadRatio[Camera2];

		
		UpdateTableData(info, "All_Check_Info");

	}
	else if(nIDEvent==ID_UPDATE_DEFECT_INFO_TIMER)
	{
		CString strStopTime;
		CTime CurrTime = CTime::GetCurrentTime();
		strStopTime.Format("%d-%d-%d %d:%d:%d",CurrTime.GetYear(),CurrTime.GetMonth(),CurrTime.GetDay(),
			CurrTime.GetHour(),CurrTime.GetMinute(),CurrTime.GetSecond());

		int i=0,j=0;
		int iGood = 0;
		CString strTemp,strTable;
		s_CheckDefectInfo info;

		strTable.Format("%s","Defect_Statistic_Info");
		info.sStartTime=m_sStartTime;
		info.sStopTime=strStopTime;
		info.sModelName=m_sModelName;
		info.iTotalCount=m_iFrameCount[Camera1];
 		info.iTotalGood=m_iFrameCount[Camera1]-m_iHardKickCount[Camera1];
 		info.iTotalBad=m_iHardKickCount[Camera1];
// 		info.iTotal1=m_iFrameCount[Camera1];
// 		info.iBad1=m_iHardKickCount[Camera1];
// 		info.iTotal2=m_iFrameCount[Camera2];
// 		info.iBad2=m_iHardKickCount[Camera2];
		info.iTotal1=0;
		info.iBad1=0;
		info.iTotal2=0;
		info.iBad2=0;

		for(i=0;i<c_AllElementCount;i++)
		{
			for(j=0;j<MAX_DEFECT_TYPE_COUNT;j++)
			{
				info.iDefectCount[i*MAX_DEFECT_TYPE_COUNT+j]=m_iDefectCount[i][j];
			}
		}
//		UpdateDefectTableData(info,strTable);	
	}
	else if(nIDEvent == DisplayCount)
	{
		CString strInfo("");
		int iTemp1 = 0, iTemp2=0;
		double dRatio = 0;

		//m_iCheckCountTotal = (m_iFrameCount[Camera1]-m_iEmptyCount);

		//m_iCheckGoodCountTotal = (m_iFrameCount[Camera1]-m_iHardKickCount[Camera1]-m_iEmptyCount-m_iHardKickCount[Camera6]);

		//if(m_iFrameCount[Camera1]>0 && m_iHardKickCount[Camera1]>0)
		double lfGoodRatio{ 0 };
		int iGoodCnt = m_iCheckTubeCnt[Camera6] - m_iHardKickCount[Camera6];
		if(0!=m_iCheckCountTotal)
		{
			//m_dwCheckBadRatio = 100*((double)m_iHardKickCount[Camera1]/m_iFrameCount[Camera1]);
			lfGoodRatio = (iGoodCnt + m_iLastGoodCntTotle) / (double)(m_iCheckCountTotal + m_iLastCheckCntTotle);
		}

		// �豸����
		m_iniAlgConfig.WriteInt(_T("ͳ����Ϣ"), _T("�ϴμ������"), m_iCheckCountTotal + m_iLastCheckCntTotle);
		m_iniAlgConfig.WriteInt(_T("ͳ����Ϣ"), _T("�ϴκϸ�Ʒ��"), iGoodCnt + m_iLastGoodCntTotle);
		strInfo.Format("%d",m_iCheckCountTotal+ m_iLastCheckCntTotle);
		g_pView->m_TotalData.Display(strInfo,TEXT(""),0,FALSE);
			
		//�ϸ�Ʒ��
		strInfo.Format("%d", iGoodCnt+m_iLastGoodCntTotle);
		g_pView->m_TotalGoodData.Display(strInfo,_T(""),0,FALSE);
		
		//�ϸ���
		strInfo.Format("%0.3f%%",lfGoodRatio*100);
		g_pView->m_TotalGoodRatioData.Display(strInfo,TEXT(""),0,FALSE);
	
	}
	else if (nIDEvent == CheckSpeedRealTime)// ʵʱ�������ٶ�
	{

		if (!m_bIsCheck[Camera1])// �����������3��������������� [2012-2-10 by zhy]
		{
			if (g_pView!=NULL)
			{
				CString strTmp;
				strTmp.Format("%d ��/����",0);
 				g_pView->m_SpeedData.Display(strTmp.GetBuffer(strTmp.GetLength()),"",0,FALSE);
			}
		}
		else // ֻҪ�м������� ����������� Ϊ��������
		{
			CString strTmp;
			int i=0,iCount=0,iNum=0;

			if (g_pView!=NULL)
			{
// #ifdef __RealGrabber_
// 				
// #else
// 				int iCamgroup=0;
// 
// 				if (m_bIsCheck[Camera1])
// 					iCamgroup=Camera1;
// 
// 				iCount = (m_iCheckImageCount[iCamgroup]-m_iLastCheckSpeedCount)*60000/m_iCheckSpeedInterval;
// 				strTmp.Format("%d ��/����", iCount);
//  				g_pView->m_SpeedData.Display(strTmp.GetBuffer(strTmp.GetLength()),"",0,FALSE);
// 				m_iLastCheckSpeedCount = m_iCheckImageCount[iCamgroup];		
// #endif
				int iLastCnt = 0;
				if (m_queFrameCnt.size() > 0)
					iLastCnt = m_queFrameCnt.back();
				if (iLastCnt != m_iFrameCount[Camera1])
					m_queFrameCnt.push_back(m_iFrameCount[Camera1]);
				if (m_queTubeCnt.size() > 0)
					iLastCnt = m_queTubeCnt.back();
				if (iLastCnt != m_iCheckTubeCnt[Camera1])
					m_queTubeCnt.push_back(m_iCheckTubeCnt[Camera1]);
				if (m_queFrameCnt.size() > 31)
					m_queFrameCnt.pop_front();
				if (m_queTubeCnt.size() > 31)
					m_queTubeCnt.pop_front();
				if (m_queFrameCnt.size() > 1 && m_queTubeCnt.size() > 1)
				{
					int iFrameSpeed = (double)(m_iFrameCount[Camera1] - m_queFrameCnt.front()) * 60000 / (double)m_iCheckSpeedInterval / m_queFrameCnt.size() - 1;
					int ifTubeSpeed = (double)(m_iCheckTubeCnt[Camera1] - m_queTubeCnt.front()) * 60000 / (double)m_iCheckSpeedInterval / m_queTubeCnt.size() - 1;
					if (ifTubeSpeed < 0)
						ifTubeSpeed = 0;
					strTmp.Format(tr("%d ��/����"), ifTubeSpeed);

					g_pView->m_SpeedData.Display(strTmp, tr(""), 0, FALSE);
				}

// 				iCount = (m_iFrameCount[Camera1] - m_iLastCheckSpeedCount) * 60000 / m_iCheckSpeedInterval;
// 				strTmp.Format("%d ��/����", iCount);// 5��һ�Σ�1����ͳ��
// 
// 				g_pView->m_SpeedData.Display(strTmp, "", 0, FALSE);
// 				m_iLastCheckSpeedCount = m_iFrameCount[Camera1];
			}		
		}		
	}
	else if(nIDEvent==ShowDataInfo)
	{
		CString strGrab;

		strGrab.Format("[���:%d-%d][�޷�:%d-%d][�ܲɼ�:%d-%d-%d-%d-%d-%d][��Ѫ�ܼ���:%d-%d-%d-%d-%d-%d][�쳣����:%d-%d-%d-%d-%d-%d]",
			m_iFrameCount[Camera1],
			m_iFrameCount[Camera5],
			m_iHardKickCount[Camera1],
			m_iHardKickCount[Camera6],
			m_iPicCount[Camera1],
			m_iPicCount[Camera2],
			m_iPicCount[Camera3],
			m_iPicCount[Camera4],
			m_iPicCount[Camera5],
			m_iPicCount[Camera6],
			m_iCheckTubeCnt[Camera1],
			m_iCheckTubeCnt[Camera2],
			m_iCheckTubeCnt[Camera3],
			m_iCheckTubeCnt[Camera4],
			m_iCheckTubeCnt[Camera5],
			m_iCheckTubeCnt[Camera6],
			m_iAlgExceptionCnt[Camera1],
			m_iAlgExceptionCnt[Camera2],
			m_iAlgExceptionCnt[Camera3],
			m_iAlgExceptionCnt[Camera4],
			m_iAlgExceptionCnt[Camera5],
			m_iAlgExceptionCnt[Camera6]
		);

		m_wndStatusBar.SetPaneText(0, strGrab);

		strGrab.Format("[���ʱ��:%.0fms-%.0fms-%.0fms-%.0fms-%.0fms-%.0fms]",m_dAlgCheckTime[Camera1],m_dAlgCheckTime[Camera2],m_dAlgCheckTime[Camera3],
			m_dAlgCheckTime[Camera4], m_dAlgCheckTime[Camera5], m_dAlgCheckTime[Camera6]);
		m_wndStatusBar.SetPaneText(1, strGrab);

		strGrab.Format("[�������:%d-%d-%d-%d-%d-%d[%d]]",m_iCheckErrorCount[Camera1],m_iCheckErrorCount[Camera2],m_iCheckErrorCount[Camera3],
			m_iCheckErrorCount[Camera4], m_iCheckErrorCount[Camera5], m_iCheckErrorCount[Camera6], m_iErrorCntOf5And6);
		m_wndStatusBar.SetPaneText(2, strGrab);
	}
	CFrameWnd::OnTimer(nIDEvent);
}

// ��ʼ�ɼ�
inline BOOL CMainFrame::StartCameraGrab(const int iCameraSN)
{
	if (iCameraSN >= CAMERA_COUNT)
	{
		CString  strExceptionDes;
		strExceptionDes.Format("StartCameraGrab-���%d�������������!",iCameraSN+1);
		m_LogManager.MyWriteLogFile("�ɼ�����",strExceptionDes,OperationLog);
		return FALSE;
	}
	

	BOOL bReturnValue = FALSE;	
	bReturnValue = m_CameraGrabber[iCameraSN].StartGrab();

	
	//  ��ӿ�ʼ�ɼ�������
	if(bReturnValue == FALSE)
	{
		s_GBERRORINFO GrabberInfo;
		m_CameraGrabber[iCameraSN].GetLastErrorInfo(&GrabberInfo);
		CString strExceptionDes;
		strExceptionDes.Format("���%d",iCameraSN+1);
		CString strExceptionContent("");
		strExceptionDes += GrabberInfo.strErrorDescription;
		strExceptionDes += GrabberInfo.strErrorRemark;
		m_LogManager.MyWriteLogFile("�ɼ�����",strExceptionDes,OperationLog);
		return FALSE;
	}
	return TRUE;
}

// ֹͣ�ɼ�
inline BOOL CMainFrame::StopCameraGrab(const int iCameraSN)
{
	if (iCameraSN >= CAMERA_COUNT)
	{
		CString  strExceptionDes;
		strExceptionDes.Format("StopCameraGrab-���%d�������������!",iCameraSN+1);
		m_LogManager.MyWriteLogFile("�ɼ�����",strExceptionDes,OperationLog);
		return FALSE;
	}
	
	m_CameraGrabber[iCameraSN].StopGrab();
	
	return TRUE;
}

inline void CMainFrame::QuitCheckProgram()  //�˳�������
{
	int i=0;

	int cxScreen=0, cyScreen=0;
	CRect rcTemp;
	int x_LD=0, y_LD=0, w_LD=0, h_LD=0;

	m_bCloseCount=TRUE;
	m_bIsDetecting = FALSE;
	KillTimer(CheckSpeedRealTime);//ֹͣ����ֹͣ�������ٶ�
	KillTimer(IO24BSTATUS);
	KillTimer(DisplayCount);
	KillTimer(SaveCount);
	KillTimer(ShowDataInfo);



	KillTimer(ID_START_CHECK_TIMER);
	KillTimer(ID_UPDATE_DEFECT_INFO_TIMER);
	KillTimer(ID_UPDATE_DEFECT_SHOW_INFO);
	
	SaveSystemConfig(m_sSysConfigFileName);

	for (i=0;i<CAMERA_COUNT;i++)
	{
		StopCameraGrab(i); //ֹͣ�ɼ�
	}
		
	try
	{
		ReleaseGrabber();
		TRACE("-------�ͷ�����ɼ�����!---------------\n");
	}
	catch (...)
	{
		TRACE("-------�ͷ�����ɼ�����!---------------\n");
	}

#ifdef USE_IO_CARD
	try
	{
		ReleaseIOCard();
		TRACE("-------�ͷŽӿڿ�����!---------------\n");
		
	}
	catch (...)
	{
		TRACE("-------�ͷŽӿڿ�����!---------------\n");
	}		
#endif	
	
	try
	{
		KillAllThread(); //�ر������߳�
		TRACE("-------�ͷ��߳̽���!---------------\n");
	}
	catch (...)
	{
		TRACE("-------�ͷ��̲߳ɼ�����!---------------\n");
	}
	
	
	try
	{
		ReleaseDataFunc();
		TRACE("-------�ͷ��������ڴ����------------\r\n");
	}
	catch (...)
	{
		TRACE("-------�ͷ�����������!---------------\n");
	}	

	m_InspectionObj.Free();
	for (auto&a : m_hEventSGActive)
		CloseHandle(a);
	int k = 0;
	int j = 0;
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	CString str;
	switch(wParam)
	{
	case ID_INDICATOR_OPERATION:
		{
			str.Format("����״̬:");
			switch(lParam)
			{
			case SystemStarting:
				str += "��������ϵͳ...";
				break;
			case SystemStarted:
				str += "ϵͳ������!!!";
				break;
			case SystemClosing:
				str += "�����˳�ϵͳ...";
				break;
			case Checking:
				str += "���ڼ��...";				
				break;
			case StopChecked:
				str += "�����ֹͣ";
				break;
			case Grabbing:
				str.Format("��ʼ�ɼ�...");
				break;
			case StopGrabbing:
				str.Format("ֹͣ�ɼ�");
				break;
			case ContinueImageSaving:
				if(m_iIsCircle)
				{
					str.Format("����ѭ����������ͼ��:%d-%d-%d",
						m_iSaveCircleCameraImageCount[Camera1],
						m_iSaveCircleCameraImageCount[Camera2],
						m_iSaveCircleCameraImageCount[Camera3]);
				}
				else
				{		
					str.Format("���ڱ�������ͼ��:%d-%d-%d",
						m_iSaveCameraImageCount[Camera1],
						m_iSaveCameraImageCount[Camera2],
						m_iSaveCameraImageCount[Camera3]);
				}
				break;
			case ErrorImageSaving:
				if(m_iIsCircle)
				{
					str.Format("����ѭ���������ͼ��:%d-%d",
						m_iSaveCircleCameraImageCount[Camera1],
						m_iSaveCircleCameraImageCount[Camera2]);
				}
				else
				{
					str.Format("���ڱ������ͼ��:%d-%d-%d",
						m_iSaveCameraImageCount[Camera1],
						m_iSaveCameraImageCount[Camera2],
						m_iSaveCameraImageCount[Camera3]);
				}
				break;
			case AbnormalImageSaving:
				str.Format("���ڱ����쳣ͼ��:%d",m_iSaveCameraImageCount[Camera1]);
				break;
			case ModelImageSaving:			
				str.Format("���ڱ���ģ��ͼ��:%d",m_iSaveCameraImageCount[Camera1]);		
				break;
			case ImageSaved:
				str.Format("ͼ�񱣴����");
				break;
			case StopSave:
				str.Format("ֹͣ����");
				break;
			default:
				str.Format("����״̬");
				break;
			}
		}
		//m_wndStatusBar.SetPaneText(3, str);
		return TRUE;
	case ID_MSG_ALARM:
	{
		CString* pStr = (CString*)lParam;
		CString sError = _T("");
		if (NULL != pStr)
		{
			sError = *pStr;
			delete pStr;
		}
		if (m_bIOCardInitSuccess)
		{
			m_IOCard[e_IOCard_2].setCardOutput(m_iPortOfAlarm, 1);
			m_IOCard[e_IOCard_2].setCardOutput(6, 1);
			CDlgAlarm dlg(NULL, sError);
			dlg.DoModal();
		}
		Sleep(20);
		if (m_bIOCardInitSuccess)
		{
			m_IOCard[e_IOCard_2].setCardOutput(m_iPortOfAlarm, 0);
			m_IOCard[e_IOCard_2].setCardOutput(6, 0);
		}
	}
		return true;
	default:
		break;
	}
	return CFrameWnd::OnCommand(wParam, lParam);
}

CString CMainFrame::GetCurTime(DWORD *dwTime)
{
	LARGE_INTEGER Large;
	LONGLONG qt;
	int nHour=0,nMinute=0,nSecond=0,nMiliSecond=0;
	QueryPerformanceCounter(&Large);
	qt=Large.QuadPart;
				
	nMiliSecond=qt*1000/m_dFreq;
	nSecond=nMiliSecond/1000;
	nMinute=nSecond/60;
	nHour=nMinute/60;
	
	CString sRet;
	sRet.Format("%02d:%02d:%02d.%03d",nHour,nMinute%60,nSecond%60,nMiliSecond%1000);
	
	return sRet;
}

BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd* pDemoWnd = NULL;
	int i{ 0 };
	switch(pCopyDataStruct->dwData)
	{
	case 10:
		{
			pDemoWnd = CWnd::FindWindow(NULL, m_sDemoName);	
			break;
		}	
	case 30:/** ����ģ��              [6/26/2017 ConanSteve]**/
		{
			StopMachine();
			Sleep(2000);
			pDemoWnd = CWnd::FindWindow(NULL,m_sDemoName);
			if(NULL==pDemoWnd)
			{
				AfxMessageBox("�޷��ҵ��㷨DEMO����");
				break;
			}

			CString strOld,strNew;
			strOld = m_sModelName;
			strNew = (char*)pCopyDataStruct->lpData;
			
		
			LRESULT ret=1;
			s_Status sRet;
			s_AlgModelInput sModelInfo;
			ZeroMemory(&sModelInfo,sizeof(s_AlgModelInput));
			ZeroMemory(&sRet,sizeof(s_Status));
			
			for(int i=0;i<c_AllElementCount;i++)
			{
				sModelInfo.iCheckSN=i;
				sModelInfo.iOperSig=0;
				
			}

			if(TRUE==m_bOpenAlgCheck)
			{
				BOOL bRet = FALSE;

				// ��Ҫ������
				s_Status sRet;
				s_AlgModelInput sModelInfo;
				ZeroMemory(&sModelInfo,sizeof(s_AlgModelInput));
				ZeroMemory(&sRet,sizeof(s_Status));
				
				LabelCheck::s_DemoAlgSetParam sParam;
				ZeroMemory(&sParam,sizeof(LabelCheck::s_DemoAlgSetParam));
				
				int* pModelParam[c_AllElementCount];
				
				
				for(i=0;i<c_AllElementCount;i++)
				{
					//if (i == 0||i==2)
					//	continue;
					ZeroMemory(&sParam,sizeof(LabelCheck::s_DemoAlgSetParam));
					sParam.bTopWin=TRUE;
					memcpy(sParam.chModelName,strNew.GetBuffer(0),strNew.GetLength());
					sParam.iCheckSN=i;
					sParam.pParent=NULL;
					
					sModelInfo.iCheckSN=i;
					sModelInfo.iOperSig=0;
					
					sModelInfo.pParam = (int*)&sParam;									
					sRet = m_InspectionObj.Set(sModelInfo);
					

					
					if(sRet.iStatusType!=0)
					{
						bRet = FALSE;
						break;
					}
					else
					{
						bRet = TRUE;
					}
				}	
				

				// ���سɹ�
				if(TRUE==bRet)
				{	
					m_bReadModel = TRUE;
					ret = pDemoWnd->PostMessage(WM_APP+4,NULL,(LPARAM)0);
					m_sModelName = strNew;
					m_sOldModelName = strOld;
					m_evtTrigDemoMould.SetEvent();
				}
				else 
				{
					// ����ʧ�ܷ���ʧ������[2014-11-17]
					m_bReadModel = FALSE;
					ret = pDemoWnd->PostMessage(WM_APP+4,NULL,(LPARAM)1);
					m_sModelName = strNew;
					m_sOldModelName = strOld;
					m_evtTrigDemoMould.SetEvent();
					
				}
				/************************************************************************/
				/*                       [5/24/2017 ConanSteve]                         */
				SYSTEMTIME sysTime{ 0 };
				CString sLoadModeInfoFilePath{ TEXT("") };
				CString sLoadModelIndex{ TEXT("") };
				CString sLoadModelInfo{ TEXT("") };
				GetLocalTime(&sysTime);
				sLoadModeInfoFilePath = m_sWorkPath + TEXT("\\ModelInfo\\") + m_sModelName + TEXT("\\LoadModelLog.ini");
				int iLoadLogTimes = GetPrivateProfileInt(TEXT("MainConfiguration"), TEXT("LoadModelTimes"), 0, sLoadModeInfoFilePath);
				++iLoadLogTimes;
				sLoadModelIndex.Format(TEXT("%d"), iLoadLogTimes);
				sLoadModelInfo.Format(TEXT("%04d%02d%02d_%02d:%02d:%02d"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
				sLoadModelInfo += bRet ? TEXT("\tLoad Succeed!") : TEXT("\tLoad Fail!");
				WritePrivateProfileString(TEXT("MainConfiguration"), TEXT("LoadModelTimes"), sLoadModelIndex, sLoadModeInfoFilePath);
				WritePrivateProfileString(TEXT("LoadModelTime"), sLoadModelIndex, sLoadModelInfo, sLoadModeInfoFilePath);

				/************************************************************************/
			}

			break;
		}		
	}

	return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}

int CMainFrame::RotateImage(BYTE* pSrcImageBuff, int iSrcWidth, int iSrcHeight, int iImageBitCount, float fRotateAngle, 
							BYTE* pDestImageBuff, int& iDestWidth, int& iDestHeight)
{
	
	// ͼ��ÿ�е��ֽ���
	int    iLineBytes;
	
	// ��ת��ͼ��Ŀ�ȣ�iNewWidth'��������4�ı�����
	int    iNewLineBytes;
	
	// ָ��Դ���ص�ָ��
	LPSTR    lpSrc;
	
	// ָ����תͼ���Ӧ���ص�ָ��
	LPSTR    lpDst;
	
	// ѭ����������������DIB�е����꣩
	int    i;
	int    j;
	
	// ������ԴDIB�е�����
	int    i0;
	int    j0;
		
	// ��ת�Ƕȵ����Һ�����
	float    fSina, fCosa;
	
	// Դͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	float    fSrcX1,fSrcY1,fSrcX2,fSrcY2,fSrcX3,fSrcY3,fSrcX4,fSrcY4;
	
	// ��ת���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	float    fDstX1,fDstY1,fDstX2,fDstY2,fDstX3,fDstY3,fDstX4,fDstY4;
	
	// �����м䳣��
	float    f1,f2;
	
	// ����ͼ��ÿ�е��ֽ���
	iLineBytes = WIDTHBYTES(iSrcWidth * 8);
			
	// ������ת�Ƕȵ�����
	fSina = (float) sin((double)fRotateAngle);
	
	// ������ת�Ƕȵ�����
	fCosa = (float) cos((double)fRotateAngle);
	
	// ����ԭͼ���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	fSrcX1 = (float) (-(iSrcWidth - 1) / 2);
	fSrcY1 = (float) ( (iSrcHeight - 1) / 2);
	fSrcX2 = (float) ( (iSrcWidth - 1) / 2);
	fSrcY2 = (float) ( (iSrcHeight - 1) / 2);
	fSrcX3 = (float) (-(iSrcWidth - 1) / 2);
	fSrcY3 = (float) (-(iSrcHeight - 1) / 2);
	fSrcX4 = (float) ( (iSrcWidth - 1) / 2);
	fSrcY4 = (float) (-(iSrcHeight - 1) / 2);
	
	// ������ͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	fDstX1 = fCosa * fSrcX1 + fSina * fSrcY1;
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 = fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 = fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 = fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;
	
	// ������ת���ͼ��ʵ�ʿ��
	iDestWidth = (int) ( max( fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2) ) + 0.5);
	
	// ������ͼ��ÿ�е��ֽ���
	iNewLineBytes = WIDTHBYTES(iDestWidth * 8);
	
	// ������ת���ͼ��߶�
	iDestHeight = (int) ( max( fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2) ) + 0.5);

	if(iDestHeight%4!=0)
	{
		iDestHeight = (iDestHeight/4)*4+4;
	}

	if(iDestWidth%4!=0)
	{
		iDestWidth = (iDestWidth/4)*4+4;
	}
	
	// �������������������Ժ�ÿ�ζ�������
	f1 = (float) (-0.5 * (iDestWidth - 1) * fCosa - 0.5 * (iDestHeight - 1) * fSina
		+ 0.5 * (iSrcWidth - 1));
	f2 = (float) ( 0.5 * (iDestWidth - 1) * fSina - 0.5 * (iDestHeight - 1) * fCosa
		+ 0.5 * (iSrcHeight - 1));
		
	// ���ͼ��ÿ�н��в���
	for(i = 0; i < iDestHeight; i++)
	{
		// ���ͼ��ÿ�н��в���
		for(j = 0; j < iDestWidth; j++)
		{

			if(8==iImageBitCount)
			{
				// ָ����DIB��i�У���j�����ص�ָ��
				// ע��˴���Ⱥ͸߶�����DIB�Ŀ�Ⱥ͸߶�
				lpDst = (char *)pDestImageBuff + iNewLineBytes * (iDestHeight - 1 - i) + j;
				
				// �����������ԴDIB�е�����
				i0 = (int) (-((float) j) * fSina + ((float) i) * fCosa + f2 + 0.5);
				j0 = (int) ( ((float) j) * fCosa + ((float) i) * fSina + f1 + 0.5);
				
				// �ж��Ƿ���Դͼ��Χ��
				if( (j0 >= 0) && (j0 < iSrcWidth) && (i0 >= 0) && (i0 < iSrcHeight))
				{
					// ָ��ԴDIB��i0�У���j0�����ص�ָ��
					lpSrc = (char *)pSrcImageBuff + iLineBytes * (iSrcHeight - 1 - i0) + j0;
					
					// ��������
					*lpDst = *lpSrc;
				}
				else
				{
					// ����Դͼ��û�е����أ�ֱ�Ӹ�ֵΪ255
					* ((unsigned char*)lpDst) = 255;
				}
			}
			else if(24==iImageBitCount)
			{
// 				tem1=*(pImageBuff+i*lImageWidth*3+3*j);
// 				tem2=*(pImageBuff+i*lImageWidth*3+3*j+1);
// 				tem3=*(pImageBuff+i*lImageWidth*3+3*j+2);
// 				*(pImageBuff+i*lImageWidth*3+3*j)=*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j));
// 				*(pImageBuff+i*lImageWidth*3+3*j+1)=*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j)+1);
// 				*(pImageBuff+i*lImageWidth*3+3*j+2)=*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j)+2);
// 				*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j))=tem1;
// 				*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j)+1)=tem2;
// 				*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j)+2)=tem3;

			}
	
		}	
	}


	return 0;
}

BOOL CMainFrame::AlarmAction(BOOL bStart)
{

#ifdef USE_IO_CARD
	if(bStart)
	{
		m_IOCard[m_iAlarmCardNum-1].setCardOutput(m_iAlarmOutPort,1);
	}
	else
	{
		m_IOCard[m_iAlarmCardNum-1].setCardOutput(m_iAlarmOutPort,0);
	}
#endif

	return TRUE;
}


// ����ȱ����Ϣ��¼
void CMainFrame::UpdateDefectTableData(s_CheckDefectInfo info, CString strTableName)
{
	int i=0,j=0;
	CString strSQL(""),strTemp("");
	strSQL.Format("Select * from %s", strTableName);
	(m_RecordSet).CreateInstance(__uuidof(Recordset));
	
	try
	{
		(m_RecordSet)->Open(strSQL.GetBuffer(strSQL.GetLength()),
			_variant_t((IDispatch*)(m_Connect),true),
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch (_com_error * e)
	{
		AfxMessageBox(e->ErrorMessage());
		return;
	}
	
	if((m_RecordSet)->ADOEOF)
	{
		(m_RecordSet)->PutCollect("StartTime",_variant_t(info.sStartTime));
		(m_RecordSet)->PutCollect("StopTime",_variant_t(info.sStopTime));
		(m_RecordSet)->PutCollect("ModelName", _variant_t(info.sModelName));
		(m_RecordSet)->PutCollect("TotalCount", _variant_t((long)info.iTotalCount));
		(m_RecordSet)->PutCollect("TotalGood", _variant_t((long)info.iTotalGood));
		(m_RecordSet)->PutCollect("TotalBad", _variant_t((long)info.iTotalBad));
		(m_RecordSet)->PutCollect("Total1", _variant_t((long)info.iTotal1));
		(m_RecordSet)->PutCollect("Bad1", _variant_t((long)info.iBad1));
		(m_RecordSet)->PutCollect("Total2", _variant_t((long)info.iTotal2));
		(m_RecordSet)->PutCollect("Bad2", _variant_t((long)info.iBad2));

		for(i=0;i<c_AllElementCount;i++)
		{
			for(j=0;j<MAX_DEFECT_TYPE_COUNT;j++)
			{
				strTemp.Format("Type%d",(i*MAX_DEFECT_TYPE_COUNT+j+1));
				(m_RecordSet)->PutCollect(_variant_t(strTemp), _variant_t((long)info.iDefectCount[i*MAX_DEFECT_TYPE_COUNT+j]));
			}
		}

		(m_RecordSet)->Update();
	}
	else
	{
		while(!(m_RecordSet)->ADOEOF)
		{
			(m_RecordSet)->MoveNext();
			if((m_RecordSet)->ADOEOF)
			{
				(m_RecordSet)->MovePrevious();
				(m_RecordSet)->PutCollect("StartTime",_variant_t(info.sStartTime));
				(m_RecordSet)->PutCollect("StopTime",_variant_t(info.sStopTime));
				(m_RecordSet)->PutCollect("ModelName", _variant_t(info.sModelName));
				(m_RecordSet)->PutCollect("TotalCount", _variant_t((long)info.iTotalCount));
				(m_RecordSet)->PutCollect("TotalGood", _variant_t((long)info.iTotalGood));
				(m_RecordSet)->PutCollect("TotalBad", _variant_t((long)info.iTotalBad));
				(m_RecordSet)->PutCollect("Total1", _variant_t((long)info.iTotal1));
				(m_RecordSet)->PutCollect("Bad1", _variant_t((long)info.iBad1));
				(m_RecordSet)->PutCollect("Total2", _variant_t((long)info.iTotal2));
				(m_RecordSet)->PutCollect("Bad2", _variant_t((long)info.iBad2));
				
				for(i=0;i<c_AllElementCount;i++)
				{
					for(j=0;j<MAX_DEFECT_TYPE_COUNT;j++)
					{
						strTemp.Format("Type%d",(i*MAX_DEFECT_TYPE_COUNT+j+1));
						(m_RecordSet)->PutCollect(_variant_t(strTemp), _variant_t((long)info.iDefectCount[i*MAX_DEFECT_TYPE_COUNT+j]));
					}
				}

				(m_RecordSet)->Update();
				break;
			}
		}
	}
	
	(m_RecordSet)->Close();
}	

// ����һ��ȱ����Ϣ�¼�¼
void CMainFrame::InsertDefectTableData(s_CheckDefectInfo info, CString strTableName)
{
	int i=0,j=0;
	CString strSQL(""),strTemp("");
	strSQL.Format("Select * from %s", strTableName);
	(m_RecordSet).CreateInstance(__uuidof(Recordset));

	try
	{
		(m_RecordSet)->Open(strSQL.GetBuffer(strSQL.GetLength()),
			_variant_t((IDispatch*)(m_Connect),true),
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch (_com_error* e)
	{
		AfxMessageBox(e->ErrorMessage());
		return;
	}

	if((m_RecordSet)->ADOEOF)
	{
		(m_RecordSet)->AddNew();
		(m_RecordSet)->PutCollect("StartTime",_variant_t(info.sStartTime));
		(m_RecordSet)->PutCollect("StopTime",_variant_t(info.sStopTime));
		(m_RecordSet)->PutCollect("ModelName", _variant_t(info.sModelName));
		(m_RecordSet)->PutCollect("TotalCount", _variant_t((long)info.iTotalCount));
		(m_RecordSet)->PutCollect("TotalGood", _variant_t((long)info.iTotalGood));
		(m_RecordSet)->PutCollect("TotalBad", _variant_t((long)info.iTotalBad));
		(m_RecordSet)->PutCollect("Total1", _variant_t((long)info.iTotal1));
		(m_RecordSet)->PutCollect("Bad1", _variant_t((long)info.iTotalBad));
		(m_RecordSet)->PutCollect("Total2", _variant_t((long)info.iTotalCount));
		(m_RecordSet)->PutCollect("Bad2", _variant_t((long)info.iTotalBad));
		
		for(i=0;i<c_AllElementCount;i++)
		{
			for(j=0;j<MAX_DEFECT_TYPE_COUNT;j++)
			{
				strTemp.Format("Type%d",(i*MAX_DEFECT_TYPE_COUNT+j+1));
				(m_RecordSet)->PutCollect(_variant_t(strTemp), _variant_t((long)info.iDefectCount[i*MAX_DEFECT_TYPE_COUNT+j]));
			}
		}

		(m_RecordSet)->Update();
	}
	else
	{
		while(!(m_RecordSet)->ADOEOF)
		{
			(m_RecordSet)->MoveNext();
			if((m_RecordSet)->ADOEOF)
			{
				(m_RecordSet)->AddNew();
				(m_RecordSet)->PutCollect("StartTime",_variant_t(info.sStartTime));
				(m_RecordSet)->PutCollect("StopTime",_variant_t(info.sStopTime));
				(m_RecordSet)->PutCollect("ModelName", _variant_t(info.sModelName));
				(m_RecordSet)->PutCollect("TotalCount", _variant_t((long)info.iTotalCount));
				(m_RecordSet)->PutCollect("TotalGood", _variant_t((long)info.iTotalGood));
				(m_RecordSet)->PutCollect("TotalBad", _variant_t((long)info.iTotalBad));
				(m_RecordSet)->PutCollect("Total1", _variant_t((long)info.iTotal1));
				(m_RecordSet)->PutCollect("Bad1", _variant_t((long)info.iTotalBad));
				(m_RecordSet)->PutCollect("Total2", _variant_t((long)info.iTotalCount));
				(m_RecordSet)->PutCollect("Bad2", _variant_t((long)info.iTotalBad));
				
				for(i=0;i<c_AllElementCount;i++)
				{
					for(j=0;j<MAX_DEFECT_TYPE_COUNT;j++)
					{
						strTemp.Format("Type%d",(i*MAX_DEFECT_TYPE_COUNT+j+1));
						(m_RecordSet)->PutCollect(_variant_t(strTemp), _variant_t((long)info.iDefectCount[i*MAX_DEFECT_TYPE_COUNT+j]));
					}
				}

				(m_RecordSet)->Update();	
				break;
			}
		}
	}

	(m_RecordSet)->Close();
}


//�޷ϻص�
void CMainFrame::GlobalKickCallbackFunc(const s_KickParam* pParam)
{
	if (pParam && pParam->pThis)
    {
        CMainFrame* pMF = (CMainFrame*) pParam->pThis;
		pMF->KickCallbackFunc1(pParam);
    }
}


void CMainFrame::KickCallbackFunc1(const s_KickParam* sParam)
{
	int i{ 0 };
	int iImageSN=0,iKickSN=0;
	iImageSN = sParam->iImageSN;
	iKickSN = sParam->iKickSN;

	bool bKick[c_AllElementCount]{0};

	LabelCheck::s_AlgInspResult sOut[c_AllElementCount]{0};

	if(e_KickPort_1==iKickSN)
	{
		memcpy(sOut,sParam->pParam,4*sizeof(LabelCheck::s_AlgInspResult));
		if (e_AlgStatus_Good == sOut[Camera1].nAlgStatus|| e_AlgStatus_Bad == sOut[Camera1].nAlgStatus||e_AlgStatus_AlgException == sOut[Camera1].nAlgStatus)
		{
			++m_iCheckCountTotal;
		}
	}
	else if(e_KickPort_2==iKickSN)
	{
		memcpy(&sOut[Camera5],sParam->pParam,sizeof(LabelCheck::s_AlgInspResult));
	}
	else if(e_KickPort_3==iKickSN)
	{
		memcpy(&sOut[Camera6],sParam->pParam,sizeof(LabelCheck::s_AlgInspResult));
	}

	for(int i=0;i<c_AllElementCount;i++)
	{
		m_dAlgCheckTime[i] = sOut[i].nTimeTotle;

		if(m_iSaveCameraImageCount_Setting[i]>0 || m_bSaveCameraImage[i])
		{
			if(m_iSaveCameraImageCount[i]<m_iSaveCameraImageCount_Setting[i] )
			{
				if(0==m_iSaveImageType)		//����ͼ��
				{
					m_iSaveCameraImageCount[i]++;
				}
				else if(1==m_iSaveImageType)	//����ͼ��
				{
					if(e_AlgStatus_Good!=sOut[i].nAlgStatus && e_AlgStatus_Bad!=sOut[i].nAlgStatus)
					{
						m_iSaveCameraImageCount[i]++;
					}
				}
			}
		}

		BOOL bComplete = TRUE;
		for(int j=0;j<c_AllElementCount;j++)
		{
			if(m_iSaveCameraImageCount[i]<m_iSaveCameraImageCount_Setting[i] )
			{
				bComplete=FALSE;
				break;
			}
		}

		if(TRUE==bComplete)
		{
			for(int j=0;j<c_AllElementCount;j++)
			{
				m_iSaveCameraImageCount_Setting[i]=0;
				//m_bSaveCameraImage[i]=FALSE;
			}
			PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,StopSave);			
		}
		else
		{
			if(SaveGrabImage==m_iSaveImageType)
			{
				PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,ContinueImageSaving);
			}
			else if(SaveErrorImage==m_iSaveImageType)
			{
				PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,ErrorImageSaving);
			}
		}
	}

	if(e_KickMode_KickAll==m_iKickMode)		//ȫ��
	{
		/** Update check error count              [5/16/2017 ConanSteve]**/
		switch (iKickSN)
		{
		case e_KickPort_1:
			for (int i = 0; i < Camera5; ++i)
			{
				++m_iCheckErrorCount[i];
			}
			break;
		case e_KickPort_2:
			++m_iCheckErrorCount[Camera5];
			break;
		case e_KickPort_3:
			++m_iCheckErrorCount[Camera6];
			break;
		default:
			break;
		}
		/** Write result              [5/16/2017 ConanSteve]**/
		if (TRUE == m_bIOCardValid)
		{
			switch (iKickSN)
			{
			case e_KickPort_1:
				m_IOCard[e_IOCard_1].sendChckRslt(TRUE, iImageSN % 256, Camera1);
				break;
			case e_KickPort_2:
				m_IOCard[e_IOCard_1].sendChckRslt(TRUE, iImageSN % 256, Camera4);
				break;
			case e_KickPort_3:
				m_IOCard[e_IOCard_2].sendChckRslt(TRUE, iImageSN % 256, Camera1);
				break;
			default:
				break;
			}
		}
	}
	else if(e_KickMode_AllNotKick==m_iKickMode)	//ȫ�����߷�
	{		
		if (TRUE == m_bIOCardValid)
		{
			switch (iKickSN)
			{
			case e_KickPort_1:
				m_IOCard[e_IOCard_1].sendChckRslt(FALSE, iImageSN % 256, Camera1);
				break;
			case e_KickPort_2:
				m_IOCard[e_IOCard_1].sendChckRslt(FALSE, iImageSN % 256, Camera4);
				break;
			case e_KickPort_3:
				m_IOCard[e_IOCard_2].sendChckRslt(FALSE, iImageSN % 256, Camera1);
				break;
			default:
				break;
			}
		}
	}
	else if(e_KickMode_Alternative==m_iKickMode)	//������
	{
		if(iImageSN%2==0)
		{
			/** Update check error count              [5/16/2017 ConanSteve]**/
			switch (iKickSN)
			{
			case e_KickPort_1:
				for (int i = 0; i < Camera5; ++i)
				{
					++m_iCheckErrorCount[i];
				}
				break;
			case e_KickPort_2:
				++m_iCheckErrorCount[Camera5];
				break;
			case e_KickPort_3:
				++m_iCheckErrorCount[Camera6];
				break;
			default:
				break;
			}
			/** Write result              [5/16/2017 ConanSteve]**/
			if (TRUE == m_bIOCardValid)
			{
				switch (iKickSN)
				{
				case e_KickPort_1:
					m_IOCard[e_IOCard_1].sendChckRslt(TRUE, iImageSN % 256, Camera1);
					break;
				case e_KickPort_2:
					m_IOCard[e_IOCard_1].sendChckRslt(TRUE, iImageSN % 256, Camera4);
					break;
				case e_KickPort_3:
					m_IOCard[e_IOCard_2].sendChckRslt(TRUE, iImageSN % 256, Camera1);
					break;
				default:
					break;
				}
			}
		}
		else
		{
			if (TRUE == m_bIOCardValid)
			{
				if (e_KickPort_1 == iKickSN)
				{
					m_IOCard[e_IOCard_1].sendChckRslt(FALSE, iImageSN % 256, Camera1);
				}
				else if (e_KickPort_2 == iKickSN)
				{
					m_IOCard[e_IOCard_1].sendChckRslt(FALSE, iImageSN % 256, Camera4);
				}
				else if (e_KickPort_3 == iKickSN)
				{
					m_IOCard[e_IOCard_2].sendChckRslt(FALSE, iImageSN % 256, Camera1);
				}
			}
		}
	}
	else if(e_KickMode_Noraml==m_iKickMode)		//�����޷�
	{
		switch (iKickSN)
		{
		case e_KickPort_1:
		{
			for (i = 0; i < Camera5; i++)
			{
				/************************************************************************/
				/*                       [6/15/2017 ConanSteve]                         */
				if (e_AlgStatus_Bad == sOut[i].nAlgStatus)
				{
					++m_iCheckErrorCount[i];
				}
				if (e_AlgStatus_Good == sOut[i].nAlgStatus || e_AlgStatus_Bad == sOut[i].nAlgStatus || e_AlgStatus_AlgException == sOut[Camera1].nAlgStatus)
				{
					++m_iCheckTubeCnt[i];
				}
				if (e_AlgStatus_AlgException == sOut[i].nAlgStatus)
				{
					++m_iAlgExceptionCnt[i];
				}

				/************************************************************************/
				if (e_AlgStatus_Good != sOut[i].nAlgStatus && e_AlgStatus_Empty != sOut[i].nAlgStatus)	// ���� 
				{

					if (TRUE == m_bIsKick[i])
					{
						bKick[i] = TRUE;
					}
					else
					{
						bKick[i] = FALSE;
					}

					if (TRUE == m_bStartRecord)
					{
						CString str1;
						str1.Format("KickSN:%d-%d=Cam:%d=Image:%d=ET:%d=Time:%s=\r\n", iKickSN, m_bIsKick[iKickSN],
							i, iImageSN, sOut[i].nAlgStatus, GetCurTime());
						m_fileKick[i].Write(str1);
					}
				}
				else
				{
					bKick[i] = FALSE;
					if (TRUE == m_bStartRecord)
					{
						CString str1;

						str1.Format("KickSN:%d-%d=Cam:%d=Image:%d=ET:%d=Time:%s=\r\n", iKickSN, m_bIsKick[iKickSN], i, iImageSN, sOut[i].nAlgStatus, GetCurTime());

						m_iRC_Good[i]++;
						if (m_iRC_Good[i] > m_iRecord_Set)
						{
							m_iRC_Good[i] = 0;
							m_fileGood[i].SeekToBegin();
						}
						m_fileGood[i].Write(str1.GetBuffer(0), str1.GetLength());
					}
				}
			}
			bool bRet = TRUE == bKick[Camera1] || TRUE == bKick[Camera2] || TRUE == bKick[Camera3] || TRUE == bKick[Camera4];
			if (!bRet)//����Ǻùܣ�����,����Ƿ���ڿչܣ�����У���
			{
				if (e_AlgStatus_Good == sOut[Camera1].nAlgStatus && (e_AlgStatus_Empty == sOut[Camera2].nAlgStatus ||
					e_AlgStatus_Empty == sOut[Camera3].nAlgStatus
					|| e_AlgStatus_Empty == sOut[Camera4].nAlgStatus))
				{
					bRet = true;
				}
			}
			if (iImageSN <= m_iOffsetBtw1And4)//���ͼ���С�ڵ���һ�Ĺ�λƫ�Ʋ���
				bRet = FALSE;
			if ( bRet )
			{

				if (TRUE == m_bIOCardValid)
				{
					m_IOCard[e_IOCard_1].sendChckRslt(TRUE, iImageSN % 256, Camera1); 					
				}
				
			}
			else
			{
				if (TRUE == m_bIOCardValid)
				{
					m_IOCard[e_IOCard_1].sendChckRslt(FALSE, iImageSN % 256, Camera1);
				}
			}
			/*************************************<����ͣ��>************************************[12/20/2017 ConanSteve]*/
			for (int k = Camera1; k< Camera5; ++k)
			{
				if (2 == sOut[k].nAlgStatus || 1 == sOut[k].nAlgStatus)//**[12/20/2017 ConanSteve]**:  
				{
					++m_arr_iCurNumOfConstantKick[k];
				}
				else
				{
					m_arr_iCurNumOfConstantKick[k] = 0;
				}
				if (m_arr_iCurNumOfConstantKick[k] >= m_iMaxNumOfConstantKick)
				{
					AlarmStopMachine(_T("�������󱨾�ͣ��"));
					fill(m_arr_iCurNumOfConstantKick.begin(), m_arr_iCurNumOfConstantKick.end(), 0);
				}
			}
		}
			break;
		case e_KickPort_2:
		{
			for (i = Camera5; i <= Camera5; i++)
			{
				/************************************************************************/
				/*                       [6/15/2017 ConanSteve]                         */
				if (e_AlgStatus_Bad == sOut[i].nAlgStatus)
				{
					++m_iCheckErrorCount[i];
				}
				//if (e_AlgStatus_Good == sOut[i].nAlgStatus || e_AlgStatus_Bad == sOut[i].nAlgStatus)
				{
					++m_iCheckTubeCnt[i];
				}
				if (e_AlgStatus_AlgException == sOut[i].nAlgStatus)
				{
					++m_iAlgExceptionCnt[i];
				}
				/************************************************************************/
				if (sOut[i].nAlgStatus != e_AlgStatus_Good && e_AlgStatus_Empty != sOut[i].nAlgStatus)	// ���� 
				{

					if (TRUE == m_bIsKick[i])
					{
						bKick[i] = TRUE;
					}
					else
					{
						bKick[i] = FALSE;
					}

					if (TRUE == m_bStartRecord)
					{
						CString str1;

						str1.Format("KickSN:%d-%d=Cam:%d=Image:%d=ET:%d=\r\n", iKickSN, m_bIsKick[iKickSN],
							i, iImageSN, sOut[i].nAlgStatus);
						//m_arr_fileKick[i].Write(str1);
					}
				}
				else
				{
					bKick[i] = FALSE;

					if (TRUE == m_bStartRecord)
					{
						CString str1;

						str1.Format("KickSN:%d-%d=Cam:%d=Image:%d=ET:%d=Time:%s=\r\n", iKickSN, m_bIsKick[iKickSN], i, iImageSN, sOut[i].nAlgStatus, GetCurTime());

						m_iRC_Good[i]++;
						if (m_iRC_Good[i] > m_iRecord_Set)
						{
							m_iRC_Good[i] = 0;
							m_fileGood[i].SeekToBegin();
						}
						m_fileGood[i].Write(str1.GetBuffer(0), str1.GetLength());
					}
				}
			}
			/************************************************************************/
			/*                [5/8/2017 ConanSteve]                                 */
			m_csLabelRsltTable.Lock();
			if (!m_iWaitRsltFrame.empty())
			{
				s_5CheckInfo s5CheckInfo;
				s5CheckInfo.bKick = bKick[Camera5];
				//s5CheckInfo.bKick = true;
				s5CheckInfo.iImgSN5 = iImageSN;
				s5CheckInfo.iCheckRslt = sOut[Camera5].nAlgStatus;
				m_sLabelRst.push(s5CheckInfo);
				//m_arrKickTable2[(m_iFrameCount[Camera6] + m_iDisBtwn5And6) % 256] = m_bLabelRst.front();
				int iImgsSN6 = (m_iFrameCount[Camera6] + m_iDisBtwn5And6);// -(m_iWaitRsltFrame.size() - 1));
				m_arrKickTable2[iImgsSN6%256] = m_sLabelRst.front();
				CString sInfo{ _T("") };
				sInfo.Format(_T("[ImageSN5:%d][ImgSN6:%d][Bool5:%s][WatiRsltFrameSize:%d]"), m_sLabelRst.front().iImgSN5, iImgsSN6, m_sLabelRst.front().bKick?_T("TRUE"):_T("FALSE"),  m_iWaitRsltFrame.size());
				m_fileKick[4].Write(sInfo);
				m_sLabelRst.pop();
				m_iWaitRsltFrame.pop();
			}
			m_csLabelRsltTable.Unlock();
			/************************************************************************/
		}
			break;
		case e_KickPort_3:
		{
			for (i = Camera6; i <= Camera6; i++)
			{
				/** �ùܼ���              [6/21/2017 ConanSteve]**/
				if (e_AlgStatus_Good == sOut[i].nAlgStatus)
				{
					++m_iCheckGoodCountTotal;
				}
				/************************************************************************/
				if (e_AlgStatus_Good == sOut[i].nAlgStatus || e_AlgStatus_Bad == sOut[i].nAlgStatus || e_AlgStatus_AlgException == sOut[i].nAlgStatus)
				{
					++m_iCheckTubeCnt[i];
				}
				if (e_AlgStatus_AlgException == sOut[i].nAlgStatus)
				{
					++m_iAlgExceptionCnt[i];
				}
				//if (5 == sOut[i].nAlgStatus)
				//{
				//	++m_iCheckErrorCount[i];
				//}
				/************************************************************************/
				if (e_AlgStatus_Good != sOut[i].nAlgStatus && e_AlgStatus_Empty != sOut[i].nAlgStatus)	// ���� 
				{
					++m_iCheckErrorCount[i];
					if (TRUE == m_bIsKick[i])
					{
						bKick[i] = TRUE;
					}
					else
					{
						bKick[i] = FALSE;
					}

					if (TRUE == m_bStartRecord)
					{
						CString str1;

						str1.Format("KickSN:%d-%d=Cam:%d=Image:%d=ET:%d=", iKickSN, m_bIsKick[iKickSN],
							i, iImageSN, sOut[i].nAlgStatus);
						//m_arr_fileKick[i].Write(str1);
					}
				}
				else/** good product              [6/9/2017 ConanSteve]**/
				{
					bKick[i] = FALSE;
					if (TRUE == m_bStartRecord)
					{
						CString str1;

						str1.Format("KickSN:%d-%d=Cam:%d=Image:%d=ET:%d=Time:%s=\r\n", iKickSN, m_bIsKick[iKickSN], i, iImageSN, sOut[i].nAlgStatus, GetCurTime());

						m_iRC_Good[i]++;
						if (m_iRC_Good[i] > m_iRecord_Set)
						{
							m_iRC_Good[i] = 0;
							m_fileGood[i].SeekToBegin();
						}
						m_fileGood[i].Write(str1.GetBuffer(0), str1.GetLength());
					}
				}
			}
			m_csLabelRsltTable.Lock();
			if (true == bKick[Camera6] || true == m_arrKickTable2[iImageSN % 256].bKick)
			{
				if (true == bKick[Camera6] && true == m_arrKickTable2[iImageSN % 256].bKick)
				{
					m_iErrorCntOf5And6++;
				}
				if (true == m_bIOCardValid)
				{
					m_IOCard[e_IOCard_2].sendChckRslt(TRUE, iImageSN % 256, 0);
					CString sInfo{ _T("") };
					sInfo.Format(_T("[ImgSN5:%8d][ImgSN6:%8d][bKick6:%s][bKick5:%s][iAlgStatus6:%d]"), m_arrKickTable2[iImageSN % 256].iImgSN5,
						iImageSN, bKick[Camera6] ? _T("true") : _T("false"), m_arrKickTable2[iImageSN % 256].bKick ? _T("true") : _T("false"),
						 sOut[Camera6].nAlgStatus);
					m_fileKick[5].Write(sInfo);
				}
			}
			else
			{
				if (TRUE == m_bIOCardValid)
				{
					m_IOCard[e_IOCard_2].sendChckRslt(FALSE, iImageSN % 256, 0);
				}
			}
			m_arrKickTable2[iImageSN % 256].bKick = false;
			m_csLabelRsltTable.Unlock();
			/*************************************<ͣ��>************************************[12/20/2017 ConanSteve]*/
			for (int j = 0; j < 5; ++j)
			{
				if (0 != m_arr_iAlgStopMachineCtrl[j] && sOut[Camera6].nStopMachineType[j])
				{
 					CString sErrorInfo{ _T("") };
// 					switch (j)
// 					{
// 					case 0:
// 						sErrorInfo = _T("�ޱ�ǩ(���Ӳ�ת)ͣ��");
// 						break;
// 					case 1:
// 						sErrorInfo = _T("��ǩͣ��");
// 						break;
// 					default:
// 						break;
// 					}
					sErrorInfo = _T("ȱ��ͣ��");
					AlarmStopMachine(sErrorInfo);
					return;
				}
			}
			for (int k = Camera6; k < 6; ++k)
			{
				if (2 == sOut[Camera6].nAlgStatus || 1 == sOut[Camera6].nAlgStatus)//**[12/20/2017 ConanSteve]**:  
				{
					++m_arr_iCurNumOfConstantKick[Camera6];
				}
				else
				{
					m_arr_iCurNumOfConstantKick[Camera6] = 0;
				}
				if (m_arr_iCurNumOfConstantKick[Camera6] >= m_iMaxNumOfConstantKick)
				{
					AlarmStopMachine(_T("�������󱨾�ͣ��"));
					fill(m_arr_iCurNumOfConstantKick.begin(), m_arr_iCurNumOfConstantKick.end(), 0);
				}
			}
			
		}
			break;
		default:
			break;
		}

		
	}
}

//״̬�ص�
void CMainFrame::GlobalStatusCallbackFunc(const s_Status* pParam)
{
	if (pParam && pParam->pThis)
    {
        CMainFrame* pMF = (CMainFrame*) pParam->pThis;
		pMF->StatusCallbackFunc1(pParam);
    }
}


void CMainFrame::StatusCallbackFunc1(const s_Status* sParam)
{
	
}

DWORD CMainFrame::GetIOCardImageNumThread(LPVOID lpParam)
{
	CMainFrame* This = (CMainFrame*)lpParam;
	
	DWORD dRet = This->GetIOCardImageNumThreadFunc();
	
	return dRet;
}
DWORD CMainFrame::GetIOCardImageNumThreadFunc()
{
	
//	return 0;

	while(m_bCloseCount==FALSE)
	{
		
		if(TRUE==m_bIsDetecting)
		{
			GetCount_Guangdian();
			
			GetCount_Tifei();
			
			GetCount_TuXiangHao();
		}
		Sleep(1);
	}
	
	m_evtGetIOCardImageNum.SetEvent();

	return 0;
}

BOOL CMainFrame::GetCount_Guangdian()
{		
	int i=0;
	int iStatus = 0;		//״̬�Ĵ���
	int iIn0 =0;			//��1 In0��״̬
	int iDiffBottle=0;		//��������ֵ
		
	array<int, c_AllElementCount> nPECount{0};
	
	array<double,c_AllElementCount> dd1{0.0};
	array<double,c_AllElementCount> dDiff{0.0};
	
	array<LONGLONG,c_AllElementCount> lQt1{0};
	array<LONGLONG,c_AllElementCount> lQt2{0};
	array<LONGLONG,c_AllElementCount> lLastQt2{0};
	
	array<LARGE_INTEGER,c_AllElementCount> iLarge;
	
	CString strText,strInfo;
	
	//������
#ifdef USE_IO_CARD
	nPECount[Camera1] = nPECount[Camera2] = nPECount[Camera3] = nPECount[Camera4] = m_IOCard[e_IOCard_1].readCounter(0)%256;
	nPECount[Camera5] = m_IOCard[e_IOCard_1].readCounter(1)%256;
	nPECount[Camera6] = m_IOCard[e_IOCard_2].readCounter(1) % 256;/** �ֳ���2��IN0��Ϊ��IN1              [5/18/2017 ConanSteve]**/
#else
	for (int i = 0; i < c_AllElementCount; ++i)
	{
		nPECount[i] = m_iGrabCircleCount[i] % 256;
	}
#endif
	
	for(i=0;i<c_AllElementCount;i++)
	{			
		//��n������������
		if(m_iLastFrameCount[i] > 200 && nPECount[i] <= 50)
		{
			++m_iFrameCountOverFlowNumber[i];
		}
		
		m_iFrameCount[i] = nPECount[i] + 256*m_iFrameCountOverFlowNumber[i];
		
		if(nPECount[i] != m_iLastFrameCount[i])
		{	
			
			QueryPerformanceCounter(&iLarge[i]);
			lQt1[i]=iLarge[i].QuadPart;
			dd1[i]=lQt1[i]-m_lLastQt_PE[i];
			dDiff[i]=1000*dd1[i]/m_dFreq;
			m_lLastQt_PE[i]=lQt1[i];


// 			CString csTemp{ TEXT("") };
// 			SYSTEMTIME sysTime{ 0 };
// 			GetLocalTime(&sysTime);
// 			csTemp.Format(_T("%04d%02d%02d_%02d:%02d:%02d:\t\t"), sysTime.wYear, sysTime.wMonth, sysTime.wDay,
// 				sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
			strInfo.Format("=ͼ���:%06d=%s=���:%0.0f=Last:%d=PE:%d\r\n",
				m_iFrameCount[i],GetCurTime(),dDiff[i],m_iLastFrameCount[i],nPECount[i]);
	/*		strInfo = csTemp + strInfo;*/
			
			if(TRUE==m_bStartRecord)
			{
				m_iRC_PE[i]++;
				if(m_iRC_PE[i]>m_iRecord_Set)
				{
					m_iRC_PE[i]=0;
					m_filePE[i].SeekToBegin();
				}
				m_filePE[i].Write(strInfo.GetBuffer(255),strInfo.GetLength());
			}
			m_iLastFrameCount[i] = nPECount[i];// ��¼���n��һ֡����
			if (Camera5 == i)/** ������5�Ĺ���Ӧ����Ѫ�ܣ�Ϊ���5�ġ��ȴ��޷ϻص���������һ��������              [5/16/2017 ConanSteve]**/
			{
				m_csLabelRsltTable.Lock();
				m_iWaitRsltFrame.push(m_iFrameCount[Camera5]);
				m_arr_ImgSNFormFive2Six[m_iFrameCount[Camera5]%256] = m_iFrameCount[Camera6];
				m_csLabelRsltTable.Unlock();
			}
		}	
	}

	return TRUE;
}

BOOL CMainFrame::GetCount_Tifei()
{
	float iBadRatio=0.0;
	
	int i=0;		
	int nKickCount[c_AllElementCount]{0};
	//memset(nKickCount,0,sizeof(int)*c_KickCount);		
	CString strInfo;

#ifdef USE_IO_CARD
	nKickCount[Camera1] = nKickCount[Camera2]= nKickCount[Camera3]= nKickCount[Camera4]=m_IOCard[e_IOCard_1].readCounter(4)%256;   //��ȡ�߷�1���޷ϼ���
	//nKickCount[Camer5] = m_IOCard[e_IOCard_1].readCounter(7)%256;   //��ȡ�߷�2���޷ϼ���
	nKickCount[Camera6] = m_IOCard[e_IOCard_2].readCounter(4)%256;   //��ȡ�߷�3���޷ϼ���
#else
	for (int i = 0; i < c_AllElementCount; ++i)
	{
		nKickCount[i] = m_iCheckErrorCount[i] % 256;
	}
#endif
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// �ж����1���޷ϼ���
	// �ж���һ֡�ļ���Ϊ255����һ֡�ļ���Ϊ0����������������Ͻ�֡���������ۼ�
	
	for(i=0;i<c_AllElementCount;i++)
	{
		if(m_iLastKickCount[i] > 200 && nKickCount[i] <= 50)
		{
			m_iKickFlowNumber[i] += 1;
		}
		// ������������Ϊ��������ۼӣ�����һ 
		m_iHardKickCount[i] = nKickCount[i] + 256*m_iKickFlowNumber[i];	
		
		if(m_iLastKickCount[i]!=nKickCount[i])
		{
			// ��ǩ��λ�޷ϼ����仯
			//if(e_KickPort_2==i)
			//{
			//	m_sOut5[(m_iFrameCount[Camera6]+m_iKickOffset5)%256].nAlgStatus=e_AlgStatus_Bad;
			//}
			m_iLastKickCount[i] = nKickCount[i];// ��¼��һ֡����
				
			if(TRUE==m_bStartRecord && TRUE==m_bIsDetecting)
			{

				

				strInfo.Format("=Group:%d=Image:%d=%s=\r\n",i,m_iHardKickCount[i],
					GetCurTime());
				m_iRC_KickImage[i]++;
				if(m_iRC_KickImage[i]>m_iRecord_Set)
				{
					m_iRC_KickImage[i]=0;
					m_fileKickImage[i].SeekToBegin();
				}
				/************************************************************************/
				/*                       [5/19/2017 ConanSteve]                         */
// 				CString csTemp{ TEXT("") };
// 				SYSTEMTIME sysTime{ 0 };
// 				GetLocalTime(&sysTime);
// 				csTemp.Format(_T("%04d%02d%02d_%02d:%02d:%02d:\t\t"), sysTime.wYear, sysTime.wMonth, sysTime.wDay,
// 					sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
// 				strInfo = csTemp + strInfo;
				/************************************************************************/
				m_fileKickImage[i].Write(strInfo.GetBuffer(0),strInfo.GetLength());
			}
		}
	}
	
	return TRUE;
}

BOOL CMainFrame::GetCount_TuXiangHao()
{
	int i=0;
	
	int nImageCount[c_AllElementCount];
	memset(nImageCount,0,sizeof(int)*c_AllElementCount);
		
	int nLastRecordKick[c_AllElementCount];
	memset(nLastRecordKick,0,sizeof(int)*c_AllElementCount);
	
	double dd1[c_AllElementCount] = { 0.0 };
	double dDiff[c_AllElementCount] = { 0.0 };

	LONGLONG lQt1[c_AllElementCount] = { 0 };
	LONGLONG lQt2[c_AllElementCount]={0};
	
	LARGE_INTEGER iLarge[c_AllElementCount];
	
	CString strText,strInfo;

#ifdef USE_IO_CARD
	nImageCount[Camera1]=m_IOCard[Camera1].readCounter(28)%256;
	nImageCount[Camera2]=m_IOCard[Camera1].readCounter(28)%256;
	nImageCount[Camera3]=m_IOCard[Camera1].readCounter(28)%256;
	nImageCount[Camera4]=m_IOCard[Camera1].readCounter(28)%256;
	nImageCount[Camera5]=m_IOCard[Camera1].readCounter(29)%256;
	nImageCount[Camera6]=m_IOCard[Camera2].readCounter(28)%256;
#else
	nImageCount[Camera1]=m_iGrabCircleCount[Camera1]%256;
	nImageCount[Camera2]=m_iGrabCircleCount[Camera2]%256;
	nImageCount[Camera3]=m_iGrabCircleCount[Camera3]%256;
	nImageCount[Camera4]=m_iGrabCircleCount[Camera4]%256;
	nImageCount[Camera5]=m_iGrabCircleCount[Camera5]%256;
	nImageCount[Camera6]=m_iGrabCircleCount[Camera6]%256;
#endif
	
	// ͼ��żĴ���
	for(i=0;i<c_AllElementCount;i++)
	{			
		if(m_iLastImageCountInPE[i]>200 && nImageCount[i]<50)
		{
			m_iFlowOverImageCountInPE[i]++;
		}	
		m_iImageCountInPE[i]=nImageCount[i]+256*m_iFlowOverImageCountInPE[i];
	
		if(nImageCount[i] != m_iLastImageCountInPE[i])
		{	
			if(Camera6==i)
			{
				memset(&m_sOut5[m_iImageCountInPE[i]%256],0,sizeof(LabelCheck::s_AlgInspResult));
			}

			// [2014-10-21 zhy]
			QueryPerformanceCounter(&iLarge[i]);

			lQt2[i]=iLarge[i].QuadPart;
			dd1[i]=lQt2[i]-m_lLastQt_TX[i];
			dDiff[i]=1000*dd1[i]/m_dFreq;
			m_lLastQt_TX[i]=lQt2[i];		

			strInfo.Format("=ͼ���:%06d=���:%0.0f=%s=\r\n",
				m_iImageCountInPE[i],dDiff[i],GetCurTime());
			
			if(TRUE==m_bStartRecord && TRUE==m_bIsDetecting)
			{
				m_iRC_RecordImage[i]++;
				if(m_iRC_RecordImage[i]>m_iRecord_Set)
				{
					m_iRC_RecordImage[i]=0;
					m_fileRecordImage[i].SeekToBegin();
				}
				m_fileRecordImage[i].Write(strInfo.GetBuffer(0),strInfo.GetLength());
			}
			m_iLastImageCountInPE[i]=nImageCount[i];
		}
	}
	
	return TRUE;
}

BOOL CMainFrame::LoadCamConfig(CString strModelName)
{
	int i{0};
	return true;
	CString strOri,strAlg,strDestPath;
	CString sPathFileOfSysCfgInCfg{ _T("") }, sPathFileOfSysCfgInModelInfo{ _T("") };
	CString sPathFileOfCamInCfg{ _T("") };
	strAlg.Format("%s\\ModelInfo\\%s",m_sWorkPath,strModelName);
#ifdef __RealGrabber_
	for(i=0;i<CAMERA_COUNT;i++)
	{
		sPathFileOfSysCfgInModelInfo.Format("%s\\%s",strAlg, m_sCamConfigName[i]);

		if(FALSE==PathFileExists(sPathFileOfSysCfgInModelInfo))
		{
			CopyFile(m_sCamConfigPath[i], sPathFileOfSysCfgInModelInfo,false);
		}
		else
		{
			CopyFile(sPathFileOfSysCfgInModelInfo,m_sCamConfigPath[i],false);
		}
	}
#endif
	return TRUE;
}


double CMainFrame::GetFreeSpaceOfHardDisk(CString strDisk)
{
	double dRet=0;

	ULARGE_INTEGER lpuse; 
    ULARGE_INTEGER lptotal; 
    ULARGE_INTEGER lpfree; 
    GetDiskFreeSpaceEx(strDisk,&lpuse,&lptotal,&lpfree);  

	dRet = (LONGLONG)lpfree.QuadPart/(1024.0*1024);

	return dRet;
}


void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

	// TODO: Add your message handler code here
}


bool CMainFrame::DeleteDirectory(char* sDirName)
{

	CFileFind tempFind;

	char sTempFileFind[MAX_PATH];

	sprintf(sTempFileFind, "%s/*.*", sDirName);
	BOOL IsFinded = tempFind.FindFile(sTempFileFind);
	while (IsFinded)
	{

		IsFinded = tempFind.FindNextFile();

		if (!tempFind.IsDots())
		{
			char sFoundFileName[MAX_PATH]{ 0 };
			strcpy(sFoundFileName, tempFind.GetFileName().GetBuffer(MAX_PATH));

			if (tempFind.IsDirectory())
			{
				char sTempDir[MAX_PATH]{ 0 };
				sprintf(sTempDir, "%s/%s", sDirName, sFoundFileName);
				DeleteDirectory(sTempDir);
			}
			else
			{
				char sTempFileName[MAX_PATH]{ 0 };
				sprintf(sTempFileName, "%s/%s", sDirName, sFoundFileName);
				DeleteFile(sTempFileName);
			}
		}
	}
	tempFind.Close();
	if (!RemoveDirectory(sDirName))
	{
		return FALSE;
	}
	return TRUE;
}

//���� �����������ӵĺ��� [4/30/2010 GZ]
BOOL  CMainFrame::CameraGrabFuncSingleEx(int iCameraSN,
	int iAllElementSN)//��������Ĳɼ�����
{
	_CrtCheckMemory();
	if (iCameraSN < 0)
	{
		CString  strExceptionDes;
		strExceptionDes.Format("CameraGrabFuncSingle-������%d����!", iCameraSN);
		m_LogManager.MyWriteLogFile("�ɼ��ص�", strExceptionDes, AbnormityLog);
		return FALSE;
	}

	BOOL bRet = FALSE;
	int iGrabCircle = 0, iPicSN = 0;
	bRet = JudgeGrabCircleNew(iCameraSN, iAllElementSN, iGrabCircle, iPicSN);

	try
	{
		/*if (bRet)*/
		{
			CString str1;
			/** Write Logs              [5/5/2017 ConanSteve]**/
			if (TRUE == m_bStartRecord)
			{
				str1.Format("==Image: %06d=Grab: %06d=Pic:%d=%s==\r\n",
					m_iHardImageCount[iAllElementSN], iGrabCircle, iPicSN, GetCurTime());
				m_iRC_ImageNum[iAllElementSN]++;
				if (m_iRC_ImageNum[iAllElementSN] > m_iRecord_Set)
				{
					m_iRC_ImageNum[iAllElementSN] = 0;
					m_fileImageNum[iAllElementSN].SeekToBegin();
				}
				m_fileImageNum[iAllElementSN].Write(str1.GetBuffer(0), str1.GetLength());
			}


			// ��д��ͼ�����ݡ��ɼ���Ϣ���ӿڿ���Ϣ��				
			int nReturn{0};
			char* pImageBuffer{NULL};

			// �õ�ͼ���ڴ��ַ
			bRet = m_CameraGrabber[iCameraSN].GetParamInt(GBImageBufferAddr, nReturn);
			ASSERT(bRet == TRUE);

			pImageBuffer = (char*)nReturn;
			ASSERT(pImageBuffer != NULL);

			s_CheckInParam sIn;
			LabelCheck::s_AlgInspInPara sParam;
			sParam.bDsp = m_iImageShowMode;
			sParam.bRunCheck = TRUE;
			sParam.iCircleIndex = m_iHardImageCount[iCameraSN];
			sParam.iCount = m_iPicCountPerCam[iCameraSN];
			sParam.nCamIndex = iCameraSN;
			sParam.nChannel = m_sCameraImageInfo[iCameraSN].m_iCameraImagePixelSize;
			sParam.nDisplayMode = m_iImageShowMode;
			sParam.nInSig = 0;
			sParam.nSaveImageMode = m_bSaveCameraImage[iAllElementSN] ? m_iSaveImageType : -1;
			sParam.nHeight = m_sCameraImageInfo[iCameraSN].m_iCameraImageHeight;
			sParam.nWidth = m_sCameraImageInfo[iCameraSN].m_iCameraImageWidth;
			sParam.pcImageData = (char*)(pImageBuffer);

			sParam.bDisplaySys = 0==m_iShowDevice?true:false;//*[7/5/2017 ConanSteve]*:  0��ʾϵͳͼ��1��ʾ�㷨ͼ��
			sParam.bDetectAlg = m_bOpenCameraAlgCheck[iAllElementSN];
			sParam.nDispFirstorLast = 2;
#ifdef SPOT
			sParam.iRunMode = 0;
#else
			sParam.iRunMode = 1;
#endif // SPOT


			ZeroMemory(sParam.chSaveImagePath, sizeof(sParam.chSaveImagePath));
			memcpy(sParam.chSaveImagePath, m_sSaveImagePath[iCameraSN].GetBuffer(0), m_sSaveImagePath[iCameraSN].GetLength());

			sIn.iCheckSN = iAllElementSN;
			sIn.iImageSN = m_iHardImageCount[iAllElementSN];
			sIn.pImageData = pImageBuffer;
			sIn.pParam = &sParam;

			sIn.iReserve1 = e_SysDetectMode_Single;

			//*[7/7/2017 ConanSteve]*:  ��չ����

			m_arr_ExtenPara[iAllElementSN].push_back(COptionalFunc(e_Fn_CheckInParam_SmallImageNumOfEachGrab).SetInt(m_iPicCountPerCam[iAllElementSN]));
			m_arr_ExtenPara[iAllElementSN].push_back(COptionalFunc(e_Fn_CheckInParam_ImageWidth).SetInt(m_sCameraImageInfo[iAllElementSN].m_iCameraImageWidth));
			m_arr_ExtenPara[iAllElementSN].push_back(COptionalFunc(e_Fn_CheckInParam_ImageHeight).SetInt(m_sCameraImageInfo[iAllElementSN].m_iCameraImageHeight));
			m_arr_ExtenPara[iAllElementSN].push_back(COptionalFunc(e_Fn_CheckInParam_ImageChannel).SetInt(m_sCameraImageInfo[iAllElementSN].m_iCameraImagePixelSize));
			m_arr_ExtenPara[iAllElementSN].push_back(COptionalFunc(e_Fn_CheckInParam_WhetherCicleSaveImage).SetBool(1 == m_iIsCircle ? true : false));
			m_arr_ExtenPara[iAllElementSN].push_back(COptionalFunc(e_Fn_CheckInParam_SaveImagePath).SetPointer(m_sSaveImagePath[iAllElementSN].GetBuffer(0)));
			sIn.iReserve4 = (int)&m_arr_ExtenPara[iAllElementSN];

			if (TRUE == m_bOpenAlgCheck)
			{
				m_InspectionObj.Detect(sIn);
			}
		}
	}
	catch (...)
	{
		TRACE(TEXT("----------------�ɼ��ص�:���%d���������ɼ�����!---------------\n"), iCameraSN + 1);
		CString  strExceptionDes;
		strExceptionDes.Format(TEXT("CameraGrabFuncSingle-���%d���������ɼ�����=%d=%d=%d=!"), iCameraSN + 1,
			m_iHardImageCount[iAllElementSN], iGrabCircle, iPicSN);
		m_LogManager.MyWriteLogFile(TEXT("�ɼ��ص�"), strExceptionDes, AbnormityLog);
	}
	return true;
}

bool CMainFrame::AlarmStopMachine(LPCTSTR sErrorInfo)
{
	CString *pHintMsg = new CString();
	*pHintMsg = sErrorInfo;
	SendMessage(WM_COMMAND, ID_MSG_ALARM, (LPARAM)pHintMsg);
	return false;
}


int CMainFrame::StopMachine()
{
	m_IOCard[e_IOCard_2].setCardOutput(6, 1);
	Sleep(20);
	m_IOCard[e_IOCard_2].setCardOutput(6, 0);
	return 0;
}
