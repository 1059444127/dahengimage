
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the IOCARD_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// IOCONTROL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef IOCARD_EXPORTS
#define IOCONTROL_API __declspec(dllexport)
#else
#define IOCONTROL_API __declspec(dllimport)
#endif

// This class is exported from the IoCard.dll

#ifndef INCLODE_IOCARD_6356
#define  INCLODE_IOCARD_6356

#define		BOARDTYPE_3S500		0
#define		BOARDTYPE_LX45		1

typedef	struct _CTRLOUT_RESULT {
	int		nline;
	int		nIndex;
	BYTE	mOP;
	bool	mNeedDelay;
	int		mDelay;//0-127
}CTRLOUT_RESULT,*PCTRLOUT_RESULT;


typedef	struct _CTRLOUT_RESULT_INDEP {
	int		nline;
	int		nIndex;
	int		nResult;
}CTRLOUT_RESULT_INDEP,*PCTRLOUT_RESULT_INDEP;

typedef struct  _CTRL_TIMEREG_LX45
{
	DWORD	m_timeCnt10us;
	
	WORD	m_HCntRem0;
	WORD	m_Fcnt0;
	DWORD	m_HCntAll0;

	WORD	m_HCntRem1;
	WORD	m_Fcnt1;
	DWORD	m_HCntAll1;

	WORD	m_HCntRem2;
	WORD	m_Fcnt2;
	DWORD	m_HCntAll2;

	WORD	m_HCntRem3;
	WORD	m_Fcnt3;
	DWORD	m_HCntAll3;
}CTRL_TIMEREG_LX45,*PCTRL_TIMEREG_LX45;

typedef struct  _CTRL_TIMEREG
{
	DWORD	m_timeCnt10us;
	DWORD	m_HCntAll1;
	DWORD	m_HCntAll2;
	int		m_HCntRem1;
	int		m_HCntRem2;
	int		m_Fcnt1;
	int		m_Fcnt2;
}CTRL_TIMEREG,*PCTRL_TIMEREG;


#define		EN_FPGATIMEINT		0x02
#define		EN_INPUTINT			0x04
#define		EN_C51INT			0x01


// �ص����������ṹ��
typedef struct IOParam
{
	PVOID pContext;			// ���thisָ��
	int iParam1;			// �������кż�����ֵ
	int iParam2;			// ����ʱ����Ϣ
	int iMsgType;			// ��Ϣ����
	
	int iReserve1;			// ������1
	int iReserve2;			// ������2
	int iReserve3;			// ������3
	int iReserve4;			// ������4
	
}s_IOParam;
typedef void (WINAPI* PIOCALLBACK)(const s_IOParam* pParam);
// ��ʼ�������ṹ��
typedef struct InitParam
{
	PVOID pContext;			// thisָ��
	char* strPath;			//��ʼ��·��
	PIOCALLBACK pFunc;		//�ص�����
	
	int iReserve1;			// ������1
	int iReserve2;			// ������2
	int iReserve3;			// ������3
	int iReserve4;			// ������4	
}s_InitParam;

class __declspec(dllexport) CioControl
{
	BOOL  ReadWorkMode2Param(const char* strPath);
	BOOL  ReadParameterFile_NewLX45(const char* strPath);
	BOOL  ReadParameterFile_OLDS500(const char* strPath);
	void  SetCodeFramelg_LX45();
public:
	CioControl();
	virtual ~CioControl();

	void* m_pData;
	int		mWork_Mode;
	int		mCntVOutDelay;
	int		mKeepExBase_t;
	int		mKeepExBase_h;
	int		mRunType1;
	int		mRunType2;
	int		mCode1dsType;
	int		mCode2dsType;
	int		mCode1hMax;
	int		mCode2hMax;
	int		mCodehNum[4];//LX45�ĸ���������ÿ֡����
	DWORD   mInCoeOut;//�����������������
	DWORD   mEnInIntFlag;//�����������ӳ��ж�ʹ��
	DWORD   mInIntDelayms;//�����ӳ��ж��ӳ�ʱ��
	BOOL    m_bOpenCardOK;//��������
	s_InitParam* m_pInitParam;//��ʼ���ṹ��
	BOOL	m_bRejectTest;//�߷Ѳ���
	int		m_nRejectQueue;
	int		m_nRejectOutPort;
	int		m_nRejectMode;
	CString m_strInitFile;
	BOOL    m_bSetParamInternal;
	CRITICAL_SECTION cs;// ���̶߳��������

	//�豸����
	BOOL Open(int mDev,HWND hWnd=NULL);
	BOOL SetMsgWnd(HWND hWnd=NULL);
	BOOL Close();
	BOOL IsOpen();
	int  ReadBoardNo();//���忨��
	//�豸������汾��Ϣ
	int  ReadBoardInfo(char *sysVer);//���忨�����Ӳ��ϵͳ�汾��Ϣ
	int  GetSoftVer(char *SoftVer);//���忨���������汾��Ϣ:DLL/LIB
	int  GetHardVer(char *HardVer);//���忨����Ĺ̼��汾��Ϣ:FPGA/C51/����
	
	BOOL  WriteBoardNo(int nbd);//д�忨��
	BOOL  WriteBoardVer(char *sysVer);//д�忨�����ϵͳ�汾��Ϣ,�Ƽ���������


	//����������Ʒ�ʽ : ͳһ���ƣ�ÿ�����е�ֻ��һ��������Ʋ�����������,��Ҫ����һ������ͬʱ���ƶ�����������ִ��Ч�ʽϸߣ�
	BOOL SetResult(int nChannel,int nIndex,int nResult,int delay=0);//ÿ�ο���һ�������Ʒ����
	BOOL SetResultEx(int num,PCTRLOUT_RESULT pRes);//ÿ�ο���N�������Ʒ����
	//����������Ʒ�ʽ : �������ƣ�ÿ�����еĸ���������Ʋ����������ƣ���Ҫ����һ�����ж������ƶ�����������ִ��Ч�ʽϵͣ�C51����Ŀ��Ʋ��������������
	BOOL SetOutCtrlByResult(int nChannel,int nIndex,int lineoutpout,int nResult);//ÿ��ֻ����һ�������Ʒ������һ�����
	BOOL SetOutCtrlByResultEx(int nChannel,int nIndex,int nResult);//ÿ�ο���һ�������Ʒ������M�����
	BOOL SetOutCtrlByResultEx2(int num,PCTRLOUT_RESULT_INDEP pCtrl);//ÿ�ο���N�������Ʒ������N*M�����
	
	//����������\����\���ֵ
	int ReadInputIndex(int nChannel);//����������ֵ8λ
	int ReadInputCount(int inputn);//���������ֵ32λ

	int ReadOutputIndex(int nChannel,int nPos);//������������˳����������ֵ8λ
	int ReadOutputCount(int nChannel,int nPos);//������������˳����������ֵ32λ
	int ReadOutputCtrlCnt(int nChannel,int nPos);//���ݶ���λ�ö����������ֵ32λ
	int ReadOutputBTCnt(int outputn,int nPos);//������������˳���������߼�����
	int ReadOutputCount(int outputn);//������˿ں�ֱ�Ӷ��������ֵ32λ
	int ReadOutputCtrlCnt(int outputn);//������˿ں�ֱ�Ӷ��������ֵ32λ
	int ReadOutputBTCnt(int outputn);//������˿ں�ֱ�Ӷ�������߼�����
	//���ӳٶ�������\����\���ֵ
	int ReadInputdelayIndex(int nChannel);//�������ӳ���������ֵ8λ
	int ReadOutputdelayIndex(int nChannel,int nPos);//������������˳����ӳ��������ֵ8λ

	int ReadInPortdelayCount(int ninPort,int nDelay);//������˿ں�ֱ�Ӷ��ӳ��������ֵ8λ,ͬʱ�����ӳ�
	int ReadOutputdelayCount(int noutPort,int nDelay);//������˿ں�ֱ�Ӷ��ӳ��������ֵ8λ,ͬʱ�����ӳ�

	BOOL ReadParameterFile(s_InitParam* pParam);
	BOOL ReadParameterFile(const char* strPath);//����������ó�ʼ��״̬,����������
//	BOOL WriteParameterFile(const char* strPath);
	
	BOOL IOWrite(int addr,BYTE mval);
	BOOL IORead(int addr,BYTE *pval);

	//��չ����
	//�õ������ƽ״̬
	BOOL ReadInPortStatus(BYTE &inPortReg);
	//�յ��ж��¼�,��ȡ���뼰���ӳٱ仯״̬�ͷ���ʱ��
	BOOL ReadInChanged(UCHAR &instatus,UCHAR &indelaystatus,DWORD &time2uscnt);

	//��������,needReset=TRUE��Ҫ��λ���ڲ�ִ��ResetALL()
	BOOL StartWork(BOOL needReset=TRUE);
	//ֹͣ����,���������������Ͷ��в���
	BOOL StopWork(BOOL cutALL=TRUE);
	//���¸�λ��ʼ��,��λC51���Ʋ�����FPGA����״̬�����м�����
	BOOL ResetALL();
	//���¸�λ���м�����
	BOOL ResetCount();
	//��ͣ������Ƽ���	
	BOOL PauseInCntWork(int nPortIn);
	//���������������
	BOOL ContinueInCntWork(int nPortIn);
	//���뵱ǰʱ�����,�ɺ���
	BOOL GetAllTmParam(PCTRL_TIMEREG nowTmParam);
	//���뵱ǰʱ�����,���ݾɺ���,������ʹ��
	BOOL GetAllTmParam(PCTRL_TIMEREG_LX45 nowTmParam);
	//����ʱ��/����������
	BOOL GetTm2us(DWORD &nowTm2us);
	BOOL GetTmCoder(int coder_n,DWORD &nowhAll,DWORD &nowf,DWORD &nowh);
	BOOL GetTmCoder(int coder_n,DWORD &nowhAll);
	BOOL GetTmCoder(int coder_n,DWORD &nowf,DWORD &nowh);

	//��������/����������
	BOOL GetRate(int port,float &rate);

	// �������״̬ nOutport ʵ��out�ڣ�0-23��
	BOOL SetOutIOStatus(int nOutport, BOOL bStatus);

	//��ʾ�Ի���
	void ShowDialog();
};

#endif

IOCONTROL_API int fnIoControl(void);