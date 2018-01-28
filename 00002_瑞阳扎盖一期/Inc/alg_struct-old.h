#if !defined(AFX_CHECK_H__3D35A3B4_D261_4116_8EBD_714891F3AEF3__INCLUDED_)
#define AFX_CHECK_H__3D35A3B4_D261_4116_8EBD_714891F3AEF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STATUS_STRING_MAXLENGTH	256			//����������Ϣ������ַ���
//��������״̬����
#define STATUS_NOR	             0     //����
#define STATUS_ILG			    1	   //�����Ƿ�
#define STATUS_PFW				 2	   //ͼ���ʽ����
#define STATUS_FOF               3     //�ļ�����ʧ��
#define	STATUS_LLF	             4	   //����licenseʧ��

#define STATUS_MOS				 5     //ģ������ɹ�
#define STATUS_MOF				 6     //ģ�����ʧ��
#define STATUS_MOC				 7     //ģ�����ȡ�� 

#define STATUS_TRAIN_FAIL        8     //ѵ��ʧ��
#define STATUS_FILE_OPERATE_FAIL 9	   //�ļ�����ʧ��
#define STATUS_OTHER			 10	   //����ԭ��  
#define STATUS_THREAD_BUSY       11	   //�����߳�æ
#define STATUS_FATAL_ERROR		 12    //�㷨����
#define STATUS_BAD_INSPCT		 13	   //�д���Ľ���



//ģ�������־
#define READ_MODEL				0	//��ȡģ��
#define NEW_MODEL				1	//�½�ģ��
#define MODIFY_MODEL			2	//�޸�ģ��
#define DELETE_MODEL			3	//ɾ��ģ��

/*ʣ�µ��Ժ���������*/
#define NUM_THREAD				12	//�߳���
#define NUM_BL_CAM				4	//���������⽺�Ҹ���
#define THREAD_NUM_ILLEGAL		-1  //�̸߳������Ϸ�


//���й�λ nCapSuleStatus
#define ONLY_INIT				-2	//��ʼ��
#define EMPTY_SLOT				-1	//��λ��
#define GOOD_CAPSULE			0	//�ý���
#define DEFECT_FIND				1	//Sobel���ֵ�ȱ��
#define LOCATE_FAIL				2	//��λʧ��
#define BAD_INCISION			3	//�п�ȱ��
#define WRONG_SIZE				4	//�ߴ����

//������λ
#define NO_CHECK_RECTS			0	//FrontLocate��ģ��ʱδ�ҵ������ļ�
#define FRONTLOCATE_PARA_ILL	-1	//FrontLocateѵ�������Ĳ����Ƿ�
#define MONO_CAPSULE			5	//��Ƭ
#define CROP_EXCEPTION			6	//crop_domainʱ����
#define	WRONG_COLOR				7	//��ɫ����
#define BAD_REFLECTION			8	//����������

namespace Alg{
	
	//ͼ����Ϣ�ṹ������
	typedef struct _ImageInfo
	{

	}s_ImageInfo;


	// ����ִ��״̬��Ϣ
	typedef struct _Status
	{
		int		nStatusType;									//����״̬����
		char	chErrorInfo[STATUS_STRING_MAXLENGTH];			//������Ϣ����
		char    chErrorContext[STATUS_STRING_MAXLENGTH];		//������ϸ��Ϣ����	
		void*   Empty;
	}s_Status;

	// �㷨��ʼ������
	typedef struct _AlgInitParam
	{
		int		nWidth[10];         //ͼ����
		int		nHeight[10];	    //ͼ��߶�
		CDC		*pDC[15];			//��󻯴��ھ��  1,2,3,4,4.1,4.2,4.3,4.4,5,5.1,5.2,5.3,5.4
		int		nCameraNum[10];		//�������
	}s_AlgInitPara;

	typedef struct _AlgSingleInit
	{
		int		nWidth;         //ͼ����
		int		nHeight;	    //ͼ��߶�
		CDC		**pDC;			//���ھ��
		int		nCameraIndex;	//�����λ��
		int		nObjIndex;		//�ڼ�������
	}s_AlgSingleInit;


	// ���������� algorithm check input parameter
	typedef struct _AlgInspectionInputParam
	{
		int		nWidth;			//ͼ���
		int		nHeight;		//ͼ���
		int		nChannel;		//ͨ����			<0ʱ ΪHalcon��ͼ������
		int		nCamIndex;		//�����			
		char	*pcImageData;	//ͼ������	
		int		bRunCheck;		//�Ƿ��� 0���� 1��
		int		nInSig;			//����Ϊ0 !!!	2ֻ��ʾͼ
		int		bDsp;			//0����ʾ 1ȫ��ʾ��Ե 2��ʾ����ͼ���Ե	101ȫ��ʾ��� 102�������
	}s_AlgInspInPara;

	//ģ���������
	typedef struct _AlgModelInput
	{
		char	chModelName[255];
		int		nOperSig;
		/*
		-2��ʾģ�崰�ڣ�-1����ģ�崰��(�ⲿ���tabҳ�л�ʱʹ��)��0��ģ�壬1ϵͳ��DEMO�����ڣ�2˫��Ӧ�ô�DEMO�����ڣ�
		3ɾ��ģ��, 4����ģ��, 41�ñ���ͼ�񱣴��ģ��ͼ��, 5�������, 6�ƶ�����
		51����ˢ���棬52������ sRst.nStatusType=0��53����ģ��֮�����ʾ����
		101�������ͼ��102ֻ��ʾͼ��103����ģ��ͼ��104���ȡͼ���ԣ�105ѯ���Ƿ��ڻ�
		202ѯ���Ƿ�Ҫ���� sRst.nStatusType=0����
		203��ȡ�汾��
		300copyWindow
		1001-1500Ϊ����λ����
		*/
		CWnd*	pParent;	//����ɾ������ģ��ʱ��NULL
		CRect	rect;
		bool	bTopWin;	//�Ƿ�Ϊ��ǰ���ڣ�ģ�崰�ڳ�ʼ��ʱ��ǰ��ʾ��һ·ѡ���
	}s_AlgModelInput;


	// ���������� algorithm check output parameter
	typedef struct _CapsuleStatus
	{
		int		nStatus;
		char	chInfo[255];	//�Ƕ���Ϣ
	}s_CapsuleStatus;

	typedef struct _AlgInspectionReslut
	{
		int		nAlgStatus;		//0:�� 1:�л��� 2:�㷨�쳣 -1:ȫ��
		int     nErrorLink;		//0Ϊ����ƿ 1��ƿ
		char	chExpInfo[255];
		s_CapsuleStatus		sCapules[20];
		void*   Empty;
	}s_AlgInspResult;

	// ���������� algorithm model parameter
	// SetAlgParam����ʹ��
// 	typedef struct _AlgModelPara
// 	{
// 		char	chModelName[MAX_PATH];
// 		int		nCamNumber;
// 		int		nOperateSig;//0��ģ�壬1�½�ģ�壬2�޸�ģ�壬3ɾ��ģ��
// 	}s_AlgModelPara;

	//Rectangle
	typedef struct _CRect
	{
		int nLefttopR;
		int nLefttopC;
		int nMiddleR;
		int nMiddleC;
		int nRightBottomR;
		int nRightBottomC;
	}s_CRect;

	//Circle
	typedef struct _CCircle
	{
		double	dRadius;
		int		nRow;
		int		nColumn;
	}s_CCircle;

	// ���������� algorithm model parameter

	// ģ�����ó�ʼ����Ϣ
	typedef struct _AlgModelInit
	{		
	}s_AlgModelInit;

	//���������ó�ʼ����Ϣ
	typedef struct _AlgChecklInit
	{
   	}s_AlgCheckInit;

	// �㷨��Ϣ�ṹ��
	typedef struct _AlgInfo
	{
		CString DllVersion;	
	}s_AlgInfo;
}

#endif // !defined(AFX_CHECK_H__3D35A3B4_D261_4116_8EBD_714891F3AEF3__INCLUDED_)