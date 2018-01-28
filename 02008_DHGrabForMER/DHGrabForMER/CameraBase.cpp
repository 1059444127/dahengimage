//--------------------------------------------------------------- 
/** 
\file      CameraBase.cpp
\brief     CameraBase function
\version   v1.0.13035.9301 
\date      2013-5-30 
*/ 
//--------------------------------------------------------------- 

#include "stdafx.h"
#include "CameraBase.h"
#include "ExFn.h"


CameraBase::CameraBase()
{
	m_nImageWidth = 0;
	m_nImageHeight = 0;
	m_nWndWidth = 0;
	m_nWndHeight = 0;
	m_nPayLoadSize = 0;
	m_pShowWnd = NULL;
	m_pDC = NULL;
	m_pLogWnd = NULL;
	m_hDevice = NULL;
	m_chBmpBuf = new char[2048];
	m_bIsOpen = false;
	m_bIsSnaping = false;
	m_bShowImg = true;
	m_bSaveBMP = true;
	m_bSaveAVI = false;
	m_bEnableGetImageProcess = false;
	m_pBmpInfo = NULL;
	m_pImgRGBBuffer = NULL;
	m_pImgRaw8Buffer = NULL;
	m_strBMPFolder = _T("C:\\test");
	m_strAVIFolder = _T("");
	m_pAVIFile = NULL;
	m_bCompressed = false;
	m_nTimeFrame = 0;
	m_nTotalNum = 0;
	m_bInitSuccess = false;
	m_nImageByteCount = 1;
	m_nFrameCount = 0;

	m_nWidth = 0;
	m_nHeight = 0;
	m_nOffsetX = 0;
	m_nOffsetY = 0;
	m_nExposureMode = 0;
	m_dExposureTime = 0;
	m_bExposureAuto = false;
	m_nGain = 0;
	m_bGainAuto = false;
	m_nBlacklevel = 0;
	m_bBlacklevelAuto = false;
	m_dBalanceRatio = 0;
	m_bBalanceRatioAuto = false;
	m_bTriggerMode = false;
	m_nTriggerActivation = 0;
	m_pParamSetDlg = NULL;
}

CameraBase::~CameraBase()
{
	if (m_chBmpBuf != NULL)
	{
		delete []m_chBmpBuf;
		m_chBmpBuf = NULL;
	}
}

// ��ʼ��
BOOL CameraBase::Init(const s_GBINITSTRUCT* pInitParam)
{
	//AfxMessageBox("asd");
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	emStatus= GXInitLib();//��ʼ�����ÿ�------------

	BOOL bRet = false;

	s_GBINITSTRUCT InitCardParam;
	InitCardParam.iGrabberSN = pInitParam->iGrabberSN;
	//��char�ĺ�����д�뵽ָ�����ַ�����
// 	_tprintf_s(InitCardParam.strGrabberFile, pInitParam->strGrabberFile);//��ȡ��������ļ�λ��
// 
// 	_tprintf_s(InitCardParam.strDeviceName, pInitParam->strDeviceName);//��ȡ�豸������
// 	_tprintf_s(InitCardParam.strDeviceMark, pInitParam->strDeviceMark);//��ȡSN
	memcpy_s(InitCardParam.strGrabberFile, GBMaxFileNameLen, pInitParam->strGrabberFile, GBMaxFileNameLen);
	memcpy_s(InitCardParam.strDeviceName, GBMaxFileNameLen, pInitParam->strDeviceName, GBMaxFileNameLen);
	memcpy_s(InitCardParam.strDeviceMark, GBMaxFileNameLen, pInitParam->strDeviceMark, GBMaxFileNameLen);
	
	InitCardParam.CallBackFunc = pInitParam->CallBackFunc;		//���ûص�����
	InitCardParam.Context = pInitParam->Context;				//���ص������Ĵ���ָ�봫��
	InitCardParam.EventCallbackFunc = pInitParam->EventCallbackFunc;	//����������Ӧ�Ļص�����ָ�봫��
	InitCardParam.iGrabberTypeSN = pInitParam->iGrabberTypeSN ;// �ɼ������ͱ�� [10/19/2010 SJC]

	
	emStatus = Open(&InitCardParam);//��������ã����ҳ�ʼ��

	if (emStatus == GX_STATUS_SUCCESS)
	{
		bRet = true;
	}	
	return bRet;
}

bool CameraBase::Init(const CGrabInitInfo& rGrabInitInfo)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	emStatus = GXInitLib();//��ʼ�����ÿ�------------

	bool bRet = false;

	s_GBINITSTRUCT InitCardParam;
	InitCardParam.iGrabberSN = rGrabInitInfo.iGrabSN;
	//��char�ĺ�����д�뵽ָ�����ַ�����
	//_tprintf_s(InitCardParam.strGrabberFile, rGrabInitInfo.lpszPathFileOfGrab);//��ȡ��������ļ�λ��
	memcpy_s(InitCardParam.strGrabberFile, GBMaxFileNameLen, rGrabInitInfo.lpszPathFileOfGrab, GBMaxFileNameLen);
	GetPrivateProfileString(_T("Grabber Parameters"), _T("GrabberName"), _T(""), InitCardParam.strDeviceName, GBMaxTextLen, rGrabInitInfo.lpszPathFileOfGrab);
	GetPrivateProfileString(_T("Grabber Parameters"), _T("GrabberMark"), _T(""), InitCardParam.strDeviceMark, GBMaxTextLen, rGrabInitInfo.lpszPathFileOfGrab);
	InitCardParam.CallBackFunc = rGrabInitInfo.CallBackFunc;		//���ûص�����
	InitCardParam.Context = rGrabInitInfo.Context;				//���ص������Ĵ���ָ�봫��
	InitCardParam.EventCallbackFunc = rGrabInitInfo.EventCallbackFunc;	//����������Ӧ�Ļص�����ָ�봫��
	InitCardParam.iGrabberTypeSN = GetPrivateProfileInt(_T("Grabber Parameters"), _T("GrabberTypeSN"), -1, rGrabInitInfo.lpszPathFileOfGrab);// �ɼ������ͱ�� [10/19/2010 SJC]


	emStatus = Open(&InitCardParam);//��������ã����ҳ�ʼ��

	if (emStatus == GX_STATUS_SUCCESS)
	{
		bRet = true;
	}
	return bRet;
}

//��ʼ�ɼ�
BOOL CameraBase::StartGrab()
{
	StartCallbackAcq();
	return TRUE;
} 

//ֹͣ�ɼ�
BOOL CameraBase::StopGrab()
{
	StopCallbackAcq();
	return TRUE;
} 

// ��֡�ɼ� 
BOOL CameraBase::Snapshot()
{	

// 	AfxMessageBox("������ݲ�֧�ֵ�֡�ɼ���");

	SnapCallbackAcq();

	return TRUE;
}

//����ͨ�ò���
BOOL CameraBase::SetParamInt(GBParamID Param, int nReturnVal)
{
	switch (Param)
	{
	case GBImageWidth:
		break;
	case GBImageHeight:
		break;
	case GBImagePixelSize:
		break;
	case GBImageBufferSize:
		break;
	case GBImageBufferAddr:
		break;
	case GBGrabberTypeSN:
		break;
	case GBExposureTime:
	{
		SetFloat(GX_FLOAT_EXPOSURE_TIME, (float)nReturnVal);
		m_dExposureTime = m_iExposureTime = nReturnVal;
	}
		break;
	case GBParamID::GBGain:
	{
		m_nGain = nReturnVal;
		SetInt(GX_INT_GAIN, m_nGain);
	}
	break;
	default:
		break;
	}
	return TRUE;
}

//�õ�ͨ�ò���
BOOL CameraBase::GetParamInt(GBParamID Param, int &nReturnVal)
{
	switch(Param)
	{
	case GBImageWidth:
		nReturnVal = m_nWidth;
		break;
	case GBImageHeight:
		nReturnVal = m_nHeight;
		break;
	case GBImagePixelSize:
		nReturnVal = m_nImageByteCount;
		break;
	case GBImageBufferSize:
// 		nReturnVal = m_nWidth * m_nHeight * m_nImageByteCount;
		nReturnVal = m_nWidth * m_nHeight; //[2016-2-24 zhy]
		break;
	case GBImageBufferAddr:
		nReturnVal = (int)m_pImgRGBBuffer;
		break;

	case GBGrabberTypeSN:
		nReturnVal = m_nGrabberType;
		break;

	case GBExposureTime:
		nReturnVal = m_dExposureTime;
		break;
	case GBParamID::GBGain:
		nReturnVal = m_nGain;
		break;
	case GBParamID::GBIsColorFilter:
		nReturnVal =  m_bIsColorFilter;
		break;
	case GBParamID::GBBayerType:
		nReturnVal = m_iBayerType;
		break;
	case GBParamID::GBEnableFlipImage:
		nReturnVal = m_bEnableFlip;
		break;
	default:
		AfxMessageBox(_T("GetParamInt switch(Param)  default:..."));
		m_LastErrorInfo.iErrorCode = DCErrorGetParam;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("�õ���������"));
		_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetParamInt()����"));
		
		return FALSE;
	}
	
	return TRUE;
}

bool CameraBase::GetParamBool(GBParamID Param, bool bRetVal)
{
	switch (Param)
	{
	default:
		break;
	}
	return true;
}

//���ò���
bool CameraBase::SetParamBool(GBParamID Param, bool bInputVal)
{
	return true;
}

//���ò����Ի���
void CameraBase::CallParamDialog()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(m_pParamSetDlg != NULL )
	{
		AfxMessageBox(_T("���ƴ����Ѵ�!")) ;
		return ;
	}
	m_pParamSetDlg=new CSetParamDialog();
	m_pParamSetDlg->m_pCamera = this;
	m_pParamSetDlg->Create(CSetParamDialog::IDD);
	m_pParamSetDlg->CenterWindow();
    m_pParamSetDlg->ShowWindow(SW_SHOW);

}

// ��ô�����Ϣ
void CameraBase::GetLastErrorInfo(s_GBERRORINFO *pErrorInfo)
{
}

BOOL CameraBase::MERSetParamInt(MERParamID Param,int nInputVal1,int nInputVal2,int nInputVal3,int nInputVal4,int nInputVal5,void *sInputVal6)
{
	GX_STATUS status = GX_STATUS_SUCCESS;	

	bool bRet = false;
	if (m_bIsSnaping)
	{
		StopGrab();
		bRet = true;
	}

	switch(Param)
	{
	case MERSnapMode:
		if (nInputVal1 == 0)
		{
			SetEnum(GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_OFF);
			m_bTriggerMode = false;
		} 
		else
		{
			SetEnum(GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_ON);
			m_bTriggerMode = true;
		}
		break;
	case MERExposure:
		SetFloat(GX_FLOAT_EXPOSURE_TIME,(float)nInputVal1);
		m_dExposureTime = nInputVal1;
		break;
	case MERGain:
		SetInt(GX_INT_GAIN,nInputVal1);
		m_nGain = nInputVal1;
		break;
	case MERBlacklevel:
		SetInt(GX_INT_BLACKLEVEL,nInputVal1);
		m_nBlacklevel = nInputVal1;
		break;
	case MERWidth:				//[2017-3-24 zhy]
		if((nInputVal1+m_nOffsetX)>GetCameraMaxWid())
		{
			AfxMessageBox(_T("���+Xƫ�Ƴ������ֵ"));
			return FALSE;
		}
		status = SetInt(GX_INT_WIDTH, nInputVal1);	
		if(status!=GX_STATUS_SUCCESS)
		{
			AfxMessageBox(_T("���ÿ��ʧ��!"));
			return FALSE;
		}
		else
		{
			m_nWidth=nInputVal1;
		}
		break;
	case MERHeight:		//[2017-3-24 zhy]
		if((nInputVal1+m_nOffsetY)>GetCameraMaxHei())
		{
			AfxMessageBox(_T("�߶�+Yƫ�Ƴ������ֵ"));
			return FALSE;
		}
		status = SetInt(GX_INT_HEIGHT, nInputVal1);	
		if(status!=GX_STATUS_SUCCESS)
		{
			AfxMessageBox(_T("���ø߶�ʧ��!"));
			return FALSE;
		}
		else
		{
			m_nHeight=nInputVal1;
		}
		break;
	case MERXOffset:		//[2017-3-24 zhy]
		if((nInputVal1+m_nWidth)>GetCameraMaxWid())
		{
			AfxMessageBox(_T("���+Xƫ�Ƴ������ֵ"));
			return FALSE;
		}
		status = SetInt(GX_INT_OFFSET_X, nInputVal1);	
		if(status!=GX_STATUS_SUCCESS)
		{
			AfxMessageBox(_T("����Xƫ����ʧ��!"));
			return FALSE;
		}
		else
		{
			m_nOffsetX=nInputVal1;
		}
		break;
	case MERYOffset:		//[2017-3-24 zhy]
		if((nInputVal1+m_nHeight)>GetCameraMaxHei())
		{
			AfxMessageBox(_T("�߶�+Yƫ�Ƴ������ֵ"));
			return FALSE;
		}
		status = SetInt(GX_INT_OFFSET_Y, nInputVal1);	
		if(status!=GX_STATUS_SUCCESS)
		{
			AfxMessageBox(_T("����Yƫ����ʧ��!"));
			return FALSE;
		}
		else
		{
			m_nOffsetY=nInputVal1;
		}
		break;
	}

	if (bRet)
	{
		StartGrab();
	}
	return TRUE;
}

//�õ����ר�в���
BOOL CameraBase::MERGetParamInt(MERParamID Param, int &nReturnVal1,int &nReturnVal2,int &nReturnVal3)
{
	__int64 ntemp;
	double dtemp;
	switch(Param)
	{
	case MERSnapMode:
		GetEnum(GX_ENUM_TRIGGER_MODE,&ntemp);
		nReturnVal1 = ntemp;
		break;
	case MERExposure:
		GetFloat(GX_FLOAT_EXPOSURE_TIME,&dtemp);
		nReturnVal1 = (int)dtemp;
		break;
	case MERGain:
		GetInt(GX_INT_GAIN,&ntemp);
		nReturnVal1 = ntemp;
		break;
	case MERBlacklevel:
		GetInt(GX_INT_BLACKLEVEL,&ntemp);
		nReturnVal1 = ntemp;
		break;
	}
	return TRUE;
}



int  CameraBase::GetCameraMaxWid()//�������������
{
	__int64 ntemp;
	GetInt(GX_INT_WIDTH_MAX,&ntemp);
	m_maxwid=ntemp;
	return ntemp;



}
int  CameraBase::GetCameraMaxHei()//�����������߶�
{
	__int64 ntemp;
	double dtemp;
	GetInt(GX_INT_HEIGHT_MAX,&ntemp);
	m_maxhei=ntemp;
	return ntemp;


}




//����˿ڷ����ź�
BOOL CameraBase::SetOutputValue(int nOutputPort,int nOutputValue)
{
	return TRUE;
}

//----------------------------------------------------------------------------------
/**
\brief  ���豸
\return ���豸�ɹ��򷵻�GX_STATUS_SUCCESS�����򷵻ش�����
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::Open(const s_GBINITSTRUCT* pInitParam)
{
	m_nGrabberSN   = pInitParam->iGrabberSN +1;			//��� // ��˾�����Ŵ�1��ʼ,Ϊ�˺������������ݣ��Լ�1 [9/16/2010 SJC]
	m_CallBackFunc = pInitParam->CallBackFunc;			//�ص�����ָ��
	m_EventCallbackFunc	= pInitParam->EventCallbackFunc;//��������Ӧ�¼��ص�����
	m_Context      = pInitParam->Context;				//��ŵ��ó�ʼ�������Ķ����thisָ��
	
	m_sInitFile	   = pInitParam->strGrabberFile;		//���ڳ�ʼ�����ļ�????

	m_sDeviceName  = pInitParam->strDeviceName;			//�ɼ��豸��
	m_strDeviceMark.Format(_T("%s"),pInitParam->strDeviceMark);	
	m_nGrabberType = pInitParam->iGrabberTypeSN ;		// �ɼ�������

	CFileFind fileSerch;
	if(!fileSerch.FindFile(m_sInitFile))
	{
		AfxMessageBox(_T("�����ʼ���ļ�������!"));
		m_LastErrorInfo.iErrorCode = DCErrorOpenCam;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("�����ʼ���ļ�������!"));
		_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("���֤�豸��ʼ���ļ����Ƿ���ȷ"));
		return GX_STATUS_ERROR;
	}
	
	InitParamFromINI();// ��ȡ��������ļ������������ļ�

	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	try
	{
		//ö���豸��������ʱʱ��1000ms[��ʱʱ���û��������ã�û���Ƽ�ֵ]
		uint32_t nDeviceNum = 0;
		emStatus = GXUpdateDeviceList(&nDeviceNum, 1000);//��ȡ�豸����
		m_nTotalNum = nDeviceNum;
		if (emStatus != GX_STATUS_SUCCESS)
		{
			AfxMessageBox(_T("��ȡ�������ʧ��!"));
			return emStatus;
		}

		if (m_nTotalNum <= 0)
		{
			AfxMessageBox(_T("�豸����С�ڵ���0!"));

			return GX_STATUS_ERROR;
		}
		
		//�������кŴ����
		GX_OPEN_PARAM openParam;
		openParam.openMode = GX_OPEN_SN;
		openParam.accessMode = GX_ACCESS_EXCLUSIVE;
		char pDeviceMark[256];
#ifdef UNICODE
		string strTemp = CStringConvert::UCS2_ANSI(m_strDeviceMark.GetBuffer());
		memcpy_s(pDeviceMark, 256, strTemp.c_str(), 256);
		openParam.pszContent = pDeviceMark;
#else
		openParam.pszContent = (LPSTR)(LPCTSTR)m_strDeviceMark;
#endif
		//openParam.pszContent = (LPSTR)(LPCTSTR)m_strDeviceMark;
		emStatus = GXOpenDevice(&openParam,&m_hDevice);//����ָ���ĵ�ǰ�豸

		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("��ʧ�ܣ����к�Ϊ:%s !"),m_strDeviceMark);
			AfxMessageBox(str);

			return emStatus;
		}

		m_bIsOpen = true;

		//ѡ�񴥷��źű������¼�
		GXSetEnum(m_hDevice,GX_ENUM_EVENT_SELECTOR,GX_ENUM_EVENT_SELECTOR_FRAMESTART_OVERTRIGGER);
		
		//���������źű������¼�
		GXSetEnum(m_hDevice,GX_ENUM_EVENT_NOTIFICATION,GX_ENUM_EVENT_NOTIFICATION_ON);
		
		//ע�ᴥ���źű������¼��ص�����
		GXRegisterFeatureCallback(m_hDevice,this,OnFeatureCallbackFun,GX_INT_EVENT_FRAMESTART_OVERTRIGGER,&m_hCB);

//--------------------------������豸���------------------------------------		
		if (!m_bInitSuccess)
		{
			// ���ò���
			SetInitParam();
			
			emStatus = __PrepareForShowImg();
			if (emStatus != GX_STATUS_SUCCESS)
			{
				return emStatus;
			}
			
			if (m_nImageWidth < m_nWidth || m_nImageHeight < m_nHeight)
			{
				AfxMessageBox(_T("��Ȼ��߸߶ȳ����������ֵ"));

				m_LastErrorInfo.iErrorCode = DCErrorValOverFlow;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("��Ȼ��߸߶ȳ����������ֵ"));
				_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("Init()�����е�HVGetDeviceInfo()"));
				return GX_STATUS_ERROR;
			}
			m_bInitSuccess = TRUE;
		}
		else
		{
			AfxMessageBox(_T("����ظ���ʼ��"));

			m_LastErrorInfo.iErrorCode = DCErrorInit;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("����Ѿ���ʼ���ɹ�!"));
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("����ظ���ʼ��"));
			return GX_STATUS_SUCCESS;
		}
	}
	catch(...)
	{
		AfxMessageBox(_T("Init()������try��׽�����쳣"));
		m_LastErrorInfo.iErrorCode = DCErrorInit;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("��ʼ��ʧ��"));
		_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("Init()������try��׽�����쳣"));
		return GX_STATUS_ERROR;
	}
	//**[9/7/2017 ConanSteve]**:  ��ʼ��ͨ�ò���
	m_iImageWidth = m_nWidth;
	m_iImageHeight = m_nHeight;
	m_iImagePixelSize = m_nImageByteCount;
	m_iOffsetX = m_nOffsetX;
	m_iOffsetY = m_nOffsetY;
	bool bIsColorFilter{ false };
	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &bIsColorFilter);
	m_bIsColorFilter = bIsColorFilter ? TRUE : FALSE;
	VERIFY_STATUS_RET(emStatus);
	if (TRUE == m_bIsColorFilter)
	{
		emStatus = GXGetEnum(m_hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &m_nPixelColorFilter);
		m_iBayerType = m_nPixelColorFilter;
	}
	m_bEnableFlip = TRUE;
	return emStatus;
}

//��ȡ�����ļ�
void CameraBase::InitParamFromINI()
{
	try
	{
		m_nWidth = GetPrivateProfileInt(_T("Camera"), _T("Width"), GetCameraMaxWid(), m_sInitFile);//Ĭ��ֵΪ��������ֵ
		m_nHeight = GetPrivateProfileInt(_T("Camera"), _T("Height"), GetCameraMaxHei(), m_sInitFile);
		m_nOffsetX = GetPrivateProfileInt(_T("Camera"), _T("OffsetX"), 0, m_sInitFile);
		m_nOffsetY = GetPrivateProfileInt(_T("Camera"), _T("OffsetY"), 0, m_sInitFile);
		m_nExposureMode = GetPrivateProfileInt(_T("Camera"), _T("ExposureMode"), 0, m_sInitFile);
		m_dExposureTime = GetPrivateProfileInt(_T("Camera"), _T("ExposureTime"), 10000, m_sInitFile);
		m_bExposureAuto = GetPrivateProfileInt(_T("Camera"), _T("ExposureAuto"), 0, m_sInitFile);
		m_nGain = GetPrivateProfileInt(_T("Camera"), _T("Gain"), 0, m_sInitFile);
		m_bGainAuto = GetPrivateProfileInt(_T("Camera"), _T("GainAuto"), 0, m_sInitFile);
		m_nBlacklevel = GetPrivateProfileInt(_T("Camera"), _T("Blacklevel"), 0, m_sInitFile);
		m_bBlacklevelAuto = GetPrivateProfileInt(_T("Camera"), _T("BlacklevelAuto"), 0, m_sInitFile);
		m_dBalanceRatio = GetPrivateProfileInt(_T("Camera"), _T("BalanceRatio"), 0, m_sInitFile);
		m_bBalanceRatioAuto = GetPrivateProfileInt(_T("Camera"), _T("BalanceRatioAuto"), 0, m_sInitFile);
		m_bTriggerMode = GetPrivateProfileInt(_T("Camera"), _T("TriggerMode"), 0, m_sInitFile);
	    m_nTriggerActivation = GetPrivateProfileInt(_T("Camera"), _T("TriggerActivation"), 0, m_sInitFile);
		m_nTriggerSource = GetPrivateProfileInt(_T("Camera"),_T("TriggerSource"),0,m_sInitFile); //[2016-11-18 zhy] 
	}
	catch (...) 
	{
		m_LastErrorInfo.iErrorCode = DCErrorReadIniFile;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, "��ȡ�����ļ�����");
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "InitParamFromINI()����");
	}
}

//�������������Ϣ
void CameraBase::SaveParamToINI()
{
	CString str;
	str.Format(_T("%d"), m_nWidth);
	WritePrivateProfileString(_T("Camera"), _T("Width"), str, m_sInitFile);
	str.Format(_T("%d"), m_nHeight);
	WritePrivateProfileString(_T("Camera"), _T("Height"), str, m_sInitFile);
	str.Format(_T("%d"), m_nOffsetX);
	WritePrivateProfileString(_T("Camera"), _T("OffsetX"), str, m_sInitFile);
	str.Format(_T("%d"), m_nOffsetY);
	WritePrivateProfileString(_T("Camera"), _T("OffsetY"), str, m_sInitFile);
	str.Format(_T("%d"), m_nExposureMode);
	WritePrivateProfileString(_T("Camera"), _T("ExposureMode"), str, m_sInitFile);
	str.Format(_T("%d"), (int)m_dExposureTime);
	WritePrivateProfileString(_T("Camera"), _T("ExposureTime"), str, m_sInitFile);
	str.Format(_T("%d"), (int)m_bExposureAuto);
	WritePrivateProfileString(_T("Camera"), _T("ExposureAuto"), str, m_sInitFile);
	str.Format(_T("%d"), m_nGain);
	WritePrivateProfileString(_T("Camera"), _T("Gain"), str, m_sInitFile);
	str.Format(_T("%d"), (int)m_bGainAuto);
	WritePrivateProfileString(_T("Camera"), _T("GainAuto"), str, m_sInitFile);
	str.Format(_T("%d"), m_nBlacklevel);
	WritePrivateProfileString(_T("Camera"), _T("Blacklevel"), str, m_sInitFile);
	str.Format(_T("%d"), (int)m_bBlacklevelAuto);
	WritePrivateProfileString(_T("Camera"), _T("BlacklevelAuto"), str, m_sInitFile);
	str.Format(_T("%d"), m_dBalanceRatio);
	WritePrivateProfileString(_T("Camera"), _T("BalanceRatio"), str, m_sInitFile);
	str.Format(_T("%d"), (int)m_bBalanceRatioAuto);
	WritePrivateProfileString(_T("Camera"), _T("BalanceRatioAuto"), str, m_sInitFile);
	str.Format(_T("%d"), (int)m_bTriggerMode);
	WritePrivateProfileString(_T("Camera"), _T("TriggerMode"), str, m_sInitFile);
	str.Format(_T("%d"), m_nTriggerActivation);
	WritePrivateProfileString(_T("Camera"), _T("TriggerActivation"), str, m_sInitFile);
	str.Format(_T("%d"), m_nTriggerSource);
	WritePrivateProfileString(_T("Camera"), _T("TriggerSource"), str, m_sInitFile);

}

void CameraBase::SetInitParam()
{
	//���ø���Ȥ����
	GX_STATUS status = GX_STATUS_SUCCESS;	
	status = SetInt(GX_INT_WIDTH, m_nWidth);	
	status = SetInt(GX_INT_HEIGHT, m_nHeight);
	status = SetInt(GX_INT_OFFSET_X, m_nOffsetX);
	status = SetInt(GX_INT_OFFSET_Y, m_nOffsetY);

	status = SetEnum(GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);
	status = SetEnum(GX_ENUM_GAIN_AUTO,GX_GAIN_AUTO_CONTINUOUS);
	//status=SetEnum(GX_ENUM_PIXEL_FORMAT,mono)

	//���ô���ģʽ
	if (m_bTriggerMode)
	{
		status = SetEnum(GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_ON);
	}
	else
	{
		status = SetEnum(GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_OFF);
	}
	//--------------------------------------------------------------------------------------------------
	
	//���ô������ʽ
	switch(m_nTriggerActivation)
	{
	case 0:
		status = SetEnum(GX_ENUM_TRIGGER_ACTIVATION,GX_TRIGGER_ACTIVATION_RISINGEDGE);
		break;
	case 1:
		status = SetEnum(GX_ENUM_TRIGGER_ACTIVATION,GX_TRIGGER_ACTIVATION_FALLINGEDGE);
		break;
	}
	
	// ���ô���Դ
	status = GXSetEnum(m_hDevice, GX_ENUM_TRIGGER_SOURCE, m_nTriggerSource);

	//�����ع�ģʽ
	switch(m_nExposureMode)
	{
	case 0:
//		status = SetEnum(GX_ENUM_EXPOSURE_MODE,GX_EXPOSURE_MODE_OFF);//����ˮ��Gige�ӿڵ�SDKû������ṹ��ı�ʾ�����Ծ�ɾ����
		break;
	case 1:
		status = SetEnum(GX_ENUM_EXPOSURE_MODE,GX_EXPOSURE_MODE_TIMED);
		break;
	case 2:
		status = SetEnum(GX_ENUM_EXPOSURE_MODE,GX_EXPOSURE_MODE_TRIGGERWIDTH);
		break;
	default:
		//status = SetEnum(GX_ENUM_EXPOSURE_MODE,GX_EXPOSURE_MODE_OFF);
		break;
	}
	//�����Զ��ع�
	if (m_bExposureAuto)
	{
		status = SetEnum(GX_ENUM_EXPOSURE_AUTO,GX_EXPOSURE_AUTO_CONTINUOUS);
	}
	else
	{
        status = SetEnum(GX_ENUM_EXPOSURE_AUTO,GX_EXPOSURE_AUTO_OFF);
		//�����ع�ʱ��
	    status = SetFloat(GX_FLOAT_EXPOSURE_TIME,m_dExposureTime);
	}

	//�����Զ�����
	if (m_bGainAuto)
	{
		status = SetEnum(GX_ENUM_GAIN_AUTO,GX_GAIN_AUTO_CONTINUOUS);
	}
	else
	{
		status = SetEnum(GX_ENUM_GAIN_AUTO,GX_GAIN_AUTO_OFF);
		//��������
	    status = SetInt(GX_INT_GAIN,m_nGain);
	}

	if (m_bBlacklevelAuto)
	{
		status = SetEnum(GX_ENUM_BLACKLEVEL_AUTO,GX_BLACKLEVEL_AUTO_CONTINUOUS);
	}
	else
	{
		status = SetEnum(GX_ENUM_BLACKLEVEL_AUTO,GX_BLACKLEVEL_AUTO_OFF);
		//���úڵ�ƽ
	    status = SetInt(GX_INT_BLACKLEVEL,m_nBlacklevel);
	}
	
//------------------------------------------------------------------------------------------------------
	
// 	//�����Զ���ƽ��
// 	if (m_bBalanceRatioAuto)
// 	{
// 		status = SetEnum(GX_ENUM_BALANCE_WHITE_AUTO,GX_BALANCE_WHITE_AUTO_CONTINUOUS);
// 	}
// 	else
// 	{
// 		status = SetEnum(GX_ENUM_BALANCE_WHITE_AUTO,GX_BALANCE_WHITE_AUTO_OFF);
// 		//���ð�ƽ��
//      	status = SetFloat(GX_FLOAT_BALANCE_RATIO,m_dBalanceRatio);
// 	}
	

}

//----------------------------------------------------------------------------------
/**
\brief  �ر��豸
\return �ر��豸�ɹ��򷵻�GX_STATUS_SUCCESS�����򷵻ش�����
*/
//----------------------------------------------------------------------------------
BOOL CameraBase::Close()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	BOOL bRet = FALSE;
	//**[9/11/2017 ConanSteve]**:  �ȱ������
	CExFn::WritePrivateProfileInt(_T("Camera"), _T("Gain"), m_nGain, m_sInitFile);
	CExFn::WritePrivateProfileInt(_T("Camera"), _T("ExposureTime"), m_dExposureTime, m_sInitFile);
	if (IsOpen())
	{
		if (IsSnaping())
		{
			emStatus = StopCallbackAcq();
			if (emStatus != GX_STATUS_SUCCESS)
			{
				return FALSE;
			}
		}

		__UnPrepareForShowImg();


		//ע�������źű������¼��ص�
		//GXUnregisterFeatureCallback(m_hDevice, GX_INT_EVENT_FRAMESTART_OVERTRIGGER, m_hCB);
		
		emStatus = GXCloseDevice(m_hDevice);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			return FALSE;
		}
		bRet = TRUE;
		m_bIsOpen = false;

	}
	return bRet;
}

//----------------------------------------------------------------------------------
/**
\brief  �豸��״̬
\return �豸���򷵻�true�����򷵻�false
*/
//----------------------------------------------------------------------------------
bool CameraBase::IsOpen()
{
	return m_bIsOpen;
}



//----------------------------------------------------------------------------------
/**
\brief  ע��ͼ����ʾ����
\param  pWnd  ָ��ͼ����ʾ����ָ��
\return ͼ����ʾ����ָ�벻ΪNULL������GX_STATUS_SUCCESS�����򷵻�GX_STATUS_INVALID_PARAMETER
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::RegisterShowWnd(void* pWnd)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	if (pWnd == NULL)
	{
		return GX_STATUS_INVALID_PARAMETER;
	}

	m_pShowWnd = pWnd;

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ע����־��ʾ����
\param  pWnd  ָ����־��ʾ����ָ��
\return ��־��ʾ����ָ�벻ΪNULL������GX_STATUS_SUCCESS�����򷵻�GX_STATUS_INVALID_PARAMETER
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::RegisterLogWnd(void* pWnd)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	if (pWnd == NULL)
	{
		return GX_STATUS_INVALID_PARAMETER;
	}

	m_pLogWnd = pWnd;

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ��ʼע��ص�
\return ���ӿڵ��óɹ����򷵻�GX_STATUS_SUCCESS�����򷵻�GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::StartCallbackAcq()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;

	if (IsOpen())
	{
		emStatus = GXRegisterCaptureCallback(m_hDevice, this, __OnFrameCallbackFun);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			__UnPrepareForShowImg();
			return emStatus;
		}
	//	__PrintInfotoWnd(MsgComposer("ע��ɼ��ص�����"));
		emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_START);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			__UnPrepareForShowImg();
			return emStatus;
		}
	//	__PrintInfotoWnd(MsgComposer("���Ϳ�������"));
		m_bIsSnaping = true;
	}	
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ֹͣע��ص�
\return ���ӿڵ��óɹ����򷵻�GX_STATUS_SUCCESS�����򷵻�GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::StopCallbackAcq()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen() && IsSnaping())
	{
		//�����ǰ����¼����Ƶ������ֹͣ¼��
		if (m_bSaveAVI)
		{
			EnableSaveAVI(false);
		}

		emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_STOP);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			return emStatus;
		}
		//__PrintInfotoWnd(MsgComposer("����ͣ������"));
		emStatus = GXUnregisterCaptureCallback(m_hDevice);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			return emStatus;
		}
		//__PrintInfotoWnd(MsgComposer("ע���ɼ��ص�����"));

		m_bIsSnaping = false;		
	}	
	return emStatus;
}

GX_STATUS CameraBase::SnapCallbackAcq()
{
	GX_STATUS  emStatus = GX_STATUS_SUCCESS;
	
	// ������������
	emStatus = GXSendCommand(m_hDevice,GX_COMMAND_TRIGGER_SOFTWARE);

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ʹ����ʾͼ����
\param  bShowImg  true��ʾ��ʾͼ��false��ʾ����ʾͼ��
\return ��
*/
//----------------------------------------------------------------------------------
void CameraBase::EnableShowImg(bool bShowImg)
{
	m_bShowImg = bShowImg;
}

//----------------------------------------------------------------------------------
/**
\brief  ʹ�ܱ���BMP����
\param  bSaveBMP  true��ʾ���棻false��ʾ������
\return ��
*/
//----------------------------------------------------------------------------------
void CameraBase::EnableSaveBMP(bool bSaveBMP)
{
	m_bSaveBMP = bSaveBMP;
}

void CameraBase::CompressedAVI(bool bCompressed)
{
	m_bCompressed = bCompressed;
}
//----------------------------------------------------------------------------------
/**
\brief  ʹ�ܱ���AVI����
\param  bSaveAVI  true��ʾ���棻false��ʾ������
\return ��
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::EnableSaveAVI(bool bSaveAVI)
{
//	m_SaverCriSec.Enter();
// 	m_bSaveAVI = bSaveAVI;
// 
 	GX_STATUS emStatus = GX_STATUS_SUCCESS;
// 	if (m_bSaveAVI)
// 	{
// 		emStatus = __PrepareForSaveAVI();
// 		if (emStatus != GX_STATUS_SUCCESS)
// 		{
// 			m_SaverCriSec.Leave();
// 			__UnPrepareForSaveAVI();
// 			return emStatus;
// 		}
// 	}
// 	else
// 	{
// 		__UnPrepareForSaveAVI();
// 	}
// 	m_SaverCriSec.Leave();
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ���ñ���BMPͼƬ���ļ���·��
\param  pBMPFolder  ·����β������б��
\return ��
*/
//----------------------------------------------------------------------------------
void CameraBase::SetBMPFolder(TCHAR* pBMPFolder)
{
	m_strBMPFolder = CString(pBMPFolder);
}

//----------------------------------------------------------------------------------
/**
\brief  ���ñ���AVI��Ƶ���ļ���·��
\param  szAVIFolder  ·����β������б��
\return ��
*/
//----------------------------------------------------------------------------------
void CameraBase::SetAVIFolder(TCHAR* pAVIFolder)
{
	m_strAVIFolder = CString(pAVIFolder);
}

//----------------------------------------------------------------------------------
/**
\brief  �豸��ʼ�ɼ�״̬
\return �豸��ʼ�ɼ��򷵻�true�����򷵻�false
*/
//----------------------------------------------------------------------------------
bool CameraBase::IsSnaping()
{
	return m_bIsSnaping;
}

GX_STATUS CameraBase::IsImplemented(GX_FEATURE_ID emFeatureID, bool* pbIsImplemented)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXIsImplemented(m_hDevice, emFeatureID, pbIsImplemented);
		//��ӡ��־��Ϣ
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("ʧ��:GXIsImplemented ��������:%s ������:%d"),chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
	     	_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("IsImplemented()����"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡ����������
\param  emFeatureID  ������ID
\param  pszName    ָ�򷵻ع��������Ƶ�ָ��
\param  pnSize     ��������ַ�������
\return            ���ӿڵ��óɹ����򷵻�GX_STATUS_SUCCESS�����򷵻�GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetFeatureName(GX_FEATURE_ID emFeatureID, char* pszName, size_t* pnSize)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetFeatureName(m_hDevice, emFeatureID, pszName, pnSize);
		//��ӡ��־��Ϣ
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("ʧ��:GXGetFeatureName ��������:%s ������:%d"),pszName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
	     	_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetFeatureName()����"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡInt���Ͳ�����Сֵ�����ֵ������
\param  emFeatureID  ������ID
\param  pIntRange  ָ�򷵻�int�ͽṹ���ָ��
\return            ���ӿڵ��óɹ����򷵻�GX_STATUS_SUCCESS�����򷵻�GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetIntRange(GX_FEATURE_ID emFeatureID, GX_INT_RANGE* pIntRange)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetIntRange(m_hDevice, emFeatureID, pIntRange);
		//��ӡ��־��Ϣ
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("ʧ��:GXGetIntRange ��������:%s ������:%d"),chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
	     	_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetIntRange()����"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡint�Ͳ�����ǰֵ
\param  emFeatureID  ������ID
\param  pnValue    ָ�򷵻ص�ǰֵ��ָ��
\return            ���ӿڵ��óɹ����򷵻�GX_STATUS_SUCCESS�����򷵻�GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetInt(GX_FEATURE_ID emFeatureID, int64_t* pnValue)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetInt(m_hDevice, emFeatureID, pnValue);
		//��ӡ��־��Ϣ
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("ʧ��:GXGetInt ��������:%s ������:%d"),chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
	     	_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetInt()����"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ����Int����ֵ���ܲ���
\param  emFeatureID  ������ID
\param  pnValue    �û���Ҫ���õ�ֵ
\return            ���ӿڵ��óɹ����򷵻�GX_STATUS_SUCCESS�����򷵻�GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::SetInt(GX_FEATURE_ID emFeatureID, int64_t nValue)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXSetInt(m_hDevice, emFeatureID, nValue);
		//��ӡ��־��Ϣ
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("ʧ��:GXSetInt ��������:%s ������:%d"),chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorSetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
	     	_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("SetInt()����"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡFloat���Ͳ�������Сֵ�����ֵ����������λ��������Ϣ
\param  emFeatureID    ������ID
\param  pFloatRange  ָ�򷵻�Float���ͽṹ���ָ��
\return              ���ӿڵ��óɹ����򷵻�GX_STATUS_SUCCESS�����򷵻�GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetFloatRange(GX_FEATURE_ID emFeatureID, GX_FLOAT_RANGE* pFloatRange)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetFloatRange(m_hDevice, emFeatureID, pFloatRange);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		emStatus = GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("ʧ��:GXGetFloatRange ��������:%s ������:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetFloatRange()����"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡFloat���͵�ǰ����ֵ
\param  emFeatureID    ������ID
\param  pdValue      ָ�򷵻�Float���Ͳ���ֵ��ָ��
\return              ���ӿڵ��óɹ����򷵻�GX_STATUS_SUCCESS�����򷵻�GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetFloat(GX_FEATURE_ID emFeatureID, double* pdValue)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetFloat(m_hDevice, emFeatureID, pdValue);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("ʧ��:GXGetFloat ��������:%s ������:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetFloat()����"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ����Float���Ͳ���ֵ
\param  emFeatureID    ������ID
\param  dValue       �û���Ҫ���õ�ֵ
\return              ���ӿڵ��óɹ����򷵻�GX_STATUS_SUCCESS�����򷵻�GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::SetFloat(GX_FEATURE_ID emFeatureID, double dValue)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXSetFloat(m_hDevice, emFeatureID, dValue);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("ʧ��:GXSetFloat ��������:%s ������:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorSetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("SetFloat()����"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡö����Ŀ�ѡ�����
\param  emFeatureID    ������ID
\param  pnEntryNums  ָ�򷵻ؿ�ѡ�������ָ��
\return              ���ӿڵ��óɹ����򷵻�GX_STATUS_SUCCESS�����򷵻�GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetEnumEntryNums(GX_FEATURE_ID emFeatureID, uint32_t* pnEntryNums)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetEnumEntryNums(m_hDevice, emFeatureID, pnEntryNums);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("ʧ��:GXGetEnumEntryNums ��������:%s ������:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetEnumEntryNums()����"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡö������ֵ��������Ϣ
\param  emFeatureID         ������ID
\param  pEnumDescription  ָ��ö�����������Ϣ������ָ��
\param  pBufferSize       ָ������ö����������Ϣ�������С
\return                   ���ӿڵ��óɹ����򷵻�GX_STATUS_SUCCESS�����򷵻�GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetEnumDescription(GX_FEATURE_ID emFeatureID, GX_ENUM_DESCRIPTION* pEnumDescription, size_t* pBufferSize)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetEnumDescription(m_hDevice, emFeatureID, pEnumDescription, pBufferSize);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("ʧ��:GXGetEnumDescription ��������:%s ������:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetEnumDescription()����"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡ��ǰö��ֵ
\param  emFeatureID  ������ID
\param  pnValue    ָ�򷵻ص�ö��ֵ��ָ��
\return            ���ӿڵ��óɹ����򷵻�GX_STATUS_SUCCESS�����򷵻�GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetEnum(GX_FEATURE_ID emFeatureID, int64_t* pnValue)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetEnum(m_hDevice, emFeatureID, pnValue);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("ʧ��:GXGetEnum ��������:%s ������:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetEnum()����"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ����ö��ֵ
\param  emFeatureID  ������ID
\param  nValue     �û���Ҫ���õ�ö��ֵ
\return            ���ӿڵ��óɹ����򷵻�GX_STATUS_SUCCESS�����򷵻�GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::SetEnum(GX_FEATURE_ID emFeatureID, int64_t nValue)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXSetEnum(m_hDevice, emFeatureID, nValue);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("ʧ��:GXGetEnum ��������:%s ������:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorSetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("SetEnum()����"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡbool����ֵ
\param  emFeatureID  ������ID
\param  nValue     ָ�򷵻صĲ���ֵ��ָ��
\return            ���ӿڵ��óɹ����򷵻�GX_STATUS_SUCCESS�����򷵻�GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetBool(GX_FEATURE_ID emFeatureID, bool* pbValue)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetBool(m_hDevice, emFeatureID, pbValue);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("ʧ��:GXGetBool ��������:%s ������:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetBool()����"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ����bool����ֵ
\param  emFeatureID  ������ID
\param  bValue     �û���Ҫ���õĲ���ֵ
\return            ���ӿڵ��óɹ����򷵻�GX_STATUS_SUCCESS�����򷵻�GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::SetBool(GX_FEATURE_ID emFeatureID, bool bValue)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXSetBool(m_hDevice, emFeatureID, bValue);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("ʧ��:GXSetBool ��������:%s ������:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorSetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("SetBool()����"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡ�ַ�������ֵ�ĳ���
\param  emFeatureID  ������ID
\param  bValue     ָ�򷵻صĳ���ֵ��ָ�룬����ֵ��ĩλ'\0'�����ȵ�λ�ֽ�
\return            ���ӿڵ��óɹ����򷵻�GX_STATUS_SUCCESS�����򷵻�GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetStringLength(GX_FEATURE_ID emFeatureID, size_t* pnSize)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetStringLength(m_hDevice, emFeatureID, pnSize);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("ʧ��:GXGetStringLength ��������:%s ������:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetStringLength()����"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡ�ַ�������ֵ������
\param  emFeatureID  ������ID
\param  pszContent ָ���û�������ַ��������ַ
\param  pnSize     ��ʾ�û�������ַ�����������ַ�ĳ���
\return            ���ӿڵ��óɹ����򷵻�GX_STATUS_SUCCESS�����򷵻�GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetString(GX_FEATURE_ID emFeatureID, char* pszContent, size_t* pnSize)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetString(m_hDevice, emFeatureID, pszContent, pnSize);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("ʧ��:GXGetString ��������:%s ������:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetString()����"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  �����ַ���ֵ����
\param  emFeatureID  ������ID
\param  pszContent ָ���û���Ҫ���õ��ַ�����ַ
\return            ���ӿڵ��óɹ����򷵻�GX_STATUS_SUCCESS�����򷵻�GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::SetString(GX_FEATURE_ID emFeatureID, char* pszContent)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXSetString(m_hDevice, emFeatureID, pszContent);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("ʧ��:GXSetString ��������:%s ������:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorSetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("SetString()����"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡ������ݳ���
\param  emFeatureID  ������ID
\param  pnSize     ָ�򷵻صĳ���ֵ��ָ�룬���ȵ�λ�ֽ�
\return            ���ӿڵ��óɹ����򷵻�GX_STATUS_SUCCESS�����򷵻�GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetBufferLength(GX_FEATURE_ID emFeatureID, size_t* pnSize)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetBufferLength(m_hDevice, emFeatureID, pnSize);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("ʧ��:GXGetBufferLength ��������:%s ������:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetBufferLength()����"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ��ȡ�������
\param  emFeatureID  ������ID
\param  pBuffer    ָ���û�����Ŀ�������ڴ��ַָ��
\param  pnSize     ��ʾ�û�����Ļ�������ַ�ĳ���
\return            ���ӿڵ��óɹ����򷵻�GX_STATUS_SUCCESS�����򷵻�GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetBuffer(GX_FEATURE_ID emFeatureID, uint8_t* pBuffer, size_t* pnSize)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetBuffer(m_hDevice, emFeatureID, pBuffer, pnSize);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("ʧ��:GXGetBuffer ��������:%s ������:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetBuffer()����"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ���ÿ������
\param  emFeatureID  ������ID
\param  pBuffer    ָ���û�����Ŀ�������ڴ��ַָ��
\param  nSize      �û�����Ļ�������ַ�ĳ���
\return            ���ӿڵ��óɹ����򷵻�GX_STATUS_SUCCESS�����򷵻�GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::SetBuffer(GX_FEATURE_ID emFeatureID, uint8_t* pBuffer, size_t nSize)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXSetBuffer(m_hDevice, emFeatureID, pBuffer, nSize);
		char chFeatureName[64] = {0};
		size_t nSizeValue = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSizeValue);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("ʧ��:GXSetBuffer ��������:%s ������:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorSetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("SetBuffer()����"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ����������
\param  emFeatureID  ������ID
\return            ���ӿڵ��óɹ����򷵻�GX_STATUS_SUCCESS�����򷵻�GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::SendCommand(GX_FEATURE_ID emFeatureID)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXSendCommand(m_hDevice, emFeatureID);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ���Ϳ��������,�˽ӿڿ���ֱ�ӻ�ȡͼ��ע��˽ӿڲ�����ص��ɼ���ʽ���ã�
\param  pFrameData  ָ���û��������������ͼ�����ݵĵ�ַָ��
\param  nTimeout    ȡͼ�ĳ�ʱʱ��
\return             ���ӿڵ��óɹ����򷵻�GX_STATUS_SUCCESS�����򷵻�GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetImage(GX_FRAME_DATA* pFrameData, int32_t nTimeout)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
// 	if (IsOpen())
// 	{
// 		emStatus = GXGetImage(m_hDevice, pFrameData, nTimeout);
// 		if (emStatus != GX_STATUS_SUCCESS)
// 		{
// 			__PrintInfotoWnd(MsgComposer("ʧ��:GXGetImage ������:%d", emStatus));
// 		}
// 		else
// 		{
// 			if (m_bEnableGetImageProcess)
// 			{
// 				//���Ƚ��ص��������ͼ�����ݣ������RGB���ݣ��Ա��������ʾ�ʹ洢
// 				__ProcessData((BYTE*)pFrameData->pImgBuf,m_pImgRGBBuffer, m_nImageWidth, m_nImageHeight);
// 				//��ָ��������һ֡һ֡�Ļ�ͼ
// 				__DrawImg(m_pImgRGBBuffer, m_nImageWidth, m_nImageHeight);
// 				//��ָ���洢Ŀ¼��һ֡һ֡�Ĵ洢bmpͼ
// 				__SaveBMP(m_pImgRGBBuffer, m_nImageWidth, m_nImageHeight);
// 			}
// 		}
// 		
// 	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  ʹ��GetImage�ӿڵ��ڲ�������̣�����ͼƬ��Ϣ����ʾ������
\param  bEnableGetImageProcess  true��ʾʹ�ܣ�false��ʾ����
\return ��
*/
//----------------------------------------------------------------------------------
void CameraBase::EnableGetImageProcess(bool bEnableGetImageProcess)
{
	m_bEnableGetImageProcess = bEnableGetImageProcess;
}



//----------------------------------------------------------------------------------
/**
\brief  Ϊͼ����ʾ׼����Դ
\return �ɹ�����GX_STATUS_SUCCESS;���򷵻���Ӧ�����룬�����ѯGX_STATUS_LIST
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::__PrepareForShowImg()
{      
	GX_STATUS emStatus = GX_STATUS_ERROR;
	//��ȡͼ�����ݴ�С
	emStatus = GXGetInt(m_hDevice, GX_INT_PAYLOAD_SIZE, &m_nPayLoadSize);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		AfxMessageBox(_T("��ȡ���ݴ�С����!"));

		return emStatus;
	}
	//���ͼ���ʽ
	emStatus = GXGetEnum(m_hDevice, GX_ENUM_PIXEL_FORMAT, &m_nPixelFormat);//?????????????????

	if (emStatus != GX_STATUS_SUCCESS)
	{
		AfxMessageBox(_T("��ȡ���ظ�ʽ����!"));

		return emStatus;
	}
	switch(m_nPixelFormat)
	{
	case GX_PIXEL_FORMAT_MONO8:
	case GX_PIXEL_FORMAT_MONO10:
	case GX_PIXEL_FORMAT_MONO12:
	    m_nImageByteCount = 1 ;
		break;
	default:
		m_nImageByteCount = 1 ;
		break;
	}

	//��ѯ��ǰ����Ƿ�֧��GX_ENUM_PIXEL_COLOR_FILTER
	bool bIsImplemented = false;
	GXIsImplemented(m_hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &bIsImplemented);
	if(bIsImplemented)
	{
		emStatus = GXGetEnum(m_hDevice,GX_ENUM_PIXEL_COLOR_FILTER,&m_nPixelColorFilter);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			AfxMessageBox(_T("��ȡBayer��ʽ����!"));

			return emStatus;
		}
	}

	//��ȡ���
	emStatus = GXGetInt(m_hDevice, GX_INT_WIDTH, &m_nImageWidth);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		AfxMessageBox(_T("��ȡͼ���ȳ���!"));

		return emStatus;
	}
	//��ȡ�߶�
	emStatus = GXGetInt(m_hDevice, GX_INT_HEIGHT, &m_nImageHeight);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		AfxMessageBox(_T("��ȡͼ��߶ȳ���!"));

		return emStatus;
	}

	if ((m_nImageWidth != 0) && (m_nImageHeight != 0))
	{
		//Ϊ�洢Raw8���ݿ��ٿռ�
		m_pImgRaw8Buffer = new BYTE[size_t(m_nImageWidth*m_nImageHeight)];
		if (m_pImgRaw8Buffer == NULL)
		{
			return GX_STATUS_INVALID_HANDLE;
		}
		//Ϊ�洢RGB���ݿ��ٿռ�
		m_pImgRGBBuffer = new BYTE[size_t(m_nImageWidth*m_nImageHeight*3)];
		if (m_pImgRGBBuffer == NULL)
		{
			if (m_pImgRaw8Buffer != NULL)
			{
				delete[]m_pImgRaw8Buffer;
				m_pImgRaw8Buffer = NULL;
			}
			return GX_STATUS_INVALID_HANDLE;
		}
	}
	else
	{
		AfxMessageBox(_T("ͼ���Ȼ�߶�Ϊ0!"));

		return GX_STATUS_INVALID_HANDLE;
	}

	return emStatus;
}


//----------------------------------------------------------------------------------
/**
\brief  �ͷ�Ϊͼ����ʾ׼����Դ
\return �޷���ֵ
*/
//----------------------------------------------------------------------------------
void CameraBase::__UnPrepareForShowImg()
{
	if (m_pImgRGBBuffer != NULL)
	{
		delete []m_pImgRGBBuffer;
		m_pImgRGBBuffer = NULL;
	}
	if (m_pImgRaw8Buffer != NULL)
	{
		delete []m_pImgRaw8Buffer;
		m_pImgRaw8Buffer = NULL;
	}
}

//----------------------------------------------------------------------------------
/**
\brief  ����������ԭʼ����ת��ΪRGB����
\param  pImageBuf  ָ��ͼ�񻺳�����ָ��
\param  pImageRGBBuf  ָ��RGB���ݻ�������ָ��
\param  nImageWidth ͼ���
\param  nImageHeight ͼ���
\return �޷���ֵ
*/
//----------------------------------------------------------------------------------
void CameraBase::__ProcessData(BYTE * pImageBuf, BYTE * pImageRGBBuf, int64_t nImageWidth, int64_t nImageHeight)
{
	switch(m_nPixelFormat)
	{
		//�����ݸ�ʽΪ12λʱ��λ��ת��Ϊ4-11
	case GX_PIXEL_FORMAT_BAYER_GR12:
	case GX_PIXEL_FORMAT_BAYER_RG12:
	case GX_PIXEL_FORMAT_BAYER_GB12:
	case GX_PIXEL_FORMAT_BAYER_BG12:
		//��12λ��ʽ��ͼ��ת��Ϊ8λ��ʽ
		DxRaw16toRaw8(pImageBuf, m_pImgRaw8Buffer, (VxUint32)nImageWidth, (VxUint32)nImageHeight, DX_BIT_4_11);	

		//��Raw8ͼ��ת��ΪRGBͼ���Թ���ʾ
		DxRaw8toRGB24(m_pImgRaw8Buffer, pImageRGBBuf,(VxUint32)nImageWidth, (VxUint32)nImageHeight, RAW2RGB_NEIGHBOUR,
		DX_PIXEL_COLOR_FILTER(m_nPixelColorFilter), TRUE);		        
		break;

		//�����ݸ�ʽΪ12λʱ��λ��ת��Ϊ2-9
	case GX_PIXEL_FORMAT_BAYER_GR10:
	case GX_PIXEL_FORMAT_BAYER_RG10:
	case GX_PIXEL_FORMAT_BAYER_GB10:
	case GX_PIXEL_FORMAT_BAYER_BG10:
		////��12λ��ʽ��ͼ��ת��Ϊ8λ��ʽ,��Чλ��2-9
		DxRaw16toRaw8(pImageBuf, m_pImgRaw8Buffer, (VxUint32)nImageWidth, (VxUint32)nImageHeight, DX_BIT_2_9);

		//��Raw8ͼ��ת��ΪRGBͼ���Թ���ʾ
		DxRaw8toRGB24(m_pImgRaw8Buffer, pImageRGBBuf, (VxUint32)nImageWidth, (VxUint32)nImageHeight, RAW2RGB_NEIGHBOUR,
		DX_PIXEL_COLOR_FILTER(m_nPixelColorFilter), TRUE);	
		break;

	case GX_PIXEL_FORMAT_BAYER_GR8:
	case GX_PIXEL_FORMAT_BAYER_RG8:
	case GX_PIXEL_FORMAT_BAYER_GB8:
	case GX_PIXEL_FORMAT_BAYER_BG8:
		//��Raw8ͼ��ת��ΪRGBͼ���Թ���ʾ
		DxRaw8toRGB24(pImageBuf,pImageRGBBuf, (VxUint32)nImageWidth, (VxUint32)nImageHeight,RAW2RGB_NEIGHBOUR,
			DX_PIXEL_COLOR_FILTER(m_nPixelColorFilter),TRUE);	
		break;

	case GX_PIXEL_FORMAT_MONO12:
		//��12λ��ʽ��ͼ��ת��Ϊ8λ��ʽ
		DxRaw16toRaw8(pImageBuf, m_pImgRaw8Buffer, (VxUint32)nImageWidth, (VxUint32)nImageHeight, DX_BIT_4_11);	
		//��Raw8ͼ��ת��ΪRGBͼ���Թ���ʾ
		DxRaw8toRGB24(m_pImgRaw8Buffer, pImageRGBBuf, (VxUint32)nImageWidth, (VxUint32)nImageHeight, RAW2RGB_NEIGHBOUR,
		DX_PIXEL_COLOR_FILTER(NONE), TRUE);		        
		break;

	case GX_PIXEL_FORMAT_MONO10:
		//��10λ��ʽ��ͼ��ת��Ϊ8λ��ʽ
		DxRaw16toRaw8(pImageBuf, m_pImgRaw8Buffer, (VxUint32)nImageWidth, (VxUint32)nImageHeight, DX_BIT_4_11);	
		//��Raw8ͼ��ת��ΪRGBͼ���Թ���ʾ
		DxRaw8toRGB24(m_pImgRaw8Buffer,pImageRGBBuf, (VxUint32)nImageWidth, (VxUint32)nImageHeight,RAW2RGB_NEIGHBOUR,
			DX_PIXEL_COLOR_FILTER(NONE), TRUE);		        
		break;

	case GX_PIXEL_FORMAT_MONO8:
		//��Raw8ͼ��ת��ΪRGBͼ���Թ���ʾ
		DxRaw8toRGB24(pImageBuf,pImageRGBBuf, (VxUint32)nImageWidth, (VxUint32)nImageHeight,RAW2RGB_NEIGHBOUR,
			DX_PIXEL_COLOR_FILTER(NONE), TRUE);	


	default:
		break;
	}
}


//----------------------------------------------------------------------------------
/**
\brief  �ص�����
\param  pFrame  ָ��ɼ��ص������ݽṹָ��
\return �޷���ֵ
*/
//----------------------------------------------------------------------------------
void __stdcall CameraBase::__OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	CameraBase *pCamera = (CameraBase *)(pFrame->pUserParam);
	if (pFrame->status == 0)
	{
		BOOL bRet = FALSE;
		try
		{
			//���Ƚ��ص��������ͼ�����ݣ������RGB���ݣ��Ա��������ʾ�ʹ洢
		  //  pCamera->__ProcessData((BYTE*)pFrame->pImgBuf, pCamera->m_pImgRGBBuffer, pCamera->m_nImageWidth, pCamera->m_nImageHeight);
            // memcpy(pCamera->m_pImgRGBBuffer,(BYTE*)pFrame->pImgBuf,pCamera->m_nImageWidth*pCamera->m_nImageHeight);
			//�����ݽ��д�ֱ������
			int i=0;
			
			//	break;for( i=0;i<pCamera->m_nImageHeight;i++)
			{
				//memcpy(pCamera->m_pImgRGBBuffer+pCamera->m_nImageWidth*i,(BYTE*)pFrame->pImgBuf+pCamera->m_nImageWidth*(pCamera->m_nImageHeight-i-1), pCamera->m_nImageWidth);
				memcpy(pCamera->m_pImgRGBBuffer,(BYTE*)pFrame->pImgBuf, pCamera->m_nImageWidth*pCamera->m_nImageHeight);
			}
			//pCamera->__SaveBMP(pCamera->m_pImgRGBBuffer, pCamera->m_nImageWidth, pCamera->m_nImageHeight);
		}
		catch (...)
		{
			bRet = TRUE;
			pCamera->m_LastErrorInfo.iErrorCode = DCErrorSendBufAdd;
			_tprintf_s(pCamera->m_LastErrorInfo.strErrorDescription, "����ͼ���ڴ��ַʧ��");
			_tprintf_s(pCamera->m_LastErrorInfo.strErrorRemark, "__OnFrameCallbackFun()����");
		}
		
		s_GBSIGNALINFO GrabInfo;
		GrabInfo.Context = pCamera->m_Context;
		GrabInfo.iGrabberTypeSN = pCamera->m_nGrabberType; // �ɼ������ͱ��
		GrabInfo.iGrabberSN  = pCamera->m_nGrabberSN - 1; // �����Ŵ�0��ʼ,��֤���������һ��
		GrabInfo.iErrorCode = GBOK;
		GrabInfo.iFrameCount = pFrame->nFrameID;
		GrabInfo.nTimestamp = pFrame->nTimestamp;
		if (bRet)
		{
			GrabInfo.iErrorCode = pCamera->m_LastErrorInfo.iErrorCode;
			_tprintf_s(GrabInfo.strDescription, pCamera->m_LastErrorInfo.strErrorDescription);
		}
    	pCamera->m_CallBackFunc(&GrabInfo);
	}
}
//----------------------------------------------------------------------------------
/**
\brief  ����bmpͼ��
\param  pImageBuf  ָ��ͼ�񻺳�����ָ��
\param  nImageWidth ͼ���
\param  nImageHeight ͼ���
\return �޷���ֵ
*/
//----------------------------------------------------------------------------------
void CameraBase::__SaveBMP(BYTE * pImageBuf, int64_t nImageWidth, int64_t nImageHeight)
{
	if (!m_bSaveBMP || m_strBMPFolder.GetLength() == 0)
	{
		return;
	}
	
	BITMAPFILEHEADER	bfh	= {0};
	DWORD				dwBytesRead	= 0;
	DWORD               dwImageSize = DWORD(nImageWidth * nImageHeight * 3);
	
	bfh.bfType		= (WORD)'M' << 8 | 'B';			                        //�����ļ�����
	bfh.bfOffBits	= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);	//�����ļ�ͷ��С��ɫ
	bfh.bfSize		= bfh.bfOffBits + dwImageSize;		                    //�ļ���С
	
	SYSTEMTIME system;
	GetLocalTime(&system);
	CString strBMPFileName = MsgComposer(_T("\\%02d%02d%02d%03d.bmp"),
		system.wHour, 
		system.wMinute, 
		system.wSecond, 
		system.wMilliseconds);
	
	CString strBMPFilePath = m_strBMPFolder + strBMPFileName;
	
	HANDLE hFile = ::CreateFile((LPCTSTR)strBMPFilePath,
		GENERIC_WRITE ,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if (hFile != INVALID_HANDLE_VALUE) 
	{
		::WriteFile(hFile, &bfh, sizeof(BITMAPFILEHEADER), &dwBytesRead, NULL );
		::WriteFile(hFile, m_pBmpInfo, sizeof(BITMAPINFOHEADER), &dwBytesRead, NULL );
		::WriteFile(hFile, pImageBuf, dwImageSize, &dwBytesRead, NULL );				
		CloseHandle(hFile);
	}
}


//Զ���豸�¼��ص�������
void GX_STDC CameraBase::OnFeatureCallbackFun(GX_FEATURE_ID_CMD emFeatureID, void* pUserParam)
{
	CameraBase *pCamera = (CameraBase *)(pUserParam);
    if (emFeatureID == GX_INT_EVENT_FRAMESTART_OVERTRIGGER)
	{
		//��ȡ�����������¼���ID���¼�����	
		int64_t nFrameID=0;	
		GXGetInt(pCamera->m_hDevice, GX_INT_EVENT_FRAMESTART_OVERTRIGGER, &nFrameID);

		int64_t nTimestamp=0;
		GXGetInt(pCamera->m_hDevice, GX_INT_EVENT_FRAMESTART_OVERTRIGGER_TIMESTAMP, &nTimestamp);

		//s_GBEVENTINFO EventInfo;
		s_GBSIGNALINFO EventInfo;
		ZeroMemory(&EventInfo,sizeof(s_GBSIGNALINFO));

		EventInfo.Context = pCamera->m_Context;
		EventInfo.iGrabberTypeSN = pCamera->m_nGrabberType; // �ɼ������ͱ��
		EventInfo.iGrabberSN  = pCamera->m_nGrabberSN - 1; // �����Ŵ�0��ʼ,��֤���������һ��
		EventInfo.iErrorCode = GBOK;
		EventInfo.iFrameCount = nFrameID;
		EventInfo.nTimestamp = nTimestamp;

    	pCamera->m_EventCallbackFunc(&EventInfo);
	}
	return;    
}
