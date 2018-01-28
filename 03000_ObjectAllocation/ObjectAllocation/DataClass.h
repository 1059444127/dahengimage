// DataClass.h: interface for the CDataClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATACLASS_H__5ECB82A9_A4F3_49B6_BE83_9C3AE25DDC0C__INCLUDED_)
#define AFX_DATACLASS_H__5ECB82A9_A4F3_49B6_BE83_9C3AE25DDC0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ObjAlloc.h"
#include "DlgObjStatus.h"
#include "ObjStatusList.h"
#include "InspectClass.h"
#include "SpendTime.h"
#include <queue>
#include "DlgRunInfo.h"
#include "StdAfx.h"
#include "alg_struct.h"
#include "algorithm"
#include "numeric"
#include "set"
#include "ExFn.h"

#ifdef DEBUG
#define CAMERA_COUNT 6
#endif



using namespace std;

#define MAX_CHAR_LENGTH		1000
#define MAX_CAMERA_COUNT	50
#define  MAX_KICK_COUNT     50
#define MAX_IMAGE_COUNT		2000000000

const int c_iMaxFileNum{ 20 };

typedef CInspBase*(*pFunc_Create)(int);
typedef void(*pFunc_Destroy)(CInspBase*);
typedef int(*pFunc_GetWndNum)();

typedef struct _ThreadParam
{
	void* pThis;
	int iThreadID;
}s_ThreadParam;

const int c_iErrorInfoTableLen{ 1024 };
const int c_iMaxStringLen{ 1024 };


class CDataClass
{
public:
	CDataClass();
	virtual ~CDataClass();

	// ��ʼ�� ͨ��
	s_Status Init(s_CheckInitParam sInitParam);//�����ڳ�ʼ��+������

											   // �ͷ���Դ
	s_Status Free();

	// ��⺯�� DEMOʹ��
	//s_Status Check(int iIndex, int iThreadID, s_CheckInParam sCheckInParam, s_CheckOutParam* sCheckOutParam);

	// ��������
	s_Status ShowDetail(int iCheckSN);

	s_Status Set(s_AlgModelInput sAlgModelInput);//������

	void DisplayObjectsStatus(int nCmdShow);

	// ��⺯�� ����ʹ��
	s_Status Detect(s_CheckInParam sCheckInParam);



protected:
	CString m_strAppDir{ TEXT("") }, m_strCfgFilePath{ TEXT("") };
	CString m_strAlgconfigDir{ TEXT("") };			//AlgConfig.ini ·��
	CString m_strVitrualDisk{ TEXT("") };			//����Ӳ������
	CStringArray m_strArrDllPath;		//��Ŷ�̬�����Ƶľ���

										//�����	 
	long*	m_plObjBusyTimes{ NULL };						//��¼ÿ·������ȫæ�Ĵ���
	int*	m_pnObjectNumber{ NULL };						//ÿһ·�ļ��������
	unique_ptr<bool[]>    m_pbIsDrawing{ nullptr };							//ÿ·�Ƿ��ڻ�ͼ

	int		m_iCallType{ 0 };								//�������� 0Ϊ����ʵʱ����	1ΪDEMO����		2Ϊ����ģ�����
	int     m_iCheckGroupNum{ 0 };					//�㷨���·�� ���������

	int		m_iMachineType{ 0 };						//����Ϊ0 ҩҺΪ1

	BOOL	m_bShowStatus{ 0 };		//**[7/26/2017 ConanSteve]**:  �Ƿ���¶�����ⴰ����Ϣ
	bool	m_bKillThread{ false };							//ɱ������߳�

	int		m_iCameraNum{ 0 };							//�������
	int		m_iKickQueueNum{ 0 };						//�޷Ͽڸ���
	int		m_iCheckThreadNum{ 0 };						//����̸߳���

	int		m_iSaveImageNum[MAX_CAMERA_COUNT]{ 0 };		//����ͼ����� ÿ�ο�ʼ��ͼ��ʼΪ1;
	int     m_iProductNum[MAX_CAMERA_COUNT]{ 0 };			//���Ҳ�Ʒ����

	int		m_iTimeType[MAX_CAMERA_COUNT]{ 0 };			//ÿ���޷϶��еĶ�ʱ��׼ 0Ϊʱ����� 1Ϊ֡
	int		m_iTimeOut[MAX_CAMERA_COUNT]{ 0 };			//��ʱʱ��
	int		m_iTimeOutIndex[MAX_CAMERA_COUNT]{ 0 };	//ÿ���޷϶����жϳ�ʱ�õ������

	int		m_iImageWidth[MAX_CAMERA_COUNT]{ 0 };		//ÿ���ͼ����
	int		m_iImageHeight[MAX_CAMERA_COUNT]{ 0 };		//ÿ���ͼ��߶�
	int		m_iImageByte[MAX_CAMERA_COUNT]{ 0 };			//ÿ���ͼ���ֽ�
	array<int, MAX_CAMERA_COUNT> m_iCamSmallImgCnt{ 0 };//*[6/30/2017 ConanSteve]*:ÿ�������ǰͼ���ͼ�����
	array<int, MAX_CAMERA_COUNT>		m_iImageSize;			//ÿ���ͼ���С

	int		m_iCamCircle[MAX_CAMERA_COUNT]{ 0 };			//ÿ��������ͼ�����
	int		m_iCamGroup[MAX_CAMERA_COUNT]{ 0 };			//ÿ��������

	array<int, MAX_CAMERA_COUNT> m_arr_iCamLastImgSN{ 0 };
	array<int, MAX_CAMERA_COUNT> m_arr_iCamSmallImgNum{ 0 };//*[6/30/2017 ConanSteve]*:ÿ���������ͼ������С��ͼ����
	int		m_iCamKick[MAX_CAMERA_COUNT]{ 0 };			//ÿ������ڵ��޷϶��б��
	int		m_iCamKickOffsetX[MAX_CAMERA_COUNT]{ 0 };	//ÿ����е�һ��������޷�ƫ����X(�ֽ�)
	int		m_iCamKickOffsetY[MAX_CAMERA_COUNT]{ 0 };	//ÿ����е�һ��������޷�ƫ����Y(�ֽ�)
	int		m_iCamKickOffsetX_Written[MAX_CAMERA_COUNT]{ 0 };	//ÿ����е�һ��������޷�ƫ����X(ͳ���Ƿ�д����),��ǽ���Ƿ�д��


	int		m_iCheckOutNum[MAX_CAMERA_COUNT]{ 0 };		//ÿ������㷨��ⷵ�صĽ������
	int		m_iCheckOutSize[MAX_CAMERA_COUNT]{ 0 };		//ÿ�������ṹ��void* pParam��С����λΪ�ֽڣ�
	int		m_iCheckInSize[MAX_CAMERA_COUNT]{ 0 };		//ÿ�������ṹ��void* pParam��С
	int     m_iCheckStatusSize[MAX_CAMERA_COUNT]{ 0 };	//ÿ������״̬�ṹ��void* pParam��С

	int		m_iGroupCheckInSize[MAX_CAMERA_COUNT]{ 0 };		//ÿ������ṹ��void* pParam��С��
	int		m_iGroupCheckOutSize[MAX_CAMERA_COUNT]{ 0 };		//ÿ������ṹ��void* pParam��С��
	int		m_iGroupCheckStatusSize[MAX_CAMERA_COUNT]{ 0 };	//ÿ�鷵��״̬�ṹ��void* pParam��С��
	int		m_iGroupCheckOutNum[MAX_CAMERA_COUNT]{ 0 };		//ÿ������ṹ���н������
	int		m_iGroupCheckOutPerResultSize[MAX_CAMERA_COUNT]{ 0 }; //ÿ������ṹ����ÿ�������С����λΪ�ֽڣ�

	array<UINT, MAX_KICK_COUNT>	m_iKickErrorCount;	//ÿ���޷϶��л�Ʒ����
	int		m_iKickResultNum[MAX_CAMERA_COUNT]{ 0 };		//ÿ���޷϶�����ÿ��Ԫ�ذ������������
	int		m_iKickElementNum[MAX_CAMERA_COUNT]{ 0 };	//ÿ���޷϶������������
	int		m_iKickQueueSize[MAX_CAMERA_COUNT]{ 0 };		//ÿ���޷϶�����Ԫ�ش�С����λΪ�ֽڣ�
	int		m_iKickFirstCam[MAX_CAMERA_COUNT]{ 0 };		//ÿ���޷϶����е�һ��������
	int		m_iKickCircleSN[MAX_CAMERA_COUNT]{ 0 };		//�޷��߳���ʹ�õļ������
	int		m_iKickImageSN[MAX_CAMERA_COUNT][256];  //ÿ���޷϶��е�ͼ���
	BYTE*	m_pKickResult[MAX_CAMERA_COUNT][256]{ NULL };	//����޷ϱ���Ϣ
	BYTE*	m_pbKickWrite[MAX_CAMERA_COUNT][256]{ NULL };	//��¼ÿ���޷ϱ���Ƿ�д��

	int		m_iGroupErrorCount[MAX_CAMERA_COUNT]{ 0 };			//ÿ�����д������
	int		m_iGroupImageSize[MAX_CAMERA_COUNT]{ 0 };			//ÿ���������������ͼ���С�ܺ�
	int		m_iGroupElementNum[MAX_CAMERA_COUNT]{ 0 };			//ÿ�������������
	int		m_iGroupFirstCam[MAX_CAMERA_COUNT]{ 0 };				//ÿ�����е�һ������ı�ţ����ڻ�õ�ǰ��ʹ�õ��޷϶��кż��޷�ƫ����,��0Ϊ��ʼ
	int		m_iGroupLastImageSN[MAX_CAMERA_COUNT]{ 0 };			//ÿ������һ��ͼ���
	int		m_iGroupCircle[MAX_CAMERA_COUNT]{ 0 };				//ÿ�鴦���ͼ�����
	int		m_iGroupCallCount[MAX_CAMERA_COUNT]{ 0 };			//ÿ�����ɼ��ص����ܸ���
	int	    m_iGroupCheckQueueNum[MAX_CAMERA_COUNT]{ 0 };		//ÿ���������Ԫ�ظ���
	int		m_iGroupAbnormalNum[MAX_CAMERA_COUNT]{ 0 };			//ÿ����Ϊ���æ�����ĸ���
	int		m_iGroupLeftObj[MAX_CAMERA_COUNT]{ 0 };				//���ж������
	int		m_iGroupLeftList[MAX_CAMERA_COUNT]{ 0 };				//���ж��и���
	int		m_iGroupAlgCheck[MAX_CAMERA_COUNT]{ 0 };				//�㷨������

	int     m_iGroupIsSaving[MAX_CAMERA_COUNT]{ 0 };				//ÿ���Ƿ����ڱ���ͼ�� 0Ϊδ������ 1Ϊ������
	bool	m_bGroupSaveImage[MAX_CAMERA_COUNT]{ 0 };			//ÿ���Ƿ񱣴�ͼ��	0Ϊ������ 1Ϊ����
	int		m_iGroupSaveImageType[MAX_CAMERA_COUNT]{ 0 };		//ÿ�鱣��ͼ�������
	int		m_iGroupSaveImageCount[MAX_CAMERA_COUNT]{ 0 };		//ÿ�����ڱ���ͼ�������
	int		m_iGroupSaveImageCount_SET[MAX_CAMERA_COUNT]{ 0 };	//ÿ�鱣��ͼ����趨ֵ


	int		m_iIndexInGroup[MAX_CAMERA_COUNT]{ 0 };		//ÿ����������е����
	int		m_iIndexInKick[MAX_CAMERA_COUNT]{ 0 };		//ÿ������ĵ�һ��������޷϶����е����

	int		m_iPeriodImageSN[MAX_CAMERA_COUNT][256]{ 0 };//ÿ�����ڵ�ͼ���
	int		m_iPeriodIndex[MAX_CAMERA_COUNT][256]{ 0 };	//����Ԫ������

	int		m_iMaxLogLineNum{ 0 };								//��־��¼����
	int		m_iCallInterval{ 0 };						//����ص�֮������Ļص����
	int		m_iPeriodInterval{ 0 };						//��Ʒ���ڼ��������Сʱ����
	int		m_iSaveImageStatus{ 0 };						//ֹͣ����ͼ�� 0:��ʼ״̬ 1:����ͼ�� 2:���߳��Ѿ�ֹͣ����ͼ�����Ѹ���ϵͳ

	int		m_iIdleJob[MAX_CAMERA_COUNT]{ 0 };			//��·�����󣨵�ַ��
	array<array<int, MAX_CAMERA_COUNT>, 20>	m_nObjIndex;			//ÿ·��������״̬ 1Ϊ����� 0Ϊ����

	int*	m_pWriteDataStatus[MAX_CAMERA_COUNT]{ NULL };	//����̶߳���ÿ��Ԫ�ص�״̬  0Ϊ���� 1Ϊ����д��ͼ������ 2Ϊд���������
	int*	m_pDetectStatus[MAX_CAMERA_COUNT]{ NULL };		//����̶߳���ÿ��Ԫ�ص�״̬ 1Ϊ���ڼ���� 0Ϊδ���
	int*	m_pCircleSN[MAX_CAMERA_COUNT]{ NULL };			//����̶߳���ÿ��Ԫ�صı�� ���С���ȱ�����
	BYTE*   m_pImageData[MAX_CAMERA_COUNT]{ NULL };			//���ͼ������[2015-10-30 zhy]
	BYTE*	m_pCheckInParam[MAX_CAMERA_COUNT]{ NULL };		//��ż�������е�*pParam��Ϣ
	BYTE*	m_pCheckOutParam[MAX_CAMERA_COUNT]{ NULL };		//��ż������е�*pParam��Ϣ
															//BYTE*	m_pCheckStatusParam[MAX_CAMERA_COUNT]{ NULL };	//��ż�����е�*pParam��Ϣ
	array<BYTE*, MAX_CAMERA_COUNT> m_pCheckStatusParam;

	void*	m_pCallThis{ NULL };							//�ص����������Ķ���ָ��

	double  m_dFreq{ 0 };								//����ʱ��ʹ��

	bool	m_bWriteLog{ false };							//�Ƿ�д��־
	BOOL	m_bCamComplete[MAX_CAMERA_COUNT][256];	//ÿһ·�����ͼ�Ƿ���

	HANDLE m_hImageInspectStart[MAX_CAMERA_COUNT]{ 0 };				//��������¼��������

	CSpendTime m_spTime[MAX_CAMERA_COUNT][256];					//ͳ�Ƴ�ʱ

	LONGLONG m_lCamTime[MAX_CAMERA_COUNT][256];					//��¼�ɼ�����ʱ�䣬����������ͼ���ʱ���
	LONGLONG m_lPeriodTime[MAX_CAMERA_COUNT][256];				//��¼ÿ������Ĳ�ͼ����ʱ�䣬�����ж�������

	s_ThreadParam m_sThreadParam[MAX_CAMERA_COUNT]{ NULL };				//����̲߳������ͼ�����
	s_CheckInParam* m_spCheckInParam[MAX_CAMERA_COUNT]{ NULL };			//��ż���̲߳���
	s_CheckOutParam* m_spCheckOutParam[MAX_CAMERA_COUNT]{ NULL };		//��ż����
																		/*****************************************************************************************************************************************************************************************/
	s_CheckInParam** m_sppCheckInParamEx{ NULL };//**[7/24/2017 ConanSteve]**:  ��ż���̲߳������ͼ������
	s_CheckOutParam** m_sppCheckOutParamEx{ NULL };
	s_Status** m_sppCheckStatusParamEx{ NULL };			//**[7/24/2017 ConanSteve]**:  ��ż����,�ͼ������
	BYTE**	m_ppCheckInParamEx{ NULL };		//**[7/24/2017 ConanSteve]**:  ��ż�������е�*pParam��Ϣ,�ͼ������
	BYTE**	m_ppCheckOutParamEx{ NULL };		//**[7/24/2017 ConanSteve]**:  ��ż������е�*pParam��Ϣ,�ͼ������
	BYTE**	m_ppCheckStatusParamEx{ NULL };	//**[7/24/2017 ConanSteve]**:  ��ż�����е�*pParam��Ϣ,�ͼ������
	int** m_ppCacheUseCnt{ NULL };	//**[8/1/2017 ConanSteve]**:  ������ʹ�ü���
	array<array<int, 50>, MAX_CAMERA_COUNT> m_ppObjUseCnt{ NULL };	//**[8/1/2017 ConanSteve]**:  ������ʹ�ü���
																	/*****************************************************************************************************************************************************************************************/

//	array<array<COptionalFunc,20>, 20> m_ppCopyWindowParaCacheQueue;//**[10/9/2017 ConanSteve]**:  ͼ�񻺴���ж�Ӧ����Ϣ
//	array<array<int, 20>, 20> m_arr_iTimeOfWaitInCacheQueue;
	array<array<LONGLONG, 20>, 20> m_arr_ldTickOfPushInCacheQueue;
//	array<array<COptionalFunc, 20>, 20> m_ppCopyWindowParaAlgObjQueue;//**[10/9/2017 ConanSteve]**:  �㷨��������ж�Ӧ����Ϣ
	array<array<int, 20>, 20> m_arr_iTimeOfWaitInAlgObjQueue;
	array<array<LONGLONG, 20>, 20> m_arr_ldTickOfPushInAlgObjQueue;
	
	s_Status* m_spCheckStatusParam[MAX_CAMERA_COUNT]{ NULL };			//��ż����

	CCriticalSection m_csTime;									//��ȡʵ��
	CCriticalSection m_csGlobalCheck;							//���ʱ��ѯ
	CCriticalSection m_csWriteDataStatus[MAX_CAMERA_COUNT];		//����д���ݻ���
	CCriticalSection m_csCamComplete[MAX_CAMERA_COUNT];			//�����ɼ�����
	CCriticalSection m_csCameraDiff[MAX_CAMERA_COUNT];			//����������ʱ�以��
	CCriticalSection m_csGroupDiff[MAX_CAMERA_COUNT];			//������ʱ�以��
	CCriticalSection m_csAlgCheck[MAX_CAMERA_COUNT];			//�����㷨���
	CCriticalSection m_csAlgDisp[MAX_CAMERA_COUNT];				//�����㷨��ʾ
	CCriticalSection m_csAlgSave[MAX_CAMERA_COUNT];				//�����㷨����
	CCriticalSection m_csSaveImage[MAX_CAMERA_COUNT];			//����ͼ��
	CCriticalSection m_csKickResult[MAX_CAMERA_COUNT][256];		//�����޷Ͻ��

	array<array<CCriticalSection, 256>, MAX_KICK_COUNT> m_csBoolKickWrite;

	CEvent m_evtKillKickThread;									//�ر��޷��߳�
	CEvent m_evtKillInspThread[50];								//�رռ���߳�



	array<array<CLogFile, c_iMaxFileNum>, MAX_CAMERA_COUNT> m_fileCam;
	array<array<CLogFile, c_iMaxFileNum>, MAX_CAMERA_COUNT> m_fileDetect;
	array<array<CLogFile, c_iMaxFileNum>, MAX_CAMERA_COUNT> m_fileCheck;
	array<array<CLogFile, c_iMaxFileNum>, MAX_CAMERA_COUNT> m_fileKick;
	CLogFile m_fileRunInfoStatus;
	CLogFile m_fileDebugInfo;
	CCriticalSection m_csRunInfoStatus;
	array<UINT, MAX_CAMERA_COUNT> m_iGroupBadCnt;//**[9/25/2017 ConanSteve]**:  ÿ��λ��Ʒ��
	array<UINT, MAX_CAMERA_COUNT> m_iGroupGoodCnt;//**[9/25/2017 ConanSteve]**:  ÿ��λ��Ʒ��
	array<UINT, MAX_KICK_COUNT> m_iKickGoodCnt;//**[9/25/2017 ConanSteve]**:  ÿ���޷ϻص�ȫ0�ļ���


	shared_ptr<CDlgObjStatus> m_pObjStatusDlg{ NULL };								//��ʾ����״̬

	PKICKCALLBACK m_CheckCallback;								//�޷ϻص�
	PSTATUSCALLBACK m_StatusCallback;							//�м�״̬�ص�

																// ��ʼ��������
	BOOL InitObjects(CString strInitFile, int iCallType);
	void GetLogDirPath(CString&);

	// ����̺߳���
	static DWORD ImageInspectionThread(LPVOID lpParam);
	DWORD ImageInspectionThreadFunc(int iThreadID);
	/*	DWORD InspectionContext(int iWorkStationSN, int iThreadID);*/

	// �޷��߳�
	static DWORD KickThread(LPVOID lpParam);
	/*	DWORD KickThreadFunc();*/

	UINT ThreadFunc_ChangeStatus(LPVOID);

	// �ж��Ƿ���������
	BOOL JudgeNewCircle(int iGroupSN, int iCameraSN, int iImageSN, int iCircleSN);

	// д�޷ϱ�
	/*	BOOL ThreadWriteCheckResult(int iGroupSN, int iIndex);*/

	// д�������ʼ��д��ʼ���������ʱд�������
	//**[7/26/2017 ConanSteve]**:  bInit==true .��ʾ��ͼд��ͼ�Ľ��, false��ʾ�������Ľ��
	BOOL ThreadWriteResult(int iImageSN, int iGroupSN, int iKickSN, bool bInit);


	// ����ͼ��
	/*	BOOL ThreadSaveImage(int iGroup, int iIndex);*/

	// ������־�ļ�
	BOOL CreateLogFile();
	BOOL FlushLogFile();//**[8/17/2017 ConanSteve]**:  �����־�ļ�������
						//void WriteLogInfo(e_LogType eLogType, int iGroup, int iIndex, CString& sInfo);//**[8/23/2017 ConanSteve]**:  д����־��Ϣ

						// �������Ŀ¼
	BOOL CreateMultiLevelPath(const CString &strPath);

	// �ж�Ŀ¼�Ƿ����
	/*	BOOL JudgePathExist(const CString& strPath);*/

	// ����ͼ��
	int SaveBmp(const TCHAR* pPathName, int iWidth, int iHeight, int iByte, TBYTE *pDest);






	// ��ʾ״̬��Ϣ
	int ShowCurrStatus();

	// �ر������߳�
	int KillAllThread();

	// �ͷ��߳�ʹ���ڴ�
	int ReleaseThreadData();
	/************************************************************************/
	/*                       [6/6/2017 ConanSteve]                          */

private:
	//**[7/24/2017 ConanSteve]**:  �������ͼ��⺯��
	s_Status DetectEx(s_CheckInParam sCheckInParam);
	//**[7/24/2017 ConanSteve]**:  2��Detect() ��⺯��
	DWORD InspectionContextEx2(int iGroupSN, int iThreadID);
	//	s_Status InitEx2(s_CheckInitParam sInitParam);
	void CheckEx2(int iIndex, int iThreadID, s_CheckInParam sCheckInParam);
	BOOL ThreadWriteCheckResultEx2(int iGroupSN, s_CheckInParam* spCheckInParam, s_CheckOutParam* spCheckOutParam);
	DWORD KickThreadFuncEx2();
	bool CopyCheckData(int iIndexInCache, int iIndexOfObj, int iGroupSN);
	void UpdateRunInfo();

	typedef struct s_ImgInfoBeenSaved {
		int SN{ 0 };
		vector<CString> vec_sSaveImgPaths;
	}s_ImgInfoBeenSaved;
	typedef struct s_ImgInfoWaitForSave {
		int SN{ 0 };
		int iMinIndex{ 0 };
		s_CheckInParam sCheckInParam;
	}s_ImgInfoWaitForSave;
	queue<s_ImgInfoBeenSaved > m_queSaveImgHistory;/** ��ͼ·����ʷ              [6/6/2017 ConanSteve]**/
	queue<s_ImgInfoWaitForSave> m_queImgsWaitForSave;
	CCriticalSection m_srwlSaveImgHistory;
	CCriticalSection m_srwlImgsWaitForSave;
	int m_iSizeOfOneArrImgs{ 0 };/**   ����ͼ�Ĵ�С             [6/6/2017 ConanSteve]**/
	HANDLE m_hSaveImgs{ 0 };
	HANDLE m_hEventSaveImgs{ 0 };
	int m_iSaveImgObj{ 0 };
	int m_iSaveImgNamingRegulation{ 0 };
	int m_iProductID{ 0 };
	bool m_bIsCircle{ false };//**[7/14/2017 ConanSteve]**:  �Ƿ�ѭ����ͼ
	int FnSavebmp(const TCHAR *lpszPathName, int W, int H, int nByte, TBYTE *lpDest);
	DWORD static FnThreadSaveImg(PVOID pParam);
	DWORD FnThreadSaveImgFunc();
	/*	BOOL OP_Directory(LPCSTR lpsrcPath, LPCSTR lpdstPath, int type, int flag);*/
	ULONGLONG GetFreeSpaceOfHardDisk(LPCTSTR sDisk);
	//*[7/3/2017 ConanSteve]*:  ɾ���ļ�
	bool DeleteDirectory(TCHAR* sDirName);
	TCHAR* GetErrorInfo(LPCTSTR lpszErrorInfo = NULL);//**[8/2/2017 ConanSteve]**:  ʹ�ô�����Ϣ�����������淵�صĴ�����Ϣ
	array<CCriticalSection, MAX_CAMERA_COUNT> m_srwlImgQue;
	//*[7/3/2017 ConanSteve]*:  �����Ƿ���Ҫ����ͼ�������Ҫ�������ڱ�����ͼ��������ô�ͼ���б�־
	typedef struct s_ResetWriteStatus {
		bool bCheckComplete{ false };
		volatile bool bSaveComplete{ false };
		s_ResetWriteStatus()
		{

		}
		void FnSetVal(bool bCheckComplete, bool bSaveComplete) {
			this->bCheckComplete = bCheckComplete;
			this->bSaveComplete = bSaveComplete;
		}
		bool bFnIsComplete() {
			return bCheckComplete && bSaveComplete;
		}
	}s_ResetWriteStatus;
	array<int, MAX_CAMERA_COUNT> m_iSmallImgSize;
	int m_iObjDetectMode{ 0 };//*[7/14/2017 ConanSteve]*:  ���������㷨��Detectģʽ��1��Detect����2��Detect
	int m_iSysDetectMode{ 0 };//*[7/14/2017 ConanSteve]*:  ϵͳ���ö�����Detectģʽ
	array<shared_ptr<s_ResetWriteStatus>, 50> m_pResetWriteStatus{ nullptr };
	//*[7/3/2017 ConanSteve]*:  ������ʾϵͳͼ����
	array<BITMAPINFO*, MAX_CAMERA_COUNT> m_pBitmapInfo{ NULL };
	//*[7/3/2017 ConanSteve]*:  ��ʾϵͳͼ��ṹ
	array<s_DrawSysPic, MAX_CAMERA_COUNT> m_arrDrawSysPic;
	bool m_bShowObjRunInfoWnd{ false };//**[7/26/2017 ConanSteve]**:  �Ƿ���ʾ�����ʵʱ�����Ϣ
	array<array<char, c_iMaxStringLen>, c_iErrorInfoTableLen> m_arrErrorInfo;
	int m_iErrorInfoIndex{ 0 };
	CCriticalSection m_csErrorInfoTable;//**[7/31/2017 ConanSteve]**: ������Ϣѭ���б��ٽ���
										//shared_ptr<CDlgRunInfo> m_pDlgRunInfo;
	CDlgRunInfo* m_pDlgRunInfo{ NULL };//**[11/9/2017 ConanSteve]**:  �����������Ϣ����
	array<int, MAX_CAMERA_COUNT> m_iMaxOffsetOfKick{ 0 };//**[8/1/2017 ConanSteve]**:  ÿ���޷Ͽ����й�λ�����ƫ����
	array<int, MAX_CAMERA_COUNT> m_iKickRemediation{ 0 };//**[7/31/2017 ConanSteve]**:  ��ʱ���߼���
	array<int, MAX_CAMERA_COUNT> m_arr_iAlgExceptionCnt{ 0 };//**[8/1/2017 ConanSteve]**:  �㷨�쳣����
	array<int, MAX_CAMERA_COUNT> m_iKickOverTimeCnt{ 0 };//**[8/2/2017 ConanSteve]**:  �޷ϳ�ʱ����
	int m_iAlgExceptionVal{ 0 };//**[8/1/2017 ConanSteve]**:  �㷨�쳣ֵ
	int m_iDefaultCheckRslt{ -1 };//**[7/26/2017 ConanSteve]**:  �����Ĭ��ֵ
	int m_iLostImageRslt{ -4 };//**[12/27/2017 ConanSteve]**:  �����ͼĬ�ϼ����
	bool m_bEnableXCGUI{ false };//**[9/20/2017 ConanSteve]**:  �Ƿ�ʹ���Ų�GUI��̬��
	bool m_bEnableOvertimeCallback{ false };//**[9/20/2017 ConanSteve]**:  �Ƿ�򿪳�ʱ�ص�����
	CINICfg m_iniAlgCfg;//**[9/20/2017 ConanSteve]**:  algConfig�����ļ�
	bool m_bEnableExtenInfo{ false };//**[10/21/2017 ConanSteve]**:  �Ƿ�ʹ����չ��Ϣ�ṹ��
	bool m_bEnableInitObjectAndCheck{ true };//**[10/21/2017 ConanSteve]**:  �Ƿ�ִ�ж����ĳ�ʼ���ͼ��
	bool m_bEnableAlgInit{ true };//**[10/21/2017 ConanSteve]**:  �Ƿ�ִ���㷨��ʼ��
	int m_iSingleTest{ -1 };//**[11/9/2017 ConanSteve]**:  �Ƿ�·���ԣ�Ϊ-1ʱ�����м���鶼��⣬Ϊ��ʱ��ֻ��⵱ǰ·

	array<bool, MAX_CAMERA_COUNT> m_arr_bIsLoadModel{ false };//**[1/6/2018 ConanSteve]**:�Ƿ����ڻ��ߵȴ�����ģ��
	void InitTestInfo();//**[10/16/2017 ConanSteve]**:  �������ļ���ȡ������Ϣ


						/************************************************************************/


						// 	// Generated message map functions
						// 	//{{AFX_MSG(CTestExeDlg)
						// 	//}}AFX_MSG
						// 	LRESULT OnMsgCreateStDlg(WPARAM, LPARAM);
						// 	DECLARE_MESSAGE_MAP()
public:

	bool IsCheckCompleted(int iGroupSN, int iKickGroupSN, int iImgSN);//**[9/20/2017 ConanSteve]**: ������й�λ�Ƿ�����ɣ���������ɣ�ʹ���޷ϻص� 
	bool ReseMultiObject();
	bool Reset();
};

#endif // !defined(AFX_DATACLASS_H__5ECB82A9_A4F3_49B6_BE83_9C3AE25DDC0C__INCLUDED_)