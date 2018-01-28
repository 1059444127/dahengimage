

/************************************************************************/
/* Copyright (c) 2016, �������ͼ���Ӿ����޹�˾  
/* All rights reserved.													

// 2015-08-21
//�Ƽ���������ͽṹ�嶨��

2016.09.21
�����������쳣�ļ���ͼ

2016-10-08
�����������ƣ�³���������Ρ��������۽���������
��������ϸ������������	(0:OK,1:NA,20:����,30:װ��,40:����,50:ƿ������,51:ƿ�����ƣ�ƿ���²�-������,52:ƿ������,60:��ʯ,70:�ܸ�,80:����)

2016-10-11 
�����������쳣��ͼ���뷽��������ͨ��s_DetectInParam����
s_DetectModelInput�������쳣ͼ���ļ���·�����㷨catch���쳣���Զ������쳣ͼ��
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


//��ʼ�������Ϣ
typedef struct _SCHECKCAMERAINFO
{
	int iHeight;  //�߶�
	int iWidth;   //���
	int iBitCount;//λ��
	
	int iType;    //�������
	int iCount;   //����
	
}s_CheckCameraInfo;

// ����㷨��ʼ��
typedef struct _DetectInitParam 
{
	int* pHWnd;						// ���ھ��(���ڴ���0��ʾ���¼����㷨����)
	int  iAlgSN;				    // �㷨��(1:�㷨һ	2:�㷨��  n:�㷨n)
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
	void* pImageInfo;	// pImageInfo����s_CheckCameraInfo�ṹ��ָ���׵�ַ

	int* pImageData;	// ͼ������
	int	iDisplayMode;	// ��ʾģʽ(0:����ʾ,1:��ʾ����,2:��ʾ��ƿ,3:��ʾ��Ʒ,4:��ʾ��Ʒ,5:��ʾ��ƿ)

	int iCircleIndex;	// ͼ��Circle��
	BOOL bSnapScreen;	// �Ƿ�ץ�� true��ʾץ����false��ʾ��ץ��
	int  iScreenIndex;  // ץ����� ͼƬ��������checksn-snapindex.jpeg������0-1.jpeg
	int	iRejectTest;	// 0��ʾ������� 1��ʾ���߷ϲ��� ȫNG�� 2��ʾ���߷Ѳ��� ȫOK��

/////2016.05.25������ͼ/////////////////////////////////////////////////////////////////////
	int iSaveType;		// ��ͼ���� 0-���� 1-��Ʒ 2-��Ʒ��-1-�쳣
	char* cSavePath;	// ��ͼ·��ָ�����ļ��У�����R:\SaveImages\AllImages\2016-10-14 09-38. �㷨����iCricleIndex��iSaveType�Լ�iCheckSN����ͼ��
						// ����ͼƬ��������format("%s\\Camera%d\\ProductID(%06d)\\Type(%d)_%02d.bmp",
											//cSavePath,iCheckSN+1,iCircleIndex,iSaveType,���);
						
//////////////////////////////////////////////////////////////////////////

	int	iReserve1;		// ����
	int iReserve2;
	int iReserve3;

}s_DetectInParam;

// ����������
typedef struct _DetectOutParam 
{  
    int		iErrorType;					// ��������	(0:OK,1:NA,20:����,30:װ��,40:����,50:ƿ������,51:ƿ�����ƣ�ƿ���²�-������,52:ƿ������,60:��ʯ,70:�ܸ�,80:����)
	int		iBubbleValue;				// ���ݱ�־0--û�����ݣ�1--������(iReserve1��ΪiBubble)
	int		iImageBrightness;			// ͼ�����ȣ�iReserve2��ΪiLightValue��
	int     iTimeConsuming;				// ����ʱ
	
	int		iDefectCount;				// ȱ�ݸ���

	char	chCheckInfo[MAX_LENGTH];	// ������Ϣ
	
	int		iReserve1;
	int		iReserve2;
	int		iReserve3;

}s_DetectOutParam;

typedef struct _DetectModelInput
{
	char    chPath[MAX_LENGTH];		// ����·��(����..\Run\Prescription\Alg)
	char    chLog[MAX_LENGTH];     	// ��־�ļ�·��(��¼�����޸ļ�¼)
	char    chImageBufferPath[MAX_LENGTH];//�������ͼ·�� ..\ImageBuffer

	
	// ǰ�˴���·����ʽ��R:\SaveImages\AbnormityImages,�㷨�Լ�������Ŀ¼
	// �����쳣ʱ���������ӣ�R:\SaveImages\AbnormityImages\2016-10-14 09-38-00\Camera1\\ProductID(000001)\Type(-1)_01.bmp
	char    chAbnormalPath[MAX_LENGTH];	//�쳣ͼƬ���·�����㷨catch���쳣ʱ��ͼ��

}s_DetectModelInput;

#endif // !defined(AFX_ALGRITHM_INTERFACE_H_)

