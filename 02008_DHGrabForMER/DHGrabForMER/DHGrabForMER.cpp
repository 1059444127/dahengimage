/************************************************************************/
/* Copyright (c) 2006, �й���㼯�ű���ͼ���Ӿ������ֹ�˾   
/* All rights reserved.													
/*																		
/* �ļ����ƣ� GrabberExport.cpp												
/* �ļ���ʶ�� �����ù���ƻ���
/* ժҪ�� �����ɼ�����
/*
/* ��ǰ�汾�� 1.0
/* ���ߣ� ��ͬ
/* ������ڣ� 2013��10��12��
/************************************************************************/

#include "stdafx.h"
#include "DHGrabForMER.h"
#include "CameraBase.H"

CDHGrabForMER::CDHGrabForMER()
{
	//���������ķ���
	m_pGrabber = new CameraBase;
}

CDHGrabForMER::~CDHGrabForMER()
{
	if(m_pGrabber != NULL)
	{
		delete m_pGrabber;
		m_pGrabber = NULL;
	}
}
	
//��ʼ��
BOOL CDHGrabForMER::Init(const s_GBINITSTRUCT* pInitParam)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Init(pInitParam));
}
bool CDHGrabForMER::Init(const CGrabInitInfo& rGrabInitInfo)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Init(rGrabInitInfo));
}

//�ر�
BOOL CDHGrabForMER::Close()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Close());
}

//��ʼ�ɼ�
BOOL CDHGrabForMER::StartGrab()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->StartGrab());
}

//ֹͣ�ɼ�
BOOL CDHGrabForMER::StopGrab()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->StopGrab());
}

// ��֡�ɼ�
BOOL CDHGrabForMER::Snapshot()
{
	ASSERT(m_pGrabber != NULL);
	return(((CameraBase *)m_pGrabber)->Snapshot());
}
	
//�õ�����
BOOL CDHGrabForMER::GetParamInt(GBParamID Param, int &nOutputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->GetParamInt(Param, nOutputVal));
}
	
//���ò���
BOOL CDHGrabForMER::SetParamInt(GBParamID Param, int nInputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->SetParamInt(Param, nInputVal));
}

bool CDHGrabForMER::GetParamBool(GBParamID Param, bool bRetVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->GetParamBool(Param, bRetVal));
}

//���ò���
bool CDHGrabForMER::SetParamBool(GBParamID Param, bool bInputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->SetParamBool(Param, bInputVal));
}

//���ò����Ի���
void CDHGrabForMER::CallParamDialog()
{
	ASSERT(m_pGrabber != NULL);
	m_pGrabber->CallParamDialog();
}

//�õ�������Ϣ
void CDHGrabForMER::GetLastErrorInfo(s_GBERRORINFO *pErrorInfo)
{
	ASSERT(m_pGrabber != NULL);
	m_pGrabber->GetLastErrorInfo(pErrorInfo);
}		

BOOL CDHGrabForMER::MERSetParamInt(MERParamID Param, int nInputVal1,int nInputVal2,int nInputVal3,int nInputVal4,int nInputVal5,void *sInputVal6)
{
	ASSERT(m_pGrabber != NULL);

	BOOL bRet = FALSE;	

	bRet = ((CameraBase *)m_pGrabber)->MERSetParamInt(Param, nInputVal1,nInputVal2,nInputVal3,nInputVal4,nInputVal5,sInputVal6);

	return bRet;
}

BOOL CDHGrabForMER::MERGetParamInt(MERParamID Param, int &nOutputVal1,int &nOutputVal2,int &nOutputVal3)
{
	ASSERT(m_pGrabber != NULL);
	
	BOOL bRet = FALSE;
	bRet = ((CameraBase *)m_pGrabber)->MERGetParamInt(Param, nOutputVal1,nOutputVal2,nOutputVal3);


	return bRet;
}

int CDHGrabForMER::MERSetOutputParamInt(int nOutputPort,int nOutputVal)
{
	ASSERT(m_pGrabber != NULL);
	BOOL bRet = FALSE;

	if(((CameraBase *)m_pGrabber)->SetOutputValue(nOutputPort,nOutputVal))
	{
		return 0;
	}//�����������
	else
	{
		return 1;
	}

}


// �������������Ϣ
void CDHGrabForMER::SaveParamToINI()
{
	((CameraBase *)m_pGrabber)->SaveParamToINI();
}

//
//////////////////////////////////////////////////////////////////////////
