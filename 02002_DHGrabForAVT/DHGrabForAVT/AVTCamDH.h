// AVTCamDH.h: interface for the CAVTCamDH class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVTCAMDH_H__DED529A9_1D8C_4586_AB4D_5583AC9C174D__INCLUDED_)
#define AFX_AVTCAMDH_H__DED529A9_1D8C_4586_AB4D_5583AC9C174D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Afxmt.h"
#include <FGCamera.h>

#include "DHGrabForAVT.h"


typedef struct
{
  BOOL          LimitSpeed;                             // Limit speed
  UINT32HL      Guid;                                   // GUID for this camera
  UINT8         NodeId;                                 // ID of this node
  TCHAR          DeviceName[128];                        // Name of device
  CFrameWnd     *pCameraFrame;                           // Pointer to camera frame
}CAMERAINFO;

enum ErrorStruct
{
	ErrorNoFindCam = 0,		// û���ҵ����
	ErrorGrabberSN,			// �����ų����ܹ��ҵ��������
	ErrorGetCamInfo,		// �õ������Ϣʧ��
	ErrorValOverFlow,		// �������
	ErrorOpenCam,			// �����ʧ��
	ErrorInit,				// ��ʼ��ʧ��
	ErrorStartGrab,			// ��ʼ�ɼ�ʧ��
	ErrorStopGrab,			// ֹͣ�ɼ�ʧ��
	ErrorClose,				// �ر�ʧ��
	ErrorSendBufAdd,		// ����ͼ���ڴ��ַʧ��
	ErrorGetParam,			// �Ƿ���ȡ����
	ErrorSetParam,			// �Ƿ����ò���
	ErrorParamDlg,			// ���ò������öԻ���ʧ��
	ErrorReadIniFile		// ��ȡ�����ļ�ʧ��
};

typedef struct
{
  UINT32                ImageFormat;  //ͼ���ʽ
  UINT32				Format7ModeAdress;  //Format7ModeX��ַ
  UINT32                XSize;        //ͼ����
  UINT32                XSizeMax;     //ͼ�������
  UINT32                XSizeMin;     //ͼ����С���
  UINT32                XSizeUnit;	  //ͼ���Ⱥ���ֱ���
  
  UINT32                YSize;        //ͼ��߶�
  UINT32                YSizeMax;     //ͼ�����߶�
  UINT32                YSizeMin;     //ͼ����С�߶�
  UINT32                YSizeUnit;	  //ͼ��߶�����ֱ���
  
  UINT32                XPos;         //ͼ�����ϽǺ�����
  UINT32                XPosMax;      //ͼ�����Ͻ���������
  UINT32                XPosMin;      //ͼ�����Ͻ���С������
  UINT32                XPosUnit;     //ͼ�����ϽǺ�����ֱ���
  
  UINT32                YPos;        //ͼ�����Ͻ�������
  UINT32                YPosMax;     //ͼ�����Ͻ����������
  UINT32                YPosMin;     //ͼ�����Ͻ���С������
  UINT32                YPosUnit;    //ͼ�����Ͻ�������ֱ���
  
  UINT32                Trigger;     //������ʽ
  UINT32				Debounce;
  UINT32				DebounceMin;
  UINT32				DebounceMax;

  UINT32                FrameBufferCount;    //����֡��

  UINT8                 BayerPattern;           // Used bayer pattern
  UINT8                 YIsRawMode;             // Raw mode flag
  UINT8                 Y16Invert;              // Inversmode for Y16
  
  UINT32                DmaFlags;               // Dma flags

  UINT32                WhiteBalanceCB;         //��ƽ��CB����  
  UINT32                WhiteBalanceMinCB;      //��ƽ����СCB����
  UINT32                WhiteBalanceMaxCB;      //��ƽ�����CB����
  
  UINT32                WhiteBalanceCR;         //��ƽ��CR����
  UINT32                WhiteBalanceMinCR;      //��ƽ����СCR����
  UINT32                WhiteBalanceMaxCR;      //��ƽ�����CR����

  UINT32                Brightness;     //����
  UINT32                BrightnessMin;  //������Сֵ
  UINT32                BrightnessMax;  //�������ֵ
  UINT32                Gain;           //����
  UINT32                GainMin;        //������Сֵ
  UINT32                GainMax;        //�������ֵ
  UINT32                Shutter;        //�ع�
  UINT32                ShutterMin;     //�ع���Сֵ
  UINT32                ShutterMax;     //�ع����ֵ
}CAMPARMS;


const int MaxFileNameLen = 255;			// ��ʼ���ļ�������󳤶�

static int   m_iMaxGrabberSN=0;         //����һ���ɼ���SN  

const int iMaxCameraNum = 20;			//�ɼ���֧�ֵ���������

typedef struct _AVTInitStruct			// ������ʼ���ṹ
{
	int iGrabberTypeSN;						// �ɼ������ͱ��
	int nGrabberSN;							// �ɼ������
    TCHAR strDeviceName[GBMaxTextLen];		// �豸����
	TCHAR strDeviceMark[GBMaxTextLen];		// �豸��ʶ
	TCHAR strGrabberFile[GBMaxFileNameLen];	// ���ڳ�ʼ�����ļ�
	PGBCALLBACK CallBackFunc;				// �ص�����ָ��
	PVOID Context;							// ���ӵ�лص������Ķ����thisָ��	
}s_AVT_INITSTRUCT;// ������ʼ���ṹ

//static CEvent m_NodelistChangedEvent;	//����б�仯�¼�
void WINAPI FGCallBackFunc(void* Context,UINT32 wParam,void* lParam); //�ɼ��ص�����

class CAVTCamDH : public CGrabber 
{
private:
	//////////////////////////////////////////////////////////////////////////
	//�������
	int             m_nGaidHigh;             //���кŸ�λֵ��λ
	int             m_nGaidLow;              //���кŵ�λֵ��λ  
	BOOL            m_InitSucceed;           //��ʼ���Ƿ�ɹ�
	BOOL            m_bSnapShot;			 //�Ƿ������ɼ�

public:
	CAMPARMS        m_CamParms;             // Some parameters...
	CAMERAINFO     *m_pCameraInfo;          // Node lists camera info    
	BOOL            m_CameraActive;         // Camera active or not
	BOOL            m_bIsColorCamera;       //�Ƿ�Ϊ��ɫ��� 
	CFGCamera       m_Camera;               // Our camera we work with
	DWORD           m_FrameCnt;             // Number of received frames
	int				m_nMultishotNum;        //�趨Multishot�ɼ�����

	int      m_nImageWidth;          //ͼ����
	int      m_nImageHeight;         //ͼ��߶�
	int      m_nImageBufferSize;     //ͼ�񻺳�����С
	int      m_nImageByteCount;      //ͼ���ֽ���	
	BYTE*    m_pbImageBufferAdd;  // ���ڴ�����ͼ���ڴ��ַ
	//////////////////////////////////////////////////////////////////////////
	BOOL m_bSet1,m_bSet2,m_bSet3,m_bSet4,m_bSet5;		//���Ʊ�־λ
	
public:	
	CAVTCamDH();
	virtual ~CAVTCamDH();

	//�����ӿ�
	BOOL Init(const s_GBINITSTRUCT* pInitParam);
	virtual bool Init(const CGrabInitInfo& rGrabInitInfo) override;
	BOOL Close();	  // �ر�
	BOOL StartGrab(); // ��ʼ�ɼ�
	BOOL StopGrab();  // ֹͣ�ɼ�
	BOOL Snapshot(); // ��֡�ɼ�
	BOOL SetParamInt(GBParamID Param, int nReturnVal);   
	BOOL GetParamInt(GBParamID Param, int &nReturnVal);
	
	//���ò���
	BOOL SetParamfloat(int Param, float nInputVal);
	
	//�õ�����
	BOOL GetParamfloat(int Param, float &nOutputVal);

	void CallParamDialog();//���ò����Ի���
	void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo);//�õ�������Ϣ
	
	BOOL GetTriggerParam(int &ParamValOutput);
	BOOL SetTriggerParam(int ParamValInput);
	BOOL GetSystemInfo(CString &sOutputInfo);
	BOOL SetAOI(RECT rtAOI);
	BOOL GetAOI(RECT &rtAOI);
	BOOL Reset();
	BOOL CreateDefaultConfigurationFile(CString &sInitFile);

	//�ص�����
	void OnNewFrame();
	void OnNodelistChanged();
	void OnError(int nError);
protected:

	void CheckResult(CString pFunction,DWORD Result,int n);
	BOOL SetCameraFromINI();
	
	BOOL Init_Grabber_Module(); // ��ʼ���ɼ�ģ�� 
	BOOL Exit_Grabber_Module(); // �ͷŲɼ�ģ��
	BOOL InitParm(const s_AVT_INITSTRUCT* pInitParam); // ��ʼ��
	BOOL InitCamera();
//	int TestFramerate();
	//BOOL m_bTestFramerate;

	//�ж�·���Ƿ����
	BOOL IsPathExist(const CString &strPath);
	//�ж��ļ��Ƿ����
	BOOL IsFileExist(const CString &strFileName);
	//����һ�����Ŀ¼��������ھͲ�����
	//(strPath  �Ϸ�: "D:\\abc\\me" �� "me\\you" �� ""; 
	//			���Ϸ�: "D:\\abc\\me\\" �� "me\\you\\" �� "\\")
	BOOL CreateMultiLevelPath(const CString &strPath);

	//���������ļ�
	BOOL SaveToConfigurationFile(CString &sInitFile);

	int m_nGrabberSN;				// ���

	// �������� [4/8/2014 SJC]
	CString m_strLanType;

protected:	

	int m_nGrabberTypeSN;           // �ɼ������ͺ� 
	CString m_sInitFile;			// ���ڳ�ʼ�����ļ�
	CString m_sDeviceName;			// �豸��
	CString m_sDeviceMark;          // �豸��ʶ  
	PGBCALLBACK m_CallBackFunc;		//�ص�����ָ��
	PVOID m_Context;				// ��ŵ��ó�ʼ�������Ķ����thisָ��
	s_GBERRORINFO m_LastErrorInfo;	// ������Ϣ

//	int m_nInitNum;
	UINT32 m_lFrameLengh;
	BOOL m_bFirstFrame;
};

#endif // !defined(AFX_AVTCAMDH_H__DED529A9_1D8C_4586_AB4D_5583AC9C174D__INCLUDED_)
