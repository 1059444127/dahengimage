// JAICamDH.cpp: implementation of the CJAICamDH class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JAIGrabber.h"
#include "JAICamDH.h"
#include "DlgCameraSetting.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJAICamDH::CJAICamDH()
{
	// ������λ��
	m_iImageWidth=1392;			//��
	m_iImageHeight=1040;			//��
	m_iImageByteCount=1;		//λ��
	m_iImageBufferSize=1392*1040*3;		//�ڴ��С
	m_pImageBufferAdd=NULL;	//�ڴ��ַ
	m_iGain = 0;
	m_iExposureTime = 0;
	m_iMinGain = 0;
	m_iMaxGain = 0;
	m_iMinExp = 0;
	m_iMaxExp = 0;
	m_iTriggerMode = 0;

	
	// ���������Ϣ
	m_iGrabberSN=0;			//������
	m_iGrabberTypeSN=0;		//��̬�����ͺ�
	m_sInitFile="";			//��ʼ���ļ�����
	m_sDeviceName="";		//�豸����
	m_sDeviceMark="";		//�豸��ʶ
	m_pContext=NULL;		//��ŵ��ó�ʼ��������this����ָ��
	
	m_iCamCount=0;			//�������
	m_iFrameCount=0;
	m_bInitSucceed=FALSE;	//�Ƿ��ʼ���ɹ�
	m_bCameraActive=FALSE;	//��ʼ��û�вɼ�
	m_hGainNode = NULL;
	m_hExposureNode = NULL;

	m_pImageInfo = NULL;
	m_pImageInfo = (J_tIMAGE_INFO*)new BYTE[sizeof(J_tIMAGE_INFO)];
	m_pImageBufferAdd = new BYTE[1500*1500*3];

}

CJAICamDH::~CJAICamDH()
{
	if(m_pImageInfo!=NULL)
	{
		delete m_pImageInfo;
		m_pImageInfo = NULL;
	}

	if(m_pImageBufferAdd!=NULL)
	{
		delete m_pImageBufferAdd;
		m_pImageBufferAdd = NULL;
	}
}

//��ʼ��
BOOL CJAICamDH::Init(const s_GBINITSTRUCT* pInitParam)
{
	//�����ʼ��
	s_JAI_INITSTRUCT InitCardParam;
	InitCardParam.iGrabberTypeSN = pInitParam->iGrabberTypeSN;  // �ɼ������ͱ��
	InitCardParam.nGrabberSN = pInitParam->nGrabberSN;          // �ɼ������
	sprintf(InitCardParam.strDeviceName, pInitParam->strDeviceName);  // �豸����
	sprintf(InitCardParam.strDeviceMark, pInitParam->strDeviceMark); // �豸��ʶ
	sprintf(InitCardParam.strGrabberFile, pInitParam->strGrabberFile); // ���ڳ�ʼ�����ļ�
	InitCardParam.CallBackFunc = pInitParam->CallBackFunc;
	InitCardParam.Context = pInitParam->Context;
	
	// 
	m_iGrabberSN = pInitParam->nGrabberSN;
	m_iGrabberTypeSN = pInitParam->iGrabberTypeSN;
	m_sDeviceMark.Format("%s",pInitParam->strDeviceMark);
	m_sDeviceName.Format("%s",pInitParam->strDeviceName);
	m_sInitFile.Format("%s",pInitParam->strGrabberFile);
	m_pContext = pInitParam->Context;
	m_CallBackFunc = pInitParam->CallBackFunc;		
	
	// Ѱ�������ļ�
	CFileFind find;
	if(FALSE==find.FindFile(m_sInitFile))
	{
		m_bInitSucceed = FALSE;
		m_LastErrorInfo.nErrorCode = ErrorInit; 
		sprintf(m_LastErrorInfo.strErrorDescription, "��������ļ�����ָ��·����");
		sprintf(m_LastErrorInfo.strErrorRemark, "Init()����");
		return FALSE;
	}


	if(FALSE==InitFactory())
	{
		return FALSE;
	}


	// ���豸SN��������������Ƴ�
	if(pInitParam->nGrabberSN>=m_iCamCount)
	{
		m_bInitSucceed = FALSE;
		m_LastErrorInfo.nErrorCode = ErrorInit; 
		sprintf(m_LastErrorInfo.strErrorDescription, "�����Ŵ���ʵ���������");
		sprintf(m_LastErrorInfo.strErrorRemark, "Init()����");
		return FALSE;
	}
	else if(pInitParam->nGrabberSN<0)
	{
		m_bInitSucceed = FALSE;
		m_LastErrorInfo.nErrorCode = ErrorInit; 
		sprintf(m_LastErrorInfo.strErrorDescription, "�����Ų���С��0");
		sprintf(m_LastErrorInfo.strErrorRemark, "Init()����");
		return FALSE;
	}

	// ��ʼ�����
	InitCamera();

	m_bInitSucceed = TRUE;

	return TRUE;
}

//�ر�
BOOL CJAICamDH::Close()
{

	J_STATUS_TYPE retval;

	if (m_bInitSucceed == TRUE)// �����ʼ���ɹ�
	{		
		if (m_bCameraActive) // ������ڲɼ�״̬
		{
			StopGrab();
		}	
		
		if(NULL!=m_hThread)
		{
			retval = J_Image_CloseStream(m_hThread);
			m_hThread = NULL;
		}

		if(J_ST_SUCCESS!=retval)
		{
			m_LastErrorInfo.nErrorCode = ErrorClose;  // ����������� 
			sprintf(m_LastErrorInfo.strErrorDescription, "Close Stream Failed!");
			sprintf(m_LastErrorInfo.strErrorRemark, "Close()����");
			return FALSE;
		}

	}

	if (m_hCam)
	{
		// Close camera
		retval = J_Camera_Close(m_hCam);
		if(J_ST_SUCCESS!=retval)
		{
			m_LastErrorInfo.nErrorCode = ErrorClose;  // ����������� 
			sprintf(m_LastErrorInfo.strErrorDescription, "Close Camera Failed!");
			sprintf(m_LastErrorInfo.strErrorRemark, "Close()����");
			return FALSE;
		}

		m_hCam = NULL;
		TRACE("Closed camera \n");
    }

	if (m_hFactory)
    {
        // Close factory
        retval=J_Factory_Close(m_hFactory);
		if(J_ST_SUCCESS!=retval)
		{
			m_LastErrorInfo.nErrorCode = ErrorClose;  // ����������� 
			sprintf(m_LastErrorInfo.strErrorDescription, "Close factory Failed!");
			sprintf(m_LastErrorInfo.strErrorRemark, "Close()����");
			return FALSE;
		}

        m_hFactory = NULL;
        TRACE("Closed factory\n");
    }

	return TRUE;
}

//��ʼ�ɼ�
BOOL CJAICamDH::StartGrab()
{
	if(FALSE==m_bInitSucceed || NULL==m_hCam)
	{
		m_LastErrorInfo.nErrorCode = ErrorStartGrab;  // ����������� 
		sprintf(m_LastErrorInfo.strErrorDescription, "���û�г�ʼ��");
		sprintf(m_LastErrorInfo.strErrorRemark, "StartGrab()����");
		return FALSE;
	}

	if (m_bCameraActive) //���������ڲɼ�����ֹͣ�ɼ�
	{
		StopGrab();
	}
	
	if (m_bCameraActive) //�Ƿ����ڲɼ�
	{
		m_LastErrorInfo.nErrorCode = ErrorStartGrab;  // ����������� 
		sprintf(m_LastErrorInfo.strErrorDescription, "����Ѿ��ڲɼ�");
		sprintf(m_LastErrorInfo.strErrorRemark, "StartGrab()����");
		return FALSE;
	}


	// Start Acquision
	if(FALSE==m_bCameraActive)
	{
		J_STATUS_TYPE retval;
		retval = J_Camera_ExecuteCommand(m_hCam, NODE_NAME_ACQSTART);

		if(J_ST_SUCCESS!=retval)
		{
			m_LastErrorInfo.nErrorCode = ErrorStartGrab;  // ����������� 
			sprintf(m_LastErrorInfo.strErrorDescription, "start grab failed!");
			sprintf(m_LastErrorInfo.strErrorRemark, "StartGrab()����");
			return FALSE;
		}
		else
		{
			m_bCameraActive = TRUE;
		}
	}


	return TRUE;
}

//ֹͣ�ɼ�
BOOL CJAICamDH::StopGrab()
{

	if(!m_bInitSucceed || NULL==m_hCam)
	{
		m_LastErrorInfo.nErrorCode = ErrorStopGrab;  // ����������� 
		sprintf(m_LastErrorInfo.strErrorDescription, "���û�г�ʼ��");
		sprintf(m_LastErrorInfo.strErrorRemark, "StopGrab()����");
		return FALSE;
	}

	if (!m_bCameraActive)// �Ƿ������ɼ�
	{
		m_LastErrorInfo.nErrorCode = ErrorStopGrab;  // ����������� 
		sprintf(m_LastErrorInfo.strErrorDescription, "�����δ���ڲɼ�״̬");
		sprintf(m_LastErrorInfo.strErrorRemark, "StopGrab()����");
		return FALSE;
	}


	J_STATUS_TYPE retval;
	retval = J_Camera_ExecuteCommand(m_hCam, NODE_NAME_ACQSTOP);

	if(J_ST_SUCCESS!=retval)
	{
		m_LastErrorInfo.nErrorCode = ErrorStopGrab;  // ����������� 
		sprintf(m_LastErrorInfo.strErrorDescription, "stop grab failed!");
		sprintf(m_LastErrorInfo.strErrorRemark, "StopGrab()����");
		return FALSE;
	}
	else
	{
		m_bCameraActive = FALSE;
	}

	return TRUE;
}
	
//�õ�����
BOOL CJAICamDH::GetParamInt(GBParamID Param, int &nOutputVal)
{
	J_STATUS_TYPE retval;

	switch(Param)
	{
	case 0://GBImageWidth:
		nOutputVal = m_iImageWidth;
		break;
	case 1://GBImageHeight:
		nOutputVal = m_iImageHeight;
		break;
	case 2://GBImagePixelSize:
		nOutputVal = m_iImageByteCount;
		break;
	case 3://GBImageBufferSize:
		nOutputVal = m_iImageWidth * m_iImageHeight * m_iImageByteCount;
		break;
	case 4://GBImageBufferAddr:
		nOutputVal = (int)m_pImageBufferAdd;
		break;
	case GBIsColorFilter://GBColor,					//�������  �ڰ׺Ͳ�ɫ 
		nOutputVal = 1; // �Ƿ��ɫ��� 
		break;
	case GBExposureTime://GBBrightness,				//������ȴ�С
		UINT32 OutputVal;
		OutputVal = 0;		
		break;
	case GBShutter://GBShutter,				//�������ֵ
		// Get ExposureTimeRaw Node
		retval = J_Camera_GetNodeByName(m_hCam, NODE_NAME_EXPOSURE, &m_hExposureNode);
		if (retval != J_ST_SUCCESS) 
		{
			m_bInitSucceed = FALSE;
			m_LastErrorInfo.nErrorCode = ErrorInit; 
			sprintf(m_LastErrorInfo.strErrorDescription,"GetParamInt(shutter)ʧ��failed!");
			sprintf(m_LastErrorInfo.strErrorRemark, "GetParamInt()����");
			return FALSE;
		}
		else
		{
			int64_t int64Val;
			retval = J_Node_GetValueInt64(m_hExposureNode, TRUE, &int64Val);
			m_iExposureTime  = (int)int64Val;
			nOutputVal = (int)int64Val;
		}
		break;
	case GBGain://GBGain,					//��������С
		// Get Gain Node
		retval = J_Camera_GetNodeByName(m_hCam, NODE_NAME_GAIN, &m_hGainNode);
		if (retval != J_ST_SUCCESS) 
		{
			m_bInitSucceed = FALSE;
			m_LastErrorInfo.nErrorCode = ErrorInit; 
			sprintf(m_LastErrorInfo.strErrorDescription,"Get gain failed!");
			sprintf(m_LastErrorInfo.strErrorRemark, "GetParamInt()����");
			return FALSE;
		}
		else
		{
			// Get/Set Value
			int64_t int64Val;
			retval = J_Node_GetValueInt64(m_hGainNode, TRUE, &int64Val);
			m_iGain  = (int)int64Val;
			nOutputVal = (int)int64Val;
		}

		break;

		//////////////////////////////////////////////////////////////////////////
	default:
		m_LastErrorInfo.nErrorCode = ErrorGetParam;
		sprintf(m_LastErrorInfo.strErrorDescription, "�õ���������");
		sprintf(m_LastErrorInfo.strErrorRemark, "GetParamInt()����");

		return FALSE;
	}


	return TRUE;
}
	
//���ò���
BOOL CJAICamDH::SetParamInt(GBParamID Param, int nInputVal)
{

	J_STATUS_TYPE retval;

	switch(Param)
	{
	case GBShutter://GBShutter,				//�������ֵ
		// Get ExposureTimeRaw Node
		retval = J_Camera_GetNodeByName(m_hCam, NODE_NAME_EXPOSURE, &m_hExposureNode);
		if (retval != J_ST_SUCCESS) 
		{
			m_bInitSucceed = FALSE;
			m_LastErrorInfo.nErrorCode = ErrorInit; 
			sprintf(m_LastErrorInfo.strErrorDescription,"SetParamInt(shutter) failed!");
			sprintf(m_LastErrorInfo.strErrorRemark, "SetParamInt()����");
			return FALSE;
		}
		else
		{
			retval = J_Node_SetValueInt64(m_hExposureNode, TRUE, nInputVal);
			if (retval != J_ST_SUCCESS) 
			{
				m_bInitSucceed = FALSE;
				m_LastErrorInfo.nErrorCode = ErrorInit; 
				sprintf(m_LastErrorInfo.strErrorDescription,"SetParamInt(shutter)2 failed!");
				sprintf(m_LastErrorInfo.strErrorRemark, "SetParamInt()����");
				return FALSE;
			}

			m_iExposureTime = nInputVal;
		}
		break;
	case GBGain://GBGain,					//��������С
		retval = J_Camera_GetNodeByName(m_hCam, NODE_NAME_GAIN, &m_hGainNode);
		if (retval != J_ST_SUCCESS) 
		{
			m_bInitSucceed = FALSE;
			m_LastErrorInfo.nErrorCode = ErrorInit; 
			sprintf(m_LastErrorInfo.strErrorDescription,"Get gain failed!");
			sprintf(m_LastErrorInfo.strErrorRemark, "SetParamInt()����");
			return FALSE;
		}
		else
		{
			// Get/Set Value
			retval = J_Node_SetValueInt64(m_hGainNode, TRUE, nInputVal);

			if (retval != J_ST_SUCCESS) 
			{
				m_bInitSucceed = FALSE;
				m_LastErrorInfo.nErrorCode = ErrorInit; 
				sprintf(m_LastErrorInfo.strErrorDescription,"Get gain2 failed!");
				sprintf(m_LastErrorInfo.strErrorRemark, "SetParamInt()����");
				return FALSE;
			}
			m_iGain = nInputVal;

		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}


//�õ�����
BOOL CJAICamDH::GetParamfloat(int Param, float &nOutputVal)
{
	return TRUE;
}

//���ò���
BOOL CJAICamDH::SetParamfloat(int Param, float nInputVal)
{

	return TRUE;
}

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

//���ò����Ի���
void CJAICamDH::CallParamDialog()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#ifdef _DEBUG
	// Workaround for ASSERT in WINCORE.CPP 884 (CWnd::AssertValid)
	tempRoutingFrame rframe;
#endif

	CDlgCameraSetting dlg;
	dlg.m_iMaxExp = m_iMaxExp;
	dlg.m_iMinExp = m_iMinExp;
	dlg.m_iMaxGain = m_iMaxGain;
	dlg.m_iMinGain = m_iMinGain;
	dlg.m_iExposureTime = m_iExposureTime;
	dlg.m_iGain = m_iGain;
	dlg.SetOwner(this,m_sInitFile);
	int iRet = dlg.DoModal();

	delete dlg;
}

//�õ�������Ϣ
void CJAICamDH::GetLastErrorInfo(s_GBERRORINFO *pErrorInfo)
{
	if (pErrorInfo != NULL)
	{
		memcpy(pErrorInfo,&m_LastErrorInfo,sizeof(s_GBERRORINFO));
	}	
}	

BOOL CJAICamDH::GetTriggerParam(int &ParamValOutput)
{
	BOOL bRet = FALSE;
	// �������Ӧ����
	return bRet;
}

BOOL CJAICamDH::SetTriggerParam(int ParamValInput)
{
	BOOL bRet = FALSE;
	return bRet;
}
BOOL CJAICamDH::GetSystemInfo(CString &sOutputInfo)
{
	BOOL bRet = FALSE;
	return bRet;
}

BOOL CJAICamDH::SetAOI(RECT rtAOI)
{
	BOOL bRet = FALSE;
	return bRet;
}

BOOL CJAICamDH::GetAOI(RECT &rtAOI)
{
	BOOL bRet = FALSE;
	return bRet;
}


BOOL CJAICamDH::Reset()
{
	BOOL bRet = FALSE;
	return bRet;
}

BOOL CJAICamDH::CreateDefaultConfigurationFile(CString &sInitFile)
{
	BOOL bRet = FALSE;
	return bRet;
}

BOOL CJAICamDH::SaveToConfigurationFile(CString &sInitFile)
{
	BOOL bRet = FALSE;

	return bRet;
}
//////////////////////////////////////////////////////////////////////////
// DirectShowר�нӿ� 
// ��֡�ɼ�
BOOL CJAICamDH::Snapshot()
{
	return TRUE;
}

BOOL CJAICamDH::InitFactory()
{
	J_STATUS_TYPE   retval;
    uint32_t        iSize;
    uint32_t        iNumDev;
	bool8_t         bHasChange;


	// Open factory
    retval = J_Factory_Open("" , &m_hFactory);
    if (retval != J_ST_SUCCESS)
    {
		m_bInitSucceed = FALSE;
		m_LastErrorInfo.nErrorCode = ErrorInit; 
		sprintf(m_LastErrorInfo.strErrorDescription, "Open factory failed!");
		sprintf(m_LastErrorInfo.strErrorRemark, "Init()����");
		return FALSE;
    }
    TRACE("Opening factory succeeded\n");
	
 
	
	// Update camera list
    retval = J_Factory_UpdateCameraList(m_hFactory, &bHasChange);
    if (retval != J_ST_SUCCESS)
    {
		m_bInitSucceed = FALSE;
		m_LastErrorInfo.nErrorCode = ErrorInit; 
		sprintf(m_LastErrorInfo.strErrorDescription, "Update Camera list failed!");
		sprintf(m_LastErrorInfo.strErrorRemark, "Init()����");
		return FALSE;
    }
    TRACE("Updating camera list succeeded\n");
	
    // Get the number of Cameras
    retval = J_Factory_GetNumOfCameras(m_hFactory, &iNumDev);
    if (retval != J_ST_SUCCESS)
    {
		m_bInitSucceed = FALSE;
		m_LastErrorInfo.nErrorCode = ErrorInit; 
		sprintf(m_LastErrorInfo.strErrorDescription, "Get Number of Cameras failed!");
		sprintf(m_LastErrorInfo.strErrorRemark, "Init()����");
		return FALSE;
    }
	
    if (iNumDev == 0)
    {
		m_bInitSucceed = FALSE;
		m_LastErrorInfo.nErrorCode = ErrorInit; 
		sprintf(m_LastErrorInfo.strErrorDescription, "no camera was found!");
		sprintf(m_LastErrorInfo.strErrorRemark, "Init()����");
		return FALSE;
    }
	else
	{
		m_iCamCount = iNumDev;
	}
	
    TRACE("%d cameras were found\n", iNumDev);
	
	CString str1;
	str1.Format("���%d ��ʼ���������:%d",m_iGrabberSN, m_iCamCount);
// 	AfxMessageBox(str1);

	
	// Get camera IDs
	iSize = J_CAMERA_ID_SIZE;


	BOOL bFind = FALSE;

	for(int i=0;i<m_iCamCount;i++)
	{
		m_sCameraID[0]=0;
		iSize = J_CAMERA_ID_SIZE;

		retval = J_Factory_GetCameraIDByIndex(m_hFactory, i, m_sCameraID, &iSize);
		if (retval != J_ST_SUCCESS)
		{
			m_bInitSucceed = FALSE;
			m_LastErrorInfo.nErrorCode = ErrorInit; 
			sprintf(m_LastErrorInfo.strErrorDescription, "Could not get the camera ID!");
			sprintf(m_LastErrorInfo.strErrorRemark, "Init()����");		
			return FALSE;
		}
		TRACE("Camera ID: %s\n", m_sCameraID);
		
		// ��֤sn��
		int8_t sSerialNumber[J_CAMERA_INFO_SIZE];
		J_Factory_GetCameraInfo(m_hFactory,m_sCameraID,CAM_INFO_SERIALNUMBER,sSerialNumber,&iSize);
		
		CString str;
		str.Format("673537%s0609",sSerialNumber);
		str1.Format("���%d ���:%d, ���к�:%s",m_iGrabberSN, i+1, sSerialNumber);
// 		AfxMessageBox(str1);
		
		if(0==str.Compare(m_sDeviceName))
		{
			bFind = TRUE;
			break;
		}
	}

	str1.Format("���%d���������к�Ϊ:%s", m_iGrabberSN, m_sDeviceName);
// 	AfxMessageBox(str1);

	if(TRUE!=bFind)
	{
		m_bInitSucceed = FALSE;
		m_LastErrorInfo.nErrorCode = ErrorInit; 
		sprintf(m_LastErrorInfo.strErrorDescription, "DeviceName is not correct!");
		sprintf(m_LastErrorInfo.strErrorRemark, "Init()����");
		return FALSE;
	}

	return TRUE;
}

BOOL CJAICamDH::InitCamera()
{

	J_STATUS_TYPE   retval;


	// Open camera
	retval = J_Camera_Open(m_hFactory, m_sCameraID, &m_hCam);
	if (retval != J_ST_SUCCESS)
	{
		m_bInitSucceed = FALSE;
		m_LastErrorInfo.nErrorCode = ErrorInit; 
		sprintf(m_LastErrorInfo.strErrorDescription, "Open camera failed!");
		sprintf(m_LastErrorInfo.strErrorRemark, "Init()����");
		return FALSE;
	}


	// Load camera configuration files
	retval = J_Camera_LoadSettings(m_hCam,m_sInitFile);
//	retval = J_Camera_LoadSettings(m_hCam,m_sInitFile,LOAD_FORCE_WRITE);
// 	retval = J_Camera_LoadSettings(m_hCam,m_sInitFile,LOAD_VALIDATE_ONLY);

	if(retval==J_ST_VALIDATION_ERROR || retval==J_ST_VALIDATION_WARNING)
	{
		uint32_t BufferSize = 0;
		retval = J_Camera_GetSettingsValidationErrorInfo(m_hCam,NULL,&BufferSize);

		if(retval==J_ST_SUCCESS)
		{
			// Allocate enough room for the info string!
			int8_t *buffer = (int8_t *)malloc(BufferSize);
			// And now we get the actual erro information
			retval = J_Camera_GetSettingsValidationErrorInfo(m_hCam, buffer, &BufferSize);
			OutputDebugStringA(buffer);

			CString str;
			str.Format("%s",buffer);
			m_bInitSucceed = FALSE;
			m_LastErrorInfo.nErrorCode = ErrorInit; 
			sprintf(m_LastErrorInfo.strErrorDescription,str);
			sprintf(m_LastErrorInfo.strErrorRemark, "Init()����");

			// Remember to free the buffer again
			free(buffer);

			return FALSE;
		}

		m_bInitSucceed = FALSE;
		m_LastErrorInfo.nErrorCode = ErrorInit; 
		sprintf(m_LastErrorInfo.strErrorDescription,"Load setting failed!");
		sprintf(m_LastErrorInfo.strErrorRemark, "Init()����");
		return FALSE;

	}

	SIZE	ViewSize;
	int     bpp = 0;
    int64_t int64Val;
    int64_t pixelFormat;

	// 
	if(m_hCam)
	{
		// Get Width from the camera
		
		int8_t x1[255];
		uint32_t x2;
		retval = J_Camera_GetValueString(m_hCam,"PartialScan",x1,&x2);
		retval = J_Camera_GetValueInt64(m_hCam, NODE_NAME_WIDTH, &int64Val);
		if (retval != J_ST_SUCCESS) 
		{
			m_bInitSucceed = FALSE;
			m_LastErrorInfo.nErrorCode = ErrorInit; 
			sprintf(m_LastErrorInfo.strErrorDescription,"Get Width Failed!");
			sprintf(m_LastErrorInfo.strErrorRemark, "Init()����");
			return FALSE;
		}
		else
		{
			ViewSize.cx = (LONG)int64Val;     // Set window size cx
			m_iImageWidth = (int)int64Val;
		}

		
		// Get Height from the camera
		retval = J_Camera_GetValueInt64(m_hCam, NODE_NAME_HEIGHT, &int64Val);
		if (retval != J_ST_SUCCESS) 
		{
			m_bInitSucceed = FALSE;
			m_LastErrorInfo.nErrorCode = ErrorInit; 
			sprintf(m_LastErrorInfo.strErrorDescription,"Get height failed");
			sprintf(m_LastErrorInfo.strErrorRemark, "Init()����");
			return FALSE;
		}
		else
		{
			ViewSize.cy = (LONG)int64Val;     // Set window size cy
			m_iImageHeight = (int)int64Val;
		}
		
		// Get pixelformat from the camera
		retval = J_Camera_GetValueInt64(m_hCam, NODE_NAME_PIXELFORMAT, &int64Val);
		if (retval != J_ST_SUCCESS) 
		{
			m_bInitSucceed = FALSE;
			m_LastErrorInfo.nErrorCode = ErrorInit; 
			sprintf(m_LastErrorInfo.strErrorDescription,"Get pixformat failed!");
			sprintf(m_LastErrorInfo.strErrorRemark, "Init()����");
			return FALSE;
		}
		else
		{
			pixelFormat = int64Val;
			m_iImageByteCount = 1;
		}

		m_iImageBufferSize = m_iImageWidth*m_iImageHeight*m_iImageByteCount;
		
		// Calculate number of bits (not bytes) per pixel using macro
		bpp = J_BitsPerPixel(pixelFormat);

		// Get Gain Node
		retval = J_Camera_GetNodeByName(m_hCam, NODE_NAME_GAIN, &m_hGainNode);
		if (retval != J_ST_SUCCESS) 
		{
			m_bInitSucceed = FALSE;
			m_LastErrorInfo.nErrorCode = ErrorInit; 
			sprintf(m_LastErrorInfo.strErrorDescription,"Get gain failed!");
			sprintf(m_LastErrorInfo.strErrorRemark, "Init()����");
			return FALSE;
		}
		else
		{
			// Get/Set Value
			retval = J_Node_GetValueInt64(m_hGainNode, TRUE, &int64Val);
			m_iGain  = (int)int64Val;

			retval = J_Node_GetMinInt64(m_hGainNode, &int64Val);
			m_iMinGain = (int)int64Val;

			retval = J_Node_GetMaxInt64(m_hGainNode, &int64Val);
			m_iMaxGain = (int)int64Val;


		}

		// Get ExposureTimeRaw Node
		retval = J_Camera_GetNodeByName(m_hCam, NODE_NAME_EXPOSURE, &m_hExposureNode);
		if (retval != J_ST_SUCCESS) 
		{
			m_bInitSucceed = FALSE;
			m_LastErrorInfo.nErrorCode = ErrorInit; 
			sprintf(m_LastErrorInfo.strErrorDescription,"Get exposure time failed!");
			sprintf(m_LastErrorInfo.strErrorRemark, "Init()����");
			return FALSE;
		}
		else
		{
			// Get/Set Value
			retval = J_Node_GetValueInt64(m_hExposureNode, TRUE, &int64Val);
			m_iExposureTime  = (int)int64Val;

			retval = J_Node_GetMinInt64(m_hExposureNode, &int64Val);
			m_iMinExp = (int)int64Val;

			retval = J_Node_GetMaxInt64(m_hExposureNode, &int64Val);
			m_iMaxExp = (int)int64Val;
		}


		retval = J_Image_OpenStream(m_hCam, 0, reinterpret_cast<J_IMG_CALLBACK_OBJECT>(this), reinterpret_cast<J_IMG_CALLBACK_FUNCTION>(&CJAICamDH::StreamCBFunc1), &m_hThread, (ViewSize.cx*ViewSize.cy*bpp)/8);
		if (retval != J_ST_SUCCESS) 
		{
			m_bInitSucceed = FALSE;
			m_LastErrorInfo.nErrorCode = ErrorInit; 
			sprintf(m_LastErrorInfo.strErrorDescription,"Could not open stream!");
			sprintf(m_LastErrorInfo.strErrorRemark, "Init()����");
			return FALSE;
		}
		TRACE("Opening stream succeeded\n");

	}

	return TRUE;
}

void CJAICamDH::StreamCBFunc1(J_tIMAGE_INFO * pAqImageInfo)
{
	if(m_hCam)
	{
		m_iFrameCount++;
		s_GBSIGNALINFO info;
		info.nGrabberSN = m_iGrabberSN;
		info.iGrabberTypeSN = m_iGrabberTypeSN;
		info.Context = m_pContext;
		info.nFrameCount = m_iFrameCount;
		info.nErrorCode = 0;
		info.iReserve1 = 0;
		info.iReserve2 = 0;
		sprintf(info.strDescription,"����");

		memcpy(m_pImageBufferAdd,pAqImageInfo->pImageBuffer,m_iImageWidth*m_iImageHeight);
		
		m_CallBackFunc(&info);
	}
}


