/************************************************************************/
/* Copyright (c) 2010, �й���㼯�ű���ͼ���Ӿ������ֹ�˾�Ӿ�ϵͳ��  
/* All rights reserved.													
/*																		
/* �ļ����ƣ� DHGrabberForAVT.h	
/* ժҪ�� ���AVT���FGCamera�汾(2.10)������ǰ�汾  ���и��߰汾����Ҫ����
/*
/* ��ǰ�汾�� 3.0.0.1
/* �޸��ߣ� �ɭ
/* �޸�����ժҪ����ʼ���ṹ�弰�ɼ��ص��ṹ����롰�ɼ������ͱ�š���
/* ������ڣ� 2010��1��4��
/*
/* ��ǰ�汾�� 3.0.0.1
/* ��ǰ�汾��д��������
/* ��ǰ��¼����Ӧ������������FGCamera;��ȡ��ƽ�����ֵ;�������ÿ��޸Ĵ�����ʼ��;
/* ������ȡ����桢�ع����ֵ���޸Ĵ���ֵ����;�޸ĺڰ������Ҫ��ð�ƽ��ֵ
/* �޸��ߣ� �ɭ
/************************************************************************/

#include "stdafx.h"
#include "DHGrabForAVT.h"
#include "AVTCamDH.h"

CDHGrabForAVT::CDHGrabForAVT()
{
	m_pGrabber = new CAVTCamDH;
}

CDHGrabForAVT::~CDHGrabForAVT()
{
	if(m_pGrabber != NULL)
	{
		delete m_pGrabber;
		m_pGrabber = NULL;
	}
}

//��ʼ��
BOOL CDHGrabForAVT::Init(const s_GBINITSTRUCT* pInitParam)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Init(pInitParam));
}

bool CDHGrabForAVT::Init(const CGrabInitInfo& rGrabInitInfo)
{
	return m_pGrabber->Init(rGrabInitInfo);
}

//�ر�
BOOL CDHGrabForAVT::Close()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Close());
}

//��ʼ�ɼ�
BOOL CDHGrabForAVT::StartGrab()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->StartGrab());
}

//ֹͣ�ɼ�
BOOL CDHGrabForAVT::StopGrab()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->StopGrab());
}
	
//�õ�����
BOOL CDHGrabForAVT::GetParamInt(GBParamID Param, int &nOutputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->GetParamInt(Param, nOutputVal));
}
	
//���ò���
BOOL CDHGrabForAVT::SetParamInt(GBParamID Param, int nInputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->SetParamInt(Param, nInputVal));
}


//�õ�����
BOOL CDHGrabForAVT::GetParamfloat(int Param, float &nOutputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->GetParamfloat(Param, nOutputVal));
}

//���ò���
BOOL CDHGrabForAVT::SetParamfloat(int Param, float nInputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->SetParamfloat(Param, nInputVal));
}

//���ò����Ի���
void CDHGrabForAVT::CallParamDialog()
{
	ASSERT(m_pGrabber != NULL);
	m_pGrabber->CallParamDialog();
}

//�õ�������Ϣ
void CDHGrabForAVT::GetLastErrorInfo(s_GBERRORINFO *pErrorInfo)
{
	ASSERT(m_pGrabber != NULL);
	m_pGrabber->GetLastErrorInfo(pErrorInfo);
}	

BOOL CDHGrabForAVT::GetTriggerParam(int &ParamValOutput)
{
	BOOL bRet = FALSE;
	// �������Ӧ����
	bRet = m_pGrabber->GetTriggerParam(ParamValOutput);
	return bRet;
}

BOOL CDHGrabForAVT::SetTriggerParam(int ParamValInput)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->SetTriggerParam(ParamValInput);
	return bRet;
}
BOOL CDHGrabForAVT::GetSystemInfo(CString &sOutputInfo)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->GetSystemInfo(sOutputInfo);
	return bRet;
}

BOOL CDHGrabForAVT::SetAOI(RECT rtAOI)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->SetAOI(rtAOI);
	return bRet;
}

BOOL CDHGrabForAVT::GetAOI(RECT &rtAOI)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->GetAOI(rtAOI);
	return bRet;
}


BOOL CDHGrabForAVT::Reset()
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->Reset();
	return bRet;
}

BOOL CDHGrabForAVT::CreateDefaultConfigurationFile(CString &sInitFile)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->CreateDefaultConfigurationFile(sInitFile);
	return bRet;
}

BOOL CDHGrabForAVT::SaveToConfigurationFile(CString &sInitFile)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->SaveToConfigurationFile(sInitFile);
	return bRet;
}
//////////////////////////////////////////////////////////////////////////
// DirectShowר�нӿ� 
// ��֡�ɼ�
BOOL CDHGrabForAVT::Snapshot()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Snapshot());
}





