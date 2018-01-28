// DataClass.cpp: implementation of the CDataClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ObjectAllocation.h"
#include "DataClass.h"
#include <afxmt.h>
#include "typeinfo"
// #include "excel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


#define SELFMSG_CREATESTDLG	WM_USER+15
#define SELFMSG_CHANGEST	WM_USER+16

CDataClass::CDataClass()
{

	m_strAppDir = CExFn::GetCurrentAppPath();
	m_strAlgconfigDir.Format(_T("%s\\Config\\AlgConfig.ini"), m_strAppDir);

	LARGE_INTEGER iLarge{ 0 };
	QueryPerformanceFrequency(&iLarge);
	m_dFreq = (double)iLarge.QuadPart;
	m_fileDebugInfo.Open(CExFn::GetCurrentAppPath() + _T("\\DebugInfo.txt"));
	fill(m_pCheckStatusParam.begin(), m_pCheckStatusParam.end(), nullptr);
	fill(m_iGroupGoodCnt.begin(), m_iGroupGoodCnt.end(), 0);
	fill(m_iGroupBadCnt.begin(), m_iGroupBadCnt.end(), 0);
	fill(m_iKickErrorCount.begin(), m_iKickErrorCount.end(), 0);
	fill(m_iKickGoodCnt.begin(), m_iKickGoodCnt.end(), 0);

}


CDataClass::~CDataClass()
{

}

// BEGIN_MESSAGE_MAP(CTestExeDlg, CDialog)
// //{{AFX_MSG_MAP(CTestExeDlg)
// ON_WM_SYSCOMMAND()
// ON_WM_PAINT()
// ON_WM_QUERYDRAGICON()
// ON_BN_CLICKED(IDC_BTN_SHOW, OnBtnShow)
// ON_BN_CLICKED(IDC_BTN_HIDE, OnBtnHide)
// ON_MESSAGE(99, OnMsgInit)
// //}}AFX_MSG_MAP
// END_MESSAGE_MAP()

s_Status CDataClass::Init(s_CheckInitParam sInitParam)
{
	int i = 0, j = 0;
	s_Status sError;
	sError.pThis = m_pCallThis;

	// ����λ�������
	CString strInitFile{ _T("") };
	strInitFile.Format(_T("%s"), sInitParam.pFilePath);
	m_CheckCallback = sInitParam.KickCallbackFunc;
	m_StatusCallback = sInitParam.StatusCallbackFunc;
	m_pCallThis = sInitParam.pThis;
	m_iCallType = sInitParam.iCallType;

	if (sInitParam.iReserve1 != e_MachineType_Capsule && sInitParam.iReserve1 != e_MachineType_Liquor)
	{
		m_iMachineType = e_MachineType_Capsule;
	}
	else
	{
		m_iMachineType = sInitParam.iReserve1;
	}
	// ���Բ����Ƿ�Ϸ� ���Ϸ�1���Ϸ�0;
	if (!InitObjects(strInitFile, sInitParam.iCallType))
	{
		sError.iStatusType = STATUS_ILG;
		sError.pErrorInfo = GetErrorInfo(_T("Insufficient Construction Information"));
		return sError;
	}
	if (!m_bEnableInitObjectAndCheck)
		return sError;
	// ��demo���ã���������ڴ�
	if (e_CallType_Demo != m_iCallType)
	{
		// �����󡢹�λ �Ƿ��ڼ����߻�ͼ
		m_plObjBusyTimes = new long[m_iCheckGroupNum + 1]{ 0 };
		m_pbIsDrawing = make_unique<bool[]>(m_iCheckGroupNum + 1);
		fill(m_pbIsDrawing.get(), m_pbIsDrawing.get() + m_iCheckGroupNum + 1, false);

		for (i = 0; i < m_iCheckGroupNum; i++)
		{
			//m_nObjIndex[i] = new int[m_pnObjectNumber[i] + 1]{e_DetectStatus_NoChecking};//�����Ƿ���У�0���У�1æ
			fill(m_nObjIndex[i].begin(), m_nObjIndex[i].end(), e_DetectStatus_NoChecking);
		}

		m_ppCacheUseCnt = new int*[m_iCheckGroupNum];

		// ��ʼ��ͼ�����ݺͼ���̲߳���
		/************************************************************************/
		//**[8/2/2017 ConanSteve]**:  2��Detect��������
		if (e_ObjDetectMode_2 == m_iObjDetectMode)
		{
			m_ppCheckInParamEx = new BYTE*[m_iCheckGroupNum];
			m_ppCheckOutParamEx = new BYTE*[m_iCheckGroupNum];
			m_ppCheckStatusParamEx = new BYTE*[m_iCheckGroupNum];
			m_sppCheckInParamEx = new s_CheckInParam *[m_iCheckGroupNum];
			m_sppCheckOutParamEx = new s_CheckOutParam*[m_iCheckGroupNum];
			m_sppCheckStatusParamEx = new s_Status*[m_iCheckGroupNum];
		}
		/************************************************************************/
		for (i = 0; i < m_iCheckGroupNum; i++)
		{
			// ������������߳��ں��¼�
			m_hImageInspectStart[i] = CreateEvent(NULL, FALSE, FALSE, NULL);

			m_pWriteDataStatus[i] = new int[m_iGroupCheckQueueNum[i]];
			memset(m_pWriteDataStatus[i], 0, sizeof(int)*m_iGroupCheckQueueNum[i]);
			m_pDetectStatus[i] = new int[m_iGroupCheckQueueNum[i]];
			memset(m_pDetectStatus[i], 0, sizeof(int)*m_iGroupCheckQueueNum[i]);
			m_pCircleSN[i] = new int[m_iGroupCheckQueueNum[i]];
			memset(m_pCircleSN[i], 0, sizeof(int)*m_iGroupCheckQueueNum[i]);
			// 
			m_pImageData[i] = new BYTE[m_iGroupImageSize[i] * m_iGroupCheckQueueNum[i]];
			memset(m_pImageData[i], 0, sizeof(BYTE)*m_iGroupImageSize[i] * m_iGroupCheckQueueNum[i]);
			m_pCheckInParam[i] = new BYTE[m_iGroupCheckQueueNum[i] * m_iGroupCheckInSize[i]];
			memset(m_pCheckInParam[i], 0, sizeof(BYTE)*m_iGroupCheckQueueNum[i] * m_iGroupCheckInSize[i]);
			m_pCheckOutParam[i] = new BYTE[m_iGroupCheckQueueNum[i] * m_iGroupCheckOutSize[i]];
			memset(m_pCheckOutParam[i], 0, sizeof(BYTE)*m_iGroupCheckQueueNum[i] * m_iGroupCheckOutSize[i]);
			m_pCheckStatusParam[i] = new BYTE[m_iGroupCheckQueueNum[i] * m_iGroupCheckStatusSize[i]];

			memset(m_pCheckStatusParam[i], 0, sizeof(BYTE)*m_iGroupCheckQueueNum[i] * m_iGroupCheckStatusSize[i]);
			m_spCheckInParam[i] = new s_CheckInParam[m_iGroupCheckQueueNum[i]];
			memset(m_spCheckInParam[i], 0, sizeof(s_CheckInParam)*m_iGroupCheckQueueNum[i]);
			m_spCheckOutParam[i] = new s_CheckOutParam[m_iGroupCheckQueueNum[i]];
			memset(m_spCheckOutParam[i], 0, sizeof(s_CheckOutParam)*m_iGroupCheckQueueNum[i]);
			m_spCheckStatusParam[i] = new s_Status[m_iGroupCheckQueueNum[i]];
			memset(m_spCheckStatusParam[i], 0, sizeof(s_Status)*m_iGroupCheckQueueNum[i]);
			m_ppCacheUseCnt[i] = new int[m_iGroupCheckQueueNum[i]]{ 0 };
			memset(m_ppCacheUseCnt[i], 0, sizeof(int)*m_iGroupCheckQueueNum[i]);

			m_pResetWriteStatus[i] = shared_ptr<s_ResetWriteStatus>(new s_ResetWriteStatus[m_iGroupCheckQueueNum[i]], [](s_ResetWriteStatus *p)
			{
				delete[] p;
			});

			for (j = 0; j < m_iGroupCheckQueueNum[i]; j++)
			{
				m_spCheckInParam[i][j].pImageData = (char*)m_pImageData[i] + j * m_iGroupImageSize[i];
				m_spCheckInParam[i][j].pParam = m_pCheckInParam[i] + j * m_iGroupCheckInSize[i];
				m_spCheckOutParam[i][j].pParam = m_pCheckOutParam[i] + j * m_iGroupCheckOutSize[i];
				m_spCheckStatusParam[i][j].pParam = m_pCheckStatusParam[i] + j * m_iGroupCheckStatusSize[i];
			}
			/************************************************************************/
			//**[8/2/2017 ConanSteve]**:  2��Detect�����������
			if (e_ObjDetectMode_2 == m_iObjDetectMode)
			{
				m_ppCheckInParamEx[i] = new BYTE[m_pnObjectNumber[i] * m_iGroupCheckInSize[i]];
				m_ppCheckOutParamEx[i] = new BYTE[m_pnObjectNumber[i] * m_iGroupCheckOutSize[i]];
				m_ppCheckStatusParamEx[i] = new BYTE[m_pnObjectNumber[i] * m_iGroupCheckStatusSize[i]];
				m_sppCheckInParamEx[i] = new s_CheckInParam[m_pnObjectNumber[i]];
				m_sppCheckOutParamEx[i] = new s_CheckOutParam[m_pnObjectNumber[i]];
				m_sppCheckStatusParamEx[i] = new s_Status[m_pnObjectNumber[i]];

				for (int j = 0; j < m_pnObjectNumber[i]; ++j)
				{
					m_sppCheckInParamEx[i][j].pParam = m_ppCheckInParamEx[i] + j * m_iGroupCheckInSize[i];
					m_sppCheckOutParamEx[i][j].pParam = m_ppCheckOutParamEx[i] + j * m_iGroupCheckOutSize[i];
					m_sppCheckStatusParamEx[i][j].pParam = m_ppCheckStatusParamEx[i] + j * m_iGroupCheckStatusSize[i];

				}

			}
			/************************************************************************/

		}
		// ��ż������״̬
		for (i = 0; i < m_iKickQueueNum; i++)
		{
			for (j = 0; j < 256; j++)
			{
				m_pKickResult[i][j] = new BYTE[m_iKickQueueSize[i]];
				m_pbKickWrite[i][j] = new BYTE[m_iKickResultNum[i]];
			}
		}

		// ��������߳�
		CWinThread* pThread = NULL;
		for (i = 0; i < m_iCheckThreadNum; i++)
		{
			pThread = NULL;
			m_sThreadParam[i].pThis = this;
			m_sThreadParam[i].iThreadID = i;
			pThread = AfxBeginThread((AFX_THREADPROC)ImageInspectionThread, &m_sThreadParam[i], THREAD_PRIORITY_HIGHEST);
			if (NULL == pThread)
			{
				DWORD dError = GetLastError();
				sError.iStatusType = STATUS_ILG;
				char chInfo[c_iMaxStringLen]{ 0 };
				sprintf_s(chInfo, "CObjAlloc::Initialize Init Image Inspection Thread Failed!\r\nError Code: %d", dError);
				sError.pErrorInfo = GetErrorInfo(chInfo);
				return sError;
			}
		}

		// �����޷��߳�
		pThread = NULL;
		pThread = AfxBeginThread((AFX_THREADPROC)KickThread, this, THREAD_PRIORITY_NORMAL);
		if (NULL == pThread)
		{
			DWORD dwError = GetLastError();
			sError.iStatusType = STATUS_ILG;
			sError.pErrorInfo = GetErrorInfo("CObjAlloc::Initialize Init Kick Thread Failed!");
			return sError;
		}

		/*************************************<��ʼ��״̬����>*************************************/
		m_pObjStatusDlg = NULL;
		// 		if (!m_pObjStatusDlg)
		// 		{
		// 			CRect rcDlg{ 0 }, rcNewDlg{0};
		// 			int iLeft = 0, iTop = 0;
		// 			int iWScreen = GetSystemMetrics(SM_CXSCREEN);
		// 			int iHScreen = GetSystemMetrics(SM_CYSCREEN);
		// 
		// 			m_pObjStatusDlg = make_shared<CDlgObjStatus>(new CDlgObjStatus);
		// 			m_pObjStatusDlg->InitData(m_iCheckGroupNum, m_pnObjectNumber);
		// 			m_pObjStatusDlg->Create(IDD_DLG_OBJSTATUS);
		// 			m_pObjStatusDlg->GetWindowRect(&rcDlg);
		// 			iLeft = (iWScreen - rcDlg.Width()) / 2;
		// 			iTop = (iHScreen = rcDlg.Height()) / 2;
		// 			m_pObjStatusDlg->MoveWindow(iLeft, iTop, rcDlg.Width(), rcDlg.Height());
		// 			m_pObjStatusDlg->ShowWindow(SW_HIDE);
		// 		}
		m_pDlgRunInfo = new CDlgRunInfo(NULL, m_bEnableXCGUI);
		m_pDlgRunInfo->Create(IDD_DIALOG_RunInfo);
		if (m_bShowObjRunInfoWnd)
			m_pDlgRunInfo->ShowWindowEx(SW_SHOW);
		/************************************************************************/
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// �㷨���

	// ����λ������
	// ���붯̬��
	CString strDllPath{ _T("") };
	for (i = 0; i < m_iCheckGroupNum; ++i)
	{
		if (-1 != m_iSingleTest)
		{
			if (i != m_iSingleTest)
				continue;
		}

		strDllPath = m_strArrDllPath.GetAt(i);
		if (strDllPath == _T("NULL"))
		{
			continue;
		}

		if (m_pnObjectNumber[i] < 0 || m_pnObjectNumber[i]>100)
		{
			sError.iStatusType = STATUS_ILG;
			TCHAR chInfo[c_iMaxStringLen]{ 0 };
			_tprintf_s(chInfo, tr("CObjAlloc::Initialize Wrong Number of Objects of Camera%d"), i + 1);
			sError.pErrorInfo = GetErrorInfo(chInfo);
			return sError;
		}
		//strDllPath = m_strArrDllPath.GetAt(n);
		// 		BOOL bExist = PathFileExists(strDllPath);
		// 		SetCurrentDirectory(_T("D:\\Workspace\\Projects\\DahengProjects\\CheckSystem\\CheckSystem2\\bin"));
		HMODULE hDll = LoadLibrary(strDllPath);
		if (hDll)
		{
			// ���������
			pFunc_Create createobj;
			createobj = (pFunc_Create)(GetProcAddress(hDll, _T("create_instance")));
			if (createobj)
			{
				if (m_bEnableInitObjectAndCheck)
				{
					CInspBase* pInsp = createobj(m_pnObjectNumber[i]);
					if (NULL == pInsp)
					{
						sError.iStatusType = STATUS_ILG;
						sError.pErrorInfo = GetErrorInfo(tr("CObjAlloc::Initialize Create Inspection Objects Failed!"));
						FreeLibrary(hDll);
						return sError;
					}
					m_iIdleJob[i] = (int)pInsp;
				}
			}
			else
			{
				AfxMessageBox(CExFn::GetLastErrorMsg());
				sError.iStatusType = STATUS_ILG;
				char chInfo[c_iMaxStringLen]{ 0 };
				sprintf_s(chInfo, tr("CObjAlloc::Initialize Execute Create Function Fail Camera%d"), i + 1);
				sError.pErrorInfo = GetErrorInfo(chInfo);
				FreeLibrary(hDll);
				return sError;
			}
		}
		else
		{
			sError.pErrorInfo = GetErrorInfo(CExFn::GetLastErrorMsg().GetBuffer());
			sError.iStatusType = STATUS_ILG;
			return sError;
		}
	}





	// ������ʼ��
	int nObjIdx = 0;
	try
	{
		// ֻ�м��˵��ó�ʼ������Init()
		if (e_CallType_Demo != sInitParam.iCallType)
		{
			for (i = 0; i < m_iCheckGroupNum; ++i)
			{
				if (-1 != m_iSingleTest)
				{
					if (i != m_iSingleTest)
						continue;
				}

				strDllPath = m_strArrDllPath.GetAt(i);
				if (strDllPath == _T("NULL"))
				{
					continue;
				}
				s_CheckInitSingleParam sSingleInit;
				sSingleInit.iCheckSN = i;//+1;
										 // 				sSingleInit.pParam = ((DWORD*)sInitParam.pParam+i);  //ÿ�ε��ô��Ĳ�������һ����
										 // 				sSingleInit.pParam = sInitParam.pParam;				 //[2016-5-23] �۽���Ҫ��ÿ�δ��������ַ��ͬ

				if (m_iMachineType == e_MachineType_Capsule)
				{
					sSingleInit.pParam = sInitParam.pParam;				//����ʹ��
				}
				else if (m_iMachineType == e_MachineType_Liquor)
				{
					sSingleInit.pParam = (int*)sInitParam.pParam + i;		//[2016-5-23] �۽���Ҫ��ÿ�δ��������ַ��ͬ
				}

				for (int nObjIdx = 0; nObjIdx < m_pnObjectNumber[i]; ++nObjIdx)
				{
					CInspBase* pInsp = (CInspBase*)m_iIdleJob[i] + nObjIdx;
					sSingleInit.iObjIndex = nObjIdx;
					if (m_bEnableInitObjectAndCheck)
						sError = pInsp->Init(sSingleInit);
					if (sError.iStatusType != STATUS_NOR)
					{
						return sError;
					}
				}
			}
		}
	}
	catch (...)
	{
		DWORD dError = GetLastError();
		sError.iStatusType = STATUS_ILG;
		char chInfo[MAX_CHAR_LENGTH]{ 0 };
		sprintf(chInfo, "CObjAlloc::Initialize Init Fail Check%d Object%d\r\nError Code: %d", i + 1, nObjIdx + 1, dError);
		sError.pErrorInfo = GetErrorInfo(chInfo);
		return sError;
	}

	//������ʾ����
	//	DisplayObjectsStatus(SW_SHOW);

	/************************************************************************/
	/*       ����������ͼ�߳�                [6/6/2017 ConanSteve]                         */
	if (e_CallType_Demo != m_iCallType)
	{
		m_hSaveImgs = (HANDLE)_beginthreadex(NULL, 0, (unsigned(_stdcall*)(void*))FnThreadSaveImg, this, 0, NULL);
		/** ��������¼�              [6/12/2017 ConanSteve]**/
		m_hEventSaveImgs = CreateEvent(NULL, FALSE, FALSE, NULL);
		CloseHandle(m_hSaveImgs);
	}
	/************************************************************************/
	ReseMultiObject();
	return sError;
}


s_Status CDataClass::ShowDetail(int iCheckSN)
{
	s_Status sError;
	ZeroMemory(&sError, sizeof(s_Status));
	sError.iStatusType = STATUS_NOR;
	sError.pThis = m_pCallThis;

	if (0 == iCheckSN)
	{
		m_bShowStatus = TRUE;
		m_pObjStatusDlg->ShowWindow(SW_SHOW);

	}
	else
	{
		m_bShowStatus = FALSE;
		m_pObjStatusDlg->ShowWindow(SW_HIDE);

	}

	return sError;
}

s_Status CDataClass::Set(s_AlgModelInput sAlgModelInput)
{
	s_Status sError;
	if (!m_bEnableInitObjectAndCheck)
		return sError;
	ZeroMemory(&sError, sizeof(s_Status));
	sError.iStatusType = STATUS_NOR;
	sError.pThis = m_pCallThis;

	int i = 0, j = 0;

	int iCheckSN = sAlgModelInput.iCheckSN;
	int iOpera = sAlgModelInput.iOperSig;
	int iImageType = sAlgModelInput.iReserve1;	//����ͼ��ʱ����ָ����ͼ����
	int iImageCount = sAlgModelInput.iReserve2;	//����ͼ��ʱ����ָ����ͼ����

	ASSERT(iCheckSN >= 0 && iCheckSN <= m_iCameraNum);

	if (-1 != m_iSingleTest)
	{
		if (iCheckSN != m_iSingleTest)
			return sError;
	}
	switch (iOpera)
	{
	case e_AlgModelInputOper_LoadModel:
	{
		fill(m_arr_bIsLoadModel.begin(), m_arr_bIsLoadModel.end(), true);
		for (i = 0; i < m_pnObjectNumber[iCheckSN]; ++i)
		{
			if (!m_bEnableInitObjectAndCheck)
				continue;
			CInspBase* pInsp = (CInspBase*)(m_iIdleJob[iCheckSN]) + i;
			CExtenInfo extenInfo;
			extenInfo.push_back(COptionalFunc(e_Fn_AlgModelInput_CheckObjIndex).SetInt(i));
			extenInfo.push_back(COptionalFunc(e_Fn_AlgModelInput_FirstCheckObjAddr).SetPointer((void*)m_iIdleJob));
			//sAlgModelInput.iReserve4 = (int)(&extenInfo);
			{

				int iTemp = 0;
				while (m_nObjIndex[iCheckSN][i] == e_DetectStatus_Checking)
				{
					Sleep(1);
					iTemp++;
					if (iTemp > 2000)
					{
						fill(m_arr_bIsLoadModel.begin(), m_arr_bIsLoadModel.end(), false);
						sError.iStatusType = STATUS_ALG_ABNORMAL;
						return sError;
					}
				}
			}
			sError = pInsp->Set(sAlgModelInput);
			if (sError.iStatusType != STATUS_NOR)
				return sError;
		}
		fill(m_arr_bIsLoadModel.begin(), m_arr_bIsLoadModel.end(), false);
	}
	break;
	case e_AlgModelInputOper_SaveImage:
	{
		//�鿴�Ƿ��ͼ
		if (iImageCount > 0)
		{
			m_csSaveImage[iCheckSN].Lock();
			m_bGroupSaveImage[iCheckSN] = true;
			m_iGroupSaveImageType[iCheckSN] = iImageType;
			m_iGroupSaveImageCount[iCheckSN] = 0;
			m_iGroupSaveImageCount_SET[iCheckSN] = iImageCount;
			m_csSaveImage[iCheckSN].Unlock();

			m_iSaveImageStatus = 1;

			for (i = 0; i < MAX_CAMERA_COUNT; i++)
			{
				m_iSaveImageNum[i] = 1;
			}
		}
		else
		{
			m_csSaveImage[iCheckSN].Lock();
			m_bGroupSaveImage[iCheckSN] = false;
			m_iGroupSaveImageCount[iCheckSN] = 0;
			m_iGroupSaveImageCount_SET[iCheckSN] = 0;
			m_csSaveImage[iCheckSN].Unlock();
		}
		m_iSaveImgObj = sAlgModelInput.iReserve3;
		m_iSaveImgNamingRegulation = sAlgModelInput.iReserve4;

		m_srwlSaveImgHistory.Lock();
		while (!m_queSaveImgHistory.empty())
			m_queSaveImgHistory.pop();
		m_srwlSaveImgHistory.Unlock();
	}
	break;
	//**[7/31/2017 ConanSteve]**:  �鿴�����������Ϣ
	case e_AlgModelInputOper_UseExtenInfoFunc:
	{
		CExtenInfo* pExtenInfo{ NULL };
		pExtenInfo = (CExtenInfo*)sAlgModelInput.iReserve4;
		if (NULL == pExtenInfo)
			return sError;
		{
			m_bShowObjRunInfoWnd = pExtenInfo->GetFuncStruct(e_Fn_AlgModelInput_WhetherShowObjectRunInfo).GetBool();
			if (m_bShowObjRunInfoWnd)
			{
				m_pDlgRunInfo->ShowWindowEx(SW_SHOW);
			}
			else
			{
				m_pDlgRunInfo->ShowWindowEx(SW_HIDE);
			}
		}
	}
	break;
	case e_AlgModelInputOper_Reset:
	{
		ReseMultiObject();
	}
	break;
	default:
	{
		for (i = 0; i < m_pnObjectNumber[iCheckSN]; ++i)
		{
			if (!m_bEnableInitObjectAndCheck)
				continue;
			CInspBase* pInsp = (CInspBase*)(m_iIdleJob[iCheckSN]) + i;
			if (NULL != pInsp)
				sError = pInsp->Set(sAlgModelInput);
			if (sError.iStatusType != STATUS_NOR)
				return sError;
		}
	}
	break;
	}



	return sError;
}

s_Status CDataClass::Free()
{
	s_Status sError;
	if (!m_bEnableInitObjectAndCheck)
		return sError;
	ZeroMemory(&sError, sizeof(s_Status));
	sError.iStatusType = STATUS_NOR;
	sError.pThis = m_pCallThis;

	ReleaseThreadData();

	return sError;
}

void CDataClass::DisplayObjectsStatus(int nCmdShow) //����״̬д�ļ�����ʾ
{
	// 	if (nCmdShow == SW_HIDE)
	// 	{
	// 		m_bShowStatus = FALSE;
	// 	}
	// 	else if (nCmdShow == SW_SHOW)
	// 	{
	// 		m_bShowStatus = TRUE;
	// 	}
}
CString ConvertErrorCodeToString(DWORD ErrorCode)
{
	CString sErrMsg{ _T("") };
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, ErrorCode, 0, (PTSTR)sErrMsg.GetBuffer(1000), 0, NULL);
	sErrMsg.ReleaseBuffer();
	return sErrMsg;
}
BOOL CDataClass::InitObjects(CString strInitFile, int iCallType)//���Զ�̬���Ƿ���Ե���
{
	/************************************************************************/
	/*                       [6/6/2017 ConanSteve]                         */
	/************************************************************************/
	//�������ļ���ȡ�����·������̬����
	//array<char, MAX_PATH> chBuf{0};
	CString strSegment{ _T("") };
	CString sVal{ _T("") };
	strSegment.Format(_T("Inspection Parameters"));
	GetPrivateProfileString(strSegment, "CheckNum", "5", sVal.GetBuffer(80), 80, strInitFile);
	sVal.ReleaseBuffer();
	/************************************************************************/
	m_iniAlgCfg.Init(CExFn::GetCurrentAppPath() + _T("\\Config\\AlgConfig.ini"));
	m_bEnableExtenInfo = m_iniAlgCfg.GetBoolEx(_T("SystemConfiguration"), _T("EnableExtenInfo"), false, _T("�Ƿ�ʹ�ö������չ��Ϣ�ṹ��"));
	m_bEnableOvertimeCallback = m_iniAlgCfg.GetBoolEx(_T("Systemconfiguration"), _T("EnableOvertimeCallback"), true, _T("�Ƿ�ʹ�ó�ʱ�ص�"));
	m_iSysDetectMode = m_iniAlgCfg.GetIntEx(_T("SystemConfiguration"), _T("SystemDetectMode"), e_SysDetectMode_Multi, _T("ϵͳ���ģʽ,1Ϊ�������ͼ��⡣0Ϊϵͳ��ͼ"));
	m_iObjDetectMode = m_iniAlgCfg.GetIntEx(_T("SystemConfiguration"), _T("ObjectDetectMode"), e_ObjDetectMode_1, _T("0Ϊ1��Detect��1Ϊ2��Detect"));
	m_bShowObjRunInfoWnd = m_iniAlgCfg.GetBoolEx(_T("SystemConfiguration"), _T("EnableObjectRunInfoWindow"), false, _T("�Ƿ�򿪴���������Ϣ"));
	m_bEnableXCGUI = m_iniAlgCfg.GetBoolEx(_T("SystemConfiguration"), _T("EnableXCGUI"), false, _T("�Ƿ�ʹ��XCGUI���"));
	m_iSingleTest = m_iniAlgCfg.GetIntEx(_T("SystemConfiguration"), _T("SingleTest"), -1, _T("���е�·���ԣ�-1ʱ����������,��0��ʼ"));
	m_bEnableInitObjectAndCheck = m_iniAlgCfg.GetBoolEx(_T("SystemConfiguration"), _T("EnableInitMultiObjectAndCheck"), true, _T("�Ƿ�ִ�ж����ĳ�ʼ�����㷨��⹤��"));
	/************************************************************************/
	m_iCheckGroupNum = _ttoi(sVal.GetBuffer());
	if (m_iCheckGroupNum <= 0)
		return FALSE;

	int iTemp = 0;

	//�������ļ����������
	CString strAlg{ _T("") };
	int nCheckObjNum = 0;
	int i = 0, j = 0;
	m_pnObjectNumber = new int[m_iCheckGroupNum + 1]{ 0 };
	for (i = 0; i < m_iCheckGroupNum; ++i)
	{
		if (iCallType == e_CallType_Demo)//demo����
		{
			m_pnObjectNumber[i] = 1;
		}
		else
		{
			strAlg.Format(_T("CheckObjNum_%d"), i + 1);
			GetPrivateProfileString(strSegment, strAlg, _T("1"), sVal.GetBuffer(80), 80, strInitFile);
			sVal.ReleaseBuffer();
			nCheckObjNum = _ttoi(sVal.GetString());
			m_pnObjectNumber[i] = nCheckObjNum;
		}
	}

	// �����ҵ�ÿ����̬��Ľӿ�
	CString strDllKeyName{ _T("") }, strDllPath{ _T("") };
	CString strBuf{ _T("") };
	CString sDebugInfo{ _T("") };
	for (i = 0; i < m_iCheckGroupNum; ++i)
	{
		strDllKeyName.Format(_T("CheckDllName_%d"), i + 1);
		sDebugInfo.Format(tr("CheckDllName_%d does not exist!"), i + 1);
		sVal = m_iniAlgCfg.GetStringEx(strSegment, strDllKeyName, sVal, _T("NULL"));
		if (sVal == _T("NULL") || _T("") == sVal)//��ǰ·��̬��Ϊ�գ�����һ��
		{
			m_strArrDllPath.InsertAt(i, _T("NULL"));
			AfxMessageBox(sDebugInfo);
			return false;
		}
		strDllPath.Format(_T("%s\\%s"), m_strAppDir.GetBuffer(0), sVal.GetString());
		if (0 == strDllPath.Right(4).CompareNoCase(_T(".dll")))
			strDllPath = strDllPath.Left(strDllPath.GetLength() - 4);
#ifdef DEBUG
		strDllPath += _T("D");
#endif

		if (strDllPath.Right(4) != _T(".dll") && strDllPath.Right(4) != _T(".DLL"))
			strDllPath += _T(".dll");

		bool bRet = PathFileExists(strDllPath.GetBuffer());
		if (false == bRet)
		{
			CString sInfo{ _T("") };
			sInfo.Format(tr("�㷨��⶯̬��_%d\"%s\" ������!"), i + 1, strDllPath);
			AfxMessageBox(sInfo);
			return false;
		}
		else
			m_strArrDllPath.InsertAt(i, strDllPath);
	}

	// ��ȡ����̸߳���
	strSegment.Format(_T("Basic Parameters"));
	m_iCheckThreadNum = m_iniAlgCfg.GetIntEx(_T("Basic Parameters"), _T("CheckThreadNum"), 10, _T("����̸߳���"));

	// ��ȡ�������

	m_iCameraNum = m_iniAlgCfg.GetIntEx(_T("Basic Parameters"), _T("CameraNum"), 1, _T("�������"));

	// ��ȡ�޷϶��и���
	strSegment.Format(_T("Basic Parameters"));
	m_iKickQueueNum = m_iniAlgCfg.GetIntEx(_T("Basic Parameters"), _T("KickQueueNum"), 3);

	/** Last checking group[5/3/2017 ConanSteve]**/
	int iLastGroup = MAX_IMAGE_COUNT;
	/** the current index in checking group              [5/3/2017 ConanSteve]**/
	int	iGroupIndex = 0;
	int iLastKick = MAX_IMAGE_COUNT, iKickIndex = 0, iLastKickSize = 0;
	int iKickResult = 0, iLastKickResult = 0;
	for (i = 0; i < m_iCameraNum; i++)
	{
		// ��ȡͼ����λ��
		strSegment.Format(_T("Camera%d Parameters"), i + 1);
		strAlg.Format(_T("Width"));
		GetPrivateProfileString(strSegment, strAlg, _T("1292"), sVal.GetBuffer(80), 80, strInitFile);
		sVal.ReleaseBuffer();
		m_iImageWidth[i] = _ttoi(sVal.GetString());

		m_iImageHeight[i] = m_iniAlgCfg.GetIntEx(strSegment, _T("Height"), 954, _T("ͼ��߶�"));

		//*[6/30/2017 ConanSteve]*:  ��ȡ��ǰ�����ͼ������Сͼ����
		m_arr_iCamSmallImgNum[i] = GetPrivateProfileInt(strSegment, _T("PictureNumberPerCamera"), 1, strInitFile);

		strAlg.Format(_T("Byte"));
		m_iImageByte[i] = m_iniAlgCfg.GetIntEx(strSegment, strAlg, 1, _T("ÿ�����ֽ���"));

		m_iImageSize[i] = m_iImageWidth[i] * m_iImageHeight[i] * m_iImageByte[i];

		// ��������
		strAlg.Format(_T("CheckGroup"));
		GetPrivateProfileString(strSegment, strAlg, _T("1"), sVal.GetBuffer(80), 80, strInitFile);
		sVal.ReleaseBuffer();
		m_iCamGroup[i] = (_ttoi(sVal.GetString()) - 1);

		// ������ڵ��޷϶��б��
		strAlg.Format(_T("KickGroup"));
		GetPrivateProfileString(strSegment, strAlg, _T("1"), sVal.GetBuffer(80), 80, strInitFile);
		sVal.ReleaseBuffer();
		m_iCamKick[i] = (_ttoi(sVal.GetString()) - 1);

		// ��������޷϶����е�ƫ����
		strAlg.Format(_T("KickOffset"));
		GetPrivateProfileString(strSegment, strAlg, _T("10"), sVal.GetBuffer(80), 80, strInitFile);
		sVal.ReleaseBuffer();
		m_iCamKickOffsetY[i] = _ttoi(sVal.GetString());


		// ��������㷨�������
		strAlg.Format(_T("CheckOutNum"));
		GetPrivateProfileString(strSegment, strAlg, _T("1"), sVal.GetBuffer(80), 80, strInitFile);
		sVal.ReleaseBuffer();
		m_iCheckOutNum[i] = _ttoi(sVal.GetString());

		// ÿ������������ṹ��void* pParam���ֽڴ�С
		strAlg.Format(_T("CheckOutSize"));
		GetPrivateProfileString(strSegment, strAlg, _T("300"), sVal.GetBuffer(80), 80, strInitFile);
		sVal.ReleaseBuffer();
		iTemp = _ttoi(sVal.GetString());
		m_iCheckOutSize[i] = m_iCheckOutNum[i] * iTemp;


		// ÿ���������ṹ��void* pParam��С
		strAlg.Format(_T("CheckInSize"));
		GetPrivateProfileString(strSegment, strAlg, _T("400"), sVal.GetBuffer(80), 80, strInitFile);
		sVal.ReleaseBuffer();
		m_iCheckInSize[i] = _ttoi(sVal.GetString());

		// ÿ��״̬�ṹ��void* pParam��С
		strAlg.Format(_T("CheckStatusSize"));
		m_iCheckStatusSize[i] = m_iniAlgCfg.GetIntEx(strSegment, strAlg, 200, _T("�㷨���״̬�ṹ���е�void* pParam��С���ֽ�Ϊ��λ��"));

		// ͳ��ÿ�������������
		m_iGroupElementNum[m_iCamGroup[i]]++;

		// ͳ��ÿ���޷϶����������
		m_iKickElementNum[m_iCamKick[i]]++;

		// ͳ��ÿ��������ͼ�����ݴ�С
		m_iGroupImageSize[m_iCamGroup[i]] += m_iImageSize[i];

		//*[6/30/2017 ConanSteve]*:  ������Ʒ����ͼ��Ĵ�С֮��
		m_iSizeOfOneArrImgs += m_iImageSize[i];

		//*[7/14/2017 ConanSteve]*:  ����ÿ���������ͼ�Ĵ�С
		m_iSmallImgSize[i] = m_iImageSize[i] / m_arr_iCamSmallImgNum[i];

		// ͳ��ÿ��������е��㷨���������� void* pParam��С
		m_iGroupCheckInSize[m_iCamGroup[i]] += m_iCheckInSize[i];

		// ͳ��ÿ��������е��㷨���������� void* pParam��С
		m_iGroupCheckOutSize[m_iCamGroup[i]] += m_iCheckOutSize[i];

		// ͳ��ÿ��������е��㷨��ⷵ��״̬���� void* pParam��С
		m_iGroupCheckStatusSize[m_iCamGroup[i]] += m_iCheckStatusSize[i];

		// ͳ��ÿ��������е��㷨����������ĸ���
		m_iGroupCheckOutNum[m_iCamGroup[i]] += m_iCheckOutNum[i];

		// ͳ��ÿ���޷϶�����ÿ��Ԫ�ص��ֽڴ�С
		m_iKickQueueSize[m_iCamKick[i]] += m_iCheckOutSize[i];

		// ͳ��ÿ���޷϶��н������
		m_iKickResultNum[m_iCamKick[i]] += m_iCheckOutNum[i];

		// ��ȡ��ǰ��ĵ�һ�������
		if (1 == m_iGroupElementNum[m_iCamGroup[i]])
		{
			m_iGroupFirstCam[m_iCamGroup[i]] = i;
		}

		// ��ȡ��ǰ�޷϶����е�һ��������
		if (1 == m_iKickElementNum[m_iCamKick[i]])
		{
			m_iKickFirstCam[m_iCamKick[i]] = i;
		}

		// ÿ������ڶ�Ӧ���е����
		if (m_iCamGroup[i] != iLastGroup)
		{
			iGroupIndex = 0;
			m_iIndexInGroup[i] = iGroupIndex;
			iLastGroup = m_iCamGroup[i];
		}
		else
		{
			iGroupIndex++;
			m_iIndexInGroup[i] = iGroupIndex;
		}


		// ÿ�������һ������ڶ�Ӧ���޷϶����е����
		if (m_iCamKick[i] != iLastKick)
		{
			iKickIndex = 0;
			m_iIndexInKick[i] = iKickIndex;

			iLastKickSize = 0;
			m_iCamKickOffsetX[i] = iLastKickSize;
			iLastKickSize = m_iCheckOutSize[i];

			iLastKickResult = 0;
			m_iCamKickOffsetX_Written[i] = iLastKickResult;
			iLastKickResult = m_iCheckOutNum[i];

			iLastKick = m_iCamKick[i];
		}
		else
		{
			iKickIndex++;
			m_iIndexInKick[i] = iKickIndex;

			m_iCamKickOffsetX[i] = iLastKickSize;
			iLastKickSize += m_iCheckOutSize[i];

			m_iCamKickOffsetX_Written[i] = iLastKickResult;
			iLastKickResult += m_iCheckOutNum[i];
		}
	}

	// ���ÿ���޷϶������޷�ƫ������С�������
	//**[7/31/2017 ConanSteve]**:  ���޷�ƫ�����������
	array<int, MAX_CAMERA_COUNT> iMinKickSN{ 0 };
	for (i = 0; i < MAX_CAMERA_COUNT; i++)
	{
		iMinKickSN[i] = MAX_IMAGE_COUNT;
		m_iMaxOffsetOfKick[i] = 0;
	}

	for (i = 0; i < m_iCameraNum; i++)
	{
		for (j = 0; j < m_iKickQueueNum; j++)
		{
			if (m_iCamKick[i] == j)
			{
				if (m_iCamKickOffsetY[i] < iMinKickSN[j])
				{
					iMinKickSN[j] = m_iCamKickOffsetY[i];
					m_iTimeOutIndex[j] = i;
				}
				if (m_iCamKickOffsetY[i] > m_iMaxOffsetOfKick[j])
				{
					m_iMaxOffsetOfKick[j] = m_iCamKickOffsetY[i];
				}
			}
		}
	}
	for (int i = 0; i < m_iKickQueueNum; ++i)
	{
		m_iKickRemediation[i] = (-1)*m_iMaxOffsetOfKick[i];
	}
	// ��֤ÿ����������ṹ����void* pParam��С ������ÿ�������С
	int iResult = 0;
	for (i = 0; i < m_iCheckGroupNum; i++)
	{
		if (m_iGroupCheckOutNum[i] > 0)
		{
			iResult = m_iGroupCheckOutSize[i] % m_iGroupCheckOutNum[i];

			if (0 != iResult)
			{
				// ÿ�������С��Ϊ0 ˵������������
				AfxMessageBox(_T("Error Code: 2!"));
				return FALSE;
			}
			else
			{
				m_iGroupCheckOutPerResultSize[i] = m_iGroupCheckOutSize[i] / m_iGroupCheckOutNum[i];
			}
		}
		else
		{
			m_iGroupCheckOutPerResultSize[i] = 0;
		}
	}

	// ��ȡ�����в���
	for (i = 0; i < m_iCheckGroupNum; i++)
	{
		strSegment.Format(_T("CheckGroup%d"), i + 1);
		m_iGroupCheckQueueNum[i] = m_iniAlgCfg.GetIntEx(strSegment, _T("ElementNum"), 2, _T("ÿ������黺����еĳ���"));
	}

	// ��ȡ�޷϶��в���
	for (i = 0; i < m_iKickQueueNum; i++)
	{
		strSegment.Format(_T("KickGroup%d"), i + 1);
		strAlg.Format(_T("TimeType"));
		m_iTimeType[i] = m_iniAlgCfg.GetIntEx(strSegment, strAlg, 0, _T("�޷϶���1��ʱ��׼ 0Ϊʱ�䣬��λΪ���룻1Ϊ֡/�У���ʱ��λΪ֡����/��������Ƶ"));

		strAlg.Format(_T("TimeOut"));
		CString sAnnotation = 0 == m_iTimeType[i] ? _T("����") : _T("֡/��");
		m_iTimeOut[i] = m_iniAlgCfg.GetIntEx(strSegment, strAlg, 120);
	}


	// ��ͬ���֮�����С���
	strSegment.Format(_T("Other Parameters"));
	strAlg.Format(_T("MinIntervalOfSameCamera"));
	m_iCallInterval = m_iniAlgCfg.GetIntEx(strSegment, strAlg, 20, _T("ͬһ���ͬһͼ�����С�������"));

	strSegment.Format(_T("Other Parameters"));
	strAlg.Format(_T("MinIntervalOfDiffPeriod"));
	m_iPeriodInterval = m_iniAlgCfg.GetIntEx(strSegment, strAlg, 30);

	strSegment.Format(_T("Other Parameters"));
	strAlg.Format(_T("bWriteLog"));
	m_bWriteLog = m_iniAlgCfg.GetBoolEx(strSegment, strAlg, false);

	strSegment.Format(_T("Other Parameters"));
	strAlg.Format(_T("MaxLogNum"));
	m_iMaxLogLineNum = m_iniAlgCfg.GetIntEx(strSegment, strAlg, 10000);

	strSegment.Format(_T("DEMO Controls"));
	strAlg.Format(_T("VirtualDisk"));
	GetPrivateProfileString(strSegment, strAlg, _T("M"), m_strVitrualDisk.GetBuffer(5), 5, m_strAlgconfigDir);
	m_strVitrualDisk.ReleaseBuffer();
	WritePrivateProfileString(strSegment, strAlg, m_strVitrualDisk, m_strAlgconfigDir);


	// ������־�ļ�

	if (e_CallType_Demo != m_iCallType && TRUE == m_bWriteLog)
	{
		if (!CreateLogFile())
		{
			return FALSE;
		}
	}
	//**[7/26/2017 ConanSteve]**:  ��ȡ�޷ϱ��Ĭ��ֵ
	m_iDefaultCheckRslt = m_iniAlgCfg.GetIntEx(_T("SystemConfiguration"), _T("DefaultCheckResult"), -3, _T("�޷ϱ�Ĭ�ϼ����"));
	m_iLostImageRslt = m_iniAlgCfg.GetIntEx(_T("SystemConfiguration"), _T("DefaultLostImageResult"), -4, _T("�޷ϱ��ڶ����������ͼʱд�ļ����"));
	//**[8/1/2017 ConanSteve]**:  ��ȡ�㷨�쳣�ļ����
	m_iAlgExceptionVal = GetPrivateProfileInt(_T("SystemConfiguration"), _T("AlgorithmExceptionValue"), 0, m_strAlgconfigDir);

	return TRUE;
}

UINT CDataClass::ThreadFunc_ChangeStatus(LPVOID lParam)
{
	int nWS = ((int*)lParam)[0];
	int nObj = ((int*)lParam)[1];
	int nFunc = ((int*)lParam)[2];

	if (nFunc >= 0 && nFunc <= 2)
		m_pObjStatusDlg->SendMessage(1700 + nFunc, nWS * 1000 + nObj);

	return 1;
}

void CDataClass::GetLogDirPath(CString& strDir)
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);

	strDir = m_strAppDir + _T("\\LogFiles");
	if (TRUE != PathFileExists(strDir))
		CreateDirectory(strDir.GetBuffer(0), 0);
	strDir += _T("\\InspectObjsLog");
	if (TRUE != PathFileExists(strDir))
		CreateDirectory(strDir.GetBuffer(0), 0);
	CString strTimeDir;
	strTimeDir.Format(_T("\\%04d-%02d-%02d"), systime.wYear, systime.wMonth, systime.wDay);
	strDir += strTimeDir;
	if (TRUE != PathFileExists(strDir))
		CreateDirectory(strDir.GetBuffer(0), 0);
	strTimeDir.Format(_T("\\%02d-%02d-%02d"), systime.wHour, systime.wMinute, systime.wSecond);
	strDir += strTimeDir;
	if (TRUE != PathFileExists(strDir))
		CreateDirectory(strDir.GetBuffer(0), 0);
}

s_Status CDataClass::Detect(s_CheckInParam sCheckInParam)
{

	s_Status sError;
	if (m_arr_bIsLoadModel[sCheckInParam.iCheckSN])//**[1/7/2018 ConanSteve]**:	����������¼���ģ�壬ֱ�ӷ��� 
		return sError;

	if (!m_bEnableInitObjectAndCheck)
		return sError;
	if (-1 != m_iSingleTest)
	{
		if (sCheckInParam.iCheckSN != m_iSingleTest)
			return sError;
	}
	/************************************************************************/
	/*                       [6/29/2017 ConanSteve]                         */
	m_iSysDetectMode = sCheckInParam.iReserve1;
	//**[9/15/2017 ConanSteve]**:  �ж��Ƿ�ʹ������չ��Ϣ�ṹ�壬���û�ã���iReserve4��Ϊ0����ֹδ��ʼ���ı���
	if (!m_bEnableExtenInfo)
		sCheckInParam.iReserve4 = 0;
	if (e_SysDetectMode_Single == m_iSysDetectMode)
	{
		return DetectEx(sCheckInParam);
	}
	/************************************************************************/
	char chInfo[MAX_CHAR_LENGTH]{ 0 };
	array<char, MAX_CHAR_LENGTH> chLogInfo{ 0 };

	int i = 0, j = 0;
	int iComplete = 0;
	int iCurrIndex = 0, iMinIndex = 0;
	int iMinCircleSN = MAX_IMAGE_COUNT;

	bool bBusy = true;

	float fDiff = 0;
	LONGLONG lInfo = 0;
	LARGE_INTEGER liValue = { 0 };

	sError.iStatusType = STATUS_NOR;
	sError.pThis = m_pCallThis;
	sError.pErrorInfo = NULL;

	// ��ֹͣ������ٵ���
	if (m_bKillThread)
	{
		sError.iStatusType = STATUS_ILG;
		sError.pErrorInfo = GetErrorInfo("Kill Thread!");
		return sError;
	}


	// 	// ���ݿͻ����þ����Ƿ���ʾ����ʹ�����
	// 	ShowCurrStatus();

	if (e_CallType_Demo != m_iCallType)
	{
		// �����
		int iCheckSN = sCheckInParam.iCheckSN;

		// ͼ���
		int iImageSN = sCheckInParam.iImageSN;

		// ���
		int iGroupSN = m_iCamGroup[iCheckSN];

		// ����������˳���
		int iOrderSN = m_iIndexInGroup[iCheckSN];

		// ���к�
		int iKickSN = m_iCamKick[iCheckSN];

		ASSERT(iCheckSN >= 0 && iCheckSN < m_iCameraNum);



		// ÿһ·���������ĸ���
		m_iCamCircle[iCheckSN]++;
		int iCircleSN = m_iCamCircle[iCheckSN];

		QueryPerformanceCounter(&liValue);
		lInfo = liValue.QuadPart;

		// ��ǰ�������ͼ������һ��ͼ���ʱ���
		m_csCameraDiff[iCheckSN].Lock();
		fDiff = 1000 * (lInfo - m_lCamTime[iCheckSN][(iCircleSN - 1) % 256]) / m_dFreq;
		m_lCamTime[iCheckSN][iCircleSN % 256] = lInfo;
		m_csCameraDiff[iCheckSN].Unlock();

		CString sInfo{ _T("") };
		sInfo.Format(_T("[�����:%2d] [ͼ���:%8d] [������:%2d] [�޷Ϻ�:%2d] [�����������:%2d] [�޷�����:%2d] [������:%8d]"), iCheckSN,
			iImageSN, iGroupSN, iKickSN + 1, iOrderSN, m_iIndexInKick[iCheckSN], iCircleSN);
		m_fileCam[iCheckSN][0].Write(sInfo);


		/** ������������Ǹ��޷϶��е����߷Ͽ�����������ţ���ʼ���޷ϳ�ʱ��ʱ              [5/3/2017 ConanSteve]**/
		if (m_iTimeOutIndex[iKickSN] == iCheckSN)
		{
			m_iKickImageSN[iKickSN][iCircleSN % 256] = iImageSN;

			// ��ʱ��׼��ʱ��
			if (m_bEnableOvertimeCallback)
			{
				if (e_TimeType_MilliSecond == m_iTimeType[iKickSN])
				{
					m_spTime[iKickSN][iCircleSN % 256].Start();
				}
			}
			m_iKickCircleSN[iKickSN] = iCircleSN;
		}

		//**[7/19/2017 ConanSteve]**:   �ж��Ƿ���һ��������
		if (TRUE == JudgeNewCircle(iGroupSN, iCheckSN, iImageSN, iCircleSN))
		{
			//**[7/28/2017 ConanSteve]**:  ��ͼд�޷ϱ�Ĭ�Ͻ��
			ThreadWriteResult(iImageSN, iGroupSN, iKickSN, false);
			//**[7/19/2017 ConanSteve]**:   �ж���һ�������Ƿ����,��δ��ɣ���ͼ���
			int iLastImageSN = m_iPeriodImageSN[iGroupSN][m_iGroupCircle[iGroupSN] % 256];
			int iLastIndex = m_iPeriodIndex[iGroupSN][m_iGroupCircle[iGroupSN] % 256];

			iComplete = 0;
			/** �ж�ָ�������ͼ���Ƿ���              [5/5/2017 ConanSteve]**/
			for (i = 0; i < m_iGroupElementNum[iGroupSN]; i++)
			{
				if (m_bCamComplete[m_iGroupFirstCam[iGroupSN] + i][iLastImageSN % 256])
				{
					iComplete++;
				}
			}

			//*[7/5/2017 ConanSteve]*:  д��־
			CString sInfo{ _T("") };
			sInfo.Format(_T("[�����:%2d] [ͼ���:%8d] [������:%2d] [�޷϶��к�:%2d] [��һͼ���:%8d] [��һ����:%2d] [�������ͼ�����:%8d] [�������ͼ����:%2d] [���������:%d]"),
				iCheckSN, iImageSN, iGroupSN, iKickSN, iLastImageSN, iLastIndex, m_iCamCircle[iCheckSN], iComplete,
				m_iGroupElementNum[iGroupSN]);
			m_fileDetect[iGroupSN][1].Write(sInfo);


			//**[7/19/2017 ConanSteve]**:   ����ʵ�ʲɼ���δ����ͼ����Ȼ��ȥ���
			if (m_iCallType == e_CallType_RealTime)
			{
				if (iComplete != m_iGroupElementNum[iGroupSN] && m_iCamCircle[iCheckSN] != 1)
				{
					// �������
					SetEvent(m_hImageInspectStart[iGroupSN]);

					m_csWriteDataStatus[iGroupSN].Lock();
					m_pWriteDataStatus[iGroupSN][iLastIndex] = e_WriteDataStatus_Complete;
					m_csWriteDataStatus[iGroupSN].Unlock();


					sInfo.Format(_T("[������:%2d] [�����:%2d] [��ǰͼ���:%08d] [��һ��λͼ���:%8d] [��һ��λ������:%2d]"),
						iGroupSN, iCheckSN, iImageSN, iLastImageSN, iComplete);
					m_fileDetect[iGroupSN][6].Write(sInfo);
				}
			}

			/*************************************<��ѯ�����п��ж���>************************************[10/9/2017 ConanSteve]*/
			m_csWriteDataStatus[iGroupSN].Lock();
			int iStatusBefore = 0;
			for (i = 0; i < m_iGroupCheckQueueNum[iGroupSN]; i++)
			{
				iStatusBefore = m_pWriteDataStatus[iGroupSN][i];
				if (m_pWriteDataStatus[iGroupSN][i] != e_WriteDataStatus_Leisure)
				{
					continue;
				}
				else
				{
					bBusy = false;
					iCurrIndex = i;
					m_pWriteDataStatus[iGroupSN][i] = e_WriteDataStatus_Busy;
					++m_ppCacheUseCnt[iGroupSN][i];
					break;
				}
			}
			m_csWriteDataStatus[iGroupSN].Unlock();


			/** д��־              [5/5/2017 ConanSteve]**/
			sInfo.Format(_T("[�����:%2d] [������:%2d] [ͼ���:%8d] [�����Ƿ�æ:%s] [�������Before����:%d] [����д״̬:%3s]"),
				iCheckSN, iGroupSN, iImageSN, bBusy ? _T("��") : _T("��"), iCurrIndex,
				e_WriteDataStatus_Leisure == iStatusBefore ? _T("����") : (e_WriteDataStatus_Busy == iStatusBefore) ? _T("����д") : _T("��д��"));
			m_fileDetect[iGroupSN][2].Write(sInfo);

			//������2 �����ȴ�������С���Ԫ�������޳������У�������������	
			// �滻��Сͼ���
			if (true == bBusy)
			{
				m_iGroupAbnormalNum[iCheckSN]++;

				m_csWriteDataStatus[iGroupSN].Lock();
				for (i = 0; i < m_iGroupCheckQueueNum[iGroupSN]; ++i)
				{
					if (iMinCircleSN > m_pCircleSN[iGroupSN][i] && m_pWriteDataStatus[iGroupSN][i] == e_WriteDataStatus_Complete && m_pDetectStatus[iGroupSN][i] == e_DetectStatus_NoChecking)
					{
						iMinCircleSN = m_pCircleSN[iGroupSN][i];
						iMinIndex = i;
					}
				}
				m_csWriteDataStatus[iGroupSN].Unlock();

				// ��δ�ڼ���Ԫ��
				if (MAX_IMAGE_COUNT != iMinCircleSN)
				{
					//�ɵ�ͼ��Ž����޷϶���
					iCurrIndex = iMinIndex;
					int iOldImageSN = m_spCheckInParam[iGroupSN][iMinIndex].iImageSN;

					m_iGroupCircle[iGroupSN]++;

					// ��ͼ���д�����
					ThreadWriteResult(iOldImageSN, iGroupSN, iKickSN, true);

					// ����״̬�ص� ֪ͨϵͳ
					TCHAR chStatusInfo[MAX_CHAR_LENGTH]{ 0 };
					s_Status sStatus;
					sStatus.iCheckSN = iCheckSN;
					sStatus.iStatusType = STATUS_THROW_IMAGE;
					sStatus.pThis = m_pCallThis;
					sStatus.pParam = NULL;
					_tprintf_s(chStatusInfo, _T("Checking Image No.%d, Throwing Image No.%d!"), iImageSN, iOldImageSN);
					sStatus.pErrorInfo = GetErrorInfo(chStatusInfo);
					m_StatusCallback(&sStatus);

					m_csWriteDataStatus[iGroupSN].Lock();
					m_pWriteDataStatus[iGroupSN][iCurrIndex] = e_WriteDataStatus_Busy;
					m_csWriteDataStatus[iGroupSN].Unlock();


					//����ͼ������ͼ��,����д��״̬
					m_spCheckInParam[iGroupSN][iCurrIndex].iCheckSN = iGroupSN;
					m_spCheckInParam[iGroupSN][iCurrIndex].iImageSN = iImageSN;
					m_spCheckInParam[iGroupSN][iCurrIndex].iReserve1 = sCheckInParam.iReserve1;
					m_spCheckInParam[iGroupSN][iCurrIndex].iReserve2 = sCheckInParam.iReserve2;
					m_spCheckInParam[iGroupSN][iCurrIndex].iReserve3 = sCheckInParam.iReserve3;
					m_spCheckInParam[iGroupSN][iCurrIndex].iReserve4 = sCheckInParam.iReserve4;
					LARGE_INTEGER liTime;
					QueryPerformanceCounter(&liTime);
					m_arr_ldTickOfPushInCacheQueue[iGroupSN][iCurrIndex]= liTime.QuadPart;

					m_pCircleSN[iGroupSN][iCurrIndex] = m_iGroupCircle[iGroupSN];

					m_iPeriodImageSN[iGroupSN][m_iGroupCircle[iGroupSN] % 256] = sCheckInParam.iImageSN;
					m_iPeriodIndex[iGroupSN][m_iGroupCircle[iGroupSN] % 256] = iCurrIndex;

					m_srwlImgQue[iGroupSN].Lock();
					BYTE* pAddr = (BYTE*)(m_spCheckInParam[iGroupSN][iCurrIndex].pImageData + iOrderSN * m_iImageSize[iCheckSN]);
					memcpy_s(pAddr, m_iImageSize[iCheckSN], sCheckInParam.pImageData, m_iImageSize[iCheckSN]);

					BYTE* pParam = (BYTE*)(m_spCheckInParam[iGroupSN][iCurrIndex].pParam);
					memcpy_s(pParam, m_iGroupCheckInSize[iGroupSN], sCheckInParam.pParam, m_iGroupCheckInSize[iGroupSN]);
					m_srwlImgQue[iGroupSN].Unlock();

					for (i = 0; i < m_iGroupElementNum[iGroupSN]; i++)
					{
						m_csCamComplete[m_iGroupFirstCam[iGroupSN] + i].Lock();
						m_bCamComplete[m_iGroupFirstCam[iGroupSN] + i][iImageSN % 256] = FALSE;
						m_csCamComplete[m_iGroupFirstCam[iGroupSN] + i].Unlock();
					}
					//**[7/18/2017 ConanSteve]**:  write logs
					sInfo.Format(_T("�����:%2d] [������:%2d] [��������:%2d] [�鴦��ͼ�����:%d] [��ͼ����:%d] [����ͼ���:%d] [��������:%d]"),
						iCheckSN, iGroupSN, iCurrIndex, m_iGroupCircle[iGroupSN], iMinCircleSN,
						m_iPeriodImageSN[iGroupSN][m_iGroupCircle[iGroupSN] % 256],
						m_iPeriodIndex[iGroupSN][m_iGroupCircle[iGroupSN] % 256]);
					m_fileDetect[iGroupSN][7].Write(sInfo);
				}
				else		//�޿��滻Ԫ��
				{
					//**[9/22/2017 ConanSteve]**:  ��ǰͼ��д�����
					ThreadWriteResult(iImageSN, iGroupSN, iKickSN, true);
					sError.iStatusType = STATUS_ILG;
					ZeroMemory(&chInfo[0], MAX_CHAR_LENGTH);

					CString str1{ _T("") }, str2{ _T("") };

					// ����
					str1.Format(_T("Cam:%d"), iGroupSN);

					// ͼ���
					str2.Format(_T("C:%d "), iImageSN);
					str1 += str2;

					// �������е����
					for (i = 0; i < m_iGroupCheckQueueNum[iGroupSN]; i++)
					{
						str2.Format(_T("%d "), m_pCircleSN[iGroupSN][i]);
						str1 += str2;
					}

					// ��������Ԫ��ͼ������д��״̬
					str2.Format(_T("W"));
					for (i = 0; i < m_iGroupCheckQueueNum[iGroupSN]; i++)
					{
						str2.Format(_T("%d "), m_pWriteDataStatus[iGroupSN][i]);
						str1 += str2;
					}

					// ��������Ԫ�ؼ��״̬
					str2.Format(_T("D"));
					for (i = 0; i < m_iGroupCheckQueueNum[iGroupSN]; i++)
					{
						str2.Format(_T("%d "), m_pDetectStatus[iGroupSN][i]);
						str1 += str2;
					}

					//memcpy(&chInfo[0], str1.GetBuffer(0), str1.GetLength());


					sInfo.Format(_T("[�����:%2d] [������:%2d] [������:%2d] [�鴦��ͼ�����:%8d] [��ͼ����:%2d] [����ͼ���:%8d] [����Ԫ������:%d]"),
						iCheckSN, iGroupSN, iCurrIndex, m_iGroupCircle[iGroupSN], iMinCircleSN,
						m_iPeriodImageSN[iGroupSN][m_iGroupCircle[iGroupSN] % 256],
						m_iPeriodIndex[iGroupSN][m_iGroupCircle[iGroupSN] % 256]);
					m_fileDetect[iGroupSN][7].Write(sInfo);

					// ����״̬�ص� ֪ͨϵͳ���ж���Ԫ���Ѿ����꣬��ȫ���μӼ��
					char chStatusInfo[MAX_CHAR_LENGTH];
					ZeroMemory(chStatusInfo, MAX_CHAR_LENGTH);
					s_Status sStatus;
					ZeroMemory(&sStatus, sizeof(sStatus));
					sStatus.iCheckSN = iCheckSN;
					sStatus.iStatusType = STATUS_THROW_IMAGE;
					sStatus.pThis = m_pCallThis;
					sStatus.pParam = NULL;
					sprintf(chStatusInfo, "Current Image No.%d, All Elements are busy!", iImageSN);
					sStatus.pErrorInfo = GetErrorInfo(chStatusInfo);
					m_StatusCallback(&sStatus);

					return sStatus;
				}
			}
			/** ����ǰͼ�����ӽ�������              [5/5/2017 ConanSteve]**/
			else
			{
				m_iGroupCircle[iGroupSN]++;

				m_spCheckInParam[iGroupSN][iCurrIndex].iCheckSN = iGroupSN;
				m_spCheckInParam[iGroupSN][iCurrIndex].iImageSN = iImageSN;
				m_spCheckInParam[iGroupSN][iCurrIndex].iReserve1 = sCheckInParam.iReserve1;
				m_spCheckInParam[iGroupSN][iCurrIndex].iReserve2 = sCheckInParam.iReserve2;
				m_spCheckInParam[iGroupSN][iCurrIndex].iReserve3 = sCheckInParam.iReserve3;
				m_spCheckInParam[iGroupSN][iCurrIndex].iReserve4 = sCheckInParam.iReserve4;

				m_pCircleSN[iGroupSN][iCurrIndex] = m_iGroupCircle[iGroupSN];

				m_iPeriodImageSN[iGroupSN][m_iGroupCircle[iGroupSN] % 256] = iImageSN;
				m_iPeriodIndex[iGroupSN][m_iGroupCircle[iGroupSN] % 256] = iCurrIndex;


				m_srwlImgQue[iGroupSN].Lock();
				BYTE* pAddr = (BYTE*)m_spCheckInParam[iGroupSN][iCurrIndex].pImageData + iOrderSN * m_iImageSize[iCheckSN];
				memcpy_s(pAddr, m_iImageSize[iCheckSN], sCheckInParam.pImageData, m_iImageSize[iCheckSN]);

				BYTE* pParam = (BYTE*)(m_spCheckInParam[iGroupSN][iCurrIndex].pParam);
				memcpy_s(pParam, m_iGroupCheckInSize[iGroupSN], sCheckInParam.pParam, m_iGroupCheckInSize[iGroupSN]);
				m_srwlImgQue[iGroupSN].Unlock();
				int iSize = 0;
				int iWidth = 0;
				int iHeight = 0;
				int iBit = 0;
				int iType = 0;
				int iCount = 0;

				/** д��־              [5/5/2017 ConanSteve]**/
				sInfo.Format("[�����:%2d] [������:%2d] [������:%2d] [�鴦��ͼ�����:%8d] [����ͼ���:%8d] [����Ԫ������:%d]",
					iCheckSN, iGroupSN, iCurrIndex, m_iGroupCircle[iGroupSN],
					m_iPeriodImageSN[iGroupSN][m_iGroupCircle[iGroupSN] % 256],
					m_iPeriodIndex[iGroupSN][m_iGroupCircle[iGroupSN] % 256]);
				m_fileDetect[iGroupSN][3].Write(sInfo);

				LARGE_INTEGER liTime;
				QueryPerformanceCounter(&liTime);
				m_arr_ldTickOfPushInCacheQueue[iGroupSN][iCurrIndex]=liTime.QuadPart;

				for (i = 0; i < m_iGroupElementNum[iGroupSN]; i++)
				{
					m_csCamComplete[m_iGroupFirstCam[iGroupSN] + i].Lock();
					m_bCamComplete[m_iGroupFirstCam[iGroupSN] + i][iImageSN % 256] = FALSE;
					m_csCamComplete[m_iGroupFirstCam[iGroupSN] + i].Unlock();
				}
			}
		}
		/** ����������              [5/5/2017 ConanSteve]**/
		// ���������ڣ��ж��Ƿ��Ѿ�д�룻��ûд�������ڴ�д�����ݣ�
		// ��д���ˣ����ѯ��һ��ͼ����Ƿ��Ѿ���д�룬��δд����ʱ����̫С�����ж�Ϊ���ǵ���ͼ�����ͬ
		else
		{

			sInfo.Format(_T("[�����:%2d] [������:%2d] [ͼ���:%8d] [ʱ���:%4.0f]"),
				iCheckSN, iGroupSN, iImageSN, fDiff);
			m_fileDetect[iGroupSN][1].Write(sInfo);

			sError.iStatusType = STATUS_ILG;
			sError.pThis = m_pCallThis;

			memset(&chInfo[0], 0, MAX_CHAR_LENGTH);
			sprintf_s(chInfo, "=Cam:%d=Image:%d=Same Image\r\n", iCheckSN, iImageSN);
			sError.pErrorInfo = GetErrorInfo(chInfo);

			return sError;
		}

		m_csCamComplete[iCheckSN].Lock();
		m_bCamComplete[iCheckSN][iImageSN % 256] = TRUE;
		m_csCamComplete[iCheckSN].Unlock();


		iComplete = 0;
		//**[7/19/2017 ConanSteve]**:   �鿴����λһ������ͼ���Ƿ�����
		for (i = 0; i < m_iGroupElementNum[iGroupSN]; i++)
		{
			if (m_bCamComplete[m_iGroupFirstCam[iGroupSN] + i][iImageSN % 256])
			{
				iComplete++;
			}
		}
		/** �������ͼ�񣬴������              [5/5/2017 ConanSteve]**/
		if (iComplete == m_iGroupElementNum[iGroupSN])
		{
			m_csWriteDataStatus[iGroupSN].Lock();
			m_pWriteDataStatus[iGroupSN][iCurrIndex] = e_WriteDataStatus_Complete;
			m_csWriteDataStatus[iGroupSN].Unlock();

			// �������
			SetEvent(m_hImageInspectStart[iGroupSN]);

			sInfo.Format(_T("[�����:%2d] [������:%2d] [ͼ���:%8d] [��������:%8d] [��������:%2d] [�޷ϱ��Ƿ�д��:%d]"),
				iCheckSN, iGroupSN, iImageSN, iCircleSN, iCurrIndex, *m_pbKickWrite[0][iImageSN % 256]);
			m_fileDetect[iGroupSN][4].Write(sInfo);
		}
	}
	return sError;
}

BOOL CDataClass::JudgeNewCircle(int iGroupSN, int iCameraSN, int iImageSN, int iCircleSN)
{

	BOOL bRet = FALSE;
	LONGLONG lInfo;
	LARGE_INTEGER liValue{ 0 };
	char chInfo[MAX_CHAR_LENGTH]{ 0 };

	m_iGroupCallCount[iGroupSN]++;
	QueryPerformanceCounter(&liValue);
	lInfo = liValue.QuadPart;

	m_csGroupDiff[iGroupSN].Lock();
	float dDiff = (lInfo - m_lPeriodTime[iGroupSN][(m_iGroupCallCount[iGroupSN] - 1) % 256]) * 1000 / m_dFreq;
	m_lPeriodTime[iGroupSN][m_iGroupCallCount[iGroupSN] % 256] = lInfo;
	m_csGroupDiff[iGroupSN].Unlock();

	if (e_CallType_RealTime == m_iCallType)
	{
		if (iImageSN != m_iGroupLastImageSN[iGroupSN])
		{
			bRet = TRUE;
		}
		else
		{
			if (dDiff > m_iPeriodInterval)
			{
				bRet = TRUE;
			}
			else
			{
				bRet = FALSE;
			}
		}
	}
	else if (e_CallType_Simulation == m_iCallType)
	{
		// ģ��ɼ� 
		if (iImageSN != m_iGroupLastImageSN[iGroupSN] && iCircleSN > m_iGroupCircle[iGroupSN])
		{
			bRet = TRUE;
		}
		else
		{
			bRet = FALSE;
		}
	}


	CString sInfo{ _T("") };
	sInfo.Format(_T("[�����:%2d] [������:%2d] [ͼ���:%8d] [��һͼ���:%8d] [�Ƿ�������:%2s] [TimeDiff:%4.0f]"),
		iCameraSN, iGroupSN, iImageSN, m_iGroupLastImageSN[iGroupSN], bRet ? _T("��") : _T("��"), dDiff);
	m_fileDetect[iGroupSN][0].Write(sInfo);
	// 	if (m_iGroupLastImageSN[iGroupSN] == iImageSN && bRet == TRUE)
	// 	{
	// 		int kk = 0;
	// 	}
	m_iGroupLastImageSN[iGroupSN] = iImageSN;

	return bRet;
}

DWORD CDataClass::ImageInspectionThread(LPVOID lpParam)
{
	CDataClass* pThis = (CDataClass*)(((s_ThreadParam*)lpParam)->pThis);
	int iThreadID = ((s_ThreadParam*)lpParam)->iThreadID;
	DWORD dwRet = pThis->ImageInspectionThreadFunc(iThreadID);
	return dwRet;
}

DWORD CDataClass::ImageInspectionThreadFunc(int iThreadID)
{
	int iGroupSN = 0;

	while (false == m_bKillThread)
	{
		iGroupSN = WaitForMultipleObjects(m_iCheckGroupNum, m_hImageInspectStart, FALSE, 10) - WAIT_OBJECT_0;
		if (iGroupSN >= 0 && iGroupSN < m_iCheckGroupNum && false == m_bKillThread)
		{
			CString sInfo{ _T("") };
			sInfo.Format(_T("[�����:%2d]  [�߳�ID:%2d]"), iGroupSN, iThreadID);
			m_fileCheck[iGroupSN][5].Write(sInfo);
			if (m_arr_bIsLoadModel[iGroupSN])
			{
				// 				UpdateRunInfo();
				// 				Sleep(1);
				continue;
			}
			InspectionContextEx2(iGroupSN, iThreadID);

		}
	}

	m_evtKillInspThread[iThreadID].SetEvent();

	TRACE(_T("Kill Insp %d Thread!\r\n"), iThreadID + 1);


	return 0;
}
DWORD CDataClass::InspectionContextEx2(int iGroupSN, int iThreadID)
{
	// �ҵ������С�Ľ��м��
	if (0 == iGroupSN)
		UpdateRunInfo();
	int i{ 0 }, j{ 0 };
	int iMaxNum{ 0 }, iMaxGroup{ 0 }, iTempMaxNum{ 0 };
	int iMinIndex{ 0 };
	int iMinCircleSN{ MAX_IMAGE_COUNT };
	BOOL bRet{ TRUE };

	while (!m_bKillThread)
	{
		iMaxNum = 0;
		iMaxGroup = 0;

		/*************************************<�����������д������ݳ���������>*************************************/
		{
			m_csGlobalCheck.Lock();
			for (i = 0; i < m_iCheckGroupNum; i++)
			{
				iTempMaxNum = 0;

				m_csWriteDataStatus[i].Lock();
				for (j = 0; j < m_iGroupCheckQueueNum[i]; j++)
				{
					if (m_pWriteDataStatus[i][j] == e_WriteDataStatus_Complete && m_pDetectStatus[i][j] == e_DetectStatus_NoChecking)
					{
						iTempMaxNum++;
					}
				}
				m_csWriteDataStatus[i].Unlock();

				if (iTempMaxNum > iMaxNum)
				{
					iMaxNum = iTempMaxNum;
					iMaxGroup = i;
				}
			}
			m_csGlobalCheck.Unlock();
		}
		{
			if (iMaxNum == 0)
			{
				break;
			}
			else
			{
				iGroupSN = iMaxGroup;
			}
		}

		/*************************************<���Ҷ�Ӧ���������������С������>*************************************/
		{
			iMinCircleSN = MAX_IMAGE_COUNT;

			m_csWriteDataStatus[iGroupSN].Lock();
			for (i = 0; i < m_iGroupCheckQueueNum[iGroupSN]; i++)
			{
				if (m_pWriteDataStatus[iGroupSN][i] == e_WriteDataStatus_Complete && m_pDetectStatus[iGroupSN][i] == e_DetectStatus_NoChecking)
				{
					if (m_pCircleSN[iGroupSN][i] < iMinCircleSN)
					{
						iMinIndex = i;
						iMinCircleSN = m_pCircleSN[iGroupSN][i];
					}
				}
			}

			if (MAX_IMAGE_COUNT == iMinCircleSN)
			{
				m_csWriteDataStatus[iGroupSN].Unlock();
				break;
			}
			else
			{
				m_pDetectStatus[iGroupSN][iMinIndex] = e_DetectStatus_Checking;
			}

			m_csWriteDataStatus[iGroupSN].Unlock();
		}
		//������Сͼ���
		CheckEx2(iMinIndex, iThreadID, m_spCheckInParam[iGroupSN][iMinIndex]);
	}


	return 0;
}

// BOOL CDataClass::ThreadWriteCheckResult(int iGroupSN, int iIndex)
// {
// 	int i = 0;
// 
// 	// ��ȡ�����һ�������
// 	int iFirstCameraSN = m_iGroupFirstCam[iGroupSN];
// 
// 	// ��ȡ�����޷϶��к�
// 	int iKickQueueSN = m_iCamKick[iFirstCameraSN];
// 
// 	// ��ȡ���ں�
// 	int iImageSN = m_spCheckInParam[iGroupSN][iIndex].iImageSN;
// 
// 	// ��ȡ���޷϶��е�ƫ����(��ͼ������Ϊ��λ)��ȷ���޷ϱ������λ��
// 	int	iKickY = m_spCheckInParam[iGroupSN][iIndex].iImageSN + m_iCamKickOffsetY[iFirstCameraSN];
// 
// 	// ���޷ϱ��еĺ���λ��
// 	int iKickX = m_iCamKickOffsetX[iFirstCameraSN];
// 	int iKickX_R = m_iCamKickOffsetX_Written[iFirstCameraSN];
// 
// 	BYTE* pDestAddr = NULL, *pDestAddr2 = NULL;
// 	BYTE* pOriAddr = NULL;
// 
// 	int iOffsetX1 = 0, iOffsetX1_R = 0, iOffsetY1 = 0;
// 	int iOutResult = 0;
// 	iOutResult = m_iCheckOutNum[iFirstCameraSN];
// 
// 	CString str1, str2;
// 
// 	//��������ֲ�ͬ����д���޷ϱ���
// 	for (i = 0; i < m_iCheckOutNum[iFirstCameraSN]; i++)
// 	{
// 		iOffsetY1 = (iKickY + iOutResult - i - 1) % 256;
// 		iOffsetX1 = iKickX + (iOutResult - i - 1)*m_iGroupCheckOutPerResultSize[iGroupSN];
// 		iOffsetX1_R = iKickX_R + iOutResult - i - 1;
// 
// 		m_csKickResult[iKickQueueSN][iOffsetY1].Lock();
// 		// 		pDestAddr = m_pKickResult[iKickQueueSN]+iOffsetY1*m_iKickQueueSize[iKickQueueSN]+iOffsetX1;
// 		// 		pDestAddr2 = m_pbKickWrite[iKickQueueSN]+iOffsetY1*m_iKickResultNum[iKickQueueSN]+iOffsetX1_R;
// 		pDestAddr = m_pKickResult[iKickQueueSN][iOffsetY1] + iOffsetX1;
// 		pDestAddr2 = m_pbKickWrite[iKickQueueSN][iOffsetY1] + iOffsetX1_R;
// 
// 		//**[7/28/2017 ConanSteve]**:  ������Ľṹ���ַ
// 		pOriAddr = (BYTE*)m_spCheckOutParam[iGroupSN][iIndex].pParam + i*m_iGroupCheckOutPerResultSize[iGroupSN];
// 
// 		memset(pDestAddr, 0, m_iGroupCheckOutPerResultSize[iGroupSN]);
// 		memcpy(pDestAddr, pOriAddr, m_iGroupCheckOutPerResultSize[iGroupSN]);
// 
// 		*pDestAddr2 = 1;
// 
// 		str1.Format("%d", *((int*)pOriAddr));
// 		str2 += str1;
// 
// 		m_csKickResult[iKickQueueSN][iOffsetY1].Unlock();
// 
// 	}
// 
// 
// 	if (TRUE == m_bWriteLog)
// 	{
// 		m_iFileCheck[iGroupSN][3]++;
// 		if (m_iFileCheck[iGroupSN][3] > m_iLogNum)
// 		{
// 			m_iFileCheck[iGroupSN][3] = 0;
// 			m_fileCheck[iGroupSN][3].SeekToBegin();
// 		}
// 
// 		CString strInfo;
// 		strInfo.Format(_T("[������:%2d] [�����������:%2d] [ͼ���:%8d] [����д״̬:%s] [������״̬:%s] [�޷϶��к�:%4d] [�޷�ƫ����Y:%3d] [�޷�ƫ��X:%3d] [Ŀ�ĵ�ַ:0x%08X] [Info:%10s] [CurrentTime:%s]\n"),
// 			iGroupSN, iIndex, iImageSN,
// 			e_WriteDataStatus_Leisure == m_pWriteDataStatus[iGroupSN][iIndex] ? _T("����") : (e_WriteDataStatus_Busy == m_pWriteDataStatus[iGroupSN][iIndex] ? _T("дæ") : _T("д��")),
// 			e_DetectStatus_NoChecking ==m_pDetectStatus[iGroupSN][iIndex]?_T("δ���"):_T("�����"),
// 			iKickQueueSN, iKickY, iKickX,
// 			pDestAddr, str2, GetCurrTime());
// 		m_fileCheck[iGroupSN][3].WriteString(strInfo);
// 	}
// 
// 
// 	return TRUE;
// }

BOOL CDataClass::ThreadWriteCheckResultEx2(int iGroupSN, s_CheckInParam* spCheckInParam, s_CheckOutParam* spCheckOutParam)
{
	int i = 0;
	if (NULL == spCheckInParam || NULL == spCheckOutParam)
		return false;

	// ��ȡ�����һ�������
	int iFirstCameraSN = m_iGroupFirstCam[iGroupSN];

	// ��ȡ�����޷϶��к�
	int iKickQueueSN = m_iCamKick[iFirstCameraSN];

	// ��ȡ���ں�
	int iImageSN = spCheckInParam->iImageSN;

	// ��ȡ���޷϶��е�ƫ����(��ͼ������Ϊ��λ)��ȷ���޷ϱ������λ��
	int	iKickY = spCheckInParam->iImageSN + m_iCamKickOffsetY[iFirstCameraSN];

	// ���޷ϱ��еĺ���λ��
	int iKickX = m_iCamKickOffsetX[iFirstCameraSN];
	int iKickX_R = m_iCamKickOffsetX_Written[iFirstCameraSN];

	BYTE* pDestAddr = NULL, *pDestAddr2 = NULL;
	BYTE* pOriAddr = NULL;

	int iOffsetX1 = 0, iOffsetX1_R = 0, iOffsetY1 = 0;
	int iOutResult = 0;//**[12/27/2017 ConanSteve]**:  ���޷����һ������ļ��������
	iOutResult = m_iCheckOutNum[iFirstCameraSN];

	CString sAllAlgStatus{ _T("") }, sSingleAlgStatus{ _T("") };

	//��������ֲ�ͬ����д���޷ϱ���
	for (i = 0; i < m_iCheckOutNum[iFirstCameraSN]; i++)
	{
		iOffsetY1 = (iKickY + iOutResult - i - 1) % 256;
		iOffsetX1 = iKickX + (iOutResult - i - 1)*m_iGroupCheckOutPerResultSize[iGroupSN];
		iOffsetX1_R = iKickX_R + iOutResult - i - 1;

		m_csKickResult[iKickQueueSN][iOffsetY1].Lock();
		pDestAddr = m_pKickResult[iKickQueueSN][iOffsetY1] + iOffsetX1;
		pDestAddr2 = m_pbKickWrite[iKickQueueSN][iOffsetY1] + iOffsetX1_R;

		pOriAddr = (BYTE*)spCheckOutParam->pParam + i * m_iGroupCheckOutPerResultSize[iGroupSN];


		memset(pDestAddr, 0, m_iGroupCheckOutPerResultSize[iGroupSN]);
		memcpy(pDestAddr, pOriAddr, m_iGroupCheckOutPerResultSize[iGroupSN]);

		*pDestAddr2 = 1;

		sSingleAlgStatus.Format(_T("%d"), *((int*)pOriAddr));
		sAllAlgStatus += sSingleAlgStatus;

		m_csKickResult[iKickQueueSN][iOffsetY1].Unlock();

	}


	CString strInfo{ _T("") };
	strInfo.Format(_T("[������:%2d] [ͼ���:%8d] [�޷϶��к�:%4d] [�޷�ƫ����Y:%3d] [�޷�ƫ��X:%3d] [Ŀ�ĵ�ַ:0x%08X] [�����:%10s]"),
		iGroupSN, iImageSN,
		iKickQueueSN, iKickY, iKickX,
		pDestAddr, sAllAlgStatus);
	m_fileCheck[iGroupSN][3].Write(strInfo);

	if (!m_bEnableOvertimeCallback)
	{
		m_csBoolKickWrite[iKickQueueSN][iImageSN % 256].Lock();
		IsCheckCompleted(iGroupSN, iKickQueueSN, iImageSN);
		m_csBoolKickWrite[iKickQueueSN][iImageSN % 256].Unlock();
	}

	return TRUE;
}

DWORD CDataClass::KickThread(LPVOID lpParam)
{
	CDataClass* pThis = (CDataClass*)lpParam;

	pThis->KickThreadFuncEx2();


	return 0;
}


BOOL CDataClass::CreateMultiLevelPath(const CString &strPath)
{
	//����һ�����Ŀ¼��������ھͲ�����
	//(strPath  �Ϸ�: "D:\\abc\\me" �� "me\\you" �� ""; 
	//			���Ϸ�: "D:\\abc\\me\\" �� "me\\you\\" �� "\\")
	CString strTempPath{ _T("") };// = strPath;
	strTempPath.Format(_T("%s"), strPath);
	CList<CString, CString&> FolderStrList;
	int nStrLen = 0;

	for (;;)
	{
		nStrLen = strTempPath.GetLength();
		if (nStrLen <= 0)
		{
			break;//��ͷ���ļ���
		}

		BOOL bPathExist = FALSE;//��·���Ƿ��Ѿ�����

		if (strTempPath.Right(1) == _T(":"))
		{
			bPathExist = PathFileExists(strTempPath + _T("\\"));
		}
		else
		{
			bPathExist = PathFileExists(strTempPath);
		}

		if (bPathExist)//���·������
		{
			break;//���Ѵ��ڵ�·���½��ļ���
		}

		//���·��������		
		//ÿ�ν�����һ���ļ��������

		int nPos = 0;
		nPos = strTempPath.ReverseFind(_T('\\'));//�Ӻ���ǰȷ���ļ�����
		if (nPos == -1)//�ѵ����һ���ļ�����
		{
			nPos = 0;//����һ���ļ��������
		}

		//���ļ��������
		CString strTemp = strTempPath.Right(nStrLen - nPos);
		FolderStrList.AddHead(strTemp);
		strTempPath.Delete(nPos, nStrLen - nPos);
	}

	//�����ļ���������˳����
	while (FolderStrList.IsEmpty() == FALSE)
	{
		strTempPath += FolderStrList.RemoveHead();
		BOOL bRet = FALSE;
		bRet = CreateDirectory(strTempPath, NULL);
		if (bRet == FALSE)
		{
			return FALSE;
		}
	}

	return TRUE;
}

// CString CDataClass::GetCurrTime()
// {
// 	LARGE_INTEGER iLarge;
// 	LONGLONG qt;
// 	int nHour = 0, nMinute = 0, nSecond = 0, nMiliSecond = 0;
// 	QueryPerformanceCounter(&iLarge);
// 	qt = iLarge.QuadPart;
// 
// 	nMiliSecond = qt * 1000 / m_dFreq;
// 	nSecond = nMiliSecond / 1000;
// 	nMinute = nSecond / 60;
// 	nHour = nMinute / 60;
// 
// 	CString strInfo;
// 	strInfo.Format(_T("%02d:%02d:%02d.%03d"), nHour % 24, nMinute % 60, nSecond % 60, nMiliSecond % 1000);
// 	return strInfo;
// }

int CDataClass::SaveBmp(const TCHAR* pPathName, int iWidth, int iHeight, int iByte, TBYTE *pDest)
{
	BITMAPFILEHEADER bmFH;
	int nPaletteEntries = 256;
	DWORD dwSize = iWidth * iHeight*iByte;

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

BOOL CDataClass::CreateLogFile()
{
	int i = 0, j = 0;
	BOOL bRet = TRUE;
	CString strAppPath, strLog;
	CString strDetect, strCheck, strKick, strInfo;

	strAppPath = CExFn::GetCurrentAppPath();
	// 	strLog.Format("%sLogfiles\\MultipleObj",strAppPath);
	strLog.Format(_T("%s:\\Logfiles\\MultipleObj"), m_strVitrualDisk);
	DeleteDirectory(strLog.GetBuffer());

	CExFn::CreateMultiDirectory(strLog.GetBuffer());

	strDetect.Format(_T("%s\\Detect"), strLog);
	CExFn::CreateMultiDirectory(strDetect.GetBuffer());
	if (!PathFileExists(strDetect))
	{
		if (!CreateMultiLevelPath(strDetect))
		{
			CString sErrorInfo{ _T("") };
			sErrorInfo.Format(_T("Create logfiles path1 failed!\n%s"), strDetect);
			AfxMessageBox(sErrorInfo);
			return FALSE;
		}
	}

	strCheck.Format(_T("%s\\Check"), strLog);
	CExFn::CreateMultiDirectory(strCheck.GetBuffer());
	if (!PathFileExists(strCheck))
	{
		if (!CreateMultiLevelPath(strCheck))
		{
			CString sErrorInfo{ _T("") };
			sErrorInfo.Format(_T("Create logfiles path2 failed!\n%s"), strCheck);
			AfxMessageBox(sErrorInfo);
			return FALSE;
		}
	}

	strKick.Format(_T("%s\\Kick"), strLog);
	CExFn::CreateMultiDirectory(strKick.GetBuffer());
	if (!PathFileExists(strKick))
	{
		if (!CreateMultiLevelPath(strKick))
		{
			CString sErrorInfo{ _T("") };
			sErrorInfo.Format(_T("Create logfiles path3 failed!\n%s"), strKick);
			AfxMessageBox(sErrorInfo);
			return FALSE;
		}
	}

	m_fileRunInfoStatus.Open(strLog + _T("\\RunInfoStatus.txt"));
	for (i = 0; i < m_iCameraNum; i++)
	{
		for (j = 0; j < c_iMaxFileNum; j++)
		{
			strInfo.Format(_T("%s\\Camera%02d_%02d_"), strDetect, i + 1, j);
			bool bOpenFile = true;
			switch (j)
			{
			case 0:
				strInfo += _T("ͼƬ��Ϣ");
				break;
			default:
				bOpenFile = false;
				break;
			}
			strInfo += _T(".txt");
			if (bOpenFile)
			{
				m_fileCam[i][j].Open(strInfo, &m_bWriteLog);
				m_fileCam[i][j].SetMaxLine(m_iMaxLogLineNum);
			}
		}
	}

	for (i = 0; i < m_iCheckGroupNum; i++)
	{
		for (j = 0; j <c_iMaxFileNum; j++)
		{
			strInfo.Format(_T("%s\\Detect%02d_%02d_"), strDetect, i + 1, j);
			bool bOpenFile = true;
			switch (j)
			{
			case 0:
				strInfo += _T("�Ƿ���������");
				break;
			case 1:
				strInfo += _T("Detect������ͼ��Ϣ");
				break;
			case 2:
				strInfo += _T("���������Ϣ");
				break;
			case 3:
				strInfo += _T("��ͼ��Ϣ");
				break;
			case 4:
				strInfo += _T("�����̼߳����Ϣ");
				break;
			case 5:
				strInfo += _T("���ǰ�޷ϱ���Ϣ");
				break;
			case 6:
				strInfo += _T("δ����ͼֱ���ͼ���Ϣ");
				break;
			case 7:
				strInfo += _T("��������ͼ��Ϣ");
				break;
			case 8:
				strInfo += _T("Checkǰ��Ϣ");
				break;
			case 9:
				strInfo += _T("�㷨��ͼʱ��");
				break;
			case 10:
				strInfo += _T("С��ͼ����Խ�綪ͼ");
				break;
			default:
				bOpenFile = false;
				break;
			}
			strInfo += _T(".txt");
			if (bOpenFile)
			{
				m_fileDetect[i][j].Open(strInfo, &m_bWriteLog);
				m_fileDetect[i][j].SetMaxLine(m_iMaxLogLineNum);
			}
		}

		for (j = 0; j < c_iMaxFileNum; j++)
		{
			bool bOpenFile = true;
			strInfo.Format(_T("%s\\CheckGroup%02d_%02d_"), strCheck, i + 1, j);
			switch (j)
			{
			case 0:
				strInfo += _T("���ǰ������Ϣ");
				break;
			case 1:
				strInfo += _T("��ǰ�������Ϣ");
				break;
			case 2:
				strInfo += _T("�������Ϣ");
				break;
			case 3:
				strInfo += _T("д�޷ϱ���Ϣ");
				break;
			case 4:
				strInfo += _T("����껺�������Ϣ");
				break;
			case 5:
				strInfo += _T("����߳���Ϣ");
				break;
			case 6:
				strInfo += _T("������æ��Ϣ");
				break;
			case 7:
				strInfo += _T("�㷨�쳣��Ϣ");
				break;
			default:
				bOpenFile = false;
				break;
			}
			strInfo += _T(".txt");
			if (bOpenFile)
			{
				m_fileCheck[i][j].Open(strInfo, &m_bWriteLog);
				m_fileCheck[i][j].SetMaxLine(m_iMaxLogLineNum);
			}
		}

	}

	for (i = 0; i < m_iKickQueueNum; i++)
	{
		for (j = 0; j < c_iMaxFileNum; j++)
		{
			bool bOpenFile = true;
			strInfo.Format("%s\\Kick%02d_%d_", strKick, i + 1, j);
			switch (j)
			{
			case 0:
				strInfo += _T("�޷ϱ���Ϣ");
				break;
			case 1:
				strInfo += _T("���������־��¼");
				break;
			case 2:
				strInfo += _T("��Ʒ��Ϣ");
				break;
			default:
				bOpenFile = false;
				break;
			}
			strInfo += _T(".txt");
			if (bOpenFile)
			{
				m_fileKick[i][j].Open(strInfo, &m_bWriteLog);
				m_fileKick[i][j].SetMaxLine(m_iMaxLogLineNum);
			}
		}
	}


	return bRet;
}

BOOL CDataClass::ThreadWriteResult(int iImageSN, int iGroupSN, int iKickSN, bool bInit)
{

	// ��ȡ�����һ�������
	int iFirstCameraSN = m_iGroupFirstCam[iGroupSN];

	// ��ȡ���޷϶��е�ƫ����(��ͼ������Ϊ��λ)��ȷ���޷ϱ������λ��
	int	iKickY = iImageSN + m_iCamKickOffsetY[iFirstCameraSN];

	// ���޷ϱ��еĺ���λ��
	int iKickX = m_iCamKickOffsetX[iFirstCameraSN];
	int iKickX_IsWritten = m_iCamKickOffsetX_Written[iFirstCameraSN];

	BYTE* pDestAddr = NULL;
	BYTE *pDestAddr2 = NULL;

	int iOffsetX1 = 0, iOffsetX1_R = 0, iOffsetY1 = 0;
	int iOutResult = 0;//**[9/22/2017 ConanSteve]**:  һ����1
	iOutResult = m_iCheckOutNum[iFirstCameraSN];

	//��������ֲ�ͬ����д���޷ϱ���
	for (int i = 0; i < m_iCheckOutNum[iFirstCameraSN]; i++)
	{
		iOffsetY1 = (iKickY + iOutResult - i - 1) % 256;
		iOffsetX1 = iKickX + (iOutResult - i - 1)*m_iGroupCheckOutPerResultSize[iGroupSN];
		iOffsetX1_R = iKickX_IsWritten + iOutResult - i - 1;

		m_csKickResult[iKickSN][iOffsetY1].Lock();
		pDestAddr = m_pKickResult[iKickSN][iOffsetY1] + iOffsetX1;
		pDestAddr2 = m_pbKickWrite[iKickSN][iOffsetY1] + iOffsetX1_R;

		int *pDefaultStatus = (int*)pDestAddr;
		*pDestAddr = m_iDefaultCheckRslt;
		BYTE *pDefaulWrite = pDestAddr2;//**[9/22/2017 ConanSteve]**:  �Ƿ��Ѿ�д����ֵ
		if (bInit)
		{
			*pDefaultStatus = m_iLostImageRslt;//**[9/22/2017 ConanSteve]**:  ��ͼд-4
			*pDefaulWrite = 1;
		}
		else
		{
			*pDefaultStatus = m_iDefaultCheckRslt;//**[9/22/2017 ConanSteve]**:  Ĭ��д-3
			*pDefaulWrite = 0;
		}

		m_csKickResult[iKickSN][iOffsetY1].Unlock();

		CString sInfo{ _T("") };
		sInfo.Format(_T("[������:%2d] [ͼ���:%8d] [�޷Ϻ�:%2d] [�Ƿ��ʼ��:%s] [Ŀ�ĵ�ַ:0x%08X] [�Ƿ�д��:%2s] [Yƫ��:%8d] [Xƫ��:%8d] [X_Rƫ��:%8d]"),
			iGroupSN, iImageSN, iKickSN, TRUE == bInit ? _T("��") : _T("��"), pDestAddr2, TRUE == *pDefaulWrite ? _T("��") : _T("��"),
			iOffsetY1, iOffsetX1, iOffsetX1_R);
		m_fileDetect[iGroupSN][5].Write(sInfo);
	}
	if (!m_bEnableOvertimeCallback)
	{
		m_csBoolKickWrite[iKickSN][iImageSN % 256].Lock();
		IsCheckCompleted(iGroupSN, iKickSN, iImageSN);
		m_csBoolKickWrite[iKickSN][iImageSN % 256].Unlock();
	}
	return TRUE;
}

int CDataClass::ShowCurrStatus()
{
	// 	int i = 0, j = 0;
	// 
	// 	if (TRUE == m_bShowStatus)
	// 	{
	// 		char chObj[MAX_CHAR_LENGTH];
	// 		char chCam[MAX_CHAR_LENGTH];
	// 		char chList[MAX_CHAR_LENGTH];
	// 		char chImage[MAX_CHAR_LENGTH];
	// 		char chTemp[MAX_CHAR_LENGTH];
	// 
	// 		for (i = 0; i < m_iCheckGroupNum; i++)
	// 		{
	// 			int iTemp = 0;
	// 
	// 			m_csWriteDataStatus[i].Lock();
	// 			for (j = 0; j < m_iGroupCheckQueueNum[i]; j++)
	// 			{
	// 				if (e_WriteDataStatus_Leisure == m_pWriteDataStatus[i][j])
	// 				{
	// 					iTemp++;
	// 				}
	// 			}
	// 
	// 			m_csWriteDataStatus[i].Unlock();
	// 
	// 			m_iGroupLeftList[i] = iTemp;
	// 		}
	// 
	// 		ZeroMemory(chList, MAX_CHAR_LENGTH);
	// 		sprintf(chList, "������״̬:\r\n");
	// 		for (i = 0; i < m_iCheckGroupNum; i++)
	// 		{
	// 			ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 			sprintf(chTemp, " %d ", i + 1);
	// 			strcat(chList, chTemp);
	// 
	// 			for (j = 0; j < m_iGroupCheckQueueNum[i]; j++)
	// 			{
	// 				ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 				sprintf(chTemp, "%d ", m_pWriteDataStatus[i][j]);
	// 				strcat(chList, chTemp);
	// 			}
	// 
	// 			ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 			sprintf(chTemp, "  %d\r\n", m_iGroupLeftList[i]);
	// 			strcat(chList, chTemp);
	// 		}
	// 
	// 		ZeroMemory(chImage, MAX_CHAR_LENGTH);
	// 		sprintf(chImage, "����Ԫ�ؼ��ͼ���:\r\n");
	// 		for (i = 0; i < m_iCheckGroupNum; i++)
	// 		{
	// 			ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 			sprintf(chTemp, " %d ", i + 1);
	// 			strcat(chImage, chTemp);
	// 
	// 			for (j = 0; j < m_iGroupCheckQueueNum[i]; j++)
	// 			{
	// 				ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 				sprintf(chTemp, "%d ", m_pCircleSN[i][j]);
	// 				strcat(chImage, chTemp);
	// 			}
	// 
	// 			ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 			sprintf(chTemp, "\r\n");
	// 			strcat(chImage, chTemp);
	// 		}
	// 
	// 
	// 		for (i = 0; i < m_iCheckGroupNum; i++)
	// 		{
	// 			int iTemp = 0;
	// 			for (j = 0; j < m_pnObjectNumber[i]; j++)
	// 			{
	// 				if (0 == m_nObjIndex[i][j])
	// 				{
	// 					iTemp++;
	// 				}
	// 			}
	// 			m_iGroupLeftObj[i] = iTemp;
	// 		}
	// 
	// 		ZeroMemory(chObj, MAX_CHAR_LENGTH);
	// 		sprintf(chObj, "��������״̬��\r\n");
	// 		for (i = 0; i < m_iCheckGroupNum; i++)
	// 		{
	// 			ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 			sprintf(chTemp, " %d ", i + 1);
	// 			strcat(chObj, chTemp);
	// 
	// 			for (j = 0; j < m_pnObjectNumber[i]; j++)
	// 			{
	// 				ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 				sprintf(chTemp, "%d ", m_nObjIndex[i][j]);
	// 				strcat(chObj, chTemp);
	// 			}
	// 
	// 			ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 			sprintf(chTemp, "  %d \r\n", m_iGroupLeftObj[i]);
	// 			strcat(chObj, chTemp);
	// 		}
	// 
	// 		ZeroMemory(chCam, MAX_CHAR_LENGTH);
	// 		sprintf(chCam, "ÿ������鶪ͼ����:\r\n");
	// 		for (i = 0; i < m_iCameraNum; i++)
	// 		{
	// 			ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 			sprintf(chTemp, "%d ", m_iGroupAbnormalNum[i]);
	// 			strcat(chCam, chTemp);
	// 		}
	// 
	// 		ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 		sprintf(chTemp, "\r\n�����ͼ����:\r\n");
	// 		strcat(chCam, chTemp);
	// 
	// 		for (i = 0; i < m_iCameraNum; i++)
	// 		{
	// 			ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 			sprintf(chTemp, "%d ", m_iCamCircle[i]);
	// 			strcat(chCam, chTemp);
	// 		}
	// 
	// 		ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 		sprintf(chTemp, "\r\n������:\r\n");
	// 		strcat(chCam, chTemp);
	// 
	// 		for (i = 0; i < m_iCheckGroupNum; i++)
	// 		{
	// 			ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 			sprintf(chTemp, " %d", m_iGroupAlgCheck[i]);
	// 			strcat(chCam, chTemp);
	// 		}
	// 		m_pObjStatusDlg->ShowInfo(chObj, chList, chImage, chCam);
	// 	}

	return 0;
}


int CDataClass::KillAllThread()
{
	DWORD dwResult = -1;

	//Sleep(1000);

	for (int i = 0; i < m_iCheckThreadNum; i++)
	{
		dwResult = WaitForSingleObject(m_evtKillInspThread[i], 1000);
		if (dwResult == WAIT_OBJECT_0)
		{
			TRACE(_T("...����߳�%d�˳��ɹ�!\n"), i + 1);
		}
	}

	dwResult = WaitForSingleObject(m_evtKillKickThread, INFINITE);
	if (dwResult == WAIT_OBJECT_0)
	{
		TRACE("...�޷��߳��˳��ɹ�!\n");
	}
	return 0;
}

int CDataClass::ReleaseThreadData()
{



	/************************************************************************/
	if (e_CallType_Demo != m_iCallType)
	{
		m_bKillThread = true;
		KillAllThread();
	}
	CString strDllPath;
	int i = 0, j = 0;
	for (i = 0; i < m_iCheckGroupNum; ++i)
	{
		strDllPath = m_strArrDllPath.GetAt(i);
		if (strDllPath == _T("NULL"))
		{
			continue;
		}

		HMODULE hDll = LoadLibrary(strDllPath.GetBuffer(0));
		if (hDll)
		{
			pFunc_Destroy destroyobj;
			destroyobj = (pFunc_Destroy)(GetProcAddress(hDll, _T("destroy_instance")));
			if (destroyobj&&m_bEnableInitObjectAndCheck)
			{
				CInspBase* pInsp1 = NULL;
				for (int j = 0; j < m_pnObjectNumber[i]; ++j)
				{
					pInsp1 = (CInspBase*)m_iIdleJob[i] + j;
					break;

				}
				//destroyobj(pInsp1);
				FreeLibrary(hDll);
			}
			else
			{

			}
		}
		else
		{

		}

		// �ͷ��ڴ�
		// 		if (m_nObjIndex[i] != NULL)
		// 		{
		// 			delete[] m_nObjIndex[i];
		// 			m_nObjIndex[i] = NULL;
		// 		}
	}//endfor

	if (m_pnObjectNumber != NULL)
	{
		delete[] m_pnObjectNumber;
		m_pnObjectNumber = NULL;
	}



	if (m_plObjBusyTimes != NULL)
	{
		delete[] m_plObjBusyTimes;
		m_plObjBusyTimes = NULL;
	}

	for (i = 0; i < m_iCheckGroupNum; i++)
	{
		// �ͷ������ڴ�
		if (m_pImageData[i] != NULL)
		{
			delete[]m_pImageData[i];
			m_pImageData[i] = NULL;
		}

		// �ͷż����ж���״̬ʹ�õ��ڴ�
		if (m_pWriteDataStatus[i] != NULL)
		{
			delete[]m_pWriteDataStatus[i];
			m_pWriteDataStatus[i] = NULL;
		}

		if (m_pDetectStatus[i] != NULL)
		{
			delete[]m_pDetectStatus[i];
			m_pDetectStatus[i] = NULL;
		}

		// �ͷż����ж�����ʹ�õ��ڴ�
		if (m_pCircleSN[i] != NULL)
		{
			delete[]m_pCircleSN[i];
			m_pCircleSN[i] = NULL;
		}

		// �Ƿ����߳��ڴ�
		if (m_spCheckInParam[i] != NULL)
		{
			delete[]m_spCheckInParam[i];
			m_spCheckInParam[i] = NULL;
		}

		if (m_spCheckOutParam[i] != NULL)
		{
			delete[]m_spCheckOutParam[i];
			m_spCheckOutParam[i] = NULL;
		}

		if (m_spCheckStatusParam[i] != NULL)
		{
			delete[]m_spCheckStatusParam[i];
			m_spCheckStatusParam[i] = NULL;
		}

		if (m_pCheckInParam[i] != NULL)
		{
			delete[]m_pCheckInParam[i];
			m_pCheckInParam[i] = NULL;
		}

		if (m_pCheckOutParam[i] != NULL)
		{
			delete[]m_pCheckOutParam[i];
			m_pCheckOutParam[i] = NULL;
		}

		if (m_pCheckStatusParam[i] != NULL)
		{
			delete[]m_pCheckStatusParam[i];
			m_pCheckStatusParam[i] = NULL;
		}
	}

	for (i = 0; i < m_iKickQueueNum; i++)
	{
		for (j = 0; j < 256; j++)
		{
			m_csKickResult[i][j].Lock();
			if (m_pKickResult[i][j] != NULL)
			{
				delete[]m_pKickResult[i][j];
				m_pKickResult[i][j] = NULL;
			}

			if (m_pbKickWrite[i][j] != NULL)
			{
				delete[]m_pbKickWrite[i][j];
				m_pbKickWrite[i][j] = NULL;
			}
			m_csKickResult[i][j].Unlock();
		}
	}
	if (e_CallType_Demo != m_iCallType)
	{
		if (e_ObjDetectMode_2 == m_iObjDetectMode)
		{
			for (int i = 0; i < m_iCheckGroupNum; ++i)
			{
				delete[] m_ppCheckInParamEx[i];
				delete[] m_ppCheckOutParamEx[i];
				delete[] m_ppCheckStatusParamEx[i];
				delete[] m_sppCheckInParamEx[i];
				delete[] m_sppCheckOutParamEx[i];
				delete[] m_sppCheckStatusParamEx[i];
			}


			delete[] m_ppCheckInParamEx;
			delete[] m_ppCheckOutParamEx;
			delete[] m_ppCheckStatusParamEx;
			delete[] m_sppCheckInParamEx;
			delete[] m_sppCheckOutParamEx;
			delete[] m_sppCheckStatusParamEx;
		}
		for (int i = 0; i < m_iCheckGroupNum; ++i)
		{
			if (NULL != m_ppCacheUseCnt[i])
				delete[] m_ppCacheUseCnt[i];

		}
		if (NULL != m_ppCacheUseCnt)
			delete[] m_ppCacheUseCnt;
	}

	if (NULL != m_pDlgRunInfo)
	{
		delete m_pDlgRunInfo;
	}
	return 0;
}

int CDataClass::FnSavebmp(const TCHAR *lpszPathName, int W, int H, int nByte, TBYTE *lpDest)
{
	DWORD dwSize = W * H*nByte;
	BITMAPFILEHEADER bmFH{ 0 };
	int nPaletteEntries = 256;

	if (nByte < 3)
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
	char bmpBuf[2048];
	FileBMP = (BITMAPINFO *)bmpBuf;
	FileBMP->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	FileBMP->bmiHeader.biWidth = W;
	FileBMP->bmiHeader.biHeight = H;
	FileBMP->bmiHeader.biPlanes = 1;
	FileBMP->bmiHeader.biBitCount = nByte * 8;
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
	DWORD dwOffset = 0;

	if (ImageFile.Open(lpszPathName, CFile::modeCreate | CFile::modeReadWrite))
	{
		ImageFile.Write(&bmFH, sizeof(BITMAPFILEHEADER));
		ImageFile.Write(&FileBMP->bmiHeader, sizeof(BITMAPINFOHEADER));
		ImageFile.Write(&FileBMP->bmiColors[0], sizeof(RGBQUAD) * nPaletteEntries);
		ImageFile.Write(lpDest, dwSize);
		ImageFile.Close();
		return 1;
	}
	else
		return -1;

}

DWORD CDataClass::FnThreadSaveImg(PVOID pParam)
{
	CDataClass* pThis = (CDataClass*)(pParam);
	return pThis->FnThreadSaveImgFunc();
}

DWORD CDataClass::FnThreadSaveImgFunc()
{
	while (1)
	{
		DWORD dwRet = WaitForSingleObjectEx(m_hEventSaveImgs, 10, TRUE);
		switch (dwRet)
		{
		case WAIT_OBJECT_0:
			if (m_bKillThread)
				return 0;
			break;
		case WAIT_IO_COMPLETION:
			return 0;
		case WAIT_TIMEOUT:
			if (m_bKillThread)
			{
				return 0;
			}
		default:
			continue;
		}
		/*************************************<>*************************************/
		m_srwlImgsWaitForSave.Lock();
		while (!m_queImgsWaitForSave.empty())
		{
			s_ImgInfoWaitForSave sImgInfoWaitForSave = m_queImgsWaitForSave.front();
			m_queImgsWaitForSave.pop();
			int iGroupSN = sImgInfoWaitForSave.sCheckInParam.iCheckSN;
			int iMinIndex = sImgInfoWaitForSave.iMinIndex;
			if (0 == sImgInfoWaitForSave.sCheckInParam.iReserve4)
			{
				//**[8/8/2017 ConanSteve]**:  ��Ҫ�ͷŶ��󣬲�Ȼ�ᵼ������
				(m_pResetWriteStatus[iGroupSN].get() + iMinIndex)->bSaveComplete = true;
				if ((m_pResetWriteStatus[iGroupSN].get() + iMinIndex)->bFnIsComplete())
				{
					m_csWriteDataStatus[iGroupSN].Lock();
					m_pWriteDataStatus[iGroupSN][iMinIndex] = e_WriteDataStatus_Leisure;
					m_pDetectStatus[iGroupSN][iMinIndex] = e_DetectStatus_NoChecking;
					m_csWriteDataStatus[iGroupSN].Unlock();
				}
				m_srwlImgsWaitForSave.Unlock();
				break;
			}
			CExtenInfo* pExtenInfo = (CExtenInfo*)(int(sImgInfoWaitForSave.sCheckInParam.iReserve4));
			TCHAR* lpszSaveImagePath = pExtenInfo->GetFuncStruct(e_Fn_CheckInParam_SaveImagePath).GetArrayTCHAR();

			CString sSaveImgFolderPath{ _T("") };
#ifdef UNICODE
			wstring wsSaveImgFolderPath = CStringConvert::ANSI_UCS2(lpszSaveImagePath);
			sSaveImgFolderPath.Format(_T("%s"), wsSaveImgFolderPath.c_str());
#else
			sSaveImgFolderPath.Format(_T("%s"), lpszSaveImagePath);
#endif
			sSaveImgFolderPath.Format(_T("%s"), lpszSaveImagePath);
			/** Check Disk free space              [6/12/2017 ConanSteve]**/
			{

				ULONGLONG ulFreeSpace = GetFreeSpaceOfHardDisk(sSaveImgFolderPath);
				while (ulFreeSpace <= m_iSizeOfOneArrImgs && ulFreeSpace <50 * 1024 * 1024)
				{
					/** ���ɾ����ʷ��¼��Ϊ��,ɾ������ͼ��              [6/12/2017 ConanSteve]**/
					if (!m_queSaveImgHistory.empty())
					{
						auto sSaveImgInfo = m_queSaveImgHistory.front();
						auto vec_sSaveImgPaths = sSaveImgInfo.vec_sSaveImgPaths;
						m_queSaveImgHistory.pop();

						for (auto itera = vec_sSaveImgPaths.begin(); itera != vec_sSaveImgPaths.end(); ++itera)
						{
							DeleteDirectory((*itera).GetBuffer(MAX_PATH));
						}

					}
					ulFreeSpace = GetFreeSpaceOfHardDisk(sSaveImgFolderPath);
				}
				//	}
				/************************************************************************/
				/*       ��ͼ                [6/12/2017 ConanSteve]                         */
				{
					CString sAlgConfigPath = m_strAppDir + _T("/Config/AlgConfig.ini");
					bool bEnableSequentialCheck = 1 == GetPrivateProfileInt(_T("Demo Controls"), _T("EnableSequentialCheck"), 0, sAlgConfigPath);
					CString sSequentialCheckFolderFilter{ _T("") };
					if (bEnableSequentialCheck)
					{
						GetPrivateProfileString(_T("Demo Controls"), _T("SequentialCheckFolderFilter"), _T(""), sSequentialCheckFolderFilter.GetBuffer(256), 256, sAlgConfigPath);
					}

					CString sSaveImgFilePath{ _T("") };/** ͼ���ļ�·����              [6/12/2017 ConanSteve]**/
					CString sSaveImgFolderPath{ _T("") };/** ��ͼ·���ļ�����              [6/12/2017 ConanSteve]**/
					CString sImgSNName{ _T("") };//*[7/13/2017 ConanSteve]*:  ��ǰͼƬ�����ļ�������
					sImgSNName.Format(_T("\\%s%08d"), bEnableSequentialCheck ? sSequentialCheckFolderFilter : _T("Needle_"), sImgInfoWaitForSave.SN);
					sSaveImgFolderPath += sImgSNName;
					CreateDirectory(sSaveImgFolderPath, NULL);

					m_srwlImgQue[iGroupSN].Lock();
					int iImageWidth = pExtenInfo->GetFuncStruct(e_Fn_CheckInParam_ImageWidth).GetInt();
					int iImageHeight = pExtenInfo->GetFuncStruct(e_Fn_CheckInParam_ImageHeight).GetInt();
					int iImageChannel = pExtenInfo->GetFuncStruct(e_Fn_CheckInParam_ImageChannel).GetInt();
					for (int i = 0; i <pExtenInfo->GetFuncStruct(e_Fn_CheckInParam_SmallImageNumOfEachGrab).GetInt(); ++i)
					{
						sSaveImgFilePath.Format(_T("\\%02d.bmp"), i + 1);
						sSaveImgFilePath = sSaveImgFolderPath + sSaveImgFilePath;
						unsigned char* pAddr = (unsigned char*)(sImgInfoWaitForSave.sCheckInParam.pImageData + i * (iImageWidth)*(iImageHeight)*(iImageChannel));
						SaveBmp(sSaveImgFilePath, iImageWidth, iImageHeight, iImageChannel, pAddr);
					}
					m_srwlImgQue[iGroupSN].Unlock();
					/************************************************************************/
					/*                       [6/27/2017 ConanSteve]                         */
					(m_pResetWriteStatus[iGroupSN].get() + iMinIndex)->bSaveComplete = true;
					if ((m_pResetWriteStatus[iGroupSN].get() + iMinIndex)->bFnIsComplete())
					{
						m_csWriteDataStatus[iGroupSN].Lock();
						m_pWriteDataStatus[iGroupSN][iMinIndex] = e_WriteDataStatus_Leisure;
						m_pDetectStatus[iGroupSN][iMinIndex] = e_DetectStatus_NoChecking;
						m_csWriteDataStatus[iGroupSN].Unlock();
					}
					/************************************************************************/
					bool bNewImgSN{ false };
					if (m_queSaveImgHistory.empty())
					{
						bNewImgSN = true;
					}
					else
					{
						if (sImgInfoWaitForSave.SN != m_queSaveImgHistory.front().SN)
						{
							bNewImgSN = true;
						}
					}
					if (bNewImgSN)/** �µ�ͼ���ͼ��              [6/12/2017 ConanSteve]**/
					{
						s_ImgInfoBeenSaved sImgInfo;
						vector<CString> vec_sImgPath;
						vec_sImgPath.push_back(sSaveImgFolderPath);
						sImgInfo.SN = sImgInfoWaitForSave.SN;
						sImgInfo.vec_sSaveImgPaths = vec_sImgPath;
						m_queSaveImgHistory.push(sImgInfo);
					}
					else/** �Ѿ����ڴ�ͼ��ŵ�·����              [6/12/2017 ConanSteve]**/
					{
						s_ImgInfoBeenSaved* pImgInfo = &m_queSaveImgHistory.front();
						pImgInfo->vec_sSaveImgPaths.push_back(sSaveImgFolderPath);
					}
				}
				/************************************************************************/
			}

		}
		m_srwlImgsWaitForSave.Unlock();
		Sleep(1);
	}
}

// BOOL CDataClass::OP_Directory(LPCSTR lpsrcPath, LPCSTR lpdstPath, int type, int flag)
// {
// 	SHFILEOPSTRUCT FileOp;
// 
// 	FileOp.hwnd = NULL;
// 	FileOp.wFunc = type;
// 	FileOp.pFrom = lpsrcPath;
// 	FileOp.pTo = lpdstPath;
// 	FileOp.fFlags = flag;//FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR|FOF_NOERRORUI|FOF_SILENT;   
// 	FileOp.hNameMappings = NULL;
// 	FileOp.lpszProgressTitle = NULL;
// 
// 	int iRet = -1;
// 	iRet = SHFileOperation(&FileOp);
// 
// 	if (0 == iRet)
// 		return TRUE;
// 	else
// 		return FALSE;
// }


ULONGLONG CDataClass::GetFreeSpaceOfHardDisk(LPCTSTR pDisk)
{
	double dRet = 0;

	ULARGE_INTEGER lpuse;
	ULARGE_INTEGER lptotal;
	ULARGE_INTEGER lpfree;
	GetDiskFreeSpaceEx(pDisk, &lpuse, &lptotal, &lpfree);
	return lpfree.QuadPart;
}

bool CDataClass::DeleteDirectory(TCHAR* sDirName)
{

	CFileFind tempFind;

	TCHAR sTempFileFind[MAX_PATH];

	_tprintf_s(sTempFileFind, _T("%s/*.*"), sDirName);
	BOOL IsFinded = tempFind.FindFile(sTempFileFind);
	while (IsFinded)
	{

		IsFinded = tempFind.FindNextFile();

		if (!tempFind.IsDots())
		{
			TCHAR sFoundFileName[MAX_PATH]{ 0 };
			_tcscpy(sFoundFileName, tempFind.GetFileName().GetBuffer(MAX_PATH));

			if (tempFind.IsDirectory())
			{
				TCHAR sTempDir[MAX_PATH]{ 0 };
				_tprintf(sTempDir, _T("%s/%s"), sDirName, sFoundFileName);
				DeleteDirectory(sTempDir);
			}
			else
			{
				TCHAR sTempFileName[MAX_PATH]{ 0 };
				_tprintf(sTempFileName, _T("%s/%s"), sDirName, sFoundFileName);
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

s_Status CDataClass::DetectEx(s_CheckInParam sCheckInParam)
{
	s_Status sError;
	char chInfo[MAX_CHAR_LENGTH]{ 0 };
	char chLogInfo[MAX_CHAR_LENGTH]{ 0 };

	int i{ 0 }, j{ 0 };
	int iComplete{ 0 };
	int iCurrIndex{ 0 }, iMinIndex{ 0 };
	int iMinCircleSN{ MAX_IMAGE_COUNT };

	//*[7/4/2017 ConanSteve]*:  �ڴ滺�����Ƿ�æ
	BOOL bBusy{ TRUE };

	float fDiff{ 0.0 };
	LONGLONG lInfo{ 0 };
	LARGE_INTEGER liValue{ 0 };

	sError.iStatusType = STATUS_NOR;
	sError.pThis = m_pCallThis;
	sError.pErrorInfo = chInfo;


	// ��ֹͣ������ٵ���
	if (m_bKillThread)
	{
		sError.iStatusType = STATUS_ILG;
		m_csErrorInfoTable.Lock();
		memcpy(&m_arrErrorInfo[m_iErrorInfoIndex][0], _T("Kill Thread!"), sizeof(_T("Kill Thread!")));
		m_iErrorInfoIndex = (m_iErrorInfoIndex + 1) % c_iErrorInfoTableLen;
		m_csErrorInfoTable.Unlock();
		sError.pErrorInfo = &m_arrErrorInfo[m_iErrorInfoIndex][0];
		return sError;
	}

	if (e_CallType_Demo != m_iCallType)
	{
		//*[6/30/2017 ConanSteve]*:  �����              
		int iCameraSN = sCheckInParam.iCheckSN;
		//*[6/30/2017 ConanSteve]*:  ImageSN
		int iImageSN = sCheckInParam.iImageSN;
		//*[6/30/2017 ConanSteve]*:  GroupSN
		int iGroupSN = m_iCamGroup[iCameraSN];
		//**[7/28/2017 ConanSteve]**:  �������������
		int iIndexInGroup = m_iIndexInGroup[iCameraSN];
		int iKickSN = m_iCamKick[iCameraSN];
		//**[12/14/2017 ConanSteve]**:  �Ƿ����µ�ͼ��ţ��п���ϵͳʹ�ô��󣬵��������ڵ�ͼ��ź���һͼ�����ͬ����ʱ����������ͼ��
		bool bCamNewBigImg = m_arr_iCamLastImgSN[iCameraSN] != iImageSN;


		ASSERT(iCameraSN >= 0 && iCameraSN < m_iCameraNum);

		//*[6/30/2017 ConanSteve]*:  ÿһ·���������ĸ���
		//*[6/30/2017 ConanSteve]*:  �Ƚ��Ƿ����µĴ���ͼ
		if (bCamNewBigImg)
		{
			m_iCamCircle[iCameraSN]++;
			m_iCamSmallImgCnt[iCameraSN] = 1;
		}
		else
		{
			++m_iCamSmallImgCnt[iCameraSN];
			if (m_iCamSmallImgCnt[iCameraSN] > m_arr_iCamSmallImgNum[iCameraSN])//**[12/14/2017 ConanSteve]**:  ���ĳһͼ��ŵ�С�ż����������������������ͼ��
			{
				CString sErrorInfo{ _T("") }, sLogInfo{ _T("") };
				sErrorInfo.Format(_T("���%dͼ���%dС��ͼ����%d�����������%d��"), i + 1, iImageSN, m_iCamSmallImgCnt[iCameraSN], m_arr_iCamSmallImgNum[iCameraSN]);
				sError.pErrorInfo = GetErrorInfo(sErrorInfo);
				sLogInfo.Format(_T("[�����:%2d] [ͼ���:%8d] [������:%2d] [С�ż���:%2d] [���С�ż���:%2d]"), iCameraSN, iImageSN, iGroupSN, m_arr_iCamSmallImgNum[iCameraSN], m_iCamSmallImgCnt[iCameraSN]);
				m_fileDetect[iGroupSN][10].Write(sLogInfo);
				return sError;
			}
		}
		m_arr_iCamLastImgSN[iCameraSN] = iImageSN;
		//*[6/30/2017 ConanSteve]*:  ����ͼ�����(���������Ӧ�õ���ͼ���)
		int iCircleSN = m_iCamCircle[iCameraSN];

		//*[6/30/2017 ConanSteve]*:  ��ǰ�������ͼ������һ��ͼ���ʱ���
		m_csCameraDiff[iCameraSN].Lock();
		fDiff = 1000 * (lInfo - m_lCamTime[iCameraSN][(iCircleSN - 1) % 256]) / m_dFreq;
		m_lCamTime[iCameraSN][iCircleSN % 256] = lInfo;
		m_csCameraDiff[iCameraSN].Unlock();


		CString sInfo{ _T("") };
		sInfo.Format(_T("[�����:%2d] [ͼ���:%8d] [������:%2d] [�޷Ϻ�:%2d] [�����������:%2d] [�޷�����:%2d] [���Ÿ���:%8d]"),
			iCameraSN, iImageSN, iGroupSN, iKickSN + 1, iIndexInGroup, m_iIndexInKick[iCameraSN], iCircleSN);
		m_fileCam[iCameraSN][0].Write(sInfo);


		//*[6/30/2017 ConanSteve]*:  ������������Ǹ��޷϶��е����߷Ͽ�����������ţ���ʼ���޷ϳ�ʱ��ʱ
		if (m_iTimeOutIndex[iKickSN] == iCameraSN && bCamNewBigImg)
		{
			m_iKickImageSN[iKickSN][iCircleSN % 256] = iImageSN;

			if (m_bEnableOvertimeCallback)
			{
				//*[6/30/2017 ConanSteve]*:  ��ʱ��׼��ʱ��
				if (e_TimeType_MilliSecond == m_iTimeType[iKickSN])
				{
					m_spTime[iKickSN][iCircleSN % 256].Start();
				}
			}
			m_iKickCircleSN[iKickSN] = iCircleSN;
		}

		//*[6/30/2017 ConanSteve]*:  �ж��Ƿ���������
		if (TRUE == JudgeNewCircle(iGroupSN, iCameraSN, iImageSN, iCircleSN))
		{
			iCurrIndex = 0;
			ThreadWriteResult(iImageSN, iGroupSN, iKickSN, false);
			//*[6/30/2017 ConanSteve]*:�õ�ǰ���ͼ��Ϊδ����
			m_csCamComplete[iCameraSN].Lock();
			m_bCamComplete[iCameraSN][iImageSN % 256] = FALSE;
			m_csCamComplete[iCameraSN].Unlock();
			// �ж���һ�������Ƿ����,��δ��ɣ���ͼ���
			int iLastImageSN = m_iPeriodImageSN[iGroupSN][m_iGroupCircle[iGroupSN] % 256];
			int iLastIndex = m_iPeriodIndex[iGroupSN][m_iGroupCircle[iGroupSN] % 256];

			iComplete = 0;
			//*[7/4/2017 ConanSteve]*:  �ж�ָ����⹤λ����һ��ͼ���Ƿ���
			for (i = 0; i < m_iGroupElementNum[iGroupSN]; i++)
			{
				if (m_bCamComplete[m_iGroupFirstCam[iGroupSN] + i][iLastImageSN % 256])
				{
					iComplete++;
				}
			}
			//*[7/5/2017 ConanSteve]*:  д��־
			sInfo.Format(_T("[�����:%2d] [ͼ���:%8d] [��һͼ���:%8d] [������:%2d] [�޷϶��к�:%2d] [��һ����:%2d] [�������ͼ�����:%8d] [�������ͼ����:%2d] [���������:%d]"),
				iCameraSN, iImageSN, iGroupSN, iKickSN, iLastImageSN, iLastIndex, m_iCamCircle[iCameraSN], iComplete,
				m_iGroupElementNum[iGroupSN]);
			m_fileDetect[iGroupSN][1].Write(sInfo);

			//*[6/30/2017 ConanSteve]*:����ʵ�ʲɼ���δ����ͼ����Ȼ��ȥ���
			if (m_iCallType == e_CallType_RealTime)
			{
				if (iComplete != m_iGroupElementNum[iGroupSN] && m_iCamCircle[iGroupSN] != 1)
				{
					// �������
					SetEvent(m_hImageInspectStart[iGroupSN]);

					m_csWriteDataStatus[iGroupSN].Lock();
					m_pWriteDataStatus[iGroupSN][iLastIndex] = e_WriteDataStatus_Complete;
					m_csWriteDataStatus[iGroupSN].Unlock();

					sInfo.Format(_T("[������:%2d] [�����:%2d] [��ǰͼ���:%08d] [��һ��λͼ���:%8d] [��һ��λ������:%2d]"),
						iGroupSN, iCameraSN, iImageSN, iLastImageSN, iComplete);
					m_fileDetect[iGroupSN][6].Write(sInfo);
				}
			}

			// ��ѯ�����п��ж���
			m_csWriteDataStatus[iGroupSN].Lock();
			int iStatusBefore = 0;
			for (i = 0; i < m_iGroupCheckQueueNum[iGroupSN]; i++)
			{
				iStatusBefore = m_pWriteDataStatus[iGroupSN][i];
				if (m_pWriteDataStatus[iGroupSN][i] != e_WriteDataStatus_Leisure)
				{
					continue;
				}
				else
				{
					bBusy = FALSE;
					iCurrIndex = i;
					m_pWriteDataStatus[iGroupSN][i] = e_WriteDataStatus_Busy;
					++m_ppCacheUseCnt[iGroupSN][i];
					break;
				}
			}
			m_csWriteDataStatus[iGroupSN].Unlock();

			/** д��־              [5/5/2017 ConanSteve]**/
			sInfo.Format(_T("[�����:%2d] [������:%2d] [ͼ���:%8d] [�����Ƿ�æ:%s] [�����������:%d] [����д֮ǰ״̬:%6s]"),
				iCameraSN, iGroupSN, iImageSN, bBusy ? _T("��") : _T("��"), iCurrIndex,
				e_WriteDataStatus_Leisure == iStatusBefore ? _T("����") : (e_WriteDataStatus_Busy == iStatusBefore) ? _T("����д") : _T("��д��"));
			m_fileDetect[iGroupSN][2].Write(sInfo);

			//*[6/30/2017 ConanSteve]*:���û�п��ж��� �����ȴ�������С���Ԫ�������޳������У�������������	
			//*[6/30/2017 ConanSteve]*:�滻��Сͼ���
			++m_iGroupCircle[iGroupSN];
			if (TRUE == bBusy)
			{
				m_iGroupAbnormalNum[iGroupSN]++;//*[6/30/2017 ConanSteve]*:�쳣ͼ��������

				m_csWriteDataStatus[iGroupSN].Lock();
				for (i = 0; i < m_iGroupCheckQueueNum[iGroupSN]; ++i)
				{
					if (iMinCircleSN > m_pCircleSN[iGroupSN][i] && m_pWriteDataStatus[iGroupSN][i] == e_WriteDataStatus_Complete && m_pDetectStatus[iGroupSN][i] == e_DetectStatus_NoChecking)
					{
						iMinCircleSN = m_pCircleSN[iGroupSN][i];
						iMinIndex = i;
					}
				}
				m_csWriteDataStatus[iGroupSN].Unlock();

				//*[6/30/2017 ConanSteve]*: ��δ�ڼ���Ԫ��
				//*[6/30/2017 ConanSteve]*:�п��滻�Ķ���Ԫ��
				bool bHasCompleteNoCheck = MAX_IMAGE_COUNT != iMinCircleSN;
				sInfo.Format(_T("[��ͼͼ���:%08d] [���滻ͼ���:%08d] [�����Ƿ���δ�ͼ������:%4s] [��ͼ����:%8d]"),
					iImageSN,
					bHasCompleteNoCheck ? m_spCheckInParam[iGroupSN][iMinCircleSN].iImageSN : 0,
					bHasCompleteNoCheck ? _T("��") : _T("��"),
					m_iGroupAbnormalNum[iGroupSN]);
				m_fileDetect[iGroupSN][7].Write(sInfo);

				if (bHasCompleteNoCheck)
				{
					//*[6/30/2017 ConanSteve]*:�ɵ�ͼ��Ž����޷϶���
					iCurrIndex = iMinIndex;
					int iOldImageSN = m_spCheckInParam[iGroupSN][iMinIndex].iImageSN;

					//*[6/30/2017 ConanSteve]*:����״̬�ص� ֪ͨϵͳ
					char chStatusInfo[MAX_CHAR_LENGTH]{ 0 };
					char chInfo[c_iMaxStringLen]{ 0 };
					s_Status sStatus;
					sStatus.iCheckSN = iCameraSN;
					sStatus.iStatusType = STATUS_THROW_IMAGE;
					sStatus.pThis = m_pCallThis;
					sStatus.pParam = NULL;
					m_csErrorInfoTable.Lock();
					sprintf(chInfo, "Checking Image No.%d, Throwing Image No.%d!", iImageSN, iOldImageSN);
					sStatus.pErrorInfo = GetErrorInfo(chInfo);
					m_StatusCallback(&sStatus);

					m_csWriteDataStatus[iGroupSN].Lock();
					m_pWriteDataStatus[iGroupSN][iCurrIndex] = e_WriteDataStatus_Busy;
					m_csWriteDataStatus[iGroupSN].Unlock();

					//*[6/30/2017 ConanSteve]*:����ͼ������ͼ��,����д��״̬
					m_spCheckInParam[iGroupSN][iCurrIndex].iCheckSN = iGroupSN;
					m_spCheckInParam[iGroupSN][iCurrIndex].iImageSN = iImageSN;
					m_spCheckInParam[iGroupSN][iCurrIndex].iReserve1 = sCheckInParam.iReserve1;
					m_spCheckInParam[iGroupSN][iCurrIndex].iReserve2 = sCheckInParam.iReserve2;
					m_spCheckInParam[iGroupSN][iCurrIndex].iReserve3 = sCheckInParam.iReserve3;
					m_spCheckInParam[iGroupSN][iCurrIndex].iReserve4 = sCheckInParam.iReserve4;
					m_pCircleSN[iGroupSN][iCurrIndex] = m_iGroupCircle[iGroupSN];
					LARGE_INTEGER liTime;
					QueryPerformanceCounter(&liTime);
					m_arr_ldTickOfPushInCacheQueue[iGroupSN][iCurrIndex]=liTime.QuadPart;

					m_iPeriodImageSN[iGroupSN][m_iGroupCircle[iGroupSN] % 256] = sCheckInParam.iImageSN;
					m_iPeriodIndex[iGroupSN][m_iGroupCircle[iGroupSN] % 256] = iCurrIndex;


					m_srwlImgQue[iGroupSN].Lock();
					//*[6/30/2017 ConanSteve]*:���ͼ�񻺴��ַ
					BYTE* pAddr = (BYTE*)(m_spCheckInParam[iGroupSN][iCurrIndex].pImageData +
						(m_iCamSmallImgCnt[iCameraSN] - 1)*m_iSmallImgSize[iCameraSN]);
					for (int i = 0; i < iIndexInGroup; ++i)
					{
						pAddr += m_iImageSize[m_iGroupFirstCam[iGroupSN] + i];//*[7/14/2017 ConanSteve]*:  ����ͬһ��λ���������ͼ��ĵ�ַƫ��
					}
					memcpy_s(pAddr, m_iSmallImgSize[iCameraSN], sCheckInParam.pImageData, m_iSmallImgSize[iCameraSN]);

					BYTE* pParam = (BYTE*)(m_spCheckInParam[iGroupSN][iCurrIndex].pParam);
					memcpy_s(pParam, m_iGroupCheckInSize[iGroupSN], sCheckInParam.pParam, m_iGroupCheckInSize[iGroupSN]);
					m_srwlImgQue[iGroupSN].Unlock();

					sInfo.Format(_T("[������:%2d] [�����:%2d] [ͼ����ź�:%8d] [ͼ��С�ź�:%2d] [��������:%2d]"),
						iGroupSN, iCameraSN, iImageSN, m_iCamSmallImgCnt[iCameraSN], iIndexInGroup);
					m_fileDetect[iGroupSN][3].Write(sInfo);

				}
				//*[6/30/2017 ConanSteve]*:�޿��滻�Ķ���Ԫ��
				else
				{
					sError.iStatusType = STATUS_ILG;
					ZeroMemory(chInfo, MAX_CHAR_LENGTH);

					CString str1{ _T("") }, str2{ _T("") };

					// ����
					str1.Format(_T("Cam:%d"), iGroupSN);

					// ͼ���
					str2.Format(_T("C:%d "), iImageSN);
					str1 += str2;

					// �������е����
					for (i = 0; i < m_iGroupCheckQueueNum[iGroupSN]; i++)
					{
						str2.Format(_T("%d "), m_pCircleSN[iGroupSN][i]);
						str1 += str2;
					}

					// ��������Ԫ��ͼ������д��״̬
					str2.Format(_T("W"));
					for (i = 0; i < m_iGroupCheckQueueNum[iGroupSN]; i++)
					{
						str2.Format(_T("%d "), m_pWriteDataStatus[iGroupSN][i]);
						str1 += str2;
					}

					// ��������Ԫ�ؼ��״̬
					str2.Format(_T("D"));
					for (i = 0; i < m_iGroupCheckQueueNum[iGroupSN]; i++)
					{
						str2.Format(_T("%d "), m_pDetectStatus[iGroupSN][i]);
						str1 += str2;
					}

					memcpy(chInfo, str1.GetBuffer(0), str1.GetLength());
					// ����״̬�ص� ֪ͨϵͳ���ж���Ԫ���Ѿ����꣬��ȫ���μӼ��
					s_Status sStatus;
					char chInfo[c_iMaxStringLen]{ 0 };
					ZeroMemory(&sStatus, sizeof(sStatus));
					sStatus.iCheckSN = iCameraSN;
					sStatus.iStatusType = STATUS_THROW_IMAGE;
					sStatus.pThis = m_pCallThis;
					sStatus.pParam = NULL;
					m_csErrorInfoTable.Lock();
					sprintf(chInfo, "Current Image No.%d, All Elements are busy!", iImageSN);
					sStatus.pErrorInfo = GetErrorInfo(chInfo);
					m_StatusCallback(&sStatus);

					return sStatus;
				}

			}
			//*[6/30/2017 ConanSteve]*:����ǰͼ�����ӽ�������
			else
			{
				m_csWriteDataStatus[iGroupSN].Lock();
				m_pWriteDataStatus[iGroupSN][iCurrIndex] = e_WriteDataStatus_Busy;
				m_csWriteDataStatus[iGroupSN].Unlock();

				//*[6/30/2017 ConanSteve]*:����ͼ������ͼ��,����д��״̬
				m_spCheckInParam[iGroupSN][iCurrIndex].iCheckSN = iGroupSN;
				m_spCheckInParam[iGroupSN][iCurrIndex].iImageSN = iImageSN;
				m_spCheckInParam[iGroupSN][iCurrIndex].iReserve1 = sCheckInParam.iReserve1;
				m_spCheckInParam[iGroupSN][iCurrIndex].iReserve2 = sCheckInParam.iReserve2;
				m_spCheckInParam[iGroupSN][iCurrIndex].iReserve3 = sCheckInParam.iReserve3;
				m_spCheckInParam[iGroupSN][iCurrIndex].iReserve4 = sCheckInParam.iReserve4;
				LARGE_INTEGER liTime;
				QueryPerformanceCounter(&liTime);
				m_arr_ldTickOfPushInCacheQueue[iGroupSN][iCurrIndex]=liTime.QuadPart;

				m_pCircleSN[iGroupSN][iCurrIndex] = m_iGroupCircle[iGroupSN];

				m_iPeriodImageSN[iGroupSN][m_iGroupCircle[iGroupSN] % 256] = sCheckInParam.iImageSN;
				m_iPeriodIndex[iGroupSN][m_iGroupCircle[iGroupSN] % 256] = iCurrIndex;


				m_srwlImgQue[iGroupSN].Lock();
				//*[6/30/2017 ConanSteve]*:���ͼ�񻺴��ַ
				BYTE* pAddr = (BYTE*)(m_spCheckInParam[iGroupSN][iCurrIndex].pImageData +
					(m_iCamSmallImgCnt[iCameraSN] - 1)*m_iSmallImgSize[iCameraSN]);
				for (int i = 0; i < iIndexInGroup; ++i)
				{
					pAddr += m_iImageSize[m_iGroupFirstCam[iGroupSN] + i];//*[7/14/2017 ConanSteve]*:  ����ͬһ��λ���������ͼ��ĵ�ַƫ��
				}
				memcpy_s(pAddr, m_iSmallImgSize[iCameraSN], sCheckInParam.pImageData, m_iSmallImgSize[iCameraSN]);

				BYTE* pParam = (BYTE*)(m_spCheckInParam[iGroupSN][iCurrIndex].pParam);
				memcpy_s(pParam, m_iGroupCheckInSize[iGroupSN], sCheckInParam.pParam, m_iGroupCheckInSize[iGroupSN]);
				m_srwlImgQue[iGroupSN].Unlock();

				sInfo.Format(_T("[������:%2d] [�����:%2d] [ͼ����ź�:%8d] [ͼ��С�ź�:%2d] [��������:%2d]"),
					iGroupSN, iCameraSN, iImageSN, m_iCamSmallImgCnt[iCameraSN], iIndexInGroup);
				m_fileDetect[iGroupSN][3].Write(sInfo);

			}

		}
		//*[6/30/2017 ConanSteve]*:  ����������,ֱ�ӿ���ͼ��		
		else
		{
			m_csWriteDataStatus[iGroupSN].Lock();
			int iStatusBefore = 0;
			for (i = 0; i < m_iGroupCheckQueueNum[iGroupSN]; i++)
			{
				iStatusBefore = m_pWriteDataStatus[iGroupSN][i];
				if (m_pWriteDataStatus[iGroupSN][i] == e_WriteDataStatus_Busy)
				{
					iCurrIndex = i;
					break;
				}
			}
			m_csWriteDataStatus[iGroupSN].Unlock();

			/** д��־              [5/5/2017 ConanSteve]**/
			sInfo.Format(_T("[�����:%2d] [������:%2d] [ͼ���:%8d] [�����Ƿ�æ:%s] [�����������:%d] [����д֮ǰ״̬:%6s]"),
				iCameraSN, iGroupSN, iImageSN, bBusy ? _T("��") : _T("��"), iCurrIndex,
				e_WriteDataStatus_Leisure == iStatusBefore ? _T("����") : (e_WriteDataStatus_Busy == iStatusBefore) ? _T("����д") : _T("��д��"));
			m_fileDetect[iGroupSN][2].Write(sInfo);




			m_srwlImgQue[iGroupSN].Lock();
			//*[6/30/2017 ConanSteve]*:���ͼ�񻺴��ַ
			BYTE* pAddr = (BYTE*)(m_spCheckInParam[iGroupSN][iCurrIndex].pImageData +
				(m_iCamSmallImgCnt[iCameraSN] - 1)*m_iSmallImgSize[iCameraSN]);
			for (int i = 0; i < iIndexInGroup; ++i)
			{
				pAddr += m_iImageSize[m_iGroupFirstCam[iGroupSN] + i];
			}
			memcpy_s(pAddr, m_iSmallImgSize[iCameraSN], sCheckInParam.pImageData, m_iSmallImgSize[iCameraSN]);
			//			BYTE* pParam = (BYTE*)(m_spCheckInParam[iGroupSN][iCurrIndex].pParam);
			// 			memcpy_s(pParam, m_iGroupCheckInSize[iGroupSN], sCheckInParam.pParam, m_iGroupCheckInSize[iGroupSN]);
			// 			m_srwlImgQue[iGroupSN].Unlock();
			sInfo.Format(_T("[������:%2d] [�����:%2d] [ͼ����ź�:%8d] [ͼ��С�ź�:%2d] [��������:%2d]"),
				iGroupSN, iCameraSN, iImageSN, m_iCamSmallImgCnt[iCameraSN], iIndexInGroup);
			m_fileDetect[iGroupSN][3].Write(sInfo);
		}
		//*[6/30/2017 ConanSteve]*:  �����ǰ���ͼ��ȫ������
		if (m_iCamSmallImgCnt[iCameraSN] == m_arr_iCamSmallImgNum[iCameraSN])
		{
			m_csCamComplete[iCameraSN].Lock();
			m_bCamComplete[iCameraSN][iImageSN % 256] = TRUE;
			m_csCamComplete[iCameraSN].Unlock();
		}
		//*[6/30/2017 ConanSteve]*:  �鿴����λһ������ͼ���Ƿ�����
		iComplete = 0;
		for (i = 0; i < m_iGroupElementNum[iGroupSN]; i++)
		{
			if (m_bCamComplete[m_iGroupFirstCam[iGroupSN] + i][iImageSN % 256])
			{
				iComplete++;
			}
		}
		//*[6/30/2017 ConanSteve]*:  �����ǰ��λ�������������ͼ�񣬴������ 
		if (iComplete == m_iGroupElementNum[iGroupSN])
		{
			m_csWriteDataStatus[iGroupSN].Lock();
			m_pWriteDataStatus[iGroupSN][iCurrIndex] = e_WriteDataStatus_Complete;
			m_csWriteDataStatus[iGroupSN].Unlock();

			// �������
			SetEvent(m_hImageInspectStart[iGroupSN]);

			//*[6/30/2017 ConanSteve]*:  д��־
			sInfo.Format(_T("[�����:%2d] [������:%2d] [ͼ���:%8d] [��������:%8d] [��������:%2d] [�޷ϱ��Ƿ�д��:%d]"),
				iCameraSN, iGroupSN, iImageSN, iCircleSN, iCurrIndex, *m_pbKickWrite[0][iImageSN % 256]);
			m_fileDetect[iGroupSN][4].Write(sInfo);
		}

	}

	return sError;
}


void CDataClass::CheckEx2(int iIndex, int iThreadID, s_CheckInParam sCheckInParam)
{
	/***********************************<��ʼ������>*************************************/
	CString sLogInfo{ _T("") };
	int i = 0, j = 0;
	int iIdleObjIndex{ 0 };//**[7/14/2017 ConanSteve]**:  ���м���������
	int iImgSN = sCheckInParam.iImageSN;//ͼ��ţ���ʱû��
	int iGroupSN = sCheckInParam.iCheckSN;//���ڼ�·
	bool bBusy{ true };//**[8/2/2017 ConanSteve]**:  �������Ƿ�ȫæ
	s_CheckInParam* spCheckInParam{ NULL };
	s_CheckOutParam * spCheckOutParam{ NULL };
	s_Status* spCheckStatusParam{ NULL };
	CSpendTime spTime;
	CInspBase* pInsp{ NULL };
	int iElapse1{ 0 }, iElapse2{ 0 };//**[8/2/2017 ConanSteve]**:  �㷨2��Detect����ʱ��
	s_AlgModelInput sIn;
	/*************************************<���ҵ�ǰ״̬Ϊ���еļ�����>*************************************/
	{
		m_csAlgCheck[iGroupSN].Lock();//��������

		for (i = 0; i < m_pnObjectNumber[iGroupSN]; i++)
		{
			if (m_nObjIndex[iGroupSN][i] == e_DetectStatus_Checking)
			{
				continue;
			}
			iIdleObjIndex = i;
			++m_ppObjUseCnt[iGroupSN][i];
			m_nObjIndex[iGroupSN][i] = e_DetectStatus_Checking;//����־λ��Ϊ1
			bBusy = false;
			break;
		}
		m_csAlgCheck[iGroupSN].Unlock();//����ǰ����
	}

	/*************************************<������ȫæ>**************************************/
	if (bBusy)
	{
		InterlockedIncrement(&m_plObjBusyTimes[iGroupSN]);//m_plObjBusyTimes���������1
		m_csWriteDataStatus[iGroupSN].Lock();
		m_pDetectStatus[iGroupSN][iIndex] = e_DetectStatus_NoChecking;
		m_csWriteDataStatus[iGroupSN].Unlock();

		sLogInfo.Format(_T("[������Ϣ:�����%2d�ļ�����ȫæ]"), iGroupSN + 1);
		m_fileCheck[iGroupSN][6].Write(sLogInfo);
		return;
	}
	/*************************************<�п��м�������ͼ��>************************************/
	else
	{

		pInsp = (CInspBase*)(m_iIdleJob[iGroupSN]) + iIdleObjIndex;
		m_iGroupAlgCheck[iGroupSN]++;

		sLogInfo.Format(_T("[������:%2d] [�߳�ID:%2d] [ͼ���:%8d] [�����������:%2d] [����������:%2d]"), iGroupSN, iThreadID, iImgSN, iIndex, iIdleObjIndex);
		m_fileCheck[iGroupSN][0].Write(sLogInfo);

		m_arr_ldTickOfPushInAlgObjQueue[iGroupSN][iIdleObjIndex] = m_arr_ldTickOfPushInCacheQueue[iGroupSN][iIndex];
		LARGE_INTEGER liEndTime, liFreq;
		QueryPerformanceFrequency(&liFreq);
		QueryPerformanceCounter(&liEndTime);
		m_arr_iTimeOfWaitInAlgObjQueue[iGroupSN][iIdleObjIndex]=(liEndTime.QuadPart - m_arr_ldTickOfPushInAlgObjQueue[iGroupSN][iIdleObjIndex]) * 1000 / liFreq.QuadPart;
		/*************************************<���>*************************************/
		{
			CExtenInfo extenInfo;
			CExtenInfo* pExtenInfo{ nullptr };
			if (0 != sCheckInParam.iReserve4)//**[9/15/2017 ConanSteve]**:  �ж�ϵͳ�Ƿ񴫵��˴˲�������������ˣ���Ҫʹ��ϵͳ��ָ��
			{
				pExtenInfo = (CExtenInfo*)sCheckInParam.iReserve4;
				extenInfo = *pExtenInfo;
			}
			int iExtenInfoTemp = sCheckInParam.iReserve4;//**[9/26/2017 ConanSteve]**:  ��ʱ���汣����4��ֵ
			sCheckInParam.iReserve4 = (int)&extenInfo;
			if (e_ObjDetectMode_1 == m_iObjDetectMode)
			{
				spCheckInParam = &m_spCheckInParam[iGroupSN][iIndex];
				spCheckOutParam = &m_spCheckOutParam[iGroupSN][iIndex];
				spCheckStatusParam = &m_spCheckStatusParam[iGroupSN][iIndex];

				extenInfo.push_back(COptionalFunc(e_Fn_CheckInParam_DetectTimes).SetInt(0));
				sCheckInParam.iReserve3 = 0;
				spTime.Start();
				*spCheckStatusParam = pInsp->Detect(sCheckInParam, spCheckOutParam);
				spTime.End();
				iElapse1 = spTime.GetMillisecondInt();

			}
			else if (e_ObjDetectMode_2 == m_iObjDetectMode)
			{
				CopyCheckData(iIndex, iIdleObjIndex, iGroupSN);
				spCheckInParam = &m_sppCheckInParamEx[iGroupSN][iIdleObjIndex];
				spCheckOutParam = &m_sppCheckOutParamEx[iGroupSN][iIdleObjIndex];
				spCheckStatusParam = &m_sppCheckStatusParamEx[iGroupSN][iIdleObjIndex];

				extenInfo.push_back(COptionalFunc(e_Fn_CheckInParam_DetectTimes).SetInt(1));
				sCheckInParam.iReserve3 = 1;
				spTime.Start();
				*spCheckStatusParam = pInsp->Detect(sCheckInParam, spCheckOutParam);
				spTime.End();
				iElapse1 = spTime.GetMillisecondInt();
				//*[7/14/2017 ConanSteve]*:  �㷨����ͼ����ɣ���������ڴ�����
				{
					m_csWriteDataStatus[iGroupSN].Lock();
					m_pWriteDataStatus[iGroupSN][iIndex] = e_WriteDataStatus_Leisure;
					m_pDetectStatus[iGroupSN][iIndex] = e_DetectStatus_NoChecking;
					m_csWriteDataStatus[iGroupSN].Unlock();
				}
				extenInfo.push_back(COptionalFunc(e_Fn_CheckInParam_DetectTimes).SetInt(2));
				sCheckInParam.iReserve3 = 2;
				spTime.Start();
				*spCheckStatusParam = pInsp->Detect(sCheckInParam, spCheckOutParam);
				spTime.End();
				iElapse2 = spTime.GetMillisecondInt();
			}
			sCheckInParam.iReserve4 = iExtenInfoTemp;
		}
		/************************************************************************/
		BOOL bRet = ThreadWriteCheckResultEx2(iGroupSN, spCheckInParam, spCheckOutParam);//**[8/2/2017 ConanSteve]**:  д�޷ϱ�

																						 //**[10/9/2017 ConanSteve]**:  ��д�㷨ʱ����Ϣ
		//**[8/2/2017 ConanSteve]**:    ��־��д��ǰ�������Ϣ
		sLogInfo.Format(_T("[������:%2d] [�߳�ID:%2d] [ͼ���:%8d] [�����������:%2d] [�Ƿ��ͼ:%2s] [�Ƿ�ͼ:%2s] [1��Detectʱ��:%4d] [2��Detectʱ��:%4d] [�㷨���ʱ��:%4d] [���ݵȴ�ʱ��:%8d]"),
			iGroupSN, iThreadID, iImgSN, iIndex, 1 == spCheckOutParam->iSave ? _T("��") : _T("��"),
			1 == spCheckOutParam->iDisplay ? _T("��") : _T("��"),
			iElapse1, iElapse2, iElapse1 + iElapse2, m_arr_iTimeOfWaitInAlgObjQueue[iGroupSN][iIdleObjIndex]);
		m_fileCheck[iGroupSN][1].Write(sLogInfo);
		/*************************************<��ͼ>*************************************/
		{
			int nDispIndex{ e_WhetherDraw_NotDraw };
			m_csAlgDisp[iGroupSN].Lock();
			if (spCheckOutParam->iDisplay == e_CheckOutWhetherDraw_Draw && !m_pbIsDrawing[iGroupSN])	//0����ʾ��1��ʾ
			{
				m_pbIsDrawing[iGroupSN] = true;
				nDispIndex = e_WhetherDraw_Draw;
			}
			m_csAlgDisp[iGroupSN].Unlock();
			if (nDispIndex == e_WhetherDraw_Draw)
			{
				sIn.iOperSig = e_AlgModelInputOper_CopyWindow;
				sIn.iCheckSN = sCheckInParam.iCheckSN;
				CExtenInfo extenInfo;
				extenInfo.push_back(COptionalFunc(e_Fn_CheckInParam_TimeWait).SetInt(m_arr_iTimeOfWaitInAlgObjQueue[iGroupSN][iIdleObjIndex]));
				extenInfo.push_back(COptionalFunc(e_Fn_CheckInParam_TimeFirstDetect).SetInt(iElapse1));
				extenInfo.push_back(COptionalFunc(e_Fn_CheckInParam_TimeSecondDetect).SetInt(iElapse2));
				int i1 = extenInfo.GetFuncStruct(e_Fn_CheckInParam_TimeFirstDetect).GetInt();// +extenInfo.GetFuncStruct(e_Fn_CheckInParam_TimeWait).GetIntEx() + extenInfo.GetFuncStruct(e_Fn_CheckInParam_TimeSecondDetect).GetIntEx();
				sIn.iReserve4 = (int)&extenInfo;
				s_Status sError2;
				sError2.iStatusType = STATUS_NOR;
				sError2.pThis = m_pCallThis;
				CString sDebugInfo{ _T("") };
				sDebugInfo.Format(_T("Group:%d"), iGroupSN);
				m_fileDebugInfo.Write(_T("-----Before-----") + sDebugInfo);
				sError2 = pInsp->Set(sIn);
				if (m_bKillThread)
					return;
				m_fileDebugInfo.Write(_T("-----After-----") + sDebugInfo);
				if (NULL != m_pbIsDrawing)
					m_pbIsDrawing[iGroupSN] = false;
			}

			(m_pResetWriteStatus[iGroupSN].get() + iIndex)->FnSetVal(false, true);
		}
		/*************************************< ����ͼ��>************************************[1/11/2018 ConanSteve]*/
		if (true == m_bGroupSaveImage[iGroupSN] &&										//Ҫ�󱣴�ͼ��
			1 == spCheckOutParam->iSave &&											//����ͼ������ƥ��
			m_iGroupSaveImageCount[iGroupSN] < m_iGroupSaveImageCount_SET[iGroupSN])	//��������δ��
		{
			m_iGroupSaveImageCount[iGroupSN]++;

			m_csAlgSave[iGroupSN].Lock();
			m_iGroupIsSaving[iGroupSN] = 1;
			m_csAlgSave[iGroupSN].Unlock();

			//**[7/18/2017 ConanSteve]**:  ��д��ͼ��Ϣ
			s_AlgModelInput sIn;
			sIn.iOperSig = e_AlgModelInputOper_SaveImage;							//����ͼ���־
			sIn.iCheckSN = sCheckInParam.iCheckSN;		//��λ��

			if (m_iMachineType == e_MachineType_Capsule)
			{
				sIn.iReserve1 = (m_iSaveImageNum[iGroupSN] * 1000 + sCheckInParam.iImageSN);		//ͼ���
			}
			else if (m_iMachineType == e_MachineType_Liquor)
			{
				sIn.iReserve1 = sCheckInParam.iImageSN;		//**[7/19/2017 ConanSteve]**:  ͼ���
			}
			sIn.iReserve2 = (int)sCheckInParam.pImageData;	//*[7/14/2017 ConanSteve]*:  ͼ�����ݵ�ַ����ȫ��Ϊ�˼��ݣ��Ժ�汾�㷨��ͼ��Ӧ��ʹ�������ַ
			m_iSaveImageNum[iGroupSN]++;


			/*****************************************************************************************/
			m_bIsCircle = false;
			bool bExecuteSaveImgOper{ true };//**[7/14/2017 ConanSteve]**:  �Ƿ���Ҫ���������㷨����ִ�С���ͼ���������������ָʾ�Ƿ�Ž���ͼ���л��ߵ���pInsp-Set(sIn)
			CExtenInfo* pExtenPara{ NULL };//**[7/14/2017 ConanSteve]**:  ��չ�����ṹ��
			//**[7/14/2017 ConanSteve]**:  ʹ������չ�ṹ��
			if (0 != sCheckInParam.iReserve4)
			{
				pExtenPara = (CExtenInfo*)(sCheckInParam.iReserve4);
				//pMultiObjSaveImgPara = (CMultiObjSaveImgPara *)pExtenPara->GetFuncAddr(typeid(CMultiObjSaveImgPara));
				m_bIsCircle = pExtenPara->GetFuncStruct(e_Fn_CheckInParam_WhetherCicleSaveImage).GetBool();
			}
			/*************************************<ѭ����ͼ���̿ռ��ж�>************************************[1/11/2018 ConanSteve]*/
			if(m_bIsCircle)
			{
				if (pExtenPara->GetFuncStruct(e_Fn_CheckInParam_SaveImagePath).GetFuncCode() != e_Fn_NONE)
				{
					//*[7/6/2017 ConanSteve]*:  ��ѭ����ͼ,��Ҫ�жϴ��̿ռ��Ƿ��Լ50M��С��ʱ��ֹͣ��ͼ
					CString sSaveImgFolderPath{ _T("") };
					COptionalFunc opFn = pExtenPara->GetFuncStruct(e_Fn_CheckInParam_SaveImagePath);
					sSaveImgFolderPath.Format(_T("%s"), pExtenPara->GetFuncStruct(e_Fn_CheckInParam_SaveImagePath).GetArrayTCHAR());
					if (false == m_bIsCircle)
					{
						ULONGLONG ulFreeSpace = GetFreeSpaceOfHardDisk(sSaveImgFolderPath);
						//**[7/14/2017 ConanSteve]**:  ���ʣ����̿ռ�С��50M����ͼֹͣ 
						if (ulFreeSpace < 1024 * 1024 * 50)
						{
							bExecuteSaveImgOper = false;
							s_Status sStatus;
							sStatus.iCheckSN = iGroupSN;
							sStatus.iStatusType = STATUS_THROW_IMAGE;
							sStatus.pThis = m_pCallThis;
							sStatus.pParam = NULL;
							sStatus.pErrorInfo = NULL;
							m_StatusCallback(&sStatus);
						}
					}
					//**[7/14/2017 ConanSteve]**:  ѭ����ͼ������ͳһ�жϴ���ʣ��ռ��Ƿ���ڵ�����Ʒһ��ͼ���С֮��
					//**[7/14/2017 ConanSteve]**:  �����ѭ����ͼ��������ͼ����֮ǰ��Ȼ��Ҫ�жϴ��̿ռ䣬��Ϊ��ʱ�ռ乻��������ͼʱ���ռ���ܾͲ�����
					else
					{
						//**[7/14/2017 ConanSteve]**:  �ͷŴ��̿ռ�
						ULONGLONG ulFreeSpace = GetFreeSpaceOfHardDisk(sSaveImgFolderPath);
						while (ulFreeSpace <= m_iSizeOfOneArrImgs || ulFreeSpace <= 50 * 1024 * 1024)
						{
							//**[7/14/2017 ConanSteve]**:   ���ɾ����ʷ��¼��Ϊ��,ɾ������ͼ��
							m_srwlSaveImgHistory.Lock();
							if (!m_queSaveImgHistory.empty())
							{
								auto sSaveImgInfo = m_queSaveImgHistory.front();
								auto vec_sSaveImgPaths = sSaveImgInfo.vec_sSaveImgPaths;
								m_queSaveImgHistory.pop();

								for (auto itera = vec_sSaveImgPaths.begin(); itera != vec_sSaveImgPaths.end(); ++itera)
								{//**[1/25/2018 ConanSteve]**:  �����㷨������ļ��л��ǵ���ͼ�񣬶�ɾ��
									CString sPathOfImgs = *itera;
									CExFn::DeleteDirectory(sPathOfImgs.GetBuffer());
									sPathOfImgs += _T(".bmp");
									DeleteFile(sPathOfImgs);
								}

							}
							else
							{
								m_srwlSaveImgHistory.Unlock();
								bExecuteSaveImgOper = false;//**[8/9/2017 ConanSteve]**:  �㷨û��ʹ��������·����ͼ������ɾͼɾ����,���ٴ�ͼ
								break;
							}
							m_srwlSaveImgHistory.Unlock();
							Sleep(10);
							ulFreeSpace = GetFreeSpaceOfHardDisk(sSaveImgFolderPath);
						}
					}
				}
				else
				{
					ULONGLONG ulFreeSpace = GetFreeSpaceOfHardDisk(m_strVitrualDisk + _T(":"));
					//**[7/14/2017 ConanSteve]**:  ���ʣ����̿ռ�С��50M����ͼֹͣ 
					if (ulFreeSpace < 1024 * 1024 * 50)
					{
						bExecuteSaveImgOper = false;
						s_Status sStatus;
						sStatus.iCheckSN = iGroupSN;
						sStatus.iStatusType = STATUS_THROW_IMAGE;
						sStatus.pThis = m_pCallThis;
						sStatus.pParam = NULL;
						sStatus.pErrorInfo = NULL;
						m_StatusCallback(&sStatus);
					}
				}
			}
			/*************************************<ϵͳ��ͼ>************************************[1/11/2018 ConanSteve]*/
			if (e_SaveImgObj_MultiObj == m_iSaveImgObj)
			{
				/************************************************************************/
				/*                       [6/8/2017 ConanSteve]                         */
				/** ��Ӵ�ͼ���󵽶��У������Ѵ�ͼ�߳�              [6/12/2017 ConanSteve]**/
				if (bExecuteSaveImgOper)
				{
					m_srwlImgsWaitForSave.Lock();
					s_ImgInfoWaitForSave sImgInfoWaitForSave;
					if (e_NamingRegulation_ImgSN == m_iSaveImgNamingRegulation)
					{
						sImgInfoWaitForSave.SN = sCheckInParam.iImageSN;
					}
					else if (e_NamingRegulation_ProductSN == m_iSaveImgNamingRegulation)
					{
						sImgInfoWaitForSave.SN = sCheckInParam.iImageSN;
					}
					sImgInfoWaitForSave.sCheckInParam = sCheckInParam;
					sImgInfoWaitForSave.iMinIndex = iIndex;
					m_queImgsWaitForSave.push(sImgInfoWaitForSave);

					m_srwlImgsWaitForSave.Unlock();

					SetEvent(m_hEventSaveImgs);
					(m_pResetWriteStatus[iGroupSN].get() + iIndex)->FnSetVal(false, false);
				}

			}
			/*************************************<�㷨��ͼ>************************************[1/11/2018 ConanSteve]*/
			else //if (e_SaveImgObj_Alg == m_iSaveImgObj)
			{
				if (bExecuteSaveImgOper)
				{
//					CString sSaveImgFolderPath{ _T("") };/** ��ͼ·���ļ�����              [6/12/2017 ConanSteve]**/
					CString sImgSNName{ _T("") };//**[7/14/2017 ConanSteve]**:  ͼ�������ļ������ƣ������������ļ�����
												 //**[7/19/2017 ConanSteve]**:  ���ʹ����ѭ����ͼ����ָ�벻Ӧ��Ϊ��
					TCHAR lpszPathSaveImage[10][MAX_PATH];
					memset(lpszPathSaveImage, 0, sizeof(lpszPathSaveImage));
					if (e_Fn_NONE != pExtenPara->GetFuncStruct(e_Fn_CheckInParam_SaveImagePath).GetFuncCode())
					{
						//**[7/14/2017 ConanSteve]**:  ȷ����ͼ��������
						CString sAlgConfigPath = m_strAppDir + _T("\\Config\\AlgConfig.ini");
						bool bEnableSequentialCheck = m_iniAlgCfg.GetBoolEx(_T("Demo Controls"), _T("EnableSequentialCheck"), 0, sAlgConfigPath);
						CString sSequentialCheckFolderFilter{ _T("") };
						if (bEnableSequentialCheck)
						{
							sSequentialCheckFolderFilter = m_iniAlgCfg.GetStringEx(_T("Demo Controls"), _T("SequentialCheckFolderFilter"), sSequentialCheckFolderFilter);
						}
						int iGrabNumOfCurCheckGroup = pExtenPara->GetFuncStruct(e_Fn_CheckInParam_GrabNumPerCheckGroup).GetInt();
						memcpy_s(lpszPathSaveImage, sizeof(TCHAR)*MAX_PATH*iGrabNumOfCurCheckGroup, pExtenPara->GetFuncStruct(e_Fn_CheckInParam_SaveImagePath).GetPointer(), sizeof(TCHAR)*MAX_PATH*iGrabNumOfCurCheckGroup);
						
						CString sAppend{ _T("") };
						sAppend.Format(_T("\\%s%08d"), bEnableSequentialCheck ? sSequentialCheckFolderFilter : _T("Product_"), iImgSN);
						if (0 != spCheckOutParam->iReserve4)
						{
							CExtenInfo* pExtenInfo = (CExtenInfo*)spCheckOutParam->iReserve4;
							TCHAR* pBuf = pExtenInfo->GetFuncStruct(e_Fn_CheckOutPara_AlgCheckErrorDesc).GetArrayTCHAR();
							sAppend += _T("_") + CString(pBuf);
						}
						//**[1/25/2018 ConanSteve]**:  ƴ�ӳ������Ĵ�ͼ·��
						for (int i = 0; i < iGrabNumOfCurCheckGroup; ++i)
						{
							StrCat(lpszPathSaveImage[i], sAppend.GetBuffer());
							/*****************************************************************************************/
							//**[7/14/2017 ConanSteve]**:  ��ͬһ��ͼ��ŵĴ�ͼ�ļ��з���һ����������ͬ��ɾ��
							bool bNewImgSN{ false };
							m_srwlSaveImgHistory.Lock();
							if (m_queSaveImgHistory.empty())
							{
								bNewImgSN = true;
							}
							else
							{
								if (iImgSN != m_queSaveImgHistory.front().SN)
								{
									bNewImgSN = true;
								}
							}
							if (bNewImgSN)//**[7/14/2017 ConanSteve]**:  �µ�ͼ���ͼ��,��ӽ���ͼ��ʷ��
							{
								s_ImgInfoBeenSaved sImgInfo;
								vector<CString> vec_sImgPath;
								vec_sImgPath.push_back(lpszPathSaveImage[i]);
								sImgInfo.SN = iImgSN;
								sImgInfo.vec_sSaveImgPaths = vec_sImgPath;
								m_queSaveImgHistory.push(sImgInfo);
							}
							else/** �Ѿ����ڴ�ͼ��ŵ�·����              [6/12/2017 ConanSteve]**/
							{
								s_ImgInfoBeenSaved* pImgInfo = &m_queSaveImgHistory.front();
								pImgInfo->vec_sSaveImgPaths.push_back(lpszPathSaveImage[i]);
							}
							m_srwlSaveImgHistory.Unlock();
							/*****************************************************************************************/
						}
					}
					//**[7/14/2017 ConanSteve]**:  ִ�д�ͼ����

					LARGE_INTEGER timeBegin{ 0 }, timeEnd{ 0 }, timeFrequency{ 0 };
					QueryPerformanceCounter(&timeBegin);

					sIn.iReserve3 = (int)lpszPathSaveImage[0];
					CExtenInfo extenInfo;
					extenInfo.push_back(COptionalFunc(e_Fn_AlgModelInput_ImageSN).SetInt(iImgSN));
					extenInfo.push_back(COptionalFunc(e_Fn_AlgModelInput_ImageSavedCnt).SetInt(m_iSaveImageNum[iGroupSN]));
					extenInfo.push_back(COptionalFunc(e_Fn_AlgModelInput_SaveImagePath).SetPointer(lpszPathSaveImage));
					sIn.iReserve4 = (int)&extenInfo;
					pInsp->Set(sIn);//**[7/14/2017 ConanSteve]**:  �����㷨��ͼ
					QueryPerformanceCounter(&timeEnd);
					timeEnd.QuadPart -= timeBegin.QuadPart;
					CString sInfo{ _T("") };
					QueryPerformanceFrequency(&timeFrequency);
					double ldElapse = timeEnd.QuadPart*1000.0 / timeFrequency.QuadPart;
					sInfo.Format(_T("ProductID:%d\t CameraID:%d\t  Elapse%lf\n"), iImgSN, iGroupSN, ldElapse);
					m_fileDetect[iGroupSN][9].Write(sInfo);
				}

			}




			m_csAlgSave[iGroupSN].Lock();
			m_iGroupIsSaving[iGroupSN] = 0;
			m_csAlgSave[iGroupSN].Unlock();
		}


		PostMessage(NULL, DHOA_MSG_CHECK_FINISH, iGroupSN, iIdleObjIndex);

		/**********************************<�㷨�쳣�ж�>**************************************/
		{
			int iResult1 = 0;
			bool bHaveD = FALSE;
			int iFirstCameraSN = m_iGroupFirstCam[iGroupSN];
			int iOutCount = 0;
			bool bAlgException{ false };
			CString sCurInfo{ _T("") }, sAllInfo{ _T("") };
			iOutCount = m_iCheckOutNum[iFirstCameraSN];
			for (int i = 0; i < iOutCount; i++)
			{
				iResult1 = *((int*)((BYTE*)spCheckOutParam->pParam + (iOutCount - 1 - i)*m_iGroupCheckOutPerResultSize[iGroupSN]));
				sCurInfo.Format(_T("%2d"), iResult1);
				sAllInfo += sCurInfo;
				if (m_iAlgExceptionVal != 0 && iResult1 == m_iAlgExceptionVal)//**[8/1/2017 ConanSteve]**:  ����㷨�쳣����¼��־
					bAlgException = true;
				if (iResult1 > 0)
				{
					bHaveD = TRUE;
				}
			}
			if (bAlgException)
			{
				++m_arr_iAlgExceptionCnt[iGroupSN];//**[8/1/2017 ConanSteve]**:  �㷨�쳣������1
			}
			if (TRUE == m_bWriteLog)
			{
				if (TRUE == bHaveD)
				{
					m_iGroupErrorCount[iGroupSN]++;
				}

				//**[7/14/2017 ConanSteve]**:  �������Ϣ
				CString strInfo;
				strInfo.Format(_T("[������:%2d] [�߳�ID:%2d] [ͼ���:%8d] [����״̬����:%d] [�㷨���ʱ��:%4d] [�����������:%2d] [�������:%8d] [�����:%s]"),
					iGroupSN, iThreadID, iImgSN, spCheckStatusParam->iStatusType, iElapse1 + iElapse2,
					iIndex,
					m_iGroupErrorCount[iGroupSN], sAllInfo);
				m_fileCheck[iGroupSN][2].Write(strInfo);
				//**[8/1/2017 ConanSteve]**:  ��¼�㷨�쳣��־
				if (bAlgException)
				{
					strInfo.Format(_T("[������:%2d] [�߳�ID:%2d] [ͼ���:%8d] [����״̬����:%d] [�㷨�쳣����:%4d] [�����:%s]"), iGroupSN, iThreadID, iImgSN, spCheckStatusParam->iStatusType, m_arr_iAlgExceptionCnt[iGroupSN], sAllInfo);
					m_fileCheck[iGroupSN][7].Write(strInfo);
				}
			}
		}
		/************************************************************************/
		//**[7/31/2017 ConanSteve]**:  �ͷż�����
		{
			m_csAlgCheck[iGroupSN].Lock();
			m_nObjIndex[iGroupSN][iIdleObjIndex] = e_DetectStatus_NoChecking;//�����־λ��Ϊ0
			m_csAlgCheck[iGroupSN].Unlock();
		}
	}
	/************************************************************************/
	{
		if (spCheckStatusParam->iStatusType != 0)
		{
			// ֪ͨϵͳ �㷨�쳣
			TCHAR chInfo[c_iMaxStringLen]{ 0 };
			sprintf_s(chInfo, "Group:%d Image:%d StatusType:%d", iGroupSN, m_spCheckInParam[iGroupSN][iIndex].iImageSN,
				m_spCheckStatusParam[iGroupSN][iIndex].iStatusType);
			s_Status sStatus;
			sStatus.iCheckSN = iGroupSN;
			sStatus.iStatusType = STATUS_ALG_ABNORMAL;
			sStatus.pThis = m_pCallThis;
			sStatus.pParam = m_spCheckStatusParam[iGroupSN][iIndex].pParam;
			sStatus.pErrorInfo = GetErrorInfo(chInfo);
			m_StatusCallback(&sStatus);
		}
		/*************************************<>*************************************/
		if (e_ObjDetectMode_1 == m_iObjDetectMode)
		{
			(m_pResetWriteStatus[iGroupSN].get() + iIndex)->bCheckComplete = true;
			if (e_SaveImgObj_Alg == m_iSaveImgObj)
			{
				(m_pResetWriteStatus[iGroupSN].get() + iIndex)->bSaveComplete = true;
			}
			if ((m_pResetWriteStatus[iGroupSN].get() + iIndex)->bFnIsComplete())
			{
				m_csWriteDataStatus[iGroupSN].Lock();
				m_pWriteDataStatus[iGroupSN][iIndex] = e_WriteDataStatus_Leisure;
				m_pDetectStatus[iGroupSN][iIndex] = e_DetectStatus_NoChecking;
				m_csWriteDataStatus[iGroupSN].Unlock();
			}
		}
		/************************************************************************/
		/** Write logs              [5/5/2017 ConanSteve]**/
		sLogInfo.Format(_T("[������:%2d] [�����������:%2d] [ͼ���:%8d] [����д״̬:%s] [������״̬:%s]"),
			iGroupSN, iIndex, m_spCheckInParam[iGroupSN][iIndex].iImageSN,
			e_WriteDataStatus_Leisure == m_pWriteDataStatus[iGroupSN][iIndex] ? _T("����") : (e_WriteDataStatus_Busy == m_pWriteDataStatus[iGroupSN][iIndex] ? _T("дæ") : _T("д��")),
			e_DetectStatus_NoChecking == m_pDetectStatus[iGroupSN][iIndex] ? _T("δ���") : _T("�����"));
		m_fileCheck[iGroupSN][4].Write(sLogInfo);

	}
	ShowCurrStatus();
	//if(0==iGroupSN)
	UpdateRunInfo();
}

bool CDataClass::CopyCheckData(int iIndexInCache, int iIndexOfObj, int iGroupSN)
{
	memcpy_s(&m_ppCheckInParamEx[iGroupSN][iIndexOfObj], sizeof(m_iGroupCheckInSize[iGroupSN]), &m_pCheckInParam[iGroupSN][iIndexInCache], sizeof(m_iGroupCheckInSize[iGroupSN]));
	memcpy_s(&m_ppCheckOutParamEx[iGroupSN][iIndexOfObj], sizeof(m_iGroupCheckOutSize[iGroupSN]), &m_pCheckOutParam[iGroupSN][iIndexInCache], sizeof(m_iGroupCheckOutSize[iGroupSN]));
	memcpy_s(&m_ppCheckStatusParamEx[iGroupSN][iIndexOfObj], sizeof(m_iGroupCheckStatusSize[iGroupSN]), &m_pCheckStatusParam[iGroupSN][iIndexInCache], sizeof(m_iGroupCheckStatusSize[iGroupSN]));
	memcpy_s(&m_sppCheckInParamEx[iGroupSN][iIndexOfObj], sizeof(s_CheckInParam), &m_spCheckInParam[iGroupSN][iIndexInCache], sizeof(s_CheckInParam));
	memcpy_s(&m_sppCheckOutParamEx[iGroupSN][iIndexOfObj], sizeof(s_CheckOutParam), &m_spCheckOutParam[iGroupSN][iIndexInCache], sizeof(s_CheckOutParam));
	memcpy_s(&m_sppCheckStatusParamEx[iGroupSN][iIndexOfObj], sizeof(s_Status), &m_spCheckStatusParam[iGroupSN][iIndexInCache], sizeof(s_Status));
	return 0;
}

DWORD CDataClass::KickThreadFuncEx2()
{
	if (!m_bEnableOvertimeCallback)//**[9/20/2017 ConanSteve]**:  ������򿪳�ʱ�ص���ֱ�ӷ���
	{
		m_evtKillKickThread.SetEvent();
		return 0;
	}
	CString sInfo{ _T("") };
	int i = 0, j = 0;
	int iTime = 0, iCount = 0;
	int iOffsetY = 0;

	int* pAlgStatus{ nullptr };//**[7/19/2017 ConanSteve]**:  �㷨�����״̬
	bool bKick{ false };//**[7/19/2017 ConanSteve]**:  �Ƿ�Ҫ��
	int iCircle[MAX_KICK_COUNT]{ 0 };//**[9/20/2017 ConanSteve]**:  ÿ���߷Ͽ�
	CString sCurInfo{ _T("") }, sAllInfo{ _T("") };

	for (i = 0; i < MAX_KICK_COUNT; i++)
	{
		iCircle[i] = 1;
	}
	while (false == m_bKillThread)
	{
		for (i = 0; i < m_iKickQueueNum; i++)
		{
			for (; iCircle[i] <= m_iKickCircleSN[i]; iCircle[i]++)
			{
				iCount = 0;

				iOffsetY = m_iKickImageSN[i][iCircle[i] % 256];


				//**[7/17/2017 ConanSteve]**:   ����ʱ��׼��ʱ��
				if (e_TimeType_MilliSecond == m_iTimeType[i])
				{
					iTime = 0;

					m_spTime[i][iCircle[i] % 256].End();
					iTime = m_spTime[i][iCircle[i] % 256].GetMillisecondInt();
				}
				//**[7/19/2017 ConanSteve]**:  ��ʱ��׼��֡����
				else if (e_TimeType_Frame == m_iTimeType[i])
				{
					iTime = 0;
					iTime = (m_iKickImageSN[i][m_iKickCircleSN[i] % 256] - iOffsetY + 256) % 256;

				}
				//**[10/20/2017 ConanSteve]**:  ��ʱ���㷨δ���ؼ����
				bKick = false;
				if (iTime >= m_iTimeOut[i] || iTime < 0)
				{
					++m_iKickRemediation[i];//**[8/1/2017 ConanSteve]**:  ���߼�������					
					s_KickParam sKickParam;
					ZeroMemory(&sKickParam, sizeof(s_KickParam));
					sKickParam.pThis = m_pCallThis;
					sKickParam.iKickSN = i;
					sKickParam.iImageSN = iOffsetY;
					sKickParam.pParam = m_pKickResult[i][iOffsetY % 256];
					sKickParam.pResult = NULL;
					sAllInfo = _T("");
					for (int k = 0; k < m_iKickResultNum[i]; k++)
					{
						pAlgStatus = (int*)((BYTE*)sKickParam.pParam + k * m_iGroupCheckOutPerResultSize[0]);
						sCurInfo.Format(_T("%3d"), *pAlgStatus);

						if (k%m_iGroupCheckOutNum[0] == 0)
							sAllInfo += _T("  ");

						sAllInfo += sCurInfo;

						if (*pAlgStatus != 0)
						{
							bKick = true;
						}
					}

					if (true == bKick)
					{
						m_iKickErrorCount[i]++;
					}
					sInfo.Format(_T("[ͼ���:%8d]  [�޷ϼ���:%8d]  [�����:%s]"),
						iOffsetY, m_iKickErrorCount[i], sAllInfo);
					m_fileKick[i][0].Write(sInfo);

					//**[8/1/2017 ConanSteve]**:  д������־
					sInfo.Format(_T("[ͼ���:%8d]  [���߼���:%8d]  [�����:%s]"),
						iOffsetY, m_iKickRemediation[i], sAllInfo);
					m_fileKick[i][1].Write(sInfo);

					m_CheckCallback(&sKickParam);

					// ��ʱ״̬�ص�
					TCHAR chInfo[MAX_CHAR_LENGTH];
					ZeroMemory(chInfo, MAX_CHAR_LENGTH);
					s_Status sStatus;
					ZeroMemory(&sStatus, sizeof(sStatus));
					sStatus.iCheckSN = i;
					sStatus.iStatusType = STATUS_KICK_TIMEOUT;
					sStatus.pThis = m_pCallThis;
					sStatus.pParam = NULL;
					// #ifdef UNICODE
					// 					string strInfo = CStringConvert::UCS2_ANSI(sAllInfo.GetBuffer());
					// 					memcpy_s(chInfo, strInfo.size(), strInfo.c_str(), strInfo.size());
					// #else
					// 					sprintf(chInfo, sAllInfo.GetBuffer(0), sAllInfo.GetLength());
					// #endif
					_tprintf_s(chInfo, sAllInfo.GetBuffer(), sAllInfo.GetLength() * sizeof(TCHAR));
					sStatus.pErrorInfo = chInfo;
					m_StatusCallback(&sStatus);

					for (j = 0; j < m_iKickResultNum[i]; j++)
					{
						BYTE* pAddr = NULL;
						pAddr = m_pbKickWrite[i][iOffsetY % 256] + j;
						*pAddr = 0;
					}
				}
				else
				{
					// δ��ʱ��ѯ����Ƿ��Ѿ�ȫ��д��
					for (j = 0; j < m_iKickResultNum[i]; j++)
					{
						BYTE* pAddr = m_pbKickWrite[i][iOffsetY % 256] + j;
						if (pAddr != NULL && 1 == *pAddr)
						{
							iCount++;
						}
					}

					if (iCount == m_iKickResultNum[i])
					{
						s_KickParam sKickParam;
						ZeroMemory(&sKickParam, sizeof(s_KickParam));
						sKickParam.pThis = m_pCallThis;
						sKickParam.iKickSN = i;
						sKickParam.iImageSN = iOffsetY;
						sKickParam.pParam = m_pKickResult[i][iOffsetY % 256];
						sKickParam.pResult = NULL;

						sAllInfo = _T("");
						for (int k = 0; k < m_iKickResultNum[i]; k++)
						{
							pAlgStatus = (int*)((BYTE*)sKickParam.pParam + k * m_iGroupCheckOutPerResultSize[0]);
							sCurInfo.Format(_T("%3d"), *pAlgStatus);

							if (k%m_iGroupCheckOutNum[0] == 0)
								sAllInfo += "  ";

							sAllInfo += sCurInfo;

							if (*pAlgStatus != 0)
							{
								bKick = true;
							}
						}


						if (true == bKick)
						{
							m_iKickErrorCount[i]++;
						}
						sInfo.Format(_T("[ͼ���:%8d]  [�޷ϼ���:%8d]  [�����:%s]"),
							iOffsetY, m_iKickErrorCount[i], sAllInfo);
						m_fileKick[i][0].Write(sInfo);
						if (bKick)
						{
							m_fileKick[i][2].Write(sInfo);
						}
						m_CheckCallback(&sKickParam);
						for (j = 0; j < m_iKickResultNum[i]; j++)
						{
							BYTE* pAddr = NULL;
							pAddr = m_pbKickWrite[i][iOffsetY % 256] + j;
							*pAddr = 0;
						}
					}
					else
					{
						break;
					}
				}
			}
		}

		Sleep(1);
	}

	TRACE(_T("Kill Insp Thread!"));

	m_evtKillKickThread.SetEvent();

	return 0;
}

void CDataClass::UpdateRunInfo()
{
	CString sAllInfo{ _T("") }, sCurLine{ _T("") }, sTemp{ _T("") };
	/************************************************************************/
	//**[7/31/2017 ConanSteve]**:  �������״̬��ʹ�ü���
	for (int i = 0; i < m_iCheckGroupNum; i++)
	{
		int iTemp = 0;

		m_csWriteDataStatus[i].Lock();
		for (int j = 0; j < m_iGroupCheckQueueNum[i]; j++)
		{
			if (e_WriteDataStatus_Leisure == m_pWriteDataStatus[i][j])
			{
				iTemp++;
			}
		}
		m_csWriteDataStatus[i].Unlock();
		m_iGroupLeftList[i] = iTemp;
	}

	sCurLine.Format(_T("%100s"), tr("�������״̬��ʹ�ü���\r\n"));
	sAllInfo += sCurLine;
	for (int i = 0; i < m_iCheckGroupNum; ++i)
	{
		sCurLine.Format(_T("[GROUP:%02d] "), i + 1);
		sTemp.Format(_T("[%s:%2d] "), tr("ʣ��"), m_iGroupLeftList[i]);
		sCurLine += sTemp;
		m_csWriteDataStatus[i].Lock();
		for (int j = 0; j < m_iGroupCheckQueueNum[i]; ++j)
		{
			sTemp.Format(_T("[%2d:%s%8d] "), j + 1,
				e_WriteDataStatus_Leisure == m_pWriteDataStatus[i][j] ? _T("��") : (e_WriteDataStatus_Busy == m_pWriteDataStatus[i][j] ? _T("æ") : _T("��")),
				m_ppCacheUseCnt[i][j]);
			sCurLine += sTemp;
		}
		m_csWriteDataStatus[i].Unlock();
		sAllInfo += sCurLine + _T("\r\n");
	}
	/************************************************************************/
	//**[7/31/2017 ConanSteve]**:  ������״̬
	for (int i = 0; i < m_iCheckGroupNum; i++)
	{
		int iTemp = 0;
		for (int j = 0; j < m_pnObjectNumber[i]; j++)
		{
			if (0 == m_nObjIndex[i][j])
			{
				iTemp++;
			}
		}
		m_iGroupLeftObj[i] = iTemp;
	}
	sCurLine.Format(_T("%100s\r\n"), tr("������״̬��ʹ�ü���"));
	sAllInfo += sCurLine;
	for (int i = 0; i < m_iCheckGroupNum; ++i)
	{
		sCurLine.Format(_T("[Group:%02d] "), i + 1);
		sTemp.Format(_T("[%s:%2d] "), tr("ʣ��"), m_iGroupLeftObj[i]);
		sCurLine += sTemp;
		m_csAlgCheck[i].Lock();
		for (int j = 0; j < m_pnObjectNumber[i]; ++j)
		{
			sTemp.Format(_T("[%2d:%s%8d] "), j + 1, e_DetectStatus_Checking == m_nObjIndex[i][j] ? _T("æ") : _T("��"), m_ppObjUseCnt[i][j]);
			sCurLine += sTemp;
		}
		m_csAlgCheck[i].Unlock();
		sAllInfo += sCurLine + _T("\r\n");
	}
	/************************************************************************/
	//**[7/31/2017 ConanSteve]**:  ����鶪ͼ����
	sCurLine.Format(_T("%100s"), tr("ÿ������鶪ͼ����"));
	sAllInfo += sCurLine + _T("\r\n");
	sCurLine = _T("");
	for (int i = 0; i < m_iCheckGroupNum; ++i)
	{
		sTemp.Format(_T("[%2d:%8d] "), i + 1, m_iGroupAbnormalNum[i]);
		sCurLine += sTemp;
	}
	sAllInfo += sCurLine + _T("\r\n");
	/************************************************************************/
	//**[7/31/2017 ConanSteve]**:  �����ͼ����
	sCurLine.Format(_T("%100s"), tr("�����ͼ����"));
	sAllInfo += sCurLine + _T("\r\n");
	sCurLine = _T("");
	for (int i = 0; i < m_iCameraNum; i++)
	{
		sTemp.Format(_T("[%2d:%8d] "), i + 1, m_iCamCircle[i]);
		sCurLine += sTemp;
	}
	sAllInfo += sCurLine + _T("\r\n");
	/************************************************************************/
	//**[7/31/2017 ConanSteve]**:  ÿ�������
	sCurLine.Format(_T("%100s"), tr("ÿ�������"));
	sAllInfo += sCurLine + _T("\r\n");
	sCurLine = _T("");
	for (int i = 0; i < m_iCameraNum; i++)
	{
		sTemp.Format(_T("[%2d:%8d] "), i + 1, m_iGroupAlgCheck[i]);
		sCurLine += sTemp;
	}
	sAllInfo += sCurLine + _T("\r\n");
	/************************************************************************/
	//**[8/1/2017 ConanSteve]**:  �㷨�쳣����
	sCurLine.Format(_T("%100s"), tr("�㷨�쳣����"));
	sAllInfo += sCurLine + _T("\r\n");
	sCurLine = _T("");
	for (int i = 0; i < m_iCheckGroupNum; i++)
	{
		sTemp.Format(_T("[%2d:%8d] "), i + 1, m_arr_iAlgExceptionCnt[i]);
		sCurLine += sTemp;
	}
	sAllInfo += sCurLine + _T("\r\n");
	/************************************************************************/
	//**[7/31/2017 ConanSteve]**:  ���޼���
	sCurLine.Format(_T("%100s"), tr("��ʱ����"));
	sAllInfo += sCurLine + _T("\r\n");
	sCurLine = _T("");
	for (int i = 0; i < m_iKickQueueNum; i++)
	{
		sTemp.Format(_T("[%2d:%8d] "), i + 1, m_iKickRemediation[i]<0 ? 0 : m_iKickRemediation[i]);
		sCurLine += sTemp;
	}
	sAllInfo += sCurLine + _T("\r\n");
	/************************************************************************/



	if (m_bWriteLog)
	{
		m_csRunInfoStatus.Lock();
		m_fileRunInfoStatus.SeekToBegin();
		m_fileRunInfoStatus.Write(sAllInfo);
		m_csRunInfoStatus.Unlock();
	}
	if (m_bShowObjRunInfoWnd)
	{
		m_pDlgRunInfo->UpdateRunInfo(sAllInfo);
	}
}

TCHAR* CDataClass::GetErrorInfo(LPCTSTR lpszErrorInfo)
{
	m_csErrorInfoTable.Lock();
	if (NULL == lpszErrorInfo)
	{
		TCHAR* pTemp = &m_arrErrorInfo[(m_iErrorInfoIndex + c_iErrorInfoTableLen - 1) % c_iErrorInfoTableLen][0];
		m_csErrorInfoTable.Unlock();
		return pTemp;
	}
	char *tRet{ NULL };
	memset(&m_arrErrorInfo[m_iErrorInfoIndex][0], 0, c_iMaxStringLen);
	memcpy_s(&m_arrErrorInfo[m_iErrorInfoIndex][0], c_iMaxStringLen, lpszErrorInfo, strlen(lpszErrorInfo));
	tRet = &m_arrErrorInfo[m_iErrorInfoIndex][0];
	m_iErrorInfoIndex = (m_iErrorInfoIndex + 1) % c_iErrorInfoTableLen;
	m_csErrorInfoTable.Unlock();
	return tRet;
}

BOOL CDataClass::FlushLogFile()
{
	for (int i = 0; i < MAX_CAMERA_COUNT; ++i)
	{
		for (int j = 0; j < c_iMaxFileNum; ++j)
		{
			m_fileCam[i][j].Flush();
			m_fileDetect[i][j].Flush();
			m_fileCheck[i][j].Flush();
			m_fileKick[i][j].Flush();

		}
	}
	return TRUE;
}

void CDataClass::InitTestInfo()
{
	CXMLCfg xml(CExFn::GetCurrentAppPath() + _T("\\Config\\algConfig.xml"));
	xml.WriteInt(_T("MultiTest"), _T("MultiTest"), m_iCheckGroupNum);
}

bool CDataClass::IsCheckCompleted(int iGroupSN, int iKickSN, int iImgSN)
{
	CString sInfo{ _T("") };
	int iTime = 0, iCount = 0;

	int* pAlgStatus{ nullptr };//**[7/19/2017 ConanSteve]**:  �㷨�����״̬
	bool bKickError{ false };//**[7/19/2017 ConanSteve]**:  �Ƿ�Ҫ��
	int iCircle[MAX_KICK_COUNT]{ 0 };//**[9/20/2017 ConanSteve]**:  ÿ���߷Ͽ�
	CString sCurInfo{ _T("") }, sAllInfo{ _T("") };


	for (int j = 0; j < m_iKickResultNum[iKickSN]; j++)
	{
		BYTE* pAddr = m_pbKickWrite[iKickSN][iImgSN % 256] + j;
		if (pAddr != NULL && 1 == *pAddr)
		{
			iCount++;
		}
	}

	if (iCount == m_iKickResultNum[iKickSN])
	{
		s_KickParam sKickParam;
		ZeroMemory(&sKickParam, sizeof(s_KickParam));
		sKickParam.pThis = m_pCallThis;
		sKickParam.iKickSN = iKickSN;
		sKickParam.iImageSN = iImgSN;
		sKickParam.pParam = m_pKickResult[iKickSN][iImgSN % 256];
		sKickParam.pResult = NULL;

		sAllInfo = _T("");
		for (int k = 0; k < m_iKickResultNum[iKickSN]; k++)
		{
			pAlgStatus = (int*)((BYTE*)sKickParam.pParam + k * m_iGroupCheckOutPerResultSize[0]);
			sCurInfo.Format(_T("%3d"), *pAlgStatus);

			if (k%m_iGroupCheckOutNum[0] == 0)
				sAllInfo += _T("  ");

			sAllInfo += sCurInfo;

			if (*pAlgStatus != 0)
			{
				++m_iGroupBadCnt[iGroupSN];
				bKickError = true;
			}
			else
			{
				++m_iGroupGoodCnt[iGroupSN];
			}
		}

		if (true == bKickError)
		{
			m_iKickErrorCount[iKickSN]++;
		}
		else
		{
			++m_iKickGoodCnt[iKickSN];
		}

		sInfo.Format(_T("[ͼ���:%8d] [��Ʒ����:%8d] [�����:%s]"),
			iImgSN, m_iKickErrorCount[iKickSN], sAllInfo);
		m_fileKick[iKickSN][0].Write(sInfo);
		if (bKickError)
		{
			m_fileKick[iKickSN][2].Write(sInfo);
		}


		m_CheckCallback(&sKickParam);
		for (int j = 0; j < m_iKickResultNum[iKickSN]; j++)
		{
			BYTE* pAddr = NULL;
			pAddr = m_pbKickWrite[iKickSN][iImgSN % 256] + j;
			*pAddr = 0;
		}
	}
	return true;
}


bool CDataClass::ReseMultiObject()
{
	if (m_iCallType == e_CallType::e_CallType_Demo)
		return true;
	for (int i = 0; i < m_iCameraNum; ++i)
	{
		int iKickSN = m_iCamKick[i];
		int iCamIndexInKickQueue = m_iCamKickOffsetX_Written[i];
		int iKickOffsetY = m_iCamKickOffsetY[i];
		for (int j = 1; j <= iKickOffsetY; ++j)//**[9/24/2017 ConanSteve]**:  ͼ�����0�Ĳ����ڣ�����һѭ��֮��ͼ���Ϊ256ʱ��j == 0 ����
		{
			m_pbKickWrite[iKickSN][j][iCamIndexInKickQueue] = 1;
		}
	}

	for (int iGroupIndex = 0; iGroupIndex < m_iCheckGroupNum; ++iGroupIndex)
	{
		for (int i = 0; i < 256; ++i)
		{
			int iFirstCameraSN = m_iGroupFirstCam[iGroupIndex];

			// ���޷ϱ��еĺ���λ��
			int iKickX = m_iCamKickOffsetX[iFirstCameraSN];
			// ��ȡ�����޷϶��к�
			int iKickQueueSN = m_iCamKick[iFirstCameraSN];

			int *pDefaultStatus = (int*)(m_pKickResult[iKickQueueSN][i] + iKickX);
			*pDefaultStatus = m_iDefaultCheckRslt;
			// ��ȡ���޷϶��е�ƫ����(��ͼ������Ϊ��λ)��ȷ���޷ϱ������λ��
		}

	}
	return true;
}


bool CDataClass::Reset()
{
	return false;
}
