#pragma once
#include "DHIOCard.h"
#include "ExFn.h"
#include "array"
#include <set>
#include "DlgIOCardForRS232_XZXNYN.h"
using namespace std;


typedef int (WINAPI * POpenPort)(int);
typedef int (WINAPI * PClosePort)();
typedef int (WINAPI * PSetCoil)(int, int, bool);
typedef int (WINAPI * PGetCoil)(int, int, int);
typedef int (WINAPI * PSetMultCoil)(int, int, int, int);
typedef bool (WINAPI * PBitValueOfInt)(int, int);
typedef int (WINAPI * PBitValueToInt)(int, int, bool);

const int c_CoilNum{ 8 };
class CDHIOCardForRS232_XZXNYNInterior : public CDHIOCard
{
public:
	CDHIOCardForRS232_XZXNYNInterior() {}
	virtual ~CDHIOCardForRS232_XZXNYNInterior();
	virtual bool InitCard(CIOCardInitInfo& rIOCardInitInfo)override;
	virtual bool OpenCard()override;
	virtual bool CloseCard()override;
	//**[9/4/2017 ConanSteve]**:  �򿪽ӿڿ����Թ���
	virtual bool OpenTools(CWnd* pParent = NULL) override;
	virtual bool Enable(bool bEnable)override;
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
	virtual bool GetGrabImgSN(int iGrabSN, int& iImageSN) override;
	//**[12/13/2017 ConanSteve]**:  ��ȡָ���߷϶��ж�Ӧ������ڼ���
	virtual bool GetKickCountInfo(int iKickSN, s_KickCountInfo& sKickCntInfo) override;
	//**[12/13/2017 ConanSteve]**:  ���ýӿڿ�����ڵ�״ֵ̬������Ҫ��ԭ���ǲ�ͬ��Ŀ�Ĳ����Ҫ���ýӿڿ������״̬
	virtual bool SetOutIOStatus(int iOutport, bool bStatus) override;
	//**[12/13/2017 ConanSteve]**:  ��ȡָ��out�ڵļ���
	virtual bool GetOutputCount(int iOutport, int& iValue) override;

private:
	int BuildState(int& Value, int Bit, bool state);//���״̬,���λʱBitΪ0

protected:
	//**[9/12/2017 ConanSteve]**: �̵������ֱպϵ����ʱ������λms
	int m_iTimeOfRefreshCoil{ 0 };

	int m_iStateValue{0};//**[12/17/2017 ConanSteve]**:  �豸״̬
	CCriticalSection m_csWriteDevice;

	CIOCardInitInfo m_InitInfo;//**[12/17/2017 ConanSteve]**:  �ӿڿ���ʼ����Ϣ
	//**[9/12/2017 ConanSteve]**:  ˢ�¼̵����߳̾��
	HANDLE m_hThred{ 0 };
	//**[9/12/2017 ConanSteve]**:  ���ø��̵����պϵ�ʱ����Ϣ
	typedef struct s_HisInfoWritten {
		LONGLONG ldHisTimeOfSetCoil{ 0 };
		bool bNeedBeenReset{ false };
		s_HisInfoWritten() { bNeedBeenReset = false; }
	}s_HisInfoWritten;
	array<s_HisInfoWritten, c_CoilNum> m_sHisTimeInfoOfSetCoil;
	LONGLONG m_iFrequency{ 0 };
	//**[9/12/2017 ConanSteve]**:  �߷ϱ�
	array<array<bool, 256>, c_CoilNum> m_arrKickTable = { false };
	//**[9/13/2017 ConanSteve]**:  ��Ҫд�������Ȧ
	set<int> m_setCoilBeWritten;
	//**[9/12/2017 ConanSteve]**:  ��ʱˢ�¼̵����߳�
	DWORD static WINAPI  ThreadRefreshCoil(LPVOID pParam);
	CLogFile m_fileLogWrite;
	CLogFile m_fileLogReset;


	CString m_sPathFileOfInit{ _T("") };//**[9/11/2017 ConanSteve]**:  ��ʼ���ļ�·��
	HINSTANCE m_hDll{ 0 };
	//**[9/12/2017 ConanSteve]**:  ������Ϣ
	CString m_sLastErrorInfo{ _T("") };
	//**[9/12/2017 ConanSteve]**: 

	CDlgIOCardForRS232_XZXNYN m_dlg;
	CBitmap   m_bmpOn, m_bmpOff;

	array<int, c_CoilNum> m_arr_iTriggerRegister;//**[12/15/2017 ConanSteve]**:  �����Ĵ���������ͼ��ţ����ӿڲ�ѯʹ��
	array<s_KickCountInfo, c_CoilNum> m_arr_sKickCntInfo;//**[12/15/2017 ConanSteve]**:  �洢�߷ϱ���Ϣ,����Ŀֻ���˵�һ���߷϶���
	CINICfg m_iniCfgOfInit;
public:
	array<bool, c_CoilNum> m_arr_bOpen;//**[12/17/2017 ConanSteve]**:  ������Ȧ�Ƿ�պ�

	POpenPort pOpenPort;
	PClosePort pClosePort;
	PSetCoil pSetCoil;
	PGetCoil pGetCoil;
	PSetMultCoil pSetMultCoil;
	PBitValueOfInt pBitValueOfInt;
	PBitValueToInt pBitValueToInt;
	//OnSetCoil(int Addr, int KAddr, bool State);
private:

	DWORD ThreadRefreshCoilFunc();
private:
public:

	int m_iPort{ 0 };//**[9/11/2017 ConanSteve]**:  ���ں�
	int m_iBoardId{ 1 };
	//**[9/12/2017 ConanSteve]**:  �Ƿ�򿪽ӿڿ�
	bool m_bOpenCardSuccess{ false };
	//**[9/12/2017 ConanSteve]**:  д�����ӳ�֡��
	int m_iFrameDelayOfKick{ 0 };
	bool UpdateToolsWindowInfo();


	bool SwitchState(int iBit);

	bool WriteDevice();
};

