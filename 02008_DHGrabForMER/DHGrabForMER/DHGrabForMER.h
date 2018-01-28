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



/*
�ɼ������ͱ�Ŷ�������
0:ģ��ɼ���
1:DALSA X64�ɼ���
2:AVT���
3:ǧ�������
4:��˾CG300ģ��ɼ���
5:��˾1394���
6:EURESYS �ɼ���
7:��˾USB���
8::��˾ˮ��ϵ�����*/
#pragma once

#include "Grabber.h"

#define EXDLL

#ifdef EXDLL
#define CLASS_DECLSPEC    __declspec(dllexport)  //�������ʱʹ��
#else
#define CLASS_DECLSPEC    __declspec(dllimport)	 //��̬��Ӧ��ʱʹ��
#endif
// ˮ�����ר�в���
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
	BOOL Init(const s_GBINITSTRUCT* pInitParam);
	virtual bool Init(const CGrabInitInfo& rGrabInitInfo) override;
	
	//�ر�
	BOOL Close();
	
	//��ʼ�ɼ�
	BOOL StartGrab();
	
	//ֹͣ�ɼ�
	BOOL StopGrab();
	
	// ��֡�ɼ� 
	BOOL Snapshot();
	
	//���ò���
	BOOL SetParamInt(GBParamID Param, int nInputVal);

	bool GetParamBool(GBParamID Param, bool bRetVal);

	bool SetParamBool(GBParamID Param, bool bInputVal);
	
	//�õ�����
	BOOL GetParamInt(GBParamID Param, int &nOutputVal);
	
	//���ò����Ի���
	void CallParamDialog();
	
	//�õ�������Ϣ
	void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo);
	
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
