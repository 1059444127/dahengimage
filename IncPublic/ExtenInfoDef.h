#pragma once
//**[7/18/2017 ConanSteve]**:  ��չ���ܶ���
enum e_OptionalFuncCode {
	e_Fn_NONE = 0,//**[1/11/2018 ConanSteve]**:  Ĭ��δ����




	/*************************************<s_CheckInParam>************************************[1/11/2018 ConanSteve]*/
	e_Fn_CheckInParam_SmallImageNumOfEachGrab = 0x20000,//**[1/11/2018 ConanSteve]**:  ��ǰ��λÿ�������С��ͼ����	int
	e_Fn_CheckInParam_ImageWidth = 0x20001,//**[1/11/2018 ConanSteve]**:  ͼ����	int
	e_Fn_CheckInParam_ImageHeight = 0x20002,//**[1/11/2018 ConanSteve]**:	int
	e_Fn_CheckInParam_ImageChannel = 0x20003,//**[1/11/2018 ConanSteve]**:	int
	e_Fn_CheckInParam_WhetherCicleSaveImage = 0x20004,//**[1/11/2018 ConanSteve]**:  �Ƿ�ѭ����ͼ	bool
	e_Fn_CheckInParam_SaveImagePath = 0x20005,//**[1/11/2018 ConanSteve]**:  ��ͼ·��
	e_Fn_CheckInParam_GrabNumPerCheckGroup =0x20006,//**[1/25/2018 ConanSteve]**:  ÿ����⹤λ���������

	e_Fn_CheckInParam_DetectTimes = 0x20100,//**[1/11/2018 ConanSteve]**:  �㷨������	int

	e_Fn_CheckInParam_TimeWait = 0x20200,//**[1/11/2018 ConanSteve]**:  ͼ�����ݵȴ�ʱ��
	e_Fn_CheckInParam_TimeFirstDetect = 0x20201,//**[1/11/2018 ConanSteve]**:  һ��Detectʱ�� int
	e_Fn_CheckInParam_TimeSecondDetect = 0x20202,//**[1/11/2018 ConanSteve]**:  2��Detectʱ�� int
	e_Fn_CheckInParam_MomentOfPushQueue = 0x20203,//**[1/12/2018 ConanSteve]**:  ͼ����������е�ʱ��

	/*************************************<s_AlgModelInput>************************************[1/11/2018 ConanSteve]*/
	//**[1/11/2018 ConanSteve]**:  ��ģ��
	e_Fn_AlgModelInput_LoadModelType = 0x30000,//**[1/11/2018 ConanSteve]**:  ����ģ������� int
	e_Fn_AlgModelInput_CheckObjIndex = 0x30001,//**[1/11/2018 ConanSteve]**:  �������ڼ������е�����	void*
	e_Fn_AlgModelInput_FirstCheckObjAddr = 0x30002,//**[1/11/2018 ConanSteve]**:  �׸��������ָ��	int

	//**[1/11/2018 ConanSteve]**:  ��ͼ300ʹ��
	e_Fn_AlgModelInput_ImageSN = 0x30100,//**[1/11/2018 ConanSteve]**:  �㷨��ͼ��ͼ��� int
	e_Fn_AlgModelInput_ImageSavedCnt = 0x30101,//**[1/11/2018 ConanSteve]**:  ϵͳ�Ѿ���ͼ���� int
	e_Fn_AlgModelInput_SaveImagePath = 0x30102,//**[1/11/2018 ConanSteve]**:  �㷨��ͼ��·�������������Ϊbmp·�������������ͼΪ�ļ��У�void*תTCHAR[][MAX_PATH]

	e_Fn_AlgModelInput_WhetherShowObjectRunInfo = 0x30200,//**[1/12/2018 ConanSteve]**:  �Ƿ���ʾ�����������Ϣ����
	/*************************************<s_CheckOutPara>************************************[1/11/2018 ConanSteve]*/
	e_Fn_CheckOutPara_AlgCheckErrorDesc = 0x40000,   //**[1/10/2018 ConanSteve]**:  �㷨���صĴ������� TCHAR[]


	e_Fn_MAX = 0xFFFFFFFF
																																									 //**[1/11/2018 ConanSteve]**:  ������ͼ���
};

/*************************************<����ģ����Ʋ�������>*************************************/
enum e_LoadModelCtrlPara
{
	e_LoadModelCtrlPara_ReadPara = 1,
	e_LoadModelCtrlPara_ReadModel = 2,
	e_LoadModelCtrlPara_ReadParaAndModel = 3
};