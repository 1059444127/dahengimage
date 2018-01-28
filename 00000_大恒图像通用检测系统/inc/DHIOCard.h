#pragma once

#ifndef MAX_BUF_LEN
#define MAX_BUF_LEN  1024
#endif // MAX_BUF_LEN


enum e_IOCardType {
	e_IOCardType_NONE,//**[12/8/2017 ConanSteve]**:  �޿�
	e_IOCardType_SG,//**[12/12/2017 ConanSteve]**:  ģ��ӿڿ�
	e_IOCardType_PIOB,
	e_IOCardType_PIOC,//**[9/11/2017 ConanSteve]**:  C��
	e_IOCardType_PCI_6356,//**[12/8/2017 ConanSteve]**:  PCI6356��
	e_IOCardType_PCIE_6356,
	e_IOCardType_RS232_XZXNYN
};

/*************************************<д�����>************************************[9/13/2017 ConanSteve]*/
typedef struct s_sRsltInfoWritten
{
	int iKickSN;
	int iImgSN;
	int iRsltNum;
	bool arr_bKick[30];
	s_sRsltInfoWritten()
	{
		memset(arr_bKick, 0, sizeof(arr_bKick));
		iKickSN = 0;
		iImgSN = 0;
		iRsltNum = 0;
	}
	s_sRsltInfoWritten(bool* arr_bKick, int iKickSN, int iImgSN, int iRsltNum)
	{
		memcpy_s(this->arr_bKick, sizeof(this->arr_bKick), arr_bKick, sizeof(arr_bKick));
		this->iKickSN = iKickSN;
		this->iImgSN = iImgSN;
		this->iRsltNum = iRsltNum;
	}
}s_sRsltInfoWritten;





/*************************************<����ص�֪ͨ�ӿڿ���Ϣ>************************************[9/13/2017 ConanSteve]*/
typedef struct s_NotifyInfoOfGrabCallback
{
	int iGrabSN{0};
	int iCallbackCnt{0};
	s_NotifyInfoOfGrabCallback() = default;
	s_NotifyInfoOfGrabCallback(int iGrabSN, int iCallbackCnt)
	{
		this->iGrabSN = iGrabSN;
		this->iCallbackCnt = iCallbackCnt;
	}
}s_NotifyInfoOfGrabCallback;
/*************************************<�ӿڿ���ʼ����Ϣ>************************************[9/13/2017 ConanSteve]*/
class CIOCardInitInfo {
public:
	//**[12/13/2017 ConanSteve]**:  �ӿڿ������ļ�������������޹�
	void* pContext;//**[12/15/2017 ConanSteve]**:  ��ʼ���ӿڿ��������ģ�һ���Ǵ���ָ��CWnd*
	void* pIOCallback;//**[12/15/2017 ConanSteve]**:  �ӿڿ��ص�����ָ��
	TCHAR lpszPathFileOfInit[MAX_PATH];//**[12/19/2017 ConanSteve]**:  ��ʼ���ļ�
	//**[12/13/2017 ConanSteve]**:  ���������ӿڿ�����ں����֮���ϵ�������ļ���ʹ�ӿڿ��ܹ���ȷ����ϵͳ��ָ�������Ӧ��ͼ��ŵ���������
	TCHAR lpszPathFileOfConnection[MAX_PATH];


	CIOCardInitInfo()
	{
		memset(lpszPathFileOfInit, 0, sizeof(lpszPathFileOfInit));
		memset(lpszPathFileOfConnection, 0, sizeof(lpszPathFileOfConnection));
		pContext = NULL;
		pIOCallback = NULL;
	}
};

/*************************************<�������ü���ȡ���>************************************[9/13/2017 ConanSteve]*/
enum class IOCardParaType {
	NONE,
	RefreshPeroid,//**[9/12/2017 ConanSteve]**:  �̵���ˢ������:RS232
	FrameDelayOfKick,//**[9/12/2017 ConanSteve]**:  �ӳ�д��֡��:RS232
};
//**[9/13/2017 ConanSteve]**:  �ӿڿ���Ϣ��������Ҫ���ڰ�DHPropertyCridCtrl�ؼ���
class CIOCardInfo {
public:
	int m_iRefreshPeroid{ 0 };
	int m_iFrameDelayOfKick{ 0 };
};
//**[9/13/2017 ConanSteve]**:  ����ͨ�Žṹ��,�������úͶ�ȡ�ӿڲ�����֧��������չ
typedef struct s_IOCardParaInfo {
	IOCardParaType type;
	int* piVal{ nullptr };
	float* pfVal{ nullptr };
	bool* pbVal{ nullptr };
	s_IOCardParaInfo()
	{
		type = IOCardParaType::NONE;
	}
}s_IOCardParaInfo;
/*************************************<�޷ϼ�����Ϣ�ṹ��>************************************[12/13/2017 ConanSteve]*/
typedef struct s_KickCountInfo
{
	int iKickSN;//**[12/13/2017 ConanSteve]**:  �ӿڿ��Լ����޷϶��кţ�ע����ϵͳ�޷϶��к����ֿ�����һ�㵥�ſ���ϵͳ����ֵ��ͬ
	int iKickPortNum{ 1 };//**[12/13/2017 ConanSteve]**:  //**[12/13/2017 ConanSteve]**:  �˶��е��޷Ͽڸ������������ߺͿڷ�Һ��ɫ���ܲ�Ϊ1
	int arr_iKickPortId[30];//**[12/13/2017 ConanSteve]**: �޷϶��е�ÿ���߼��޷Ͽڶ�Ӧ��ʵ�ʽӿڿ�����˿�ֵ
	int arr_iKickPortCount[30];//**[12/13/2017 ConanSteve]**:  ÿ���޷Ͽڵļ���
	int arr_iAutoKickCnt[30];//**[12/17/2017 ConanSteve]**: ���߼��� 
	bool arr_bIsBadKick[30];//**[12/28/2017 ConanSteve]**:  ���ÿ���߷Ͽ��ߵ��ǲ��Ƿ�Ʒ
	s_KickCountInfo() {
		iKickSN = 0;
		memset(arr_iKickPortCount, 0, sizeof(arr_iKickPortCount));
		memset(arr_iKickPortId, 0, sizeof(arr_iKickPortId));
		memset(arr_iAutoKickCnt, 0, sizeof(arr_iAutoKickCnt));
		memset(arr_bIsBadKick, 1, sizeof(arr_bIsBadKick));
	}
}s_KickCountInfo;
// **[12/13/2017 ConanSteve]**:  �����߷϶��е�out��֮�����Ϣ�������޷ϲ�ѯʹ��
// typedef struct s_KickQueryDict {
// 	int iKickSN;//**[12/13/2017 ConanSteve]**:  �޷϶��к�
// 	int iKickPortNum;//**[12/13/2017 ConanSteve]**:  ��ǰ�޷϶��е��޷Ͽ���Ŀ
// }s_KickQueryDict;
/*************************************<>************************************[12/13/2017 ConanSteve]*/
class CDHIOCard
{
public:
	CDHIOCard(){}
	virtual ~CDHIOCard() {}
	virtual bool InitCard(CIOCardInitInfo& rIOCardInitInfo) = 0;//**[9/4/2017 ConanSteve]**:  ��ʼ���ӿڿ�
	virtual e_IOCardType GetIOCardType() =0;
	//**[9/4/2017 ConanSteve]**:  �򿪽ӿڿ�
	virtual bool OpenCard() { return true; };
	//**[9/4/2017 ConanSteve]**:  �رսӿڿ�
	virtual bool CloseCard() = 0;
	//**[12/13/2017 ConanSteve]**:  ���ýӿڿ���������
	virtual bool ResetIOCard(){return true;}
	//**[9/4/2017 ConanSteve]**:  �򿪽ӿڿ����Թ���, pParentΪ�մ���ģ̬�Ի��򣬷���Ϊ��ģ̬�Ի���
	virtual bool OpenTools(CWnd* pParent = NULL) { return true; }
	//**[11/28/2017 ConanSteve]**:  ʹ�ܽӿڿ������������������к͵����˿ڵ�ʹ��
	virtual bool Enable(bool bEnable) = 0;
	//**[12/12/2017 ConanSteve]**:  ʹ�����ж��У����ǲ�Ӱ�쵥���˿ڵ�״̬���ã������ǵ����ֽӿڿ�ֹͣ���ʱ����ϣ�����д�����������Ҫʹ�����Ƴ�����
	virtual bool EnableQueue(bool bEnable) {return true;}
	//**[9/12/2017 ConanSteve]**:  ��ȡ�ӿڿ�����
	virtual bool GetParam(s_IOCardParaInfo& para) {return false;};
	//**[9/12/2017 ConanSteve]**:  ���ýӿڿ�����
	virtual bool SetParam(s_IOCardParaInfo& para) {return false;};
	//**[9/11/2017 ConanSteve]**:  д�����
	virtual bool WriteKickResult(s_sRsltInfoWritten & rslt) {return true;};
	//**[9/11/2017 ConanSteve]**:  ����ص�֪ͨ,��Ҫ����һЩ�Լ�û��ͼ��żĴ����Ľӿڿ�������ͼ���
	virtual bool NotifyCardOfGrabCallBack(s_NotifyInfoOfGrabCallback &notifyInfo) {return true;};
	//**[9/4/2017 ConanSteve]**:  ��ô�����Ϣ
	virtual CString GetLastError() {return _T("");};

	/*************************************<������ȡ������>************************************[12/13/2017 ConanSteve]*/
	//**[12/13/2017 ConanSteve]**:  ��ȡ������
	virtual bool GetInputIndex(int iInputport, int &iValue) { return false; }
	//**[12/13/2017 ConanSteve]**:  ��ȡָ������Ŷ�Ӧ��ͼ���
	virtual bool GetGrabImgSN(int iGrabSN, int& iImageSN) { return false; }
	//**[12/13/2017 ConanSteve]**:  ��ȡָ���޷϶��ж�Ӧ������ڼ���
	virtual bool GetKickCountInfo(int iKickSN, s_KickCountInfo& sKickCntInfo) { return false; }
	//**[12/13/2017 ConanSteve]**:  ���ýӿڿ�����ڵ�״ֵ̬������Ҫ��ԭ���ǲ�ͬ��Ŀ�Ĳ����Ҫ���ýӿڿ������״̬
	virtual bool SetOutIOStatus(int iOutport, bool bStatus) { return true; }
	//**[12/13/2017 ConanSteve]**:  ��ȡָ��out�ڵļ���
	virtual bool GetOutputCount(int iOutport, int& iValue) { return 0; };
};
