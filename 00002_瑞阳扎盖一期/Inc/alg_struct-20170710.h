#if !defined(AFX_CHECK_H__3D35A3B4_D261_4116_8EBD_714891F3AEF3__INCLUDED_)
#define AFX_CHECK_H__3D35A3B4_D261_4116_8EBD_714891F3AEF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STATUS_STRING_MAXLENGTH	256			//����������Ϣ������ַ���

//��������״̬����
#define STATUS_THREAD_BUSY			-1		//�����߳�æ
#define STATUS_NOR					0		//����
#define STATUS_ILG					1		//�����Ƿ�
#define STATUS_THROW_IMAGE			2		//��������������ʼ��ͼ[20160531 zhy]
#define STATUS_KICK_TIMEOUT			3		//��ⳬʱ [20160531 zhy]
#define STATUS_ALG_ABNORMAL			4		//�㷨����쳣 [20160531 zhy]
#define STATUS_STOP_SAVEIMAGE		301		//ֹͣ����ͼ�� [20160531 zhy]

//��Ϣ
#define DHOA_MSG_CHECK_FINISH   WM_USER+4001

namespace Alg
{
	// ����ִ��״̬��Ϣ
	typedef struct _Status
	{	
		int  iCheckSN;                                      //����󷵻ظ����ˡ����·������0��ʼ���޹�λ����
		int	 iStatusType;									//����״̬����
		char* pErrorInfo;									//������Ϣ����
		void* pThis;										//���ӵ�лص������Ķ����thisָ��
		void* pParam;                                       //����ṹ��
	}s_Status;

	// �޷Ͻṹ��
	typedef struct _KickParam
	{
		void*	pThis;									//���ӵ�лص������Ķ����thisָ��
		void*	pParam;									//����޷Ͻ��
		char*   pResult;								//��ŷ��ؽ������

		int		iKickSN;								//�޷Ͽڱ��
		int		iImageSN;								//ͼ���
		
		int		iReserve1;                              //������
		int		iReserve2;                              //������
		int		iReserve3;                              //������
		int     iReserve4;                              //������
		
	}s_KickParam;
	
	// ��������Ļص���
	typedef void (WINAPI *PKICKCALLBACK)(const s_KickParam* sKickParam);
	typedef void (WINAPI *PSTATUSCALLBACK)(const s_Status* sStatusParam);
	
	// �����������ʼ���ṹ��
	typedef struct _CheckInitParam
	{
		PKICKCALLBACK KickCallbackFunc;				//�޷ϻص�
		PSTATUSCALLBACK StatusCallbackFunc;			//״̬�ص�	301����ͼ��ص� 302��ͼ�ص�	

		char* pFilePath;							//�����ļ�·�������ڶ�ȡ��̬�����ƣ����������
		void* pThis;								//���ӵ�лص������Ķ����thisָ��
		void* pParam;                               //����ṹ�壬����������й�λ������ָ��(ͬʱ����ͼ����ʱ��ϵͳ���㷨��������)
		
		int iCallType;								//�������ͣ�0�����˶�����ֳ����ã�1��DEMO���������	2:���˶����ģ�����

		int iReserve1;								//������
		int iReserve2;								//������
		int iReserve3;								//������
		int iReserve4;								//������
		
	}s_CheckInitParam;

	//��������㷨��ʼ���ṹ��
	typedef struct _CheckInitSingleParam
	{		
		void* pParam;                   //����ṹ��

		int iCheckSN;                   //���·����0��ʼ���޹�λ����㷨���������ļ����á�
		int	iObjIndex;		            //��ǰ���õĶ���ţ�0��ʼ���㷨�м�¼��ǰ�ڼ�����������㷨��
		
		int iReserve1;                  //������
		int iReserve2;                  //������
		int iReserve3;                  //������
		int iReserve4;                  //������
		
	}s_CheckInitSingleParam;

	// ����������
	typedef struct _CheckInParam 
	{
		void* pParam;       //����ṹ��
		char* pImageData;	//ͼ�����ݵ�ַ

		int iImageSN;		//ͼ����źţ����˴�������󣬿����ڼ�¼
		int iCheckSN;       //���·����0��ʼ���޹�λ������˴�����������ڵ��ø�·�㷨�����㷨�޹ء�
		
		int	iReserve1;		//������
		int	iReserve2;		//������
		int iReserve3;      //������
		int iReserve4;      //������
	}s_CheckInParam;

	// ����������
	typedef struct _CheckOutParam 
	{	
		char*	pFilePath;								//��ͼ·�� ÿ��ͼ�����·���ԡ�\n��Ϊ�ָ�����
		void*   pParam;                                 //����ṹ��       
		
		int     iCheckSN;                               //��ǰ���ڼ�·���㷨���ظ����ˣ�����ǵڼ�·�����㷨�޹ء�
		int		iImageSN;								//ͼ���
		int	    iDisplay;                               //0:����ʾ��1����ʾ; ��������㷨֮�����ʾ�ӿڡ��㷨ͨ��������ʾģʽ�����ظ�������Ƿ���ʾ��������1ʱ���������㷨CopyRectangle	
		int		iSave;									//�Ƿ񱣴�ͼ��
		int     iImageNum;								//����ͼ���������������ɼ�����ͼ����
		int		iPathLength;							//��ͼ·�����ȣ��뱣��ͼ�����

		int		iReserve1;                              //������
		int		iReserve2;                              //������
		int		iReserve3;                              //������
		int     iReserve4;                              //������	
	}s_CheckOutParam;
	
	//ģ���������
	typedef struct _AlgModelInput
	{
		void*   pParam;                                 //�������㷨,����ṹ��
		int     iCheckSN;                               //��ǰ���ڼ�·�����˴�����������ڵ��ø�·set�����㷨�޹ء�
		int		iOperSig; 
		//ϵͳ������  0��ģ�壬1Ϊ���ü�����(�����㷨�Ƿ���ʾͼ��) 
		//��������㷨��0��������300���鴰��copyWindow��ʵ����
		/*DEMO���㷨��
		-2��ʾģ�崰�ڣ�-1����ģ�崰��(�ⲿ���tabҳ�л�ʱʹ��)��0��ģ�壬1ϵͳ��DEMO�����ڣ�2˫��Ӧ�ô�DEMO�����ڣ�
		3ɾ��ģ��, 4����ģ��, 41�ñ���ͼ�񱣴��ģ��ͼ��, 5�������, 6�ƶ�����
		51����ˢ���棬52������ sRst.nStatusType=0��53����ģ��֮�����ʾ����
		101�������ͼ��102ֻ��ʾͼ��103����ģ��ͼ��104���ȡͼ���ԣ�105ѯ���Ƿ��ڻ�
		202ѯ���Ƿ�Ҫ���� sRst.nStatusType=0����
		203��ȡ�汾��
		300copyWindow
		301SaveImage
		1001-1500Ϊ����λ����
		*/														
		int		iReserve1;                              //������
		int		iReserve2;                              //������
		int		iReserve3;                              //������
		int     iReserve4;                              //������
	}s_AlgModelInput;


	//*[7/7/2017 ConanSteve]*:  ������ͼ��ز�������ַռ��s_CheckInParam������3
	typedef struct s_ExtenPara {
		int iSize; //*[7/7/2017 ConanSteve]*:  �ṹ���С
		int iImgNum; //*[7/7/2017 ConanSteve]*:  ÿ�����������Ʒ���ĵ�����ͼ�����
		int iImgWidth; //*[7/7/2017 ConanSteve]*:  ����ͼ�Ŀ��
		int iImgHeight; //*[7/7/2017 ConanSteve]*:  ����ͼ�ĸ߶�
		int iImgChannel; //*[7/7/2017 ConanSteve]*:  ͨ����
		bool bIsCircleSaveSaveImg; //*[7/10/2017 ConanSteve]*:  �Ƿ�ѭ����ͼ
		char chSaveImgPath[512]; //*[7/7/2017 ConanSteve]*:  ��ǰ���ͼ���ļ���·��
	}s_ExtenPara;

}

#endif // !defined(AFX_CHECK_H__3D35A3B4_D261_4116_8EBD_714891F3AEF3__INCLUDED_)