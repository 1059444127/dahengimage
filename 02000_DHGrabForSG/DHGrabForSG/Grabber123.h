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

#if !defined(Grabber_h_)
#define Grabber_h_

const int GBMaxFileNameLen = 255;
const int GBMaxTextLen = 255;

typedef struct _GBSignalInfoStruct//�ص���Ϣ�ṹ
{
	PVOID Context;								//���ӵ�лص������Ķ����thisָ��
	int iGrabberTypeSN;							// �ɼ������ͱ��	// [2009-12-18 by jzj]: add
	int nGrabberSN;								//�ɼ������
	int nErrorCode;								//������루0��������
	int nFrameCount;							//�Ѳɼ���֡����
	TCHAR strDescription[GBMaxTextLen];			//������Ϣ

	int iReserve1;								// �����ֶ�1	// [2009-12-18 by jzj]: add
	int iReserve2;								// �����ֶ�2	// [2009-12-18 by jzj]: add
	
}s_GBSIGNALINFO;//�ص���Ϣ�ṹ

//�ص�����ָ������
typedef void (WINAPI *PGBCALLBACK)(const s_GBSIGNALINFO* SignalInfo);


typedef struct _GBInitStruct//�ɼ�����ʼ���ṹ
{
	int iGrabberTypeSN;						// �ɼ������ͱ��	// [2009-12-18 by jzj]: add
	int nGrabberSN;							//�ɼ������
    char strDeviceName[GBMaxFileNameLen];	//�豸����
	char strGrabberFile[GBMaxFileNameLen];	//���ڳ�ʼ�����ļ�
	PGBCALLBACK CallBackFunc;				//�ص�����ָ��
	PVOID Context;							//���ӵ�лص������Ķ����thisָ��
	
	int iReserve1;							// �����ֶ�1	// [2009-12-18 by jzj]: add
	int iReserve2;							// �����ֶ�2	// [2009-12-18 by jzj]: add

}s_GBINITSTRUCT;//�ɼ�����ʼ���ṹ

typedef struct _GBErrorInfoStruct//������Ϣ�ṹ
{
   	int nErrorCode;							//������루0��������
	TCHAR strErrorDescription[GBMaxTextLen];	//��������
	TCHAR strErrorRemark[GBMaxTextLen];		//������Ϣ

}s_GBERRORINFO;//������Ϣ�ṹ

//��������
enum GBParamID
{
	GBImageWidth = 0,		//ͼ���� (��λ:�ֽ�)
	GBImageHeight,			//ͼ��߶� (��λ:�ֽ�)
	GBImagePixelSize,		//ͼ�����ش�С (��λ:�ֽ�)
	GBImageBufferSize,		//ͼ�񻺳�����С (��λ:�ֽ�)
	GBImageBufferAddr,		//ͼ�񻺳�����ַ
	GBGrabberTypeSN			// �ɼ������ͱ��	// [2009-12-18 by jzj]: add
};

//������붨��
const int GBOK = 0;

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
	
	//���ò���
	virtual BOOL SetParamInt(GBParamID Param, int nInputVal) = 0;
	
	//�õ�����
	virtual BOOL GetParamInt(GBParamID Param, int &nOutputVal) = 0;
	
	//���ò����Ի���
	virtual void CallParamDialog() = 0;
	
	//�õ�������Ϣ
	virtual void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo) = 0;
};

#endif // !define(Grabber_h_)