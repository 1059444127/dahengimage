
#pragma once
#include "DHIOCard.h"



#ifdef DHIOCARDFORPIO24C

#define CLASS_DECLSPEC    __declspec(dllexport)  //�������ʱʹ��
#else
#define CLASS_DECLSPEC    __declspec(dllimport)	 //��̬��Ӧ��ʱʹ��
#endif


class CLASS_DECLSPEC CDHIOCardForPIO24C : public CDHIOCard
{
public:
	CDHIOCardForPIO24C();
	virtual ~CDHIOCardForPIO24C();
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
	//**[12/13/2017 ConanSteve]**:  ��ȡָ���޷϶��ж�Ӧ������ڼ���
	virtual bool GetKickCountInfo(int iKickSN, s_KickCountInfo& sKickCntInfo) override;
	//**[12/13/2017 ConanSteve]**:  ���ýӿڿ�����ڵ�״ֵ̬������Ҫ��ԭ���ǲ�ͬ��Ŀ�Ĳ����Ҫ���ýӿڿ������״̬
	virtual bool SetOutIOStatus(int iOutport, bool bStatus) override;
	//**[12/13/2017 ConanSteve]**:  ��ȡָ��out�ڵļ���
	virtual bool GetOutputCount(int iOutport, int& iValue) override;
protected:
	CDHIOCard* m_pIOCard;
};

