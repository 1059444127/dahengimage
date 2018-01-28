/************************************************************************/
/* Copyright (c) 2014, �������ͼ���Ӿ����޹�˾  
/* All rights reserved.													
/*																		
/* �ļ����ƣ� DHGrabberJAI.h	
/*
/* ��ǰ�汾�� 1.0.0.1Beta1
/* �޸��ߣ� �Ի���
/* �޸����ݣ� ׫д�¶�̬��
/* ������ڣ� 2014-11-3
/************************************************************************/

/*
�ɼ������ͱ�Ŷ�������
0:ģ��ɼ���
1:DALSA X64�ɼ���
2:AVT���
3:ǧ�������
4:��˾CG300ģ��ɼ���
5:��˾1394���
6:EURESYS �ɼ���
7:JAIǧ�������*/


#if !defined(DHGrabber_h_)
#define DHGrabber_h_

const int GBMaxFileNameLen = 255;
const int GBMaxTextLen = 255;

typedef struct _GBSignalInfoStruct//�ص���Ϣ�ṹ
{
	PVOID Context;								// ���ӵ�лص������Ķ����thisָ��
	int iGrabberTypeSN;							// �ɼ������ͱ��
	int nGrabberSN;								// �ɼ������
	int nErrorCode;								// ������루0��������
	int nFrameCount;							// �Ѳɼ���֡����
	char strDescription[GBMaxTextLen];			// ������Ϣ
	int iReserve1;								// �����ֶ�1
	int iReserve2;								// �����ֶ�2	
}s_GBSIGNALINFO;//�ص���Ϣ�ṹ

//�ص�����ָ������
typedef void (WINAPI *PGBCALLBACK)(const s_GBSIGNALINFO* SignalInfo);

typedef struct _GBInitStruct//�ɼ�����ʼ���ṹ
{
	int iGrabberTypeSN;						// �ɼ������ͱ��
	int nGrabberSN;							// �ɼ������
    char strDeviceName[GBMaxTextLen];		// �豸����
	char strDeviceMark[GBMaxTextLen];		// �豸��ʶ  ���ڱ�ʶ���ӵ������˱�ʶһ��
	char strGrabberFile[GBMaxFileNameLen];	// ���ڳ�ʼ�����ļ�
	PGBCALLBACK CallBackFunc;				// �ص�����ָ��
	PVOID Context;							// ���ӵ�лص������Ķ����thisָ��	
	int iReserve1;							// �����ֶ�1
	int iReserve2;							// �����ֶ�2
}s_GBINITSTRUCT;//�ɼ�����ʼ���ṹ

typedef struct _GBErrorInfoStruct//������Ϣ�ṹ
{
   	int nErrorCode;							//������루0��������
	char strErrorDescription[GBMaxTextLen];	//��������
	char strErrorRemark[GBMaxTextLen];		//������Ϣ
}s_GBERRORINFO;//������Ϣ�ṹ


//�ɼ���
class CGrabber
{
//����
public:
	CGrabber()
	{
	}
	virtual ~CGrabber()
	{
	}
	
	//��ʼ��
	virtual BOOL Init(const s_GBINITSTRUCT* pInitParam) = 0;

	//�ر�
	virtual BOOL Close() = 0;

	//��ʼ�ɼ�
	virtual BOOL StartGrab() = 0;

	//ֹͣ�ɼ�
	virtual BOOL StopGrab() = 0;

	//��֡�ɼ�
	virtual BOOL Snapshot() = 0;
	
	//���ò����Ի���
	virtual void CallParamDialog() = 0;

	//�õ�������Ϣ
	virtual void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo) = 0 ;

	//�������Ͳ���
	virtual BOOL SetParamInt(int Param, int nInputVal) = 0;
	
	//�õ����Ͳ���
	virtual BOOL GetParamInt(int Param, int &nOutputVal) = 0;

	//���ø����Ͳ���
	virtual BOOL SetParamfloat(int Param, float nInputVal) = 0;
	
	//�õ������Ͳ���
	virtual BOOL GetParamfloat(int Param, float &nOutputVal) = 0;

	//���ô���ģʽ
	virtual BOOL SetTriggerParam(int ParamValInput) = 0;

	//�õ�����ģʽ
	virtual BOOL GetTriggerParam(int &ParamValOutput) = 0;

	//����AOI����
	virtual BOOL SetAOI(RECT rtAOI) = 0;

	//�õ�AOI����
	virtual BOOL GetAOI(RECT &rtAOI) = 0;

	//�����豸
	virtual BOOL Reset() = 0;

	//�õ�ϵͳ��Ϣ
	virtual BOOL GetSystemInfo(CString &sOutputInfo) = 0;

	//����Ĭ�������ļ�
	virtual BOOL CreateDefaultConfigurationFile(CString &sInitFile) = 0;

	//���������ļ�
	virtual BOOL SaveToConfigurationFile(CString &sInitFile) = 0;

	
};

#endif // !define(DHGrabber_h_)



// DHGrabberJAI.h: interface for the CDHGrabberJAI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DHGRABBERJAI_H__F392C2D2_F53F_48F1_ACCA_128E9E145E26__INCLUDED_)
#define AFX_DHGRABBERJAI_H__F392C2D2_F53F_48F1_ACCA_128E9E145E26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



//////////////////////////////////////////////////////////////////////////
//�ɼ���
#define	_EXPORTING
#ifdef _EXPORTING
#define CLASS_DECLSPEC    __declspec(dllexport)  //�������ʱʹ��
#else
#define CLASS_DECLSPEC    __declspec(dllimport)	 //��̬��Ӧ��ʱʹ��
#endif

class CLASS_DECLSPEC CDHGrabberJAI:CGrabber
{
public:
	CDHGrabberJAI();
	virtual ~CDHGrabberJAI();

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
	BOOL SetParamInt(int Param, int nInputVal);
	
	//�õ�����
	BOOL GetParamInt(int Param, int &nOutputVal);
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

	//////////////////////////////////////////////////////////////////////////
//����
protected:
	CGrabber *m_pGrabber;

};

#endif // !defined(AFX_DHGRABBERJAI_H__F392C2D2_F53F_48F1_ACCA_128E9E145E26__INCLUDED_)
