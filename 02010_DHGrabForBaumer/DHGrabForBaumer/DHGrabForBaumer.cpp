/************************************************************************/
/* Copyright (c) 2009, �й���㼯�ű���ͼ���Ӿ������ֹ�˾�Ӿ�ϵͳ��  
/* All rights reserved.													
/*																		
/* �ļ����ƣ� DHGrabberForBaumer.CPP	
/* ժҪ�� �����ɼ����ࣨBaumerǧ���������
/************************************************************************/

#include "stdafx.h"
#include "DHGrabForBaumer.h"
#include "BaumerCamDH.h"

CDHGrabForBaumer::CDHGrabForBaumer()
{
	m_pGrabber = new CBaumerCamDH;
}

CDHGrabForBaumer::~CDHGrabForBaumer()
{
	if(m_pGrabber != NULL)
	{
		delete m_pGrabber;
		m_pGrabber = NULL;
	}
}
	
//��ʼ��
BOOL CDHGrabForBaumer::Init(const s_GBINITSTRUCT* pInitParam)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Init(pInitParam));
}

bool CDHGrabForBaumer::Init(const CGrabInitInfo & rGrabInitInfo)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Init(rGrabInitInfo));
}

//�ر�
BOOL CDHGrabForBaumer::Close()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Close());
}

//��ʼ�ɼ�
BOOL CDHGrabForBaumer::StartGrab()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->StartGrab());
}

//ֹͣ�ɼ�
BOOL CDHGrabForBaumer::StopGrab()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->StopGrab());
}
	
//�õ�����
BOOL CDHGrabForBaumer::GetParamInt(GBParamID Param, int &nOutputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->GetParamInt((GBParamID)Param, nOutputVal));
}
	
//���ò���
BOOL CDHGrabForBaumer::SetParamInt(GBParamID Param, int nInputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->SetParamInt((GBParamID)Param, nInputVal));
}

//���ò����Ի���
void CDHGrabForBaumer::CallParamDialog()
{
	ASSERT(m_pGrabber != NULL);
	m_pGrabber->CallParamDialog();
}

//�õ�������Ϣ
void CDHGrabForBaumer::GetLastErrorInfo(s_GBERRORINFO *pErrorInfo)
{
	ASSERT(m_pGrabber != NULL);
	m_pGrabber->GetLastErrorInfo(pErrorInfo);
}		

BOOL CDHGrabForBaumer::SetParamfloat(int Param, float nInputVal)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->SetParamfloat(Param,nInputVal);
	return bRet;
}	

BOOL CDHGrabForBaumer::GetParamfloat(int Param, float& nOutputVal)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->GetParamfloat(Param, nOutputVal);
	return bRet;
}	

BOOL CDHGrabForBaumer::SetTriggerParam(int ParamValInput)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->SetTriggerParam(ParamValInput);
	return bRet;
}

BOOL CDHGrabForBaumer::SetAOI(RECT rtAOI)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->SetAOI(rtAOI);
	return bRet;
}

BOOL CDHGrabForBaumer::GetAOI(RECT &rtAOI)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->GetAOI(rtAOI);
	return bRet;
}

BOOL CDHGrabForBaumer::Snapshot()
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->Snapshot();
	return bRet;
}



//�õ�����ģʽ
BOOL CDHGrabForBaumer::GetTriggerParam(int &ParamValOutput)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->GetTriggerParam(ParamValOutput);
	return bRet;
}

//�����豸
BOOL CDHGrabForBaumer::Reset()
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->Reset();
	return bRet;
}

//�õ�ϵͳ��Ϣ
BOOL CDHGrabForBaumer::GetSystemInfo(CString &sOutputInfo)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->GetSystemInfo(sOutputInfo);
	return bRet;
}

//����Ĭ�������ļ�
BOOL CDHGrabForBaumer::CreateDefaultConfigurationFile(CString &sInitFile)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->CreateDefaultConfigurationFile(sInitFile);
	return bRet;
}

//���������ļ�
BOOL CDHGrabForBaumer::SaveToConfigurationFile(CString &sInitFile)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->SaveToConfigurationFile(sInitFile);
	return bRet;
}

