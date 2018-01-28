/************************************************************************/
/* Copyright (c) 2014, �������ͼ���Ӿ����޹�˾  
/* All rights reserved.													

// 2015-08-21
//�Ƽ���������ͽṹ�嶨��
/************************************************************************/
#include <vector>
using namespace std;

#if !defined(AFX_DHALGINTERFACE_H_)
#define AFX_DHALGINTERFACE_H_


#define MAX_LENGTH		 2048		

//�������Ͷ���
enum e_ErroeType
{
	Status_OK,   	    	         //����
	Status_Err_LicenseInvalid,       //Halcon����ļ���ʧ
	Status_Err_LicenseMismatch,      //Halcon�汾��ƥ��
	Status_Err_WinFailed,            //����ʾ����ʧ��
	Status_Err_ParaIni,	             //�����ļ���ȡ����
	Status_Err_Model,                //ģ���ļ�����
	Status_Err_ImageLess,  	         //ͼ�����ݲ������
	Status_Err_ImageBlack,           //����ͼ���д��ں�ͼ
	Status_Err_TimeOut,              //��ʱ
	Status_Err_HalconAbnor,          //halcon�㷨�쳣
	Status_Err_CheckFailed,          //demo����쳣
	Status_Err_LightFailed,          //��Դ���ȴ���
	Status_Err_Other,                //������������
};

//ȱ�����Ͷ���
enum e_DefectType
{ 
	OK,                              //��Ʒ 
	NA,                              //��ƿ 
	glass,                           //���� 
	fiber,                           //��ά
	foregionparticle,                //����
	cap,                             //ƿ�����
	body,                            //ƿ�����
	capacity,                        //װ��
	other                            //����
};

//��ʼ�������Ϣ
typedef struct _SCHECKCAMERAINFO
{
	int iHeight;  //�߶�
	int iWidth;   //���
	int iBitCount;//λ��
	
	int iType;    //�������
	int iCount;   //����
	
}s_CheckCameraInfo;

// ��ʼ��
typedef struct _DetectInitParam 
{
	int* pHWnd;						// ���ھ��(���ڴ���0��ʾ���¼����㷨����)
	int  iAlgSN;				        // �㷨��(1:�㷨һ	2:�㷨��  n:�㷨n)
	int  iRunMode;					// 0-ONLINE 1-OFFLINE,�������ߺ����߳���
		
	char chLanPath[MAX_LENGTH];		// �����ļ�·�� 
	char chLanguage[MAX_LENGTH];	// 2014-09-15�������� Chinese\English\Russian...

	int  iReserve1;
	int  iReserve2;
	int  iReserve3;
	int  iReserve4;
	
}s_DetectInitParam;


// ����������
typedef struct _DetectInParam 
{
	vector<s_CheckCameraInfo> vCameraInfo;   //ͼ����Ϣ
	int* pImageData;	// ͼ������
	int	iDisplayMode;	// ��ʾģʽ(0:����ʾ,1:��ʾ��ƿ��ƿͼ��,2:��ʾ��������ͼ��3:ֻ��ʾ��ƿͼ��4:ֻ��ʾ��Ʒͼ��5:ֻ��ʾ��ƿͼ��)

	int iCircleIndex;	// 2014-09-15ͼ��Circle��
	BOOL bSnapScreen;	// �Ƿ�ץ�� true��ʾץ����false��ʾ��ץ��
	int  iScreenIndex;  // ץ����� ͼƬ��������checksn-snapindex.jpeg������0-1.jpeg
	int	iReserve1;		// ����
	int	iReserve2;		// ����
	int iReserve3;
	int iReserve4;

}s_DetectInParam;

// ����������
typedef struct _DetectOutParam 
{  
    int		iErrorType;					// ��������	ˮ��(0:OK,1:NA,2:����,3:��ά,4:����,5:���,6:Һλ)
										//			����(0:OK,1:NA,2:����,3:���(����/��ƿ),4:װ��,5:����,6:����)
	int		iBubbleValue;				// ���ݱ�־0--û�����ݣ�1--������(iReserve1��ΪiBubble)
	int		iImageBrightness;			// ͼ�����ȣ�iReserve2��ΪiLightValue��
	int     iTimeConsuming;				// 2014-07-22����ʱ
	
	int		iDefectCount;				// 2014-07-22ȱ�ݸ���

	char	chCheckInfo[MAX_LENGTH];	// ������Ϣ
	
	int		iReserve1;
	int		iReserve2;
	int		iReserve3;

}s_DetectOutParam;

//
typedef struct _DetectModelInput
	{
		char    chPath[MAX_LENGTH];		// ����·��(����..\Run\Prescription\Alg)
		char    chLog[MAX_LENGTH];     	// ��־�ļ�·��(��¼�����޸ļ�¼)
	    char    chImageBufferPath[MAX_LENGTH];//�������ͼ·�� ..\ImageBuffer
	}s_DetectModelInput;

#endif // !defined(AFX_ALGRITHM_INTERFACE_H_)

