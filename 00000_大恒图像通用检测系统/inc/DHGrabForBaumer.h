/************************************************************************/
/* Copyright (c) 2012, �������ͼ���Ӿ����޹�˾�Ӿ�ϵͳ��
/* All rights reserved.
/*
/* �ļ����ƣ� DHGrabberForBaumer.h
/* ժҪ�� �����ɼ�����(Baumerǧ�������)
/* �����̬�������Baumerǧ���������
/* Ŀǰ�Ѿ���������ͺţ�
/* DXG04h
/*���ù��̣�(������ù��̲ο�����)
/*
/* ��ǰ�汾�� 3.0.0.1 beta1
/* �޸��ߣ� ���ǿ
/* �޸�����ժҪ������ͷ�ļ�ע��
/* ������ڣ� 2012��6��11��
/************************************************************************/

// #if !defined(DHGrabber_h_)
// #define DHGrabber_h_
// 
// const int GBMaxFileNameLen = 255;
// const int GBMaxTextLen = 255;
// 
// typedef struct _GBSignalInfoStruct//�ص���Ϣ�ṹ
// {
// 	PVOID Context;								// ���ӵ�лص������Ķ����thisָ��
// 	int iGrabberTypeSN;							// �ɼ������ͱ��
// 	int nGrabberSN;								// �ɼ������
// 	int nErrorCode;								// ������루0��������
// 	int nFrameCount;							// �Ѳɼ���֡����
// 	char strDescription[GBMaxTextLen];			// ������Ϣ
// 	int iReserve1;								// �����ֶ�1
// 	int iReserve2;								// �����ֶ�2	
// }s_GBSIGNALINFO;//�ص���Ϣ�ṹ
// 
// //�ص�����ָ������
// typedef void (WINAPI *PGBCALLBACK)(const s_GBSIGNALINFO* SignalInfo);
// 
// typedef struct _GBInitStruct//�ɼ�����ʼ���ṹ
// {
// 	int iGrabberTypeSN;						// �ɼ������ͱ��
// 	int nGrabberSN;							// �ɼ������
//     char strDeviceName[GBMaxTextLen];		// �豸����
// 	char strDeviceMark[GBMaxTextLen];		// �豸��ʶ  ���ڱ�ʶ���ӵ������˱�ʶһ��
// 	char strGrabberFile[GBMaxFileNameLen];	// ���ڳ�ʼ�����ļ�
// 	PGBCALLBACK CallBackFunc;				// �ص�����ָ��
// 	PVOID Context;							// ���ӵ�лص������Ķ����thisָ��	
// 	int iReserve1;							// �����ֶ�1
// 	int iReserve2;							// �����ֶ�2
// }s_GBINITSTRUCT;//�ɼ�����ʼ���ṹ
// 
// typedef struct _GBErrorInfoStruct//������Ϣ�ṹ
// {
//    	int nErrorCode;							//������루0��������
// 	char strErrorDescription[GBMaxTextLen];	//��������
// 	char strErrorRemark[GBMaxTextLen];		//������Ϣ
// }s_GBERRORINFO;//������Ϣ�ṹ
// 
// 
// //�ɼ���
// class CGrabber
// {
// //����
// public:
// 	CGrabber()
// 	{
// 	}
// 	virtual ~CGrabber()
// 	{
// 	}
// 	
// 	//��ʼ��
// 	virtual BOOL Init(const s_GBINITSTRUCT* pInitParam) = 0;
// 
// 	//�ر�
// 	virtual BOOL Close() = 0;
// 
// 	//��ʼ�ɼ�
// 	virtual BOOL StartGrab() = 0;
// 
// 	//ֹͣ�ɼ�
// 	virtual BOOL StopGrab() = 0;
// 
// 	//��֡�ɼ�
// 	virtual BOOL Snapshot() = 0;
// 	
// 	//���ò����Ի���
// 	virtual void CallParamDialog() = 0;
// 
// 	//�õ�������Ϣ
// 	virtual void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo) = 0 ;
// 
// 	//�������Ͳ���
// 	virtual BOOL SetParamInt(int Param, int nInputVal) = 0;
// 	
// 	//�õ����Ͳ���
// 	virtual BOOL GetParamInt(int Param, int &nOutputVal) = 0;
// 
// 	//���ø����Ͳ���
// 	virtual BOOL SetParamfloat(int Param, float nInputVal) = 0;
// 	
// 	//�õ������Ͳ���
// 	virtual BOOL GetParamfloat(int Param, float &nOutputVal) = 0;
// 
// 	//���ô���ģʽ
// 	virtual BOOL SetTriggerParam(int ParamValInput) = 0;
// 
// 	//�õ�����ģʽ
// 	virtual BOOL GetTriggerParam(int &ParamValOutput) = 0;
// 
// 	//����AOI����
// 	virtual BOOL SetAOI(RECT rtAOI) = 0;
// 
// 	//�õ�AOI����
// 	virtual BOOL GetAOI(RECT &rtAOI) = 0;
// 
// 	//�����豸
// 	virtual BOOL Reset() = 0;
// 
// 	//�õ�ϵͳ��Ϣ
// 	virtual BOOL GetSystemInfo(CString &sOutputInfo) = 0;
// 
// 	//����Ĭ�������ļ�
// 	virtual BOOL CreateDefaultConfigurationFile(CString &sInitFile) = 0;
// 
// 	//���������ļ�
// 	virtual BOOL SaveToConfigurationFile(CString &sInitFile) = 0;
// 
// 	
// };
// 
// #endif // !define(DHGrabber_h_)

#if !defined(DHGrabberBAUMER_h_)
#define DHGrabberBAUMER_h_


#include "Grabber.h"
//////////////////////////////////////////////////////////////////////////
//Baumerר�ò���
enum BaumerParamID
{
	FramesPerSecondsContinuous = 0,
	Exposure,
	ExposureMin,
	ExposureMax,
	Gain,
	GainMin,
	GainMax
};

//Baumerר�ò���ֵ
enum BaumerParamVal
{
	LineTriggerOn = 0,
	LineTriggerOff
};

//������붨��
enum ErrorStruct
{
	NormalState = 0,    // ����״̬
	ErrorNoFindCam,	    // û���ҵ����
	ErrorValOverFlow,	// �������
	ErrorGrabberSN,		// �����ų����ܹ��ҵ��������  ��������� 
	ErrorGetCamInfo,	// �õ������Ϣʧ��              ��������� 
	ErrorOpenCam,		// �����ʧ��                  ���������
	ErrorInit,			// ��ʼ��ʧ��                    ���������
	ErrorStartGrab,		// ��ʼ�ɼ�ʧ��                  ���������
	ErrorStopGrab,		// ֹͣ�ɼ�ʧ��                  ���������
	ErrorClose,			// �ر�ʧ��                      ���������
	ErrorSendBufAdd,	// ����ͼ���ڴ��ַʧ��          ���������
	ErrorGetParam,		// �Ƿ���ȡ����                  ���������
	ErrorSetParam,		// �Ƿ����ò���                  ���������
	ErrorParamDlg,	    // ���ò������öԻ���ʧ��        ���������
	ErrorReadIniFile,	// ��ȡ�����ļ�ʧ��              ���������
	ErrorLostFrame,     // �ɼ���TrashBuffer��֡         ���������
	ErrorSysLostFrame,  // ϵͳ�ڲ������Ķ�֡
	ErrorTriggerIgnored,// ͨ����Ϊ�ⴥ��Ƶ�ʹ���
	ErrorSaveInitFile	//�����ʼ���ļ�ʧ��
};

//////////////////////////////////////////////////////////////////////////
//�ɼ���
#define	_EXPORTING

#ifdef _EXPORTING
#define CLASS_DECLSPEC    __declspec(dllexport)  //�������ʱʹ��
#else
#define CLASS_DECLSPEC    __declspec(dllimport)	 //��̬��Ӧ��ʱʹ��
#endif

class CLASS_DECLSPEC CDHGrabForBaumer : public CGrabber
{
	//����
public:
	CDHGrabForBaumer();
	virtual ~CDHGrabForBaumer();

	//��ʼ��
	BOOL Init(const s_GBINITSTRUCT* pInitParam);
	virtual bool Init(const CGrabInitInfo& rGrabInitInfo) override;

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




	//���ò���
	BOOL SetParamInt(GBParamID Param, int nInputVal);

	//�õ�����
	BOOL GetParamInt(GBParamID Param, int &nOutputVal);
	//////////////////////////////////////////////////////////////////////////

	// 	9:GBGain,					//��������С
	//	15:GainMin,
	//	16:GainMax
	BOOL SetParamfloat(int Param, float nInputVal);

	BOOL GetParamfloat(int Param, float& nOutputVal);
	//////////////////////////////////////////////////////////////////////////

	//���ò����Ի���
	void CallParamDialog();

	//�õ�������Ϣ
	void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo);


	//�õ�AOI����(δ��)
	BOOL GetAOI(RECT &rtAOI);

	//����AOI����(δ��)
	BOOL SetAOI(RECT rtAOI);

	//���ô���ģʽ(δ��)
	BOOL SetTriggerParam(int ParamValInput);

	//�õ�����ģʽ(δ��)
	BOOL GetTriggerParam(int &ParamValOutput);

	//�����豸(δ��)
	BOOL Reset();

	//�õ�ϵͳ��Ϣ(δ��)
	BOOL GetSystemInfo(CString &sOutputInfo);

	//����Ĭ�������ļ�(δ��)
	BOOL CreateDefaultConfigurationFile(CString &sInitFile);

	//���������ļ�(δ��)
	BOOL SaveToConfigurationFile(CString &sInitFile);



	//����
protected:
	CGrabber *m_pGrabber;
};

#endif// !defined(DHGrabberBAUMER_h_)


//////////////////////////////////////////////////////////////////////////
//ʹ��˵��
/*

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//�޸ļ�¼
/*

*/
//////////////////////////////////////////////////////////////////////////

