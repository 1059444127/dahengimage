// DALSACamDH.h: interface for the CDALSACamDH class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BAUMERCAMDH_H__3407C25C_9F6F_44BF_8DCF_3E41CFAC6409__INCLUDED_)
#define AFX_BAUMERCAMDH_H__3407C25C_9F6F_44BF_8DCF_3E41CFAC6409__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DHGrabForBaumer.h"
#include <vector>
//#include "bgapi.h"
#include "bgapi.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <conio.h> //20120815
using namespace std;
////////////////////////////////////////////////////////////////////////////
//
//���������豸�����
const int SystemMaxNum  = 10;  

//������������
const int CameraMaxNum  = 10;

//����ͼ����������
const int ImageMaxNum  = 10;

// ��ʼ���ļ�������󳤶�
const int MaxFileNameLen = 255;

typedef struct _BAUMERInitStruct			// ������ʼ���ṹ
{
	int nGrabberSN;							// �ɼ������
    char strDeviceName[MaxFileNameLen];		// �豸����
	char strDeviceMark[GBMaxTextLen];		// �豸��ʶ
	char strGrabberFile[MaxFileNameLen];	// ���ڳ�ʼ�����ļ�
	PGBCALLBACK CallBackFunc;				// �ص�����ָ��
	PVOID Context;							// ���ӵ�лص������Ķ����thisָ��
}s_BAUMER_INITSTRUCT;// ������ʼ���ṹ

typedef struct _SystemCameraStruct			// ϵͳ�������������ʼ���ṹ
{
	BGAPI::Camera *m_pCamera;    //�������ָ��
	BGAPI::System * m_pSystem;   //���ϵͳָ��
}s_SYSTEMCAMERASTRUCT;// ϵͳ�������������ʼ���ṹ


class CBaumerCamDH : public CGrabber  
{
protected: //�������

	//�ɼ��ص�
	static BGAPI_RESULT BGAPI_CALLBACK imageCallback( void * callBackOwner, BGAPI::Image* pImage  );

	//��ʼ�������豸
	int init_systems(int * system_count, BGAPI::System ** externppSystem ,BGAPI::Camera ** externppCamera);
	//��ʼ�����
	int init_camera();

    //��ʼ��
	BOOL InitParm(const s_BAUMER_INITSTRUCT* pInitParam); 
	void Fun1();
	int Fun2(); 



protected: //�������

	DWORD          m_FrameCnt;                                 //�ɼ�ͼ�������

public:
	CBaumerCamDH();
	virtual ~CBaumerCamDH();

	BOOL Init(const s_GBINITSTRUCT* pInitParam);                //��ʼ��
	virtual bool Init(const CGrabInitInfo& rGrabInitInfo) override;
	
	BOOL StartGrab();                                           // ��ʼ�ɼ�

	BOOL StopGrab();                                            // ֹͣ�ɼ�

	BOOL Snapshot();											//��֡�ɼ�

	BOOL Close();	                                            // �ر�
	
	BOOL GetParamInt(GBParamID Param, int &nOutputVal);         //���ò���

	BOOL SetParamInt(GBParamID Param, int nInputVal);          //���ò���
	
	void CallParamDialog();                                     //���ò����Ի���
	
	void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo);           //�õ�������Ϣ

	BOOL SetParamfloat(int Param, float nInputVal);
	BOOL GetParamfloat(int Param, float& nOutputVal);
	BOOL SetTriggerParam(int ParamValInput);						//���ô�������
	BOOL SetAOI(RECT rtAOI);													//���òɼ�����
	BOOL GetAOI(RECT &rtAOI);													//�õ��ɼ�����



	//�õ�����ģʽ
	BOOL GetTriggerParam(int &ParamValOutput);
	
	//�����豸
	BOOL Reset();
	
	//�õ�ϵͳ��Ϣ
	BOOL GetSystemInfo(CString &sOutputInfo);
	
	//����Ĭ�������ļ�
	BOOL CreateDefaultConfigurationFile(CString &sInitFile);
	
	//���������ļ�
	BOOL SaveToConfigurationFile(CString &sInitFile);


	BOOL SetImageFlipMode(int iType);
	/*ʹ����Ա����Խ����Ա����˽�з��ʵ����⣬���±���ԭΪprotected����*/
	s_GBERRORINFO		m_LastErrorInfo;	    //������Ϣ2007.12.18ʹ���ƶԻ�����Է��ʣ��ṩ�������
    BOOL			    m_bGrabbing;	        //�Ƿ����ڲɼ�
	BOOL			    m_bInitSucceed;         //�Ƿ��ʼ���ɹ�
	CString				m_sInitFile;			//���ڳ�ʼ�����ļ�


	
protected:
	int  m_nGrabberSN;							//���
	CString m_sDeviceName;			            //�豸��
	CString m_strDeviceMark;					//�豸��ʶ
	PGBCALLBACK m_CallBackFunc;					//�ص�����ָ��
	PVOID m_Context;							//��ŵ��ó�ʼ�������Ķ����thisָ��

	int m_nImageWidth;                          //ͼ����
	int m_nImageHeight;                         //ͼ��߶�

	int m_nImageBufferSize;                     //ͼ�񻺴��С
	int m_nImageByteCount;                      //ͼ��ÿ�����ص��ֽ���
	void*  m_pbImageBufferAdd;                  //���ڴ�����ͼ���ڴ��ַ

	BOOL m_bSet1,m_bSet2,m_bSet3;						//����������ÿ��Ʊ�־
	BOOL m_bSquencerShot;								//��֡�ɼ����Ʊ�־
	int m_nTrigger,m_nSequencerNum;

	int m_nExMax,m_nExMin;						//��������С���ع�ֵ
	float m_nGainMax,m_nGainMin;				//��������С������ֵ

	BGAPI::Camera *m_pCamera;					//��ǰ�������ָ��
	BGAPI::System * m_pSystem;					//��ǰϵͳ����ָ��

	BGAPI::System * m_ppSystem[SystemMaxNum];			//��¼�Ѿ������������豸��������
	s_SYSTEMCAMERASTRUCT m_SystemCamera[CameraMaxNum];	//��¼�Ѿ������������������
	BGAPI::Image *m_pImage[ImageMaxNum];				//��¼�Ѿ�������ͼ����ж�������
	int m_CameraNum;									//ʵ�ʴ���������������

	int m_ndoSequencerReset;

};

#endif // !defined(AFX_BAUMERCAMDH_H__3407C25C_9F6F_44BF_8DCF_3E41CFAC6409__INCLUDED_)
