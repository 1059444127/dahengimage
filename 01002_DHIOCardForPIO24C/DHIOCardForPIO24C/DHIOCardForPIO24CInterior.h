#pragma once
#include "DHIOCard.h"
#include "PIO24BLib.h"
#include "ExFn.h"
#include "array"
#include "map"
using namespace std;

#pragma comment(lib, "..\\lib\\PIO24B_DLL.lib")

#define MAX_INPORT_NUM 4
#define MAX_OUTPORT_NUM 8
class CDHIOCardForPIO24CInterior :
	public CDHIOCard
{
public:
	CDHIOCardForPIO24CInterior();
	virtual ~CDHIOCardForPIO24CInterior();
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
	//**[9/4/2017 ConanSteve]**:  ��ô�����Ϣ
	virtual CString GetLastError() override;

	/*************************************<������ȡ������>************************************[12/13/2017 ConanSteve]*/
	//**[12/13/2017 ConanSteve]**:  ��ȡ������
	virtual bool GetInputIndex(int iInputport, int &iValue) override;
	//**[12/13/2017 ConanSteve]**:  ��ȡָ������Ŷ�Ӧ��ͼ���
	virtual bool GetGrabImgSN(int iGrabSN, int& iImageSN) override;
	//**[12/13/2017 ConanSteve]**:  ��ȡָ���޷϶��ж�Ӧ������ڼ���
	virtual bool GetKickCountInfo(int iKickSN, s_KickCountInfo& sKickCntInfo) override;
	//**[12/13/2017 ConanSteve]**:  ���ýӿڿ�����ڵ�״ֵ̬������Ҫ��ԭ���ǲ�ͬ��Ŀ�Ĳ����Ҫ���ýӿڿ������״̬
	virtual bool SetOutIOStatus(int iOutport, bool bStatus) override;
	//**[12/13/2017 ConanSteve]**:  ��ȡָ��out�ڵļ���
	virtual bool GetOutputCount(int iOutport, int& iValue) override;


private:
	CPIO24BLib m_IOCard;
	CIOCardInitInfo m_InitInfo;
	CINICfg m_iniCfgOfConnect;
	CString m_sLastError;//**[12/19/2017 ConanSteve]**:  ������Ϣ
	int m_iIOCardSN{0};//**[12/19/2017 ConanSteve]**:  �ӿڿ����
	bool m_bWriteLog{ false };//**[1/7/2018 ConanSteve]**:	�Ƿ����־ 
	bool m_bInitSuccess{ false };
	bool m_bEnableWorking{ false };//**[12/19/2017 ConanSteve]**:  �ӿڿ��Ƿ����ڹ���
	bool m_bEnableClose{ false };//**[12/19/2017 ConanSteve]**:  �Ƿ�ر��˽ӿڿ�
	int m_iGrabNum{0};//**[12/19/2017 ConanSteve]**:  �ӿڿ����ӵ��������
	int m_iKickQueueNum{0};//**[12/19/2017 ConanSteve]**:  �ӿڿ����и���
	int m_iMaxInport{ 0 };//**[12/19/2017 ConanSteve]**:  �ӿڿ�ʹ�õ����InportID
	int m_iMaxOutport{ 0 };//**[12/19/2017 ConanSteve]**:  �ӿڿ�ʹ�õ����OurportID;
	array<int, MAX_INPORT_NUM> m_arr_iInportCnt;//**[12/19/2017 ConanSteve]**:  ÿ��Inport�ļ���
	array<int, MAX_OUTPORT_NUM> m_arr_iOutportCnt;//**[12/19/2017 ConanSteve]**:  ÿ��Outport�ļ���
	array<int, 32> m_arr_iQueueRegisterCnt;//**[12/19/2017 ConanSteve]**:  ���мĴ����ĸ���
	map<int, int> m_map_GrabSN2QueueRegisterID;//**[12/19/2017 ConanSteve]**:  ��¼����Ŷ�Ӧ�Ķ��кżĴ�����ַ��ֻ������28,29,30,31
	map<int, int> m_map_SysInport2CardInport;//**[12/19/2017 ConanSteve]**: ϵͳIn�ڵ��ӿڿ�In�ڵ�ӳ�䣬һ�㿨1������ͬ����2����ͬ
	map<int, int> m_map_SysOutport2CardOutport;//**[12/19/2017 ConanSteve]**:  ϵͳOut�ڵ��ӿڿ�Out�ڵ�ӳ��
	map<int, s_KickCountInfo> m_map_SysKickSN2KickCntInfo;//**[12/19/2017 ConanSteve]**:  ϵͳ�޷϶��кŵ��޷���Ϣ��ӳ��
	map<int, int> m_map_CardKickOutport2Channel;//**[12/19/2017 ConanSteve]**:  C���޷϶�Ӧ��out�ڵ�ͨ��֮��Ĺ�ϵ

	array<CLogFile, 4> m_arr_fileKick;
private:
	bool ClearData();//**[12/30/2017 ConanSteve]**:����ӿڿ����� 
};

