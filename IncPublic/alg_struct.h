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
//#include "typeinfo"

enum e_BayerHalcon { //**[12/13/2017 ConanSteve]**:  1:GB,2:GR,3:BG,4:RG
	e_BayerHalcon_NONE =0,
	e_BayerHalcon_GB =1,
	e_BayerHalcon_GR = 2,
	e_BayerHalcon_BG = 3,
	e_BayerHalcon_RG = 4
};
//**[1/17/2018 ConanSteve]**:  ��������
enum e_CallType
{
	e_CallType_RealTime,
	e_CallType_Demo,
	e_CallType_Simulation
};
/*************************************<Demoʹ�ýṹ��>************************************[1/17/2018 ConanSteve]*/
typedef struct s_DemoAlgSetParam
{
	TCHAR	chModelName[STATUS_STRING_MAXLENGTH];   //DEMOѡͼ��·���������ļ��к͵���ͼ������Ϣ��⣬���ֻ������ͼ�㷨��Ҫ�����ļ���Ŀ¼
	CWnd*	pParent;	                            //����ɾ������ģ��ʱ��NULL
	CRect	rect;
	bool	bTopWin;	                            //�Ƿ�Ϊ��ǰ���ڣ�ģ�崰�ڳ�ʼ��ʱ��ǰ��ʾ��һ·ѡ���
	int     iCheckSN;                               //��ǰ���ڼ�·�����
	int iReserve1;//**[8/10/2017 ConanSteve]**:  ������4��������
	int iReserve2;
	int iReserve3;
	int iReserve4;//**[8/10/2017 ConanSteve]**:  �˱�����ΪרΪ��չ���ܶ���ʹ��

	s_DemoAlgSetParam()
	{
		memset(chModelName, 0, sizeof(chModelName));
		pParent = NULL;
		rect = 0;
		bTopWin = false;
		iCheckSN = 0;
		iReserve1 = iReserve2 = iReserve3 = iReserve4 = 0;
	}

}s_DemoAlgSetParam;

/*************************************<�Ĵ��㷨�ӿ�>************************************[1/17/2018 ConanSteve]*/
namespace Alg
{
	// ����ִ��״̬��Ϣ
	typedef struct s_Status
	{	
		int  iCheckSN;                                      //����󷵻ظ����ˡ����·������0��ʼ���޹�λ����
		int	 iStatusType;									//����״̬����
		TCHAR* pErrorInfo;									//������Ϣ����
		void* pThis;										//���ӵ�лص������Ķ����thisָ��
		void* pParam;                                       //����ṹ��
		s_Status()
		{
			iCheckSN = 0;
			iStatusType = STATUS_NOR;
			pErrorInfo = NULL;
			pThis = NULL;
			pParam = NULL;
		}
	}s_Status;

	// �޷Ͻṹ��
	typedef struct s_KickParam
	{
		void*	pThis;									//���ӵ�лص������Ķ����thisָ��
		void*	pParam;									//����޷Ͻ��
		TCHAR*   pResult;								//��ŷ��ؽ������

		int		iKickSN;								//�޷Ͽڱ��
		int		iImageSN;								//ͼ���
		
		int		iReserve1;                              //������
		int		iReserve2;                              //������
		int		iReserve3;                              //������
		int     iReserve4;                              //������

		s_KickParam()
		{
			pThis = NULL;
			pParam = NULL;
			pResult = NULL;
			iKickSN = 0;
			iImageSN = 0;
			iReserve1 = iReserve2 = iReserve3 = iReserve4 = 0;
		}

		
	}s_KickParam;
	
	// ��������Ļص���
	typedef void (WINAPI *PKICKCALLBACK)(const s_KickParam* sKickParam);
	typedef void (WINAPI *PSTATUSCALLBACK)(const s_Status* sStatusParam);
	
	// �����������ʼ���ṹ��
	typedef struct s_CheckInitParam
	{
		PKICKCALLBACK KickCallbackFunc;				//�޷ϻص�
		PSTATUSCALLBACK StatusCallbackFunc;			//״̬�ص�	301����ͼ��ص� 302��ͼ�ص�	

		TCHAR* pFilePath;							//�����ļ�·�������ڶ�ȡ��̬�����ƣ����������
		void* pThis;								//���ӵ�лص������Ķ����thisָ��
		void* pParam;                               //����ṹ�壬����������й�λ������ָ��(ͬʱ����ͼ����ʱ��ϵͳ���㷨��������)
		
		int iCallType;								//�������ͣ�0�����˶�����ֳ����ã�1��DEMO���������	2:���˶����ģ�����

		int iReserve1;								//������		����Ϊ0��ҩҺΪ1
		int iReserve2;								//������
		int iReserve3;								//������
		int iReserve4;								//������

		s_CheckInitParam()
		{
			KickCallbackFunc = NULL;
			StatusCallbackFunc = NULL;
			pFilePath = NULL;
			pThis = NULL;
			pParam = NULL;
			iCallType = 0;
			iReserve1 = iReserve2 = iReserve3 = iReserve4 = 0;
		}
		
	}s_CheckInitParam;

	//��������㷨��ʼ���ṹ��
	typedef struct s_CheckInitSingleParam
	{		
		void* pParam;                   //����ṹ��

		int iCheckSN;                   //���·����0��ʼ���޹�λ����㷨���������ļ����á�
		int	iObjIndex;		            //��ǰ���õĶ���ţ�0��ʼ���㷨�м�¼��ǰ�ڼ�����������㷨��
		
		int iReserve1;                  //������
		int iReserve2;                  //������
		int iReserve3;                  //������
		int iReserve4;                  //������

		s_CheckInitSingleParam()
		{
			pParam = NULL;
			iCheckSN = 0;
			iObjIndex = 0;
			iReserve1 = iReserve2 = iReserve3 = iReserve4 = 0;
		}
		
	}s_CheckInitSingleParam;

	// ����������
	typedef struct s_CheckInParam 
	{
		void* pParam;       //����ṹ��
		char* pImageData;	//ͼ�����ݵ�ַ

		int iImageSN;		//ͼ����źţ����˴�������󣬿����ڼ�¼
		int iCheckSN;       /**Camera SN              [5/4/2017 ConanSteve]**/
		
		int	iReserve1;		//������
		int	iReserve2;		//������
		int iReserve3;      //������
		int iReserve4;      //������
		s_CheckInParam()
		{
			pParam = NULL;
			pImageData = NULL;
			iImageSN = 0;
			iCheckSN = 0;
			iReserve1 = iReserve2 = iReserve3 = iReserve4 = 0;
		}
	}s_CheckInParam;

	enum e_OptionalFunction
	{

	};

	

	// ����������
	typedef struct s_CheckOutParam 
	{	
		TCHAR*	pFilePath;								//��ͼ·�� ÿ��ͼ�����·���ԡ�\n��Ϊ�ָ�����
		void*   pParam;                                 //����ṹ��       
		
		int     iCheckSN;                               //��ǰ���ڼ�·���㷨���ظ����ˣ�����ǵڼ�·�����㷨�޹ء�
		int		iImageSN;								//ͼ���
		int	    iDisplay;                               //0:����ʾ��1����ʾ; ��������㷨֮�����ʾ�ӿڡ��㷨ͨ��������ʾģʽ�����ظ�������Ƿ���ʾ��������1ʱ���������㷨CopyRectangle	
		int		iSave;									//�Ƿ񱣴�ͼ�� 1��ͼ�� 2����
		int     iImageNum;								//����ͼ���������������ɼ�����ͼ����
		int		iPathLength;							//��ͼ·�����ȣ��뱣��ͼ�����

		int		iReserve1;                              //������
		int		iReserve2;                              //������
		int		iReserve3;                              //������
		int     iReserve4;                              //������	

		s_CheckOutParam()
		{
			pFilePath = NULL;
			pParam = NULL;
			iCheckSN = 0;
			iImageSN = 0;
			iDisplay = 0;
			iSave = 0;
			iImageNum = 0;
			iPathLength = 0;
			iReserve1 = iReserve2 = iReserve3 = iReserve4 = 0;
		}
	}s_CheckOutParam;
	
	//ģ���������
	typedef struct s_AlgModelInput
	{
		void*   pParam;                                 //�������㷨,����ṹ��

		int     iCheckSN;                               //��ǰ���ڼ�·�����˴�����������ڵ��ø�·set�����㷨�޹ء�
		int		iOperSig;
		//ϵͳ������  0��ģ�壬1Ϊ���ü�����(�����㷨�Ƿ���ʾͼ��) 
		//��������㷨��0��������300���鴰��copyWindow��ʵ����
		/*DEMO���㷨��
		-2��ʾģ�崰�ڣ�
		-1����ģ�崰��(�ⲿ���tabҳ�л�ʱʹ��)��
		0��ģ�壬
		1ϵͳ��DEMO�����ڣ�
		2˫��Ӧ�ô�DEMO�����ڣ�
		3ɾ��ģ��, 
		4����ģ��, 
		5�������,
		6�ƶ�����
		41�ñ���ͼ�񱣴��ģ��ͼ��,  
		51����ˢ���棬
		52������ sRst.nStatusType=0��
		53����ģ��֮�����ʾ����
		101�������ͼ��
		102ֻ��ʾͼ��
		103����ģ��ͼ��
		104���ȡͼ���ԣ�
		105ѯ���Ƿ��ڻ�
		202ѯ���Ƿ�Ҫ���� sRst.nStatusType=0����
		203��ȡ�汾��
		300copyWindow
		301SaveImage
		1001-1500Ϊ����λ����
		//**[12/26/2017 ConanSteve]**:  601:���ö�������
		*/
																
		int		iReserve1;                              //������		
		int		iReserve2;                              //������		
		int		iReserve3;                              //������     
		int     iReserve4;                              //������
		s_AlgModelInput()
		{
			pParam = NULL;
			iCheckSN = 0;
			iOperSig = 0;
			iReserve1 = iReserve2 = iReserve3 = iReserve4 = 0;
		}
	}s_AlgModelInput;
}

enum e_AlgModelInputOper {
	e_AlgModelInputOper_LoadModel = 0,//**[10/9/2017 ConanSteve]**:  ϵͳ���ö�ģ��
	e_AlgModelInputOper_OpenWindows = 1,//**[10/9/2017 ConanSteve]**:  Demo���ô򿪴���
	e_AlgModelInputOper_MoveWindow = 6,//**[10/18/2017 ConanSteve]**:  Demo���ã��ƶ�����
	e_ALgModelInputOper_ShowAlgModelWnd = 101,//**[10/9/2017 ConanSteve]**:  Demo���ã���ʾ�㷨 ����
	e_AlgModelInputOper_HideAlgModelWnd = 102,//**[10/18/2017 ConanSteve]**:  Demo���ã������㷨 ����
	e_AlgModelInputOper_GetVersion = 203, //**[10/18/2017 ConanSteve]**:  ��ȡ�㷨�汾
	e_AlgModelInputOper_CopyWindow = 300,//**[10/9/2017 ConanSteve]**:  �㷨�����鴰����Ϣ��ʵ����
	e_AlgModelInputOper_SaveImage = 301,//**[7/26/2017 ConanSteve]**:  ����ͼ��
	e_AlgModelInputOper_Reset = 601,//**[12/26/2017 ConanSteve]**:  ���ö�������
	e_AlgModelInputOper_ShowRunInfo = 602,//**[1/17/2018 ConanSteve]**:  ��ʾ�����������Ϣ����
	e_AlgModelInputOper_UseExtenInfoFunc = 666,//**[7/26/2017 ConanSteve]**:  ʹ����չ���ܽṹ��
};

/*****************************************************************************************/

#endif // !defined(AFX_CHECK_H__3D35A3B4_D261_4116_8EBD_714891F3AEF3__INCLUDED_)