// MainFrm.cpp : implementation of the CMainFrame class
//
#include "stdafx.h"
#include "BottleCap.h"
#include "MainFrm.h"
#include "BottleCapDoc.h"
#include "BottleCapView.h"
#include "DlgLogin.h"
#include <io.h>
#include <MATH.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainFrame*		g_pFrame; //ȫ��ָ��
extern CBottleCapDoc*		g_pDoc;       
extern CBottleCapView*	g_pView;


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

	int i=0,j=0,k=0;

	for(i=0;i<ALLELEMENT_COUNT;i++)
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
		m_iSaveImageType[i]=0;					
		m_iIsCircle[i]=0;							

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

		m_eSaveImageType[i]=e_SaveImgObj_Alg;

		for(j=0;j<256;j++)
		{
			m_iCheckResultTable[i][j]=-1;
			m_bBottleCapResult[i][j] = FALSE;
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

		m_pImageAddr[i]=NULL;				// ���ͼ������

		ZeroMemory(m_chSavePath[i],512);

	}

	for(i=0;i<CAMERA_COUNT;i++)
	{
		m_iCamSN[i]=0;
		m_iPicCount[i]=0;

		m_iOriginSaveImageNum[i]=0;
		m_iListImagePathCount[i]=0;	
		m_bIsSimulateIniExist[i]=FALSE;				// ģ���ͼ�������ж�ģ��ɼ�ʱ�����ļ��Ƿ����
		m_bGrabFlip[i]=FALSE;						// �Ƿ���з�ת

		m_pAlgElement[i]=NULL;						// ר��Ϊ�㷨�Ĳ������ú��޸�ģ���������ڴ�
		m_pAlgErrorElement[i]=NULL;					// ר��Ϊ�㷨���һ�Ŵ���ͼ������ڴ�
		m_pbRGBImage[i]=NULL;						// ��ͼ�����任ͼ�񻺳���

		for(j=0;j<256;j++)
		{
			for(k=0;k<PIC_COUNT;k++)
			{
				m_bWriteImage[i][j][k]=FALSE;
			}
		}
	}

	for(i=0;i<PE_COUNT;i++)
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
	m_bUpdateCheckData=FALSE;

	m_dwCheckBadRatio=0.0;										// ��Ʒ��

	m_bFirstCheck=TRUE;
	m_iKickOffset=0;
	m_iKickMode=3;
	m_iCamTestMode=0;
	m_iEmptyCount=0;


}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_cfgAlgCfg.Init(CExFn::GetCurrentAppPath() + _T("\\Config\\AlgConfig.ini"));
	m_eIOCardType = (e_IOCardType)m_cfgAlgCfg.GetIntEx(_T("SystemConfiguration"), _T("IOCardType"));
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
	m_wndStatusBar.SetPaneInfo(0, ID_INDICATOR_GRAB, SBPS_NORMAL, 1500);  //�軪��������
	m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_CHECK, SBPS_NORMAL, 150);
	m_wndStatusBar.SetPaneInfo(2, ID_INDICATOR_ERROR, SBPS_NORMAL, 200);
	//m_wndStatusBar.SetPaneInfo(3, ID_INDICATOR_OPERATION, SBPS_NORMAL, 200);


	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable

	// ���ر������Ͳ˵���
	ModifyStyle(WS_CAPTION,0);
	SetMenu(NULL);

	////////////////////////////////////////////////////////////////////////
	//���"��¼���ļ�·�����ļ�"���ļ�·��
	m_sWorkPath = GetCurrentAppPath();						// ����·��
	m_sWorkPath.Delete(m_sWorkPath.GetLength()-1, 1);		// ɾ������·�������Ǹ�'\'

	CDlgLogin dlgLogin;
	if(IDOK==dlgLogin.DoModal())
	{
		m_iUserType=dlgLogin.m_iUserType;
		m_sUserName=dlgLogin.m_strUser;
		m_sPassword=dlgLogin.m_strPassword;
	}
	else
	{
		exit(0);
	}

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
	SetTimer(StartGrabTimer,100,NULL);			
	SetTimer(IntervalDisplayTimerOne,m_iIntervalDisplayTime,NULL);    //���1�����ʾʱ��
	PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,SystemStarted);

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

	
	QuitCheckProgram();	
			
	CMainFrame::OnClose();
}

void CMainFrame::InitInspectionDll()
{
	CString strPath;
	strPath.Format("%s\\Config\\AlgConfig.ini",m_sWorkPath);

	s_CheckInitParam sInit;
	s_DetectInitParam pParam[ALLELEMENT_COUNT];
 	ZeroMemory(&pParam[0],sizeof(s_DetectInitParam));
 	ZeroMemory(&pParam[1],sizeof(s_DetectInitParam));
	ZeroMemory(&pParam[2],sizeof(s_DetectInitParam));

	HWND hWnd[ALLELEMENT_COUNT];
	hWnd[0]=g_pView->GetDlgItem(IDC_ALG1)->GetSafeHwnd();
	hWnd[1]=g_pView->GetDlgItem(IDC_ALG2)->GetSafeHwnd();
	hWnd[2]=g_pView->GetDlgItem(IDC_ALG3)->GetSafeHwnd();

	pParam[0].pHWnd=(int*)(&hWnd[0]);
	pParam[0].iAlgSN=0;
	pParam[0].iRunMode=0;
	ZeroMemory(pParam[0].chLanguage,MAX_LENGTH);
	ZeroMemory(pParam[0].chLanPath,MAX_LENGTH);

	pParam[1].pHWnd=(int*)(&hWnd[1]);
	pParam[1].iAlgSN=0;
	pParam[1].iRunMode=0;
	ZeroMemory(pParam[1].chLanguage,MAX_LENGTH);
	ZeroMemory(pParam[1].chLanPath,MAX_LENGTH);

	pParam[2].pHWnd=(int*)(&hWnd[2]);
	pParam[2].iAlgSN=0;
	pParam[2].iRunMode=0;
	ZeroMemory(pParam[2].chLanguage,MAX_LENGTH);
	ZeroMemory(pParam[2].chLanPath,MAX_LENGTH);

	int pAddr[ALLELEMENT_COUNT];
	int i=0;
	for(;i<ALLELEMENT_COUNT;i++)
	{
		pAddr[i]=(int)(&pParam[i]);
	}

	sInit.KickCallbackFunc=GlobalKickCallbackFunc;
	sInit.StatusCallbackFunc=GlobalStatusCallbackFunc;

#ifdef __SGGrabber_
	sInit.iCallType=2;
#else
	sInit.iCallType=0;
#endif

	sInit.pFilePath = strPath.GetBuffer(0);
	sInit.pThis=this;
	sInit.pParam = (void*)&pAddr;
	sInit.iReserve1=1;		//0��ʾ���� 1��ʾҩҺ

	s_Status sStutus;
	ZeroMemory(&sStutus,sizeof(sStutus));
	sStutus = m_InspectionObj.Init(sInit);
	if (STATUS_NOR != sStutus.iStatusType)
		exit(-1);

	// ��Ҫ������
	s_Status sRet;
	s_AlgModelInput sModelInfo;
	ZeroMemory(&sModelInfo,sizeof(s_AlgModelInput));
	ZeroMemory(&sRet,sizeof(s_Status));

	s_DetectModelInput sParam;
	ZeroMemory(&sParam,sizeof(s_DetectModelInput));

	CString strModelPath;
	strModelPath.Format("%s\\ModelInfo\\%s",m_sWorkPath,m_sModelName);
	memcpy(sParam.chPath,strModelPath.GetBuffer(0),strModelPath.GetLength());

	CString strLogPath;
	strLogPath.Format("%s\\MendParamLog",m_sLogFilePath);
	memcpy(sParam.chLog,strLogPath.GetBuffer(0),strLogPath.GetLength());

	ZeroMemory(sParam.chImageBufferPath,MAX_LENGTH);

	int* pModelParam[ALLELEMENT_COUNT];


	for(i=0;i<ALLELEMENT_COUNT;i++)
	{
		CString strAlgImages;

		strAlgImages.Format("%s:\\SaveImages\\AlgImages\\Camera%d",m_sVirtualDisk,i+1);
		memcpy(sParam.chAbnormalPath,strAlgImages.GetBuffer(0),strAlgImages.GetLength());
		sModelInfo.iCheckSN=i;
		sModelInfo.iOperSig=0;

		pModelParam[i] = (int*)&sParam;
		sModelInfo.pParam=&pModelParam[i];
		m_InspectionObj.Set(sModelInfo);
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
	if(!inIsPathExist(strVirtualDiskPath))
	{
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
		str1.Format("%s:\\SaveImages\\GrabImages\\%04d��%02d��%02d��\\Camera1",m_sVirtualDisk,CurrTime.GetYear(),
			CurrTime.GetMonth(),CurrTime.GetDay());
		WritePrivateProfileString("DEMO Controls","ImgAlgPath_1",str1,strVirtualDiskPath);

		// д�뱣��ѭ����ͼʱ�Ĳ���
		str1.Format("%d",1);
		WritePrivateProfileString("DEMO Controls","EnableSequentialCheck",str1,strVirtualDiskPath);

		str1.Format("%s","Product");
		WritePrivateProfileString("DEMO Controls","SequentialCheckFolderFilter ",str1,strVirtualDiskPath);

	}
	else
	{
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
		str1.Format("%s:\\SaveImages\\GrabImages\\%04d��%02d��%02d��\\Camera1",m_sVirtualDisk,CurrTime.GetYear(),
			CurrTime.GetMonth(),CurrTime.GetDay());
		WritePrivateProfileString("DEMO Controls","ImgAlgPath_1",str1,strVirtualDiskPath);	

		// д�뱣��ѭ����ͼʱ�Ĳ���
		str1.Format("%d",1);
		WritePrivateProfileString("DEMO Controls","EnableSequentialCheck",str1,strVirtualDiskPath);
		
		str1.Format("%s","Product");
		WritePrivateProfileString("DEMO Controls","SequentialCheckFolderFilter ",str1,strVirtualDiskPath);
		
	}


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

	for(i=0;i<PE_COUNT;i++)
	{
		// ��¼�����Ϣ
		strName.Format("%s\\Guangdian%d.txt",strBasic,i+1);
		m_filePE[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
	}

	for(i=0;i<ALLELEMENT_COUNT;i++)
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
		m_fileKick[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		
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
	strDate.Format("%04d��%02d��%02d��",CurrTime.GetYear(),CurrTime.GetMonth(),CurrTime.GetDay());
	
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
	strDes.Format("%s\\DataFiles\\%04d��%02d��%02d��",m_sWorkPath,CurrTime.GetYear(),CurrTime.GetMonth(),CurrTime.GetDay());
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
	
	//��ȡ���ڱ���  //[2012-1-18 by zhy]
	GetPrivateProfileString("ϵͳ����", "���ڱ���", "���ͼ���ǩ�������ϵͳ���V1.0", m_sWindowTitle.GetBuffer(255), 255, strLoadFile);
	
	//��ȡ��һ��ģ������
	GetPrivateProfileString("ϵͳ����","��һ��ʹ���㷨ģ������","",strTemp.GetBuffer(255),255,strLoadFile);     //  [6/17/2010 ZXF]

	// ������������ļ�[2017-3-24]
	LoadCamConfig(strTemp);

	m_sModelName.Format("%s",strTemp);
	m_sModelPath.Format("%s\\ModelInfo\\%s",m_sWorkPath,strTemp);    

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

	if (m_iIntervalDisplayTime < 0)
	{

		strTemp.Format("��ȡ�����ļ�-ϵͳ����-ͼ����ʾ���ʱ��-%d����!",m_iIntervalDisplayTime);
		MessageBox(strTemp);
		return FALSE;
	}
	
	CString  strSession;
	int i=0,j=0;
	for (i=0;i<ALLELEMENT_COUNT;i++)
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

#ifdef __SGGrabber_
		m_BayerTransform[i].m_bAreaCamBayer = FALSE;
#endif

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
			m_eShowModeFlip[i] = FlipVertical;   //��ֱ��ת
		}
		else if (iFlip == 2)
		{
			m_eShowModeFlip[i] = FlipHorizintal; //ˮƽ��ת
		}
	}

	// ��ʼ��д��ȱ����Ϣ
	// ���λ����Ϣ
	CString strInfo;
	strInfo.Format("%d",4);
	WritePrivateProfileString("��λ1ȱ����Ϣ","ȱ�ݸ���",strInfo,strLoadFile);

	strInfo.Format("%s","����(���)ȱ��");
	WritePrivateProfileString("��λ1ȱ����Ϣ","����1",strInfo,strLoadFile);

	strInfo.Format("%s","����ȱ��");
	WritePrivateProfileString("��λ1ȱ����Ϣ","����2",strInfo,strLoadFile);

	strInfo.Format("%s","�޲�Ʒ");
	WritePrivateProfileString("��λ1ȱ����Ϣ","����3",strInfo,strLoadFile);

	strInfo.Format("%s", "����ȱ��");
	WritePrivateProfileString("��λ1ȱ����Ϣ", "����4", strInfo, strLoadFile);

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
	for(i=0;i<ALLELEMENT_COUNT;i++)
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
	m_sDemoIniPath.Format("%s\\set.ini", m_sWorkPath);	
	// Demo����
	GetPrivateProfileString("ϵͳ����","DemoName","",m_sDemoName.GetBuffer(255),255,m_sDemoIniPath);
	WritePrivateProfileString("ϵͳ����","AppName",m_sWindowTitle,m_sDemoIniPath);


	// �����ź���ʹ�õĽӿڿ���ź�����˿ں�
	m_iAlarmCardNum = GetPrivateProfileInt("�ӿڿ�����", "�����ź���ʹ�õĽӿڿ����", 1, strLoadFile);
	if(m_iAlarmCardNum<1 || m_iAlarmCardNum>IOCARD_COUNT)
	{
		AfxMessageBox("�����ź���ʹ�ýӿڿ����ô�����鿴�����ļ�!");
		return FALSE;
	}

	m_iAlarmOutPort = GetPrivateProfileInt("�ӿڿ�����", "�����ź���ʹ�õ�����˿ں�", 3, strLoadFile);
	if(m_iAlarmOutPort<0 || m_iAlarmOutPort>7)
	{
		AfxMessageBox("�����ź���ʹ�ýӿڿ�����˿����ô�����鿴�����ļ�!");
		return FALSE;
	}

	// ���ʹ�õ�������
	for(i=0;i<CAMERA_COUNT;i++)
	{
		strInfo.Format("���%d��ʹ�õĽӿڿ����",i+1);
		m_iCamCardNum[i]=GetPrivateProfileInt("�ӿڿ�����", strInfo, 1, strLoadFile);

		strInfo.Format("���%d��ʹ�õ�����˿ں�",i+1);

		if(Camera1==i)
		{
			m_iCamOutPort[i]=GetPrivateProfileInt("�ӿڿ�����", strInfo, 1, strLoadFile);
		}
		else if(Camera2==i)
		{
			m_iCamOutPort[i]=GetPrivateProfileInt("�ӿڿ�����", strInfo, 2, strLoadFile);
		}
		else if(Camera3==i)
		{
			m_iCamOutPort[i]=GetPrivateProfileInt("�ӿڿ�����", strInfo, 3, strLoadFile);
		}
	}


	// ��ȡ�ڶ�·������X��ʱ֡����
	CString strPIO24B;
	strPIO24B.Format("%s\\PIO24B_reg_init.txt",m_sWorkPath);
	m_iKickOffset = GetPrivateProfileInt("PIO24B", "89", 2, strPIO24B);

	CTime curTime = CTime::GetCurrentTime();

	for(i=0;i<ALLELEMENT_COUNT;i++)
	{
		m_sSaveImagePath[i].Format("%s:\\%s\\%s\\%04d��%02d��%02d��\\Camera%d",
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
	m_iPicCountPerCam[Camera1] = GetPrivateProfileInt("PIO24B", "44", 12, strPIO24B);
	m_iPicCountPerCam[Camera2] = GetPrivateProfileInt("PIO24B", "70", 12, strPIO24B);
	m_iPicCountPerCam[Camera3] = GetPrivateProfileInt("PIO24B", "68", 0, strPIO24B);

	m_iPicCountPerCam[Camera1]++;
	m_iPicCountPerCam[Camera2]++;
	m_iPicCountPerCam[Camera3]++;

	CTime CurrTime = CTime::GetCurrentTime();
	m_sStartTime.Format("%d-%d-%d %d:%d:%d",CurrTime.GetYear(),CurrTime.GetMonth(),CurrTime.GetDay(),
		CurrTime.GetHour(),CurrTime.GetMinute(),CurrTime.GetSecond());

	return TRUE;
}

// ����ϵͳ�����ļ�
BOOL CMainFrame::SaveSystemConfig(const CString &strLoadFile)
{
	CString strValue;
		
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

	strValue.Format("%d",m_iAlarmCardNum);
	WritePrivateProfileString("�ӿڿ�����","�����ź���ʹ�õĽӿڿ����",strValue, strLoadFile);
	
	strValue.Format("%d",m_iAlarmOutPort);
	WritePrivateProfileString("�ӿڿ�����","�����ź���ʹ�õ�����˿ں�",strValue, strLoadFile);

	CString  strSession;
	int i=0;

	for (;i<ALLELEMENT_COUNT;i++)
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
		WritePrivateProfileString(strSession, "�Ƿ����Bayer�任", strValue, strLoadFile);
		
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
	BOOL  bRet = FALSE;
	try{
		if(e_IOCardType_PIOC==m_eIOCardType)
		{
			for (int i = 0; i < IOCARD_COUNT; i++)
			{
				if (Camera1 == i)
				{
					bRet = m_IOCard[i].openCard(0);

					if (bRet == FALSE)
					{
						AfxMessageBox("Open IOCard Failed!");
						return FALSE;
					}

					m_IOCard[i].hardReset();
					m_IOCard[i].initRegisters("PIO24B_reg_init.txt", "PIO24B");
					m_IOCard[i].softReset();
				}
			}
		}
		else if (e_IOCardType_NewCard == m_eIOCardType)
		{
			if (TRUE == m_NewIOCard.IsOpen())
			{
				m_NewIOCard.Close();
				m_bReadIOCardParamFile = true;
			}
			bRet = m_NewIOCard.Open(0, m_hWnd);
			if (FALSE == bRet)
				return FALSE;

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
			if(e_IOCardType_PIOC == m_eIOCardType)
			{
				for (int i = 0; i < IOCARD_COUNT; i++)
				{
					m_IOCard[i].enable(FALSE);	      // �ر�ʹ�ܽӿڿ� [1/18/2012 dh]
					m_IOCard[i].closeCard();
				}
			}
			else if (e_IOCardType_NewCard == m_eIOCardType)
			{
				m_NewIOCard.PauseInCntWork(-1);

				for (int j = 0; j < 24; j++)
				{
					m_NewIOCard.SetOutIOStatus(j, FALSE);
				}
				m_NewIOCard.Close();
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
			TRACE("���%d�ɼ������ʼ���ɹ�!\n",i+1);
		}
		
		//��ȡ�ɼ���ͼ���ȡ��߶ȡ�λ��
		m_CameraGrabber[i].GetParamInt(GBImageWidth, m_sCameraImageInfo[i].m_iCameraImageWidth);			// �������ͼ����
		m_CameraGrabber[i].GetParamInt(GBImageHeight, m_sCameraImageInfo[i].m_iCameraImageHeight);			// �������ͼ��߶�
		m_CameraGrabber[i].GetParamInt(GBImageBufferSize, m_sCameraImageInfo[i].m_iCameraImageSize);		// �������ͼ���С
		m_CameraGrabber[i].GetParamInt(GBImagePixelSize, m_sCameraImageInfo[i].m_iCameraImagePixelSize);	// �������ͼ�����ش�С
#ifdef __RealGrabber_
		if(m_BayerTransform[i].m_bAreaCamBayer)
		{
			m_sCameraImageInfo[i].m_iCameraImagePixelSize = m_sCameraImageInfo[i].m_iCameraImagePixelSize*3;  // ��ɫ���
			m_sCameraImageInfo[i].m_iCameraImageSize = m_sCameraImageInfo[i].m_iCameraImageSize*3;
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

		ZeroMemory(&m_sCamInfo[i],sizeof(s_CheckCameraInfo));
		m_sCamInfo[i].iBitCount=m_sCameraImageInfo[i].m_iCameraImagePixelSize*8;
		m_sCamInfo[i].iCount=m_iPicCountPerCam[i];
		m_sCamInfo[i].iHeight= m_sCameraImageInfo[i].m_iCameraImageHeight;
		m_sCamInfo[i].iWidth= m_sCameraImageInfo[i].m_iCameraImageWidth;
		m_sCamInfo[i].iType=0;


		//�����ͼ���Сд��AlgCofig����
		CString strPath;
		strPath.Format("%s\\Config\\AlgConfig.ini",m_sWorkPath);

		int iWidth = m_sCamInfo[i].iWidth*m_iPicCountPerCam[i];
		int iHeight = m_sCamInfo[i].iHeight;

		CString strApp,strValue;
		strApp.Format("Camera%d Parameters",i+1);
		strValue.Format("%d //���ͼ����",iWidth);
		WritePrivateProfileString(strApp,"Width",strValue,strPath);
	
		strValue.Format("%d //���ͼ��߶�",iHeight);
		WritePrivateProfileString(strApp,"Height",strValue,strPath);

		//�������������ǩ��Ŀ��Ϊ1
		strValue.Format("%d //ÿ���㷨��ⷵ�ؽ������",1);
		WritePrivateProfileString(strApp,"CheckOutNum",strValue,strPath);
		
		//�����������ṹ���С
		strValue.Format("%d //�㷨���ʱ�ṹ��s_CheckInParam�� void* pParam�Ĵ�С",sizeof(s_DetectInParam));
		WritePrivateProfileString(strApp,"CheckInSize",strValue,strPath);
		
		//����������ṹ���С
		strValue.Format("%d //�㷨���ʱ�ṹ��s_CheckOutParam�� void* pParam�Ĵ�С",sizeof(s_DetectOutParam));
		WritePrivateProfileString(strApp,"CheckOutSize",strValue,strPath);
		
		// д�����������ļ���ʱ����
		strValue.Format("%d",(m_iKickOffset-1));
		strApp.Format("KickGroup%d",i+1);
		WritePrivateProfileString(strApp,"TimeOut",strValue,strPath);
	}	
	//////////////////////////////////////////////////////////////////////////
	return TRUE;
}

// �ͷŲɼ�����
inline BOOL CMainFrame::ReleaseGrabber()
{
	for (int i=0;i<CAMERA_COUNT;i++)
	{
		m_CameraGrabber[i].Close();
		TRACE("�ر����%d\r\n",i+1);
	}

	return TRUE;
}

// ��ʼ���ڴ�������
inline BOOL CMainFrame::InitDataFunc()
{	
	int i = 0;
	int j = 0;
	int k = 0;
	int iImageSize=0;

	for(i=0;i<ALLELEMENT_COUNT;i++)
	{
		for(j=0;j<DATA_ELEMENT_COUNT;j++)
		{
			m_sElement[i][j].bIsCheck=TRUE;
			m_sElement[i][j].bHaveError=FALSE;
			m_sElement[i][j].iIsUsing=0;
			m_sElement[i][j].iIsDetecting=0;
			m_sElement[i][j].iCameraSN=0;
			m_sElement[i][j].iCircleSN=0;
			m_sElement[i][j].iImageSN=0;
			m_sElement[i][j].strErrorInfo="";
		}
	}


	// ����洢Bayer�任�ڴ�
	for (i=0;i<CAMERA_COUNT;i++)
	{
		int iWidth=0,iHeight=0,iByte=0;
		iWidth = m_sCameraImageInfo[i].m_iCameraImageWidth;
		iHeight = m_sCameraImageInfo[i].m_iCameraImageHeight;
		iByte = m_sCameraImageInfo[i].m_iCameraImagePixelSize;
		m_pbRGBImage[i] = new BYTE[iWidth*iHeight*iByte];
	}

	
	// Ϊ�㷨ģ�����÷����ڴ�
	for(i=0;i<CAMERA_COUNT;i++)
	{	
		try
		{
			m_pAlgElement[i]=new BYTE[m_iMaxCameraImageWidth*m_iMaxCameraImageHeight*m_iMaxCameraImagePixelSize*m_iPicCountPerCam[i]];
			memset(m_pAlgElement[i],0,m_iMaxCameraImageWidth*m_iMaxCameraImageHeight*m_iMaxCameraImagePixelSize*m_iPicCountPerCam[i]);
			
			m_pAlgErrorElement[i]=new BYTE[m_iMaxCameraImageWidth*m_iMaxCameraImageHeight*m_iMaxCameraImagePixelSize*m_iPicCountPerCam[i]];
			memset(m_pAlgErrorElement[i],0,m_iMaxCameraImageWidth*m_iMaxCameraImageHeight*m_iMaxCameraImagePixelSize*m_iPicCountPerCam[i]);	
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
		for(i=0; i<ALLELEMENT_COUNT; i++)
		{
			iImageSize = m_sCameraImageInfo[i].m_iCameraImageSize;
			int iTotal = iImageSize*m_iPicCountPerCam[i]*DATA_ELEMENT_COUNT;
			m_pImageAddr[i] = NULL;
			m_pImageAddr[i] = new BYTE[iImageSize*m_iPicCountPerCam[i]*DATA_ELEMENT_COUNT];
			ZeroMemory(m_pImageAddr[i],iImageSize*m_iPicCountPerCam[i]*DATA_ELEMENT_COUNT);
		}
	}
	catch (...)
	{
		CString  strExceptionDes;
		strExceptionDes.Format("InitDataFunc-����������Ϣ���пռ����!");
		m_LogManager.MyWriteLogFile("��ʼ��",strExceptionDes,AbnormityLog);
		return FALSE;
	}


	////////////////////////////////////////////////////////////////
	//[2012-5-2 by zhy]: add
	try
	{
		for(i=0; i<ALLELEMENT_COUNT; i++)
		{
			m_hImageInspectStart[i] = CreateEvent(NULL,FALSE,FALSE,NULL);
			if(ERROR_ALREADY_EXISTS == GetLastError())
			{
				CString strExceptionDes("");
				strExceptionDes.Format("%s","����Ѿ�����--CreateEvent!");
				AfxMessageBox(strExceptionDes);
				return FALSE;
			}
			else if(NULL == m_hImageInspectStart[i])
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

	return TRUE;
}

// �ͷ��ڴ�������
inline BOOL CMainFrame::ReleaseDataFunc()
{
	int i = 0;
	int j = 0;
	int k = 0;
	int  iAllInfoElementCount=0;
	CString strinfo;

	// �ͷ�Byter�任�ڴ�
	for (i=0;i<CAMERA_COUNT;i++)
	{
		if (m_pbRGBImage[i] != NULL)
		{
			delete []m_pbRGBImage[i];
			m_pbRGBImage[i]=NULL;
		}
	}

	TRACE("-----�ͷ� RGB------\r\n");


	// �ͷ�Ϊ�㷨�������ú��޸�ģ��ר�ſ������ڴ�
	for(i=0;i<CAMERA_COUNT;i++)
	{
		if(m_pAlgElement[i] != NULL)
		{
			delete m_pAlgElement[i];
			m_pAlgElement[i]=NULL;
		}

		if(m_pAlgErrorElement[i] != NULL)
		{
			delete m_pAlgErrorElement[i];
			m_pAlgErrorElement[i]=NULL;
		}	
	}

	TRACE("-----�ͷ� Alg------\r\n");
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//[2012-5-2 by zhy]
	for( i=0; i<ALLELEMENT_COUNT; i++)
	{
		CloseHandle(m_hImageInspectStart[i]);
	}

	TRACE("-----�ͷ� Handle------\r\n");

	for(i=0;i<ALLELEMENT_COUNT;i++)
	{
		if(i==Camera2)
		{
			int ii=0;
		}

		if(m_pImageAddr[i]!=NULL)
		{
			delete m_pImageAddr[i];
			m_pImageAddr[i]=NULL;
		}

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


// 	if (0 != iGrabSN)
// 		return;
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

		if(!CameraGrabFuncSingle(iGrabSN,iGrabSN))
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
		if(Camera1==iGrabSN)
		{
			if (!CameraGrabFuncSingle(
				iGrabSN,
				Camera1
				))
			{
				CString  strExceptionDes;
				strExceptionDes.Format("CameraGrabFuncSingle-���%d�ɼ�����FALSE!",SigInfo->nGrabberSN+1);
				m_LogManager.MyWriteLogFile("�ɼ��ص�",strExceptionDes,OperationLog);
				return;
			}
		}
		else if(Camera2==iGrabSN)
		{
			if (!CameraGrabFuncSingle(
				iGrabSN,
				Camera2
				))
			{
				CString  strExceptionDes;
				strExceptionDes.Format("CameraGrabFuncSingle-���%d�ɼ�����FALSE!",SigInfo->nGrabberSN+1);
				m_LogManager.MyWriteLogFile("�ɼ��ص�",strExceptionDes,OperationLog);
				return;
			}
		}
		else if(Camera3==iGrabSN)
		{
			if (!CameraGrabFuncSingle(
				iGrabSN,
				Camera3
				))
			{
				CString  strExceptionDes;
				strExceptionDes.Format("CameraGrabFuncSingle-���%d�ɼ�����FALSE!",SigInfo->nGrabberSN+1);
				m_LogManager.MyWriteLogFile("�ɼ��ص�",strExceptionDes,OperationLog);
				return;
			}
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
	return TRUE;
}

// ɱ�������߳�
inline BOOL CMainFrame::KillAllThread()
{	
	DWORD  wlc = 0;

	m_bWaitDemoMould=TRUE;
	m_bStopReadKickImageNo=TRUE;

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
			CBottleCapView* pView = (CBottleCapView*) g_pView;
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
	BITMAPFILEHEADER bmFH;
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
	CBottleCapView* pView = (CBottleCapView*)GetActiveView();

	KillTimer(ID_START_CHECK_TIMER);
	KillTimer(ID_UPDATE_DEFECT_INFO_TIMER);
	
	int i=0,j=0;
	for(i=0;i<ALLELEMENT_COUNT;i++)
	{
		m_iCheckImageCount[i]=0;
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

		for(j=0;j<MAX_DEFECT_TYPE_COUNT;j++)
		{
			m_iDefectCount[i][j]=0;
		}
	}

	for(i=0;i<PE_COUNT;i++)
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

	m_iCheckCountTotal=0;										// ���ݿ������õļ������
	m_iCheckErrorCountTotal=0;
	m_iCheckGoodCountTotal=0;
	m_iEmptyCount=0;
	m_iLastCheckSpeedCount=0;


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
	VARIANT RecordAffected;

	CString strSQL;

	if(strTable=="All_Check_Info")
	{
		strSQL.Format("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
			"Create table ",
			strTable,
			"( ID int IDENTITY (1, 1) NOT NULL PRIMARY KEY,",
			"User char(255),",
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
		strSQL.Format("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
			"Create table ",
			strTable,
			"( ID int IDENTITY (1, 1) NOT NULL PRIMARY KEY,",
			"User char(255),",
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
	info.strUser=m_sUserName;
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
	iDefectInfo.sUser=m_sUserName;
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

	for(i=0;i<ALLELEMENT_COUNT;i++)
	{
		for(j=0;j<MAX_DEFECT_TYPE_COUNT;j++)
		{
			iDefectInfo.iDefectCount[i*MAX_DEFECT_TYPE_COUNT+j]=0;
		}
	}


	InitTableData(strTemp);
	InsertDefectTableData(iDefectInfo,strTemp);
	SetTimer(ID_UPDATE_DEFECT_INFO_TIMER, 1000, NULL);
}

void CMainFrame::InitTableData(CString strTableName)
{
	int nTemp=0;
	BOOL bRet=FALSE;
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

	///////////////////////////////////////////////
	// [2017-6-14 by zhy]
	// ����ֶθ���
	int iField = 0;
	iField = m_RecordSet->GetFields()->Count;
	
	int i=0;
	CString strField;
	for(i=0;i<iField;i++)
	{
		// ��ȡ�ֶ�����
		strField=(LPCSTR)_bstr_t(m_RecordSet->GetFields()->GetItem((long)i)->GetName());
		
		// ��ѯ�����Ƿ���ڸ��ֶΣ����ڱ㲻����
		if(strField=="User")
		{
			bRet = TRUE;
			break;
		}
	}

	m_RecordSet->Close();

	AFX_MANAGE_STATE(AfxGetModuleState());        //��������һ��Ҫ��
	AfxGetModuleState()->m_dwVersion = 0x0601;
	
	CString strDatabase;
	strDatabase.Format(_T("%s\\MP.MDB"),m_sWorkPath);
	
	if(FALSE==bRet)
	{
		// ����һ���ֶ�User
		CDaoDatabase db;
		
		try
		{
			// �����ݿ�
			db.Open(strDatabase);
			
			// �����ݱ�
			CDaoTableDef td(&db);
			td.Open(strTableName);
			
			// ����ֶ�
			CDaoFieldInfo info;
			info.m_strName = "User";
			info.m_nType=dbText;
			info.m_lSize=255;
			info.m_lAttributes=dbVariableField;
			info.m_bAllowZeroLength=TRUE;
			info.m_bRequired = FALSE;
			td.CreateField(info);
		}
		catch (CException* e)
		{
			e->ReportError();
		}
		
		if(db.IsOpen())
			db.Close();
	}
	
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
		(m_RecordSet)->PutCollect("User",_variant_t(info.strUser)); //[2017-6-14 add]
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

				(m_RecordSet)->PutCollect("User",_variant_t(info.strUser)); //[2017-6-14 add]

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
		(m_RecordSet)->PutCollect("User",_variant_t(info.strUser)); //[2017-6-14 add]
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
				(m_RecordSet)->PutCollect("User",_variant_t(info.strUser)); //[2017-6-14 add]
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
	


	if (g_pView!=NULL)
	{
		g_pView->SetDlgItemText(IDC_BTN_START,"ֹͣ���");
		g_pView->m_BtnStart.SetIcon(IDI_ICON_STOP,BS_LEFT);
	}
	int i=0,j=0;

	for(i=0;i<ALLELEMENT_COUNT;i++)
	{
		m_iCheckImageCount[i]=0;
		m_bDisplayAlgImage[i]=TRUE;
		m_bDisplayImage[i] = TRUE;
#ifdef __SGGrabber_
		m_iCheckErrorCount[i]=0;
		m_iGrabCircleCount[i]=0;
#endif	
	}

	for(i=0;i<CAMERA_COUNT;i++)
	{
		m_iPicCount[i]=0;
	}

	PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,Checking);

	int iParam1=0,iParam2=0;
	int iNewParam1=0,iNewParam2=0;
	
	if (m_bIOCardValid && m_bIOCardInitSuccess) //�ӿڿ���Ч�ҳ�ʼ���ɹ�
	{
		try
		{
			if(e_IOCardType_PIOC == m_eIOCardType)
				m_IOCard[Camera1].enable(true); //[2012-2-1 by zhy]
			else if (e_IOCardType_NewCard == m_eIOCardType)
			{
				if (m_bReadIOCardParamFile)
				{
					CString sPathFileOfIOCardConfig{ _T("") };
					CExFn::GetPrivateProfileString(_T("SystemConfiguration"), _T("PathFileOfInit"), sPathFileOfIOCardConfig, CExFn::GetCurrentAppPath() + _T("\\Config\\AlgConfig.ini"));
					sPathFileOfIOCardConfig = CExFn::GetCurrentAppPath() + _T("\\") + sPathFileOfIOCardConfig;
					BOOL bRet = m_NewIOCard.ReadParameterFile(sPathFileOfIOCardConfig.GetBuffer());
					if (FALSE == bRet)
						return FALSE;
					m_bReadIOCardParamFile = false;
				}

				m_csIOCard.Lock();
				m_NewIOCard.ContinueInCntWork(-1);
				m_csIOCard.Unlock();
				
			}
			

			// ȫ���޷�
// 			if(TRUE==m_bIsKick[Camera1])
// 			{	
// 				iParam1 = m_IOCard[Camera1].readParam(20);  
// 				iNewParam1 = iParam1 & (65280+128+64+32+16+8+4);
// 				iNewParam1 = iNewParam1 | (2*0+1*0);
// 				
// 				m_IOCard[Camera1].writeParam(20,iNewParam1);		
// 				
// 				// 					m_IOCard[Camera1].writeParam(20,0);		// �μ�Register Reference �е�address = 0x428 ����ֵΪ00000000 00000000
// 				// 					m_IOCard2.writeParam(20,0);		// ����Ϊ 00000000 00000000			
// 			}
// 			// 1����
// 			else
// 			{
// 				iParam1 = m_IOCard[Camera1].readParam(20);  
// 				iNewParam1 = iParam1 & (65280+128+64+32+16+8+4);
// 				iNewParam1 = iNewParam1 | (2*1+1*0);
// 				
// 				m_IOCard[Camera1].writeParam(20,iNewParam1);					
// 			}
		}
		catch (...)
		{
			CString  strExceptionDes;
			strExceptionDes.Format("StartCheck-���ýӿڿ��쳣!");
			m_LogManager.MyWriteLogFile("�ӿڿ�����",strExceptionDes,AbnormityLog);
		}
	}
	
	SetTimer(CheckSpeedRealTime,m_iCheckSpeedInterval,NULL);// ����ٶ�ʵʱ��ʱ��������

	SetTimer(IO24BSTATUS,200,NULL);

	SetTimer(DisplayCount,100,NULL);

	SetTimer(SaveCount,60000,NULL);

	SetTimer(ShowDataInfo,300,NULL);


	if(TRUE==m_bSoftResetIOCard)
	{
		InitDataBase();
	}

	m_bIsDetecting=TRUE;

	return TRUE;
}

BOOL CMainFrame::StopCheck()
{
	int i=0;

	if (g_pView!=NULL)
	{
		g_pView->SetDlgItemText(IDC_BTN_START,"��ʼ���");
		g_pView->m_BtnStart.SetIcon(IDI_ICON_START,BS_LEFT);
	}

	m_bIsDetecting=FALSE;
	
	for(i=0;i<ALLELEMENT_COUNT;i++)
	{
		m_bDisplayImage[i] = FALSE;
		m_bDisplayAlgImage[i]=FALSE;
	}
	PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,StopChecked);

#ifdef USE_IO_CARD
	if(m_bIOCardValid && m_bIOCardInitSuccess)
	{
		try
		{
			//��ֹ�ӿڿ�
			if (e_IOCardType_PIOC == m_eIOCardType)
			{
				m_IOCard[Camera1].enable(false); //[2012-2-1 by zhy]
			}
			else if (e_IOCardType_NewCard == m_eIOCardType)
			{
				m_csIOCard.Lock();
				m_NewIOCard.PauseInCntWork(-1);
				m_csIOCard.Unlock();
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
	
	//  [12/3/2009 GZ]
	KillTimer(CheckSpeedRealTime);//ֹͣ����ֹͣ�������ٶ�
	KillTimer(IO24BSTATUS);
	KillTimer(DisplayCount);
	KillTimer(SaveCount);
	KillTimer(ShowDataInfo);

	UpdateCheckData();

	return TRUE;
}

// ��ȡ�����ź�
inline BOOL CMainFrame::ReadMachineSignal(int iGrabSn)
{
	static int iHardImageCount2=0;
	int nImageNum1 = 0;//**[10/29/2017 ConanSteve]**:  �ӿڿ����������źż�����Ҳ����ͼ��
	int nImageNum3 = 0;
	
	try
	{
		if(Camera1==iGrabSn)
		{
			if (e_IOCardType_PIOC == m_eIOCardType)
				nImageNum1 = m_IOCard[Camera1].readCounter(28) % 256; //��ȡͼ����� ͼ��ż���x����Ӧ��1·����Ĳ�ͼ�ص���,Out1
			else if (e_IOCardType_NewCard == m_eIOCardType)
				nImageNum1 = m_NewIOCard.ReadOutputCount(1);
		}
		else if(Camera2==iGrabSn)
		{
			if (e_IOCardType_PIOC == m_eIOCardType)
				nImageNum1 = m_IOCard[Camera1].readCounter(31) % 256; //��ȡͼ����� ͼ��ż���2����Ӧ��2·����Ĳ�ͼ�ص���Out2
			else if (e_IOCardType_NewCard == m_eIOCardType)
				nImageNum1 = m_NewIOCard.ReadOutputCount(2);
		}	 
		else if(Camera3==iGrabSn)
		{
			if(e_IOCardType_PIOC == m_eIOCardType)
				nImageNum1	= m_IOCard[Camera1].readCounter(29)%256; //��ȡͼ����� ͼ��ż���0����Ӧ��3·����Ĳ�ͼ�ص���Out3
			else if(e_IOCardType_NewCard == m_eIOCardType)
				nImageNum1 = m_NewIOCard.ReadOutputCount(3);
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
		strExceptionDes.Format("ReadMachineSignal-��ȡ�����ź��쳣!");
		m_LogManager.MyWriteLogFile("�ӿڿ�����",strExceptionDes,AbnormityLog);
		return FALSE;
	}

}

//���� �����������ӵĺ��� [4/30/2010 GZ]
BOOL  CMainFrame::CameraGrabFuncSingle(int iCameraSN,
									   int iAllElementSN)//��������Ĳɼ�����
{
	
	if (iCameraSN < 0)
	{
		CString  strExceptionDes;
		strExceptionDes.Format("CameraGrabFuncSingle-������%d����!",iCameraSN);
		m_LogManager.MyWriteLogFile("�ɼ��ص�",strExceptionDes,AbnormityLog);
		return FALSE;
	}
	
	BOOL bRet = FALSE;
	int iGrabCircle=0,iPicSN=0;

	bRet = JudgeGrabCircleNew(iCameraSN,iAllElementSN,iGrabCircle,iPicSN);

	try
	{
		if(bRet)
		{	
			CString str1;
			str1.Format("==Image: %06d=Grab: %06d=Pic:%d=%s==\r\n",
				m_iHardImageCount[iAllElementSN],iGrabCircle,iPicSN,GetCurTime());
			
			if(TRUE==m_bStartRecord)
			{
				m_iRC_ImageNum[iAllElementSN]++;
				if(m_iRC_ImageNum[iAllElementSN]>m_iRecord_Set)
				{
					m_iRC_ImageNum[iAllElementSN]=0;
					m_fileImageNum[iAllElementSN].SeekToBegin();
				}
				m_fileImageNum[iAllElementSN].Write(str1.GetBuffer(0),str1.GetLength());
			}
			
			// ����ϢԪ��ûʹ����ʼʹ��
			m_sElement[iAllElementSN][iGrabCircle%DATA_ELEMENT_COUNT].bIsCheck = m_bIsCheck[iAllElementSN];
			m_sElement[iAllElementSN][iGrabCircle%DATA_ELEMENT_COUNT].bHaveError=FALSE;
			m_sElement[iAllElementSN][iGrabCircle%DATA_ELEMENT_COUNT].iCameraSN=iAllElementSN;
			m_sElement[iAllElementSN][iGrabCircle%DATA_ELEMENT_COUNT].iCircleSN = iGrabCircle;
#ifdef __SGGrabber_
			m_sElement[iAllElementSN][iGrabCircle%DATA_ELEMENT_COUNT].iImageSN = iGrabCircle;	
#else
			m_sElement[iAllElementSN][iGrabCircle%DATA_ELEMENT_COUNT].iImageSN = m_iHardImageCount[iAllElementSN];
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

			if(iPicSN>=m_iPicCountPerCam[iAllElementSN])
			{
				return FALSE;
			}

			m_sElement[iAllElementSN][iGrabCircle%DATA_ELEMENT_COUNT].iIsUsing=1;

			if(1==m_iCamTestMode)
			{
				if (pImageBuffer != NULL)
				{	
					if(m_BayerTransform[iCameraSN].m_bAreaCamBayer)
					{
						ConvertBayer2Rgb(m_pbRGBImage[iCameraSN],
							pImageBuffer,
							iImageWidth,
							iImageHeight,
							BAYER2RGB_NEIGHBOUR,
							m_pLutR,
							m_pLutG,
							m_pLutB,
							FALSE,
							BAYER_RG);

						int iOffset = iImageSize*m_iPicCountPerCam[iAllElementSN]*(iGrabCircle%DATA_ELEMENT_COUNT);
						
						// ���ڴ�дͼ������
						memcpy(m_pImageAddr[iAllElementSN]+iOffset,
							m_pbRGBImage[iCameraSN],
							iImageSize);
					}
					else
					{
						int iOffset = iImageSize*m_iPicCountPerCam[iAllElementSN]*(iGrabCircle%DATA_ELEMENT_COUNT);
						memcpy(m_pImageAddr[iAllElementSN]+iOffset, pImageBuffer,
							iImageSize);
					}								
					
					// �Բɼ���ͼ�����ˮƽ��ֱ����
					if (m_bGrabFlip[iCameraSN])
					{
						int iOffset = iImageSize*m_iPicCountPerCam[iAllElementSN]*(iGrabCircle%DATA_ELEMENT_COUNT);
						MirrorSingleImage(m_pImageAddr[iAllElementSN]+iOffset,
							iImageWidth,iImageHeight,iImageByte*8,m_eShowModeFlip[iCameraSN]);
					}
				}

				ThreadDisplayImage(iAllElementSN,iGrabCircle%DATA_ELEMENT_COUNT);
			}
			else
			{
				if (pImageBuffer != NULL)
				{	
					if(m_BayerTransform[iCameraSN].m_bAreaCamBayer)
					{
						ConvertBayer2Rgb(m_pbRGBImage[iCameraSN],
							pImageBuffer,
							iImageWidth,
							iImageHeight,
							BAYER2RGB_NEIGHBOUR,
							m_pLutR,
							m_pLutG,
							m_pLutB,
							FALSE,
							BAYER_RG);
						
						int iOffset = iImageSize*m_iPicCountPerCam[iAllElementSN]*(iGrabCircle%DATA_ELEMENT_COUNT);

						// ���ڴ�дͼ������
						memcpy(m_pImageAddr[iAllElementSN]+iOffset+iPicSN*iImageSize,
							m_pbRGBImage[iCameraSN],
							iImageSize);
						
					}
					else
					{
						int iOffset = iImageSize*m_iPicCountPerCam[iAllElementSN]*(iGrabCircle%DATA_ELEMENT_COUNT);

						if(TRUE==m_bStartRecord)
						{
							m_iRC_WriteImage[iAllElementSN]++;
							if(m_iRC_WriteImage[iAllElementSN]>m_iRecord_Set)
							{
								m_iRC_WriteImage[iAllElementSN]=0;
								m_fileWriteImage[iAllElementSN].SeekToBegin();
							}
							
							str1.Format("==Image:%d==Grab:%d==pic:%d=%s==%d=%d=%d=\r\n",m_iHardImageCount[iAllElementSN],
								iGrabCircle,iPicSN,GetCurTime(),m_pImageAddr[iAllElementSN],iOffset,iImageSize);
							m_fileWriteImage[iAllElementSN].Write(str1.GetBuffer(0),str1.GetLength());
						}

						memcpy(m_pImageAddr[iAllElementSN]+iOffset+iPicSN*iImageSize, pImageBuffer,
							iImageSize);
					}	
					
					
					// �Բɼ���ͼ�����ˮƽ��ֱ����
					if (m_bGrabFlip[iCameraSN])
					{
						int iOffset = iImageSize*m_iPicCountPerCam[iAllElementSN]*(iGrabCircle%DATA_ELEMENT_COUNT);

						MirrorSingleImage(m_pImageAddr[iAllElementSN]+iOffset+iPicSN*iImageSize,
							iImageWidth,iImageHeight,iImageByte*8,m_eShowModeFlip[iCameraSN]);
					}

				
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
						
						CString strSavePath;
						CTime curTime = CTime::GetCurrentTime();
						

						s_CheckInParam sIn; 
						ZeroMemory(&sIn,sizeof(s_CheckInParam));


						memcpy(m_chSavePath[iAllElementSN],m_sSaveImagePath[iAllElementSN].GetBuffer(0),m_sSaveImagePath[iAllElementSN].GetLength());
						int iOffset = iImageSize*m_iPicCountPerCam[iAllElementSN]*(iGrabCircle%DATA_ELEMENT_COUNT);

						s_DetectInParam sParam;
						sParam.iCircleIndex=m_iHardImageCount[iAllElementSN];
						sParam.iDisplayMode=m_iImageShowMode;
						sParam.iRejectTest=0;
						sParam.iSaveType=m_iSaveImageType[iAllElementSN];
						sParam.iScreenIndex=0;
						sParam.bSnapScreen=false;
						
						sParam.cSavePath=m_sSaveImagePath[iAllElementSN].GetBuffer(0);
						sParam.pImageInfo=&m_sCamInfo[iAllElementSN];
						sParam.pImageData=(int*)m_pImageAddr[iAllElementSN]+iOffset;

						
						m_arrObjSaveImgPara[iAllElementSN].SetImgWidth(m_sCameraImageInfo[iAllElementSN].m_iCameraImageWidth);
						m_arrObjSaveImgPara[iAllElementSN].SetImgHeight(m_sCameraImageInfo[iAllElementSN].m_iCameraImageHeight);
						m_arrObjSaveImgPara[iAllElementSN].SetImgChannel(m_sCameraImageInfo[iAllElementSN].m_iCameraImagePixelSize);
						m_arrObjSaveImgPara[iAllElementSN].SetImgNum(m_iPicCountPerCam[iAllElementSN]);
						m_arrObjSaveImgPara[iAllElementSN].SetIsCirCleSaveImg( TRUE==m_iIsCircle[iAllElementSN]);
						m_arrObjSaveImgPara[iAllElementSN].SetSaveImgFolderPath(m_sSaveImagePath[iAllElementSN].GetBuffer(), m_sSaveImagePath[iAllElementSN].GetLength());
						m_arrExten[iAllElementSN].clear();
 						m_arrExten[iAllElementSN].push_back((COptionalFunc*)&m_arrObjSaveImgPara[iAllElementSN]);
						sIn.iReserve4 = (int)&m_arrExten[iAllElementSN];
						
						
						sIn.iCheckSN=iAllElementSN;
						sIn.iImageSN=m_iHardImageCount[iAllElementSN];
						sIn.pImageData=(char*)m_pImageAddr[iAllElementSN]+iOffset;


						sIn.pParam=&sParam;
		
						if(m_bOpenImageSave==TRUE)
						{
							CString strSave;
							strSave.Format("%s\\TempImage\\Camera%d",m_sSaveImagePath[iAllElementSN],iAllElementSN+1);
							
							if(!inIsPathExist(strSave))
							{
								if(!inCreateMultiLevelPath(strSave))
								{
									CString str;
									str.Format("����·��ʧ��:%s",strSave);
									AfxMessageBox(str);
									return FALSE;
								}
							}
							
							if(m_iHardImageCount[iAllElementSN]==5)
							{
								for(int i=0;i<m_iPicCountPerCam[iAllElementSN];i++)
								{
									strSavePath.Format("%s\\%d.bmp",strSave,i+1);
									savebmp(strSavePath,iImageWidth,iImageHeight,iImageByte,
										m_pImageAddr[iAllElementSN]+iOffset+i*iImageSize);
								}
							}
						}
						
						if(TRUE==m_bOpenAlgCheck)
						{
							m_InspectionObj.Detect(sIn);
						}
						m_iCheckImageCount[iAllElementSN]++;
						
						if(m_bDisplayImage[iAllElementSN])
						{
							ThreadDisplayImage(iAllElementSN,iGrabCircle%DATA_ELEMENT_COUNT);
						}
					}
				}
				else
				{
					return FALSE;
				}
			}

			m_sElement[iAllElementSN][iGrabCircle%DATA_ELEMENT_COUNT].iIsUsing=0;
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
	m_bBottleCapResult[iAllElementSN][m_iHardImageCount[iAllElementSN]%256] = FALSE;
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
	if (nIDEvent == StartGrabTimer)
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

// 		CString strStopTime;
// 		CTime CurrTime = CTime::GetCurrentTime();
// 		strStopTime.Format("%d-%d-%d %d:%d:%d",CurrTime.GetYear(),CurrTime.GetMonth(),CurrTime.GetDay(),
// 			CurrTime.GetHour(),CurrTime.GetMinute(),CurrTime.GetSecond());
// 		
// 		CString strCheckTotal,strBadTotal,strGoodTotal, strBadTotalRatio;
// 		CString strTotal[ALLELEMENT_COUNT],strGood[ALLELEMENT_COUNT];
// 		CString strBad[ALLELEMENT_COUNT],strBadRatio[ALLELEMENT_COUNT];
// 
// 		strCheckTotal.Format("%d",m_iCheckCountTotal);
// 		strBadTotal.Format("%d",m_iCheckErrorCountTotal);
// 		strGoodTotal.Format("%d",m_iCheckGoodCountTotal);
// 		
// 		int iGood=0;
// 		float fRatio=0.0;
// 		
// 		for(int i=0;i<ALLELEMENT_COUNT;i++)
// 		{
// 			strTotal[i].Format("%d",m_iFrameCount[i]);
// 			strBad[i].Format("%d",m_iHardKickCount[i]);
// 			
// 			iGood = m_iFrameCount[i]-m_iHardKickCount[i];
// 			if(iGood<=0)
// 			{
// 				strGood[i].Format("%d",0);
// 			}
// 			else
// 			{
// 				strGood[i].Format("%d",iGood);
// 			}
// 			
// 			if(m_iFrameCount[i]<=0 || m_iHardKickCount[i]<=0)
// 			{
// 				strBadRatio[i].Format("%0.3f%%",0.0);
// 			}
// 			else
// 			{
// 				fRatio = ((float)100*m_iHardKickCount[i])/m_iFrameCount[i];
// 				strBadRatio[i].Format("%0.3f%%",fRatio);
// 			}
// 		}
// 		
// 		if(m_iCheckErrorCountTotal<=0 || m_iCheckCountTotal<=0)
// 		{
// 			strBadTotalRatio.Format("%0.3f%%",0.0);
// 		}
// 		else
// 		{
// 			strBadTotalRatio.Format("%0.3f%%",((float)100*m_iCheckErrorCountTotal)/((float)m_iCheckCountTotal));
// 		}
// 		
// 		s_CheckResultInfo info;
// 		info.strStartTime=m_sStartTime;
// 		info.strStopTime=strStopTime;
// 		info.strModelName=m_sModelName;
// 		info.strTotalCount= strCheckTotal;
// 		info.strTotalGood = strGoodTotal;
// 		info.strTotalBad=strBadTotal;
// 		info.strTotalBadRatio=strBadTotalRatio;
// 		info.strTotal1 = strTotal[Camera1];
// 		info.strGood1 = strGood[Camera1];
// 		info.strBad1 = strBad[Camera1];
// 		info.strBadRatio1 = strBadRatio[Camera1];
// // 		info.strTotal2 = strTotal[Camera2];
// // 		info.strGood2 = strGood[Camera2];
// // 		info.strBad2 = strBad[Camera2];
// // 		info.strBadRatio2 = strBadRatio[Camera2];
// 
// 		
// 		UpdateTableData(info, "All_Check_Info");
	
		UpdateCheckData();
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
		info.sUser=m_sUserName;
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

		for(i=0;i<ALLELEMENT_COUNT;i++)
		{
			for(j=0;j<MAX_DEFECT_TYPE_COUNT;j++)
			{
				info.iDefectCount[i*MAX_DEFECT_TYPE_COUNT+j]=m_iDefectCount[i][j];
			}
		}
		UpdateDefectTableData(info,strTable);	
	}
	else if(nIDEvent == DisplayCount)
	{
		CString strInfo("");
		int iTemp1 = 0, iTemp2=0;
		double dRatio = 0;

		m_iCheckCountTotal = (m_iFrameCount[Camera1]-m_iEmptyCount);

// 		m_iCheckGoodCountTotal = (m_iFrameCount[Camera1]-m_iHardKickCount[Camera1]-m_iEmptyCount);
// 
// 		m_iCheckErrorCountTotal = m_iHardKickCount[Camera1]+m_iEmptyCount;

		m_iCheckGoodCountTotal = (m_iFrameCount[Camera1]-m_iHardKickCount[Camera1]);
		m_iCheckErrorCountTotal = m_iHardKickCount[Camera1]-m_iEmptyCount;

		if(m_iFrameCount[Camera1]>0 && m_iHardKickCount[Camera1]>0)
		{
			m_dwCheckBadRatio = 100*((double)m_iHardKickCount[Camera1]/m_iFrameCount[Camera1]);
		}
		else
		{
			m_dwCheckBadRatio = 100;
		}

		// �豸����
		strInfo.Format("%d",m_iCheckCountTotal);
		g_pView->m_TotalData.Display(strInfo,"",0,FALSE);
			
		//�ϸ�Ʒ��
		strInfo.Format("%d",m_iCheckGoodCountTotal);
		g_pView->m_TotalGoodData.Display(strInfo,"",0,FALSE);
		
		//�ϸ���
		float fGoodRate{ 0.0 };
		if (m_iCheckCountTotal != 0)
		{
			fGoodRate = (float)100.0*m_iCheckGoodCountTotal / m_iCheckCountTotal;
		}
		//strInfo.Format("%0.3f%%",(100-m_dwCheckBadRatio));
		strInfo.Format("%0.3f%%", fGoodRate);
		g_pView->m_TotalGoodRatioData.Display(strInfo,"",0,FALSE);
	
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
#ifdef __RealGrabber_
				iCount = (m_iFrameCount[Camera1]-m_iLastCheckSpeedCount)*60000/m_iCheckSpeedInterval;
				strTmp.Format("%d ��/����",iCount);// 5��һ�Σ�1����ͳ��

				g_pView->m_SpeedData.Display(strTmp,"",0,FALSE);
				m_iLastCheckSpeedCount = m_iFrameCount[Camera1];
#else
				int iCamgroup=0;

				if (m_bIsCheck[Camera1])
					iCamgroup=Camera1;

				iCount = (m_iCheckImageCount[iCamgroup]-m_iLastCheckSpeedCount)*60000/m_iCheckSpeedInterval;
				strTmp.Format("%d ��/����", iCount);
 				g_pView->m_SpeedData.Display(strTmp.GetBuffer(strTmp.GetLength()),"",0,FALSE);
				m_iLastCheckSpeedCount = m_iCheckImageCount[iCamgroup];		
#endif
			}		
		}		
	}
	else if(nIDEvent==ShowDataInfo)
	{
		CString strGrab;

		strGrab.Format("���-%d-�ɼ�:%d-%d-%d-�޷�:%d-�ܲɼ�:%d-%d-%d",
			m_iFrameCount[Camera1],
			m_iGrabCircleCount[Camera1],
			m_iGrabCircleCount[Camera2],
			m_iGrabCircleCount[Camera3],
			m_iHardKickCount[Camera1],
			m_iPicCount[Camera1],
			m_iPicCount[Camera2],
			m_iPicCount[Camera3]);

		m_wndStatusBar.SetPaneText(0, strGrab);

		strGrab.Format("���ʱ��:%.0fms-%.0fms-%.0fms",m_dAlgCheckTime[Camera1],m_dAlgCheckTime[Camera2],m_dAlgCheckTime[Camera3]);
		m_wndStatusBar.SetPaneText(1, strGrab);

		strGrab.Format("�������:%d-%d-%d",m_iCheckErrorCount[Camera1],m_iCheckErrorCount[Camera2],m_iCheckErrorCount[Camera3]);
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
	
	KillTimer(ID_START_CHECK_TIMER);
	KillTimer(ID_UPDATE_DEFECT_INFO_TIMER);
	KillTimer(ID_UPDATE_DEFECT_SHOW_INFO);
	
	SaveSystemConfig(m_sSysConfigFileName);
		
	try
	{
		for (i=0;i<CAMERA_COUNT;i++)
		{
			StopCameraGrab(i); //ֹͣ�ɼ�
		}
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
	
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	CString str;
	switch(wParam)
	{
	case ID_INDICATOR_GRAB:
		{
			CString  strGrab;
			str.Format("�ɼ�״̬:");
			switch(lParam)
			{
			case Grabbing:
				strGrab.Format("�ɼ�:%d",m_iGrabCircleCount[Camera1]);
				break;
			case IOCardInfo:
				{
					str.Format("���:%d",m_iFrameCount[Camera1]);
					strGrab.Format("-�ɼ�:%d-�޷�:%d-�ܲɼ�:%d",m_iGrabCircleCount[Camera1],
						m_iHardKickCount[Camera1],m_iPicCount[Camera1]);
					str += strGrab;
					break;
				}
			default:
				str.Format("�ɼ�״̬:");
				break;
			}			
		}
		m_wndStatusBar.SetPaneText(0, str);
		return TRUE;
	case ID_INDICATOR_CHECK:
		{
			str.Format("�����Ϣ:");
			switch(lParam)
			{
			case CheckResult:
				str.Format("�����Ϣ:%d-",m_iGrabCircleCount[Camera1]);
				break;
			case AlgCheckTime:
				str.Format("���ʱ��:%.0fms",m_dAlgCheckTime[Camera1]);
				break;
			default:
				str.Format("�����Ϣ:");
				break;
			}			
		}
		m_wndStatusBar.SetPaneText(1, str);
		return TRUE;
	case ID_INDICATOR_ERROR:
		{
			switch(lParam)
			{
			case CheckResult:
				str.Format("�������:%d-",m_iCheckErrorCount[Camera1]);
				break;
			default:
				str.Format("�������:");
				break;
			}			
		}
		m_wndStatusBar.SetPaneText(2, str);
		return TRUE;
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
				if(m_iIsCircle[Camera1])
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
				if(m_iIsCircle[Camera1])
				{
					str.Format("����ѭ���������ͼ��:%d-%d-%d",
						m_iSaveCircleCameraImageCount[Camera1],
						m_iSaveCircleCameraImageCount[Camera2],
						m_iSaveCircleCameraImageCount[Camera3]);
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
	}
	return CFrameWnd::OnCommand(wParam, lParam);
}

CString CMainFrame::GetCurTime()
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

	switch(pCopyDataStruct->dwData)
	{
	case 10:
		{
			pDemoWnd = CWnd::FindWindow(NULL, m_sDemoName);	
			break;
		}	
	case 30:
		{
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
			
			int i=0;
			for(;i<ALLELEMENT_COUNT;i++)
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
				
				s_DetectModelInput sParam;
				ZeroMemory(&sParam,sizeof(s_DetectModelInput));
				
				CString strModelPath;
				strModelPath.Format("%s\\ModelInfo\\%s",m_sWorkPath,strNew);
				memcpy(sParam.chPath,strModelPath.GetBuffer(0),strModelPath.GetLength());
				
				CString strLogPath;
				strLogPath.Format("%s\\MendParamLog",m_sLogFilePath);
				memcpy(sParam.chLog,strLogPath.GetBuffer(0),strLogPath.GetLength());
				
				ZeroMemory(sParam.chImageBufferPath,MAX_LENGTH);
				
				int* pModelParam[ALLELEMENT_COUNT];
				
				
				for(i=0;i<ALLELEMENT_COUNT;i++)
				{
					CString strAlgImages;
					
					strAlgImages.Format("%s:\\SaveImages\\AlgImages\\Camera%d",m_sVirtualDisk,i+1);
					memcpy(sParam.chAbnormalPath,strAlgImages.GetBuffer(0),strAlgImages.GetLength());
					
					
					sModelInfo.iCheckSN=i;
					sModelInfo.iOperSig=0;
					
					pModelParam[i] = (int*)&sParam;
					
					sModelInfo.pParam=&pModelParam[i];
					
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
		if(e_IOCardType_PIOC == m_eIOCardType)
			m_IOCard[m_iAlarmCardNum-1].setCardOutput(m_iAlarmOutPort,1);
	}
	else
	{
		if (e_IOCardType_PIOC == m_eIOCardType)
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
		(m_RecordSet)->PutCollect("User",_variant_t(info.sUser));//[2017-6-14]
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

		for(i=0;i<ALLELEMENT_COUNT;i++)
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
				(m_RecordSet)->PutCollect("User",_variant_t(info.sUser));
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
				
				for(i=0;i<ALLELEMENT_COUNT;i++)
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
		(m_RecordSet)->PutCollect("User",_variant_t(info.sUser));
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
		
		for(i=0;i<ALLELEMENT_COUNT;i++)
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
				(m_RecordSet)->PutCollect("User",_variant_t(info.sUser));
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
				
				for(i=0;i<ALLELEMENT_COUNT;i++)
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

	BOOL bKick[ALLELEMENT_COUNT];
	BOOL bHave[ALLELEMENT_COUNT];
	ZeroMemory(bKick,ALLELEMENT_COUNT*sizeof(BOOL));
	ZeroMemory(bHave,ALLELEMENT_COUNT*sizeof(BOOL));

	s_DetectOutParam sOut[ALLELEMENT_COUNT];
	ZeroMemory(sOut,ALLELEMENT_COUNT*sizeof(s_DetectOutParam));

	memcpy(sOut,sParam->pParam,ALLELEMENT_COUNT*sizeof(s_DetectOutParam));

	int iImageSN=0,iKickSN=0;
	iImageSN = sParam->iImageSN;
	iKickSN = sParam->iKickSN;

	if(iImageSN>m_iFrameCount[Camera1])
	{
		return;
	}

	int i=0;

	for(;i<ALLELEMENT_COUNT;i++)
	{
		m_dAlgCheckTime[i] = sOut[i].iTimeConsuming;

		if(TRUE==m_bSaveCameraImage[i])
		{
			// ѭ����ͼ
			if(1==m_iIsCircle[i])
			{
				if(0==m_iSaveImageType[i])		//����ͼ��
				{
					m_iSaveCircleCameraImageCount[i]++;
				}
				else if(1==m_iSaveImageType[i])	//����ͼ��
				{
					if(0!=sOut[i].iErrorType && 1!=sOut[i].iErrorType)
					{
						m_iSaveCircleCameraImageCount[i]++;
					}
				}
				
				if(SaveGrabImage==m_iSaveImageType[i])
				{
					PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,ContinueImageSaving);
				}
				else if(SaveErrorImage==m_iSaveImageType[i])
				{
					PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,ErrorImageSaving);
				}
			}
			else
			{
				if(m_iSaveCameraImageCount_Setting[i]>0 || m_bSaveCameraImage[i])
				{
					if(m_iSaveCameraImageCount[i]<m_iSaveCameraImageCount_Setting[i] )
					{
						if(0==m_iSaveImageType[i])		//����ͼ��
						{
							m_iSaveCameraImageCount[i]++;
						}
						else if(1==m_iSaveImageType[i])	//����ͼ��
						{
							if(0!=sOut[i].iErrorType && 1!=sOut[i].iErrorType)
							{
								m_iSaveCameraImageCount[i]++;
							}
						}
					}
				}
				
				if(m_iSaveCameraImageCount[Camera1]>=m_iSaveCameraImageCount_Setting[Camera1] &&
					m_iSaveCameraImageCount[Camera2]>=m_iSaveCameraImageCount_Setting[Camera2] &&
					m_iSaveCameraImageCount[Camera3]>=m_iSaveCameraImageCount_Setting[Camera3] )
				{
					m_iSaveCameraImageCount_Setting[Camera1]=0;
					m_iSaveCameraImageCount_Setting[Camera2]=0;
					m_iSaveCameraImageCount_Setting[Camera3]=0;
					
					m_bSaveCameraImage[Camera1]=FALSE;
					m_bSaveCameraImage[Camera2]=FALSE;
					m_bSaveCameraImage[Camera3]=FALSE;
					
					PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,StopSave);			
				}
				else
				{
					if(SaveGrabImage==m_iSaveImageType[i])
					{
						PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,ContinueImageSaving);
					}
					else if(SaveErrorImage==m_iSaveImageType[i])
					{
						PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,ErrorImageSaving);
					}
				}
			}
		}	
	}

	if(0==m_iKickMode)		//ȫ��
	{
		m_iCheckErrorCount[iKickSN]++;
		
		if(TRUE==m_bIsKick[iKickSN])
		{
			if(TRUE==m_bIOCardValid)
			{
				if (e_IOCardType_PIOC == m_eIOCardType)
					m_IOCard[Camera1].sendChckRslt(TRUE, iImageSN % 256, Camera1);
				else if (e_IOCardType_NewCard == m_eIOCardType)
					;//**[10/29/2017 ConanSteve]**:  nothing to do
			}
		}
	}
	else if(1==m_iKickMode)	//ȫ�����߷�
	{
//		m_iCheckErrorCount[iKickSN]++;
		
		if(TRUE==m_bIsKick[iKickSN])
		{
			if(TRUE==m_bIOCardValid)
			{
				if (e_IOCardType_PIOC == m_eIOCardType)
					m_IOCard[Camera1].sendChckRslt(FALSE, iImageSN % 256, Camera1);
				else if (e_IOCardType_NewCard == m_eIOCardType)
					m_NewIOCard.SetOutCtrlByResultEx(0, iImageSN, FALSE);
			}
		}
	}
	else if(2==m_iKickMode)	//������
	{
		if(iImageSN%2==0)
		{
			m_iCheckErrorCount[iKickSN]++;
			
			if(TRUE==m_bIsKick[iKickSN])
			{
				if(TRUE==m_bIOCardValid)
				{
					if (e_IOCardType_PIOC == m_eIOCardType)
						m_IOCard[Camera1].sendChckRslt(TRUE, iImageSN % 256, Camera1);
					else if (e_IOCardType_PIOC == m_eIOCardType)
						m_NewIOCard.SetOutCtrlByResultEx(0, iImageSN, TRUE);
				}
			}
		}
		else
		{
			if(TRUE==m_bIsKick[iKickSN])
			{
				if (e_IOCardType_PIOC == m_eIOCardType)
					m_IOCard[Camera1].sendChckRslt(FALSE, iImageSN % 256, Camera1);
				else if (e_IOCardType_PIOC == m_eIOCardType)
					m_NewIOCard.SetOutCtrlByResultEx(0, iImageSN, FALSE);
			}
		}
	}
	else if(3==m_iKickMode)		//�����޷�
	{
		if(sOut[0].iErrorType==1)
		{
			m_iEmptyCount++;
		}

		for(i=0;i<ALLELEMENT_COUNT;i++)
		{
//			if(sOut[i].iErrorType!=0 && sOut[i].iErrorType!=1)		// ���� ����ƿ
			if(sOut[i].iErrorType!=0)		//��ƿ����������������߷�[2017-7-20 zhy]
			{	
				m_iCheckErrorCount[i]++;
				
				/*************************************<>************************************[8/28/2017 ConanSteve]*/
				switch (sOut[i].iErrorType)
				{
				case 21://**[8/28/2017 ConanSteve]**:  ����ȱ��
					++m_iDefectCount[0][0];
					break;
				case 20://**[8/28/2017 ConanSteve]**:  ���ǣ���ڣ�ȱ��
					++m_iDefectCount[0][1];
					break;
				case 1://**[8/28/2017 ConanSteve]**:  �޲�Ʒ
					++m_iDefectCount[0][2];
					break;
				case 80://**[8/28/2017 ConanSteve]**:  ����ȱ��
					++m_iDefectCount[0][3];
					break;
				default:
					break;
				}
				/************************************************************************/
				bHave[i]=TRUE;

				if(TRUE==m_bIsKick[i])
				{
					bKick[i]=TRUE;
				}
				else
				{
					bKick[i]=FALSE;
				}

				if(TRUE==m_bStartRecord)
				{
					CString str1;
					
					str1.Format("KickSN:%d-%d=Cam:%d=Image:%d=ET:%d=Time:%s=\r\n",iKickSN,m_bIsKick[iKickSN],
						i,iImageSN,sOut[i].iErrorType,GetCurTime());
					
					m_iRC_Kick[i]++;
					if(m_iRC_Kick[i]>m_iRecord_Set)
					{
						m_iRC_Kick[i]=0;
						m_fileKick[i].SeekToBegin();
					}
					m_fileKick[i].Write(str1.GetBuffer(0),str1.GetLength());
				}
			}
			else
			{
				bKick[i]=FALSE;

				if(TRUE==m_bStartRecord)
				{
					CString str1;
					
					str1.Format("KickSN:%d-%d=Cam:%d=Image:%d=ET:%d=Time:%s=\r\n",iKickSN,m_bIsKick[iKickSN],i,iImageSN,sOut[i].iErrorType,GetCurTime());
					
					m_iRC_Good[i]++;
					if(m_iRC_Good[i]>m_iRecord_Set)
					{
						m_iRC_Good[i]=0;
						m_fileGood[i].SeekToBegin();
					}
					m_fileGood[i].Write(str1.GetBuffer(0),str1.GetLength());
				}
			}
		}

		if(TRUE==bKick[Camera1] || TRUE==bKick[Camera2] || TRUE==bKick[Camera3])
		{
			if(TRUE==m_bIOCardValid)
			{
				if (e_IOCardType_PIOC == m_eIOCardType)
					m_IOCard[Camera1].sendChckRslt(TRUE, iImageSN % 256, iKickSN);
				else if (e_IOCardType_PIOC == m_eIOCardType)
					m_NewIOCard.SetOutCtrlByResultEx(0, iImageSN, TRUE);
			}
		}
		else
		{
			if(TRUE==m_bIOCardValid)
			{
				if (e_IOCardType_PIOC == m_eIOCardType)
					m_IOCard[Camera1].sendChckRslt(FALSE, iImageSN % 256, iKickSN);
				else if (e_IOCardType_PIOC == m_eIOCardType)
					m_NewIOCard.SetOutCtrlByResultEx(0, iImageSN, FALSE);
			}
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
		
	int nPECount[PE_COUNT];
	memset(nPECount,0,sizeof(int)*PE_COUNT);
	
	double dd1[PE_COUNT]={0.0};
	double dDiff[PE_COUNT]={0.0};
	
	LONGLONG lQt1[PE_COUNT]={0};
	LONGLONG lQt2[ALLELEMENT_COUNT]={0};
	LONGLONG lLastQt2[ALLELEMENT_COUNT]={0};
	
	LARGE_INTEGER iLarge[PE_COUNT];
	
	CString strText,strInfo;
	
	//������
#ifdef USE_IO_CARD
	if (e_IOCardType_PIOC == m_eIOCardType)
	{
		nPECount[Camera1] = m_IOCard[Camera1].readCounter(0) % 256;
		nPECount[Camera2] = m_IOCard[Camera1].readCounter(1) % 256;
		nPECount[Camera3] = m_IOCard[Camera1].readCounter(0) % 256;
	}
	else if (e_IOCardType_NewCard == m_eIOCardType)
	{
		nPECount[Camera1] = nPECount[Camera2] = nPECount[Camera3] = m_NewIOCard.ReadInputIndex(0) % 256;
	}
	
#else
	nPECount[Camera1] = m_iGrabCircleCount[Camera1]%256;
	nPECount[Camera2] = m_iGrabCircleCount[Camera2]%256;
	nPECount[Camera3] = m_iGrabCircleCount[Camera3]%256;
#endif
	
	for(i=0;i<PE_COUNT;i++)
	{			
		//��n������������
		if(m_iLastFrameCount[i] > 200 && nPECount[i] <= 50)
		{
			m_iFrameCountOverFlowNumber[i] += 1;
		}
		
		m_iFrameCount[i] = nPECount[i] + 256*m_iFrameCountOverFlowNumber[i];
		
		if(nPECount[i] != m_iLastFrameCount[i])
		{	
			
			QueryPerformanceCounter(&iLarge[i]);
			lQt1[i]=iLarge[i].QuadPart;
			dd1[i]=lQt1[i]-m_lLastQt_PE[i];
			dDiff[i]=1000*dd1[i]/m_dFreq;
			m_lLastQt_PE[i]=lQt1[i];
			
			strInfo.Format("=ͼ���:%06d=%s=���:%0.0f=Last:%d=PE:%d\r\n",
				m_iFrameCount[i],GetCurTime(),dDiff[i],m_iLastFrameCount[i],nPECount[i]);
			
			if(TRUE==m_bStartRecord)
			{
				m_iRC_PE[i]++;
				if(m_iRC_PE[i]>m_iRecord_Set)
				{
					m_iRC_PE[i]=0;
					m_filePE[i].SeekToBegin();
				}
				m_filePE[i].Write(strInfo.GetBuffer(0),strInfo.GetLength());
			}
			m_iLastFrameCount[i] = nPECount[i];// ��¼���n��һ֡����
		}	
	}

	return TRUE;
}

BOOL CMainFrame::GetCount_Tifei()
{
	float iBadRatio=0.0;
	
	int i=0;		
	int nKickCount[ALLELEMENT_COUNT];
	memset(nKickCount,0,sizeof(int)*ALLELEMENT_COUNT);		
	CString strInfo;

#ifdef USE_IO_CARD
	nKickCount[Camera1] = m_IOCard[Camera1].readCounter(4)%256;   //��ȡ���1���޷ϼ���
	nKickCount[Camera1] = m_NewIOCard.ReadOutputCtrlCnt(0, 0);
#else
	nKickCount[Camera1] = m_iCheckErrorCount[Camera1]%256;
	nKickCount[Camera2] = m_iCheckErrorCount[Camera2]%256;
#endif
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// �ж����1���޷ϼ���
	// �ж���һ֡�ļ���Ϊ255����һ֡�ļ���Ϊ0����������������Ͻ�֡���������ۼ�
	
	for(i=0;i<1;i++)
	{
		if(m_iLastKickCount[i] > 200 && nKickCount[i] <= 50)
		{
			m_iKickFlowNumber[i] += 1;
		}
		// ������������Ϊ��������ۼӣ�����һ 
		m_iHardKickCount[i] = nKickCount[i] + 256*m_iKickFlowNumber[i];	
		
		if(m_iLastKickCount[i]!=nKickCount[i])
		{
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
				m_fileKickImage[i].Write(strInfo.GetBuffer(0),strInfo.GetLength());
			}
		}
	}
	
	return TRUE;
}

BOOL CMainFrame::GetCount_TuXiangHao()
{
	int i=0;
	
	int nImageCount[ALLELEMENT_COUNT];
	memset(nImageCount,0,sizeof(int)*ALLELEMENT_COUNT);
		
	int nLastRecordKick[ALLELEMENT_COUNT];
	memset(nLastRecordKick,0,sizeof(int)*ALLELEMENT_COUNT);
	
	double dd1[PE_COUNT]={0.0};
	double dDiff[PE_COUNT]={0.0};
	
	LONGLONG lQt1[PE_COUNT]={0};
	LONGLONG lQt2[ALLELEMENT_COUNT]={0};
	LONGLONG lLastQt1[PE_COUNT]={0};
	
	LARGE_INTEGER iLarge[PE_COUNT];
	
	CString strText,strInfo;

#ifdef USE_IO_CARD
	if (e_IOCardType_PIOC == m_eIOCardType)
	{
		nImageCount[Camera1] = m_IOCard[Camera1].readCounter(28) % 256;
		nImageCount[Camera2] = m_IOCard[Camera1].readCounter(31) % 256;
		nImageCount[Camera3] = m_IOCard[Camera1].readCounter(29) % 256;
	}
	else if (e_IOCardType_NewCard == m_eIOCardType)
	{
		nImageCount[Camera1] = m_NewIOCard.ReadOutputCount(1) % 256;
		nImageCount[Camera2] = m_NewIOCard.ReadOutputCount(2) % 256;
		nImageCount[Camera3] = m_NewIOCard.ReadOutputCount(3) % 256;
	}
#else
	nImageCount[Camera1]=m_iGrabCircleCount[Camera1]%256;
	nImageCount[Camera2]=m_iGrabCircleCount[Camera2]%256;
	nImageCount[Camera3]=m_iGrabCircleCount[Camera3]%256;
#endif
	
	// ͼ��żĴ���
	for(i=0;i<ALLELEMENT_COUNT;i++)
	{			
		if(m_iLastImageCountInPE[i]>200 && nImageCount[i]<50)
		{
			m_iFlowOverImageCountInPE[i]++;
		}	
		m_iImageCountInPE[i]=nImageCount[i]+256*m_iFlowOverImageCountInPE[i];
	
		if(nImageCount[i] != m_iLastImageCountInPE[i])
		{	
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
	int i=0;

	CString strOri,strAlg,sPathFileInModelInfo, sPathFileInConfig;
	strAlg.Format("%s\\ModelInfo\\%s",m_sWorkPath,strModelName);
//#ifdef __RealGrabber_
	for(i=0;i<CAMERA_COUNT;i++)
	{
		CString strSession{ _T("") }, strDeviceInitFile, str;
		strSession.Format("�豸%c��ʼ���ļ���", 'A' + i);
		CExFn::GetPrivateProfileString("�ɼ�����", strSession, strDeviceInitFile,m_sSysConfigFileName);
		//strDeviceInitFile.Format("%s\\config\\%s", m_sWorkPath, str);

		sPathFileInModelInfo.Format("%s\\%s",strAlg, strDeviceInitFile);
		sPathFileInConfig = CExFn::GetCurrentAppPath() + _T("\\Config\\") + strDeviceInitFile;
		if(FALSE==PathFileExists(sPathFileInModelInfo))
		{
			CopyFile(sPathFileInConfig, sPathFileInModelInfo,false);
		}
		else
		{
			CopyFile(sPathFileInModelInfo, sPathFileInConfig,false);
		}
	}
//#endif
	return TRUE;
}

void CMainFrame::UpdateCheckData()
{
	if(TRUE==m_bUpdateCheckData)
	{
		return;
	}

	m_bUpdateCheckData=TRUE;
	CString strStopTime;
	CTime CurrTime = CTime::GetCurrentTime();
	strStopTime.Format("%d-%d-%d %d:%d:%d",CurrTime.GetYear(),CurrTime.GetMonth(),CurrTime.GetDay(),
		CurrTime.GetHour(),CurrTime.GetMinute(),CurrTime.GetSecond());
	
	CString strCheckTotal,strBadTotal,strGoodTotal, strBadTotalRatio;
	CString strTotal[ALLELEMENT_COUNT],strGood[ALLELEMENT_COUNT];
	CString strBad[ALLELEMENT_COUNT],strBadRatio[ALLELEMENT_COUNT];
	
	strCheckTotal.Format("%d",m_iCheckCountTotal);
	strBadTotal.Format("%d",m_iCheckErrorCountTotal);
	strGoodTotal.Format("%d",m_iCheckGoodCountTotal);
	
	int iGood=0;
	float fRatio=0.0;
	
	for(int i=0;i<ALLELEMENT_COUNT;i++)
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
	info.strUser=m_sUserName;
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

		m_bUpdateCheckData=FALSE;
}

BOOL CMainFrame::DeleteDirectory(char* sDirName)
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
			char sFoundFileName[MAX_PATH];
			strcpy(sFoundFileName, tempFind.GetFileName().GetBuffer(MAX_PATH));

			if (tempFind.IsDirectory())
			{
				char sTempDir[MAX_PATH];
				sprintf(sTempDir, "%s/%s", sDirName, sFoundFileName);
				DeleteDirectory(sTempDir);
			}	//�ŷ� 2017/7/7 ������ 16:05:37	
			else
			{
				char sTempFileName[MAX_PATH];
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


