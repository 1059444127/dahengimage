// JAICamDH.h: interface for the CJAICamDH class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JAICAMDH_H__423502D3_7E46_4956_A554_CD59E4ADFAC3__INCLUDED_)
#define AFX_JAICAMDH_H__423502D3_7E46_4956_A554_CD59E4ADFAC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DHGrabForJAI.h"
// #include <Jai_Error.h>
// #include <Jai_Types.h>
//#include <Jai_Factory_Dynamic.h>

#include <Jai_Factory.h>

const int MaxFileNameLen = 255;			// ��ʼ���ļ�������󳤶�
const int iMaxCameraNum = 20;			//�ɼ���֧�ֵ���������

#define NODE_NAME_WIDTH         "Width"
#define NODE_NAME_HEIGHT        "Height"
#define NODE_NAME_PIXELFORMAT   "PixelFormat"
#define NODE_NAME_GAIN          "GainRaw"
#define NODE_NAME_EXPOSURE      "ExposureTimeRaw"
#define NODE_NAME_ACQSTART      "AcquisitionStart"
#define NODE_NAME_ACQSTOP       "AcquisitionStop"

typedef struct _JAIInitStruct			// ������ʼ���ṹ
{
	int iGrabberTypeSN;						// �ɼ������ͱ��
	int nGrabberSN;							// �ɼ������
    char strDeviceName[GBMaxTextLen];		// �豸����
	char strDeviceMark[GBMaxTextLen];		// �豸��ʶ
	char strGrabberFile[GBMaxFileNameLen];	// ���ڳ�ʼ�����ļ�
	PGBCALLBACK CallBackFunc;				// �ص�����ָ��
	PVOID Context;							// ���ӵ�лص������Ķ����thisָ��	
}s_JAI_INITSTRUCT;// ������ʼ���ṹ

enum ErrorStruct
{
	ErrorNoFindCam = 0,		// û���ҵ����
	ErrorGrabberSN,			// �����ų����ܹ��ҵ��������
	ErrorGetCamInfo,		// �õ������Ϣʧ��
	ErrorValOverFlow,		// �������
	ErrorOpenCam,			// �����ʧ��
	ErrorInit,				// ��ʼ��ʧ��
	ErrorStartGrab,			// ��ʼ�ɼ�ʧ��
	ErrorStopGrab,			// ֹͣ�ɼ�ʧ��
	ErrorClose,				// �ر�ʧ��
	ErrorSendBufAdd,		// ����ͼ���ڴ��ַʧ��
	ErrorGetParam,			// �Ƿ���ȡ����
	ErrorSetParam,			// �Ƿ����ò���
	ErrorParamDlg,			// ���ò������öԻ���ʧ��
	ErrorReadIniFile		// ��ȡ�����ļ�ʧ��
};

void WINAPI FGCallBackFunc(void* Context,UINT32 wParam,void* lParam); //�ɼ��ص�����

class CJAICamDH: public CGrabber
{
public:
	CJAICamDH();
	virtual ~CJAICamDH();

	FACTORY_HANDLE  m_hFactory;             // Factory Handle
	int8_t m_sCameraID[J_CAMERA_ID_SIZE];	// CameraIDs
	CAM_HANDLE      m_hCam;					// Camera Handles
	THRD_HANDLE     m_hThread;				// Stream handles
	J_tIMAGE_INFO*	m_pImageInfo;			// ����ɼ�ͼ����Ϣ
    NODE_HANDLE     m_hGainNode;            // Handle to "GainRaw" node
    NODE_HANDLE     m_hExposureNode;        // Handle to "ExposureTimeRaw" node


	// ������λ��
	int m_iImageWidth;			//��
	int m_iImageHeight;			//��
	int m_iImageByteCount;		//λ��
	int m_iImageBufferSize;		//�ڴ��С
	BYTE* m_pImageBufferAdd;	//�ڴ��ַ
	int	 m_iGain;
	int  m_iExposureTime;
	int	 m_iMinGain;
	int	 m_iMaxGain;
	int  m_iMinExp;
	int  m_iMaxExp;
	int  m_iTriggerMode;

	// ���������Ϣ
	int m_iGrabberSN;			//������
	int m_iGrabberTypeSN;		//��̬�����ͺ�
	CString m_sInitFile;		//��ʼ���ļ�����
	CString m_sDeviceName;		//�豸����
	CString m_sDeviceMark;		//�豸��ʶ
	PGBCALLBACK m_CallBackFunc;		// �ص�����ָ��
	VOID* m_pContext;			//��ŵ��ó�ʼ��������this����ָ��
	s_GBERRORINFO m_LastErrorInfo;	// ������Ϣ


	int m_iCamCount;			//�������
	int m_iFrameCount;			//�ɼ�����
	BOOL m_bInitSucceed;		//�Ƿ��ʼ���ɹ�
	BOOL m_bCameraActive;		//����Ƿ����ڲɼ�
	
	// �ص�����
	void StreamCBFunc1(J_tIMAGE_INFO * pAqImageInfo);


	// ��ʼ�����
	BOOL InitCamera();

	// ��ʼ����������
	BOOL InitFactory();


	//��ʼ��
	BOOL Init(const s_GBINITSTRUCT* pInitParam);

	//�ر�
	BOOL Close();

	//��ʼ�ɼ�
	BOOL StartGrab();

	//ֹͣ�ɼ�
	BOOL StopGrab();

	//����ɼ�
	BOOL Snapshot();


	//////////////////////////////////////////////////////////////////////////
	//	�����������
	//  int Param:
	// 	0:GBImageWidth,				//ͼ���� (��λ:�ֽ�)
	// 	1:GBImageHeight,			//ͼ��߶� (��λ:�ֽ�)
	// 	2:GBImagePixelSize,			//ͼ�����ش�С (��λ:�ֽ�)
	// 	3:GBImageBufferSize,		//ͼ�񻺳�����С (��λ:�ֽ�)
	// 	4:GBImageBufferAddr,		//ͼ�񻺳�����ַ
	// 	5:GBGrabberTypeSN,			//�ɼ������ͱ��
	
	// 	6:GBColor,					//�������  �ڰ׺Ͳ�ɫ 
	// 	7:GBBrightness,				//������ȴ�С
	// 	8:GBShutter,				//�������ֵ
	// 	9:GBGain,					//��������С
	// 	10:GBU,						//���Uֵ
	// 	11:GBV,						//���Vֵ
	// 	12:GBTemperature			//����¶�
	// 	13:Multishot				//�趨Multishot�ɼ�����
	//���ò���
	BOOL SetParamInt(GBParamID Param, int nInputVal);
	
	//�õ�����
	BOOL GetParamInt(GBParamID Param, int &nOutputVal);
	//////////////////////////////////////////////////////////////////////////

	//���ò���(δ��)
	BOOL SetParamfloat(int Param, float nInputVal);
	
	//�õ�����(δ��)
	BOOL GetParamfloat(int Param, float &nOutputVal);

	//���ò����Ի���
	void CallParamDialog();

	//�õ�������Ϣ
	void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo);

	//////////////////////////////////////////////////////////////////////////
	//	�����������
	// 	0:AVTTriggerOff = 0,	//�ⴥ���ر�//�ڲ�����,�����ɼ�
	// 	1:AVTTriggerOn,	    	//�����ش���//�ⴥ������
	// 	2:AVTTriggerMode2,		//�½��ش���
	// 	3:AVTTriggerMode3,		//�ߵ�ƽ����
	// 	4:AVTTriggerMode4,		//�͵�ƽ����
	// 	5:AVTTriggerMode5,		//���ģʽ�����ش���
	// 	6:AVTTriggerMode6,		//���ģʽ�½��ش���	

	BOOL GetTriggerParam(int &ParamValOutput);

	BOOL SetTriggerParam(int ParamValInput);
	//////////////////////////////////////////////////////////////////////////


	/************************************************************************/
	/*  ���ܣ���òɼ���������
	/*  �����RECT    �ɼ����ڴ�С
	/*	���أ���ȷ:TRUE   ����:FALSE
	/************************************************************************/
	BOOL GetAOI(RECT &rtAOI);

	/************************************************************************/
	/*  ���ܣ����òɼ���������
	/*  ���룺RECT    �ɼ����ڴ�С
	/*	���أ���ȷ:TRUE   ����:FALSE
	/************************************************************************/
	BOOL SetAOI(RECT rtAOI);

	/************************************************************************/
	/*  ���ܣ������������к�
	/*  �����nGaidHigh  ����λ
	/*	�����nGaidLow   �;�λ
	/*	���أ���ȷ:TRUE   ����:FALSE
	/************************************************************************/
	BOOL GetSystemInfo(CString &sOutputInfo);

	/************************************************************************/
	/*  ���ܣ����������λ������������������쳣�޻ص������ʹ�ã�
	/*  ��������������
	/*  ��һ��:����ڲ�����Ĭ������;
	/*  �ڶ���: bus resets will occur;
	/*	������:	The FPGA will be rebooted; 
	/*	���Ĳ�:����INI�����ļ�����;
	/*  ���룺
	/*	���أ���ȷ:TRUE   ����:FALSE
	/************************************************************************/
	BOOL Reset();

	/************************************************************************/
	/*  ���ܣ�����Ĭ��INI�����ļ�
	/*  ���룺sInitFile ��ȷ��ʽ��D:\\Run\\CameraParm.ini
	/*	���أ���ȷ:TRUE   ����:FALSE
	/************************************************************************/
	BOOL CreateDefaultConfigurationFile(CString &sInitFile);


	//���������ļ�
	BOOL SaveToConfigurationFile(CString &sInitFile);

};

#endif // !defined(AFX_JAICAMDH_H__423502D3_7E46_4956_A554_CD59E4ADFAC3__INCLUDED_)
