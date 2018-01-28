
/************************************************************************/
/* Copyright (c) 2009, �������ͼ���Ӿ����޹�˾�Ӿ�ϵͳ��     
/* All rights reserved.													
/*																		
/* �ļ�����: ParamDefine.h		
/* ժ    Ҫ: 
/*
/* ��ǰ�汾: 1.2
/* ��    ��: �ɭ
/* �������: 2009��08��13��
/* 2009-8-16  ���������źźϲ��������ź���
/* 2009-8-19  ��������Ϣ���м���ɼ����ڼ����������ź�
/* ���bayer�任����
/* 2009-8-21 �������ŷ����Ԫ����;���屣��ͼ��Ԫ��
/************************************************************************/

#pragma  once

//#include "Image.h"
#include "alg_struct.h"
using namespace Alg; // ʹ�������ռ� [2009-9-15-yangfs]

#define  MAX_COUNT_BY_CIRCLE    20     //һ���ɼ������ڲɼ������ͼ����

////////////////////////////////////////////////
// PI����
#ifndef PI
#ifdef  M_PI
#define PI               M_PI
#else
#define PI               3.14159265358979323846
#endif
#endif
#ifndef PI_2
#define PI_2             (PI / 2.0)
#endif
#ifndef PI_4
#define PI_4             (PI / 4.0)
#endif


#if defined(_WIN32)||defined(TRIMEDIA)||defined(_TMS320C6X)||defined (__vxworks)
#ifndef M_PI
#define M_PI             PI
#endif
#ifndef M_PI_2
#define M_PI_2           (PI / 2.0)
#endif
#ifndef M_PI_4
#define M_PI_4           (PI / 4.0)
#endif
#endif
////////////////////////////////////////////

//�����������Ҫ�ı����ͽṹ��

//����ͼ������
enum e_SaveImageType	
{
	GrabImage = 0,				//�ɼ���ͼ��
	ErrorImage,		            //�����ͼ��
	AbnormityImage			    //�쳣��ͼ��
};

// �ɼ�ͼ������
enum e_GrabImageType	
{
	GImageBody = 0,	    // ƿ��
	GImageBottom		// ƿ��
};

// �ɼ�״̬
enum e_GrabStatus	
{
	GSNormal = 0,	// ����
	GSLostFrame		// ��֡
};

// ������
enum e_CameraType	
{
	Camera1 = 0,	 
	Camera2,		 
	Camera3,
	Camera4,
	Camera5,
	Camera6,
	Camera7,
	Camera8,
	Camera9,
};

enum e_ThreadType
{
	CheckThread1 = 0,
	CheckThread2,
	CheckThread3,
	CheckThread4,
	CheckThread5,
	CheckThread6,
	CheckThread7,
	CheckThread8,
	CheckThread9,  //[2012-4-16 by zhy]
};

enum e_WorkState  //����״̬(����״̬����ʾ)
{
	SystemStarting = 0,			//��������ϵͳ
	SystemStarted,				//ϵͳ������
	IOCardInfo,                 //�ӿڿ���Ϣ
	Checking,					//���ڼ��...
	CheckResult,                //�����
	StopChecked,					//�����ֹͣ
	AlgCheckTime,               //�㷨���ʱ��
	SetCheckParam,              //�����㷨����
	Grabbing,					//�ɼ���...
	StopGrabbing,				//ֹͣ�ɼ�
	ContinueImageSaving,        //��������ͼ��
	ConstantImageSaving,        //�̶�����ͼ��
	ErrorImageSaving,           //����ͼ�񱣴�
	AbnormalImageSaving,        //�쳣ͼ�񱣴�
	ModelImageSaving,			//ģ��ͼ�񱣴�
	ImageSaved,					//ͼ�񱣴����
	StopSave,                   //ֹͣ����
	SystemClosing,				//�����˳�ϵͳ
	FirstStop,					//��һ��λ�޸�ͣ��
	ThirdStop,					//������λ�޸�ͣ��
	NoEnoughPressure,			//��ѹ����ͣ��
	BatchFinished,				//������ɱ���
	FirstNoKick,				//��һ��λ���޷�
	SecondNoKick,				//�ڶ���λ���޷�
	ThirdNoKick,				//������λ���޷�
	FirstAlwaysKick,			//��һ��λ�����޷�
	SecondAlwaysKick,			//�ڶ���λ�����޷�
	ThirdAlwaysKick,			//������λ�����޷�
	TestBeltSpeed				//����Ƥ���ٶ�

};  //����״̬

//////////////////////////////////////////////////////////////////////////
// ���ݽṹ

// �����ź�
class CMachineSignal  //���ݾ����ֳ�����������������ź�����
{
public:	
	int m_iReturnCount1;	// ��һ��������
	int m_iReturnCount2;	// �ڶ���������
	int m_iReturnCount3;	// �ڶ���������
	int m_iImageCount1;		// �ӿڿ�ͼ���źż���  ͼ��ż���0 ��Ӧ���1
	int m_iImageCount2;		// ͼ��ż���1  // ��Ӧ���2
	int m_iImageCount3;		// ͼ��ż���2  // ��Ӧ���3
	int m_iImageCount4;     // ͼ��ż���3  // ��Ӧ���4
	int m_iKickCount1;      // ���1��2�߷ϼ��� 
    int m_iKickCount2;		// ���3���޷ϼ��� [2012-4-11 by zhy]
	int m_iKickCount3;		// ���3���޷ϼ��� [2012-4-11 by zhy]
	int m_iTriggerCount1;   // ���1�Ĵ�������
	int m_iTriggerCount2;  // ���2�Ĵ�������
	int m_iTriggerCount3;  // ���3�Ĵ�������
public:
	
	CMachineSignal()
	{
		m_iReturnCount1		= 0; 
		m_iReturnCount2		= 0;
		m_iReturnCount3		= 0;
		m_iImageCount1		= 0;
		m_iImageCount2		= 0;
		m_iImageCount3		= 0;
		m_iImageCount4		= 0;
		m_iKickCount1		= 0;       // ���1�߷ϼ���
		m_iKickCount2		= 0;       // ���2���޷ϼ���
		m_iKickCount3		= 0;       // ���3���޷ϼ���
		m_iTriggerCount1	= 0;
		m_iTriggerCount2	= 0;
		m_iTriggerCount3	= 0;
	}

	virtual ~CMachineSignal()
	{
	}

	CMachineSignal(const CMachineSignal &MachineSignal)
	{
		m_iReturnCount1		= MachineSignal.m_iReturnCount1;
		m_iReturnCount2		= MachineSignal.m_iReturnCount2;
		m_iReturnCount3		= MachineSignal.m_iReturnCount3;
		m_iImageCount1		= MachineSignal.m_iImageCount1;
		m_iImageCount2		= MachineSignal.m_iImageCount2;
		m_iImageCount3		= MachineSignal.m_iImageCount3;
		m_iImageCount4		= MachineSignal.m_iImageCount4;
		m_iKickCount1		= MachineSignal.m_iKickCount1;
		m_iKickCount2		= MachineSignal.m_iKickCount2;
		m_iKickCount3		= MachineSignal.m_iKickCount3;
		m_iTriggerCount1	= MachineSignal.m_iTriggerCount1;
		m_iTriggerCount2	= MachineSignal.m_iTriggerCount2;
		m_iTriggerCount3	= MachineSignal.m_iTriggerCount3;
	}

	const CMachineSignal& operator=(const CMachineSignal &MachineSignal)
	{
		m_iReturnCount1		= MachineSignal.m_iReturnCount1;
		m_iReturnCount2		= MachineSignal.m_iReturnCount2;
		m_iReturnCount3		= MachineSignal.m_iReturnCount3;
		m_iImageCount1		= MachineSignal.m_iImageCount1;
		m_iImageCount2		= MachineSignal.m_iImageCount2;
		m_iImageCount3		= MachineSignal.m_iImageCount3;
		m_iImageCount4		= MachineSignal.m_iImageCount4;
		m_iKickCount1		= MachineSignal.m_iKickCount1;
		m_iKickCount2		= MachineSignal.m_iKickCount2;
		m_iKickCount3		= MachineSignal.m_iKickCount3;
		m_iTriggerCount1	= MachineSignal.m_iTriggerCount1;
		m_iTriggerCount2	= MachineSignal.m_iTriggerCount2;
		m_iTriggerCount3	= MachineSignal.m_iTriggerCount3;
		return *this;
	}
};

//�ɼ�ͼ����Ϣ
class CGrabImageInfo
{
public:
	e_GrabImageType     m_eGrabImageType;		// ͼ������
	BOOL			    m_bHaveImage;           // �Ƿ���ͼ��
	CImage			    m_myImage;	            // �����ͼ����Ϣ  ��ȡ��߶ȡ���������
	DWORD				m_iCircleCount;			// ���ź�
	int                 m_iCameraSN;			// ������
public:
	CGrabImageInfo()
	{
		m_eGrabImageType = GImageBody;  // ͼ������
		m_bHaveImage     = FALSE;       // �Ƿ���ͼ��
		m_iCameraSN   = 0;           // ͼ�����
		m_iCircleCount   = 0;           // �ɼ��������� ��������
	}

	virtual ~CGrabImageInfo()
	{
	}
};

// �ɼ�������Ϣ
class CGrabMachineInfo
{
public:
	int				m_iGrabSpendTime;	// �ɼ�����ʱ��	
	e_GrabStatus	m_eGrabStatus;		// �ɼ�״̬
	BOOL			m_bIsCheck;			// �Ƿ���
	CMachineSignal	m_MachineSignal;	// �����ź���Ϣ
	CString			m_strDescription;	// ������Ϣ����

public:

	CGrabMachineInfo()
	{
		m_iGrabSpendTime= 0;
		m_bIsCheck		= FALSE;
		m_eGrabStatus	= GSNormal;
	}

	virtual ~CGrabMachineInfo()
	{
	}
	CGrabMachineInfo(const CGrabMachineInfo &GrabMachineInfo)
	{
		m_iGrabSpendTime	= GrabMachineInfo.m_iGrabSpendTime;	
		m_eGrabStatus		= GrabMachineInfo.m_eGrabStatus;	
		m_strDescription    = GrabMachineInfo.m_strDescription; 
		m_bIsCheck			= GrabMachineInfo.m_bIsCheck;
		m_MachineSignal     = GrabMachineInfo.m_MachineSignal;
	}

	const CGrabMachineInfo& operator=(const CGrabMachineInfo &GrabMachineInfo)
	{
		m_iGrabSpendTime	= GrabMachineInfo.m_iGrabSpendTime;	
		m_eGrabStatus		= GrabMachineInfo.m_eGrabStatus;	
		m_strDescription    = GrabMachineInfo.m_strDescription; 
		m_bIsCheck			= GrabMachineInfo.m_bIsCheck;
		m_MachineSignal		= GrabMachineInfo.m_MachineSignal;
		return *this;
	}
};

//Bayer�任����
class CBayerTransform
{
public:
	BOOL	m_bAreaCamBayer;				// �������ͼ���Ƿ����Bayerת��
	BYTE	m_pLutR[256];                  //Bayer�任��ɫ���ұ�
	BYTE	m_pLutG[256];                  //Bayer�任��ɫ���ұ�
	BYTE	m_pLutB[256];                  //Bayer�任��ɫ���ұ�
	double	m_dBayerRedGain;             //Bayer�任�ĺ�ɫ��ƽ������ֵ
	double  m_dBayerGreenGain;           //Bayer�任����ɫ��ƽ������ֵ
	double  m_dBayerBlueGain;            //Bayer�任����ɫ��ƽ������ֵ
public:
	CBayerTransform()
	{
		m_bAreaCamBayer = FALSE;
		m_dBayerRedGain = 0.0;             //Bayer�任�ĺ�ɫ��ƽ������ֵ
		m_dBayerGreenGain = 0.0;           //Bayer�任����ɫ��ƽ������ֵ
		m_dBayerBlueGain = 0.0;            //Bayer�任����ɫ��ƽ������ֵ
		for (int i=0; i< 256;i++)
		{
			m_pLutR[i] = min((int)(i*m_dBayerRedGain),255);
			m_pLutG[i] = min((int)(i*m_dBayerGreenGain),255);
			m_pLutB[i] = min((int)(i*m_dBayerBlueGain),255);
		}
	}
	
	virtual ~CBayerTransform()
	{

	}
	CBayerTransform(const CBayerTransform &BayerTransform)
	{
		m_bAreaCamBayer = BayerTransform.m_bAreaCamBayer;
		m_dBayerRedGain = BayerTransform.m_dBayerRedGain;      //Bayer�任�ĺ�ɫ��ƽ������ֵ
		m_dBayerGreenGain = BayerTransform.m_dBayerGreenGain;  //Bayer�任����ɫ��ƽ������ֵ
		m_dBayerBlueGain = BayerTransform.m_dBayerBlueGain;    //Bayer�任����ɫ��ƽ������ֵ
		for (int i=0; i< 256;i++)
		{
			m_pLutR[i] = min((int)(i*m_dBayerRedGain),255);
			m_pLutG[i] = min((int)(i*m_dBayerGreenGain),255);
			m_pLutB[i] = min((int)(i*m_dBayerBlueGain),255);
		}
	}
	const CBayerTransform& operator=(const CBayerTransform &BayerTransform)
	{
		m_bAreaCamBayer = BayerTransform.m_bAreaCamBayer;
		m_dBayerRedGain = BayerTransform.m_dBayerRedGain;      //Bayer�任�ĺ�ɫ��ƽ������ֵ
		m_dBayerGreenGain = BayerTransform.m_dBayerGreenGain;  //Bayer�任����ɫ��ƽ������ֵ
		m_dBayerBlueGain = BayerTransform.m_dBayerBlueGain;    //Bayer�任����ɫ��ƽ������ֵ
		for (int i=0; i< 256;i++)
		{
			m_pLutR[i] = min((int)(i*m_dBayerRedGain),255);
			m_pLutG[i] = min((int)(i*m_dBayerGreenGain),255);
			m_pLutB[i] = min((int)(i*m_dBayerBlueGain),255);
		}
		return *this;
	}

};

// class   CSaveImageInfo  //����ͼ��Ԫ��
// {
// public:
// 	int         	 m_iWorkStationSN;	   //������
// 	int              m_iCircleSN;   //�ɼ��������� ��������
// 	int				 m_iImageSN;
// 	eSaveImageType   m_eSaveImageType; //��������
// 	CGrabImageInfo   m_GrabImageInfo;  //С��ͼ����Ϣ
// 	CString			 m_strSaveTime;  //����ʱ��
// public:
// 	CSaveImageInfo()
// 	{
// 		m_iWorkStationSN = 0;	 // ������
// 		m_iCircleSN = 0;  //�ɼ���������
// 		m_iImageSN = 0;
// 		m_eSaveImageType = SaveGrabImage; //��������
// 	}
// 	virtual ~CSaveImageInfo()
// 	{
// 
// 	}
// };


//������ϢԪ����   //���ݾ�����Ŀ������������µĳ�Ա	
class CAllInformationElement
{
public:
	int         		m_iCamGroup;				//������[5/20/2010 ZXF] [2012-1-19 by zhy]
	CGrabMachineInfo	m_GrabMachineInfo;			//�ɼ�������Ϣ
	CPtrList			m_listSingleImageData;		//һ���ɼ������ڲɼ���ͼ������
	DWORD               m_iCircleCount;				//�ɼ����ڸ��� �����ź�
	DWORD				m_iHardCircleCount;         //�ӽӿڿ�������ͼ��ţ����ڴ����������û��ȫ����Ӧ������m_iCircleCount��һ��
	BOOL                m_bHaveError;				//����Ƿ��д�
	CString				m_strErrorInfo;
public:

	CAllInformationElement()
	{
		m_iCamGroup = 0;
		m_iCircleCount = 0;
		m_iHardCircleCount = 0;
		m_bHaveError = FALSE;
		m_strErrorInfo="";
	}

	virtual ~CAllInformationElement()
	{
		// �ͷ�����С��ͼ����Դ
		CGrabImageInfo* pGrabImageInfo = NULL;
		while (m_listSingleImageData.IsEmpty() == FALSE)
		{
			pGrabImageInfo = (CGrabImageInfo*)m_listSingleImageData.RemoveHead();
			if (pGrabImageInfo != NULL)
			{
				delete pGrabImageInfo;
				pGrabImageInfo = NULL;
			}
		}
	}
};


//�˵�����
enum e_MenuType
{
	UserMenu = 0,		//��ͨ�û��˵�
	AdvUserMenu			//�߼��û��˵�
};

//��ʱ������
enum e_TimerType
{
	ViewUpdateTimer = 0,			//View������ˢ�¶�ʱ�� ��һ��
	TimeTimer,						//ʱ�䶨ʱ��
	DetectTimer,					//���ʱ�䶨ʱ��
	StartGrabTimer,					//��ʼ�ɼ���ʱ��
	InspectSystemTimer,				//����ϵͳ����״̬��ʱ��
	IntervalDisplayTimerOne,		//���1�����ʾ��ʱ��
	IntervalDisplayTimerTwo,		//���2�����ʾ��ʱ��
	IntervalDisplayTimerThree,		//���3�����ʾ��ʱ��
	CheckSpeedRealTime,				//����ٶ�ʵʱ��ʾ [12/3/2009 GZ]
	CameraStatus,					//���״̬ [1/21/2010 GZ]
	IO24BSTATUS,
	AlarmOff,  
	AlarmOn,						
	DisplayCount,					// ���豸�Ѿ�������ƿ������
	SaveCount,						// �������ļ�д�����ݣ���������
	UpdateCheckData_1,				// ���°���λ�������
	UpdateCheckData_2,				// ���°�λ�ü������
	ShowDataInfo,					// ��ʾ������Ϣ
	CameraTest,						// �������
};

class CImageInformation//**[1/2/2018 ConanSteve]**:  �㷨�Ͷ����ʹ�õĽṹ��
{
public:
	int m_iCameraImageWidth;		// �ɼ�ͼ����
	int m_iCameraImageHeight;		// �ɼ�ͼ��߶�
	int m_iCameraImageSize;			// �ɼ�ͼ���С
	int m_iCameraImagePixelSize;	// �ɼ�ͼ�����ش�С	
	int m_iBayerType{ 0 };//**[9/7/2017 ConanSteve]**:  Bayer�任���͡��������ļ���ȡ�ģ�����Halcon��׼
	int m_iFlipType{ 0 };//**[9/7/2017 ConanSteve]**: ��ת���ͣ�0����ת��1ˮƽ��ת��2��ֱ��ת��3ȫ��ת���㷨ʹ�ã�ϵͳ����
};

//���ݿ���е���Ŀ����
typedef struct _CheckResultInfo
{
	CString strStartTime;			//����ʱ��
	CString strStopTime;			//ͣ��ʱ��
	CString	strModelName;			// ģ������
	CString strTotalCount;			// �������
	CString strTotalGood;			// ��Ʒ����
	CString strTotalBad;			// ��Ʒ��
	CString strTotalBadRatio;		// ��Ʒ��
	CString strTotal1;				// ��1��λ�������
	CString strGood1;				// ��1��λ��Ʒ
	CString strBad1;				// ��1��λ��Ʒ
	CString strBadRatio1;			// ��1��λ��Ʒ��
	CString strTotal2;				// ��2��λ�������
	CString strGood2;				// ��2��λ��Ʒ
	CString strBad2;				// ��2��λ��Ʒ
	CString strBadRatio2;			// ��2��λ��Ʒ��

}s_CheckResultInfo;

//���ݿ���е���Ŀ����
typedef struct _CheckDefectInfo
{
	CString sStartTime;			//����ʱ��
	CString sStopTime;			//ͣ��ʱ��
	CString	sModelName;			// ģ������
	int iTotalCount;			// �������
	int iTotalGood;				// ��Ʒ����
	int iTotalBad;				// ��Ʒ��
	int iTotal1;				// ��λ1�������
	int iBad1;					// ��λ1��Ʒ��
	int iTotal2;				// ��λ2�������
	int iBad2;					// ��λ2��Ʒ��
	int	iDefectCount[200];		// ÿ��ȱ�ݸ���
}s_CheckDefectInfo;


typedef struct _UserInfo
{
	CString strUser;											// �û�����
	CString strMonitor;											// �೤
	CString strInspector;										// �ʼ�Ա
	CString strProductCode;										// ��Ʒ����
	CString strBatch;											// ��������
	CString strSize;											// �ͺ�
	CString strFormat;											// ���
	CString strBrand;											// �̱�
	CString strColor;											// ��ɫ
	CString strAluminumMaterial;								// ���Ʋ���
	CString strMouldNum;										// ģ�ߺ�
	CString strMachineCode;										// ������
	CString strClass;											// ���
}s_UserInfo;

typedef struct _LoginInfo
{
	CString strUserPath;										// �洢����Ա��Ϣ��·��
	CString strMonitorPath;										// �洢�೤��Ϣ��·��
	CString strInspectorPath;									// �洢�ʼ�Ա��Ϣ��·��
	CString strProductPath;										// �洢��Ʒ��Ϣ��·��
	CString strSysConfigFileName;								// ϵͳ�����ļ�·��
	CString strAlgIniFilePath;									// �㷨�����ļ�·��
	CString strBatchCode;										// ��Ʒ����
	CString strModelName;										// ģ������
	CString strUser;											// ����Ա
	CString strMonitor;											// �೤
	CString strMachineCode;										// ������
	CString strClass;											// ���
}s_LoginInfo;

typedef struct _ElementInfo
{
	int iCameraSN;			//��λ��
	int iCircleSN;			//�ɼ���
	int iImageSN;			//ͼ���
	int iIsUsing;			//�Ƿ����ڴ�д��ͼ������,0Ϊδд��1Ϊ����д��2Ϊд�����
	int iIsDetecting;		//�Ƿ����ڼ��
	BOOL bHaveError;		//�Ƿ��д�
	BOOL bIsCheck;			//�Ƿ����ڼ��
	BYTE* pAddr;			//�ڴ��ַ
	CString strErrorInfo;	//������Ϣ
}s_ElementInfo;

typedef struct _ThreadParam
{
	void* pThis;
	int iThreadID;
}s_ThreadParam;

//////////////////////////////END////////////////////////////////////////////