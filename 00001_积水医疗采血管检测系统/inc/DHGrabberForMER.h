/************************************************************************/
/* Copyright (c) 2013, �й���㼯�ű���ͼ���Ӿ������ֹ�˾�Ӿ�ϵͳ��  
/* All rights reserved.													
/*	




/* �ļ����ƣ� DHGrabberForMER.h	
/* ժҪ�� �����ɼ����ࣨ��˾ˮ��Gige�ӿڵ������
/* ��ǰ�汾��1.64/32.0.4
/* ʹ��SDK�İ汾Ϊ1.1.1501.8211
/* �޸��ߣ�����
/* ��Ҫ�޸����ݣ�����µ�sdk��ȥ���ع�ģʽ����������������64bit��dll����������ͷ�ļ�,���Ӳɼ������ͷ���ֵ
/* ������ڣ�2015��5��28��



/* �ļ����ƣ� DHGrabberForMER.h	
/* ժҪ�� �����ɼ����ࣨ��˾ˮ��usb�����
/* ��ǰ�汾��1.0.0.2
/* �޸��ߣ�����
/* ��Ҫ�޸����ݣ��޸�������ԶԻ������Ӳ��ֺ���
/* ������ڣ�2013��12��2��

/* ��ǰ�汾��1.0.0.1
/* �޸��ߣ���ͬ
/* ��Ҫ�޸����ݣ�ʵ�ֻ����ĳ�ʼ���������ɼ���ֹͣ�ɼ�����֡�ɼ��Ͳ������ý���
/* ������ڣ�2013��10��12��

/************************************************************************/

#if !defined(DHGrabber_h_)
#define DHGrabber_h_

/*
�ɼ������ͱ�Ŷ�������
0:ģ��ɼ���
1:DALSA X64�ɼ���
2:AVT���
3:ǧ�������
4:��˾CG300ģ��ɼ���
5:��˾1394���
6:EURESYS �ɼ���
7:��˾USB���
8::��˾ˮ��ϵ�����*/

const int GBMaxFileNameLen = 255;
const int GBMaxTextLen = 255;

typedef struct _GBSignalInfoStruct//�ص���Ϣ�ṹ
{
	PVOID Context;								//���ӵ�лص������Ķ����thisָ��
	int nGrabberTypeSN;							//�ɼ������ͱ��
	int nGrabberSN;								//�ɼ������
	int nErrorCode;								//������루0��������
	int nFrameCount;							//�Ѳɼ���֡����
	unsigned __int64 nTimestamp;				//ʱ���
	char strDescription[GBMaxTextLen];			//������Ϣ

	int iReserve1;								// �����ֶ�1
	int iReserve2;								// �����ֶ�2
	
}s_GBSIGNALINFO;//�ص���Ϣ�ṹ


typedef struct _GBEventInfoStruct//�ص���Ϣ�ṹ
{
	PVOID Context;								//���ӵ�лص������Ķ����thisָ��
	int iGrabberTypeSN;							//�ɼ������ͱ��
	int nGrabberSN;								//�ɼ������
	int nErrorCode;								//������루0��������
	int nFrameCount;							//�Ѳɼ���֡����
	__int64 nTimestamp;							//ʱ���

	char strDescription[GBMaxTextLen];			//������Ϣ
	
	int iReserve1;								// �����ֶ�1
	int iReserve2;								// �����ֶ�2
	
}s_GBEVENTINFO;//�ص���Ϣ�ṹ

//�ص�����ָ������
typedef void (WINAPI *PGBCALLBACK)(const s_GBSIGNALINFO* SignalInfo);

//��������Ӧ�¼��Ļص�����
typedef void (WINAPI *PGBEVENTCALLBACK)(const s_GBEVENTINFO* EventInfo);

typedef struct _GBInitStruct//�ɼ�����ʼ���ṹ
{
	int iGrabberTypeSN;						// �ɼ������ͱ��
	int nGrabberSN;							//�ɼ������
    char strDeviceName[GBMaxTextLen];		//�豸����
	char strDeviceMark[GBMaxTextLen];		//�豸��ʶ
	char strGrabberFile[GBMaxFileNameLen];	//���ڳ�ʼ�����ļ�
	PGBCALLBACK CallBackFunc;				//�ص�����ָ��
	PGBEVENTCALLBACK EventCallbackFunc;		//��������Ӧ�¼��Ļص�����

	PVOID Context;							//���ӵ�лص������Ķ����thisָ��
	int iReserve1;							// �����ֶ�1
	int iReserve2;							// �����ֶ�2

}s_GBINITSTRUCT;//�ɼ�����ʼ���ṹ

typedef struct _GBErrorInfoStruct//������Ϣ�ṹ
{
   	int nErrorCode;							//������루0��������
	char strErrorDescription[GBMaxTextLen];	//��������
	char strErrorRemark[GBMaxTextLen];		//������Ϣ

}s_GBERRORINFO;//������Ϣ�ṹ

//��������
enum GBParamID
{
	GBImageWidth = 0,		//ͼ���� (��λ:�ֽ�)
	GBImageHeight,			//ͼ��߶� (��λ:�ֽ�)
	GBImagePixelSize,		//ͼ�����ش�С (��λ:�ֽ�)
	GBImageBufferSize,		//ͼ�񻺳�����С (��λ:�ֽ�)
	GBImageBufferAddr,		//ͼ�񻺳�����ַ
	GBGrabberTypeSN			//�ɼ������ͱ��
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

#endif// !defined(DHGrabber_h_)

#if !defined(DHGrabberMER_h_)
#define DHGrabberMER_h_

// ˮ�����ר�в���
enum MERParamID
{
	MERSnapMode,				// ͼ��ɼ�ģʽ
	MERGain,						// ����
	MERExposure,				// �ع�ʱ��
	MERBlacklevel,			    // �ڵ�ƽ
	MERBalanceRatio,             // ��ƽ��
	MERWidth,						//���	//[2017-3-24 zhy]
	MERHeight,						//�߶�	//[2017-3-24 zhy]
	MERXOffset,						//X����ƫ����	//[2017-3-24 zhy]
	MERYOffset						//Y����ƫ����	//[2017-3-24 zhy]
};


#ifdef _EXPORTING
#define CLASS_DECLSPEC    __declspec(dllexport)  //�������ʱʹ��
#else
#define CLASS_DECLSPEC    __declspec(dllimport)	 //��̬��Ӧ��ʱʹ��
#endif


//�ɼ���
class CLASS_DECLSPEC CDHGrabberMER : public CGrabber
{
	//����
public:
	CDHGrabberMER();
	virtual ~CDHGrabberMER();
	
	//��ʼ��
	BOOL Init(const s_GBINITSTRUCT* pInitParam);
	
	//�ر�
	BOOL Close();
	
	//��ʼ�ɼ�
	BOOL StartGrab();
	
	//ֹͣ�ɼ�
	BOOL StopGrab();
	
	// ��֡�ɼ� 
	BOOL Snapshot();
	
	//���ò���
	BOOL SetParamInt(GBParamID Param, int nInputVal);
	
	//�õ�����
	BOOL GetParamInt(GBParamID Param, int &nOutputVal);
	
	//���ò����Ի���
	void CallParamDialog();
	
	//�õ�������Ϣ
	void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo);
	
	//////////////////////////////////////////////////////////////////////////
	// ��˾���ר�нӿ�	
	BOOL MERSetParamInt(MERParamID Param, int nInputVal1,int nInputVal2 = 0,int nInputVal3 = 0,int nInputVal4 = 0,int nInputVal5 = 0,void *sInputVal6 = NULL);
	BOOL MERGetParamInt(MERParamID Param, int &nOutputVal1,int &nOutputVal2,int &nOutputVal3);	
	int  MERSetOutputParamInt(int nOutputPort,int nOutputVal);//��������Զ���������� 
	//����ֵ 0:�ɹ� 1:��֧���Զ����ź� 2:�����Զ����ź�ʧ�� 3:�򿪶˿�ʧ�� 4:д���ֵʧ�� 

	
	void SaveParamToINI();		// �������������Ϣ
	
	//
	//////////////////////////////////////////////////////////////////////////
	
	//����
protected:
	CGrabber *m_pGrabber;
};

#endif// !defined(DHGrabberMER_h_)