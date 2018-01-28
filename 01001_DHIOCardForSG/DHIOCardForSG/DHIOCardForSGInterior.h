#pragma once
#include "DHIOCard.h"
#include "memory"
#include "map"
#include "resource.h"
#include "DlgIOCard.h"
#include "ExFn.h"

#include <mmsystem.h>   
#pragma comment(lib, "WINMM.LIB") //**[12/8/2017 ConanSteve]**:  ��ý���ʱ��



#define ID_EVENT_TIMER WM_USER+100
#define MAX_QUEUE 30//**[12/8/2017 ConanSteve]**:  ���֧��30�����У�ʵ�������ڶ�ý���ʱ���������ƣ������ܴﵽ���ֵ

#define MAX_OUTPORT_NUM 24
#define MAX_INPORT_NUM 12
using namespace std;
class CDHIOCardForSGInterior :
	public CDHIOCard
{
public:
	CDHIOCardForSGInterior();
	~CDHIOCardForSGInterior();
	virtual bool InitCard(CIOCardInitInfo& rIOCardInitInfo)override;
	virtual bool OpenCard()override;
	virtual bool CloseCard()override; 
	virtual bool ResetIOCard()override;
	virtual bool OpenTools(CWnd* pParent/* = NULL */)override;
	virtual bool Enable(bool bEnable)override;
	virtual bool EnableQueue(bool bEnable) override;
	virtual bool GetParam(s_IOCardParaInfo& para) override;//**[9/12/2017 ConanSteve]**:  ��ȡ�ӿڿ�����
	virtual bool SetParam(s_IOCardParaInfo& para) override;//**[9/12/2017 ConanSteve]**:  ���ýӿڿ�����
	virtual bool WriteKickResult(s_sRsltInfoWritten & rslt)override;//**[9/11/2017 ConanSteve]**:  д�����
	virtual bool NotifyCardOfGrabCallBack(s_NotifyInfoOfGrabCallback &notifyInfo)override;//**[9/11/2017 ConanSteve]**:  ����ص�֪ͨ
	virtual e_IOCardType GetIOCardType() override;
	virtual CString GetLastError()override;//**[9/4/2017 ConanSteve]**:  ��ô�����Ϣ


	/*************************************<������ȡ������>************************************[12/13/2017 ConanSteve]*/
	//**[12/13/2017 ConanSteve]**:  ��ȡ������
	virtual bool GetInputIndex(int iInputport, int &iValue) override;
	//**[12/13/2017 ConanSteve]**:  ��ȡָ������Ŷ�Ӧ��ͼ���
	virtual bool GetGrabImgSN(int iGrabSN, int& iImageSN)override;
	//**[12/13/2017 ConanSteve]**:  ��ȡָ���޷϶��ж�Ӧ������ڼ���
	virtual bool GetKickCountInfo(int iKickSN, s_KickCountInfo& sKickCntInfo) override;
	//**[12/13/2017 ConanSteve]**:  ���ýӿڿ�����ڵ�״ֵ̬������Ҫ��ԭ���ǲ�ͬ��Ŀ�Ĳ����Ҫ���ýӿڿ������״̬
	virtual bool SetOutIOStatus(int iOutport, bool bStatus) override;
	//**[12/13/2017 ConanSteve]**:  ��ȡָ��out�ڵļ���
	virtual bool GetOutputCount(int iOutport, int& iValue) override;
private:
	CString m_sLastErrorInfo{ _T("") };
	bool m_bCloseCard{ false };
	bool  m_bEnableQueue{false};//**[12/8/2017 ConanSteve]**:  �ӿڿ��Ƿ��ڹ���
	unique_ptr<CDlgIOCard> m_pDlgTools;//**[12/8/2017 ConanSteve]**:  �ӿڿ����Թ��ߴ���
	CString m_sPathFileOfInit;//**[12/8/2017 ConanSteve]**:  ����ӿڿ������ļ�
	CINICfg m_iniCfg;//**[12/8/2017 ConanSteve]**:  �����ļ�
	array<int, MAX_QUEUE> m_arr_iTimerID;//**[12/8/2017 ConanSteve]**:  ÿ�����ж�Ӧ�Ķ�ý���ʱ��ID
	array<s_KickCountInfo, MAX_OUTPORT_NUM> m_arr_sKickCntInfo;//**[12/13/2017 ConanSteve]**:  �޷ϼ�����Ϣ
	array<vector<int>, MAX_INPORT_NUM> m_arr_veciQueueOutInfo;//**[12/17/2017 ConanSteve]**:  ÿ�����ж�Ӧ��out�ںŵļ���
	map<UINT, int> m_mapTimerID2QueueID;//**[12/9/2017 ConanSteve]**:  ����Ӽ�ʱ��ID�ҵ���Ӧ�Ķ��к�
	//**[12/13/2017 ConanSteve]**:  ����ŵ�out��֮�������ģ�����
	map<int, int> m_mapGrabSN2PhysicalPortId;
	map<int, int> m_mapInportId2QueueId;//**[12/17/2017 ConanSteve]**:  �����ID�����к�ID���ֵ�


public:
	CIOCardInitInfo m_InitInfo;
	int m_iKickQueueNum{ 1 };//**[12/13/2017 ConanSteve]**:  �޷϶��и���
	int m_iQueueNum;//**[12/8/2017 ConanSteve]**:  �������и���
	array<int, MAX_QUEUE> m_arr_iQueueInport;//**[12/14/2017 ConanSteve]**:  ���������
	array<int, MAX_QUEUE> m_arr_iActiveInterval{0};//**[12/8/2017 ConanSteve]**:  ÿ�����д����Ĳ�ͼ����
	array<int, MAX_QUEUE> m_arr_iTriggerCnt{0};//**[12/8/2017 ConanSteve]**:  ÿ�����д����Ĵ���
	array<int, MAX_INPORT_NUM> m_arr_iInportCnt{ 0 };//**[12/14/2017 ConanSteve]**:  ÿ��In�ڵļ������ܹ�12���Ĵ���
	array<int, MAX_OUTPORT_NUM> m_arr_iOutportCnt{ 0 };//**[12/13/2017 ConanSteve]**:  ÿ��out�ڵļ���,�ܹ�24���Ĵ���
	array<bool, MAX_OUTPORT_NUM> m_arr_bOutportStatus;//**[12/16/2017 ConanSteve]**:  Out�ڿ���״̬
	array<HANDLE, MAX_OUTPORT_NUM> m_arr_OutputEventHandle;//**[12/9/2017 ConanSteve]**:  ÿ�����ж�Ӧ���ں��¼����
	array<bool, MAX_INPORT_NUM> m_arr_bEnableInternalActive;//**[12/17/2017 ConanSteve]**:  ������Ƿ��ڴ���

	map<int, int> m_mapSysKickSN2CardKickId;//**[12/18/2017 ConanSteve]**:  ϵͳ�޷϶��кŵ��ӿڿ��޷϶��б�ŵ��ֵ䣬��Ҫ����һ��ϵͳ����ӿڿ�����������C��

public:
	//**[12/13/2017 ConanSteve]**:  �������ж���
	bool AddMultimediaTimer();
	//**[12/13/2017 ConanSteve]**:  ��ʱ�����ڴ����ص�����
	static void CALLBACK CallBackFuncTimer(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2);
	void CallBackFuncTimerFunc(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2);
	//**[12/13/2017 ConanSteve]**:  �ر����ж���
	bool DeleteMultimediaTimer();
	bool TriggerInport(int iInportId);
};

