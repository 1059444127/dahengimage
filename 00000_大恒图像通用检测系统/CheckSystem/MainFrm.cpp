
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CheckSystem.h"
#include "CheckSystemDoc.h"
#include "CheckSystemView.h"
#include "DlgDefectStatistics.h"
#include "DlgDataStatistics.h"

#include "MainFrm.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame


extern CMainFrame*		g_pFrame; //ȫ��ָ��
extern CCheckSystemDoc*		g_pDoc;
extern CCheckSystemView*	g_pView;

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

static UINT indicators[] =
{
//	ID_SEPARATOR,           // status line indicator
// 	ID_INDICATOR_CAPS,
// 	ID_INDICATOR_NUM,
// 	ID_INDICATOR_SCRL,
ID_INDICATOR_OPERATION,
ID_INDICATOR_GRAB,
ID_INDICATOR_CHECK,
ID_INDICATOR_ERROR,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	for (int i = 0; i < 256; i++)
	{
		m_pLutR[i] = min((int)(i * 0), 255);
		m_pLutG[i] = min((int)(i * 0), 255);
		m_pLutB[i] = min((int)(i * 0), 255);
	}
	CoInitialize(NULL);
}

CMainFrame::~CMainFrame()
{
	
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	//ShowWindow(SW_MAXIMIZE);
	//����״̬��
	m_wndStatusBar.SetPaneInfo(0, ID_INDICATOR_OPERATION, SBPS_NORMAL, 300);
	m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_GRAB, SBPS_NORMAL, 1800);  
// 	m_wndStatusBar.SetPaneInfo(2, ID_INDICATOR_CHECK, SBPS_NORMAL, 150);
// 	m_wndStatusBar.SetPaneInfo(3, ID_INDICATOR_ERROR, SBPS_NORMAL, 200);
	//Sleep(10000);
	
	//HWND hwnd1 = g_pView->m_pdlgRunning->GetDlgItem(ID_STATIC_ALGORITHM_IMAGE + 0)->GetSafeHwnd();
	//Sleep(100)
	//InitMultiObject();
	InitializeSystem();

	return 0;
}


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


// CMainFrame message handlers



BOOL CMainFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, LPCTSTR lpszMenuName, DWORD dwExStyle, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	return CFrameWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, lpszMenuName, dwExStyle, pContext);
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	cs.hMenu = NULL;

	m_cxScreenSize = GetSystemMetrics(SM_CXSCREEN);
	m_cyScreenSize = GetSystemMetrics(SM_CYSCREEN);
	cs.style = WS_POPUP;
	cs.x = 0;
	cs.y = 0;
	cs.cx = m_cxScreenSize;
	cs.cy = m_cyScreenSize;
	CString sPathFileOfUICfg{ _T("") };
	sPathFileOfUICfg.Format(_T("%s_%d_%d.ini"), CExFn::GetCurrentAppPath() + _T("\\Config\\UIConfig"), m_cxScreenSize, m_cyScreenSize);
	CINICfg iniUICfg(sPathFileOfUICfg);
	int iWindowType = iniUICfg.GetIntEx(_T("��������"), _T("����������"), 0, _T("0����󻯣� 1���Զ����С"));
	int iWidth = 0, iHeight = 0;
	iWidth = iniUICfg.GetIntEx(_T("��������"), _T("�Զ�����"), 0);
	iHeight = iniUICfg.GetIntEx(_T("��������"), _T("�Զ���߶�"), 0);
	if (1 == iWindowType)
	{
		if (100 < iWidth&&iWidth<cs.cx)
			cs.cx = iWidth;
		if(100<iHeight&&iHeight<cs.cy)
			cs.cy = iHeight;
	}
	return CFrameWnd::PreCreateWindow(cs);
}


BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return CFrameWnd::OnEraseBkgnd(pDC);
}

void CMainFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	m_bIsDetecting = false;
	bExit = true;
	Sleep(200);
	ReleaseMemory();
	ReleaseResource();
	WriteCfg();

	KillTimer(ID_EVENT_UPDATE_INFO);
	KillTimer(ID_EVENT_UPDATE_CHECK_SPEED);
	//exit(-1);
	CFrameWnd::OnClose();
}


bool CMainFrame::ReadCfg()
{
	bool bRet = false;
	bRet = ReadCfgOfUI();
	if(!bRet)
		return false;
	bRet = ReadCfgOfAlg();
	if (!bRet)
		return false;
	bRet = ReadCfgOfSys();
	if (!bRet)
		return false;
	return true;
}


bool CMainFrame::InitializeSystem()
{
	m_sPathOfApp = CExFn::GetCurrentAppPath();
	m_sPathOfCfg = m_sPathOfApp + _T("\\Config");
	m_sPathFileOfAlgCfg = m_sPathOfApp + _T("\\Config\\AlgConfig.ini");
	m_iniAlgConfig.Init(m_sPathFileOfAlgCfg);//**[11/16/2017 ConanSteve]**:  ��ʼ��AlgConfig.ini�ļ�
	m_xmlAlgConfig.Init(m_sPathOfApp + _T("\\Config\\AlgConfig.xml"));
	/*************************************<>************************************[11/16/2017 ConanSteve]*/
	ReadCfg();//**[8/25/2017 ConanSteve]**:  ��ȡ�����ļ�
	/*************************************<������̨��ʼ������>************************************[8/25/2017 ConanSteve]*/
	//HANDLE hThred = (HANDLE)_beginthreadex(NULL, 0, (unsigned(_stdcall*)(void*))ThreadInitBackstage, this, 0, NULL);
	//HANDLE hThred = chBEGINTHREADEX(NULL, 0, ThreadInitBackstage, this, 0, NULL);
	//CloseHandle(hThred);
	//ThreadInitBackstageFunc(1);

	return true;
}


bool CMainFrame::ReadCfgOfUI()
{
	return true;
}


bool CMainFrame::ReleaseMemory()
{

	return false;
}




bool CMainFrame::ReadCfgOfAlg()
{
	/*************************************<��ȡ���м��ģʽ>************************************[8/25/2017 ConanSteve]*/
	/************************************************************************/
	CString sHashKey{ _T("") }, sHashValue{ _T("") }, sAppName{_T("")};
	int iKeyValue{ 0 };
	m_sPathOfVirtualDisk = m_iniAlgConfig.GetStringEx(_T("Demo Controls"), _T("VirtualDisk"), m_sPathOfVirtualDisk, _T("M"));
	m_sPathOfVirtualDisk += _T(":");
	if (!PathFileExists(m_sPathOfVirtualDisk))
	{
		//**[8/25/2017 ConanSteve]**:  ����Ӳ��·�������ڣ�������ȷ��
		AfxMessageBox(tr("The path of virtual disk doesn't exist, please check it again!"));
		exit(0);
	}

	bool bDisableCreate = m_iniAlgConfig.GetBoolEx(_T("DEMO Controls"), _T("DisableCreate"), true);

	/*************************************<дDemo������ť������>************************************[8/25/2017 ConanSteve]*/ 
	m_iniAlgConfig.WriteString(_T("DEMO Controls"), _T("ImgBtn1Text"), tr("Locate Grab Images"));
	m_iniAlgConfig.WriteString(_T("DEMO Controls"), _T("ImgBtn2Text"), tr("Locate Error Images"));
	m_iniAlgConfig.WriteString(_T("DEMO Controls"), _T("ImgBtn3Text"), tr("Locate Not Empty Images"));


	/*************************************<д������ͼ���ļ���·��>************************************[8/25/2017 ConanSteve]*/ 
	sHashValue = m_sPathOfVirtualDisk + _T("\\SaveImages\\GrabImages");
	m_iniAlgConfig.WriteString(_T("DEMO Controls"), _T("ImgBtn1Path"), sHashValue);
	sHashValue = m_sPathOfVirtualDisk + _T("\\SaveImages\\ErrorImages");
	m_iniAlgConfig.WriteString(_T("DEMO Controls"), _T("ImgBtn2Path"), sHashValue);
	sHashValue = m_sPathOfVirtualDisk + _T("\\SaveImages\\ModelImages");
	m_iniAlgConfig.WriteString(_T("DEMO Controls"), _T("ImgBtn3Path"), sHashValue);

	/*************************************<д��������ͼ�ļ��м����ز���>************************************[8/25/2017 ConanSteve]*/
	m_iniAlgConfig.GetBoolEx(_T("DEMO Controls"), _T("EnableSequentialCheck"), FALSE, _T("�Ƿ������ͼ�ļ��м��"));//**[8/24/2017 ConanSteve]**:  �Ƿ��Demo����ͼ��⹦��	
	/*************************************<Demo����ͼ�����ļ���ǰ׺>*************************************/ 
	m_iniAlgConfig.GetStringEx(_T("DEMO Controls"), _T("SequentialCheckFolderFilter"), sHashValue, _T(""));
	/*************************************<��ȡ�ϴ�ʹ��ģ������>*************************************[8/25/2017 ConanSteve]*/
	m_sModelName = m_iniAlgConfig.GetStringEx(_T("Demo Controls"), _T("DefaultModel"), m_sModelName);
	/*************************************<������־�ļ���·��>*************************************/ 
	m_sPathOfSysLogs = m_sPathOfVirtualDisk + _T("\\LogFiles\\CheckSystemLogs");
	//CExFn::DeleteDirectory(m_sPathOfSysLogs);
	CExFn::CreateMultiDirectory(m_sPathOfSysLogs);
	m_sPathFileOfDebugInfo = m_sPathOfSysLogs + _T("\\SystemDebugInfo.txt");
	m_fileDebugInfo.Open(m_sPathFileOfDebugInfo);
	m_fileErrorInfo.Open(m_sPathOfSysLogs + _T("\\SystemErrorInfo.txt"));
	/*************************************<ϵͳ���>************************************[9/5/2017 ConanSteve]*/
	m_bEnableDrawImg = m_iniAlgConfig.GetBoolEx(_T("SystemConfiguration"), _T("EnableDrawImage"), false, _T("ϵͳ�Ƿ�ͼ"));
	m_bEnableInitIOCard = TRUE == GetPrivateProfileInt(_T("SystemConfiguration"), _T("EnableInitializeIOCard"), TRUE, m_sPathFileOfAlgCfg);
	CExFn::WritePrivateProfileInt(_T("SystemConfiguration"), _T("EnableInitializeIOCard"), m_bEnableInitIOCard ? TRUE : FALSE, m_sPathFileOfAlgCfg, _T("�Ƿ��ʼ���ӿڿ�"));
	m_bEnableInitGrabber = m_iniAlgConfig.GetBoolEx(_T("SystemConfiguration"), _T("EnableInitializeGrabber"), true, _T("�Ƿ��ʼ�����"));
	//CExFn::WritePrivateProfileInt(_T("SystemConfiguration"), _T("EnableInitializeGrabber"), m_bEnableInitGrabber ? TRUE : FALSE, m_sPathFileOfAlgCfg, _T("�Ƿ��ʼ�����"));
	m_bEnableInitMultiObjectAndCheck = m_iniAlgConfig.GetBoolEx(_T("SystemConfiguration"), _T("EnableInitMultiObjectAndCheck"), true, _T("�Ƿ�ִ�ж����ĳ�ʼ�����㷨��⹤��"));
	/*************************************<״̬����Ϣ��ʾ����>************************************[12/28/2017 ConanSteve]*/
	sAppName = _T("״̬����Ϣ��ʾ����");
	m_bEnableShowPE = m_iniAlgConfig.GetBoolEx(sAppName, _T("��������Ƿ���ʾ"), true);
	m_bEnableShowAutoKick = m_iniAlgConfig.GetBoolEx(sAppName, _T("���߼����Ƿ���ʾ"), true);
	m_bEnableShowGrabLostFrame= m_iniAlgConfig.GetBoolEx(sAppName, _T("��ͼ��֡�Ƿ���ʾ"), true);
	/*************************************<����㷨�����ļ����Ƿ����>*************************************/
	m_sPathOfAlgModelInfo= m_sPathOfApp+_T("\\ModelInfo");
	if (!PathFileExists(m_sPathOfAlgModelInfo))
	{
		if (FALSE == CreateDirectory(m_sPathOfAlgModelInfo, NULL))
		{
			AfxMessageBox(tr("����·��ʧ��, ��ȷ�ϱ����㷨�����ļ����Ƿ�������ȷ!!!"));
			return false;
		}
	}
	m_sPathOfAlgModelInfo += _T("\\") + m_sModelName;
	/*************************************<���ModeInfo���Ƿ���CheckSystemConfig.ini�ļ�>************************************[9/1/2017 ConanSteve]*/
	m_sPathFileOfCheckSysCfgInModelInfo = m_sPathOfAlgModelInfo + _T("\\CheckSystemConfig.ini");
	if (FALSE == PathFileExists(m_sPathFileOfCheckSysCfgInModelInfo))
	{
		CString sInfo{ _T("") };
		sInfo.Format(_T("%s\"%s\"%s"),tr("�䷽"), m_sModelName, tr("ϵͳ�����ļ�������!"));
		AfxMessageBox(sInfo);
		exit(0);
	}
	m_iniCheckSystemConfig.Init(m_sPathFileOfCheckSysCfgInModelInfo);//**[11/16/2017 ConanSteve]**:  ��ʼ��CheckSystemConfig.ini�ļ�
// 	if (FALSE == PathFileExists(m_sPathFileOfCheckSysCfgInModelInfo))
// 	{
// 		CopyFile(m_sPathFileOfCheckSysCfgInModelInfo, m_sPathFileOfCheckSysCfgInConfig, FALSE);//**[9/1/2017 ConanSteve]**:  ǿ�Ƹ���
// 	}
	/*************************************<���ģ����������ļ��Ƿ����>*************************************/
	return true;
}


bool CMainFrame::ReadCfgOfSys()
{
	CString sErrorInfo{ _T("") };
	CString sModuleName{ _T("") };
	m_sWindowTitle = m_iniCheckSystemConfig.GetStringEx(_T("ϵͳ����"), _T("���ڱ���"), m_sWindowTitle);
	
	m_iPENum = m_iniCheckSystemConfig.GetIntEx(_T("IOCard Parameters"), _T("������"), 0);//**[11/16/2017 ConanSteve]**:  ��ȡ��Ʒ������
	m_iGrabberNum = m_iniCheckSystemConfig.GetIntEx(_T("�ɼ�����"), _T("�豸����"), 0);
	m_iniAlgConfig.WriteInt(_T("Basic Parameters"), _T("CameraNum"), m_iGrabberNum, _T("�������"));
	m_eProductLine = (e_ProductLine)m_iniCheckSystemConfig.GetIntEx(_T("Critical Config"), _T("ProductLineType"), e_ProductLine_NO, _T("��Ʒ�����ͣ�1Ϊͨ���㷨��2Ϊ�����㷨��3Ϊ�Ƽ���㷨�� 4Ϊ��Ѫ���㷨"));
	m_iniAlgConfig.WriteInt(_T("Critical Config"), _T("ProductLineType"), m_eProductLine, _T("��Ʒ�����ͣ�1Ϊͨ���㷨��2Ϊ�����㷨��3Ϊ�Ƽ���㷨�� 4Ϊ��Ѫ���㷨"));
	m_iAssemblyLineNum = m_iniCheckSystemConfig.GetIntEx(_T("Critical Config"), _T("AssemblyLineNumber"), 1, _T("��Ʒ�ߣ�ͨ��������"));
	/*************************************<����SystemConfig���õ�AlgConfig�ļ��У���������ʼ��ʹ��>************************************[11/24/2017 ConanSteve]*/
	/*************************************<��ˮ�����>************************************[12/28/2017 ConanSteve]*/
	m_iCheckSpeedUpdateInterval = m_iniCheckSystemConfig.GetIntEx(_T("Critical Config"), _T("CheckSpeedUpdateInterval"), 1000, _T("����ٶ�ˢ�����ڣ���λ��ms"));
	/*************************************< �޷�����>************************************[11/27/2017 ConanSteve]*/
	{
		//**[11/27/2017 ConanSteve]**: ����ÿ���޷϶��еĳ�ʱ����
		sModuleName = _T("Basic Parameters");
		m_iKickQueueNum = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("KickQueueNum"), 1);
		m_iniAlgConfig.WriteInt(sModuleName, _T("KickQueueNum"), m_iKickQueueNum);
		for (int iKickQueueIndex = 0; iKickQueueIndex < m_iKickQueueNum; ++iKickQueueIndex)
		{
			CString sModuleName{ _T("") };
			sModuleName.Format(_T("KickGroup%d"), iKickQueueIndex + 1);
			int iTimeType = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("TimeType"), 1, _T("�޷϶���1��ʱ��׼ 0Ϊʱ�䣬��λΪ���룻1Ϊ֡/�У���ʱ��λΪ֡����/��������Ƶ"));
			m_iniAlgConfig.WriteInt(sModuleName, _T("TimeType"), iTimeType, _T("�޷϶���1��ʱ��׼ 0Ϊʱ�䣬��λΪ���룻1Ϊ֡/�У���ʱ��λΪ֡����/��������Ƶ"));
			int iTimeout = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("TimeOut"), 5);
			m_iniAlgConfig.WriteInt(sModuleName, _T("TimeOut"), iTimeout);
			m_arr_eKickType[iKickQueueIndex] = (e_KickType)m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("KickType"), 0, _T("0�ǵ�·�޷ϣ�1�Ƕ�·��ֱ�޷ϣ�2��·ˮƽ�޷�"));
			m_iniAlgConfig.WriteInt(sModuleName, _T("KickType"), m_arr_eKickType[iKickQueueIndex], _T("0�ǵ�·�޷ϣ�1�Ƕ�·��ֱ�޷ϣ�2��·ˮƽ�޷�"));

			int iAssemblyId = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("AssemblySN"), -1, _T("��ǰ�޷�����������ˮ�ߺţ���0��ʼ����"));

			if (0 <= iAssemblyId&&iAssemblyId < m_iAssemblyLineNum)
			{
				m_map_KickSN2AssemblySN[iKickQueueIndex] = iAssemblyId;
				m_arr_veciKickSNInAssemblyLine[iAssemblyId].push_back(iKickQueueIndex);
			}
			else
			{
				sErrorInfo.Format(_T("�߷���%d��ˮ�ߺŲ���Ϊ%d, �����¼�������ļ�!"), iKickQueueIndex, iAssemblyId);
				m_fileErrorInfo.Write(sErrorInfo);
				AfxMessageBox(sErrorInfo);
				exit(0);
			}

		}
	}
	/*************************************<��������>************************************[12/28/2017 ConanSteve]*/
	{
		sModuleName = _T("Inspection Parameters");
		m_iCheckGroupNum = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("CheckNum"), 1);
		m_iniAlgConfig.WriteInt(sModuleName, _T("CheckNum"), m_iCheckGroupNum, _T("�������"));

		for (int i = 0; i < m_iCheckGroupNum; ++i)
		{
			//**[12/28/2017 ConanSteve]**:  ������̬������
			sModuleName = { _T("Inspection Parameters") };
			CString sDllNameKey{ _T("") }, sDllNameValue{ _T("") }, sCheckObjNumKey{ _T("") };
			int iCheckObjNumValue{ 2 };
			sDllNameKey.Format(_T("CheckDllName_%d"), i + 1);
			sDllNameValue = m_iniCheckSystemConfig.GetStringEx(sModuleName, sDllNameKey, sDllNameValue);
			m_iniAlgConfig.WriteString(sModuleName, sDllNameKey, sDllNameValue);

			sCheckObjNumKey.Format(_T("CheckObjNum_%d"), i + 1);
			iCheckObjNumValue = m_iniCheckSystemConfig.GetIntEx(sModuleName, sCheckObjNumKey, 2);
			m_iniAlgConfig.WriteInt(sModuleName, sCheckObjNumKey, iCheckObjNumValue);

			sModuleName.Format(_T("CheckGroup%d"), i + 1);
			int iKickSN = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("KickSN"), -1, _T("����������߷϶��кţ���0��ʼ"));
			//m_iniAlgConfig.WriteInt(sModuleName, _T("KickSN"), iKickSN, _T("����������߷϶��к�"));
			if (0 <= iKickSN && iKickSN < m_iKickQueueNum)
			{
				m_arr_veciCheckSNInKickQueue[iKickSN].push_back(i);
				int iAssemblyId = m_map_KickSN2AssemblySN[iKickSN];
				m_arr_veciCheckGroupIndexOfAsseblyLine[iAssemblyId].push_back(i);
				m_map_CheckSN2KickSN[i] = iKickSN;
			}
			else
			{
				sErrorInfo.Format(_T("�����%d�߷϶��кŲ���Ϊ%d, �����¼�������ļ�!"), i, iKickSN);
				m_fileErrorInfo.Write(sErrorInfo);
				AfxMessageBox(sErrorInfo);
				exit(0);
			}
		}
	}
	/*************************************<�����������>************************************[12/28/2017 ConanSteve]*/
	for (int i = 0; i < m_iGrabberNum; ++i)
	{
		sModuleName.Format(_T("Camera%d Parameters"), i + 1);
		int iCheckSN = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("CheckGroup"), -1, _T("������ڼ����,��0��ʼ"));
		if (0 <= iCheckSN && iCheckSN < m_iCheckGroupNum)
		{
			m_iniAlgConfig.WriteInt(sModuleName, _T("CheckGroup"), iCheckSN + 1, _T("������ڼ���飬��1��ʼ"));
			int iKickSN = m_map_CheckSN2KickSN[iCheckSN];
			m_iniAlgConfig.WriteInt(sModuleName, _T("KickGroup"), iKickSN + 1, _T("��������޷϶��кţ���1��ʼ"));
		}
		else
		{
			sErrorInfo.Format(_T("���%d���ڼ����ID%d����ȷ�������¼�������ļ���"), i + 1, iCheckSN);
			m_fileErrorInfo.Write(sErrorInfo);
			AfxMessageBox(sErrorInfo);
			exit(0);
		}

		m_arr_iSmallImgCntPerGrab[i] = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("SmallImageNumberPerGrabber"), 1, _T("���ÿ��ͼ��Ųɼ���ͼ��С����"));
		m_iniAlgConfig.WriteInt(sModuleName, _T("PictureNumberPerCamera"), m_arr_iSmallImgCntPerGrab[i], _T("ͼ������"));
		m_arr_sCameraImageInfo[i].m_iBayerType = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("HalconBayerType"), 0, _T("�㷨�ã�Halcon Bayer�任����,0:���任��1:GB,2:GR,3:BG,4:RG"));
		m_arr_sCameraImageInfo[i].m_iFlipType = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("HalconFlipType"), 0, _T("�㷨�ã�ϵͳ���� ��ת���ͣ�0����ת��1ˮƽ��ת��2��ֱ��ת��3ȫ��ת���㷨ʹ��"));
		m_arr_iSystemBayerType[i] = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("SystemBayerType"), 0, _T("ϵͳ��Bayer�任�����͡�0�����任�� 1��RG��2��GB��3��GR��4��BG"));
		m_arr_iSystemFilpType[i] = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("SystemFlipType"), 0, _T("ϵͳBayer��ת����. 0:NO, 1:��ֱ 2:ˮƽ"));
		m_arr_sCameraImageInfo[i].m_iCameraImagePixelSize = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("Byte"), 1, _T("������ͼ������ÿ�����ֽ�����С��ϵͳ���㷨Bayer�任֮���ͼΪ3��ԭͼΪ1"));
		m_iniAlgConfig.WriteInt(sModuleName, _T("Byte"), m_arr_sCameraImageInfo[i].m_iCameraImagePixelSize, _T("������ͼ������ÿ�����ֽ�����С��ϵͳ���㷨Bayer�任֮���ͼΪ3��ԭͼΪ1"));

		m_arr_BayerTransform[i].m_bAreaCamBayer = m_arr_iSystemBayerType[i] != 0 ? TRUE : FALSE;
		m_arr_BayerTransform[i].m_dBayerRedGain = m_iniCheckSystemConfig.GetDoubleEx(sModuleName, _T("Bayerת��Red����"), 0);
		m_arr_BayerTransform[i].m_dBayerGreenGain = m_iniCheckSystemConfig.GetDoubleEx(sModuleName, _T("Bayerת��Green����"), 0);
		m_arr_BayerTransform[i].m_dBayerBlueGain = m_iniCheckSystemConfig.GetDoubleEx(sModuleName, _T("Bayerת��Blue����"), 0);
		if (m_arr_BayerTransform[i].m_bAreaCamBayer)
		{
			SetLutTable(m_arr_BayerTransform[i].m_dBayerRedGain, m_arr_BayerTransform[i].m_dBayerGreenGain,
				m_arr_BayerTransform[i].m_dBayerBlueGain);
		}
		m_arr_bWhetherImageConvertedToMultiObj[i] = m_iniCheckSystemConfig.GetBoolEx(sModuleName, _T("IsBayerConverted"), false, _T("�㷨���յ��Ƿ���ϵͳBayer�任���ͼ��"));

	}
	
	/*************************************<��ȡȱ������>************************************[11/24/2017 ConanSteve]*/
	for (int iCheckGroupSN = 0; iCheckGroupSN < m_iCheckGroupNum; ++iCheckGroupSN)
	{
		int iDefectSN = 0;
		CString sDefectDesc{ _T("") };
		CString sModuleName{ _T("") }, sKeyName{ _T("") };
		sModuleName.Format(_T("��λ%dȱ������"), iCheckGroupSN+1);
		sKeyName.Format(_T("ȱ����"));
		int iDefectNum = m_iniCheckSystemConfig.GetIntEx(sModuleName, sKeyName);
		for (int iDefectIndex = 1; iDefectIndex <= iDefectNum; ++iDefectIndex)
		{
			sKeyName.Format(_T("ȱ������%d"), iDefectIndex);
			sDefectDesc = m_iniCheckSystemConfig.GetStringEx(sModuleName, sKeyName, sDefectDesc);
			if (_T("")!= sDefectDesc)
			{
				//**[12/1/2017 ConanSteve]**:  ����ǰȱ����ӽ�ϵͳ
				m_arr_mapDefectStatistics[iCheckGroupSN].insert(make_pair(iDefectIndex, s_DefectInfo(0, sDefectDesc)));
			}
		}
		
	}
	
	CINICfg iniSet(CExFn::GetCurrentAppPath() + _T("\\set.ini"));
	iniSet.WriteString(_T("ϵͳ����"), _T("AppName"), _T("CheckSystem"));
	//CheckCamCfgFile();
	CheckIOCardCfgFile();
	return false;
}


void CMainFrame::CheckCamCfgFile()
{
// 	CString sKeyName{ _T("") };
// 	CString sModuleName = _T("Grabber Parameters");
// 	CString sFileNameOfGrab{ _T("") };  //**[9/1/2017 ConanSteve]**:  ��������ļ�����
// 	for (int i = 0; i < m_iGrabberNum; ++i)
// 	{
// 		sKeyName.Format(_T("FileNameOfGrabberInit_%d"), i + 1);
// 		CExFn::GetPrivateProfileStringEx(sModuleName, sKeyName, sFileNameOfGrab, m_sPathFileOfCheckSysCfgInConfig);
// 		m_fileDebugInfo.Write(m_sPathFileOfCheckSysCfgInConfig);
// 		m_arr_sPathFileOfGrabInitInConfig[i] = m_sPathOfCfg +_T("\\")+ sFileNameOfGrab;
// 		m_arr_sPathFileOfGrabInitInModelInfo[i] = m_sPathOfAlgModelInfo + _T("\\")+ sFileNameOfGrab;
// 		//CopyFile(m_pPathFileOfGrabInitInModelInfo[i], m_arr_sPathFileOfGrabInitInConfig[i], FALSE);//**[9/1/2017 ConanSteve]**:  ǿ�Ƹ���
// 	}
}


int CMainFrame::CheckIOCardCfgFile()
{	
	return 0;
}


CRetInfo CMainFrame::InitIOCard()
{
	CRetInfo ret;
	fill(m_arr_iIOCardType.begin(), m_arr_iIOCardType.end(), e_IOCardType_SG);
	if (!m_bEnableInitIOCard)
	{
		m_fileDebugInfo.Write(_T("���棺�ӿڿ���ʼ��������"));
		ret.bRet = true;
		return ret;
	}
	m_iIOCardNum = m_iniCheckSystemConfig.GetIntEx(_T("IOCard Parameters"), _T("IOCardNumber"), 0);
	CString sKeyName{ _T("") };

	for (int i = 0; i < m_iIOCardNum; ++i)
	{
		sKeyName.Format(_T("IOCardType_%d"), i);
		m_arr_iIOCardType[i] = (e_IOCardType)m_iniCheckSystemConfig.GetIntEx(_T("IOCard Parameters"), sKeyName, 0,_T("0���޿���1ģ��ӿڿ���2��B����3��C����4��PCI6356����5��PCIE_6356���� 6��RS232_XZXNYN���ڼ̵���"));
		switch (m_arr_iIOCardType[i])
		{
		case e_IOCardType::e_IOCardType_NONE:
		{
			ret.sErrorInfo.Format(tr("�ӿڿ�%d���Ͳ���Ϊ��"), i);
			m_fileErrorInfo.Write(ret.sErrorInfo);
			ret.bRet = false;
			return ret;
		}
			continue;
			break;
		case e_IOCardType::e_IOCardType_SG:
			m_arr_pIOCard[i] = make_shared<CDHIOCardForSG>();
			break;
		case e_IOCardType::e_IOCardType_PIOB:
			break;
		case e_IOCardType::e_IOCardType_PIOC:
			m_arr_pIOCard[i] = make_shared<CDHIOCardForPIO24C>();
			break;
		case e_IOCardType::e_IOCardType_PCI_6356:
			m_arr_pIOCard[i] = make_shared<CDHIOCardForPCI6356>();
			break;
		case e_IOCardType::e_IOCardType_PCIE_6356:
			break;
		case e_IOCardType::e_IOCardType_RS232_XZXNYN:
			m_arr_pIOCard[i] = shared_ptr<CDHIOCardForRS232_XZXNYN>(new CDHIOCardForRS232_XZXNYN());
			break;
		default:
			break;
		}
		if (nullptr == m_arr_pIOCard[i])
		{
			ret.sErrorInfo.Format(tr("�ӿڿ�%d����ʧ��"), i);
			m_fileErrorInfo.Write(ret.sErrorInfo);
			ret.bRet = false;
			return ret;
		}
		/*************************************<��ýӿڿ������ļ�·������Ϊ��ͬ�ӿڿ������ļ�·����һ��>************************************[9/13/2017 ConanSteve]*/
		sKeyName.Format(_T("FileNameOfIOCardInit_%d"), i);
		m_arr_sPathFileOfIOCardInitFile[i] = m_iniCheckSystemConfig.GetStringEx(_T("IOCard Parameters"), sKeyName, m_arr_sPathFileOfIOCardInitFile[i], _T("NULL"));
		m_arr_sPathFileOfIOCardInitFile[i] = CExFn::GetCurrentAppPath() +_T("\\") + m_arr_sPathFileOfIOCardInitFile[i];
		sKeyName.Format(_T("PathFileBtwGrabAndIOCard_%d"), i);
		CString sPathFileBtwGrabAndIOCard{ _T("") };
		sPathFileBtwGrabAndIOCard = m_iniCheckSystemConfig.GetStringEx(_T("IOCard Parameters"), sKeyName, sPathFileBtwGrabAndIOCard, _T("NULL"));
		sPathFileBtwGrabAndIOCard = CExFn::GetCurrentAppPath() + _T("\\") + sPathFileBtwGrabAndIOCard;
		CIOCardInitInfo initInfo;
		memcpy_s(initInfo.lpszPathFileOfInit, MAX_PATH, m_arr_sPathFileOfIOCardInitFile[i], m_arr_sPathFileOfIOCardInitFile[i].GetLength() * sizeof(TCHAR));
		memcpy_s(initInfo.lpszPathFileOfConnection, MAX_PATH, sPathFileBtwGrabAndIOCard, sPathFileBtwGrabAndIOCard.GetLength() * sizeof(TCHAR));
		m_fileDebugInfo.Write(_T("�ӿڿ������ļ�:") + (CString)initInfo.lpszPathFileOfInit);
		m_fileDebugInfo.Write(_T("�ӿڿ������ļ�:") + (CString)initInfo.lpszPathFileOfConnection);
		if (!m_arr_pIOCard[i]->InitCard(initInfo))
		{
			ret.bRet = false;
			ret.sErrorInfo = m_arr_pIOCard[i]->GetLastError();
			return ret;
		}
		if (!m_arr_pIOCard[i]->OpenCard())
		{
			ret.bRet = false;
			ret.sErrorInfo = m_arr_pIOCard[i]->GetLastError();
			return ret;
		}		
	}
	ret.bRet = true;
	return ret;
}

DWORD CMainFrame::ThreadInitBackstage(PVOID pParam)
{
	return 0;
	CMainFrame* pFrame = (CMainFrame*)pParam;
	return pFrame->ThreadInitBackstageFunc(3);
}

DWORD CMainFrame::ThreadInitBackstageFunc(int iFunc)
{
	shared_ptr<CString> pCurThredId = make_shared<CString>();
	pCurThredId->Format(_T("0x%x"), GetCurrentThreadId());
	TRACE(pCurThredId->GetBuffer());
	bool bRet{ false };
	CRetInfo ret;
	CString sHintInfo{ _T("") };


	AddMsg(tr("���ڳ�ʼ���ӿڿ�......"));
	ret = InitIOCard();
	if (!ret.bRet)
	{
		sHintInfo.Format(tr("��ʼ���ӿڿ�ʧ��!\n������Ϣ��%s"), ret.sErrorInfo);
		AddMsg(sHintInfo, true);
		m_bInitIOCardSuccess = false;
		 //::SendMessage(this->m_hWnd, WM_CLOSE, 0, 0);
		// return -1;
	}
	else
	{
		m_bInitIOCardSuccess = true;
	}

	AddMsg(tr("���ڳ�ʼ�����......"));
	ret = InitGrabber();
	if (!ret.bRet)
	{
		sHintInfo.Format(tr("��ʼ�����ʧ��!\n������Ϣ��%s"), ret.sErrorInfo);
		AddMsg(sHintInfo, true);
		//::SendMessage(this->m_hWnd, WM_CLOSE, 0, 0);
		return -1;
	}
	ResizeLayout();//**[12/5/2017 ConanSteve]**:  �������ڲ��֣��������ڳ�ʼ�����֮�󣬱����ڳ�ʼ�������֮ǰ
	//**[10/30/2017 ConanSteve]**:  ��ʼ�������
	AddMsg(tr("���ڳ�ʼ�������......"));
	ret = InitMultiObject();
	if (!ret.bRet)
	{
		sHintInfo.Format(tr("��ʼ�������ʧ��!\n������Ϣ��%s"), ret.sErrorInfo);
		AddMsg(sHintInfo, true);
		//::SendMessage(this->m_hWnd, WM_CLOSE, 0, 0);
		return -1;
	}

	AddMsg(tr("���ڳ�ʼ����־ϵͳ......"));
	bRet = InitLogSys();
	if (!bRet)
	{
		//::SendMessage(this->m_hWnd, WM_CLOSE, 0, 0);
		return -1;
	}
/************************************************************************/
	bRet = AllocateMemory();
	if (!bRet)
	{
		CString sInfo = tr("��ʼ���ڴ�ʧ��");
		AddMsg(sInfo, true);
		PostQuitMessage(0);
	}
	g_pView->m_xcTitleBar.EnbaleBtnOnOff(TRUE);

	/*************************************<��ȡʹDlgParamSet�������ļ��ж�ȡ����ֵ>************************************[12/25/2017 ConanSteve]*/
	m_pdlgParamSet = make_shared<CDlgParamSet>();
	m_pdlgParamSet->Create(IDD_DIALOG_ParamSet, this);
	m_pdlgParamSet->OnBnClickedButtonApply();
	/*************************************<>************************************[12/25/2017 ConanSteve]*/

	AddMsg(tr("ϵͳ��ʼ���ɹ�!"));
	ResetSystem(1);//**[12/28/2017 ConanSteve]**:  �������м���
	StartAllThread();
	SetTimer(ID_EVENT_UPDATE_INFO, 500, NULL);
	SetTimer(ID_EVENT_UPDATE_CHECK_SPEED, m_iCheckSpeedUpdateInterval, NULL);
	//SetTimer(ID_EVENT_START_GRABBER, 300, NULL);
	return 0;
}


CRetInfo CMainFrame::InitGrabber()
{
	CRetInfo ret;
	if (!m_bEnableInitGrabber)
	{
		m_fileDebugInfo.Write(_T("���棺�����ʼ��������"));
	}
	int iCamSN{ 0 };//**[8/29/2017 ConanSteve]**:  �豸���к�
	CString sKeyName{ _T("") };
	CString sKeyValue{ _T("") };
	CString sDebugInfo{ _T("") };
	CString sCamName{ _T("") };//**[8/29/2017 ConanSteve]**:  �������
	CString sFileNameOfCamInit{ _T("") };//**[8/29/2017 ConanSteve]**:  �����ʼ���ļ�����
	CString sModuleName{ _T("") };
	/*************************************<��������ļ���Ӧ���������>************************************[8/29/2017 ConanSteve]*/
	m_iUnitedGrabType = (e_GrabType)m_iniCheckSystemConfig.GetIntEx(_T("�ɼ�����"), _T("UnitedGrabberType"), (int)e_GrabType::e_GrabType_NONE, _T("0��ģ�������2:AVT�����7:JAI����� 8��MER���"));
	if (0 != m_iGrabberNum)
	{
		m_p_iCurNumOfSaveImgOfEachGrab = make_unique<int[]>(m_iGrabberNum);
		fill(m_arr_iGrabType.begin(), m_arr_iGrabType.end(), m_iUnitedGrabType);
	}
	if (0 != m_iCheckGroupNum)
	{
		fill(m_arr_iCheckCntOfCheckGroup.begin(), m_arr_iCheckCntOfCheckGroup.end(), 0);	
		fill(m_arr_iGoodCntOfCheckGroup.begin(), m_arr_iGoodCntOfCheckGroup.end(), 0);//**[12/5/2017 ConanSteve]**:  ÿ��������Ʒ����
		fill(m_arr_iBadCntOfCheckGroup.begin(), m_arr_iBadCntOfCheckGroup.end(), 0);//**[12/1/2017 ConanSteve]**:  ÿ��λȱ������
		fill(m_arr_iAlgExceptionCntOfCheckGroup.begin(), m_arr_iAlgExceptionCntOfCheckGroup.end(), 0);//**[12/5/2017 ConanSteve]**:  ÿ��λ�㷨�쳣����
		fill(m_arr_bEnableExecCheckOfEachGroup.begin(), m_arr_bEnableExecCheckOfEachGroup.end(), true);


	}
	if (0 != m_iKickQueueNum)
	{
		m_p_fileKickCntInfoChange = make_unique<CLogFile[]>(m_iKickQueueNum);

	}
	/*************************************<�����ʼ�����>************************************[8/29/2017 ConanSteve]*/
	array<CString, MAX_CAMER_COUNT> arr_sGrabDevName;//**[12/28/2017 ConanSteve]**:  ����豸����
	array<CString, MAX_CAMER_COUNT> arr_sGrabDevMark;//**[12/28/2017 ConanSteve]**:  �豸��ʶ
	array<int, MAX_CAMER_COUNT> arr_GrabSN;//**[12/28/2017 ConanSteve]**:  ������
	for (int i = 0; i < m_iGrabberNum; ++i)
	{
//  		if (0 == i)
//  			continue;
		/*************************************<��ȡ�����ļ�>************************************[8/29/2017 ConanSteve]*/
		sModuleName = _T("�ɼ�����");
		/*************************************<�豸����>************************************[8/29/2017 ConanSteve]*/
		{
			sKeyName.Format(_T("�豸%C����"), _T('A') + i);
			if (e_GrabType::e_GrabType_NONE == m_iUnitedGrabType)
			{
				m_arr_iGrabType[i] = (e_GrabType)m_iniCheckSystemConfig.GetIntEx(sModuleName, sKeyName, (int)m_iUnitedGrabType);
			}
			else
			{
				m_arr_iGrabType[i] = m_iUnitedGrabType;
			}
			switch (m_arr_iGrabType[i])
			{
			case e_GrabType::e_GrabType_SG:
				m_arr_pGrabber[i] = make_shared<CDHGrabForSG>();
				break;
			case e_GrabType::e_GrabType_AVT:
				//m_arr_pGrabber[i] = make_shared<CDHGrabForAVT>();
				break;
			case e_GrabType::e_GrabType_BAUMER:
				//m_arr_pGrabber[i] = shared_ptr<CDHGrabForBaumer>(new CDHGrabForBaumer);
				break;
			case e_GrabType::e_GrabType_JAI:
				//m_arr_pGrabber[i] = make_shared<CDHGrabForJAI>();
				break;
			case e_GrabType::e_GrabType_MER:
				m_arr_pGrabber[i] = make_shared<CDHGrabForMER>();
				break;
			default:
				break;
			}
		}
		if (nullptr == m_arr_pGrabber[i])
		{
			ret.sErrorInfo = tr("��ϵͳ��֧�ֵ�ǰ�ͺ����������ϵ������Ա��");
			ret.bRet = false;
			return ret;
		}
		/*************************************<�豸���>************************************[12/28/2017 ConanSteve]*/
		{
			sKeyName.Format(_T("�豸%C���"), _T('A') + i);
			arr_GrabSN[i] = m_iniCheckSystemConfig.GetIntEx(sModuleName, sKeyName, -1);
			if (iCamSN < 0 || arr_GrabSN[i] >= m_iGrabberNum)
			{
				m_fileDebugInfo.Write(CExFn::GetLastErrorMsg());
				sDebugInfo.Format(tr("��ʼ����������%d�����%d����ȷ"), i + 1, arr_GrabSN[i]);
				m_fileDebugInfo.Write(sDebugInfo);
				ret.bRet = false;
				ret.sErrorInfo = sDebugInfo;
				return ret;
			}
		}
		/*************************************<�豸����>************************************[12/28/2017 ConanSteve]*/
		{
			sKeyName.Format(_T("�豸%C����"), _T('A') + i);
			arr_sGrabDevName[i] = m_iniCheckSystemConfig.GetStringEx(sModuleName, sKeyName, arr_sGrabDevName[i]);
		}
		/*************************************<�豸��ʶ>************************************[12/28/2017 ConanSteve]*/
		{
			sKeyName.Format(_T("�豸%C��ʶ"), _T('A') + i);
			arr_sGrabDevMark[i] = m_iniCheckSystemConfig.GetStringEx(sModuleName, sKeyName, arr_sGrabDevMark[i]);
		}
		/*************************************<�豸��ʼ���ļ���>************************************[12/28/2017 ConanSteve]*/
		{
			sKeyName.Format(_T("�豸%C��ʼ���ļ���"), _T('A') + i);
			sFileNameOfCamInit = m_iniCheckSystemConfig.GetStringEx(sModuleName, sKeyName, sFileNameOfCamInit);
			m_arr_sPathFileOfGrabInitInModelInfo[i] = m_sPathOfAlgModelInfo + _T("\\") + sFileNameOfCamInit;
			if (!PathFileExists(m_arr_sPathFileOfGrabInitInModelInfo[i]))
			{
				sDebugInfo.Format(tr("��������:���%d�����ļ�%s�ļ�����ָ��·����"), i + 1, sFileNameOfCamInit);
				AfxMessageBox(sDebugInfo);
				m_fileErrorInfo.Write(sDebugInfo);
				ret.bRet = false;
				ret.sErrorInfo = sDebugInfo;
				return ret;
			}
		}
		
		bool bRet = false;
		{
// 			CGrabInitInfo GrabInfo;
// 			GrabInfo.iGrabSN = iCamSN;
// 			GrabInfo.Context = this;
// 			GrabInfo.CallBackFunc = AreaGrabOverCallback;
// 			memcpy_s(GrabInfo.lpszPathFileOfGrab, MAX_PATH * sizeof(TCHAR), m_arr_sPathFileOfGrabInitInModelInfo[i], m_arr_sPathFileOfGrabInitInModelInfo[i].GetLength() * sizeof(TCHAR));
// 			if (!m_bEnableInitGrabber)
// 				continue;
// 			bool bRet = TRUE == m_arr_pGrabber[i]->Init(GrabInfo);
		}

		{
			s_GBINITSTRUCT sAreaGrabber;
			sAreaGrabber.iGrabberTypeSN = m_arr_iGrabType[i];
			sAreaGrabber.iGrabberSN = arr_GrabSN[i];
			sAreaGrabber.Context = this;
			sAreaGrabber.CallBackFunc = AreaGrabOverCallback;
			memcpy_s(sAreaGrabber.strDeviceName, MAX_PATH * sizeof(TCHAR), arr_sGrabDevName[i].GetBuffer(), arr_sGrabDevName[i].GetLength() * sizeof(TCHAR));
			memcpy_s(sAreaGrabber.strDeviceMark, MAX_PATH * sizeof(TCHAR), arr_sGrabDevMark[i].GetBuffer(), arr_sGrabDevMark[i].GetLength() * sizeof(TCHAR));
			memcpy_s(sAreaGrabber.strGrabberFile, MAX_PATH * sizeof(TCHAR), m_arr_sPathFileOfGrabInitInModelInfo[i].GetBuffer(), m_arr_sPathFileOfGrabInitInModelInfo[i].GetLength() * sizeof(TCHAR));
			if (!m_bEnableInitGrabber)
				continue;
			bRet = m_arr_pGrabber[i]->Init(&sAreaGrabber);
		}
		if (!bRet)
		{
			s_GBERRORINFO GrabberInfo;
			m_arr_pGrabber[i]->GetLastErrorInfo(&GrabberInfo);
			sDebugInfo.Format(tr("���%d��ʼ��ʧ��!"), i + 1);
			sDebugInfo += GrabberInfo.strErrorDescription;
			AddMsg(sDebugInfo, true);
			ret.bRet = false;
			ret.sErrorInfo = sDebugInfo;
			return ret;
		}
		
		sDebugInfo.Format(_T("��ʼ�����%d�ɹ���"), i + 1);
		AddMsg(sDebugInfo);
		m_arr_pGrabber[i]->GetParamInt(GBImageWidth, m_arr_GrabInfo[i].m_iImageWidth);			// �������ͼ����
		m_arr_pGrabber[i]->GetParamInt(GBImageHeight, m_arr_GrabInfo[i].m_iImageHeight);			// �������ͼ��߶�
		m_arr_pGrabber[i]->GetParamInt(GBImageBufferSize, m_arr_GrabInfo[i].m_iImageSize);		// �������ͼ���С
		m_arr_pGrabber[i]->GetParamInt(GBImagePixelSize, m_arr_GrabInfo[i].m_iImagePixelSize);	// �������ͼ�����ش�С

		//**[1/3/2018 ConanSteve]**:	�ǳ���Ҫ����������ʵ�֣��漰��ϵͳͼ����ʾ��BitmapInfo�Ƿ���ȷ 
		m_arr_pGrabber[i]->GetParamInt(GBIsColorFilter, m_arr_GrabInfo[i].m_bIsColorFilter);//**[9/11/2017 ConanSteve]**:  �Ƿ��ǲ�ͼ
		m_arr_pGrabber[i]->GetParamInt(GBEnableFlipImage, m_arr_GrabInfo[i].m_iFlipType);//**[9/11/2017 ConanSteve]**:  �Ƿ���Ҫ��ת


		m_arr_pGrabber[i]->GetParamInt(GBExposureTime, m_arr_GrabInfo[i].m_iExposureTime);//**[9/11/2017 ConanSteve]**:  �ع�ʱ��
		m_arr_pGrabber[i]->GetParamInt(GBGain, m_arr_GrabInfo[i].m_iGain);//**[9/11/2017 ConanSteve]**:  ����
		m_arr_pGrabber[i]->GetParamInt(GBBayerType, m_arr_GrabInfo[i].m_iBayerType);//**[9/11/2017 ConanSteve]**:  Bayerת������


		sModuleName.Format(_T("Camera%d Parameters"), i + 1);

		int iWidth = m_arr_sCameraImageInfo[i].m_iCameraImageWidth = m_arr_GrabInfo[i].m_iImageWidth;
		int iHeight = m_arr_sCameraImageInfo[i].m_iCameraImageHeight = m_arr_GrabInfo[i].m_iImageHeight;
		m_arr_sCameraImageInfo[i].m_iCameraImageSize = m_arr_sCameraImageInfo[i].m_iCameraImagePixelSize*iWidth*iHeight;
		/*************************************<�������Сд��AlgConfig.ini�ļ�>************************************[8/30/2017 ConanSteve]*/



		m_arr_iCheckGroupSNOfCurGrab[i] = m_iniAlgConfig.GetIntEx(sModuleName, _T("CheckGroup"), 1, _T("��ǰ��������Ĺ�λ��"))-1;
		m_arr_iKickGroupSNOfCurGrab[i] = m_iniAlgConfig.GetIntEx(sModuleName, _T("KickGroup"), 1, _T("��ǰ����������޷϶���")) - 1;
		CExFn::WritePrivateProfileInt(sModuleName, _T("Width"), iWidth*m_arr_iSmallImgCntPerGrab[i], m_sPathFileOfAlgCfg, _T("ͼ����"));
		CExFn::WritePrivateProfileInt(sModuleName, _T("PictureNumberPerCamera"), m_arr_iSmallImgCntPerGrab[i], m_sPathFileOfAlgCfg, _T("ͼ������"));
		CExFn::WritePrivateProfileInt(sModuleName, _T("Height"), iHeight, m_sPathFileOfAlgCfg,_T("���ͼ��߶�"));
		CExFn::WritePrivateProfileInt(sModuleName, _T("CheckOutNum"), 1, m_sPathFileOfAlgCfg, _T("ÿ���㷨��ⷵ�ؽ������"));
		CExFn::WritePrivateProfileInt(sModuleName, _T("KickOffset"), 0, m_sPathFileOfAlgCfg, _T(""));
		int iCheckInSize{ 0 }, iCheckOutSize{ 0 }, iCheckStatusSize{0};
		switch (m_eProductLine)
		{
		case e_ProductLine_NO:
			break;
		case e_ProductLine_General:
		{
			iCheckInSize = sizeof(General::CAlgInspInputParam);
			iCheckOutSize = sizeof(General::CAlgInspResult);
			iCheckStatusSize = sizeof(s_Status);
		}
			break;
		case e_ProductLine_Stopper:
		{
			iCheckInSize = sizeof(Stopper::s_AlgDetectInput);
			iCheckOutSize = sizeof(Stopper::s_AlgDetectOutput);
			iCheckStatusSize = sizeof(s_Status);
		}
			break;
		case e_ProductLine_LampInsp:
		{
			iCheckInSize = sizeof(LampInsp::s_DetectInParam);
			iCheckOutSize = sizeof(LampInsp::s_DetectOutParam);
			iCheckStatusSize = sizeof(s_Status);
		}
			break;
		case e_ProductLine_LableCheck:
		{
			iCheckInSize = sizeof(LabelCheck::s_AlgInspInPara);
			iCheckOutSize = sizeof(LabelCheck::s_AlgInspResult);
			iCheckStatusSize = sizeof(LabelCheck::s_Status);
		}
			break;
		default:
			break;
		}
		//�����������ṹ���С
		CExFn::WritePrivateProfileInt(sModuleName, _T("CheckInSize"), iCheckInSize, m_sPathFileOfAlgCfg,_T("�㷨���ʱ�ṹ��s_CheckInParam�� void* pParam�Ĵ�С"));
		//����������ṹ���С
		CExFn::WritePrivateProfileInt(sModuleName, _T("CheckOutSize"), iCheckOutSize, m_sPathFileOfAlgCfg,_T("�㷨���ʱ�ṹ��s_CheckInParam�� void* pParam�Ĵ�С"));
		m_iniAlgConfig.WriteInt(sModuleName, _T("CheckStatusSize"), iCheckStatusSize, _T("�㷨���״̬�ṹ���е�void* pParam��С���ֽ�Ϊ��λ��"));
}
	ret.bRet = true;
	return ret;
}


CRetInfo CMainFrame::InitDataBase()
{
	CRetInfo ret;
	return ret;
}


bool CMainFrame::AllocateMemory()
{
	if(0!=m_iGrabberNum)
	{	
		if (m_bEnableInitGrabber)//m_bEnableDrawImg&&
		{
			/*************************************<Bayer�任��ز���>************************************[9/6/2017 ConanSteve]*/
			/*************************************<��ʼ��ϵͳ��ͼ�ڴ�>************************************[9/5/2017 ConanSteve]*/
			//m_p_bEnableFlipImg = make_unique<bool[]>(m_iGrabberNum);
			for (int i = 0; i < m_iGrabberNum; ++i)
			{
				m_arr_pbtBufferOfBayerImg[i] = make_unique < BYTE[]>(m_arr_GrabInfo[i].m_iImageHeight* m_arr_GrabInfo[i].m_iImageWidth*3);
				m_arr_pbtBufferOfGrabImageCopy[i] = make_unique < BYTE[]>(m_arr_GrabInfo[i].m_iImageHeight* m_arr_GrabInfo[i].m_iImageWidth * 3);
			}
			/*************************************<��ʼ��Bitmap��Ϣ>************************************[9/5/2017 ConanSteve]*/
			for (int i = 0; i < m_iGrabberNum; i++)
			{
				m_arr_pBitmapInfo[i] = shared_ptr<BITMAPINFO>((BITMAPINFO*)(new BYTE[sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD)]));
				m_arr_pBitmapInfo[i]->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
				m_arr_pBitmapInfo[i]->bmiHeader.biWidth = m_arr_sCameraImageInfo[i].m_iCameraImageWidth;
				m_arr_pBitmapInfo[i]->bmiHeader.biHeight = m_arr_sCameraImageInfo[i].m_iCameraImageHeight;
				m_arr_pBitmapInfo[i]->bmiHeader.biPlanes = 1;
				m_arr_pBitmapInfo[i]->bmiHeader.biBitCount = 8*(m_arr_GrabInfo[i].m_bIsColorFilter==1?3:1);
				m_arr_pBitmapInfo[i]->bmiHeader.biCompression = BI_RGB;
				m_arr_pBitmapInfo[i]->bmiHeader.biSizeImage = 0;
				m_arr_pBitmapInfo[i]->bmiHeader.biXPelsPerMeter = 0;
				m_arr_pBitmapInfo[i]->bmiHeader.biYPelsPerMeter = 0;
				m_arr_pBitmapInfo[i]->bmiHeader.biClrUsed = 0;
				m_arr_pBitmapInfo[i]->bmiHeader.biClrImportant = 0;

				//(2003-4-2)
				//����8λλͼ��˵������Ҫ��ɫ�塣����256ɫ��λͼ����ɫ������ĸ���λ256��
				for (int k = 0; k < 256; k++)
				{
					m_arr_pBitmapInfo[i]->bmiColors[k].rgbBlue = (BYTE)k;
					m_arr_pBitmapInfo[i]->bmiColors[k].rgbGreen = (BYTE)k;
					m_arr_pBitmapInfo[i]->bmiColors[k].rgbRed = (BYTE)k;
					m_arr_pBitmapInfo[i]->bmiColors[k].rgbReserved = 0;
				}
			}
		}
		
	}
	return true;
}


void CMainFrame::AreaGrabOverCallback(const s_GBSIGNALINFO* SignalInfo)
{
	if (SignalInfo && SignalInfo->Context)
	{
		CMainFrame* pMainFrame = (CMainFrame*)SignalInfo->Context;
		pMainFrame->AreaGrabOverCallbackFunc(SignalInfo);
	}
	
}

void CMainFrame::AreaGrabOverCallbackFunc(const s_GBSIGNALINFO* SigInfo)
{
	CString sLogInfo{ _T("") };
	if (SigInfo->iErrorCode != 0)//**[12/8/2017 ConanSteve]**:  ����ɼ��д�
	{
		CString sErrorInfo{ _T("") };
		sErrorInfo.Format(_T("����ɼ��ص�����\n������룺%d\n����������%s"), SigInfo->iErrorCode, SigInfo->strDescription);
		AddMsg(sErrorInfo, true, true);
		return;
	}
	int iGrabSN = SigInfo->iGrabberSN;
	++m_arr_iGrabCallbackCnt[iGrabSN];
	if (m_bEnableInitIOCard)
	{
		for (int i = 0; i < m_iIOCardNum; ++i)
		{
			m_arr_pIOCard[i]->NotifyCardOfGrabCallBack(s_NotifyInfoOfGrabCallback(SigInfo->iGrabberSN, m_arr_iGrabCallbackCnt[iGrabSN]));
		}
	}
	GetCount_TuXiangHao();//**[12/14/2017 ConanSteve]**:  ��ȡͼ��żĴ�������֤����
	sLogInfo.Format(_T("[�����:%2d][�ɼ�����:%8d][ͼ��֡��%8d][�Ĵ������ź�:%8d]"), SigInfo->iGrabberSN, m_arr_iGrabCallbackCnt[iGrabSN], SigInfo->iFrameCount, m_arr_iHardImageCountOfGrabInRegister[iGrabSN]);
	m_p_fileGrabCallback[SigInfo->iGrabberSN].Write(sLogInfo);
	if (!m_bIsDetecting)
	{
		return;
	}
	//**[11/21/2017 ConanSteve]**:  ֪ͨ�ӿڿ�����ص�
	if (iGrabSN < 0 || iGrabSN >= m_iGrabberNum)
	{
		AddMsg(tr("������кų������÷�Χ"));
		return;
	}
	CameraGrabFuncSingle(iGrabSN);
}

bool CMainFrame::InitLogSys()
{
	CString sPathOfLogFiles = m_sPathOfVirtualDisk + _T("\\LogFiles");
	CString sPathOfSysFolder = sPathOfLogFiles + _T("\\CheckSystemLogs");
	CString sPathFileOfCurFile{ _T("") };//**[8/31/2017 ConanSteve]**:  ��ǰ��־�ļ���ȫ·��
	CExFn::CreateMultiDirectory(sPathOfSysFolder);
	if (0 != m_iGrabberNum)
	{
		m_p_fileGrabCallback = unique_ptr<CLogFile[]>(new CLogFile[m_iGrabberNum]);
		m_p_fileImgCntOfGrabInRegister = make_unique<CLogFile[]>(m_iGrabberNum);
	}
	for (int i=0;i<m_iGrabberNum;++i)
	{
		sPathFileOfCurFile.Format(_T("\\����ص�%02d.txt"), i+1);
		sPathFileOfCurFile = m_sPathOfSysLogs + sPathFileOfCurFile;
		m_p_fileGrabCallback[i].Open(sPathFileOfCurFile, &m_bEnableLog);

		sPathFileOfCurFile.Format(_T("\\���ͼ��Ĵ���%02d.txt"), i + 1);
		sPathFileOfCurFile = m_sPathOfSysLogs + sPathFileOfCurFile;
		m_p_fileImgCntOfGrabInRegister[i].Open(sPathFileOfCurFile, &m_bEnableLog);
	}

	for (int i = 0; i < m_iKickQueueNum; ++i)
	{
		sPathFileOfCurFile.Format(_T("%s\\�޷ϼ������Ϣ%02d.txt"), m_sPathOfSysLogs, i+1);
		m_arr_fileKickCallback[i].Open(sPathFileOfCurFile);
	}
	return true;
}


bool CMainFrame::StartCheck()
{
	bool bRet{ false };
	m_bIsDetecting = true;
	if (m_bEnableInitGrabber)//**[12/12/2017 ConanSteve]**:  �����ʼ�������������������
	{
		for (int i = 0; i < m_iGrabberNum; ++i)
		{
			bRet = TRUE == m_arr_pGrabber[i]->StartGrab();
			if (!bRet)
				AfxMessageBox(tr("�����ʧ�ܣ�"));
		}
	}
	if (m_bEnableInitIOCard)//**[12/12/2017 ConanSteve]**:  �����ʼ���˽ӿڿ����������ӿڿ����ж���
	{
		for (int i = 0; i < m_iIOCardNum; ++i)
		{
			m_arr_pIOCard[i]->Enable(true);
		}
	}			
	return true;
}


bool CMainFrame::StopCheck()
{
	bool bRet{ false };
	if (m_bEnableInitIOCard)//**[12/12/2017 ConanSteve]**:  �����ʼ���˽ӿڿ����������ӿڿ����ж���
	{
		for (int i = 0; i < m_iIOCardNum; ++i)
		{
			m_arr_pIOCard[i]->Enable(false);
		}
	}
	if (m_bEnableInitGrabber)
	{
		for (int i = 0; i < m_iGrabberNum; ++i)
		{
// 			if (0 == i)
// 				continue;
			bRet = TRUE == m_arr_pGrabber[i]->StopGrab();
		}
	}
//	Sleep(200);
	m_bIsDetecting = false;
	return true;
}


bool CMainFrame::AddMsg(CString sMsg, bool bHint, bool bErrorInfo)
{
	m_fileDebugInfo.Write(sMsg);
	if (bErrorInfo)
		m_fileErrorInfo.Write(sMsg);
	s_HintMsg *pHintMsg = new s_HintMsg();
	pHintMsg->sMsg = sMsg;
	pHintMsg->bHint = bHint;
	SendMessage(WM_COMMAND, ID_INDICATOR_OPERATION, (LPARAM)pHintMsg);
	return false;
}


BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch (wParam)
	{
	case ID_INDICATOR_OPERATION://**[9/5/2017 ConanSteve]**:  ϵͳʵʱ������Ϣ
	{
		if (NULL != lParam)
		{
			s_HintMsg* pHintMsg = (s_HintMsg*)lParam;
			m_wndStatusBar.SetPaneText(0, pHintMsg->sMsg);
			if (pHintMsg->bHint)
				AfxMessageBox(pHintMsg->sMsg);
			delete pHintMsg;
		}
	}
	return TRUE;
	default:
		break;
	}
	return CFrameWnd::OnCommand(wParam, lParam);
}


bool CMainFrame::CameraGrabFuncSingle(int iGrabSN)
{
	int iAddrOfCamImg{0};//**[9/6/2017 ConanSteve]**:  
	int iImgSN = m_arr_iHardImageCountOfGrabInRegister[iGrabSN];
	BYTE* pImageBuffer{ NULL };
	bool bRet{ false };
	int iImageWidth = 0, iImageHeight = 0, iImageByte = 0, iImageSize = 0;
	iImageWidth = m_arr_sCameraImageInfo[iGrabSN].m_iCameraImageWidth;
	iImageHeight = m_arr_sCameraImageInfo[iGrabSN].m_iCameraImageHeight;
	iImageByte = m_arr_sCameraImageInfo[iGrabSN].m_iCameraImagePixelSize;
	iImageSize = m_arr_sCameraImageInfo[iGrabSN].m_iCameraImageSize;
	int iCheckGroupSN = m_arr_iCheckGroupSNOfCurGrab[iGrabSN];
	bRet = TRUE == m_arr_pGrabber[iGrabSN]->GetParamInt(GBImageBufferAddr, iAddrOfCamImg);
	ASSERT(bRet);
	pImageBuffer = (BYTE*)iAddrOfCamImg;
	ASSERT(NULL!= pImageBuffer);
	//memcpy_s(&m_pBufferOfCamImg[iGrabSN][0], m_pCameraImageInfo[iGrabSN].m_iCameraImageSize, pImageBuffer, m_pCameraImageInfo[iGrabSN].m_iCameraImageSize);//**[9/6/2017 ConanSteve]**:  ����ͼ��
	
	/*************************************<ϵͳ��ͼ>************************************[12/14/2017 ConanSteve]*/
//	return false;
	if ((m_bEnableDrawImg || e_SaveImgType_NO != m_iSaveImgType&& e_SaveImgObj_System == m_iSaveImgObj))
	{
		//**[1/26/2018 ConanSteve]**:	�����κ����������ͼ�񿽱� 
		memcpy_s(&m_arr_pbtBufferOfGrabImageCopy[iGrabSN][0], m_arr_GrabInfo[iGrabSN].m_iImageSize, pImageBuffer, m_arr_GrabInfo[iGrabSN].m_iImageSize);
		switch (m_arr_iGrabType[iGrabSN])
		{
		case e_GrabType::e_GrabType_SG:
		{
			memcpy_s(&m_arr_pbtBufferOfBayerImg[iGrabSN][0], m_arr_GrabInfo[iGrabSN].m_iImageSize, pImageBuffer, m_arr_GrabInfo[iGrabSN].m_iImageSize);
		}
		break;
		case e_GrabType::e_GrabType_AVT:
			break;
		case e_GrabType::e_GrabType_BAUMER:
			break;
		case e_GrabType::e_GrabType_JAI:
		{

			if (m_arr_BayerTransform[iGrabSN].m_bAreaCamBayer)
			{
				ConvertBayer2Rgb(&m_arr_pbtBufferOfBayerImg[iGrabSN][0],
					pImageBuffer,
					iImageWidth,
					iImageHeight,
					BAYER2RGB_NEIGHBOUR,
					&m_pLutR[0],
					&m_pLutG[0],
					&m_pLutB[0],
					FALSE,
					BAYER_RG);//**[1/3/2018 ConanSteve]**:	Ĭ��=4

			}
			else
			{
				memcpy_s(&m_arr_pbtBufferOfBayerImg[iGrabSN][0], iImageSize, pImageBuffer,
					iImageSize);
			}
			// �Բɼ���ͼ�����ˮƽ��ֱ����
			if (0 != m_arr_iSystemFilpType[iGrabSN])
			{
				MirrorSingleImage(&m_arr_pbtBufferOfBayerImg[iGrabSN][0],
					iImageWidth, iImageHeight, iImageByte * 8, (e_ShowModeFlip)m_arr_iSystemFilpType[iGrabSN]);
			}
			break;
		}
		case e_GrabType::e_GrabType_MER://**[12/12/2017 ConanSteve]**:  ˮ�����
		{
			if (TRUE == m_arr_GrabInfo[iGrabSN].m_bIsColorFilter)
			{
				int iRet = DxRaw8toRGB24(pImageBuffer, &m_arr_pbtBufferOfBayerImg[iGrabSN][0],
					m_arr_sCameraImageInfo[iGrabSN].m_iCameraImageWidth, m_arr_sCameraImageInfo[iGrabSN].m_iCameraImageHeight,
					RAW2RGB_NEIGHBOUR, (DX_PIXEL_COLOR_FILTER)m_arr_GrabInfo[iGrabSN].m_iBayerType, 0 != m_arr_GrabInfo[iGrabSN].m_iFlipType);
			}
			else
			{
				for (VxUint32 i = 0; i < iImageHeight; i++)
				{
					memcpy(&m_arr_pbtBufferOfBayerImg[iGrabSN][0] + i * iImageWidth, pImageBuffer + (iImageHeight - i - 1) * iImageWidth, (size_t)iImageWidth);
				}
			}
			break;
		}
		default:
			return false;
		}
		if (m_bEnableDrawImg)
			g_pView->DisplayGrabImg(iGrabSN);
	}
	/*************************************<�㷨���>************************************[12/14/2017 ConanSteve]*/
	if (m_bEnableInitMultiObjectAndCheck)//**[11/21/2017 ConanSteve]**:  ����򿪼��
	{

		s_CheckInParam sCheckInParam;
		General::CAlgInspInputParam p_Param_Gerneral;
		Stopper::s_AlgDetectInput p_Param_Stopper;
		LampInsp::s_DetectInParam p_Param_LampInsp;
		LabelCheck::s_AlgInspInPara p_Param_LabelCheck;
		switch (m_eProductLine)
		{
		case e_ProductLine_NO:
			return false;
		case e_ProductLine_General:
		{
			p_Param_Gerneral.iDrawImgType = m_iDrawImgType;
			p_Param_Gerneral.bExecuteAlgCheck = m_arr_bEnableExecCheckOfEachGroup[iCheckGroupSN];
			p_Param_Gerneral.iDisplayImgMode = m_iAlgDisplayMode;//**[1/17/2018 ConanSteve]**:  �㷨��ʾģʽ
			p_Param_Gerneral.iSaveImageMode = (e_SaveImgObj_Alg == m_iSaveImgObj) ? m_iSaveImgType : e_SaveImgType_NO;//**[1/2/2018 ConanSteve]**:  �����ͼ�������㷨��ͼ			
			p_Param_Gerneral.lpszSaveImagePath = &m_arr_lpszPathOfSaveImg[iGrabSN];

			int iFirstGrabSNInCurGroup = m_arr_iCheckGroupFirstGrabIndexInCamGroup[iGrabSN];//**[1/24/2018 ConanSteve]**:  ��ǰ������һ�������
			p_Param_Gerneral.p_iWidth = &m_arr_iWidth[0] + iFirstGrabSNInCurGroup;
			p_Param_Gerneral.p_iHeight = &m_arr_iHeight[0] + iFirstGrabSNInCurGroup;
			p_Param_Gerneral.p_iBayer = &m_arr_iHalconBayerType[0] + iFirstGrabSNInCurGroup;
			p_Param_Gerneral.p_iRevert = &m_arr_iHalconRevertType[0] + iFirstGrabSNInCurGroup;
			p_Param_Gerneral.p_iCount = &m_arr_iSmallImgCntPerGrab[0] + iFirstGrabSNInCurGroup;
			p_Param_Gerneral.p_iChannel = &m_arr_iChannel[0] + iFirstGrabSNInCurGroup;
			sCheckInParam.pParam = &p_Param_Gerneral;
		}
			break;
		case e_ProductLine_Stopper:
		{
			p_Param_Stopper.nInSig = 0;
			p_Param_Stopper.bRunCheck = 1;
			p_Param_Stopper.nImageWidth = m_arr_GrabInfo[iGrabSN].m_iImageWidth;
			p_Param_Stopper.nImageHeight = m_arr_GrabInfo[iGrabSN].m_iImageHeight;
			p_Param_Stopper.nChannel = m_arr_GrabInfo[iGrabSN].m_iImagePixelSize;
			p_Param_Stopper.nDisplayMode = 1;
			p_Param_Stopper.nSaveImageMode = 0;
			
			memset(p_Param_Stopper.chSaveImagePath, 0, sizeof(p_Param_Stopper.chSaveImagePath));
			sCheckInParam.pParam = &p_Param_Stopper;
		}
			break;
		case e_ProductLine_LampInsp:
		{
			p_Param_LampInsp.pImageInfo = m_p_CamInfo_LampInsp.get() + iCheckGroupSN;
			p_Param_LampInsp.pImageData = (int*)pImageBuffer;
			sCheckInParam.pParam = &p_Param_LampInsp;
		}
			break;
		case e_ProductLine_LableCheck:
		{
			p_Param_LabelCheck.bDsp = 1;
			p_Param_LabelCheck.bRunCheck = TRUE;
			p_Param_LabelCheck.iCircleIndex = m_arr_iHardImageCountOfGrabInRegister[iGrabSN];
			p_Param_LabelCheck.iCount = m_arr_iSmallImgCntPerGrab[iGrabSN];
			p_Param_LabelCheck.nCamIndex = iGrabSN;
			p_Param_LabelCheck.nChannel = m_arr_sCameraImageInfo[iGrabSN].m_iCameraImagePixelSize;
			//p_Param_LabelCheck.nChannel = m_arr_GrabInfo[iGrabSN].m_bIsColorFilter == 1 ? 3 : 1; //
			p_Param_LabelCheck.nDisplayMode = m_iAlgDisplayMode;
			p_Param_LabelCheck.nInSig = 0;

			//**[12/27/2017 ConanSteve]**:  �����ǰ��λ��ͼ�򿪣��Ҵ�ͼ���Ͳ�Ϊ����ͼ����Ϊ�㷨��ͼ
			p_Param_LabelCheck.nSaveImageMode = (e_SaveImgObj_Alg == m_iSaveImgObj) ? m_iSaveImgType : e_SaveImgType_NO;
			p_Param_LabelCheck.nHeight = iImageHeight;
			p_Param_LabelCheck.nWidth = iImageWidth;
			p_Param_LabelCheck.pcImageData = m_arr_bWhetherImageConvertedToMultiObj[iGrabSN]?
				(char*)&m_arr_pbtBufferOfBayerImg[iGrabSN][0]:(char*)pImageBuffer;
			p_Param_LabelCheck.bDisplaySys = 0;//**[12/14/2017 ConanSteve]**:  1��ʾϵͳͼ��0��ʾ�㷨ͼ��
			p_Param_LabelCheck.bDetectAlg = m_arr_bEnableExecCheckOfEachGroup[iCheckGroupSN];
			p_Param_LabelCheck.nDispFirstorLast = 2;
			p_Param_LabelCheck.iRunMode = e_CallType_RealTime;
			sCheckInParam.pParam = &p_Param_LabelCheck;
		}
			break;
		default:
			break;
		}
		if (m_arr_bWhetherImageConvertedToMultiObj[iGrabSN])
		{
			sCheckInParam.pImageData = (char*)&m_arr_pbtBufferOfBayerImg[iGrabSN][0];
		}
		else
		{
			//sCheckInParam.pImageData = (char*)pImageBuffer;
			//**[1/24/2018 ConanSteve]**:  ��ʹ�������ͼ���ַ����ʹ�ÿ��ٵ�ͼ���ڴ�ռ䣬��ֹchecksystemconfig.ini�����ļ�����
			//**[1/24/2018 ConanSteve]**:  ���¶���󿽱�ͼ���ַԽ�硣
			sCheckInParam.pImageData = (char*)&m_arr_pbtBufferOfGrabImageCopy[iGrabSN][0];
		}
		sCheckInParam.iImageSN = iImgSN;
		sCheckInParam.iCheckSN = iGrabSN;

 		m_arr_ExtenInfo[iCheckGroupSN].push_back(COptionalFunc(e_Fn_CheckInParam_SmallImageNumOfEachGrab).SetInt(m_arr_iSmallImgCntPerGrab[iCheckGroupSN]));
 		m_arr_ExtenInfo[iCheckGroupSN].push_back(COptionalFunc(e_Fn_CheckInParam_ImageWidth).SetInt(m_arr_sCameraImageInfo[iCheckGroupSN].m_iCameraImageWidth));
 		m_arr_ExtenInfo[iCheckGroupSN].push_back(COptionalFunc(e_Fn_CheckInParam_ImageHeight).SetInt(m_arr_sCameraImageInfo[iCheckGroupSN].m_iCameraImageHeight));
 		m_arr_ExtenInfo[iCheckGroupSN].push_back(COptionalFunc(e_Fn_CheckInParam_ImageChannel).SetInt(m_arr_sCameraImageInfo[iCheckGroupSN].m_iCameraImagePixelSize));
 		m_arr_ExtenInfo[iCheckGroupSN].push_back(COptionalFunc(e_Fn_CheckInParam_WhetherCicleSaveImage).SetBool(m_bEnableCircleSaveImg));//*[7/10/2017 ConanSteve]*:  1��ʾѭ����ͼ��0��ʾ��ѭ����ͼ
 		m_arr_ExtenInfo[iCheckGroupSN].push_back(COptionalFunc(e_Fn_CheckInParam_SaveImagePath).SetPointer((void*)&m_arr_lpszPathOfSaveImg[iGrabSN]));
		m_arr_ExtenInfo[iCheckGroupSN].push_back(e_Fn_CheckInParam_GrabNumPerCheckGroup).SetInt(m_arr_veciGrabSNOfEachCheckGroup[iCheckGroupSN].size());
// 		m_arr_MultipleObjSaveImgPara[iCheckGroupSN].SetImgNum(m_arr_iSmallImgCntPerGrab[iCheckGroupSN]);
// 		m_arr_MultipleObjSaveImgPara[iCheckGroupSN].SetImgWidth(m_arr_sCameraImageInfo[iCheckGroupSN].m_iCameraImageWidth);
// 		m_arr_MultipleObjSaveImgPara[iCheckGroupSN].SetImgHeight(m_arr_sCameraImageInfo[iCheckGroupSN].m_iCameraImageHeight);
// 		m_arr_MultipleObjSaveImgPara[iCheckGroupSN].SetImgChannel(m_arr_sCameraImageInfo[iCheckGroupSN].m_iCameraImagePixelSize);
// 		m_arr_MultipleObjSaveImgPara[iCheckGroupSN].SetIsCirCleSaveImg(m_bEnableCircleSaveImg);//*[7/10/2017 ConanSteve]*:  1��ʾѭ����ͼ��0��ʾ��ѭ����ͼ
// 		m_arr_MultipleObjSaveImgPara[iCheckGroupSN].SetSaveImgFolderPath(m_arr_lpszPathOfSaveImg[iGrabSN], MAX_PATH);

		if (e_SaveImgType_NO != m_iSaveImgType || e_SaveImgObj_System != m_iSaveImgObj)
			sCheckInParam.iReserve4 = (int)&m_arr_ExtenInfo[iCheckGroupSN];
		else
			sCheckInParam.iReserve4 = 0;

		TCHAR(*lpszPath)[MAX_PATH] = (TCHAR(*)[MAX_PATH]) m_arr_ExtenInfo[iCheckGroupSN].GetFuncStruct(e_Fn_CheckInParam_SaveImagePath).GetPointer();
		CString sTest = lpszPath[0];
		sCheckInParam.iReserve1 = e_SysDetectMode_Single;
		m_multiObject.Detect(sCheckInParam);

	}
	
	/*************************************<ϵͳ��ͼ>************************************[12/14/2017 ConanSteve]*/  
	if (e_SaveImgType_NO != m_iSaveImgType&& e_SaveImgObj_System == m_iSaveImgObj&&
		m_p_iCurNumOfSaveImgOfEachGrab[iGrabSN]<m_iMaxNumberOfSaveImg && 
		m_arr_bEnableSaveImgOfEachCheckGroup[iCheckGroupSN])
	{
		CString sPathFileOfBMPImg{ _T("") };
		sPathFileOfBMPImg.Format(_T("%s\\SysImg_%08d.bmp"), m_arr_lpszPathOfSaveImg[iGrabSN], iImgSN);
		SaveBmp(sPathFileOfBMPImg, m_arr_sCameraImageInfo[iCheckGroupSN].m_iCameraImageWidth,
			m_arr_sCameraImageInfo[iCheckGroupSN].m_iCameraImageHeight,
			m_arr_sCameraImageInfo[iCheckGroupSN].m_iCameraImagePixelSize,
			&m_arr_pbtBufferOfBayerImg[iGrabSN][0]);
		++m_p_iCurNumOfSaveImgOfEachGrab[iGrabSN];
	}
	return true;
}


bool CMainFrame::WriteCfg()
{
	/*************************************<��ϵͳ����������ļ�������ȥ>************************************[9/6/2017 ConanSteve]*/
	//CopyFile(m_sPathFileOfCheckSysCfgInConfig, m_sPathFileOfCheckSysCfgInModelInfo, FALSE);//**[9/1/2017 ConanSteve]**:  ǿ�Ƹ���
	for (int i = 0; i < m_iGrabberNum; ++i)
	{
		//CopyFile(m_arr_sPathFileOfGrabInitInConfig[i], m_arr_sPathFileOfGrabInitInModelInfo[i], FALSE);//**[9/1/2017 ConanSteve]**:  ǿ�Ƹ���
	}
	return false;
}


bool CMainFrame::OnBtnClickOpenDemo()
{

	if (!m_threeTierRights.VerifyRights(e_UserType_Debugger))
		return false;
	//CString sCmd = _T("notepad.exe ") + m_sPathFileOfDebugInfo;
	CString sCmd{ _T("") };
#ifdef DEBUG
	sCmd.Format(_T("%s\\DemoD.exe Save:Y"), m_sPathOfApp);
#else
	sCmd.Format(_T("%s\\Demo.exe Save:Y"), m_sPathOfApp);
#endif // DEBUG


#ifdef UNICODE
	WinExec(CStringConvert::UCS2_ANSI(sCmd.GetBuffer()).c_str(), SW_NORMAL);
#else
	WinExec(sCmd, SW_NORMAL);
#endif
	return false;
}


bool CMainFrame::OnBnClickOpenParamSet()
{
	m_pdlgParamSet = make_shared<CDlgParamSet>();
	m_pdlgParamSet->Create(IDD_DIALOG_ParamSet, NULL);
	//m_pdlgParamSet->ShowWindow(SW_SHOWMAXIMIZED);
	m_pdlgParamSet->ShowWindow(SW_SHOW);
	return false;
}


bool CMainFrame::ReleaseResource()
{
	if(m_bEnableInitGrabber)
	{
		for (int i = 0; i < m_iGrabberNum; ++i)
		{
			m_arr_pGrabber[i]->Close();
		}
	}
	if (m_bEnableInitIOCard)
	{
		for (int i = 0; i < m_iIOCardNum; ++i)
		{
			m_arr_pIOCard[i]->CloseCard();
		}
	}
	if (m_bEnableInitMultiObjectAndCheck)//**[11/21/2017 ConanSteve]**:  ����򿪶������
	{
		m_multiObject.Free();
	}
	return true;
}




bool CMainFrame::JudgeGrabCircleNew(int  iCameraSN,
	int  iAllElementSN,
	int& iGrabCircle,
	int& iPicSN)
{
	return true;
}


CRetInfo CMainFrame::InitMultiObject()
{
	CRetInfo ret;
	/*************************************<��Ҫ������ʼ��>************************************[11/23/2017 ConanSteve]*/
	if (!m_bEnableInitMultiObjectAndCheck)
	{
		m_fileDebugInfo.Write(_T("���棺������ʼ��������"));
		ret.bRet = true;
		return ret;
	}
	/*************************************<>************************************[11/16/2017 ConanSteve]*/
	unique_ptr<HWND[]> p_arrCamImg = make_unique<HWND[]>(m_iGrabberNum);
	unique_ptr<HWND[]> p_arrAlgImg = make_unique<HWND[]>(m_iGrabberNum);
	//**[11/27/2017 ConanSteve]**:  ͳ��ÿ���޷϶��еļ������
	{
		unique_ptr<bool[]> p_bHasBeenSetOfCheckGroupForFirstGrabIndex = make_unique<bool[]>(m_iCheckGroupNum);//**[11/27/2017 ConanSteve]**:  ÿ�������ĵ�һ������������Ƿ�������
		
		memset(p_bHasBeenSetOfCheckGroupForFirstGrabIndex.get(), 0, sizeof(bool)*m_iCheckGroupNum);
		//memset(p_bHasBeenSetOfCheckGroupForKickQueue.get(), 0, sizeof(bool)*m_iCheckGroupNum);
		
		for (int iGrabIndex = 0; iGrabIndex < m_iGrabberNum; ++iGrabIndex)
		{
			CString sModuleName{ _T("") };
			sModuleName.Format(_T("Camera%d Parameters"), iGrabIndex + 1);
			int iCheckGroupSN = m_iniAlgConfig.GetIntEx(sModuleName, _T("CheckGroup"))-1;
			int iKickQueueSN = m_iniAlgConfig.GetIntEx(sModuleName, _T("KickGroup"))-1;
			if (iCheckGroupSN<0 || iCheckGroupSN>m_iCheckGroupNum)
			{
				ret.bRet = false;
				ret.sErrorInfo = tr("�������������ȷ��������ȷ�������ļ���");
				return ret;
			}
			if(iKickQueueSN<0||iKickQueueSN>m_iKickQueueNum)
			{
				ret.bRet = false;
				ret.sErrorInfo = tr("�޷϶�����������ȷ��������ȷ�������ļ���");
				return ret;
			}
			m_arr_veciGrabSNOfEachCheckGroup[iCheckGroupSN].push_back(iGrabIndex);

			
		}
	}
	/*************************************<ͨ���㷨>************************************[11/16/2017 ConanSteve]*/
	unique_ptr<General::CAlgSingleInitParam[]> p_arrParam_General = make_unique<General::CAlgSingleInitParam[]>(m_iCheckGroupNum);//**[11/16/2017 ConanSteve]**:  ͨ����ϵͳ�ṹ��
	/*************************************<�����㷨>************************************[11/16/2017 ConanSteve]*/
	unique_ptr<Stopper::s_AlgInitParam[]> p_arrParam_Stopper = make_unique<Stopper::s_AlgInitParam[]>(m_iCheckGroupNum);//**[11/16/2017 ConanSteve]**:  ����ϵͳ����
	CDC* pDC[31];//**[11/16/2017 ConanSteve]**:  ����/��Ѫ��,���֧��30�����
	memset(pDC, 0, sizeof(pDC));
	/*************************************<�Ƽ���㷨>************************************[11/24/2017 ConanSteve]*/
	unique_ptr<LampInsp::s_DetectInitParam[]> p_arrParam_LampInsp = make_unique<LampInsp::s_DetectInitParam[]>(m_iCheckGroupNum);
	unique_ptr<LampInsp::s_DetectInitParam*[]> pp_arrParam_LampInsp = make_unique<LampInsp::s_DetectInitParam*[]>(m_iCheckGroupNum);
	int pWnd[50];
	/*************************************<��Ѫ���㷨>************************************[12/12/2017 ConanSteve]*/
	unique_ptr<LabelCheck::s_AlgSingleInit[]> p_arrParam_LabelCheck = make_unique<LabelCheck::s_AlgSingleInit[]>(m_iCheckGroupNum);
	int pAddr_Label[31];
	int pAddr_General[51];
	memset(pAddr_Label, 0, sizeof(pAddr_Label));
	/*************************************<���ô��ھ��>************************************[11/16/2017 ConanSteve]*/
	for (int i = 0; i < m_iGrabberNum; ++i)
	{
		//**[12/14/2017 ConanSteve]**:  ͨ���㷨
		p_arrCamImg[i] = g_pView->m_pdlgRunning->GetDlgItem(ID_STATIC_CAMERA_IMAGE+i)->GetSafeHwnd();
		p_arrAlgImg[i] = g_pView->m_pdlgRunning->GetDlgItem(ID_STATIC_ALGORITHM_IMAGE + i)->GetSafeHwnd();//**[11/21/2017 ConanSteve]**:  GetDlgItem(ID_STATIC_ALGORITHM_IMAGE + i)->
		m_arr_iWidth[i] = m_arr_GrabInfo[i].m_iImageWidth;
		m_arr_iHeight[i] = m_arr_GrabInfo[i].m_iImageHeight;
		m_arr_iHalconBayerType[i] =m_arr_sCameraImageInfo[i].m_iBayerType;
		m_arr_iHalconRevertType[i] = m_arr_sCameraImageInfo[i].m_iFlipType;
		m_arr_iChannel[i] = m_arr_GrabInfo[i].m_bIsColorFilter ? 3 : 1;

		//�����㷨
		pDC[i] = (CDC*)p_arrAlgImg[i];

		//**[11/24/2017 ConanSteve]**:  �Ƽ���㷨
		pWnd[i] = (int)p_arrAlgImg[i];

		//**[12/14/2017 ConanSteve]**:  ��Ѫ���㷨

	}
	/*************************************<����˽�нṹ�����>************************************[11/16/2017 ConanSteve]*/
	for (int i = 0; i < m_iCheckGroupNum; ++i)
	{
		//**[11/16/2017 ConanSteve]**:  ͨ���㷨
		if(m_arr_veciGrabSNOfEachCheckGroup[i].size()>0)//**[11/27/2017 ConanSteve]**:  ��ֹ�û����ô��󣬵���ĳ�������û�����
			m_arr_iCheckGroupFirstGrabIndexInCamGroup[i] = m_arr_veciGrabSNOfEachCheckGroup[i][0];
		else
		{
			ret.bRet = false;
			ret.sErrorInfo.Format(_T("��λ(�����)%d�����Ϊ0�����������ļ���"), i+1);
			return ret;
		}
		int iFirstGrabSNInCurGroup = m_arr_iCheckGroupFirstGrabIndexInCamGroup[i];//**[1/24/2018 ConanSteve]**:  ��ǰ������һ�������
		p_arrParam_General[i].iStation = i;
		p_arrParam_General[i].iWindowNum = m_arr_veciGrabSNOfEachCheckGroup[i].size();
		p_arrParam_General[i].iCallType = e_CallType_RealTime;
		p_arrParam_General[i].p_iWidth = &m_arr_iWidth[0] + iFirstGrabSNInCurGroup;
		p_arrParam_General[i].p_iHeight = &m_arr_iHeight[0] + iFirstGrabSNInCurGroup;
		p_arrParam_General[i].p_iBayer = &m_arr_iHalconBayerType[0] + iFirstGrabSNInCurGroup;
		p_arrParam_General[i].p_iRevert = &m_arr_iHalconRevertType[0] + iFirstGrabSNInCurGroup;
		p_arrParam_General[i].p_iCount = &m_arr_iSmallImgCntPerGrab[0]+iFirstGrabSNInCurGroup;
		p_arrParam_General[i].p_iChannel = &m_arr_iChannel[0]+iFirstGrabSNInCurGroup;
		p_arrParam_General[i].hWndCamImg = p_arrCamImg.get() + i;
		p_arrParam_General[i].hWndAlgImg = p_arrAlgImg.get() + i;
		pAddr_General[i] = (int)&p_arrParam_General[i];

		//**[11/16/2017 ConanSteve]**:�����㷨
		p_arrParam_Stopper[i].pDC = pDC;
		p_arrParam_Stopper[i].nBayer[0] = m_arr_GrabInfo[i].m_iBayerType;
		p_arrParam_Stopper[i].nRevert[0] = m_arr_GrabInfo[i].m_iFlipType;

		//**[11/24/2017 ConanSteve]**:  �Ƽ���㷨
		p_arrParam_LampInsp[i].pHWnd = pWnd;
		p_arrParam_LampInsp[i].iRunMode = 0;//**[11/24/2017 ConanSteve]**:  ���߼��
		p_arrParam_LampInsp[i].iAlgSN = i+1;
		pp_arrParam_LampInsp[i] = &p_arrParam_LampInsp[i];

		//**[12/12/2017 ConanSteve]**:  ��Ѫ���㷨
		p_arrParam_LabelCheck[i].pDC = pDC;
		p_arrParam_LabelCheck[i].nCameraIndex = iFirstGrabSNInCurGroup;
		p_arrParam_LabelCheck[i].nWidth = m_arr_sCameraImageInfo[iFirstGrabSNInCurGroup].m_iCameraImageWidth;
		p_arrParam_LabelCheck[i].nHeight = m_arr_sCameraImageInfo[iFirstGrabSNInCurGroup].m_iCameraImageHeight;
		if (m_arr_bWhetherImageConvertedToMultiObj[i])
		{
			p_arrParam_LabelCheck[i].nBayer= 0;
		}
		else
		{
			p_arrParam_LabelCheck[i].nBayer = m_arr_sCameraImageInfo[iFirstGrabSNInCurGroup].m_iBayerType;
		}
		p_arrParam_LabelCheck[i].nRevert = m_arr_sCameraImageInfo[iFirstGrabSNInCurGroup].m_iFlipType;
		pAddr_Label[i] = (int)&p_arrParam_LabelCheck[i];
	}
	/*************************************<��ʼ���㷨��>************************************[11/16/2017 ConanSteve]*/
	s_CheckInitParam sInit;
	switch (m_eProductLine)
	{
	case e_ProductLine_NO:
		m_fileErrorInfo.Write(tr("������ʼ��ʧ��,��Ʒ�߲���Ϊδ���ã�"));
		ret.bRet = false;
		ret.sErrorInfo = tr("������ʼ��ʧ��,��Ʒ�߲���Ϊδ���ã�");
		return ret;
		break;
	case e_ProductLine_General:
	{
		sInit.pParam = (void*)pAddr_General;
// 		void* ppp = (void*)pAddr_General;
// 		sInit.pParam = (void*)p_arrParam_General.get();
		break;
	}
	case e_ProductLine_Stopper:
		sInit.pParam = p_arrParam_Stopper.get();
		break;
	case e_ProductLine_LampInsp:
		sInit.pParam = pp_arrParam_LampInsp.get();

		m_p_CamInfo_LampInsp = make_unique<LampInsp::s_CheckCameraInfo[]>(m_iCheckGroupNum);
		for (int i = 0; i < m_iCheckGroupNum; ++i)
		{
			m_p_CamInfo_LampInsp[i].iBitCount = m_arr_sCameraImageInfo[i].m_iCameraImagePixelSize*8;
			m_p_CamInfo_LampInsp[i].iCount = m_arr_iSmallImgCntPerGrab[i];
			m_p_CamInfo_LampInsp[i].iHeight = m_arr_sCameraImageInfo[i].m_iCameraImageHeight;
			m_p_CamInfo_LampInsp[i].iWidth = m_arr_sCameraImageInfo[i].m_iCameraImageWidth;
			m_p_CamInfo_LampInsp[i].iType = 1;
		}
		break;
	case e_ProductLine_LableCheck:
	{
		sInit.pParam = (void*)pAddr_Label;
	}
		break;
	default:
		break;
	}
	sInit.KickCallbackFunc = GlobalKickCallbackFunc;
	sInit.StatusCallbackFunc = GlobalStatusCallbackFunc;
	sInit.pFilePath = m_sPathFileOfAlgCfg.GetBuffer();
	sInit.pThis = this;
	sInit.iCallType = e_CallType_RealTime;
	sInit.iReserve1 = e_MachineType_Liquor;

	s_Status sStatus;
	if (m_bEnableInitMultiObjectAndCheck&&0<m_iCheckGroupNum)
	{
		sStatus = m_multiObject.Init(sInit);
		if (STATUS_NOR != sStatus.iStatusType)
		{
			ret.bRet = false;
			ret.sErrorInfo = sStatus.pErrorInfo;
			return ret;
		}
	}

	LoadModel(m_sModelName);
	/*************************************<>************************************[11/22/2017 ConanSteve]*/
	ret.bRet = true;
	return ret;
}

void CMainFrame::GlobalKickCallbackFunc(const s_KickParam * pKickParam)
{
	if (pKickParam && pKickParam->pThis)
	{
		CMainFrame* pMF = (CMainFrame*)pKickParam->pThis;
		pMF->KickCallbackFunc1(pKickParam);
	}
}

void CMainFrame::KickCallbackFunc1(const s_KickParam * pKickParam)
{
	int iKickSN = pKickParam->iKickSN;
	int iImgSN = pKickParam->iImageSN;
	int iCheckRsltNum = m_arr_veciCheckSNInKickQueue[iKickSN].size();//**[11/27/2017 ConanSteve]**:  ��ǰ�޷�����������Ŀ
	unique_ptr<General::CAlgInspResult[]> p_sOut_General = make_unique<General::CAlgInspResult[]>(iCheckRsltNum);
	unique_ptr<Stopper::s_AlgDetectOutput []> p_sOut_Stopper = make_unique<Stopper::s_AlgDetectOutput[]>(iCheckRsltNum);
	unique_ptr<LampInsp::s_DetectOutParam[]> p_sOut_LampInsp = make_unique<LampInsp::s_DetectOutParam[]>(iCheckRsltNum);
	unique_ptr<LabelCheck::s_AlgInspResult[]> p_sOut_LabelCheck = make_unique<LabelCheck::s_AlgInspResult[]>(iCheckRsltNum);
	s_sRsltInfoWritten sRstlInfoWritten;//**[11/27/2017 ConanSteve]**:  д�ӿڿ���Ϣ
	unique_ptr<bool[]> p_bKick{ nullptr };//**[11/27/2017 ConanSteve]**:  �Ƿ��޷�
	sRstlInfoWritten.iImgSN = iImgSN;
	sRstlInfoWritten.iKickSN = iKickSN;
	sRstlInfoWritten.iRsltNum = 0;
	CString sLogInfo{ _T("") }, sTemp{ _T("") };
	sLogInfo.Format(_T("[�޷Ϻ�:%2d][ͼ���:%8d]["), iKickSN, iImgSN);
	switch (m_eProductLine)
	{
	case e_ProductLine_NO:
		return;
	case e_ProductLine_General:
	{
		memcpy_s(p_sOut_General.get(), sizeof(General::CAlgInspResult)*iCheckRsltNum, pKickParam->pParam, sizeof(General::CAlgInspResult)*iCheckRsltNum);
		//**[12/4/2017 ConanSteve]**:����ÿ�������ļ����
		bool bEmpty{ false };//**[12/4/2017 ConanSteve]**:  �ղ�Ʒ
		bool bKickValueFromAlgStatus{ false };//**[12/14/2017 ConanSteve]**:  ÿ���������Ӧ���޷Ͻ��
		for (int i = 0; i < iCheckRsltNum; ++i)  
		{
			//**[12/4/2017 ConanSteve]**:  ��ѯ��ǰ������ĵļ�����

			int iCheckSN = m_arr_veciCheckSNInKickQueue[iKickSN][i];
			if (1 == p_sOut_General[i].iRsltNum)
				p_sOut_General[i].arrAlgStatus[0] = p_sOut_General[i].iAlgStatus;
			for (int j = 0; j < p_sOut_General[i].iRsltNum; ++j)
			{
				if (nullptr == p_bKick)
				{
					p_bKick = make_unique<bool[]>(p_sOut_General[0].iRsltNum);
					sRstlInfoWritten.iRsltNum = p_sOut_General[0].iRsltNum;
				}
				int iAlgStatus = p_sOut_General[i].arrAlgStatus[j];
				sTemp.Format(_T("%d-"), iAlgStatus);
				sLogInfo += sTemp;
				switch (iAlgStatus)
				{
				case e_AlgStatus_Good:
				{
					++m_arr_iCheckCntOfCheckGroup[iCheckSN];
					++m_arr_iGoodCntOfCheckGroup[iCheckSN];
					++m_arr_iGoodCntOfKickQueue[iKickSN];
					bKickValueFromAlgStatus = false;					
					bEmpty = false;
					break;
				}
				case e_AlgStatus_AlgException:
				{
					bKickValueFromAlgStatus = true;
					break;
				}
				case e_AlgStatus_Empty:
				{
					bKickValueFromAlgStatus = m_bEnableEmptyKick;
					bEmpty = true;
					break;
				}
				case e_AlgStatus_Bad:
				{
					bKickValueFromAlgStatus = true;
					++m_arr_iCheckCntOfCheckGroup[iCheckSN];
					++m_arr_iBadCntOfCheckGroup[iCheckSN];
					bEmpty = false;
					break;
				}
				case e_AlgStatus_LostImg:
				{
					bKickValueFromAlgStatus = bKickValueFromAlgStatus;
					break;
				}
				case e_AlgStatus_Overtime:
				{
					bKickValueFromAlgStatus = true;
				}
					break;
				default:
					break;
				}
				
				sRstlInfoWritten.arr_bKick[j] |= bKickValueFromAlgStatus;
				
			}

			//**[12/5/2017 ConanSteve]**:  ��ȡȱ������ͳ����Ϣ
			for (int iDefectIndex = 0; iDefectIndex < p_sOut_General[i].iLenOfDefect;++iDefectIndex)
			{
				int iDefectTypeSN = p_sOut_General[i].arrHeadOfDefectDesc[iDefectIndex];
				auto itera_find = m_arr_mapDefectStatistics[iCheckSN].find(iDefectTypeSN);
				if (itera_find != m_arr_mapDefectStatistics[iCheckSN].end())//**[12/4/2017 ConanSteve]**:  �����ǰ���ؽ�����ϴ������ļ��е�ȱ����������
				{
					itera_find->second.iCnt++;//**[12/4/2017 ConanSteve]**:  ȱ��ͳ�Ƽ�����1
				}
			}
		}
		//**[12/4/2017 ConanSteve]**:  ���������ǿ����޷���Ϊ0
		if (!bEmpty&& e_CheckTotleCntType_Rlst == m_iCheckTotleCntType&& 0 == iKickSN)
		{
			m_iCheckCntTotle++;
		}
	}
		break;
	case e_ProductLine_Stopper:
		break;
	case e_ProductLine_LampInsp:
		break;
	case e_ProductLine_LableCheck:
	{
		if (e_KickType_Single != m_arr_eKickType[iKickSN])//**[12/14/2017 ConanSteve]**:  ��Ѫ���㷨��֧�ֶ�·�޷�
			return;
		memcpy_s(p_sOut_LabelCheck.get(), sizeof(LabelCheck::s_AlgInspResult)*iCheckRsltNum, pKickParam->pParam, sizeof(LabelCheck::s_AlgInspResult)*iCheckRsltNum);
		sRstlInfoWritten.iRsltNum = 1;
		bool bKick{ false };
		for (int i = 0; i < iCheckRsltNum; ++i)
		{
			int iCheckSN = m_arr_veciCheckSNInKickQueue[iKickSN][i];
			e_AlgStatus eAlgStatus = (e_AlgStatus)(p_sOut_LabelCheck[i].nAlgStatus);
			//**[12/28/2017 ConanSteve]**:  �����
			if (e_AlgStatus_Good == eAlgStatus || e_AlgStatus_Bad == eAlgStatus || e_AlgStatus_AlgException)
			{
				m_arr_iCheckCntOfCheckGroup[iCheckSN]++;//**[12/28/2017 ConanSteve]**:  ��Ʒ����
			}
			if (e_AlgStatus_Bad == eAlgStatus || e_AlgStatus_AlgException == eAlgStatus)
			{
				m_arr_iBadCntOfCheckGroup[iCheckSN]++;//**[12/28/2017 ConanSteve]**:  ��Ʒ����
			}

			//**[12/28/2017 ConanSteve]**:  ���޷Ͻ��
			if (e_AlgStatus_Good != eAlgStatus)
			{
				bKick = true;
			}
			else
			{
				bKick = false;
			}
			sTemp.Format(_T("%2d-"), eAlgStatus);
			sLogInfo += sTemp;
			sRstlInfoWritten.arr_bKick[0] |= bKick;
		}
	}
		break;
	default:
		break;
	}
	sLogInfo += _T("]");

	//**[1/26/2018 ConanSteve]**:	�����ִ���߷ϣ������ж�Ϊfalse 
	if (!m_arr_bEnableKickOfKickQueue[iKickSN])
	{
		fill_n(sRstlInfoWritten.arr_bKick, 30, false);
	}
	m_arr_fileKickCallback[iKickSN].Write(sLogInfo);
	if (m_bEnableInitIOCard)
	{
		for (int i = 0; i < m_iIOCardNum; ++i)
		{
			if (!m_arr_pIOCard[i]->WriteKickResult(sRstlInfoWritten))
			{
				m_fileErrorInfo.Write(m_arr_pIOCard[i]->GetLastError());
			}
		}
	}
}

void CMainFrame::GlobalStatusCallbackFunc(const s_Status * sStatusParam)
{
}

void CMainFrame::StatusCallbackFunc1(const s_Status * sStatusParam)
{
}

int CMainFrame::SaveBmp(const TCHAR* pPathName, int iWidth, int iHeight, int iByte, TBYTE *pDest)
{
	BITMAPFILEHEADER bmFH;
	int nPaletteEntries = 256;
	DWORD dwSize = iWidth*iHeight*iByte;

	if (iByte < 3)
	{
		nPaletteEntries = 256;
	}
	else
	{
		nPaletteEntries = 0;
	}

	bmFH.bfType = 19778;
	//	bmFH.bfType = "BM";

	bmFH.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
		sizeof(BITMAPINFOHEADER) +
		nPaletteEntries * sizeof(RGBQUAD)) +
		dwSize;
	bmFH.bfReserved1 = bmFH.bfReserved2 = 0;
	bmFH.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) +
		sizeof(BITMAPINFOHEADER) +
		nPaletteEntries * sizeof(RGBQUAD);

	BITMAPINFO *FileBMP;
	char chBmpBuf[2048];
	FileBMP = (BITMAPINFO *)chBmpBuf;
	FileBMP->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	FileBMP->bmiHeader.biWidth = iWidth;
	FileBMP->bmiHeader.biHeight = iHeight;
	FileBMP->bmiHeader.biPlanes = 1;
	FileBMP->bmiHeader.biBitCount = iByte * 8;
	FileBMP->bmiHeader.biCompression = BI_RGB;
	FileBMP->bmiHeader.biSizeImage = 0;
	FileBMP->bmiHeader.biXPelsPerMeter = 0;
	FileBMP->bmiHeader.biYPelsPerMeter = 0;
	FileBMP->bmiHeader.biClrUsed = 0;
	FileBMP->bmiHeader.biClrImportant = 0;

	//(2003-4-2)
	//����8λλͼ��˵������Ҫ��ɫ�塣����256ɫ��λͼ����ɫ������ĸ���λ256��
	for (int k = 0; k < 256; k++)
	{
		FileBMP->bmiColors[k].rgbBlue = (BYTE)k;
		FileBMP->bmiColors[k].rgbGreen = (BYTE)k;
		FileBMP->bmiColors[k].rgbRed = (BYTE)k;
		FileBMP->bmiColors[k].rgbReserved = 0;
	}


	CFile ImageFile;

	if (ImageFile.Open(pPathName, CFile::modeCreate | CFile::modeReadWrite))
	{
		ImageFile.Write(&bmFH, sizeof(BITMAPFILEHEADER));
		ImageFile.Write(&FileBMP->bmiHeader, sizeof(BITMAPINFOHEADER));
		ImageFile.Write(&FileBMP->bmiColors[0], sizeof(RGBQUAD) * nPaletteEntries);
		ImageFile.Write(pDest, dwSize);
		ImageFile.Close();
		return 1;
	}
	else
		return -1;

}


e_BayerHalcon CMainFrame::TranslateToHalconBayer(int iBayerType)
{
	e_BayerHalcon eRet = e_BayerHalcon_NONE;
	switch (iBayerType)
	{
	case DX_PIXEL_COLOR_FILTER::NONE:
		eRet = e_BayerHalcon_NONE;
		break;
	case DX_PIXEL_COLOR_FILTER::BAYERRG:
		eRet = e_BayerHalcon_RG;
		break;
	case DX_PIXEL_COLOR_FILTER::BAYERGB:
		eRet = e_BayerHalcon_GB;
		break;
	case DX_PIXEL_COLOR_FILTER::BAYERGR:
		eRet = e_BayerHalcon_GR;
		break;
	case DX_PIXEL_COLOR_FILTER::BAYERBG:
		eRet = e_BayerHalcon_BG;
		break;
	default:
		break;
	}
	return eRet;
}



void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent)
	{
	case ID_EVENT_UPDATE_INFO://**[12/1/2017 ConanSteve]**:  ������ʾ��Ϣ
	{
		/*************************************<��ǩ��Ϣ>************************************[12/1/2017 ConanSteve]*/
		CString sText{ _T("") };
		CString sTemp{ _T("") };
		/*************************************<����Ʒ����>************************************[12/28/2017 ConanSteve]*/
		m_iCheckCntTotle = 0;	
		for (int i = 0; i < m_iAssemblyLineNum; ++i)//**[12/28/2017 ConanSteve]**:  �����������ÿ����ˮ�ߵĵ�һ�������Ĳ�Ʒ����֮��
		{
			if (0 == m_arr_veciCheckGroupIndexOfAsseblyLine[i].size())
			{
				KillTimer(ID_EVENT_UPDATE_INFO);
				sTemp.Format(tr("ϵͳ�쳣�������˳�\n����ԭ����ˮ��%d�����ļ�������Ϊ0�������¼��ϵͳ�����ļ�"), i);
				AfxMessageBox(sTemp);
				m_fileErrorInfo.Write(sTemp);
				exit(0);
			}
			int iFirstCheckSNInCurAsbly = m_arr_veciCheckGroupIndexOfAsseblyLine[i][0];//**[12/28/2017 ConanSteve]**:  ��ǰ��ˮ�ߵĵ�һ��������
			m_arr_iCheckCntOfAssembly[i] = m_arr_iCheckCntOfCheckGroup[iFirstCheckSNInCurAsbly];
			m_iCheckCntTotle += m_arr_iCheckCntOfAssembly[i];
		}
		sText.Format(_T("%s: %d"),tr("�������"), m_iCheckCntTotle);
		g_pView->m_pdlgRunning->m_staticCheckCntLabel.SetWindowText(sText);
		g_pView->m_pdlgRunning->SetDlgItemText(IDC_BUTTON_LABEL_CHECK_COUNT_TOTLE, sText);

		/*************************************<����ϵͳ�Ļ�Ʒ����>************************************[12/28/2017 ConanSteve]*/
		m_iCheckErrorCntTotle = 0;
		for (int i = 0; i < m_iAssemblyLineNum; ++i)//**[12/28/2017 ConanSteve]**:  ����ϵͳ�Ļ�Ʒ��������ÿ����ˮ�����һ�������Ĳ�Ʒ������ȥ�����ˮ�ߵ����һ���޷϶��е��޷ϸ���֮��
		{
			int iLastCheckSNInCurAsbly = *m_arr_veciCheckGroupIndexOfAsseblyLine[i].rbegin();//**[12/28/2017 ConanSteve]**:  ��ǰ��ˮ�ߵ����һ�����������
			int iLastKickSNInCurAsbly = *m_arr_veciKickSNInAssemblyLine[i].rbegin();//**[12/28/2017 ConanSteve]**:  ��ǰ��ˮ�ߵ����һ���޷϶��еı��
			s_KickCountInfo & rKickCntInfo = m_arr_sKickCntInfoOfKickQueue[iLastKickSNInCurAsbly];
			int iBadCntOfCurKickQueue = 0;//**[12/28/2017 ConanSteve]**:  ��ǰ�߷϶������ߵĻ�Ʒ֮�͡� ƿ�Ƿ�ɫ�߷ϵĲ��Ƕ��ǻ�Ʒ
			for (int j = 0; j < rKickCntInfo.iKickPortNum; ++j)
			{
				if (rKickCntInfo.arr_bIsBadKick[j])//**[12/28/2017 ConanSteve]**:  ����ǻ�Ʒ�߷�
					iBadCntOfCurKickQueue += rKickCntInfo.arr_iKickPortCount[j];
			}
			//m_arr_iCheckGoodCntOfAssembly[i] = m_arr_iCheckCntOfCheckGroup[iLastCheckSNInCurAsbly] - iBadCntOfCurKickQueue;
			m_arr_iCheckGoodCntOfAssembly[i] = m_arr_iGoodCntOfKickQueue[iLastKickSNInCurAsbly];
			m_arr_iCheckErrorCntOfAssembly[i] = m_arr_iCheckCntOfAssembly[i] - m_arr_iCheckGoodCntOfAssembly[i];
			if (0 != m_arr_iCheckCntOfAssembly[i])
				m_arr_lfGoodRatioOfAssembly[i] = m_arr_iCheckGoodCntOfAssembly[i] / (double)m_arr_iCheckCntOfAssembly[i] * 100.0;
			else
				m_arr_lfGoodRatioOfAssembly[i] = 0;
			m_iCheckErrorCntTotle += m_arr_iCheckErrorCntOfAssembly[i];
			sText.Format(_T("%s%d%s: %d"), tr("��ˮ��"), i + 1, tr("�������"), m_arr_iCheckCntOfAssembly[i]);
			g_pView->m_pdlgRunning->SetDlgItemText(IDC_BUTTON_LABEL_TOTLE_COUNT_ASSEMBLY + i, sText);
			sText.Format(_T("%s%d%s: %d"), tr("��ˮ��"), i + 1, tr("�ϸ�����"), m_arr_iCheckGoodCntOfAssembly[i]);
			g_pView->m_pdlgRunning->SetDlgItemText(IDC_BUTTON_LABEL_GOOD_COUNT_ASSEMBLY + i, sText);
			sText.Format(_T("%s%d%s: %d"), tr("��ˮ��"), i + 1, tr("���ϸ�����"), m_arr_iCheckErrorCntOfAssembly[i]);
			g_pView->m_pdlgRunning->SetDlgItemText(IDC_BUTTON_LABEL_BAD_COUNT_ASSEMBLY + i, sText);
			sText.Format(_T("%s%d%s: %.3f%%"), tr("��ˮ��"), i + 1, tr("�ϸ���"), m_arr_lfGoodRatioOfAssembly[i]);
			g_pView->m_pdlgRunning->SetDlgItemText(IDC_BUTTON_LABEL_GOOD_RATIO_ASSEMBLY + i, sText);
		}
		sText.Format(_T("%s: %d"),tr("���ϸ�����"), m_iCheckErrorCntTotle);
		g_pView->m_pdlgRunning->m_staticErrorCntLabel.SetWindowText(sText);
		g_pView->m_pdlgRunning->SetDlgItemText(IDC_BUTTON_LABEL_ERROR_COUNT_TOTLE, sText);
		/*************************************<>************************************[12/28/2017 ConanSteve]*/
		int iGoodCntTotle = m_iCheckCntTotle - m_iCheckErrorCntTotle;
		sText.Format(_T("%s: %d"), tr("�ϸ�����"),iGoodCntTotle);
		g_pView->m_pdlgRunning->m_staticGoodCntLabel.SetWindowText(sText);
		g_pView->m_pdlgRunning->SetDlgItemText(IDC_BUTTON_LABEL_GOOD_COUNT_TOTLE, sText);

		double lfGoodRate = 0;
		if (0 < iGoodCntTotle)
		{
			lfGoodRate = (double)(iGoodCntTotle) * 100.0 / m_iCheckCntTotle;
		}
		sText.Format(_T("%s: %.3f%%"), tr("�ϸ���"),lfGoodRate);
		g_pView->m_pdlgRunning->m_staticGoodRateLabel.SetWindowText(sText);
		g_pView->m_pdlgRunning->SetDlgItemText(IDC_BUTTON_LABEL_GOOD_RATE_TOTLE, sText);
		/*************************************<״̬����Ϣ>************************************[12/1/2017 ConanSteve]*/
		sText = _T("");

		if (m_bEnableShowPE)
		{
			sText += tr("���:");
			for (int i = 0; i < m_iPENum; ++i)
			{
				sTemp.Format(_T("[%d]"), m_arr_iPECount[i]);
				sText += sTemp;
			}
		}
		
		sText += (CString)_T(" ")+tr("�ɼ�")+(CString)_T(":");
		for (int i = 0; i < m_iGrabberNum; ++i)
		{
			sTemp.Format(_T("[%d]"), m_arr_iGrabCallbackCnt[i]);
			sText += sTemp;
		}
		sTemp = (CString)_T(" ")+ tr("ͼ���") + (CString)_T(":");
		sText += sTemp;
		for (int i=0;i<m_iGrabberNum;++i)
		{
			sTemp.Format(_T("[%d]"), m_arr_iHardImageCountOfGrabInRegister[i]);
			sText += sTemp;
		}
		/*****************************************************************************************/
		if (m_bEnableShowGrabLostFrame)
		{
			sText += (CString)_T(" ")+tr("��֡") + (CString)_T(":");
			for (int i = 0; i < m_iGrabberNum; ++i)
			{
				sTemp.Format(_T("[%d]"), m_arr_iLostFrameEachGrab[i]);
				sText += sTemp;
			}
		}
		/*****************************************************************************************/
		{
			sText += (CString)_T(" ")+ tr("��Ʒ��") + (CString)_T(":");
			for (int i = 0; i < m_iCheckGroupNum; ++i)
			{
				sTemp.Format(_T("[%d]"), m_arr_iCheckCntOfCheckGroup[i]);
				sText += sTemp;
			}
		}
		/*****************************************************************************************/
		{
			sTemp = (CString)_T(" ") +tr("��Ʒ") + (CString)_T(":");
			sText += sTemp;
			for (int i = 0; i < m_iCheckGroupNum; ++i)
			{
				sTemp.Format(_T("[%d]"), m_arr_iBadCntOfCheckGroup[i]);
				sText += sTemp;
			}
		}
		/*****************************************************************************************/
		{
			sTemp = (CString)_T(" ") +tr("�޷�") + (CString)_T(":");
			sText += sTemp;
			for (int i = 0; i < m_iKickQueueNum; ++i)
			{
				sText += _T("[");
				for (int j = 0; j < m_arr_sKickCntInfoOfKickQueue[i].iKickPortNum; ++j)
				{
					CString sFormat{ _T("") };
					sFormat = 0 == j ? _T("%d") : _T("-%d");
					sTemp.Format(sFormat, m_arr_sKickCntInfoOfKickQueue[i].arr_iKickPortCount[j]);
					sText += sTemp;
				}
				sText += _T("]");
			}
		}
		/*****************************************************************************************/
		if (m_bEnableShowAutoKick)
		{
			sTemp = tr("����:");
			sText += sTemp;
			for (int i = 0; i < m_iKickQueueNum; ++i)
			{
				sText += _T("[");
				for (int j = 0; j < m_arr_sKickCntInfoOfKickQueue[i].iKickPortNum; ++j)
				{
					CString sFormat{ _T("") };
					sFormat = 0 == j ? _T("%d") : _T("-%d");
					sTemp.Format(sFormat, m_arr_sKickCntInfoOfKickQueue[i].arr_iAutoKickCnt[j]);
					sText += sTemp;
				}
				sText += _T("]");
			}
		}

		m_wndStatusBar.SetPaneText(1,sText);
		/*************************************<>************************************[12/1/2017 ConanSteve]*/
		g_pView->m_pdlgRunning->m_pdlgHisErrorInfo->UpdateDefectList();
		break;
	}
	case ID_EVENT_UPDATE_CHECK_SPEED://**[12/28/2017 ConanSteve]**:  ���¼���ٶ�
	{
		int iCheckSpeedTotle{ 0 };//**[12/28/2017 ConanSteve]**:  ������ˮ�ߵļ���ٶ�֮��
		CString sInfo{ _T("") };
		for (int i = 0; i < m_iAssemblyLineNum; ++i)
		{
			if (m_arr_queCheckCntPerInterval[i].size() > 60)
			{
				m_arr_queCheckCntPerInterval[i].pop();
			}
			m_arr_queCheckCntPerInterval[i].push(m_arr_iCheckCntOfAssembly[i]);
			//**[12/28/2017 ConanSteve]**:  
			m_arr_iCheckSpeedOfAssembly[i] = (m_arr_iCheckCntOfAssembly[i] - m_arr_queCheckCntPerInterval[i].front())*((m_arr_queCheckCntPerInterval[i].size()-1)* m_iCheckSpeedUpdateInterval) / 60000;
			sInfo.Format(_T("%s%d%s: %d%s"), tr("��ˮ��"), i + 1, tr("����ٶ�"), m_arr_iCheckSpeedOfAssembly[i], tr("��/��"));
			g_pView->m_pdlgRunning->SetDlgItemText(IDC_BUTTON_LABEL_CHECK_SPEED_ASSEMBLY+i, sInfo);
			iCheckSpeedTotle += m_arr_iCheckSpeedOfAssembly[i];
		}
		sInfo.Format(_T("%s: %d%s"), tr("����ٶ�"),iCheckSpeedTotle, tr("��/��"));
		g_pView->m_pdlgRunning->SetDlgItemText(IDC_BUTTON_LABEL_CHECK_SPEED, sInfo);
		break;
	}
	case ID_EVENT_START_GRABBER:
	{
		for (int i = 0; i < m_iGrabberNum; ++i)
		{
			if (i == 0)
				continue;
			if (!StartCameraGrab(i))
			{
				KillTimer(ID_EVENT_START_GRABBER);
				return;
			}
		}
		KillTimer(ID_EVENT_START_GRABBER);
		break;
	}
	default:
			break;
	}
	CFrameWnd::OnTimer(nIDEvent);
}


bool CMainFrame::ResizeLayout()
{
	/*************************************<����ͼ�񴰿�>************************************[12/27/2017 ConanSteve]*/
	if (NULL != g_pFrame&&NULL!=g_pView)
	{
		CRect rectDlgRunning;
		g_pView->m_pdlgRunning->GetClientRect(rectDlgRunning);
		CString sPathFileOfUICfg{ _T("") };
		sPathFileOfUICfg.Format(_T("%s_%d_%d.ini"), CExFn::GetCurrentAppPath() + _T("\\Config\\UIConfig"), m_cxScreenSize, m_cyScreenSize);
		CINICfg iniCfgUI(sPathFileOfUICfg);
		int iWindowLayoutType = iniCfgUI.GetIntEx(_T("��������"), _T("ͼ�񴰿ڲ�������"), 0, _T("0:������Ӧ, 1�ǹ̶�λ��"));
		if (1 == iWindowLayoutType)//**[12/27/2017 ConanSteve]**:  �̶�λ��
		{
			for (int i = 0; i < m_iGrabberNum; ++i)
			{
				CString sModuleName{ _T("") };
				sModuleName.Format(_T("CameraImageParameters%d"), i + 1);
				g_pView->m_pdlgRunning->m_p_rectCamImg[i].left = iniCfgUI.GetIntEx(sModuleName, _T("Left"), 0);
				g_pView->m_pdlgRunning->m_p_rectCamImg[i].top = iniCfgUI.GetIntEx(sModuleName, _T("Top"), 0);
				g_pView->m_pdlgRunning->m_p_rectCamImg[i].right = iniCfgUI.GetIntEx(sModuleName, _T("Width"), 0) + g_pView->m_pdlgRunning->m_p_rectCamImg[i].left;
				g_pView->m_pdlgRunning->m_p_rectCamImg[i].bottom = iniCfgUI.GetIntEx(sModuleName, _T("Height"), 0) + g_pView->m_pdlgRunning->m_p_rectCamImg[i].top;

				sModuleName.Format(_T("AlgorithmImageParameters%d"), i + 1);
				g_pView->m_pdlgRunning->m_p_rectAlgImg[i].left = iniCfgUI.GetIntEx(sModuleName, _T("Left"), 0);
				g_pView->m_pdlgRunning->m_p_rectAlgImg[i].top = iniCfgUI.GetIntEx(sModuleName, _T("Top"), 0);
				g_pView->m_pdlgRunning->m_p_rectAlgImg[i].right = iniCfgUI.GetIntEx(sModuleName, _T("Width"), 0) + g_pView->m_pdlgRunning->m_p_rectAlgImg[i].left;
				g_pView->m_pdlgRunning->m_p_rectAlgImg[i].bottom = iniCfgUI.GetIntEx(sModuleName, _T("Height"), 0) + g_pView->m_pdlgRunning->m_p_rectAlgImg[i].top;
			}
		}
		else if (0 == iWindowLayoutType)//**[12/27/2017 ConanSteve]**:  ����Ӧ
		{
			//**[12/27/2017 ConanSteve]**:  ϵͳͼ��
			int iGap_X = 5, iGap_Y = 5;//**[12/27/2017 ConanSteve]**:  ���ڴ���֮��ľ���
			int iCnt_X = iniCfgUI.GetIntEx(_T("ϵͳͼ�񴰿�"), _T("X��������"), int(sqrt(m_iGrabberNum)+1));
			int iCnt_Y = iniCfgUI.GetIntEx(_T("ϵͳͼ�񴰿�"), _T("Y��������"), int(sqrt(m_iGrabberNum) + 1));
			int iWindowWidth = iniCfgUI.GetIntEx(_T("ϵͳͼ�񴰿�"), _T("X����"), rectDlgRunning.Width());
			int iWindowHeight = iniCfgUI.GetIntEx(_T("ϵͳͼ�񴰿�"), _T("Y��߶�"), rectDlgRunning.Height()/3);
			int iWindowLeft = iniCfgUI.GetIntEx(_T("ϵͳͼ�񴰿�"), _T("X��Left"), 0);
			int iWindowTop = iniCfgUI.GetIntEx(_T("ϵͳͼ�񴰿�"), _T("Y��Top"), 0);
			int iBaseCoordination = iniCfgUI.GetIntEx(_T("ϵͳͼ�񴰿�"), _T("��׼��"), 0, _T(" 0��ʾ������Ϊˮƽ�� 1Ϊ��ֱ"));
			CExFn::AutoLayoutRect(CRect(iWindowLeft, iWindowTop, iWindowLeft + iWindowWidth, iWindowTop + iWindowHeight),
				iCnt_X, iCnt_Y, iGap_X, iGap_Y, m_iGrabberNum, iBaseCoordination, g_pView->m_pdlgRunning->m_p_rectCamImg.get());
			//**[12/27/2017 ConanSteve]**:  �㷨ͼ��
			iCnt_X = iniCfgUI.GetIntEx(_T("�㷨ͼ�񴰿�"), _T("X��������"), int(sqrt(m_iGrabberNum) + 1));
			iCnt_Y = iniCfgUI.GetIntEx(_T("�㷨ͼ�񴰿�"), _T("Y��������"), int(sqrt(m_iGrabberNum) + 1));
			iWindowWidth = iniCfgUI.GetIntEx(_T("�㷨ͼ�񴰿�"), _T("X����"), rectDlgRunning.Width());
			iWindowHeight = iniCfgUI.GetIntEx(_T("�㷨ͼ�񴰿�"), _T("Y��߶�"), rectDlgRunning.Height()/3);
			iWindowLeft = iniCfgUI.GetIntEx(_T("�㷨ͼ�񴰿�"), _T("X��Left"), 0);
			iWindowTop = iniCfgUI.GetIntEx(_T("�㷨ͼ�񴰿�"), _T("Y��Top"), rectDlgRunning.Height() / 3);
			iBaseCoordination = iniCfgUI.GetIntEx(_T("�㷨ͼ�񴰿�"), _T("��׼��"), 0, _T(" 0��ʾ������Ϊˮƽ�� 1Ϊ��ֱ"));
			CExFn::AutoLayoutRect(CRect(iWindowLeft, iWindowTop, iWindowLeft + iWindowWidth, iWindowTop + iWindowHeight),
				iCnt_X, iCnt_Y, iGap_X, iGap_Y, m_iGrabberNum, iBaseCoordination, g_pView->m_pdlgRunning->m_p_rectAlgImg.get());

		}
		/*************************************<��������ߴ��������>************************************[12/27/2017 ConanSteve]*/
		for (int i = 0; i < m_iGrabberNum; ++i)
		{
			if (m_bEnableInitGrabber)
			{
				int iPicHeight = m_arr_sCameraImageInfo[i].m_iCameraImageHeight;
				int iPicWidth = m_arr_sCameraImageInfo[i].m_iCameraImageWidth;
				g_pView->m_pdlgRunning->m_p_rectCamImg[i] = CExFn::AutoResizeRect(iPicWidth, iPicHeight, g_pView->m_pdlgRunning->m_p_rectCamImg[i]);
				g_pView->m_pdlgRunning->m_p_rectAlgImg[i] = CExFn::AutoResizeRect(iPicWidth, iPicHeight, g_pView->m_pdlgRunning->m_p_rectAlgImg[i]);
			}
			g_pView->m_pdlgRunning->m_pStaticCamImg[i].MoveWindow(g_pView->m_pdlgRunning->m_p_rectCamImg[i]);
			g_pView->m_pdlgRunning->m_pStaticAlgImg[i].MoveWindow(g_pView->m_pdlgRunning->m_p_rectAlgImg[i]);
		}
	}
	/*************************************<������ť����Ϣ��ʾ>************************************[12/27/2017 ConanSteve]*/
	if (NULL != g_pFrame)
	{
		g_pView->m_pdlgRunning->ResizeLayout();
	}
	return true;
}


bool CMainFrame::ResetSystem(int iType)
{
	if (1 == (iType & 1))//**[12/28/2017 ConanSteve]**:  �������м���
	{
		//**[1/5/2018 ConanSteve]**:  ���
		fill(m_arr_iPECount.begin(), m_arr_iPECount.end(), 0);
		//**[12/13/2017 ConanSteve]**:  �����
		fill(m_arr_iGrabCallbackCnt.begin(), m_arr_iGrabCallbackCnt.end(), 0);
		fill(m_arr_iLastHardImageCountOfGrabInRegister.begin(), m_arr_iLastHardImageCountOfGrabInRegister.end(), 0);
		fill(m_arr_iHardImageCountOfGrabInRegister.begin(), m_arr_iHardImageCountOfGrabInRegister.end(), 0);
		fill(m_arr_iLostFrameEachGrab.begin(), m_arr_iLostFrameEachGrab.end(), 0);
		fill(m_arr_iCurSmallCntEachGrab.begin(), m_arr_iCurSmallCntEachGrab.end(), 0);

		//**[12/13/2017 ConanSteve]**:  �����
		fill(m_arr_iCheckCntOfCheckGroup.begin(), m_arr_iCheckCntOfCheckGroup.end(), 0);
		fill(m_arr_iGoodCntOfCheckGroup.begin(), m_arr_iGoodCntOfCheckGroup.end(), 0);
		fill(m_arr_iBadCntOfCheckGroup.begin(), m_arr_iBadCntOfCheckGroup.end(), 0);
		fill(m_arr_iAlgExceptionCntOfCheckGroup.begin(), m_arr_iAlgExceptionCntOfCheckGroup.end(), 0);

		//**[12/13/2017 ConanSteve]**:  �޷���
		fill(m_arr_sKickCntInfoOfKickQueue.begin(), m_arr_sKickCntInfoOfKickQueue.end(), s_KickCountInfo());
		fill(m_arr_iGoodCntOfKickQueue.begin(), m_arr_iGoodCntOfKickQueue.end(), 0);
		//**[12/28/2017 ConanSteve]**:  ��ˮ����
		fill(m_arr_iCheckCntOfAssembly.begin(), m_arr_iCheckCntOfAssembly.end(), 0);
		//**[12/5/2017 ConanSteve]**:  ����ȱ������ͳ��
		for (int i = 0; i < m_iCheckGroupNum; ++i)
		{
			for (auto itera = m_arr_mapDefectStatistics[i].begin(); itera != m_arr_mapDefectStatistics[i].end(); ++itera)
			{
				itera->second.iCnt = 0;
			}
		}
		for (int i=0; i<m_iAssemblyLineNum;++i)
		{
			while (!m_arr_queCheckCntPerInterval[i].empty())//**[12/28/2017 ConanSteve]**:  ������������
				m_arr_queCheckCntPerInterval[i].pop();
			for (int j = 0; j < 60; ++j)
			{
				m_arr_queCheckCntPerInterval[i].push(0);
			}
		}
	}
	//**[12/13/2017 ConanSteve]**:  ���ýӿڿ�
	if (2 == (iType & 2))
	{
		for (int i = 0; i < m_iIOCardNum; ++i)
		{
			m_arr_pIOCard[i]->ResetIOCard();
		}
	}
	return true;
}


bool CMainFrame::OnBtnClickOpenIOCardTools()
{
	if (m_bEnableInitIOCard)//**[12/8/2017 ConanSteve]**:  ������˽ӿڿ�
	{
		for (int i = 0; i < m_iIOCardNum; ++i)
		{
			m_arr_pIOCard[i]->OpenTools(this);
		}
	}
	return true;
}

inline bool CMainFrame::StartAllThread()
{
	CWinThread* pTemp = NULL;
	// ��ȡͼ���
	pTemp = NULL;
	pTemp = AfxBeginThread((AFX_THREADPROC)GetIOCardImageNumThread, this);
	if (NULL == pTemp)
		return false;
	pTemp = AfxBeginThread((AFX_THREADPROC)WaitLoadModelThread, this);
	if (NULL == pTemp)
		return false;
	return true;
}

DWORD CMainFrame::GetIOCardImageNumThread(LPVOID lpParam)
{
	CMainFrame* This = (CMainFrame*)lpParam;
	DWORD dRet = This->GetIOCardImageNumThreadFunc();
	return dRet;
}

DWORD CMainFrame::GetIOCardImageNumThreadFunc()
{
	//if(m_bInitIOCardSuccess)
	{
		while (!bExit)//**[12/13/2017 ConanSteve]**:  û���˳�ϵͳ
		{
			if (m_bEnableInitIOCard)//**[12/13/2017 ConanSteve]**:  ������ڼ�⣬�ҳ�ʼ���˽ӿڿ�
			{
				GetCount_Guangdian();

				GetCount_Tifei();

				GetCount_TuXiangHao();
			}
			Sleep(1);
		}
	}
	return 0;
}

DWORD CMainFrame::WaitLoadModelThread(LPVOID lpParam)
{
	CMainFrame* This = (CMainFrame*)lpParam;
	DWORD dRet = This->WaitLoadModelThreadFunc();
	return dRet;
}

DWORD CMainFrame::WaitLoadModelThreadFunc()
{
	m_hEvent_DemoMessage_SendCommandOfLoadModel = m_hEvent_DemoMessage_ReceiveSuccessOfLoadModel = m_hEvent_DemoMessage_ReveiveFailureOfLoadModel = NULL;
	m_hEvent_DemoMessage_SendCommandOfLoadModel = CreateEvent(NULL, FALSE, FALSE, TEXT("DemoMessage_SendCommandOfLoadModel"));
	m_hEvent_DemoMessage_ReceiveSuccessOfLoadModel = CreateEvent(NULL, FALSE, FALSE, TEXT("DemoMessage_ReceiveSuccessOfLoadModel"));
	m_hEvent_DemoMessage_ReveiveFailureOfLoadModel = CreateEvent(NULL, FALSE, FALSE, TEXT("DemoMessage_ReveiveFailureOfLoadModel"));
	
	if (NULL!= m_hEvent_DemoMessage_ReveiveFailureOfLoadModel)
	{
		while (!bExit)
		{
			DWORD dwRet = WaitForSingleObject(m_hEvent_DemoMessage_SendCommandOfLoadModel, 100);
			switch (dwRet)
			{
			case WAIT_OBJECT_0:
				if (LoadModel(m_sModelName).bRet)
					SetEvent(m_hEvent_DemoMessage_ReceiveSuccessOfLoadModel);
				else
					SetEvent(m_hEvent_DemoMessage_ReveiveFailureOfLoadModel);
				break;
			default:
				break;
			}
		}
		CloseHandle(m_hEvent_DemoMessage_SendCommandOfLoadModel);
		CloseHandle(m_hEvent_DemoMessage_ReceiveSuccessOfLoadModel);
		CloseHandle(m_hEvent_DemoMessage_ReveiveFailureOfLoadModel);
	}
	return 0;
}

BOOL CMainFrame::GetCount_Guangdian()
{
	for (int i = 0; i < m_iPENum; ++i)
	{
		for (int j = 0; j < m_iIOCardNum; ++j)
			m_arr_pIOCard[j]->GetInputIndex(i, m_arr_iPECount[i]);
	}
	return 0;
}

BOOL CMainFrame::GetCount_Tifei()
{
	for (int i = 0; i < m_iKickQueueNum; ++i)
	{
		for (int j = 0; j < m_iIOCardNum; ++j)
			m_arr_pIOCard[j]->GetKickCountInfo(i, m_arr_sKickCntInfoOfKickQueue[i]);
	}
	return 0;
}

BOOL CMainFrame::GetCount_TuXiangHao()
{
	m_csGetCount_TuXiangHao.Lock();
	for (int i = 0; i < m_iGrabberNum; ++i)
	{
		for (int j = 0; j < m_iIOCardNum; ++j)
			m_arr_pIOCard[j]->GetGrabImgSN(i, m_arr_iHardImageCountOfGrabInRegister[i]);
		if (m_arr_iHardImageCountOfGrabInRegister[i] != m_arr_iLastHardImageCountOfGrabInRegister[i])
		{
			m_arr_iLastHardImageCountOfGrabInRegister[i] = m_arr_iHardImageCountOfGrabInRegister[i];
			CString sInfo{ _T("") };
			sInfo.Format(_T("[��ǰͼ���:%8d][��һͼ���:%8d]"), m_arr_iHardImageCountOfGrabInRegister[i], m_arr_iLastHardImageCountOfGrabInRegister[i]);
			m_p_fileImgCntOfGrabInRegister[i].Write(sInfo);
		}
	}
	m_csGetCount_TuXiangHao.Unlock();
	return 0;
}


bool CMainFrame::OnBtnClickOpenDefectStatistics()
{
	CDlgDefectStatistics dlg;
	dlg.DoModal();
	return true;
}


BOOL CMainFrame::StartCameraGrab(int iGrabSN)
{
	BOOL bRet = FALSE;
	bRet = m_arr_pGrabber[iGrabSN]->StartGrab();
	if (FALSE == bRet)
	{
		CString sInfo{ _T() };
		sInfo.Format(_T("�����%dʧ��!"), iGrabSN + 1);
		AfxMessageBox(sInfo);
		return FALSE;
	}
	return TRUE;
}


CRetInfo CMainFrame::LoadModel(CString sModelName)
{
	/*************************************<����ģ��>************************************[11/24/2017 ConanSteve]*/
	s_Status sStatus;
	CRetInfo ret;
	s_AlgModelInput sAlgModel;
	s_DemoAlgSetParam sAlgSet;
	CString sInfo{ _T("") };
	m_fileDebugInfo.Write(_T("��ʼ����ģ��"));
	for (int i = 0; i < m_iCheckGroupNum; i++)
	{
		sAlgSet.bTopWin = TRUE;
		sAlgSet.pParent = NULL;
		sAlgSet.iCheckSN = i;
		switch (m_eProductLine)
		{
		case e_ProductLine_NO:
			break;
		case e_ProductLine_General:
		case e_ProductLine_LableCheck:
		{
			memcpy_s(sAlgSet.chModelName, MAX_PATH, sModelName.GetBuffer(0), sModelName.GetLength());
		}
		break;
		case e_ProductLine_Stopper:
			break;
		case e_ProductLine_LampInsp:
		{
			memcpy_s(sAlgSet.chModelName, MAX_PATH, m_sPathOfAlgModelInfo.GetBuffer(0), m_sPathOfAlgModelInfo.GetLength());
		}
		break;
		default:
			break;
		}

		sAlgModel.iCheckSN = i;
		sAlgModel.iOperSig = e_AlgModelInputOper_LoadModel;
		sAlgModel.pParam = &sAlgSet;
		if (m_bEnableInitMultiObjectAndCheck)
		{
			sInfo.Format(_T("��ʼ���ع�λ%dģ��"), i + 1);
			m_fileDebugInfo.Write(sInfo);
			sStatus = m_multiObject.Set(sAlgModel);
			sInfo.Format(_T("��ɼ��ع�λ%dģ��"), i + 1);
			m_fileDebugInfo.Write(sInfo);
			if (STATUS_NOR != sStatus.iStatusType)
			{
				ret.bRet = false;
				ret.sErrorInfo = sStatus.pErrorInfo;
				return ret;
			}
		}
	}
	m_fileDebugInfo.Write(_T("��ɼ���ģ��"));
	ret.bRet = true;
	return ret;
}

BOOL CMainFrame::SetLutTable(double dRatioR, double dRatioG, double dRatioB)
{
	if (dRatioR <= 0)
		return FALSE;
	if (dRatioG <= 0)
		return FALSE;
	if (dRatioB <= 0)
		return FALSE;

	for (int i = 0; i < 256; i++)
	{
		m_pLutR[i] = min((int)(i*dRatioR), 255);
		m_pLutG[i] = min((int)(i*dRatioG), 255);
		m_pLutB[i] = min((int)(i*dRatioB), 255);
	}

	return TRUE;
}

int CMainFrame::MirrorSingleImage(BYTE* pImageBuff, int lImageWidth, int lImageHeight, int lImageBitCount, e_ShowModeFlip  eShowMode) //ֻ��ˮƽ�ʹ�ֱ����
{
	if (pImageBuff == NULL)
	{
		return ImageBufferError;
	}
	if (lImageWidth < 0 || lImageHeight < 0 || lImageBitCount < 0)
	{
		return ImageWHBITError;
	}

	if (lImageBitCount == 8)
	{
		lImageWidth = WIDTHBYTES(lImageWidth * 8);
		if (eShowMode == FlipHorizintal) //**[1/3/2018 ConanSteve]**:	 ˮƽ��ת
		{
			BYTE  tem;
			for (int i = 0; i < lImageHeight; i++)
			{
				for (int j = 0; j < lImageWidth / 2; j++)
				{
					tem = *(pImageBuff + i*lImageWidth + j);
					*(pImageBuff + i*lImageWidth + j) = *(pImageBuff + i*lImageWidth + lImageWidth - 1 - j);
					*(pImageBuff + i*lImageWidth + lImageWidth - 1 - j) = tem;
				}
			}
			return TRUE;
		}
		if (eShowMode == FlipVertical) //**[1/3/2018 ConanSteve]**:	 ��ֱ��ת
		{
			BYTE  tem;
			for (int i = 0; i < lImageHeight / 2; i++)
			{
				for (int j = 0; j < lImageWidth; j++)
				{
					tem = *(pImageBuff + i*lImageWidth + j);
					*(pImageBuff + i*lImageWidth + j) = *(pImageBuff + (lImageHeight - 1 - i)*lImageWidth + j);
					*(pImageBuff + (lImageHeight - 1 - i)*lImageWidth + j) = tem;
				}
			}
			return TRUE;
		}
	}
	if (lImageBitCount == 24)//24λ����
	{
		lImageWidth = WIDTHBYTES(lImageWidth * 8);

		if (eShowMode == FlipHorizintal) //**[1/3/2018 ConanSteve]**:	 ˮƽ��ת
		{
			BYTE  tem1, tem2, tem3;
			for (int i = 0; i < lImageHeight; i++)
			{
				for (int j = 0; j < lImageWidth / 2; j++)
				{
					tem1 = *(pImageBuff + i*lImageWidth * 3 + 3 * j);
					tem2 = *(pImageBuff + i*lImageWidth * 3 + 3 * j + 1);
					tem3 = *(pImageBuff + i*lImageWidth * 3 + 3 * j + 2);
					*(pImageBuff + i*lImageWidth * 3 + 3 * j) = *(pImageBuff + i*lImageWidth * 3 + 3 * (lImageWidth - 1 - j));
					*(pImageBuff + i*lImageWidth * 3 + 3 * j + 1) = *(pImageBuff + i*lImageWidth * 3 + 3 * (lImageWidth - 1 - j) + 1);
					*(pImageBuff + i*lImageWidth * 3 + 3 * j + 2) = *(pImageBuff + i*lImageWidth * 3 + 3 * (lImageWidth - 1 - j) + 2);
					*(pImageBuff + i*lImageWidth * 3 + 3 * (lImageWidth - 1 - j)) = tem1;
					*(pImageBuff + i*lImageWidth * 3 + 3 * (lImageWidth - 1 - j) + 1) = tem2;
					*(pImageBuff + i*lImageWidth * 3 + 3 * (lImageWidth - 1 - j) + 2) = tem3;
				}
			}
			return TRUE;
		}
		if (eShowMode == FlipVertical) //**[1/3/2018 ConanSteve]**:	 ��ֱ��ת
		{
			BYTE  tem1, tem2, tem3;
			for (int i = 0; i < lImageHeight / 2; i++)
			{
				for (int j = 0; j < lImageWidth; j++)
				{
					tem1 = *(pImageBuff + i*lImageWidth * 3 + 3 * j);
					tem2 = *(pImageBuff + i*lImageWidth * 3 + 3 * j + 1);
					tem3 = *(pImageBuff + i*lImageWidth * 3 + 3 * j + 2);
					*(pImageBuff + i*lImageWidth * 3 + 3 * j) = *(pImageBuff + (lImageHeight - 1 - i)*lImageWidth * 3 + 3 * j);
					*(pImageBuff + i*lImageWidth * 3 + 3 * j + 1) = *(pImageBuff + (lImageHeight - 1 - i)*lImageWidth * 3 + 3 * j + 1);
					*(pImageBuff + i*lImageWidth * 3 + 3 * j + 2) = *(pImageBuff + (lImageHeight - 1 - i)*lImageWidth * 3 + 3 * j + 2);
					*(pImageBuff + (lImageHeight - 1 - i)*lImageWidth * 3 + 3 * j) = tem1;
					*(pImageBuff + (lImageHeight - 1 - i)*lImageWidth * 3 + 3 * j + 1) = tem2;
					*(pImageBuff + (lImageHeight - 1 - i)*lImageWidth * 3 + 3 * j + 2) = tem3;
				}
			}
			return TRUE;
		}
	}
	return TRUE;
}


bool CMainFrame::OnBtnClickOpenDataStatistics()
{
	CDlgDataStatistics dlg;
	dlg.DoModal();
	return false;
}
