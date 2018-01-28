/************************************************************************/
/* Copyright (c) 2007, �й���㼯�ű���ͼ���Ӿ������ֹ�˾�Ӿ�ϵͳ��  
/* All rights reserved.													
/*																		
/* �ļ����ƣ� GrabberExportForSG.h	
/* ժҪ�� �����ɼ�����
/*
/* ��ǰ�汾�� 3.0
/* �޸��ߣ� ��׿��
/* �޸�����ժҪ����ʼ���ṹ�弰�ɼ��ص��ṹ����롰�ɼ������ͱ�š���// [2009-12-18 by jzj]: add
/* ������ڣ� 2009��12��18��
/*
/* ��ǰ�汾�� 2.0
/* �޸��ߣ� ��׿��
/* �޸�����ժҪ�������Ͳɼ�����ʹ�ò�ͬ����������CDHGrabberSG��������Ӧһ�������ö��ֲɼ�����÷���
				 ʹ��ʾ����CGrabber* pGrabber = new CDHGrabberSG; pGrabber->StartGrab(); ��
/* ������ڣ� 2009��12��14��
/* 
/* ��ǰ�汾�� 1.1
/* �޸��ߣ� ��׿��
/* �޸�����ժҪ�����뵥֡�ɼ��ӿ�Snapshot()��
/* ������ڣ� 2007��11��05��
/*
/* ��ǰ�汾�� 1.0
/* ���ߣ� ��׿��
/* ������ڣ� 2006��07��27��
/************************************************************************/


#pragma once
#include "resource.h"

#include "Grabber.h"

//////////////////////////////////////////////////////////////////////////
//ģ��ɼ���ר�в���

// [2009-12-17 by jzj]: add 
//ģ��ɼ�������
enum SGParamID	// �޸�ʱע����SimulateGrabber.h�еġ�PARAM ID��������һ��
{
	SGIsPlaneRGB = 0,		//�Ƿ��ͨ�� (0����1����)
	SGGrabSpeed,			//��ͼ�ٶ� (��λ:����/��)
	SGImageWidth,			//ͼ���� (��λ:����)//[2006-12-01]
	SGImageHeight,			//ͼ��߶� (��λ:����)//[2006-12-01]
	SGImageByteCount,		//ͼ��ÿ�����ֽ��� (��λ:�ֽ�)//[2006-12-01]
};
//
//////////////////////////////////////////////////////////////////////////

//�ɼ���
class _declspec(dllexport) CDHGrabForSG : public CGrabber
{
//����
public:
	CDHGrabForSG();
	virtual ~CDHGrabForSG();
	
	//��ʼ��
	BOOL Init(const s_GBINITSTRUCT* pInitParam);
	virtual bool Init(const CGrabInitInfo& rGrabInitInfo);

	//�ر�
	BOOL Close();

	//��ʼ�ɼ�
	BOOL StartGrab();

	//ֹͣ�ɼ�
	BOOL StopGrab();

	//��֡�ɼ�
	BOOL Snapshot();
	
	//���ò���
	BOOL SetParamInt(GBParamID Param, int nInputVal);
	
	//�õ�����
	BOOL GetParamInt(GBParamID Param, int &nOutputVal);

	//���ò����Ի���
	void CallParamDialog();

	//�õ�������Ϣ
	void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo);

	//////////////////////////////////////////////////////////////////////////
	//ģ��ɼ���ר�нӿ�

	// [2009-12-17 by jzj]: add
	BOOL SGSetParamInt(SGParamID Param, int nInputVal);
	BOOL SGGetParamInt(SGParamID Param, int &nOutputVal);
	//
	//////////////////////////////////////////////////////////////////////////
	
//����
protected:
	CGrabber *m_pGrabber;
};

