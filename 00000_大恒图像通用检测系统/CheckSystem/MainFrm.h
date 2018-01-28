
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "CheckSystem.h"
#include "DlgParamSet.h"
#include "ImageHandle.h"
#include "ExtenInfo.h"
#include "..\\..\\IncPublic\\ThreeTierRights.h"
#include <map>
#include <queue>
#include "Raw2Rgb.h"
#pragma comment(lib, "..\\Lib\\raw2rgb.lib")
//**[9/11/2017 ConanSteve]**:  ���ͷ�ļ�
#include "DHGrabForMER.h"
#include "DHGrabForSG.h"
//#include "DHGrabForAVT.h"
//#include "DHGrabForJAI.h"
//#include "DHGrabForBaumer.h"

//**[10/30/2017 ConanSteve]**:  MultiObejctͷ�ļ�
#include "ObjAlloc.h"
#include "MultiObjectDef.h"

//**[9/11/2017 ConanSteve]**:  �ӿڿ�ͷ�ļ�
#include "DHIOCardForSG.h"
#include "DHIOCardForPCI6356.h"
#include "DHIOCardForRS232_XZXNYN.h"
#include "DHIOCardForPIO24C.h"


//**[11/24/2017 ConanSteve]**:  �㷨�ӿ�

#include "alg_struct_general.h"
#include "alg_struct_stopper.h"
#include "alg_struct_lampInsp.h"
#include "alg_struct_Label.h"//**[12/12/2017 ConanSteve]**:  ��Ѫ�ܽṹ�壬����ʹ��


/*************************************<ϵͳ�ṹ��>************************************[11/16/2017 ConanSteve]*/
//**[11/16/2017 ConanSteve]**:  ������Ϣ����
class CRetInfo{
public:
	bool bRet{ false };
	CString sErrorInfo{_T("")};
};

typedef struct s_DefectInfo {
public:
	int iCnt;//**[12/1/2017 ConanSteve]**:  ȱ�ݼ���ͳ��
	CString sDesc{ _T("") };//**[12/1/2017 ConanSteve]**:  ȱ������
	s_DefectInfo(int iCnt, LPCTSTR lpszDesc)
	{
		this->iCnt = iCnt;
		this->sDesc = lpszDesc;
	}
};


/*************************************<>************************************[11/16/2017 ConanSteve]*/

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar        m_wndStatusBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CWnd* pParentWnd = NULL, LPCTSTR lpszMenuName = NULL, DWORD dwExStyle = 0, CCreateContext* pContext = NULL);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);





/************************************************************************/
private://**[8/24/2017 ConanSteve]**:  ˽�в���
	int m_cxScreenSize{ 0 }, m_cyScreenSize{ 0 };
	//**[11/15/2017 ConanSteve]**:  ��Ʒ������
	e_ProductLine m_eProductLine{ e_ProductLine_NO };
	/*************************************<>************************************[9/18/2017 ConanSteve]*/
	bool bExit{ false };//**[12/13/2017 ConanSteve]**:  �Ƿ��˳�ϵͳ
public://**[8/24/2017 ConanSteve]**:  ���в���

	//**[1/27/2018 ConanSteve]**:	�Ƿ���Ҫ���ܹ���֤�� ��ֵֻ��ϵͳ������Ա����Ȩ���޸�
	bool m_bEnableVerification{ true };
	//**[11/23/2017 ConanSteve]**:  ��ͼ��ʽ��0�����㷨��ͼ��1��ϵͳ��ͼ
	int m_iSaveImgObj{e_SaveImgObj_Alg};
	//**[9/13/2017 ConanSteve]**:	����ͼ�����ͣ�0�ǲ���ͼ����ֵ�������ļ�����
	int m_iSaveImgType{ e_SaveImgType_NO };
	//**[8/24/2017 ConanSteve]**:  camera number
	int m_iGrabberNum{ 0 };
	//**[10/30/2017 ConanSteve]**:  �����������λ��
	int m_iCheckGroupNum{ 0 };
	//**[9/4/2017 ConanSteve]**:  ϵͳ�Ƿ��ʼ���ɹ�
	bool m_bInitSucceed{ false };
	//**[9/4/2017 ConanSteve]**:  ϵͳ�Ƿ��ʼ�����
	bool m_bInitCompleted{ false };
	//**[9/5/2017 ConanSteve]**:  �Ƿ����ڼ��
	bool m_bIsDetecting{ false };
	//**[9/5/2017 ConanSteve]**:  �Ƿ���ϵͳ��ͼ����(��ԭͼ)
	bool m_bEnableDrawImg{ false };
	//**[9/6/2017 ConanSteve]**:  �Ƿ��ʼ�����
	bool m_bEnableInitGrabber{ false };
	//**[9/6/2017 ConanSteve]**:  �Ƿ��ʼ���ӿڿ�
	bool m_bEnableInitIOCard{ false };
	//**[12/14/2017 ConanSteve]**:  �Ƿ��ʼ���ӿڿ��ɹ�
	bool m_bInitIOCardSuccess{ false };
	//**[10/30/2017 ConanSteve]**:  �Ƿ��ʼ�������
	bool m_bEnableInitMultiObjectAndCheck{ false };
	//**[9/13/2017 ConanSteve]**:  �Ƿ�ѭ����ͼ
	bool m_bEnableCircleSaveImg{ true };
	//**[1/23/2018 ConanSteve]**:	�Ƿ��Ʒ�߷�
	bool m_bEnableEmptyKick{ true };
	//**[11/23/2017 ConanSteve]**:  ϵͳ����ͼ����
	int m_iMaxNumberOfSaveImg{ 100 };
	//**[1/17/2018 ConanSteve]**:  �㷨��ͼ����
	General::e_DrawImgType m_iDrawImgType;
	//**[1/15/2018 ConanSteve]**:  ��·����
//	int m_iSingleTest = -1;
	//**[8/24/2017 ConanSteve]**:  ģ��·��
	CString m_sPathOfApp{ _T("") };
	//**[8/28/2017 ConanSteve]**:  AlgConfig.ini�ļ�����·��
	CString m_sPathFileOfAlgCfg{ _T("") };
	//**[8/24/2017 ConanSteve]**:  ����Ӳ��·��
	CString m_sPathOfVirtualDisk{ _T("") };
	//**[9/6/2017 ConanSteve]**:  ϵͳ�����ļ�·������ModelInfo�ļ����У�
	CString m_sPathFileOfCheckSysCfgInModelInfo{ _T("") };
	//**[8/25/2017 ConanSteve]**:  �㷨ģ���ļ���·��,������ǰģ������
	CString m_sPathOfAlgModelInfo{ _T("") };
	//**[8/25/2017 ConanSteve]**:  ��������
	CString m_sWindowTitle{ _T("���ͼ��ͨ�ü��ϵͳ") };
	//**[8/25/2017 ConanSteve]**:  ��ǰģ������
	CString m_sModelName{_T("")};
	//**[8/25/2017 ConanSteve]**:  �����ļ���·��
	CString m_sPathOfCfg{ _T("") };
	//**[9/1/2017 ConanSteve]**:  ϵͳ��־�ļ���·��
	CString m_sPathOfSysLogs{_T("")};
	//**[9/6/2017 ConanSteve]**:  ������Ϣ�ļ�·��
	CString m_sPathFileOfDebugInfo{ _T("") };

/*************************************<������>************************************[9/5/2017 ConanSteve]*/
//**[8/28/2017 ConanSteve]**:  ��������Ͳ�ȫ��һ��ʱ�����ô�ֵΪ666
	e_GrabType m_iUnitedGrabType{ e_GrabType::e_GrabType_SG };
	//**[9/8/2017 ConanSteve]**:  �������ô���
	shared_ptr<CDlgParamSet> m_pdlgParamSet{nullptr};

	
	//**[11/23/2017 ConanSteve]**:  ÿ��λ�Ѿ���ͼ������
	unique_ptr<int[]> m_p_iCurNumOfSaveImgOfEachGrab;
	//**[11/24/2017 ConanSteve]**:  ��ǰͼ��ŵ�С��ͼ����
	unique_ptr<int[]> m_p_iCurNumOfSmallImgOfEachImgSN;
	unique_ptr<LampInsp::s_CheckCameraInfo []> m_p_CamInfo_LampInsp;//**[11/24/2017 ConanSteve]**:  �Ƽ���㷨ͼ��ṹ��
	
	array<BYTE, 256> m_pLutR, m_pLutG, m_pLutB;
	
/*************************************<������>************************************[9/13/2017 ConanSteve]*/
	using VecInt = vector<int>;
	/*************************************<>************************************[9/15/2017 ConanSteve]*/
	CXMLCfg m_xmlAlgConfig;
	CINICfg m_iniAlgConfig;
	CINICfg m_iniCheckSystemConfig;
	
	CObjAlloc m_multiObject;//**[10/30/2017 ConanSteve]**:  �����
	CThreeTierRights m_threeTierRights;
	/*************************************<�㷨����>************************************[12/14/2017 ConanSteve]*/
	int m_iAlgDisplayMode{0};//**[12/14/2017 ConanSteve]**:  �㷨��ʾͼ�����ͣ�Ĭ��0��ʾ���У�1����ʾ��2��ʾ��Ʒ��3��ʾ��Ʒ��4��ʾ�в�Ʒ��5��ʾ�޲�Ʒ�������㷨�ɿɸ���ParaGradList���ж���
	/*************************************<��չ��Ϣ�ӿ�>************************************[11/22/2017 ConanSteve]*/


	/*************************************<�ӿڿ���>************************************[12/28/2017 ConanSteve]*/
	int m_iIOCardNum{ 0 };//**[9/11/2017 ConanSteve]**:  �ӿڿ�����
	array<CIOCardInfo, MAX_IOCARD_NUM> m_arr_IOCardInfo;//**[12/8/2017 ConanSteve]**:  �ӿڿ���Ϣ
	array<shared_ptr<CDHIOCard>, MAX_IOCARD_NUM> m_arr_pIOCard;
	array<CString, MAX_IOCARD_NUM> m_arr_sPathFileOfIOCardInitFile;//**[12/8/2017 ConanSteve]**:  �ӿڿ���ʼ���ļ����·���������Run
	array<e_IOCardType, MAX_IOCARD_NUM> m_arr_iIOCardType;


	/*************************************<������ʾ���>************************************[12/1/2017 ConanSteve]*/
	int m_iPENum{0};//**[12/28/2017 ConanSteve]**:  ����������ϵͳ�����źŸ���
	int m_iCheckTotleCntType{ e_CheckTotleCntType_Rlst};//**[12/4/2017 ConanSteve]**:  ���������������
	int m_iCheckCntTotle{ 0 };//**[12/1/2017 ConanSteve]**:  �������
	int m_iCheckErrorCntTotle{ 0 };//**[12/1/2017 ConanSteve]**:  �ܵ�ȱ������
	array<int, MAX_CAMER_COUNT> m_arr_iPECount;//**[12/28/2017 ConanSteve]**:  ������


	/*************************************<״̬����ʾ��Ϣ����>************************************[12/28/2017 ConanSteve]*/
	bool m_bEnableShowPE{ true };//**[1/26/2018 ConanSteve]**:	�Ƿ���ʾ�������
	bool m_bEnableShowAutoKick{ true };//**[1/26/2018 ConanSteve]**:	�Ƿ���ʾ����
	bool m_bEnableShowGrabLostFrame{ true };//**[1/26/2018 ConanSteve]**:	�Ƿ���ʾ�����֡

	/*************************************<�����>************************************[12/28/2017 ConanSteve]*/

	array<bool, MAX_CAMER_COUNT> m_arr_bWhetherImageConvertedToMultiObj;//**[1/26/2018 ConanSteve]**:	��ʾ�㷨���ܵ�ͼ���Ƿ���ϵͳbayer�任�ͷ�ת֮��� 
	array<int, MAX_CAMER_COUNT> m_arr_iGrabCallbackCnt;//**[12/1/2017 ConanSteve]**:  ����ɼ��ص�����
	array<int, MAX_CAMER_COUNT> m_arr_iLastHardImageCountOfGrabInRegister;//**[12/13/2017 ConanSteve]**:  �ϴ�ͼ��Ĵ�������
	array<int, MAX_CAMER_COUNT> m_arr_iHardImageCountOfGrabInRegister;//**[12/13/2017 ConanSteve]**:  �����Ӧͼ��Ĵ������ż���	
	array<int, MAX_CAMER_COUNT> m_arr_iLostFrameEachGrab;//**[1/26/2018 ConanSteve]**:	ÿ�������֡����
	array<int, MAX_CAMER_COUNT> m_arr_iCurSmallCntEachGrab;//**[1/26/2018 ConanSteve]**:	ÿ�������ǰͼ��ŵ�С�ż��� 

	array<int, MAX_CAMER_COUNT> m_arr_iWidth;//**[1/26/2018 ConanSteve]**:	���ͼ���� 
	array<int, MAX_CAMER_COUNT> m_arr_iHeight;//**[1/26/2018 ConanSteve]**:	���ͼ��߶� 
	array<int, MAX_CAMER_COUNT> m_arr_iChannel;//**[1/26/2018 ConanSteve]**:	���ͼ��ͨ�������ڰ�ͼΪ1����ɫͼΪ3 
	array<int, MAX_CAMER_COUNT> m_arr_iSmallImgCntPerGrab;//**[8/30/2017 ConanSteve]**:  ÿ���������ͼ������С��ͼ����
	array<int, MAX_CAMER_COUNT> m_arr_iSystemBayerType;//**[12/28/2017 ConanSteve]**:   ϵͳbayer�任����
	array<int, MAX_CAMER_COUNT> m_arr_iSystemFilpType;//**[12/28/2017 ConanSteve]**:  ϵͳ��ת����
	array<int, MAX_CAMER_COUNT> m_arr_iHalconBayerType;//**[1/26/2018 ConanSteve]**:	���Halcon�㷨��Ҫ��bayer�任ֵ 
	array<int, MAX_CAMER_COUNT> m_arr_iHalconRevertType;//**[1/26/2018 ConanSteve]**:	���Halcon�㷨��Ҫʵ�ֵķ�ת���� 												  
	array<int, MAX_CAMER_COUNT> m_arr_iKickGroupSNOfCurGrab;//**[11/22/2017 ConanSteve]**:  ÿ��������ڵ��޷����																	
	array<int, MAX_CAMER_COUNT> m_arr_iCheckGroupSNOfCurGrab;//**[11/22/2017 ConanSteve]**:  ÿ��������ڵļ�����	
	array<int, MAX_CAMER_COUNT> m_arr_iLastImgSNOfEachGrab;//**[11/24/2017 ConanSteve]**:  ÿ���������һ��ͼ��ͼ���	
	array<int, MAX_CAMER_COUNT> m_arr_iCheckGroupFirstGrabIndexInCamGroup;//**[11/16/2017 ConanSteve]**:  ÿ�������ĵ�һ�������������е�����	
	array<CString, MAX_CAMER_COUNT> m_arr_sPathFileOfGrabInitInModelInfo;//**[9/6/2017 ConanSteve]**:  ÿ�������ModelInfo�ļ����е������ļ�·��																  
	array<CString, MAX_CAMER_COUNT> m_arr_sPathFileOfGrabInitInConfig;//**[9/6/2017 ConanSteve]**:  ÿ�������Config�ļ����е������ļ�·��
	array<shared_ptr<CGrabber>, MAX_CAMER_COUNT> m_arr_pGrabber; //**[8/30/2017 ConanSteve]**:  �����ַ������	
	array<e_GrabType, MAX_CAMER_COUNT> m_arr_iGrabType;//**[12/12/2017 ConanSteve]**:  ����������������
	array<CGrabInfo, MAX_CAMER_COUNT> m_arr_GrabInfo;//**[9/11/2017 ConanSteve]**:  ϵͳ������������ṹ�壺�������ڰ󶨲�������ģ��
	array<CImageInformation, MAX_CAMER_COUNT> m_arr_sCameraImageInfo;//**[8/30/2017 ConanSteve]**:  ͼ����Ϣ,ֻ�ṩ���㷨ʹ�ã�ע����CGrabInfo������ CGrabInfoֻ����ϵͳ������Ľ���
	array<TCHAR[MAX_PATH], MAX_CAMER_COUNT> m_arr_lpszPathOfSaveImg;//**[11/22/2017 ConanSteve]**:  �����ͼ·���� ��TCHAR���鲻��CString��Ҫ��Ϊ�˷������󿽱���ַ�����ﵽ��Ϣ���ݣ��ӿڲ�����ʹ��CString*���ͣ���ΪVS��ͬ�汾������
	array< unique_ptr <BYTE[] >, MAX_CAMER_COUNT> m_arr_pbtBufferOfBayerImg;//**[9/6/2017 ConanSteve]**:  Bayer�任���ͼ�����ݵ�ַ/��ת���ͼ�����ݵ�ַ����Ҫ����ϵͳ�Լ���ʾ��
	array< unique_ptr <BYTE[]>, MAX_CAMER_COUNT> m_arr_pbtBufferOfGrabImageCopy;//**[1/26/2018 ConanSteve]**:	�����ͼ����п�������ֹ�����ļ����󣬵��¶���󿽱�ͼ���ַԽ�� 
	array<shared_ptr<BITMAPINFO>, MAX_CAMER_COUNT> m_arr_pBitmapInfo;
	array<CBayerTransform, MAX_CAMER_COUNT> m_arr_BayerTransform;
	
	/*************************************<�����>************************************[12/28/2017 ConanSteve]*/
	
	array<VecInt,MAX_CAMER_COUNT> m_arr_veciGrabSNOfEachCheckGroup;//**[11/27/2017 ConanSteve]**:  ÿ������������������кš�
	map<int, int> m_map_CheckSN2KickSN;//**[12/28/2017 ConanSteve]**:  ����鵽�����޷����ӳ��

	array<bool, MAX_CHECK_GROUP_NUM> m_arr_bEnableSaveImgOfEachCheckGroup;//**[11/23/2017 ConanSteve]**:  ÿ��λ�Ƿ��ͼ
	array<int, MAX_CHECK_GROUP_NUM> m_arr_iCheckCntOfCheckGroup;//**[12/5/2017 ConanSteve]**:  ÿ������Ʒ����
	array<int, MAX_CHECK_GROUP_NUM> m_arr_iGoodCntOfCheckGroup;//**[12/5/2017 ConanSteve]**:  ÿ��������Ʒ����
	array<int, MAX_CHECK_GROUP_NUM> m_arr_iBadCntOfCheckGroup;//**[12/1/2017 ConanSteve]**:  ÿ�������ȱ������
	array<int, MAX_CHECK_GROUP_NUM> m_arr_iAlgExceptionCntOfCheckGroup;//**[12/5/2017 ConanSteve]**:  ÿ��λ�㷨�쳣����
	array< map<int, s_DefectInfo>, MAX_CHECK_GROUP_NUM> m_arr_mapDefectStatistics;//**[12/1/2017 ConanSteve]**:  ȱ��ͳ��
	array<bool, MAX_CHECK_GROUP_NUM> m_arr_bEnableExecCheckOfEachGroup;//**[12/14/2017 ConanSteve]**:  ÿ����⹤λ�Ƿ�ִ�������ļ�⣬�������⣬���㷨ֱ�ӷ��غ�Ʒ
	array<CExtenInfo, MAX_CHECK_GROUP_NUM> m_arr_ExtenInfo;//**[11/22/2017 ConanSteve]**:  ��չ��Ϣ�ṹ��
	
//	array<CMultiObjSaveImgPara, MAX_CHECK_GROUP_NUM> m_arr_MultipleObjSaveImgPara;

	/*************************************<�޷���>************************************[12/28/2017 ConanSteve]*/
	int m_iKickQueueNum{ 0 };//**[11/27/2017 ConanSteve]**:  �޷�����
	map<int, int> m_map_KickSN2AssemblySN;//**[12/28/2017 ConanSteve]**:  �߷϶��кŵ���ˮ�ߺŵ�ӳ��

	array<e_KickType, MAX_KICK_QUEUE_NUM> m_arr_eKickType;	//**[12/28/2017 ConanSteve]**:  �޷�����
	array<VecInt, MAX_KICK_QUEUE_NUM> m_arr_veciCheckSNInKickQueue;//**[11/27/2017 ConanSteve]**:  ÿ���޷�������ļ�������кš�
	array<s_KickCountInfo, MAX_KICK_QUEUE_NUM> m_arr_sKickCntInfoOfKickQueue;//**[12/1/2017 ConanSteve]**:  ÿ���޷϶��е��޷ϼ���(������Դ�ڽӿڿ�ʵ��ִ�����߷϶������߷ϼ���)
	array<int, MAX_KICK_QUEUE_NUM> m_arr_iGoodCntOfKickQueue;//**[1/24/2018 ConanSteve]**:	ÿ���߷϶��лص�ʱ����ĺ�Ʒ����
	array<bool, MAX_KICK_QUEUE_NUM> m_arr_bEnableKickOfKickQueue;

	/*************************************<��ˮ����>************************************[12/28/2017 ConanSteve]*/ 
	int m_iAssemblyLineNum{ 1 };//**[12/5/2017 ConanSteve]**:  ��ˮ����������Ʒͨ����
	int m_iCheckSpeedUpdateInterval{ 1000 };//**[12/28/2017 ConanSteve]**:  ������ˮ���ٶ���ʾ������
	array<VecInt, MAX_ASSEMBLYLINE_NUM> m_arr_veciKickSNInAssemblyLine;//**[12/28/2017 ConanSteve]**:  ÿ����ˮ���е��޷����
	array<VecInt, MAX_ASSEMBLYLINE_NUM> m_arr_veciCheckGroupIndexOfAsseblyLine;//**[12/5/2017 ConanSteve]**:  ÿ����ˮ�ߵļ��������

	array<int, MAX_ASSEMBLYLINE_NUM>    m_arr_iCheckCntOfAssembly;//**[12/28/2017 ConanSteve]**:  ÿ����ˮ�ߵĲ�Ʒ����
	array<int, MAX_ASSEMBLYLINE_NUM>    m_arr_iCheckErrorCntOfAssembly;//**[12/28/2017 ConanSteve]**:  ÿ����ˮ�ߵĻ�Ʒ����
	array<int, MAX_ASSEMBLYLINE_NUM>	m_arr_iCheckGoodCntOfAssembly;//**[12/28/2017 ConanSteve]**:  ÿ����ˮ�ߵĺ�Ʒ����
	array<int, MAX_ASSEMBLYLINE_NUM>	m_arr_iCheckSpeedOfAssembly;//**[12/28/2017 ConanSteve]**:  ÿ����ˮ�ߵļ���ٶ�
	array<double, MAX_ASSEMBLYLINE_NUM>	m_arr_lfGoodRatioOfAssembly;//**[1/24/2018 ConanSteve]**:	ÿ����ˮ�ߵļ���ٶ� 
	array<queue<int>, MAX_ASSEMBLYLINE_NUM> m_arr_queCheckCntPerInterval;//**[12/28/2017 ConanSteve]**:  ÿ����ˮ����ʷ����Ʒ�����������60��
	/*************************************<��־>************************************[12/13/2017 ConanSteve]*/

	//**[8/25/2017 ConanSteve]**:  ������Ϣ��־
	CLogFile m_fileDebugInfo;
	CLogFile m_fileErrorInfo;

	//**[12/13/2017 ConanSteve]**:  ������־
	bool m_bEnableLog{ false };//**[12/13/2017 ConanSteve]**:  �Ƿ��¼��־
	unique_ptr<CLogFile[]> m_p_fileImgCntOfGrabInRegister;//**[12/13/2017 ConanSteve]**:  ͼ��Ĵ����仯��־
	unique_ptr<CLogFile[]> m_p_fileGrabCallback{ nullptr };//**[12/13/2017 ConanSteve]**:  ����ص���־
	array<CLogFile, MAX_KICK_QUEUE_NUM> m_arr_fileKickCallback;//**[12/15/2017 ConanSteve]**:  �޷ϻص���־
	unique_ptr<CLogFile[]> m_p_fileKickCntInfoChange{ nullptr };//**[12/15/2017 ConanSteve]**:  �޷���Ϣ�����仯��־

	/*************************************<�ٽ�����Դ������>************************************[12/14/2017 ConanSteve]*/
	CCriticalSection m_csGetCount_TuXiangHao;

	/*************************************<���߷�������ͨѶ�¼�>************************************[12/28/2017 ConanSteve]*/
	HANDLE m_hEvent_DemoMessage_SendCommandOfLoadModel{ NULL };//**[1/7/2018 ConanSteve]**:	���ܶ�ģ���¼� 
	HANDLE m_hEvent_DemoMessage_ReceiveSuccessOfLoadModel{ NULL };//**[1/7/2018 ConanSteve]**:	���ͼ��سɹ���Ϣ 
	HANDLE m_hEvent_DemoMessage_ReveiveFailureOfLoadModel{ NULL };//**[1/7/2018 ConanSteve]**:	���ͼ���ʧ����Ϣ 

/************************************************************************/

private://**[8/24/2017 ConanSteve]**:  ˽�к���


public://**[8/24/2017 ConanSteve]**:  ���к���
	//**[8/24/2017 ConanSteve]**:  ��ȡ�����ļ�
	bool ReadCfg();
	//**[8/24/2017 ConanSteve]**:  ��ʼ��ϵͳ
	bool InitializeSystem();
	//**[8/24/2017 ConanSteve]**:  ��ȡϵͳUI�ļ�
	bool ReadCfgOfUI();
	//**[8/24/2017 ConanSteve]**:  �ͷ�ϵͳ�ڴ�
	bool ReleaseMemory();
	afx_msg void OnClose();
	//**[8/25/2017 ConanSteve]**:  ��ȡ�㷨�����ļ�
	bool ReadCfgOfAlg();
	//**[8/25/2017 ConanSteve]**:  ��ȡ���������ļ�, CheckSystemConfig.ini
	bool ReadCfgOfSys();
	//**[8/25/2017 ConanSteve]**:  ������������ļ�
	void CheckCamCfgFile();
	//**[8/25/2017 ConanSteve]**:  ��ʼ���ӿڿ�
	CRetInfo InitIOCard();
	//**[8/25/2017 ConanSteve]**:  ������ʼ�����̺���
	DWORD static ThreadInitBackstage(PVOID pParam);
	//**[8/25/2017 ConanSteve]**:  ������ʼ��ʵ�ֺ���
	DWORD ThreadInitBackstageFunc(int iFunc =0);
	//**[8/25/2017 ConanSteve]**:  ��ʼ�����
	CRetInfo InitGrabber();
	//**[8/25/2017 ConanSteve]**:  ��ʼ�����ݿ�
	CRetInfo InitDataBase();
	//**[8/25/2017 ConanSteve]**:  �����ڴ�
	bool AllocateMemory();
	//**[8/30/2017 ConanSteve]**:  ����ص�����
	static void WINAPI AreaGrabOverCallback(const s_GBSIGNALINFO* SignalInfo);
	//**[8/31/2017 ConanSteve]**:  ��ʼ����־ϵͳ
	bool InitLogSys();
	//**[9/4/2017 ConanSteve]**:  ��ʼ���
	bool StartCheck();
	//**[9/4/2017 ConanSteve]**:  �ر�/��ͣ���
	bool StopCheck();
	//**[9/5/2017 ConanSteve]**:  ϵͳʵʱ������Ϣ
	//**[9/5/2017 ConanSteve]**:  bHint:�Ƿ�ǰ̨��ʾ
	typedef struct {
		bool bHint{ false };
		CString sMsg{_T("")};
	}s_HintMsg;
	//**[12/8/2017 ConanSteve]**:  ����Ϣ��ʾ��״̬����bHint==true��ʾ��Ҫ�ڴ���ǰ����ʾ
	//**[12/8/2017 ConanSteve]**:  bErrorInfo==true��ʾ��Ϣ��Ҫд��ϵͳ������־��
	bool AddMsg(CString sMsg, bool bHint=false, bool bErrorInfo = false);
	//**[9/5/2017 ConanSteve]**:  ����ص�
	void AreaGrabOverCallbackFunc(const s_GBSIGNALINFO* SigInfo);
	bool CameraGrabFuncSingle(int iCameraSN);
	bool WriteCfg();
	bool OnBtnClickOpenDemo();
	//**[9/11/2017 ConanSteve]**:  �������������
	bool OnBnClickOpenParamSet();
	//**[9/11/2017 ConanSteve]**:  �ͷ���Դ
	bool ReleaseResource();
	//**[9/11/2017 ConanSteve]**:  ���ӿڿ������ļ�
	int CheckIOCardCfgFile();
	inline bool JudgeGrabCircleNew(int iCameraSN,int  iAllElementSN,int& iGrabCircle,int& iPicSN);
	CRetInfo InitMultiObject();

	//**[10/30/2017 ConanSteve]**:   �޷ϻص�	
	static void WINAPI GlobalKickCallbackFunc(const s_KickParam* pKickParam);
	void KickCallbackFunc1(const s_KickParam* pKickParam);

	//**[10/30/2017 ConanSteve]**:   ״̬�ص�
	static void WINAPI GlobalStatusCallbackFunc(const s_Status* sStatusParam);
	void StatusCallbackFunc1(const s_Status* sStatusParam);
	int SaveBmp(const TCHAR* pPathName, int iWidth, int iHeight, int iByte, TBYTE *pDest);
	//**[12/6/2017 ConanSteve]**:  ��ϵͳʹ�õ�Bayer�任�㷨��Ӧ��ö��ֵת���ɷ���HalconBayer�任������ֵ
	e_BayerHalcon TranslateToHalconBayer(int iBayerType);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	bool ResizeLayout();//**[12/4/2017 ConanSteve]**:  ��ʼ�����֮�󣬸��������С���µ�����Ҫ�Ĳ��֣�ͼ��λ�ã�����֪����
	//**[12/5/2017 ConanSteve]**:  ϵͳ��������
	bool ResetSystem(int iType = 7);//**[12/28/2017 ConanSteve]**:  iType = 1ֻ���ü����� 2ֻ���ýӿڿ���4ֻ���ö����
	//**[12/8/2017 ConanSteve]**:  �򿪽ӿڿ�����
	bool OnBtnClickOpenIOCardTools();

	
	BOOL GetCount_Guangdian();// ��ȡ������
	BOOL GetCount_Tifei();// ��ȡ�޷ϼ���
	BOOL GetCount_TuXiangHao();// ��ȡͼ��żĴ�������

	bool OnBtnClickOpenDefectStatistics();//**[12/27/2017 ConanSteve]**:  ��ȱ��ͳ��
	inline bool StartAllThread();
	//**[12/28/2017 ConanSteve]**:   ��ȡ�����߳�
	static DWORD GetIOCardImageNumThread(LPVOID lpParam);
	DWORD GetIOCardImageNumThreadFunc();
	static DWORD WaitLoadModelThread(LPVOID lpParam);
	DWORD WaitLoadModelThreadFunc();
	BOOL StartCameraGrab(int iGrabSN);
	CRetInfo LoadModel(CString sModelName = _T(""));//**[1/7/2018 ConanSteve]**:	����ģ�� 
	BOOL SetLutTable(double dRatioR, double dRatioG, double dRatioB);
	int MirrorSingleImage(BYTE* pImageBuff, int lImageWidth, int lImageHeight, int lImageBitCount, e_ShowModeFlip  eShowMode);
	bool OnBtnClickOpenDataStatistics();
};


