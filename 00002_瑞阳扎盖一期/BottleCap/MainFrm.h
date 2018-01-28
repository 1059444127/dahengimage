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
#include "ObjAlloc.h"
#include "ParamDefine.h"
#include "PIO24BLib.h"
#include "Raw2Rgb.h"
#include "DHAlgInterface.h"
#include "MultiObjectDef.h"
#include "alg_struct.h"
#include "BottleCap.h"
#include "IoCard.h"

enum e_IOCardType {
	e_IOCardType_NONE,
	e_IOCardType_PIOB,
	e_IOCardType_PIOC,//**[9/11/2017 ConanSteve]**:  C��
	e_IOCardType_NewCard,
	e_IOCardType_RS232_XZXNYN
};

#define  SPOT1

#ifdef SPOT
#define __RealGrabber_							//���ʵʱ�ɼ�
#define   USE_IO_CARD							//ʹ�ýӿڿ�ʱ��
#else 
#define __SGGrabber_                                //ģ��ɼ�
#endif // SPOT





#define   PIC_COUNT						30			//ÿ��ƿ�����ո���
#define	  PE_COUNT						3			//�����źŸ���
#define	  IOCARD_COUNT					1			//IO������
#define	  ALLELEMENT_COUNT				3			//�������ĸ���
#define   CAMERA_COUNT                 3			//ϵͳʹ�õ��������    
#define   CHECK_THREAD_COUNT            8			//����̵߳ĸ���
#define   DATA_ELEMENT_COUNT			5			//�ص��ж��и���
#define   MAX_DEFECT_TYPE_COUNT		30			//ȱ�������ܸ���,ÿ����λ40��
#define   ID_START_CHECK_TIMER          102         //���ϸ������ݿ���Ϣ
#define   ID_UPDATE_DEFECT_INFO_TIMER	103				//���ϸ���ȱ��ͳ����Ϣ
#define	  ID_UPDATE_DEFECT_SHOW_INFO	104				//���ϸ���ȱ��ͳ����ʾ��Ϣ

#ifdef __RealGrabber_
// #include "..\\Inc\\DHGrabberJAI.h"
// #include "..\\Inc\\DHGrabberForAVT.h"
#include "..\\Inc\\DHGrabberForMER.h"
/*enum GBParamID
{
 		GBImageWidth = 0,		//ͼ���� (��λ:�ֽ�)
		GBImageHeight,			//ͼ��߶� (��λ:�ֽ�)
 		GBImagePixelSize,		//ͼ�����ش�С (��λ:�ֽ�)
 		GBImageBufferSize,		//ͼ�񻺳�����С (��λ:�ֽ�)
		GBGrabberTypeSN			//�ɼ������ͱ��
};*/
#else
#include "..\\Inc\\GrabberExportForSG.h"
#endif

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

	int m_iStopCountDiff[ALLELEMENT_COUNT];						// ��i����λֹͣ�����ж����� 
	int m_iInspectionObj[ALLELEMENT_COUNT];						// ��i����λ�ļ��������
	int m_iHardImageCount[ALLELEMENT_COUNT];					// ��i����λ��ͼ����ź�
	int m_iLastHardImageCount[ALLELEMENT_COUNT];				// ��i����λ�ϴ�ͼ����ź�
	int m_iLastImageCount[ALLELEMENT_COUNT];					// ��i����λ���ϴ�ͼ��Ĵ�������
	int	m_iImageCountOverFlowNumber[ALLELEMENT_COUNT];			// ��i·�������������� 
	int m_iSaveCameraImageCount[ALLELEMENT_COUNT];				// ��������ͼ��Ĵ����� ��������
	int m_iSaveCameraImageCount_Setting[ALLELEMENT_COUNT];		// �趨����Ҫ�����ͼ������
	int m_iSaveCircleCameraImageCount[ALLELEMENT_COUNT];		// ѭ������ͼ�������
	int m_iSaveImageType[ALLELEMENT_COUNT];						// ����ͼ������
	int	m_iIsCircle[ALLELEMENT_COUNT];							// �Ƿ�ѭ������ͼ��
	int m_iKickInterval[ALLELEMENT_COUNT];						// �����ļ��в������㷨�����೤ʱ�䵼���޷�
	int m_iGrabCountSingleCircle[ALLELEMENT_COUNT];				// һ�����ڲɼ���ͼ����---�����ķ���
	int m_iImageKickThread[ALLELEMENT_COUNT];					// ���޷��߳���ʹ�õ�ͼ���
	int m_iImageCheckThread[ALLELEMENT_COUNT];
	int m_iImageCountInPE[ALLELEMENT_COUNT];					// ��ȡͼ���ʱʹ�õ�ͼ����ֵ
	int m_iLastImageCountInPE[ALLELEMENT_COUNT];				// ��ȡͼ���ʱʹ�õ��ϴ�ͼ����ֵ
	int m_iFlowOverImageCountInPE[ALLELEMENT_COUNT];			// ��ȡ����ͼ���ʱʹ��ͼ����������
	int m_iHardKickCount[ALLELEMENT_COUNT];						// ��֡�߷ϼ���(��λi)
	int m_iLastKickCount[ALLELEMENT_COUNT];						// �޷�i����һ�μ���	
	int m_iKickFlowNumber[ALLELEMENT_COUNT];					// �޷�i���������
	int m_iLastImageID[ALLELEMENT_COUNT];						// ��i���޷��߳�����һ�ε�ͼ�����
	int m_iCheckResultTable[ALLELEMENT_COUNT][256];				// ��λi�ļ������¼��,�����޷��߳�
	int m_iGrabCircleCount[ALLELEMENT_COUNT];					// �ɼ��Ĵ��ż���
	int m_iCheckErrorCount[ALLELEMENT_COUNT];					// ��������
	int m_iCheckImageCount[ALLELEMENT_COUNT];					// ����ܸ���
	int m_iDefectTypePerWS[ALLELEMENT_COUNT];					// ÿ����λ��ȱ�����͸���
	int m_iDefectCount[ALLELEMENT_COUNT][MAX_DEFECT_TYPE_COUNT];// ÿ��ȱ������ͳ����ֵ
	int m_iLabelErrorCount[ALLELEMENT_COUNT];					// 1Ϊ��ǩλ�ô��������2Ϊ��ǩ�����������󱨾�����
	int m_iAbnormalImage[ALLELEMENT_COUNT];						// �����쳣ͼ������
	int m_iListImagePathCount[CAMERA_COUNT];					// ����m_ListImagePath������Ԫ�ظ���
	int m_iOriginSaveImageNum[CAMERA_COUNT];                    // ��¼����ͼ��ǰM���¸���λ����ͼ�����������ڿ�����ͼ���滻��ͼ��ʹ��
	int m_iPicCount[CAMERA_COUNT];								// ÿ��������вɼ�ͼ��
	int m_iCamCardNum[CAMERA_COUNT];							// ÿ�����ʹ�õĽӿڿ����
	int m_iCamOutPort[CAMERA_COUNT];							// ÿ�����ʹ�õ�OUT�ڱ��

	int m_iFrameCount[PE_COUNT];								// ����ص��ж�ȡ�Ĺ�����  [2012-2-10 by zhy] 
	int m_iLastFrameCount[PE_COUNT];							// ����ص��ж�ȡ�Ĺ����һ�ε�֡��	
	int m_iFrameCountOverFlowNumber[PE_COUNT];					// ����ص��ж�ȡ�Ĺ��֡��������Ĵ���

	int m_iRC_PE[PE_COUNT];										// ��¼���
	int m_iRC_ThreadBad[ALLELEMENT_COUNT];						// ����߳��и������ź�ʱ��¼
	int m_iRC_ThreadGood[ALLELEMENT_COUNT];						// ����߳��и������ź�ʱ��¼
	int m_iRC_Good[ALLELEMENT_COUNT];							// д���źż�¼
	int m_iRC_AddKickImage[ALLELEMENT_COUNT];					// ��¼����ͼ���
	int m_iRC_KickImage[ALLELEMENT_COUNT];						// ��¼�޷�ͼ���
	int m_iRC_ImageNum[ALLELEMENT_COUNT];						// ��¼�ظ���¼
	int m_iRC_Image[ALLELEMENT_COUNT];							// ��¼�������̵߳�ͼ���
	int m_iRC_BeforeD[ALLELEMENT_COUNT];						// ��¼�㷨����֮ǰ��ͼ���
	int m_iRC_Detect[ALLELEMENT_COUNT];							// ��¼�㷨�����ͼ��ż����ʱ��
	int m_iRC_Kick[ALLELEMENT_COUNT];							// ��¼�޷�ͼ���
	int m_iRC_Call[ALLELEMENT_COUNT];							// ��¼�ص���ͼ��ż��ص����
	int m_iRC_DetectTime[ALLELEMENT_COUNT];						// ��¼�㷨���ʱ��
	int m_iRC_RecordImage[ALLELEMENT_COUNT];					// ��¼�߳���ͼ��ű仯ʱ��ʱ��
	int m_iRC_SetEvent[ALLELEMENT_COUNT];						// �������߳�
	int m_iRC_WriteImage[ALLELEMENT_COUNT];						// ��ȡͼ������[2014-12-5 zhy]
	int m_iRC_StopMachine[ALLELEMENT_COUNT];					// ͣ������
	int m_iRC_RecordKick[ALLELEMENT_COUNT];						// ��¼��ȡͼ����е��޷���Ϣ
	int m_iCamSN[CAMERA_COUNT];									// ���ͼ�����
	int m_iPicCountPerCam[CAMERA_COUNT];						// ÿ������ĵ�����ͼ����

	int m_iUserType;												//0Ϊ���Ϸ��û� 1λ����Ա 2Ϊ����Ա 3Ϊ��ͨ�û�
	int m_iEmptyCount;											// ��ѹͷ����
	int m_iLicenseExpDate;										// License��������ʱ��
	int m_iCamTestMode;											// �������ģʽ 0Ϊ�����ɼ� 1Ϊ����ģʽ
	int m_iKickMode;											// �޷�ģʽ 0Ϊȫ�ߣ�1Ϊȫ���ߣ�2Ϊ�����ߣ�3Ϊ������
	int m_iKickOffset;											// �޷�ƫ����
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

	BOOL m_bFirstCheck;											// ���������߸�λ
	BOOL m_bAdvMode;											// �Ƿ��ڸ߼�����Աģʽ
	BOOL m_bReadModel;											// �ж϶�дģ���Ƿ�ɹ�
	BOOL m_bStartRecord;										// ���ƿ�ʼ��¼
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
	BOOL m_bUpdateCheckData;											//�Ƿ����ڸ�����������

	BOOL m_bIsKick[ALLELEMENT_COUNT];							// ��·�Ƿ��߷�
	BOOL m_bSaveCameraImage[ALLELEMENT_COUNT];					// �Ƿ񱣴�ͼ��
	BOOL m_bDisplayImage[ALLELEMENT_COUNT];						// ������λ�Ƿ���ʾͼ��
	BOOL m_bDisplayAlgImage[ALLELEMENT_COUNT];					// ������λ�Ƿ���ʾ�㷨ͼ��
	BOOL m_bIsCheck[ALLELEMENT_COUNT];							// �Ƿ���
	BOOL m_bIsSimulateIniExist[CAMERA_COUNT];					// ģ���ͼ�������ж�ģ��ɼ�ʱ�����ļ��Ƿ����
	BOOL m_bGrabFlip[CAMERA_COUNT];								// �Ƿ���з�ת
	BOOL m_bBottleCapResult[ALLELEMENT_COUNT][256];			// ��������ж���������ļ���� ����Ϊ�棬�޴�Ϊ��
	BOOL m_bCheckResultOffset[ALLELEMENT_COUNT][256];			// ��λ1�Ƿ��޷�ƫ�� 
	BOOL m_bLabelAlarm[ALLELEMENT_COUNT];						// ��ǩ��ⱨ��
	BOOL m_bAlarm_StopCount[ALLELEMENT_COUNT];					// ֹͣ��������
	BOOL m_bWriteImage[CAMERA_COUNT][256][PIC_COUNT];			// �Ƿ�д������

	CString m_sPassword;												//�û�����
	CString m_sUserName;												//�û�����
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
	CString m_sDemoIniPath;										// �㷨���ó���DEMO�������ļ�·��
	CString m_sDemoName;										// Demo����
	CString	m_sSimulateGrabPath[CAMERA_COUNT];					// ģ���ͼ·��
	CString m_sDefectInfo[ALLELEMENT_COUNT][MAX_DEFECT_TYPE_COUNT];//ÿ��ȱ�ݵ�����
	CString m_strErrorDesc[ALLELEMENT_COUNT][256];				//�㷨���صĴ�����Ϣ����,�����б��
	CString m_sSaveImagePath[ALLELEMENT_COUNT];					//����ͼ��·��
	CString m_sCamConfigPath[CAMERA_COUNT];					//��������ļ�·��
	CString m_sCamConfigName[CAMERA_COUNT];					//��������ļ�����


	CFile m_filePE[PE_COUNT];									// ��¼���
	CFile m_fileThreadBad[ALLELEMENT_COUNT];					// ����߳��и������ź�ʱ��¼
	CFile m_fileThreadGood[ALLELEMENT_COUNT];					// ����߳��и������ź�ʱ��¼
	CFile m_fileGood[ALLELEMENT_COUNT];							// д���źż�¼
	CFile m_fileAddKickImage[ALLELEMENT_COUNT];					// ��¼����ͼ���
	CFile m_fileKickImage[ALLELEMENT_COUNT];					// ��¼�޷�ͼ���
	CFile m_fileImageNum[ALLELEMENT_COUNT];						// ��¼�ظ���¼
	CFile m_fileImage[ALLELEMENT_COUNT];						// ��¼�������̵߳�ͼ���
	CFile m_fileBeforeD[ALLELEMENT_COUNT];						// ��¼�㷨����֮ǰ��ͼ���
	CFile m_fileDetect[ALLELEMENT_COUNT];						// ��¼�㷨�����ͼ��ż����ʱ��
	CFile m_fileKick[ALLELEMENT_COUNT];							// ��¼�޷�ͼ���
	CFile m_fileCall[ALLELEMENT_COUNT];							// ��¼�ص���ͼ��ż��ص����
	CFile m_fileDetectTime[ALLELEMENT_COUNT];					// ��¼�㷨���ʱ��
	CFile m_fileRecordImage[ALLELEMENT_COUNT];					// ��¼�߳���ͼ��ű仯ʱ��ʱ��
	CFile m_fileSetEvent[ALLELEMENT_COUNT];						// �������߳�
	CFile m_fileWriteImage[ALLELEMENT_COUNT];					// ��ȡͼ������[2014-12-5 zhy]
	CFile m_fileStopMachine[ALLELEMENT_COUNT];					// ͣ��������־
	CFile m_fileRecordKick[ALLELEMENT_COUNT];
	

	CEvent m_evtTrigDemoMould;									// ���ȴ�demo����ģ���߳��Ƿ�����[2014-11-4]
	CEvent m_evtGetIOCardImageNum;								// ��ȡ�����߳��Ƿ�����
	CEvent m_evtKillWait;

	double m_dFreq;												// ��¼�����ʱ��Ƶ��
	double m_dwCheckBadRatio;									// ��Ʒ��
	double m_dAlgCheckTime[ALLELEMENT_COUNT];					// �㷨���ʱ��

	//Bayer�仯��ر���
	BYTE m_pLutR[256];
	BYTE m_pLutG[256];
	BYTE m_pLutB[256];
	BYTE* m_pAlgElement[CAMERA_COUNT];							// ר��Ϊ�㷨�Ĳ������ú��޸�ģ���������ڴ�
	BYTE* m_pAlgErrorElement[CAMERA_COUNT];						// ר��Ϊ�㷨���һ�Ŵ���ͼ������ڴ�
	BYTE* m_pbRGBImage[CAMERA_COUNT];							// ��ͼ�����任ͼ�񻺳���
	BYTE* m_pImageAddr[ALLELEMENT_COUNT];						// ���ͼ������

	char m_chSavePath[ALLELEMENT_COUNT][512];

	CSpendTime m_SpendTime[ALLELEMENT_COUNT][256];				// ͳ���Ƿ�ʱʹ��

	LONGLONG m_prevqt1;											// ��¼���湤λ�ϴλص�������ȡ�ļ����ʱ����Ϣ
	LONGLONG m_lCameraTimeInfo[ALLELEMENT_COUNT];				// ��¼������λ�ϴλص�������ȡ�ļ����ʱ����Ϣ
	LONGLONG m_lLastQt_PE[PE_COUNT];							// ��¼����ϴμ���
	LONGLONG m_lLastQt_TX[PE_COUNT];							// ��¼ͼ����ϴμ���

	CList<CString,CString&> m_ListImagePath[CAMERA_COUNT];		// ͼ�񱣴�·��

	CCriticalSection m_csImagePath[CAMERA_COUNT];				// ����ͼ�񱣴�·������ֹ��������

	_ConnectionPtr m_Connect;									// �������ݿ�
	_RecordsetPtr m_RecordSet;									// ��¼��

	CLogManager  m_LogManager;									// ��־������
	CPIO24BLib m_IOCard[IOCARD_COUNT];			     			// PIO24B�ӿڿ�����
	CioControl m_NewIOCard;
	bool m_bReadIOCardParamFile{ true };
	e_IOCardType m_eIOCardType{ e_IOCardType::e_IOCardType_PIOC };
	CINICfg m_cfgAlgCfg;
	CBayerTransform m_BayerTransform[CAMERA_COUNT];			

	BITMAPINFO* m_pBitmapInfo[CAMERA_COUNT];				
	HANDLE m_hImageInspectStart[ALLELEMENT_COUNT];				// ��������¼��������
	e_ShowModeFlip m_eShowModeFlip[CAMERA_COUNT];				// ���з�ת����
	s_ElementInfo m_sElement[ALLELEMENT_COUNT][DATA_ELEMENT_COUNT];			// ��¼�ص���Ϣ
	s_ImageInformationStruct m_sCameraImageInfo[CAMERA_COUNT]; //����ɼ���ͼ����Ϣ

	e_SaveImgObj m_eSaveImageType[ALLELEMENT_COUNT];			//�Ƿ�ѭ����ͼ��0Ϊ��ѭ�� 1Ϊѭ����

	CObjAlloc m_InspectionObj;									// ������

	s_CheckCameraInfo m_sCamInfo[ALLELEMENT_COUNT];
//	CExtenInfo m_arrExten[ALLELEMENT_COUNT];
	array<CExtenInfo, ALLELEMENT_COUNT> m_arrExten;
	CMultiObjSaveImgPara m_arrObjSaveImgPara[ALLELEMENT_COUNT];
	CCriticalSection m_csIOCard;


	
#ifdef __SGGrabber_
	CDHGrabberSG  m_CameraGrabber[CAMERA_COUNT];				// ����ɼ�����
#else
//	CDHGrabberAVT  m_CameraGrabber[CAMERA_COUNT];				// ����ɼ�����
// 	CDHGrabberJAI m_CameraGrabber[CAMERA_COUNT];
	CDHGrabberMER m_CameraGrabber[CAMERA_COUNT];

#endif
	

// Operations
public:

	// ɾ���ļ���
	BOOL DeleteDirectory(char* sDirName);

	// ִ�б�������,bStartΪTRUE��ʼ������bStartΪFALSEֹͣ����
	BOOL AlarmAction(BOOL bStart);

	// ��ȡ��ǰʱ��
	CString GetCurTime();

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

	// ���¼�¼
	void UpdateCheckData();

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

	/*************************************<>*************************************/
public:
	int m_iSaveImgObj{ 0 };
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__4B8DECDB_A7FA_4037_B1C9_9B8E1F287E47__INCLUDED_)
