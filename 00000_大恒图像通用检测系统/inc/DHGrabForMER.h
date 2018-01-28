/************************************************************************/
/* Copyright (c) 2013, �й���㼯�ű���ͼ���Ӿ������ֹ�˾�Ӿ�ϵͳ��  
/* All rights reserved.													
/*	




/* �ļ����ƣ� DHGrabberForMER.h	
/* ժҪ�� �����ɼ����ࣨ��˾ˮ��Gige�ӿڵ������
/* ��ǰ�汾��1.64/32.0.4
/* ʹ��SDK�İ汾Ϊ1.1.1501.8211
/* �޸��ߣ�����
/* ��Ҫ�޸����ݣ�����µ�sdk��ȥ���ع�ģʽ����������������64bit��dll����������ͷ�ļ�,���Ӳɼ������ͷ���ֵ
/* ������ڣ�2015��5��28��



/* �ļ����ƣ� DHGrabberForMER.h	
/* ժҪ�� �����ɼ����ࣨ��˾ˮ��usb�����
/* ��ǰ�汾��1.0.0.2
/* �޸��ߣ�����
/* ��Ҫ�޸����ݣ��޸�������ԶԻ������Ӳ��ֺ���
/* ������ڣ�2013��12��2��

/* ��ǰ�汾��1.0.0.1
/* �޸��ߣ���ͬ
/* ��Ҫ�޸����ݣ�ʵ�ֻ����ĳ�ʼ���������ɼ���ֹͣ�ɼ�����֡�ɼ��Ͳ������ý���
/* ������ڣ�2013��10��12��

/************************************************************************/

#pragma once




#pragma once
#include "Grabber.h"

// ˮ�����ר�в���


#ifdef _EXPORTING
#define CLASS_DECLSPEC    __declspec(dllexport)  //�������ʱʹ��
#else
#define CLASS_DECLSPEC    __declspec(dllimport)	 //��̬��Ӧ��ʱʹ��
#endif

enum MERParamID
{
	MERSnapMode,				// ͼ��ɼ�ģʽ
	MERGain,						// ����
	MERExposure,				// �ع�ʱ��
	MERBlacklevel,			    // �ڵ�ƽ
	MERBalanceRatio,             // ��ƽ��
	MERWidth,						//���	//[2017-3-24 zhy]
	MERHeight,						//�߶�	//[2017-3-24 zhy]
	MERXOffset,						//X����ƫ����	//[2017-3-24 zhy]
	MERYOffset						//Y����ƫ����	//[2017-3-24 zhy]
};
//�ɼ���
class CLASS_DECLSPEC CDHGrabForMER : public CGrabber
{

	//����
public:
	CDHGrabForMER();
	virtual ~CDHGrabForMER();
	
	//��ʼ��
	virtual BOOL Init(const s_GBINITSTRUCT* pInitParam) override;
	virtual bool Init(const CGrabInitInfo& pGrabInit) override;
	
	//�ر�
	virtual BOOL Close();
	
	//��ʼ�ɼ�
	virtual BOOL StartGrab();
	
	//ֹͣ�ɼ�
	virtual BOOL StopGrab();
	
	// ��֡�ɼ� 
	virtual BOOL Snapshot();
	
	//���ò���
	virtual BOOL SetParamInt(GBParamID Param, int nInputVal);
	
	//�õ�����
	virtual BOOL GetParamInt(GBParamID Param, int &nOutputVal);
	
	//���ò����Ի���
	virtual void CallParamDialog();
	
	//�õ�������Ϣ
	virtual void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo);
	
	//////////////////////////////////////////////////////////////////////////
	// ��˾���ר�нӿ�	
	BOOL MERSetParamInt(MERParamID Param, int nInputVal1,int nInputVal2 = 0,int nInputVal3 = 0,int nInputVal4 = 0,int nInputVal5 = 0,void *sInputVal6 = NULL);
	BOOL MERGetParamInt(MERParamID Param, int &nOutputVal1,int &nOutputVal2,int &nOutputVal3);	
	int  MERSetOutputParamInt(int nOutputPort,int nOutputVal);//��������Զ���������� 
	//����ֵ 0:�ɹ� 1:��֧���Զ����ź� 2:�����Զ����ź�ʧ�� 3:�򿪶˿�ʧ�� 4:д���ֵʧ�� 

	
	void SaveParamToINI();		// �������������Ϣ
	
	//
	//////////////////////////////////////////////////////////////////////////
	
	//����
protected:
	CGrabber *m_pGrabber;
};
