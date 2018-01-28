/************************************************************************/
/* Copyright (c) 2007, �й���㼯�ű���ͼ���Ӿ������ֹ�˾�Ӿ�ϵͳ��     
/* All rights reserved.													
/*																		
/* �ļ����ƣ� Grabber.h												
/* �ļ���ʶ�� �����ù���ƻ���
/* ժҪ�� �ɼ�����������ࣩ
/*
/* ��ǰ�汾�� 3.0
/* �޸��ߣ� ��׿��
/* �޸�����ժҪ����ʼ���ṹ�弰�ɼ��ص��ṹ����롰�ɼ������ͱ�š���// [2009-12-18 by jzj]: add
/* ������ڣ� 2009��12��18��
/*
/* ��ǰ�汾�� 1.1
/* �޸��ߣ� ��׿��
/* �޸�����ժҪ�����뵥֡�ɼ��ӿ�Snapshot()��
/* ������ڣ� 2007��11��05��
/*
/* ��ʷ�汾�� 1.0
/* ���ߣ� ��׿��
/* ������ڣ� 2006��08��01��
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
7:JAIǧ�������
8::��˾ˮ��ϵ�����*/
#pragma once

enum e_GrabType {
	e_GrabType_SG = 0,
	e_GrabType_AVT = 2,
	e_GrabType_BAUMER = 3,
	e_GrabType_JAI = 7,
	e_GrabType_MER = 8,
	e_GrabType_NONE = 666
};

const int GBMaxFileNameLen{256};
const int GBMaxTextLen{256};


class s_GBSIGNALINFO//�ص���Ϣ�ṹ
{
public:
	PVOID Context;								//���ӵ�лص������Ķ����thisָ��
	int iGrabberTypeSN;							//�ɼ������ͱ��
	int iGrabberSN;								//�ɼ������
	int iErrorCode;								//������루0��������
	int iFrameCount;							//�Ѳɼ���֡����
	int &nErrorCode = iErrorCode;
	int &nFrameCount = iFrameCount;
	int &nGrabberSN = iGrabberSN;
	int &nGrabberTypeSN = iGrabberTypeSN;
	unsigned __int64 nTimestamp;				//ʱ���
	TCHAR strDescription[MAX_PATH];				//������Ϣ
	//(TCHAR &)[MAX_PATH] strDescription = strDescription;

	int iReserve1;								// �����ֶ�1
	int iReserve2;								// �����ֶ�2
	s_GBSIGNALINFO()
	{
		Context = NULL;
		iGrabberTypeSN = 0;
		iGrabberSN = 0;
		iErrorCode = 0;
		iFrameCount = 0;
		memset(strDescription, 0, sizeof(strDescription));
	}

};//�ص���Ϣ�ṹ


// typedef struct _GBEventInfoStruct//�ص���Ϣ�ṹ
// {
// 	PVOID Context;								//���ӵ�лص������Ķ����thisָ��
// 	int iGrabberTypeSN;							//�ɼ������ͱ��
// 	int iGrabberSN;								//�ɼ������
// 	int iErrorCode;								//������루0��������
// 	int iFrameCount;							//�Ѳɼ���֡����
// 	__int64 nTimestamp;							//ʱ���
// 
// 	TCHAR strDescription[GBMaxTextLen];			//������Ϣ
// 
// 	int iReserve1;								// �����ֶ�1
// 	int iReserve2;								// �����ֶ�2
// 
// }s_GBEVENTINFO;//�ص���Ϣ�ṹ

//�ص�����ָ������
typedef void (WINAPI *PGBCALLBACK)(const s_GBSIGNALINFO* SignalInfo);

// ��������Ӧ�¼��Ļص�����
// typedef void (WINAPI *PGBEVENTCALLBACK)(const s_GBEVENTINFO* EventInfo);

class s_GBINITSTRUCT//�ɼ�����ʼ���ṹ
{
public:
	int iGrabberTypeSN;						// �ɼ������ͱ��
	int iGrabberSN;							//�ɼ������
	int &nGrabberTypeSN = iGrabberTypeSN;
	int &nGrabberSN = iGrabberSN;
	TCHAR strDeviceName[MAX_PATH];		//�豸����
	TCHAR strDeviceMark[MAX_PATH];		//�豸��ʶ
	TCHAR strGrabberFile[MAX_PATH];	//���ڳ�ʼ�����ļ�
	PGBCALLBACK CallBackFunc;				//�ص�����ָ��
	PGBCALLBACK EventCallbackFunc;		//��������Ӧ�¼��Ļص�����

	PVOID Context;							//���ӵ�лص������Ķ����thisָ��
	int iReserve1;							// �����ֶ�1
	int iReserve2;							// �����ֶ�2

	s_GBINITSTRUCT()
	{
		Context = NULL;
		iGrabberTypeSN = 0;
		iGrabberSN = 0;
		iReserve1 = 0;
		iReserve2 = 0;
		CallBackFunc = NULL;
		EventCallbackFunc = NULL;
		memset(strDeviceName, 0, sizeof(strDeviceName));
		memset(strDeviceMark, 0, sizeof(strDeviceMark));
		memset(strGrabberFile, 0, sizeof(strGrabberFile));
	}

};//�ɼ�����ʼ���ṹ

class s_GBERRORINFO//������Ϣ�ṹ
{
public:
	int iErrorCode;							//������루0��������
	int &nErrorCode = iErrorCode;
	TCHAR strErrorDescription[MAX_PATH];	//��������
	TCHAR strErrorRemark[MAX_PATH];		//������Ϣ
	s_GBERRORINFO()
	{
		iErrorCode = 0;
		memset(strErrorRemark, 0, sizeof(strErrorRemark));
		memset(strErrorDescription, 0, sizeof(strErrorDescription));
	}

};//������Ϣ�ṹ


			   //��������
enum GBParamID
{
	GBImageWidth = 0,		//ͼ���� (��λ:�ֽ�)
	GBImageHeight=1,			//ͼ��߶� (��λ:�ֽ�)
	GBImagePixelSize=2,		//ͼ�����ش�С (��λ:�ֽ�)
	GBImageBufferSize=3,		//ͼ�񻺳�����С (��λ:�ֽ�)
	GBImageBufferAddr=4,		//ͼ�񻺳�����ַ
	GBGrabberTypeSN=5,			//�ɼ������ͱ��

	//**[12/28/2017 ConanSteve]**:  ���ܲ�֧�֣�
	GBExposureTime=6, //**[9/11/2017 ConanSteve]**:  �ع�ʱ��
	GBGain=7,//**[9/11/2017 ConanSteve]**:  ����
	GBIsColorFilter=8,//**[11/16/2017 ConanSteve]**:  �Ƿ��ͼ
	GBBayerType=9,//**[11/16/2017 ConanSteve]**:  Bayer�任����
	GBEnableFlipImage=10,//**[11/16/2017 ConanSteve]**:  ���ԭͼ�Ƿ���Ҫ��ת
	GBShutter=11,//**[11/16/2017 ConanSteve]**:  �������ֵ
	GBSequenceNum=12,//**[11/16/2017 ConanSteve]**:  ����ͼ����
	GBExposureMin=13,//**[11/16/2017 ConanSteve]**:  ��С�ع�ֵ
	GBExposureMax=14//**[11/16/2017 ConanSteve]**:  ����ع�ֵ

};
//**[9/7/2017 ConanSteve]**:  ���ͨ����Ϣ
class CGrabInfo
{
public:
	int m_iImageWidth;//**[9/7/2017 ConanSteve]**:  �����ǰͼ���
	int m_iImageHeight;//**[9/7/2017 ConanSteve]**:  �����ǰͼ���
	int m_iImageSize;//**[1/2/2018 ConanSteve]**:  ���ԭͼ�ߴ��С
	int m_iOffsetX;
	int m_iOffsetY;
	int m_iImagePixelSize;//**[9/7/2017 ConanSteve]**:  ����ɼ�ԭͼÿ��ͼ�ص��ֽ���
	int m_iExposureTime;//**[9/11/2017 ConanSteve]**:  �ع�ʱ��
	int m_iGain;//**[9/11/2017 ConanSteve]**:  ����
	int m_bIsColorFilter;//**[9/7/2017 ConanSteve]**:  ����Ƿ��ǲ�ɫ���
	int  m_iBayerType;//**[9/11/2017 ConanSteve]**:  Bayerת�����ͣ�ֵ��DX_PIXEL_COLOR_FILTER����һ��
	int	 m_iFlipType;//**[9/7/2017 ConanSteve]**:  ��ת����
	CGrabInfo()
	{
		m_iImageWidth = 0;
		m_iImageHeight = 0;
		m_iOffsetX = 0;
		m_iOffsetY = 0;
		m_iImagePixelSize = 0;
		m_iExposureTime = 1000;
		m_iGain = 0;
		m_bIsColorFilter = FALSE;
		m_iBayerType = 0;
		m_iFlipType = 0;
	}
};

//������붨��
const int GBOK = 0;


//**[8/29/2017 ConanSteve]**:  ��ʼ�������
class CGrabInitInfo {
public:
	int iGrabSN{ -1 };
	TCHAR lpszPathFileOfGrab[GBMaxFileNameLen];	//���ڳ�ʼ�����ļ�
	PGBCALLBACK CallBackFunc;				//�ص�����ָ��
	PGBCALLBACK EventCallbackFunc;	//����������Ӧ�Ļص�����ָ�봫��
	PVOID Context;							//���ӵ�лص������Ķ����thisָ��
public:
	CGrabInitInfo()
	{
		iGrabSN = -1;
		memset(lpszPathFileOfGrab, 0, sizeof(lpszPathFileOfGrab));
		CallBackFunc = EventCallbackFunc = NULL;
		Context = NULL;
	}
};


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
	virtual bool Init(const CGrabInitInfo& rGrabInitInfo) {return false;}

	//�ر�
	virtual BOOL Close() = 0;

	//��ʼ�ɼ�
	virtual BOOL StartGrab() = 0;

	//ֹͣ�ɼ�
	virtual BOOL StopGrab() = 0;

	//��֡�ɼ�
	virtual BOOL Snapshot() = 0;
	
	//���ò���
	virtual BOOL SetParamInt(GBParamID Param, int nInputVal) = 0;
	
	//�õ�����
	virtual BOOL GetParamInt(GBParamID Param, int &nOutputVal) = 0;

	virtual bool SetParamBool(GBParamID Param, bool bInputVal) {return false;};
	virtual bool GetParamBool(GBParamID Param, bool bRetVal) { return false; };
	
	//���ò����Ի���
	virtual void CallParamDialog() = 0;
	
	//�õ�������Ϣ
	virtual void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo) = 0;



	//���ø����Ͳ���
	virtual BOOL SetParamfloat(int Param, float nInputVal) {return false;};

	//�õ������Ͳ���
	virtual BOOL GetParamfloat(int Param, float &nOutputVal) { return false; };

	//���ô���ģʽ
	virtual BOOL SetTriggerParam(int ParamValInput) { return false; };

	//�õ�����ģʽ
	virtual BOOL GetTriggerParam(int &ParamValOutput) { return false; };

	//����AOI����
	virtual BOOL SetAOI(RECT rtAOI) { return false; };

	//�õ�AOI����
	virtual BOOL GetAOI(RECT &rtAOI) { return false; };

	//�����豸
	virtual BOOL Reset() { return false; };

	//�õ�ϵͳ��Ϣ
	virtual BOOL GetSystemInfo(CString &sOutputInfo) { return false; };

	//����Ĭ�������ļ�
	virtual BOOL CreateDefaultConfigurationFile(CString &sInitFile) { return false; };

	//���������ļ�
	virtual BOOL SaveToConfigurationFile(CString &sInitFile) { return false; };
};
