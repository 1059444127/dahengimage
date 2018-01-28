#pragma once
#include "DHIOCard.h"
#include "ExFn.h"
#include "..\\inc\\IoCard.h"
#pragma comment(lib, "..\\lib\\iocard.lib")
#include "map"
#include "ExFn.h"
using namespace std;


#define  MAX_OUTPORT_NUM 24

class CDHIOCardForPCI6356Interior:public CDHIOCard
{
public:
	CDHIOCardForPCI6356Interior();
	virtual ~CDHIOCardForPCI6356Interior();
	//**[9/4/2017 ConanSteve]**:  ��ʼ���ӿڿ�
	virtual bool InitCard(CIOCardInitInfo& rIOCardInitInfo)override;
	virtual e_IOCardType GetIOCardType() override;
	//**[9/4/2017 ConanSteve]**:  �򿪽ӿڿ�
	virtual bool OpenCard() override;
	//**[9/4/2017 ConanSteve]**:  �رսӿڿ�
	virtual bool CloseCard() override;
	//**[12/13/2017 ConanSteve]**:  ���ýӿڿ���������
	virtual bool ResetIOCard() override;
	//**[9/4/2017 ConanSteve]**:  �򿪽ӿڿ����Թ���
	virtual bool OpenTools(CWnd* pParent = NULL) override;
	//**[11/28/2017 ConanSteve]**:  ʹ�ܽӿڿ������������������к͵����˿ڵ�ʹ��
	virtual bool Enable(bool bEnable) override;
	//**[12/12/2017 ConanSteve]**:  ʹ�����ж��У����ǲ�Ӱ�쵥���˿ڵ�״̬���ã������ǵ����ֽӿڿ�ֹͣ���ʱ����ϣ�����д�����������Ҫʹ�����Ƴ�����
	virtual bool EnableQueue(bool bEnable) override;
	//**[9/12/2017 ConanSteve]**:  ��ȡ�ӿڿ�����
	virtual bool GetParam(s_IOCardParaInfo& para) override;
	//**[9/12/2017 ConanSteve]**:  ���ýӿڿ�����
	virtual bool SetParam(s_IOCardParaInfo& para) override;
	//**[9/11/2017 ConanSteve]**:  д�����
	virtual bool WriteKickResult(s_sRsltInfoWritten & rslt) override;
	//**[9/11/2017 ConanSteve]**:  ����ص�֪ͨ,��Ҫ����һЩ�Լ�û��ͼ��żĴ����Ľӿڿ�������ͼ���
	virtual bool NotifyCardOfGrabCallBack(s_NotifyInfoOfGrabCallback &notifyInfo) override;
	//**[9/4/2017 ConanSteve]**:  ��ô�����Ϣ
	virtual CString GetLastError() override;

	/*************************************<������ȡ������>************************************[12/13/2017 ConanSteve]*/
	//**[12/13/2017 ConanSteve]**:  ��ȡ������
	virtual bool GetInputIndex(int iInputport, int &iValue) override;
	//**[12/13/2017 ConanSteve]**:  ��ȡָ������Ŷ�Ӧ��ͼ���
	virtual bool GetGrabImgSN(int iGrabSN, int& iImageSN) override;
	//**[12/13/2017 ConanSteve]**:  ��ȡָ���߷Ѷ��ж�Ӧ������ڼ���
	virtual bool GetKickCountInfo(int iKickSN, s_KickCountInfo& sKickCntInfo) override;
	//**[12/13/2017 ConanSteve]**:  ���ýӿڿ�����ڵ�״ֵ̬������Ҫ��ԭ���ǲ�ͬ��Ŀ�Ĳ����Ҫ���ýӿڿ������״̬
	virtual bool SetOutIOStatus(int iOutport, bool bStatus) override;
	//**[12/13/2017 ConanSteve]**:  ��ȡָ��out�ڵļ���
	virtual bool GetOutputCount(int iOutport, int& iValue) override;

private:

	//**[12/15/2017 ConanSteve]**:   �ӿڿ��ص�����	
	static void WINAPI GlobalIOCallback(const s_IOParam* SigInfo);
	void GlobalIOCallbackFunc(const s_IOParam* SigInfo);	// 1��λ��һ������Ļص�����

private:
	CString m_sLastErrorInfo{ _T("") };
	CioControl m_IOCard;
	int m_iDevSN{ 0 };//**[11/30/2017 ConanSteve]**:  �ӿڿ��豸��
	bool m_bInitSuccess{ false };//**[12/15/2017 ConanSteve]**:  �ӿڿ��Ƿ��ʼ���ɹ�
	bool m_bNeedReadCardParam{true};//**[12/18/2017 ConanSteve]**:  �Ƿ���Ҫ��ȡ����
	CIOCardInitInfo m_IOCardInitInfo;//**[12/15/2017 ConanSteve]**:  �����ʼ������
	CINICfg m_iniCfgOfInit;
	CINICfg m_iniCfgOfConnection;
	bool m_bEnableLog{ false };
	array<CLogFile, MAX_OUTPORT_NUM> m_arr_fileKick;

	

	/*************************************<Connection>************************************[12/18/2017 ConanSteve]*/
	int m_iGrabNum{1};//**[12/18/2017 ConanSteve]**:  ϵͳ�������
	int m_iKickQueueNum{ 1 };//**[12/18/2017 ConanSteve]**:  �ӿڿ��߷Ѷ��и���
	bool m_bEnableWorking{ false };//**[12/18/2017 ConanSteve]**:  �Ƿ����ڹ���
	array<s_KickCountInfo, MAX_OUTPORT_NUM> m_arr_sKickCntInfo;//**[12/13/2017 ConanSteve]**:  �߷Ѽ�����Ϣ
	map<int, int> m_map_SysKickSN2CardKickId;//**[12/18/2017 ConanSteve]**:  ϵͳ�߷Ѷ��кŵ��ӿڿ��߷Ѷ��б�ŵ��ֵ䣬��Ҫ����һ��ϵͳ����ӿڿ�����������C��

	map<int,int> m_map_OutportOfGrabRelated;//**[12/18/2017 ConanSteve]**:  ÿ������Ŷ�Ӧ�Ĵ���out��
	typedef struct s_OutputCtrlInfo{
		int iChannel;//**[12/18/2017 ConanSteve]**:  ���к�
		int iLogicalOutport;//**[12/18/2017 ConanSteve]**:  �����߼��˿ں�
	}s_OutputCtrlInfo;
	map<int, s_OutputCtrlInfo> m_map_GrabSN2LogicalOutport;//**[12/18/2017 ConanSteve]**:  ����ŵ��߼��˿ںŵ�ӳ��

// 	typedef struct s_KickKeyName
// 	{
// 		int iKickSNOfIOCard;//**[12/18/2017 ConanSteve]**:  �ӿڿ����߷Ѷ��к�,һ����Ƕ��к�
// 		int iKickIndexOfCurQueue;//**[12/18/2017 ConanSteve]**:  ��ǰ�߷Ѷ��е��߷�����
// 		s_KickKeyName() 
// 		{ 
// 			iKickSNOfIOCard = 0; 
// 			iKickIndexOfCurQueue = 0;
// 		}
// 		s_KickKeyName(int iKickSNOfIOCard, int iKickIndexOfCurQueue)
// 		{
// 			this->iKickSNOfIOCard = iKickSNOfIOCard;
// 			this->iKickIndexOfCurQueue = iKickIndexOfCurQueue;
// 		}
// 	}s_KickKeyName;
	typedef struct s_KickKeyValue
	{
		int iChannel;//**[12/18/2017 ConanSteve]**:  ��ӦOut�����ڵ�ͨ����
		int iLogicalOutport;//**[12/18/2017 ConanSteve]**:  ��ӦOut�ڵ��߼��˿ں�
		s_KickKeyValue() 
		{
			iChannel = 0;
			iLogicalOutport = 0;
		}
		s_KickKeyValue(int iChannel, int iLogicalOutport)
		{
			this->iChannel = iChannel;
			this->iLogicalOutport = iLogicalOutport;
		}
	}s_KickKeyValue;
	map<int, s_KickKeyValue> m_map_KickPortDict;//**[12/18/2017 ConanSteve]**:  ���߷Ѷ����ҵ�ʵ�ʽӿڿ�д����ͨ�����߼��˿ں�

};

