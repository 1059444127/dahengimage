#pragma once
#include "DHIOCard.h"

#define EXDLL

#ifdef EXDLL
#define DLLDECL _declspec(dllexport)
#else
#define DLLDECL _declspec(dllimport)
#endif

class DLLDECL CDHIOCardForSG :public CDHIOCard
{
public:
	CDHIOCardForSG();
	virtual ~CDHIOCardForSG();
	virtual bool InitCard(CIOCardInitInfo& rIOCardInitInfo)override;
	virtual bool OpenCard()override;
	virtual bool CloseCard()override; 
	virtual bool ResetIOCard()override;
	virtual bool OpenTools(CWnd* pParent = NULL);//**[9/4/2017 ConanSteve]**:  �򿪽ӿڿ����Թ���
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
protected:
	CDHIOCard* m_pIOCard;
};

