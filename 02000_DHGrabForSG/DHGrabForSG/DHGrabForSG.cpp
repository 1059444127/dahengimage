/************************************************************************/
/* Copyright (c) 2007, �й���㼯�ű���ͼ���Ӿ������ֹ�˾�Ӿ�ϵͳ��  
/* All rights reserved.													
/*																		
/* �ļ����ƣ� GrabberExportForSG.cpp
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

#include "stdafx.h"
#include "DHGrabForSG.h"

#include "SimulatedCamera.h"

CDHGrabForSG::CDHGrabForSG()
{
	m_pGrabber = new CSimulatedCamera;
}

CDHGrabForSG::~CDHGrabForSG()
{
	if(m_pGrabber != NULL)
	{
		delete m_pGrabber;
		m_pGrabber = NULL;
	}
}
	
//��ʼ��
BOOL CDHGrabForSG::Init(const s_GBINITSTRUCT* pInitParam)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Init(pInitParam));
}

bool CDHGrabForSG::Init(const CGrabInitInfo& rGrabInitInfo)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Init(rGrabInitInfo));
}

//�ر�
BOOL CDHGrabForSG::Close()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Close());
}

//��ʼ�ɼ�
BOOL CDHGrabForSG::StartGrab()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->StartGrab());
}

//ֹͣ�ɼ�
BOOL CDHGrabForSG::StopGrab()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->StopGrab());
}

//��֡�ɼ�
BOOL CDHGrabForSG::Snapshot()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Snapshot());
}
	
//�õ�����
BOOL CDHGrabForSG::GetParamInt(GBParamID Param, int &nOutputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->GetParamInt(Param, nOutputVal));
}
	
//���ò���
BOOL CDHGrabForSG::SetParamInt(GBParamID Param, int nInputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->SetParamInt(Param, nInputVal));
}

//���ò����Ի���
void CDHGrabForSG::CallParamDialog()
{
	ASSERT(m_pGrabber != NULL);
	m_pGrabber->CallParamDialog();
}

//�õ�������Ϣ
void CDHGrabForSG::GetLastErrorInfo(s_GBERRORINFO *pErrorInfo)
{
	ASSERT(m_pGrabber != NULL);
	m_pGrabber->GetLastErrorInfo(pErrorInfo);
}

//////////////////////////////////////////////////////////////////////////
//ģ��ɼ���ר�нӿ�
	
// [2009-12-17 by jzj]: add
//���ò���
BOOL CDHGrabForSG::SGSetParamInt(SGParamID Param, int nInputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(((CSimulatedCamera *)m_pGrabber)->SetParamInt(Param, nInputVal));
}	

// [2009-12-17 by jzj]: add
//�õ�����
BOOL CDHGrabForSG::SGGetParamInt(SGParamID Param, int &nOutputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(((CSimulatedCamera *)m_pGrabber)->GetParamInt(Param, nOutputVal));
}
//
//////////////////////////////////////////////////////////////////////////
