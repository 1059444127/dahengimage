// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__4B8DECDB_A7FA_4037_B1C9_9B8E1F287E47__INCLUDED_)
#define AFX_MAINFRM_H__4B8DECDB_A7FA_4037_B1C9_9B8E1F287E47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LogManager.h"
#include "SpendTime.h"
#include "..\\..\\IncPublic\\ObjAlloc.h"
#include "ParamDefine.h"
#include "PIO24BLib.h"
#include "..\\..\\IncPublic\\alg_struct_Label.h"
#include "..\\..\\IncPublic\\ExtenInfo.h"
#include "..\\..\\IncPublic\\ExFn.h"
#include "..\\..\\IncPublic\\ThreeTierRights.h"

#include <array>
#include <queue>

#define SPOT


#ifdef SPOT
 #define __RealGrabber_							//���ʵʱ�ɼ�
 #define   USE_IO_CARD							//ʹ�ýӿڿ�ʱ��
#else

#define __SGGrabber_                                //ģ��ɼ�
#endif

//#define	  c_KickCount				2				//�߷Ͽڸ���
#define   PIC_COUNT						30			//ÿ��ƿ�����ո���
#define	  PE_COUNT						3			//�����źŸ���
#define	  IOCARD_COUNT					2			//IO������
#define   CAMERA_COUNT						 6			//ϵͳʹ�õ��������    
#define   CHECK_THREAD_COUNT            8			//����̵߳ĸ���
//#define   m_iSysDataEleLen			1			//�ص��ж��и���
#define   MAX_DEFECT_TYPE_COUNT		30			//ȱ�������ܸ���,ÿ����λ40��
#define   ID_START_CHECK_TIMER          102         //���ϸ������ݿ���Ϣ
#define   ID_UPDATE_DEFECT_INFO_TIMER	103				//���ϸ���ȱ��ͳ����Ϣ
#define	  ID_UPDATE_DEFECT_SHOW_INFO	104				//���ϸ���ȱ��ͳ����ʾ��Ϣ
#define   MAX_INIT_IMAGE_NUM			100000000		//��ʼ��ͼ���


/************************************************************************/
/*                       [5/16/2017 ConanSteve]                         */
const int c_AllElementCount{ 6 };			//�������ĸ���
const int c_KickCount{ 3 };
const bool g_bCSRecord{ true };
enum e_KickPort {
	e_KickPort_1=0,
	e_KickPort_2,
	e_KickPort_3
};

enum e_IOCard {
	e_IOCard_1,
	e_IOCard_2
};

enum e_KickMode {// �޷�ģʽ 0Ϊȫ�ߣ�1Ϊȫ���ߣ�2Ϊ�����ߣ�3Ϊ������
	e_KickMode_KickAll,
	e_KickMode_AllNotKick,
	e_KickMode_Alternative,
	e_KickMode_Noraml
};

#define TIMER_SG_READ_IMG 0x00FFFFFF
/************************************************************************/

#ifdef __RealGrabber_
// #include "..\\Inc\\DHGrabberJAI.h"
#include "..\\Inc\\DHGrabberForMER.h"
// enum GBParamID
// {
// 		GBImageWidth = 0,		//ͼ���� (��λ:�ֽ�)
// 		GBImageHeight,			//ͼ��߶� (��λ:�ֽ�)
// 		GBImagePixelSize,		//ͼ�����ش�С (��λ:�ֽ�)
// 		GBImageBufferSize,		//ͼ�񻺳�����С (��λ:�ֽ�)
// 		GBImageBufferAddr,		//ͼ�񻺳�����ַ
// 		GBGrabberTypeSN			//�ɼ������ͱ��
// };
#else
#include "..\\Inc\\GrabberExportForSG.h"
#endif

static int iTest[6]{ 0 };
class CMainFrame : public CFrameWnd
{

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	// Attributes
public:
	bool m_bCheckPushed{ false };

	array<int, c_AllElementCount>  m_iStopCountDiff;						// ��i����λֹͣ�����ж����� 
	array<int, c_AllElementCount>  m_iInspectionObj;						// ��i����λ�ļ��������
	array<int , c_AllElementCount> m_iHardImageCount;					// ��i����λ��ͼ����ź�
	array<int ,c_AllElementCount> m_iLastHardImageCount;				// ��i����λ�ϴ�ͼ����ź�
	array<int, c_AllElementCount> m_iLastImageCount;					// ��i����λ���ϴ�ͼ��Ĵ�������
	array<int, c_AllElementCount>	m_iImageCountOverFlowNumber;			// ��i·�������������� 
	array<int, c_AllElementCount> m_iSaveCameraImageCount;				// ��������ͼ��Ĵ����� ��������
	array<int,c_AllElementCount> m_iSaveCameraImageCount_Setting;		// �趨����Ҫ�����ͼ������
	array<int, c_AllElementCount> m_iSaveCircleCameraImageCount;		// ѭ������ͼ�������
	int m_iSaveImageType{ 0 };					// ����ͼ������
	int	m_iIsCircle{0};							// �Ƿ�ѭ������ͼ��
	array<int ,c_AllElementCount> m_iKickInterval;						// �����ļ��в������㷨�����೤ʱ�䵼���޷�
	array<int, c_AllElementCount> m_iGrabCountSingleCircle;				// һ�����ڲɼ���ͼ����---�����ķ���
	array<int, c_AllElementCount> m_iImageKickThread;					// ���޷��߳���ʹ�õ�ͼ���
	array<int, c_AllElementCount> m_iImageCheckThread;
	array<int, c_AllElementCount> m_iImageCountInPE;					// ��ȡͼ���ʱʹ�õ�ͼ����ֵ
	array<int, c_AllElementCount> m_iLastImageCountInPE;				// ��ȡͼ���ʱʹ�õ��ϴ�ͼ����ֵ
	array<int, c_AllElementCount> m_iFlowOverImageCountInPE;			// ��ȡ����ͼ���ʱʹ��ͼ����������
	array<int, c_AllElementCount> m_iHardKickCount;						// ��֡�߷ϼ���(��λi)
	array<int, c_AllElementCount> m_iLastKickCount;						// �޷�i����һ�μ���	
	array<int, c_AllElementCount> m_iKickFlowNumber;					// �޷�i���������
	array<int, c_AllElementCount> m_iLastImageID;						// ��i���޷��߳�����һ�ε�ͼ�����
	array<array<int, 256>,c_AllElementCount> m_iCheckResultTable;				// ��λi�ļ������¼��,�����޷��߳�
	array<int, c_AllElementCount> m_iGrabCircleCount;					// �ɼ��Ĵ��ż���
	array<int, c_AllElementCount> m_iTubeCnt{ 0 };						/** �ɼ��й�ͼ�ļ���              [5/26/2017 ConanSteve]**/

	array<int, c_AllElementCount> m_iCheckErrorCount{0};					// ��������
	array<int, c_AllElementCount> m_iCheckImageCount;					// ����ܸ���
	array<int, c_AllElementCount> m_iDefectTypePerWS;					// ÿ����λ��ȱ�����͸���
	array<array<int, MAX_DEFECT_TYPE_COUNT>, c_AllElementCount> m_iDefectCount{0};// ÿ��ȱ������ͳ����ֵ
	array<int, c_AllElementCount> m_iLabelErrorCount;					// 1Ϊ��ǩλ�ô��������2Ϊ��ǩ�����������󱨾�����
	array<int, c_AllElementCount> m_iAbnormalImage;						// �����쳣ͼ������
	array<int, CAMERA_COUNT> m_iOriginSaveImageNum;                    // ��¼����ͼ��ǰM���¸���λ����ͼ�����������ڿ�����ͼ���滻��ͼ��ʹ��
	array<int, CAMERA_COUNT> m_iPicCount;								// ÿ��������вɼ�ͼ��
	array<int, CAMERA_COUNT> m_iCamCardNum;							// ÿ�����ʹ�õĽӿڿ����
	array<int, CAMERA_COUNT> m_iCamOutPort;							// ÿ�����ʹ�õ�OUT�ڱ��

	array<int, c_AllElementCount> m_iFrameCount{0};								// ����ص��ж�ȡ�Ĺ�����  [2012-2-10 by zhy] 
	array<int, c_AllElementCount> m_iLastFrameCount{0};							// ����ص��ж�ȡ�Ĺ����һ�ε�֡��	
	array<int, c_AllElementCount> m_iFrameCountOverFlowNumber{0};				// ����ص��ж�ȡ�Ĺ��֡��������Ĵ���

	array<int,c_AllElementCount> m_iRC_PE;										// ��¼���
	array<int, c_AllElementCount> m_iRC_ThreadBad;						// ����߳��и������ź�ʱ��¼
	array<int, c_AllElementCount> m_iRC_ThreadGood;						// ����߳��и������ź�ʱ��¼
	array<int, c_AllElementCount> m_iRC_Good;							// д���źż�¼
	array<int, c_AllElementCount> m_iRC_AddKickImage;					// ��¼����ͼ���
	array<int, c_AllElementCount> m_iRC_KickImage;						// ��¼�޷�ͼ���
	array<int, c_AllElementCount> m_iRC_ImageNum;						// ��¼�ظ���¼
	array<int, c_AllElementCount> m_iRC_Image;							// ��¼�������̵߳�ͼ���
	array<int, c_AllElementCount> m_iRC_BeforeD;						// ��¼�㷨����֮ǰ��ͼ���
	array<int, c_AllElementCount> m_iRC_Detect;							// ��¼�㷨�����ͼ��ż����ʱ��
	array<int, c_AllElementCount> m_iRC_Kick;							// ��¼�޷�ͼ���
	array<int, c_AllElementCount> m_iRC_Call;							// ��¼�ص���ͼ��ż��ص����
	array<int, c_AllElementCount> m_iRC_DetectTime;						// ��¼�㷨���ʱ��
	array<int, c_AllElementCount> m_iRC_RecordImage;					// ��¼�߳���ͼ��ű仯ʱ��ʱ��
	array<int, c_AllElementCount> m_iRC_SetEvent;						// �������߳�
	array<int, c_AllElementCount> m_iRC_WriteImage;						// ��ȡͼ������[2014-12-5 zhy]
	array<int, c_AllElementCount> m_iRC_StopMachine;					// ͣ������
	array<int, c_AllElementCount> m_iRC_RecordKick;						// ��¼��ȡͼ����е��޷���Ϣ
	array<int, CAMERA_COUNT> m_iCamSN;									// ���ͼ�����
	array<int, CAMERA_COUNT> m_iPicCountPerCam;						// ÿ������ĵ�����ͼ����

	int m_iSaveImageSys;										//�Ƿ�ʹ��ϵͳ���򱣴�ͼ�� 0Ϊ��1Ϊ��
	int m_iShowDevice;											//��ʾͼ����豸 0Ϊϵͳͼ�� 1Ϊ�㷨ͼ��
	int m_iKickOffset5;												//���5������6�Ĺ��Ӹ���
	int m_iEmptyCount;											// ��ѹͷ����
	int m_iLicenseExpDate;										// License��������ʱ��
	int m_iCamTestMode;											// �������ģʽ 0Ϊ�����ɼ� 1Ϊ����ģʽ
	int m_iKickMode;											// �޷�ģʽ 0Ϊȫ�ߣ�1Ϊȫ���ߣ�2Ϊ�����ߣ�3Ϊ������
	int m_iSaveImage_Abnormal;									// �����쳣ͼ������
	int m_iRecord_Set;											// �趨�ı����¼������
	int m_iErrorSignal;											// �󴥷����

	int m_iAlarmCardNum;										// ������ʹ�õĽӿڿ����
	int m_iAlarmOutPort;										// ������ʹ�õĽӿڿ�����˿ں�
	int m_iCheckCountTotal;										// �������
	int m_iCheckErrorCountTotal;								// ������ƿ�Ǹ���ͳ��(���һ��ʹ�ܽӿڿ���ļ���) 
	int m_iCheckGoodCountTotal;
	int m_iMaxCameraImageWidth;									// �ɼ�ͼ����
	int m_iMaxCameraImageHeight;								// �ɼ�ͼ��߶�
	int m_iMaxCameraImageSize;									// �ɼ�ͼ���С
	int m_iMaxCameraImagePixelSize;								// �ɼ�ͼ�����ش�С
	int m_iImageShowMode;										// ��ʾͼ��ģʽ 0:����ʾ; 1:��ʾ����; 2:����ʾ����
	int m_iIntervalDisplayTime;									// ͼ����ʾ���ʱ��
	int m_iMaxSaveImageCount;									// ÿ����λ����ͼ����
	int	m_iPreAllocateSaveInfoElementCount;						// Ԥ���䱣��ͼ����ϢԪ�ظ�����������ϢԪ�ء�һ�����ڲɼ��ĸ���
	int	m_iMaxGrabCountSingleCircle;							// ȫ����λ��һ�����ڵĲɼ�ͼ������ֵ
	int	m_iLastCheckCount;										// ��¼ģ��ɼ�ʱ��һ�ε�ͼ����������ڼ���ʵʱ����ٶ�
	int m_iCheckSpeedInterval;									// �������ٶȵ�ʱ����
	int m_iLastCheckSpeedCount;									// ����ٶȼ�������

	BOOL m_bHaveStartSaveThread;						// �Ƿ����˴�ͼ�߳�
	BOOL m_bFirstCheck;											// ���������߸�λ
	BOOL m_bAdvMode;											// �Ƿ��ڸ߼�����Աģʽ
	BOOL m_bReadModel;											// �ж϶�дģ���Ƿ�ɹ�
	bool m_bStartRecord;										// ���ƿ�ʼ��¼
	BOOL m_bIsDetecting;										// �ж��Ƿ������ڼ��״̬
	BOOL m_bIOCardValid;										// �ӿڿ��Ƿ���Ч
	BOOL m_bIOCardInitSuccess;									// �ӿڿ��Ƿ��ʼ���ɹ�
	BOOL m_bOpenAlgCheck;										// �Ƿ����㷨���[2015-3-12 zhy]
	BOOL m_bOpenImageSave;										// �Ƿ񱣴�ͼ��
	BOOL m_bCalAlg;												// �Ƿ�����㷨���
	BOOL m_bSoftResetIOCard;									// �Ƿ������λ�ӿڿ�
	BOOL m_bPauseSaveImageThread;								// �Ƿ���ͣ����ͼ���߳�
	BOOL m_bKillSaveImageThread;								// �Ƿ�ֹͣ����ͼ���߳�
	BOOL m_bCloseCount;											// ���ڿ����Ƿ��ȡIO���е�ͼ�����
	BOOL m_bWaitDemoMould;
	BOOL m_bStopReadKickImageNo;								// ��ͼ���

	array<BOOL, c_AllElementCount> m_bIsKick;							// ��·�Ƿ��߷�
	array<BOOL, c_AllElementCount> m_bSaveCameraImage;					// �Ƿ񱣴�ͼ��
	array<BOOL, c_AllElementCount> m_bDisplayImage;						// ������λ�Ƿ���ʾͼ��
	array<BOOL, c_AllElementCount> m_bDisplayAlgImage;					// ������λ�Ƿ���ʾ�㷨ͼ��
	array<BOOL, c_AllElementCount> m_bIsCheck;							// �Ƿ���
	array<BOOL, CAMERA_COUNT> m_bIsSimulateIniExist;					// ģ���ͼ�������ж�ģ��ɼ�ʱ�����ļ��Ƿ����
	array<BOOL, CAMERA_COUNT> m_bGrabFlip;								// �Ƿ���з�ת
	array<array<BOOL, 256>,c_AllElementCount> m_bTubeCheckResult;			// ��������ж���������ļ���� ����Ϊ�棬�޴�Ϊ��
	array<array<BOOL,256>, c_AllElementCount> m_bCheckResultOffset;			// ��λ1�Ƿ��޷�ƫ�� 
	array<BOOL, c_AllElementCount> m_bLabelAlarm;						// ��ǩ��ⱨ��
	array<BOOL, c_AllElementCount> m_bAlarm_StopCount;					// ֹͣ��������
	array<array<array<BOOL, PIC_COUNT>,256>,c_AllElementCount> m_bWriteImage;			// �Ƿ�д������


	CString m_sModelName;										// ��ǰģ������
	CString	m_sOldModelName;									// �ɵ�ģ������
	CString	m_sModelPath;										// ģ��·��
	CString m_sWorkPath;										// Ӧ�ó�����·��
	CString m_sSysConfigFileName;								// ϵͳ�����ļ�
	CString	m_sVirtualDisk;
	CString m_sCipherPath1;										// ϵͳ�����ļ���
	CString m_sStartTime;										// ��¼��ʼ���ʱ��
	CString m_sImageFilePath;									// ͼ���ļ���·��
	CString m_sLogFilePath;										// ��־�ļ���·��
	CString m_sAlgIniFilePath;									// �㷨���ò����ļ���·��
	CString	m_sCheckLogFilePath;								// �����־�����ļ���
	CString	m_sDataFilePath;									// �������ݿ��ļ�·��
	CString m_sWindowTitle;										// ���ڱ���
	CString m_sAlgConfigFilePath;										// �㷨���ó���DEMO�������ļ�·��
	CString m_sDemoName;										// Demo����
	array<CString, CAMERA_COUNT>m_sSimulateGrabPath;					// ģ���ͼ·��
	array<array<CString, MAX_DEFECT_TYPE_COUNT>,c_AllElementCount> m_sDefectInfo;//ÿ��ȱ�ݵ�����
	array<array<CString,256>,c_AllElementCount> m_strErrorDesc;				//�㷨���صĴ�����Ϣ����,�����б��
	array<CString, c_AllElementCount> m_sSaveImagePath;					//����ͼ��·��
	array<CString, CAMERA_COUNT> m_sCamConfigPath;					//��������ļ�·��
	array<CString, CAMERA_COUNT> m_sCamConfigName;					//��������ļ�����


	array<CFile, c_AllElementCount> m_filePE;									// ��¼���
	array<CFile, c_AllElementCount> m_fileThreadBad;					// ����߳��и������ź�ʱ��¼
	array<CFile, c_AllElementCount> m_fileThreadGood;					// ����߳��и������ź�ʱ��¼
	array<CFile, c_AllElementCount> m_fileGood;							// д���źż�¼
	array<CFile, c_AllElementCount> m_fileAddKickImage;					// ��¼����ͼ���
	array<CFile, c_AllElementCount> m_fileKickImage;					// ��¼�޷�ͼ���
	array<CFile, c_AllElementCount> m_fileImageNum;						// ��¼�ظ���¼
	array<CFile, c_AllElementCount> m_fileImage;						// ��¼�������̵߳�ͼ���
	array<CFile, c_AllElementCount> m_fileBeforeD;						// ��¼�㷨����֮ǰ��ͼ���
	array<CFile, c_AllElementCount> m_fileDetect;						// ��¼�㷨�����ͼ��ż����ʱ��
	array<CLogFile, c_AllElementCount> m_fileKick;							
	array<CFile, c_AllElementCount> m_fileCall;							// ��¼�ص���ͼ��ż��ص����
	array<CFile, c_AllElementCount> m_fileDetectTime;					// ��¼�㷨���ʱ��
	array<CFile, c_AllElementCount> m_fileRecordImage;					// ��¼�߳���ͼ��ű仯ʱ��ʱ��
	array<CFile, c_AllElementCount> m_fileSetEvent;						// �������߳�
	array<CFile, c_AllElementCount> m_fileWriteImage;					// ��ȡͼ������[2014-12-5 zhy]
	array<CFile, c_AllElementCount> m_fileStopMachine;					// ͣ��������־
	array<CFile, c_AllElementCount> m_fileRecordKick;
	

	CEvent m_evtTrigDemoMould;									// ���ȴ�demo����ģ���߳��Ƿ�����[2014-11-4]
	CEvent m_evtGetIOCardImageNum;								// ��ȡ�����߳��Ƿ�����
	CEvent m_evtKillWait;

	double m_dFreq;												// ��¼�����ʱ��Ƶ��
	double m_dwCheckBadRatio;									// ��Ʒ��
	array<double, c_AllElementCount> m_dAlgCheckTime;					// �㷨���ʱ��

	//Bayer�仯��ر���
	array<BYTE,256>  m_pLutR;
	array<BYTE, 256>   m_pLutG;
	array<BYTE, 256>   m_pLutB;
//	BYTE* m_pAlgElement[CAMERA_COUNT];							// ר��Ϊ�㷨�Ĳ������ú��޸�ģ���������ڴ�
	array<BYTE*, CAMERA_COUNT> m_pbRGBImage;							// ��ͼ�����任ͼ�񻺳���
	//array<BYTE*, c_AllElementCount> m_pImageAddr;						// ���ͼ������
	array< shared_ptr<BYTE>, c_AllElementCount> m_pImageAddr{nullptr};
	array<array<CSpendTime,256>, c_AllElementCount> m_SpendTime;				// ͳ���Ƿ�ʱʹ�� 
	LONGLONG m_prevqt1;											// ��¼���湤λ�ϴλص�������ȡ�ļ����ʱ����Ϣ
	array<LONGLONG, c_AllElementCount> m_lCameraTimeInfo;				// ��¼������λ�ϴλص�������ȡ�ļ����ʱ����Ϣ
	array<LONGLONG, CAMERA_COUNT> m_lLastQt_PE;							// ��¼����ϴμ���
	array<LONGLONG, CAMERA_COUNT> m_lLastQt_TX;						// ��¼ͼ����ϴμ���


	array<CCriticalSection, CAMERA_COUNT> m_csImagePath;				// ����ͼ�񱣴�·������ֹ��������
	array<CCriticalSection, c_AllElementCount> m_csElement;				//������Ϣ


	_ConnectionPtr m_Connect;									// �������ݿ�
	_RecordsetPtr m_RecordSet;									// ��¼��

	CLogManager  m_LogManager;									// ��־������
	array<CPIO24BLib,IOCARD_COUNT> m_IOCard;			     			// PIO24B�ӿڿ�����
	array<CBayerTransform, CAMERA_COUNT> m_BayerTransform;

	array<BITMAPINFO*, CAMERA_COUNT> m_pBitmapInfo;
	array<e_ShowModeFlip, CAMERA_COUNT> m_eShowModeFlip;				// ���з�ת����
	//array<array<s_ElementInfo, m_iSysDataEleLen>, c_AllElementCount> m_sElement;			// ��¼�ص���Ϣ
	array<s_ElementInfo*, c_AllElementCount> m_sElement;
	array<s_ImageInformationStruct, CAMERA_COUNT> m_sCameraImageInfo; //����ɼ���ͼ����Ϣ

	CObjAlloc m_InspectionObj;									// ������
	array<LabelCheck::s_AlgInspResult, 256>   m_sOut5;					//���5���������

	array<s_ThreadParam, CHECK_THREAD_COUNT> m_sThreadParam;				//����̲߳���
	array<HANDLE, c_AllElementCount> m_hImageSaveStart;							// ��������ͼƬ�¼��������

	array<BOOL, 5> m_arr_iAlgStopMachineCtrl;//**[12/20/2017 ConanSteve]**:  �㷨ͣ������


#ifdef __SGGrabber_
	array<CDHGrabberSG, CAMERA_COUNT>  m_CameraGrabber;				// ����ɼ�����
#else
//	CDHGrabberAVT  m_CameraGrabber[CAMERA_COUNT];				// ����ɼ�����
// 	CDHGrabberJAI m_CameraGrabber[CAMERA_COUNT];
	array<CDHGrabberMER, CAMERA_COUNT> m_CameraGrabber;

#endif
	

// Operations
public:

	// ִ�б�������,bStartΪTRUE��ʼ������bStartΪFALSEֹͣ����
	BOOL AlarmAction(BOOL bStart);

	// ��ȡ��ǰʱ��
	CString GetCurTime(DWORD* dwTime=NULL);

	// ��ʼ���
	BOOL StartCheck();

	// ֹͣ���
	BOOL StopCheck();

	//�˳�������
	inline void QuitCheckProgram();								

	// ��ʼ�����ݿ⣨����ɾ���ռ�¼�������¼�¼����ʼ���ݿ���£�
	void InitDataBase();

	// �ж����ݿ����Ƿ��м������Ϊ0�ļ�¼�����У������ݿ���ɾ���������Ϊ0�ļ�¼
	void InitTableData(CString strTableName);		
	
	// �������һ����¼
	void UpdateTableData(s_CheckResultInfo info, CString strTableName);	

	// ����һ���¼�¼
	void InsertTableData(s_CheckResultInfo info, CString strTableName);	

	// ����ȱ����Ϣ��¼
	void UpdateDefectTableData(s_CheckDefectInfo info, CString strTableName);	

	// ����һ��ȱ����Ϣ�¼�¼
	void InsertDefectTableData(s_CheckDefectInfo info, CString strTableName);	

	// ����access���ݿ�
	BOOL ADOConnectDB();

	// ���ɱ�
	BOOL CreateTable(CString strTable);		
	
	// �ж����ݱ��Ƿ�������ݿ���
	BOOL ExistTable(CString strTable);									

	// ���ýӿڿ�����е���ض���
	void SoftResetIOCard(CString strModelName);

	// ��������Ӳ��M���µ�����ͼƬ����
	inline BOOL LoadAllImage();                                 

	// ����ͼ��
	int savebmp(const char *lpszPathName,int W,int H, int nByte, unsigned char *lpDest);

	// �����쳣ͼ��
	int SaveAbnormityImageFunc(const char* pPath,int iWidth, int iHeight, int iByte, unsigned char* pAddr);

	// ��ʼ����⶯̬��
	void InitInspectionDll();

	// ��ȡ��ִ�г������ڵ�·��
	CString GetCurrentAppPath();

	//�����ļ���·��
	BOOL FindFilePathFunc();
	
	// ��ȡϵͳ�����ļ�
	BOOL ReadSystemConfig(const CString &strLoadFile);	
	
	// ����ϵͳ�����ļ�
	BOOL SaveSystemConfig(const CString &strLoadFile);		

	// �ļ�����
	BOOL OP_Directory(LPCSTR lpsrcPath, LPCSTR lpdstPath, int type,int flag);
	
	// �������Ŀ¼
	BOOL inCreateMultiLevelPath(const CString &strPath);
	
	// �ж�Ŀ¼�Ƿ����
	BOOL inIsPathExist(const CString& strPath);

	//����LUT��
	BOOL SetLutTable(double dRatioR,double dRatioG,double dRatioB); 

	// ��ʼ���ӿڿ�
	inline BOOL InitIOCard();	
	
	// �ͷŽӿڿ�
	inline BOOL ReleaseIOCard();								
	
	// ��ʼ���ڴ�������
	inline BOOL InitDataFunc();		
	
	// �ͷ��ڴ�������
	inline BOOL ReleaseDataFunc();								
	
	// ��ʼ���ɼ�����
	inline BOOL InitGrabber();	
	
	// �ͷŲɼ�����
	inline BOOL ReleaseGrabber();	

	// ���������߳�
	inline BOOL StartAllThread();	
	
	// ɱ�������߳�
	inline BOOL KillAllThread();	

	// ��ʼ����ɼ�
	inline BOOL StartCameraGrab(const int iCameraSN);
	
	// ֹͣ����ɼ�
	inline BOOL StopCameraGrab(const int iCameraSN);			
	
	// ��ȡ�����ź���Ϣ
	inline BOOL ReadMachineSignal(int iGrabSn);	

	//��ʾͼ��
	BOOL  ThreadDisplayImage(int iCamGroup, int iIndex);		
	
	//���� �����������ӵĺ��� [4/30/2010 GZ]
	BOOL  CameraGrabFuncSingle(int iCameraSN,int iAllElementSN ); //���������ʵ�ʲɼ�����
	BOOL  CameraGrabFuncSingleEx(int iCameraSN, int iAllElementSN);
	//�ж�һ���ɼ�����
	inline BOOL  JudgeGrabCircleNew(int  iCameraSN,							//������  [11/16/2009 GZ]
									int  iAllElementSN,						//��λ��
									int& iGrabCircle,						//��¼�ɼ�ͼ���
									int& iPicSN);							//���ͼ�����к�

	//��ͼ�����ݽ��о���,ֻ��ˮƽ�ʹ�ֱ����
	int MirrorSingleImage(BYTE* pImageBuff, int lImageWidth, int lImageHeight, int lImageBitCount, e_ShowModeFlip  eShowMode);

	//�Ƿ��ͼ�����ݽ�����ת
	int RotateImage(BYTE* pSrcImageBuff, int iSrcWidth, int iSrcHeight, int iImageBitCount, float fRotateAngle, 
		BYTE* pDestImageBuff, int& iDestWidth, int& iDestHeight);

	// 1��λ�ɼ��ص�	
	static void WINAPI GlobalAreaGrabOverCallback1 (const s_GBSIGNALINFO* SigInfo);
	void AreaGrabOverCallback1(const s_GBSIGNALINFO* SigInfo);	// 1��λ��һ������Ļص�����

	// �޷ϻص�	
	static void WINAPI GlobalKickCallbackFunc(const s_KickParam* sKickParam);
	void KickCallbackFunc1(const s_KickParam* sKickParam);
	
	// ״̬�ص�
	static void WINAPI GlobalStatusCallbackFunc(const s_Status* sStatusParam);
	void StatusCallbackFunc1(const s_Status* sStatusParam);

	// ���߳����ڵȴ�����demo����ģ��
	static DWORD WaitDemoMould(LPVOID lpParam);
	DWORD WaitDemoMouldFunc();

	// ��ȡ�����߳�
	static DWORD GetIOCardImageNumThread(LPVOID lpParam);
	DWORD GetIOCardImageNumThreadFunc();


	// ��ȡĳ���̷�ʣ��ռ� ��λΪM
	double GetFreeSpaceOfHardDisk(CString strDisk);

	// ��ȡ������
	BOOL GetCount_Guangdian();
	
	// ��ȡ�޷ϼ���
	BOOL GetCount_Tifei();
	
	// ��ȡͼ��żĴ�������
	BOOL GetCount_TuXiangHao();
	
	// ������������ļ�
	BOOL LoadCamConfig(CString strModelName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void QuitPrecedure();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
//	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_iSaveSeqCnt{ 0 };
	int m_iSaveErrCnt{ 0 };
	int m_iSaveExistCnt{ 0 };
	CCriticalSection m_myCriticalSec;
	CStdioFile m_myFileTickResult;
	CStdioFile m_myFileCheckResult;
	int m_iLastFrameCnt{ 0 };
	int m_iDataElementCnt{ 0 };
	array<int, c_AllElementCount> m_iCheckTubeCnt{0};
	array<int, c_AllElementCount> m_iAlgExceptionCnt{ 0 };
	CString m_csLoadString1;
	afx_msg void OnDestroy();
	bool DeleteDirectory(char* sDirName);
	/************************************************************************/
	/*                [5/8/2017 ConanSteve]                     */
	int m_iLabelQueLen{ 0 };
	int m_iDisBtwn5And6{ 0 };
	int m_iSaveImgObj{ 0 };/** the objection of executing save images function       [6/15/2017 ConanSteve]**/
	int		m_iSaveImgNamingRegulation{ 0 };/** ��ͼ��������              [6/15/2017 ConanSteve]**/
	CCriticalSection m_csLabelRsltTable;
	array<int, c_AllElementCount> m_iExposureTime{ 0 };/** Exposure time of cameras              [6/15/2017 ConanSteve]**/
	typedef struct s_5CheckInfo {
		bool bKick{false};
		int iCheckRslt{0};
		int iImgSN5{0};
	}s_5CheckInfo;
	queue<int> m_iWaitRsltFrame;//Ϊ�˷�ֹ1�����룬����ɼ���2��ͼ��
	queue<s_5CheckInfo> m_sLabelRst;//**[9/27/2017 ConanSteve]**:  ��ǩ�ļ����
	array<s_5CheckInfo, 256> m_arrKickTable2;
	int m_iErrorCntOf5And6{0};//56��λ����ü���
	array<HANDLE, c_AllElementCount> m_hEventSGActive{ 0 };
	array<HANDLE, c_AllElementCount> m_hEventComplete{ 0 };
	int m_iSG_ElapseTimeToRead{ 0 };
	int m_iLastTime{ 0 };
	array<DWORD, c_AllElementCount> m_dwLastKickTime{ 0 };

	//boost::shared_array<boost::shared_array<char>> m_pImgDataTemp{ nullptr };
	array<shared_ptr<char>, CAMERA_COUNT> m_pImgDataTemp{ nullptr };
	CString m_sLanguage{TEXT("")};
	array<CExtenInfo, c_AllElementCount> m_arr_ExtenPara;
	int m_iSysDetectMode{ 0 };//**[7/21/2017 ConanSteve]**:  ϵͳ���ģʽ
	BOOL m_bOpenCameraAlgCheck[6]{TRUE};
	bool m_bShowObjStatusWnd{ false };//**[7/26/2017 ConanSteve]**:  �Ƿ���ʾ�����ʵʱ�����Ϣ
	int m_iSysDataEleLen{ 0 };
	int m_iObjDetectMode{ 0 };
	deque<int> m_queFrameCnt;
	deque<int> m_queTubeCnt;//*[7/12/2017 ConanSteve]*:  ��Ѫ���йܼ�������
	std::wstring m_wsResPath;
	bool m_bOpenXCWnd;//**[9/27/2017 ConanSteve]**:  �Ƿ��XC���
	int m_iPortOfAlarm{ 0 };//**[9/27/2017 ConanSteve]**:  �ӿڿ����ձ����źŵĶ˿�
	int  m_iPortOfStopMachine{ 0 };//**[9/27/2017 ConanSteve]**:  �ӿڿ��ӿ�ͣ���źŵĶ˿�
	int m_iPortOfNormalRunning{ 0 };
	int m_iMaxNumOfConstantKick{ 10 };
	int m_iOffsetBtw1And4{19};//1��λ��4��λ֮���֡����
	array<int, 6> m_arr_iCurNumOfConstantKick;//**[9/27/2017 ConanSteve]**:  ��ǰÿ���޷Ͽڵ����߸���
	CINICfg m_iniAlgConfig;
	int m_iLastCheckCntTotle{ 0 };//**[1/19/2018 ConanSteve]**:  �ϴμ�����ʱ�ļ������������
	int m_iLastGoodCntTotle{ 0 };//**[1/19/2018 ConanSteve]**:  �ϴ������������߶ϵ����ʱ�ĺ�Ʒ����

	array<int, 256> m_arr_ImgSNFormFive2Six;//�幤λ����ʱ��6��λͼ��ŵ�ӳ��

	//**[1/16/2018 ConanSteve]**:  ����Ȩ��
	CThreeTierRights m_threeTierRights;
	/************************************************************************/
	bool AlarmStopMachine(LPCTSTR sErrorInfo = NULL);
	int StopMachine();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__4B8DECDB_A7FA_4037_B1C9_9B8E1F287E47__INCLUDED_)
