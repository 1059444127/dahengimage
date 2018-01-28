/************************************************************************/
/* Copyright (c) 2007, �й���㼯�ű���ͼ���Ӿ������ֹ�˾�Ӿ�ϵͳ��     
/* All rights reserved.													
/*																		
/* �ļ����ƣ� SimulateGrabber.cpp
/* ժҪ�� ͼ��ɼ���ģ����, ʹ�������ļ����Ѵ��ڵ�ͼ���ļ�ʵ��ģ�ⶨʱ��ͼ
/*
/* ��ǰ�汾�� 3.0
/* ���ߣ� ��׿��
/* ������ڣ� 2009��12��18��	// [2009-12-18 by jzj]: add
/*
/* ��ǰ�汾�� 2.0
/* ���ߣ� ��׿��
/* ������ڣ� 2007��11��13��
/************************************************************************/

#include "stdafx.h"
#include "SimulatedCamera.h"
#include "FileOperate.h"
#include "Image.h"
#include "SpendTime.h"// [2008-1-22 by jzj]

#include "SGParamDlg.h"

// ������öԻ���ʱ��884����
class tempRoutingFrame 
{
   CFrameWnd* m_pFrame;

public:

   tempRoutingFrame(CFrameWnd * pWnd= NULL)
   {
      // Save current value
      m_pFrame = AfxGetThreadState()->m_pRoutingFrame;
      // Set to value passed in. NULL by default.
      AfxGetThreadState()->m_pRoutingFrame = pWnd;
   }

   ~tempRoutingFrame()
   {
      // Restore m_pRoutingFrame to original value.
      AfxGetThreadState()->m_pRoutingFrame = m_pFrame;
   }
};

CSimulatedCamera::CSimulatedCamera()
{
	//////////////////////////////////////////////////////////////////////////
	// ������ʼ��

	m_LastErrorInfo.iErrorCode = 0;
	memset(m_LastErrorInfo.strErrorDescription, 0, GBMaxTextLen);
	memset(m_LastErrorInfo.strErrorRemark, 0, GBMaxTextLen);
	
	m_nCardSN = 0;	//ͼ�����
	m_CallBackFunc  = NULL;	//�ص�����ָ��
	m_Context = NULL;	//��ŵ��ó�ʼ�������Ķ����thisָ��

	m_pImageBuffer = NULL;	//�����洦���õ�ͼ�񻺳���ָ��
	m_nImageWidth = 0;	//ͼ���� (��λ:�ֽ�)
	m_nImageHeight = 0;	//ͼ��߶� (��λ:�ֽ�)
	m_nImageByteCount = 0;	//ͼ�����ش�С (��λ:�ֽ�)
	m_nImageBuffSize = 0;	//ͼ�񻺳�����С (��λ:�ֽ�)
	m_bPlaneRGB = FALSE;		//�Ƿ��ͨ��
	m_pPlaneRGBBuffer = NULL;	//�����洦���õķ�ͨ��ͼ�񻺳���

	m_bInited = FALSE;	//ͼ���Ƿ��Ѿ���ʼ��

	m_bGrab = FALSE;//�Ƿ�ɼ�ͼ��
	m_bGrabbing = FALSE;//�Ƿ����ڲɼ�ͼ��
	m_nGrabSpeed = 0;//�ɼ��ٶ�(��λ: ����/��)

	m_pBmpInfo = NULL;

	m_bKillServiceThread = FALSE;

	m_nCapturedFieldCount = 0;//�ɼ�֡����
	
	m_bPreReadImage = FALSE;//�Ƿ�Ԥ�ȶ�ͼ���ڴ���
	m_nImageNum = 0;//ͼ������
	for(int i = 0; i < SGMaxImageNum; i++)
	{
		m_PreReadImageBuffArray[i] = NULL;
	}
	
	m_bImagesOK = FALSE;//ͼ���Ƿ���׼����
	m_bUpdateImagesFolder = FALSE;//����ͼ�������ļ���
	
	m_nMaxImageWidth = 0;
	m_nMaxImageHeight = 0;
	m_nMaxImageByteCount = 0;

	m_bLoopGrab = FALSE;// �Ƿ�ѭ���ɼ�// [2008-10-23 by jzj]

	m_bResetPosition = FALSE;// �Ƿ�����Ԥ��λ��// [2008-12-4 by jzj]: add

	m_iGrabberTypeSN = 0;	// �ɼ������ͱ��// [2009-12-18 by jzj]: add
	//
	//////////////////////////////////////////////////////////////////////////
}

CSimulatedCamera::~CSimulatedCamera()
{
	m_FileFinder.Close();

	if(m_bPreReadImage == FALSE)
	{
		if(m_pImageBuffer != NULL)
		{
			delete []m_pImageBuffer;
			m_pImageBuffer = NULL;
		}
		
		if(m_pPlaneRGBBuffer != NULL)
		{
			delete []m_pPlaneRGBBuffer;
			m_pPlaneRGBBuffer = NULL;
		}
	}
	else
	{
		for(int i = 0; i < m_nImageNum; i++)
		{
			if(m_PreReadImageBuffArray[i] != NULL)
			{
				delete []m_PreReadImageBuffArray[i];
				m_PreReadImageBuffArray[i] = NULL;
			}
		}
	}	

	if(m_pBmpInfo != NULL)
	{
		delete m_pBmpInfo;
		m_pBmpInfo = NULL;
	}
}

//��ʼ��ͼ��
BOOL CSimulatedCamera::Init(const s_GBINITSTRUCT* pInitParam)
{
	s_SGINITCARDSTRUCT InitCardParam;
	InitCardParam.CardSN = pInitParam->iGrabberSN;
	memcpy(InitCardParam.sInitFile, pInitParam->strGrabberFile, MAX_PATH);
	InitCardParam.CallBackFunc = pInitParam->CallBackFunc;
	InitCardParam.Context = pInitParam->Context;

	m_iGrabberTypeSN = pInitParam->iGrabberTypeSN;	// �ɼ������ͱ��// [2009-12-18 by jzj]: add
	m_strDeviceName = pInitParam->strDeviceName;
	BOOL bRet = InitCard(&InitCardParam);

	return bRet;
}

bool CSimulatedCamera::Init(const CGrabInitInfo& rGrabInitInfo)
{
	s_SGINITCARDSTRUCT InitCardParam;
	//InitCardParam.CardSN = GetPrivateProfileInt(_T("��ͼ����"), _T("CardSN"), -1, rGrabInitInfo.lpszPathFileOfGrab);
	//**[9/1/2017 ConanSteve]**:  �ж�ģ��ͼƬ�ļ���·���Ƿ����
	{
		CString sPathFileOfImgs{ _T("") };
		CExFn::GetPrivateProfileString(_T("��ͼ����"), _T("ͼ���ļ������ļ���"), sPathFileOfImgs, rGrabInitInfo.lpszPathFileOfGrab);
		if (FALSE == PathFileExists(sPathFileOfImgs))
		{
			return false;
		}
	}
	InitCardParam.CardSN = rGrabInitInfo.iGrabSN;
	InitCardParam.CallBackFunc = rGrabInitInfo.CallBackFunc;
	InitCardParam.Context = rGrabInitInfo.Context;
	memcpy_s(InitCardParam.sInitFile, sizeof(TCHAR)*GBMaxFileNameLen, rGrabInitInfo.lpszPathFileOfGrab, sizeof(TCHAR)*GBMaxFileNameLen);
	return TRUE== InitCard(&InitCardParam);
}

//�ر�ͼ��
BOOL CSimulatedCamera::Close()
{
	BOOL bRet = CloseCard();

	return bRet;
}

//��ʼ�ɼ�
BOOL CSimulatedCamera::StartGrab()
{
	if(m_bInited == FALSE)
	{
		m_LastErrorInfo.iErrorCode = SGErrorCodeNoInit;
		_stprintf_s(m_LastErrorInfo.strErrorDescription, _T("����δ��ʼ�������ܽ��иò�����"));
		_stprintf_s(m_LastErrorInfo.strErrorRemark, _T("λ�ã�StartGrab()"));
		return FALSE;
	}
	
	if(m_bPreReadImage == TRUE)// [2008-12-4 by jzj]: add
	{
		m_bResetPosition = TRUE;// [2008-12-4 by jzj]: add
	}
	else
	{
		m_bUpdateImagesFolder = TRUE;// [2008-10-23 by jzj]Ϊ��ʵ����������
	}

//	Sleep(1000);
	m_bGrab = TRUE;//�Ƿ�ɼ�ͼ��

	return TRUE;
}

//ֹͣ�ɼ�
BOOL CSimulatedCamera::StopGrab()
{
	if(m_bInited == FALSE)
	{
		m_LastErrorInfo.iErrorCode = SGErrorCodeNoInit;
		_stprintf_s(m_LastErrorInfo.strErrorDescription, _T("����δ��ʼ�������ܽ��иò�����"));
		_stprintf_s(m_LastErrorInfo.strErrorRemark, _T("λ�ã�StopGrab()"));
		return FALSE;
	}

	m_bGrab = FALSE;//�Ƿ�ɼ�ͼ��
	
	return TRUE;
}

//��֡�ɼ�
BOOL CSimulatedCamera::Snapshot()
{
	return TRUE;// ȱ��δʵ��
}

//�õ�����
BOOL CSimulatedCamera::GetParamInt(GBParamID Param, int &nOutputVal)
{
	if(m_bInited == FALSE)
	{
		m_LastErrorInfo.iErrorCode = SGErrorCodeNoInit;
		_stprintf_s(m_LastErrorInfo.strErrorDescription, _T("����δ��ʼ�������ܽ��иò�����"));
		_stprintf_s(m_LastErrorInfo.strErrorRemark, _T("λ�ã�GetParamInt(GBParamID Param, int &nOutputVal)"));
		return FALSE;
	}

	switch(Param)
	{
	case GBImageWidth:
		nOutputVal = m_nImageWidth;
		break;
	case GBImageHeight:
		nOutputVal = m_nImageHeight;
		break;
	case GBImagePixelSize:
		nOutputVal = m_nImageByteCount;
		break;
	case GBImageBufferSize:
		nOutputVal = m_nImageBuffSize;
		break;
	case GBImageBufferAddr:
		{
			if(m_bPlaneRGB)
			{
				nOutputVal = (int)m_pPlaneRGBBuffer;
			}
			else
			{
				nOutputVal = (int)m_pImageBuffer;
			}
		}
		break;	
	case GBIsColorFilter:
	{
		nOutputVal = 3 == m_nImageByteCount ? 1 : 0;
		break;
	}
	case GBGrabberTypeSN:		// �ɼ������ͱ��	// [2009-12-18 by jzj]: add
		nOutputVal = m_iGrabberTypeSN;
		break;
	default:
		m_LastErrorInfo.iErrorCode = SGErrorCodeParamIll;
		_stprintf_s(m_LastErrorInfo.strErrorDescription, _T("�������Ϸ���"));
		_stprintf_s(m_LastErrorInfo.strErrorRemark, _T("λ�ã�GetParamInt(GBParamID Param, int &nOutputVal)"));
		return FALSE;
	}

	return TRUE;
}

//���ò���
BOOL CSimulatedCamera::SetParamInt(GBParamID Param, int nInputVal)
{
	if(m_bInited == FALSE)
	{
		m_LastErrorInfo.iErrorCode = SGErrorCodeNoInit;
		_stprintf_s(m_LastErrorInfo.strErrorDescription, _T("����δ��ʼ�������ܽ��иò�����"));
		_stprintf_s(m_LastErrorInfo.strErrorRemark, _T("λ�ã�SetParamInt(GBParamID Param, int nInputVal)"));
		return FALSE;
	}

	//ASSERT(FALSE);

	m_LastErrorInfo.iErrorCode = SGErrorCodeThisFuncDisable;
	_stprintf_s(m_LastErrorInfo.strErrorDescription, _T("�ò�����Ч��"));
	_stprintf_s(m_LastErrorInfo.strErrorRemark, _T("λ�ã�SetParamInt(GBParamID Param, int nInputVal)"));
	return FALSE;
}

//���ò����Ի���
void CSimulatedCamera::CallParamDialog()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	
#ifdef _DEBUG
	tempRoutingFrame rframe;// Workaround for ASSERT in WINCORE.CPP 884 (CWnd::AssertValid)
#endif

	CSGParamDlg dlg;
	dlg.SetOwner(this);
	dlg.m_nGrabSpeed = m_nGrabSpeed;
	dlg.m_bIsPlaneRGB = m_bPlaneRGB;
	dlg.m_strImagesPath = m_strImageFileFolder;
	
	dlg.m_nImageWidth = m_nImageWidth;
	dlg.m_nImageHeight = m_nImageHeight;
	dlg.m_nChannelNum = m_nImageByteCount;
	
	dlg.DoModal();
}

//�õ�������Ϣ
void CSimulatedCamera::GetLastErrorInfo(s_GBERRORINFO *pErrorInfo)
{
	ASSERT(pErrorInfo != NULL);
	
	pErrorInfo->iErrorCode = m_LastErrorInfo.iErrorCode;
	_stprintf_s(pErrorInfo->strErrorDescription, m_LastErrorInfo.strErrorDescription);
	_stprintf_s(pErrorInfo->strErrorRemark, m_LastErrorInfo.strErrorRemark);
}

//////////////////////////////////////////////////////////////////////////
//ģ��ɼ���ר�ýӿ�

// 
BOOL CSimulatedCamera::InitCard(const s_SGINITCARDSTRUCT* pInitCardParam)
{	
	
	ASSERT(pInitCardParam != NULL);
	ASSERT(pInitCardParam->CallBackFunc != NULL);
	ASSERT(pInitCardParam->Context != NULL);

	m_iniCfg.Init(pInitCardParam->sInitFile);

	m_iPicNumOfEachGrab = m_iniCfg.GetIntEx(_T("Grabber Parameters"), _T("SmallImageNumberPerGrabber"), -1,_T("���ÿ�βɼ��ɼ���ͼ������"));//**[12/9/2017 ConanSteve]**:  
	m_iTriggerPort = m_iniCfg.GetIntEx(_T("Grabber Parameters"), _T("TriggerPort"), -1, _T("ģ��������ܲ�ͼ�Ĵ����ź�����Ӧ��ģ��ӿڿ���Out��"));
	if(pInitCardParam == NULL || pInitCardParam->CallBackFunc == NULL || pInitCardParam->Context == NULL)
	{
		m_LastErrorInfo.iErrorCode = SGErrorCodeParamIll;
		_stprintf_s(m_LastErrorInfo.strErrorDescription, _T("�������Ϸ���"));
		_stprintf_s(m_LastErrorInfo.strErrorRemark, _T("λ�ã�InitCard()"));
		return FALSE;
	}
	
	if(m_bInited == TRUE)
	{
		CloseCard();
		m_bInited = FALSE;
	}
	
	//////////////////////////////////////////////////////////////////////////
	//
	
	m_nCardSN = 0;	//ͼ�����
	m_CallBackFunc  = NULL;	//�ص�����ָ��
	m_Context = NULL;	//��ŵ��ó�ʼ�������Ķ����thisָ��

	m_pImageBuffer = NULL;	//�����洦���õ�ͼ�񻺳���ָ��
	m_nImageWidth = 0;	//ͼ���� (��λ:�ֽ�)
	m_nImageHeight = 0;	//ͼ��߶� (��λ:�ֽ�)
	m_nImageByteCount = 0;	//ͼ�����ش�С (��λ:�ֽ�)
	m_nImageBuffSize = 0;	//ͼ�񻺳�����С (��λ:�ֽ�)
	m_bPlaneRGB = FALSE;		//�Ƿ��ͨ��
	m_pPlaneRGBBuffer = NULL;	//�����洦���õķ�ͨ��ͼ�񻺳���

	m_bInited = FALSE;	//ͼ���Ƿ��Ѿ���ʼ��

	m_bGrab = FALSE;//�Ƿ�ɼ�ͼ��
	m_bGrabbing = FALSE;//�Ƿ����ڲɼ�ͼ��
	m_nGrabSpeed = 0;//�ɼ��ٶ�(��λ: ����/��)
	
	//����λͼ��Ϣ�ռ�
	m_pBmpInfo = (PBITMAPINFO)new BYTE[sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)];

	m_bKillServiceThread = FALSE;

	m_nCapturedFieldCount = 0;//�ɼ�֡����
	
	m_bPreReadImage = FALSE;//�Ƿ�Ԥ�ȶ�ͼ���ڴ���
	m_nImageNum = 0;//ͼ������
	for(int i = 0; i < SGMaxImageNum; i++)
	{
		m_PreReadImageBuffArray[i] = NULL;
	}
	
	m_bImagesOK = FALSE;//ͼ���Ƿ���׼����
	m_bUpdateImagesFolder = FALSE;//����ͼ�������ļ���
	
	m_nMaxImageWidth = 0;
	m_nMaxImageHeight = 0;
	m_nMaxImageByteCount = 0;

	m_bLoopGrab = FALSE;// �Ƿ�ѭ���ɼ�// [2008-10-23 by jzj]

	m_bResetPosition = FALSE;// �Ƿ�����Ԥ��λ��// [2008-12-4 by jzj]: add
	//
	//////////////////////////////////////////////////////////////////////////
	
	m_nCardSN = pInitCardParam->CardSN;
	memcpy_s(m_sInitFile, GBMaxFileNameLen * sizeof(TCHAR), pInitCardParam->sInitFile, GBMaxFileNameLen*sizeof(TCHAR));
	if(!IsFileExist(m_sInitFile))
	{
		m_LastErrorInfo.iErrorCode = SGErrorCodeInitFileNoExist;
		_stprintf_s(m_LastErrorInfo.strErrorDescription, _T("��ʼ���ļ������ڣ�"));
		_stprintf_s(m_LastErrorInfo.strErrorRemark, _T("λ�ã�InitCard()"));
		return FALSE;
	}
	/************************************************************************/
	/*                       [5/22/2017 ConanSteve]                         */
	if (g_bRecord)
	{
		CString sFileName{ _T("") };
		sFileName.Format(_T("%sSingleTime.txt"), m_sInitFile);
		m_fileRecordSingleTime.Open(sFileName, CFile::modeCreate | CFile::modeWrite);
		sFileName.Format(_T("%sIntervalTime.txt"), m_sInitFile);
		m_fileRecordIntervalTime.Open(sFileName, CFile::modeCreate | CFile::modeWrite);
	}
	LARGE_INTEGER large{ 0 };
	QueryPerformanceFrequency(&large);
	m_dFrequency = (double)large.QuadPart;
	/************************************************************************/
	
	//�������ļ��п�����
	GetPrivateProfileString(_T("��ͼ����"), _T("ͼ���ļ������ļ���"), _T("NULL"), m_strImageFileFolder.GetBuffer(256), 256, m_sInitFile);
	m_strImageFileFolder.ReleaseBuffer();
	m_nImageWidth = GetPrivateProfileInt(_T("��ͼ����"), _T("ͼ����"), -1, m_sInitFile);
	m_nImageHeight = GetPrivateProfileInt(_T("��ͼ����"), _T("ͼ��߶�"), -1, m_sInitFile);
	m_nImageByteCount = GetPrivateProfileInt(_T("��ͼ����"), _T("ÿ�����ֽ���"), -1, m_sInitFile);	
	m_bPlaneRGB = GetPrivateProfileInt(_T("��ͼ����"), _T("�Ƿ��ͨ��"), -1, m_sInitFile);
	m_nGrabSpeed = GetPrivateProfileInt(_T("��ͼ����"), _T("�ɼ��ٶ�"), -1, m_sInitFile);
	m_bPreReadImage = GetPrivateProfileInt(_T("��ͼ����"), _T("�Ƿ���Ԥ��ģʽ"), -1, m_sInitFile);
	m_nImageNum = GetPrivateProfileInt(_T("��ͼ����"), _T("ͼ������"), -1, m_sInitFile);
	m_bLoopGrab = GetPrivateProfileInt(_T("��ͼ����"), _T("�Ƿ�ѭ���ɼ�"), -1, m_sInitFile);// [2008-10-23 by jzj]

	if(!IsPathExist(m_strImageFileFolder))
	{
		m_LastErrorInfo.iErrorCode = SGErrorCodeInitFileParamIll;
		_stprintf_s(m_LastErrorInfo.strErrorDescription, _T("�����ļ���������ͼ���ļ������ļ��� �����ڣ�"));
		_stprintf_s(m_LastErrorInfo.strErrorRemark, _T("λ�ã�InitCard()"));
		return FALSE;
	}
	if(m_nImageWidth < 1 || m_nImageHeight < 1)
	{
		m_LastErrorInfo.iErrorCode = SGErrorCodeInitFileParamIll;
		_stprintf_s(m_LastErrorInfo.strErrorDescription, _T("�����ļ���������ͼ���� ���Ϸ���������ֵ��1 -- n��"));
		_stprintf_s(m_LastErrorInfo.strErrorRemark, _T("λ�ã�InitCard()"));
		return FALSE;
	}
	if(m_nImageByteCount < 1 || m_nImageByteCount > 4)
	{
		m_LastErrorInfo.iErrorCode = SGErrorCodeInitFileParamIll;
		_stprintf_s(m_LastErrorInfo.strErrorDescription, _T("�����ļ���������ÿ�����ֽ��� ���Ϸ���������ֵ��1 -- 4��"));
		_stprintf_s(m_LastErrorInfo.strErrorRemark, _T("λ�ã�InitCard()"));
		return FALSE;
	}
	if(m_bPlaneRGB < 0 || m_bPlaneRGB > 1)
	{
		m_LastErrorInfo.iErrorCode = SGErrorCodeInitFileParamIll;
		_stprintf_s(m_LastErrorInfo.strErrorDescription, _T("�����ļ����������Ƿ��ͨ�� ���Ϸ���������ֵ��0 -- 1��"));
		_stprintf_s(m_LastErrorInfo.strErrorRemark, _T("λ�ã�InitCard()"));
		return FALSE;
	}
	if(m_nGrabSpeed < 1 || m_nGrabSpeed > 60000)
	{
		m_LastErrorInfo.iErrorCode = SGErrorCodeInitFileParamIll;
		_stprintf_s(m_LastErrorInfo.strErrorDescription, _T("�����ļ��������󣺲ɼ��ٶ� ���Ϸ���������ֵ��1-- 60000��"));
		_stprintf_s(m_LastErrorInfo.strErrorRemark, _T("λ�ã�InitCard()"));
		return FALSE;
	}
	if(m_bPreReadImage < 0 || m_bPreReadImage > 1)
	{
		m_LastErrorInfo.iErrorCode = SGErrorCodeInitFileParamIll;
		_stprintf_s(m_LastErrorInfo.strErrorDescription, "�����ļ����������Ƿ���Ԥ��ģʽ ���Ϸ���������ֵ��0 -- 1��");
		_stprintf_s(m_LastErrorInfo.strErrorRemark, "λ�ã�InitCard()");
		return FALSE;
	}
	if(m_bPreReadImage == TRUE && (m_nImageNum < 1 || m_nImageNum > SGMaxImageNum))
	{
		m_LastErrorInfo.iErrorCode = SGErrorCodeInitFileParamIll;
		_stprintf_s(m_LastErrorInfo.strErrorDescription, _T("�����ļ���������ͼ������ ���Ϸ���������ֵ��1 -- %d��"), SGMaxImageNum);
		_stprintf_s(m_LastErrorInfo.strErrorRemark, _T("λ�ã�InitCard()"));
		return FALSE;
	}
	if(m_bPlaneRGB == TRUE && m_nImageByteCount != 3)
	{
		m_LastErrorInfo.iErrorCode = SGErrorCodeInitFileParamIll;
		_stprintf_s(m_LastErrorInfo.strErrorDescription, _T("�����ļ��������󣺷�ͨ��ʱÿ�����ֽ�������Ϊ3��"));
		_stprintf_s(m_LastErrorInfo.strErrorRemark, _T("λ�ã�InitCard()"));
		return FALSE;
	}
	if(m_bLoopGrab < 0 || m_bLoopGrab > 1)// [2008-10-23 by jzj]
	{
		m_LastErrorInfo.iErrorCode = SGErrorCodeInitFileParamIll;
		_stprintf_s(m_LastErrorInfo.strErrorDescription, _T("�����ļ����������Ƿ�ѭ���ɼ� ���Ϸ���������ֵ��0 -- 1��"));
		_stprintf_s(m_LastErrorInfo.strErrorRemark, _T("λ�ã�InitCard()"));
		return FALSE;
	}
	
	BOOL bRet = FALSE;
	CString strWildcard; 
	strWildcard.Format(_T("%s\\*.*"), m_strImageFileFolder); 
	bRet = m_FileFinder.FindFile(strWildcard);
	if(!bRet)
	{
		m_LastErrorInfo.iErrorCode = SGErrorCodeInitFileParamIll;
		_stprintf_s(m_LastErrorInfo.strErrorDescription, _T("�����ļ���������%s��û���ļ���"), m_strImageFileFolder);
		_stprintf_s(m_LastErrorInfo.strErrorRemark, _T("λ�ã�InitCard()"));
		return FALSE;
	}
	
	m_nMaxImageWidth = m_nImageWidth;
	m_nMaxImageHeight = m_nImageHeight;
	m_nMaxImageByteCount = m_nImageByteCount;
	
	m_CallBackFunc = pInitCardParam->CallBackFunc;
	m_Context = pInitCardParam->Context;
	
	m_nImageBuffSize = m_nImageWidth * m_nImageHeight * m_nImageByteCount;	

	if(m_bPreReadImage == FALSE)//��Ԥ��ģʽ
	{
		m_pImageBuffer = new BYTE[m_nImageBuffSize];
		if(m_bPlaneRGB)
		{
			m_pPlaneRGBBuffer = new BYTE[m_nImageBuffSize];
		}
	}
	else//Ԥ��ͼ��ģʽ
	{
		static BYTE *pTempConvertBuff = NULL;

		for(int i = 0; i < m_nImageNum; i++)
		{
			m_PreReadImageBuffArray[i] = new BYTE[m_nImageBuffSize];
			
			//////////////////////////////////////////////////////////////////////////
			//��ͼ���ڴ�

			CString strFilePath;
			BOOL bLastFile = FALSE;
			
			bLastFile = !m_FileFinder.FindNextFile();
			strFilePath = m_FileFinder.GetFilePath();
			
			BOOL bRet = OpenBMPFile(strFilePath, m_pBmpInfo, m_PreReadImageBuffArray[i]);
			if(bRet == FALSE)
			{
				m_LastErrorInfo.iErrorCode = SGErrorCodeReadBMPFileFail;
				_stprintf_s(m_LastErrorInfo.strErrorDescription, "��ȡͼ��%sʧ��!", strFilePath);
				_stprintf_s(m_LastErrorInfo.strErrorRemark, "λ�ã�InitCard()");
				
				return FALSE;
			}
			else
			{
				if(m_pBmpInfo->bmiHeader.biWidth != m_nImageWidth
					|| m_pBmpInfo->bmiHeader.biHeight != m_nImageHeight
					|| m_pBmpInfo->bmiHeader.biBitCount != m_nImageByteCount * 8)
				{
					m_LastErrorInfo.iErrorCode = SGErrorCodeInitFileParamIll;
					_stprintf_s(m_LastErrorInfo.strErrorDescription, "%s�ļ���������ѡͼ������", m_sInitFile);
					_stprintf_s(m_LastErrorInfo.strErrorRemark, "λ�ã�InitCard()");
					
					return FALSE;
				}
			}
			
			if(bLastFile)
			{
				m_FileFinder.FindFile(strWildcard);
			}
			//
			//////////////////////////////////////////////////////////////////////////
			
			if(m_bPlaneRGB)
			{
				if(pTempConvertBuff == NULL)
				{
					pTempConvertBuff = new BYTE[m_nImageBuffSize];
				}
				
				CImage myImage;
				myImage.ConvertRGBToPlaneRGB(pTempConvertBuff, m_PreReadImageBuffArray[i], m_nImageWidth, m_nImageHeight);

				BYTE *pTempChangeBuff = NULL;
				pTempChangeBuff = m_PreReadImageBuffArray[i];
				m_PreReadImageBuffArray[i] = pTempConvertBuff;
				pTempConvertBuff = pTempChangeBuff;
			}
		}
		
		if(pTempConvertBuff != NULL)
		{
			delete []pTempConvertBuff;
			pTempConvertBuff = NULL;
		}		
	}	
	
	m_nCapturedFieldCount = 0;//�ɼ�֡����
	m_bInited = TRUE;
	
	//�����ɼ������߳�
	HANDLE hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServiceThread, this, 0, &m_dwThreadId);
	::SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);

	return TRUE;
}



//�ر�ͼ��
BOOL CSimulatedCamera::CloseCard()
{	
	/************************************************************************/
	/*                       [5/22/2017 ConanSteve]                         */
	if (g_bRecord)
	{
		m_fileRecordSingleTime.Close();
		m_fileRecordIntervalTime.Close();
	}
	/************************************************************************/
	if(m_bInited == FALSE)
	{
		return TRUE;
	}	

	m_bGrab = FALSE;//�Ƿ�ɼ�ͼ��
	m_bKillServiceThread = TRUE;	
	//m_evtReset.SetEvent();// [2008-1-22 by jzj]

	TRACE("CSimulateCard_Kill\n");
	WaitForSingleObject(m_ServiceThreadDeadEvent, INFINITE);
	TRACE("CSimulateCard_Dead\n");

	if(m_bPreReadImage == FALSE)
	{
		if(m_pImageBuffer != NULL)
		{
			delete []m_pImageBuffer;
			m_pImageBuffer = NULL;
		}
		
		if(m_pPlaneRGBBuffer != NULL)
		{
			delete []m_pPlaneRGBBuffer;
			m_pPlaneRGBBuffer = NULL;
		}
	}
	else
	{
		for(int i = 0; i < m_nImageNum; i++)
		{
			if(m_PreReadImageBuffArray[i] != NULL)
			{
				delete []m_PreReadImageBuffArray[i];
				m_PreReadImageBuffArray[i] = NULL;
			}
		}
	}

	if(m_pBmpInfo != NULL)
	{
		delete m_pBmpInfo;
		m_pBmpInfo = NULL;
	}

	m_bInited = FALSE;
	return TRUE;
}

//�ɼ������߳�
DWORD CSimulatedCamera::ServiceThread(LPVOID lpParam)
{
	CSimulatedCamera* This = (CSimulatedCamera*)lpParam;
	
	DWORD dwRet = 0;
	if(This->m_bPreReadImage == TRUE)
	{
		This->PreReadImageServiceThreadFunc();
	}
	else
	{
		This->ServiceThreadFunc();
	}

	return dwRet;
}

//�ɼ������̺߳���
DWORD CSimulatedCamera::ServiceThreadFunc()
{
	BOOL bRet = FALSE;
	CString strWildcard; 
	strWildcard.Format(_T("%s\\*.*"), m_strImageFileFolder); 

	bRet = m_FileFinder.FindFile(strWildcard);
	if(!bRet)
	{
		m_LastErrorInfo.iErrorCode = SGErrorCodeInitFileParamIll;
		_stprintf_s(m_LastErrorInfo.strErrorDescription, _T("%s��û���ļ���"), m_strImageFileFolder);
		_stprintf_s(m_LastErrorInfo.strErrorRemark, _T("λ�ã�ServiceThreadFunc()"));
		
		s_GBSIGNALINFO SignalInfo;
		SignalInfo.iGrabberTypeSN = m_iGrabberTypeSN;// [2009-12-18 by jzj]: add
		SignalInfo.Context = m_Context;
		SignalInfo.iGrabberSN = m_nCardSN;
		SignalInfo.iErrorCode = m_LastErrorInfo.iErrorCode;
		SignalInfo.iFrameCount = 0;
		_stprintf_s(SignalInfo.strDescription, m_LastErrorInfo.strErrorDescription);
		m_CallBackFunc(&SignalInfo);//���ûص������Դ�����д���

		m_bImagesOK = FALSE;
	}
	else
	{
		m_bImagesOK = TRUE;
	}
	if(-1==m_iPicNumOfEachGrab)//**[12/9/2017 ConanSteve]**:  �ɵĲ�ͼģʽ
	{
		CSpendTime mySpendTime;// [2008-1-22 by jzj]
		mySpendTime.Start();// [2008-1-22 by jzj]

		CString strNewPath;
		BOOL btem = FALSE;
		m_FileFinder.FindNextFile();
		strNewPath = m_FileFinder.GetFileName();
		//m_bKillServiceThread = TRUE;
		while (!m_bKillServiceThread)
		{
			/************************************************************************/
			/*                       [5/17/2017 ConanSteve]                         */
	// 		HANDLE hEventActive{ 0 }, hEventComplete{ 0 };
	// 		CString sFilePath{ _T("") };
	// 		sFilePath.Format(_T("%s"), m_sInitFile);
	// 		int pos = sFilePath.ReverseFind(_T('\\'));
	// 		sFilePath = sFilePath.Right(sFilePath.GetLength()-1-pos);
	// 		while (1)
	// 		{
	// 			hEventActive = OpenEvent(EVENT_ALL_ACCESS, FALSE, sFilePath);
	// 			if (NULL != hEventActive)
	// 			{
	// 				DWORD dwRet = WaitForSingleObject(hEventActive, 10);
	// 				if (WAIT_OBJECT_0 == dwRet)
	// 					break;
	// 				if (m_bKillServiceThread)
	// 					break;				
	// 			}
	// 			else
	// 			{
	// 				Sleep(10);
	// 				break;
	// 			}
	// 		}
			/************************************************************************/
			if (m_bUpdateImagesFolder)
			{
				m_bUpdateImagesFolder = FALSE;

				// 			strWildcard.Format("%s\\*.bmp", m_strImageFileFolder); 
				strWildcard.Format(_T("%s\\%s\\*.bmp"), m_strImageFileFolder, strNewPath);

				bRet = m_BMPFileFinder.FindFile(strWildcard);
				if (!bRet)
				{
					m_LastErrorInfo.iErrorCode = SGErrorCodeReadBMPFileFail;
					_stprintf_s(m_LastErrorInfo.strErrorDescription, _T("%s��û��BMP�ļ���"), m_strImageFileFolder);
					_stprintf_s(m_LastErrorInfo.strErrorRemark, _T("λ�ã�ServiceThreadFunc()"));

					s_GBSIGNALINFO SignalInfo;
					SignalInfo.iGrabberTypeSN = m_iGrabberTypeSN;// [2009-12-18 by jzj]: add
					SignalInfo.Context = m_Context;
					SignalInfo.iGrabberSN = m_nCardSN;
					SignalInfo.iErrorCode = m_LastErrorInfo.iErrorCode;
					SignalInfo.iFrameCount = 0;
					_stprintf(SignalInfo.strDescription, m_LastErrorInfo.strErrorDescription);
					m_CallBackFunc(&SignalInfo);//���ûص������Դ�����д���

					m_bImagesOK = FALSE;
				}
				else
				{
					m_bImagesOK = TRUE;
				}
			}
			if (m_bImagesOK == FALSE)
			{
				Sleep(10);
				continue;
			}
			/************************************************************************/
			/*                       [5/24/2017 ConanSteve]                         */
	// 		hEventComplete = OpenEvent(EVENT_ALL_ACCESS, TRUE, sFilePath + _T("Complete"));
	// 		if (NULL != hEventComplete)
	// 		{
	// 			SetEvent(hEventComplete);
	// 			CloseHandle(hEventComplete);
	// 		}
	// 		CloseHandle(hEventActive);
	// 		hEventActive = NULL;
			/************************************************************************/


			if (m_bGrab)
			{
				static int iLostTime = 0;// here!!!

				mySpendTime.End();// [2008-1-22 by jzj]
				if (mySpendTime.GetMillisecondInt() >= m_nGrabSpeed - iLostTime)// �������һ֡ͼ // [2008-1-22 by jzj]

				{
					m_bGrabbing = TRUE;

					CString strFilePath;
					BOOL bLastFile = FALSE;
					/************************************************************************/
					/*                       [5/22/2017 ConanSteve]                         */
					LARGE_INTEGER curCnt{ 0 };
					DWORD dwInterval{ 0 }, dwMillisecond{ 0 };
					CString sWriteInfo{ _T("") };

					if (g_bRecord)
					{
						QueryPerformanceCounter(&curCnt);
						dwMillisecond = DWORD(curCnt.QuadPart * 1000 / m_dFrequency);
						dwInterval = DWORD(dwMillisecond - m_dwLastStartCnt);
						m_dwLastStartCnt = dwMillisecond;
						sWriteInfo.Format(_T("%d"), dwInterval);
						m_fileRecordIntervalTime.WriteString(sWriteInfo);
					}
					/************************************************************************/
					while (1)
					{
						bLastFile = !m_BMPFileFinder.FindNextFile();
						strFilePath = m_BMPFileFinder.GetFilePath();

						BOOL bRet = OpenBMPFile(strFilePath, m_pBmpInfo, m_pImageBuffer);
						if (bRet == FALSE)
						{
							m_LastErrorInfo.iErrorCode = SGErrorCodeReadBMPFileFail;
							_stprintf(m_LastErrorInfo.strErrorDescription, _T("��ȡͼ��%sʧ��!"), strFilePath);
							_stprintf(m_LastErrorInfo.strErrorRemark, _T("λ�ã�ServiceThreadFunc()"));

							s_GBSIGNALINFO SignalInfo;
							SignalInfo.iGrabberTypeSN = m_iGrabberTypeSN;// [2009-12-18 by jzj]: add
							SignalInfo.Context = m_Context;
							SignalInfo.iGrabberSN = m_nCardSN;
							SignalInfo.iErrorCode = m_LastErrorInfo.iErrorCode;
							SignalInfo.iFrameCount = 0;
							_stprintf(SignalInfo.strDescription, m_LastErrorInfo.strErrorDescription);
							m_CallBackFunc(&SignalInfo);//���ûص������Դ�����д���
						}
						else
						{
							if (m_pBmpInfo->bmiHeader.biWidth != m_nImageWidth
								|| m_pBmpInfo->bmiHeader.biHeight != m_nImageHeight
								|| m_pBmpInfo->bmiHeader.biBitCount != m_nImageByteCount * 8)
							{
								m_LastErrorInfo.iErrorCode = SGErrorCodeInitFileParamIll;
								_stprintf(m_LastErrorInfo.strErrorDescription, _T("%s�ļ���������ѡͼ������"), m_sInitFile);
								_stprintf(m_LastErrorInfo.strErrorRemark, _T("λ�ã�ServiceThreadFunc()"));

								s_GBSIGNALINFO SignalInfo;
								SignalInfo.iGrabberTypeSN = m_iGrabberTypeSN;// [2009-12-18 by jzj]: add
								SignalInfo.Context = m_Context;
								SignalInfo.iGrabberSN = m_nCardSN;
								SignalInfo.iErrorCode = m_LastErrorInfo.iErrorCode;
								SignalInfo.iFrameCount = 0;
								_stprintf(SignalInfo.strDescription, m_LastErrorInfo.strErrorDescription);
								m_CallBackFunc(&SignalInfo);//���ûص������Դ�����д���
							}
							else
							{
								m_nCapturedFieldCount++;

								if (m_bPlaneRGB)
								{
									CImage myImage;
									myImage.ConvertRGBToPlaneRGB(m_pPlaneRGBBuffer, m_pImageBuffer, m_nImageWidth, m_nImageHeight);
								}

								s_GBSIGNALINFO SignalInfo;
								SignalInfo.iGrabberTypeSN = m_iGrabberTypeSN;// [2009-12-18 by jzj]: add
								SignalInfo.Context = m_Context;
								SignalInfo.iGrabberSN = m_nCardSN;
								SignalInfo.iErrorCode = 0;
								SignalInfo.iFrameCount = m_nCapturedFieldCount;
								_stprintf(SignalInfo.strDescription, _T("%s"), m_BMPFileFinder.GetFileName().Left(GBMaxFileNameLen));
								m_CallBackFunc(&SignalInfo);//���ûص�������ͼ����д���
							}
						}

						m_bGrabbing = FALSE;

						if (bLastFile)
						{
							BOOL brettemp = FALSE;
							brettemp = !m_FileFinder.FindNextFile();
							strNewPath = m_FileFinder.GetFileName();

							if (brettemp)
							{
								strWildcard.Format(_T("%s\\Needle*.*"), m_strImageFileFolder);
								m_FileFinder.FindFile(strWildcard);
							}

							m_bUpdateImagesFolder = TRUE;

							if (m_bLoopGrab == FALSE)// [2008-10-24 by jzj]
							{
								m_bGrab = FALSE;
							}
							break;
						}
					}
					/************************************************************************/
					/*                       [5/22/2017 ConanSteve]                         */
					if (g_bRecord)
					{
						QueryPerformanceCounter(&curCnt);
						dwMillisecond = DWORD(curCnt.QuadPart * 1000 / m_dFrequency);
						dwInterval = DWORD(dwMillisecond - m_dwLastStartCnt);
						sWriteInfo.Format(_T("%d"), dwInterval);
						m_fileRecordSingleTime.WriteString(sWriteInfo);
					}
					/************************************************************************/
					mySpendTime.Start();// [2008-1-22 by jzj]
				}// if(mySpendTime.GetMillisecondInt() >= m_nGrabSpeed)// �������һ֡ͼ // [2008-1-22 by jzj]
			}
			//WaitForSingleObject(m_evtReset, iWaitTime);// [2008-1-22 by jzj]
			Sleep(1);// [2008-1-22 by jzj]
		}
	}
	else if (-1 != m_iPicNumOfEachGrab)//**[12/17/2017 ConanSteve]**:  �ⴥ��ģʽ
	{
		CString sEventName{ _T("") };
		sEventName.Format(_T("{E3B325CF-34F9-488F-B4E3-8A32D1C43D68}%02d"), m_iTriggerPort);
		HANDLE hTriggerEvent = CreateEvent(NULL, TRUE, FALSE, sEventName);
		CString sPathFile{ _T("") };
		vector<CString> vecCSPathOfFileOrFolder;//**[12/9/2017 ConanSteve]**:  �洢�ɼ��ļ��������һ��bmp�ļ������ļ���·��
		BOOL  bRet = TRUE;
		if (1 == m_iPicNumOfEachGrab)//**[12/9/2017 ConanSteve]**:  �����1,ȫ��bmp�ļ�
		{
			CFileFind fileFind;
			CString sFilter{ _T("") };
			sFilter.Format(_T("%s\\*.bmp"), m_strImageFileFolder);
			bRet = fileFind.FindFile(sFilter);
			while (bRet)
			{
				bRet = fileFind.FindNextFile();
				sPathFile = fileFind.GetFilePath();
				if (!fileFind.IsDirectory()&&!fileFind.IsDots())
				{
					vecCSPathOfFileOrFolder.push_back(sPathFile);
				}
			}
		}
		else if (1 < m_iPicNumOfEachGrab)//**[12/9/2017 ConanSteve]**:  ����ͼ�����ļ���
		{
			CFileFind fileFind;
			CString sFilter{ _T("") };
			sFilter.Format(_T("%s\\*.*"), m_strImageFileFolder);
			bRet = fileFind.FindFile(sFilter);
			while (bRet)
			{
				bRet = fileFind.FindNextFile();
				sPathFile = fileFind.GetFilePath();
				if (fileFind.IsDirectory()&&!fileFind.IsDots())
				{
					vecCSPathOfFileOrFolder.push_back(sPathFile);
				}
			}
		}

		int iIndexOfCircleReadImg = 0;//**[12/12/2017 ConanSteve]**:  ѭ����ȡͼ���λ������
		while (!m_bKillServiceThread)
		{
			DWORD dwRet = WaitForSingleObject(hTriggerEvent, 10);
			switch (dwRet)
			{
			case 0:
			{
				if (m_bGrab)//**[12/9/2017 ConanSteve]**:  ������˲ɼ�
				{
					if (1 == m_iPicNumOfEachGrab)//**[12/12/2017 ConanSteve]**:  �����������ͼ
					{
						CString sFilePathOfMBP = vecCSPathOfFileOrFolder[(iIndexOfCircleReadImg++) % vecCSPathOfFileOrFolder.size()];
						BOOL bRet = OpenBMPFile(sFilePathOfMBP, m_pBmpInfo, m_pImageBuffer);
						if (bRet == FALSE)
						{
							m_LastErrorInfo.iErrorCode = SGErrorCodeReadBMPFileFail;
							_stprintf(m_LastErrorInfo.strErrorDescription, _T("��ȡͼ��%sʧ��!"), sFilePathOfMBP);
							_stprintf(m_LastErrorInfo.strErrorRemark, _T("λ�ã�ServiceThreadFunc()"));

							s_GBSIGNALINFO SignalInfo;
							SignalInfo.iGrabberTypeSN = m_iGrabberTypeSN;// [2009-12-18 by jzj]: add
							SignalInfo.Context = m_Context;
							SignalInfo.iGrabberSN = m_nCardSN;
							SignalInfo.iErrorCode = m_LastErrorInfo.iErrorCode;
							SignalInfo.iFrameCount = 0;
							_stprintf(SignalInfo.strDescription, m_LastErrorInfo.strErrorDescription);
							m_CallBackFunc(&SignalInfo);//���ûص������Դ�����д���
						}
						else
						{
							if (m_pBmpInfo->bmiHeader.biWidth != m_nImageWidth
								|| m_pBmpInfo->bmiHeader.biHeight != m_nImageHeight
								|| m_pBmpInfo->bmiHeader.biBitCount != m_nImageByteCount * 8)
							{
								m_LastErrorInfo.iErrorCode = SGErrorCodeInitFileParamIll;
								_stprintf(m_LastErrorInfo.strErrorDescription, _T("%s�ļ���������ѡͼ������"), m_sInitFile);
								_stprintf(m_LastErrorInfo.strErrorRemark, _T("λ�ã�ServiceThreadFunc()"));

								s_GBSIGNALINFO SignalInfo;
								SignalInfo.iGrabberTypeSN = m_iGrabberTypeSN;// [2009-12-18 by jzj]: add
								SignalInfo.Context = m_Context;
								SignalInfo.iGrabberSN = m_nCardSN;
								SignalInfo.iErrorCode = m_LastErrorInfo.iErrorCode;
								SignalInfo.iFrameCount = 0;
								_stprintf(SignalInfo.strDescription, m_LastErrorInfo.strErrorDescription);
								m_CallBackFunc(&SignalInfo);//���ûص������Դ�����д���
							}
							else
							{
								m_nCapturedFieldCount++;

								if (m_bPlaneRGB)
								{
									CImage myImage;
									myImage.ConvertRGBToPlaneRGB(m_pPlaneRGBBuffer, m_pImageBuffer, m_nImageWidth, m_nImageHeight);
								}

								s_GBSIGNALINFO SignalInfo;
								SignalInfo.iGrabberTypeSN = m_iGrabberTypeSN;// [2009-12-18 by jzj]: add
								SignalInfo.Context = m_Context;
								SignalInfo.iGrabberSN = m_nCardSN;
								SignalInfo.iErrorCode = 0;
								SignalInfo.iFrameCount = m_nCapturedFieldCount;
								_stprintf(SignalInfo.strDescription, _T("%s"), sFilePathOfMBP.GetBuffer());
								m_CallBackFunc(&SignalInfo);//���ûص�������ͼ����д���
							}
						}
					}
					else if (1 < m_iPicNumOfEachGrab)
					{
						CString sFilter{_T("")};
						sFilter.Format(_T("%s\\*.bmp"), vecCSPathOfFileOrFolder[(iIndexOfCircleReadImg++) % vecCSPathOfFileOrFolder.size()]);
						CFileFind fileFind;
						bool bRet = fileFind.FindFile(sFilter);
						int iCurNum = 0;//**[12/12/2017 ConanSteve]**:  ��ǰ�ļ����Զ�ͼ������
						while (bRet&& iCurNum<m_iPicNumOfEachGrab)//**[12/12/2017 ConanSteve]**:  һֱ��ֱ���ļ������ļ�������߶��������ﵽ�趨��ֵ
						{
							bRet = fileFind.FindNextFile();
							CString sFilePathOfMBP = fileFind.GetFilePath();
							bRet = OpenBMPFile(sFilePathOfMBP, m_pBmpInfo, m_pImageBuffer);
							if (bRet == FALSE)
							{
								m_LastErrorInfo.iErrorCode = SGErrorCodeReadBMPFileFail;
								_stprintf(m_LastErrorInfo.strErrorDescription, _T("��ȡͼ��%sʧ��!"), sFilePathOfMBP);
								_stprintf(m_LastErrorInfo.strErrorRemark, _T("λ�ã�ServiceThreadFunc()"));

								s_GBSIGNALINFO SignalInfo;
								SignalInfo.iGrabberTypeSN = m_iGrabberTypeSN;// [2009-12-18 by jzj]: add
								SignalInfo.Context = m_Context;
								SignalInfo.iGrabberSN = m_nCardSN;
								SignalInfo.iErrorCode = m_LastErrorInfo.iErrorCode;
								SignalInfo.iFrameCount = 0;
								_stprintf(SignalInfo.strDescription, m_LastErrorInfo.strErrorDescription);
								m_CallBackFunc(&SignalInfo);//���ûص������Դ�����д���
							}
							else
							{
								if (m_pBmpInfo->bmiHeader.biWidth != m_nImageWidth
									|| m_pBmpInfo->bmiHeader.biHeight != m_nImageHeight
									|| m_pBmpInfo->bmiHeader.biBitCount != m_nImageByteCount * 8)
								{
									m_LastErrorInfo.iErrorCode = SGErrorCodeInitFileParamIll;
									_stprintf(m_LastErrorInfo.strErrorDescription, _T("%s�ļ���������ѡͼ������"), m_sInitFile);
									_stprintf(m_LastErrorInfo.strErrorRemark, _T("λ�ã�ServiceThreadFunc()"));

									s_GBSIGNALINFO SignalInfo;
									SignalInfo.iGrabberTypeSN = m_iGrabberTypeSN;// [2009-12-18 by jzj]: add
									SignalInfo.Context = m_Context;
									SignalInfo.iGrabberSN = m_nCardSN;
									SignalInfo.iErrorCode = m_LastErrorInfo.iErrorCode;
									SignalInfo.iFrameCount = 0;
									_stprintf(SignalInfo.strDescription, m_LastErrorInfo.strErrorDescription);
									m_CallBackFunc(&SignalInfo);//���ûص������Դ�����д���
								}
								else
								{
									m_nCapturedFieldCount++;

									if (m_bPlaneRGB)
									{
										CImage myImage;
										myImage.ConvertRGBToPlaneRGB(m_pPlaneRGBBuffer, m_pImageBuffer, m_nImageWidth, m_nImageHeight);
									}

									s_GBSIGNALINFO SignalInfo;
									SignalInfo.iGrabberTypeSN = m_iGrabberTypeSN;// [2009-12-18 by jzj]: add
									SignalInfo.Context = m_Context;
									SignalInfo.iGrabberSN = m_nCardSN;
									SignalInfo.iErrorCode = 0;
									SignalInfo.iFrameCount = m_nCapturedFieldCount;
									_stprintf(SignalInfo.strDescription, _T("%s"), sFilePathOfMBP.GetBuffer());
									m_CallBackFunc(&SignalInfo);//���ûص�������ͼ����д���
								}
							}
							iCurNum++;
						}

					}
				}
			}
			case WAIT_TIMEOUT:
				break;
			default:
				break;
			}
			//Sleep(1);
		}
		CloseHandle(hTriggerEvent);
		
	}
	m_ServiceThreadDeadEvent.SetEvent();
	return 0;
}

//Ԥ��ͼ��Ĳɼ������̺߳���
DWORD CSimulatedCamera::PreReadImageServiceThreadFunc()
{	
	CSpendTime mySpendTime;// [2008-1-22 by jzj]
	mySpendTime.Start();// [2008-1-22 by jzj]
	
	int nPosition = 0;// [2008-12-4 by jzj]: add

	while(!m_bKillServiceThread)
	{	
		if(m_bResetPosition == TRUE)// [2008-12-4 by jzj]: add
		{
			m_bResetPosition = FALSE;	
			nPosition = 0;
		}

		if(m_bGrab)
		{
			mySpendTime.End();// [2008-1-22 by jzj]
			if(mySpendTime.GetMillisecondInt() >= m_nGrabSpeed)// �������һ֡ͼ // [2008-1-22 by jzj]
			{
				//int nPosition = 0;// [2008-12-4 by jzj]: delete
				
				m_bGrabbing = TRUE;
				
				//nPosition = m_nCapturedFieldCount % m_nImageNum;// [2008-12-4 by jzj]: delete
				m_pImageBuffer = m_PreReadImageBuffArray[nPosition];
				
				if(m_pImageBuffer == NULL)
				{
					m_LastErrorInfo.iErrorCode = SGErrorCodeReadImageFromMemFail;
					_stprintf(m_LastErrorInfo.strErrorDescription, _T("���ڴ��ж�ȡͼ��ʧ��!"));
					_stprintf(m_LastErrorInfo.strErrorRemark, _T("λ�ã�PreReadImageServiceThreadFunc()"));
					
					s_GBSIGNALINFO SignalInfo;
					SignalInfo.iGrabberTypeSN = m_iGrabberTypeSN;// [2009-12-18 by jzj]: add
					SignalInfo.Context = m_Context;
					SignalInfo.iGrabberSN = m_nCardSN;
					SignalInfo.iErrorCode = m_LastErrorInfo.iErrorCode;
					SignalInfo.iFrameCount = 0;
					_stprintf(SignalInfo.strDescription, m_LastErrorInfo.strErrorDescription);
					m_CallBackFunc(&SignalInfo);//���ûص������Դ�����д���
				}
				else
				{
					m_nCapturedFieldCount++;				
					
					if(m_bPlaneRGB)
					{
						m_pPlaneRGBBuffer = m_pImageBuffer;
					}
					
					s_GBSIGNALINFO SignalInfo;
					SignalInfo.iGrabberTypeSN = m_iGrabberTypeSN;// [2009-12-18 by jzj]: add
					SignalInfo.Context = m_Context;
					SignalInfo.iGrabberSN = m_nCardSN;
					SignalInfo.iErrorCode = 0;
					SignalInfo.iFrameCount = m_nCapturedFieldCount;
					_stprintf(SignalInfo.strDescription, _T("%s"), m_strImageFileNameArray[nPosition].Left(GBMaxFileNameLen));
					m_CallBackFunc(&SignalInfo);//���ûص�������ͼ����д���
				}	
				
				m_bGrabbing = FALSE;

				// [2008-12-4 by jzj]: add				
				nPosition++;
				if(nPosition >= m_nImageNum)
				{
					nPosition = 0;
					
					if(m_bLoopGrab == FALSE)
					{
						m_bGrab = FALSE;
					}
				}
				
				mySpendTime.Start();// [2008-1-22 by jzj]
			}// if(mySpendTime.GetMillisecondInt() >= m_nGrabSpeed)// �������һ֡ͼ // [2008-1-22 by jzj]
		}

		//WaitForSingleObject(m_evtReset, m_nGrabSpeed);// [2008-1-22 by jzj]
		Sleep(1);// [2008-1-22 by jzj]
	}

	m_ServiceThreadDeadEvent.SetEvent();
	return 0;
}

//�õ�����
//BOOL CSimulateGrabber::GetParamInt(SGPARAMID Param, PINT32 nOutputVal)
BOOL CSimulatedCamera::GetParamInt(SGPARAMID Param, int& nOutputVal)// [2009-12-17 by jzj]: modify
{
	if(m_bInited == FALSE)
	{
		m_LastErrorInfo.iErrorCode = SGErrorCodeNoInit;
		_stprintf(m_LastErrorInfo.strErrorDescription, _T("����δ��ʼ�������ܽ��иò�����"));
		_stprintf(m_LastErrorInfo.strErrorRemark, _T("λ�ã�GetParamInt(SGPARAMID Param, PINT32 nOutputVal)"));
		return FALSE;
	}

	switch(Param)
	{
	case SG_IMAGE_WIDTH:
		//*nOutputVal = m_nImageWidth;// [2009-12-17 by jzj]: modify
		nOutputVal = m_nImageWidth;
		break;
	case SG_IMAGE_HEIGHT:
		//*nOutputVal = m_nImageHeight;// [2009-12-17 by jzj]: modify
		nOutputVal = m_nImageHeight;
		break;
	case SG_IMAGE_BYTE_COUNT:
		//*nOutputVal = m_nImageByteCount;// [2009-12-17 by jzj]: modify
		nOutputVal = m_nImageByteCount;
		break;
	case SG_IMAGE_BUFFER_SIZE:
		//*nOutputVal = m_nImageBuffSize;// [2009-12-17 by jzj]: modify
		nOutputVal = m_nImageBuffSize;
		break;
	case SG_IMAGE_BUFFER_ADDR:
		{
			if(m_bPlaneRGB)
			{
				//*nOutputVal = (int)m_pPlaneRGBBuffer;// [2009-12-17 by jzj]: modify
				nOutputVal = (int)m_pPlaneRGBBuffer;
			}
			else
			{
				//*nOutputVal = (int)m_pImageBuffer;// [2009-12-17 by jzj]: modify
				nOutputVal = (int)m_pImageBuffer;
			}
		}
		break;
	case SG_IS_PLANERGB:
		//*nOutputVal = m_bPlaneRGB;// [2009-12-17 by jzj]: modify
		nOutputVal = m_bPlaneRGB;
		break;
	case SG_GRAB_SPEED:
		//*nOutputVal = m_nGrabSpeed;// [2009-12-17 by jzj]: modify
		nOutputVal = m_nGrabSpeed;
		break;
	default:
		m_LastErrorInfo.iErrorCode = SGErrorCodeParamIll;
		_stprintf(m_LastErrorInfo.strErrorDescription, _T("�������Ϸ���"));
		_stprintf(m_LastErrorInfo.strErrorRemark, _T("λ�ã�GetParamInt(SGPARAMID Param, PINT32 nOutputVal)"));
		return FALSE;
	}

	return TRUE;
}

//���ò���
BOOL CSimulatedCamera::SetParamInt(SGPARAMID Param, int nInputVal)
{
	if(m_bInited == FALSE)
	{
		m_LastErrorInfo.iErrorCode = SGErrorCodeNoInit;
		_stprintf(m_LastErrorInfo.strErrorDescription, _T("����δ��ʼ�������ܽ��иò�����"));
		_stprintf(m_LastErrorInfo.strErrorRemark, _T("λ�ã�SetParamInt(SGPARAMID Param, int nInputVal)"));
		return FALSE;
	}
	
	switch(Param)
	{
	case SG_IS_PLANERGB://�ģ�����У��
		{
			BOOL bPlaneRGB = nInputVal;
			if(bPlaneRGB)
			{
				if(m_nImageByteCount != 3)
				{
					return FALSE;
				}

				if(m_pPlaneRGBBuffer == NULL)
				{
					m_pPlaneRGBBuffer = new BYTE[m_nImageBuffSize];
				}
			}				
			m_bPlaneRGB = bPlaneRGB;
		}
		break;
	case SG_GRAB_SPEED://�ģ�����У��
		m_nGrabSpeed = nInputVal;
		//m_evtReset.SetEvent();// [2008-1-22 by jzj]
		break;
	case SG_IMAGE_WIDTH:
		if(nInputVal > m_nMaxImageWidth)
		{
			m_LastErrorInfo.iErrorCode = SGErrorCodeParamIll;
			_stprintf(m_LastErrorInfo.strErrorDescription, _T("�������Ϸ������ӦС�ڵ���%d"), m_nImageWidth);
			_stprintf(m_LastErrorInfo.strErrorRemark, _T("λ�ã�SetParamInt(SGPARAMID Param, int nInputVal)"));
			return FALSE;
		}

		m_nImageWidth = nInputVal;
		break;
	case SG_IMAGE_HEIGHT:
		if(nInputVal > m_nMaxImageHeight)
		{
			m_LastErrorInfo.iErrorCode = SGErrorCodeParamIll;
			_stprintf(m_LastErrorInfo.strErrorDescription, _T("�������Ϸ����߶�ӦС�ڵ���%d"), m_nImageHeight);
			_stprintf(m_LastErrorInfo.strErrorRemark, _T("λ�ã�SetParamInt(SGPARAMID Param, int nInputVal)"));
			return FALSE;
		}

		m_nImageHeight = nInputVal;
		break;
	case SG_IMAGE_BYTE_COUNT:
		if(nInputVal > m_nMaxImageByteCount)
		{
			m_LastErrorInfo.iErrorCode = SGErrorCodeParamIll;
			_stprintf(m_LastErrorInfo.strErrorDescription, _T("�������Ϸ���ÿ�����ֽ���ӦС�ڵ���%d"), m_nImageByteCount);
			_stprintf(m_LastErrorInfo.strErrorRemark, _T("λ�ã�SetParamInt(SGPARAMID Param, int nInputVal)"));
			return FALSE;
		}

		m_nImageByteCount = nInputVal;
		break;
	default:
		m_LastErrorInfo.iErrorCode = SGErrorCodeParamIll;
		_stprintf(m_LastErrorInfo.strErrorDescription, _T("�������Ϸ���"));
		_stprintf(m_LastErrorInfo.strErrorRemark, _T("λ�ã�SetParamInt(SGPARAMID Param, int nInputVal)"));
		return FALSE;
	}
	
	return TRUE;
}
//
//////////////////////////////////////////////////////////////////////////