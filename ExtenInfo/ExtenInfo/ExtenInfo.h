// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� EXTENINFO_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// EXTENINFO_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�

#pragma once
#include "typeinfo"
#include "iostream"
#include "string"
using namespace std;


#ifdef EXTENINFO_EXPORTS
#define EXTENINFO_API __declspec(dllexport)
#else
#define EXTENINFO_API __declspec(dllimport)
#endif

// �����Ǵ� ExtenInfo.dll ������
// class EXTENINFO_API CExtenInfo {
// public:
// 	CExtenInfo(void);
// 	// TODO:  �ڴ�������ķ�����
// };

extern EXTENINFO_API int nExtenInfo;

EXTENINFO_API int fnExtenInfo(void);


//**[7/18/2017 ConanSteve]**:  ��չ���ܶ���
enum e_OptionalFuncCode {
	e_OptionalFuncCode_NONE = 0,//**[1/11/2018 ConanSteve]**:  Ĭ��δ����
	Sys2Obj_SaveImgPara = 1,	//**[7/26/2017 ConanSteve]**:  ϵͳʹ�ö������㷨��ͼ��ز���
	Obj2Alg_SaveImgPara = 2,	//**[7/26/2017 ConanSteve]**:  �������㷨��ͼ��ز���
	Sys2Obj_SetObjStatus = 3,	//**[7/26/2017 ConanSteve]**:  �����Ƿ���ʾ�����ʵʱ״̬����
	Obj2Alg_SetAlgObject = 4, //**[7/26/2017 ConanSteve]**:  ��ʼ���������ʱ���������������һ��������ĵ�ַ
	Obj2Alg_AlgDetectPara = 5,//**[10/9/2017 ConanSteve]**:  ���������㷨Detectʱʹ��
	Obj2Alg_AlgCopyWindow = 6,//**[10/9/2017 ConanSteve]**:  ���������㷨�Ŀ�������ʱʹ��


	//**[1/11/2018 ConanSteve]**:  s_CheckInitParam

	/*************************************<s_CheckInParam>************************************[1/11/2018 ConanSteve]*/
	e_Fn_CheckInParam_SaveImageNum = 0x2000,//**[1/11/2018 ConanSteve]**:  ����ͼ����int
	e_Fn_CheckInParam_ImageWidth = 0x2001,//**[1/11/2018 ConanSteve]**:  ͼ����int
	e_Fn_CheckInParam_ImageHeight = 0x2002,//**[1/11/2018 ConanSteve]**:  int
	e_Fn_CheckInParam_ImageChannel= 0x2003,//**[1/11/2018 ConanSteve]**:  int
	e_Fn_CheckInParam_WhetherCicleSaveImage = 0x2004,//**[1/11/2018 ConanSteve]**:  �Ƿ�ѭ����ͼbool
	e_Fn_CheckInParam_SaveImagePath = 0x2005,//**[1/11/2018 ConanSteve]**:  ��ͼ·��,ʹ��


	/*************************************<s_CheckOutPara>************************************[1/11/2018 ConanSteve]*/
	e_Fn_CheckOutPara_AlgCheckErrorDesc = 0x4000   //**[1/10/2018 ConanSteve]**:  �㷨���صĴ�������

	//**[1/11/2018 ConanSteve]**:  ������ͼ���
	
};

/*************************************<����ģ����Ʋ�������>*************************************/
enum e_LoadModelCtrlPara
{
	e_LoadModelCtrlPara_ReadPara = 1,
	e_LoadModelCtrlPara_ReadModel = 2,
	e_LoadModelCtrlPara_ReadParaAndModel = 3
};
/************************************************************************/

class EXTENINFO_API COptionalFunc//**[7/18/2017 ConanSteve]**:  ��չ���ܸ���
{
public:
	COptionalFunc() {}
	virtual e_OptionalFuncCode GetFuncCode();//**[9/25/2017 ConanSteve]**:  ��õ�ǰ����Ĵ���
	virtual bool SetFuncCode(e_OptionalFuncCode);
protected:
	void* pInterior;
};

//**[1/10/2018 ConanSteve]**:  ʹ��256*256��TCHAR�ַ�������Ϣ
class EXTENINFO_API CGeneralParam :public COptionalFunc
{
public:
	CGeneralParam();
	CGeneralParam(e_OptionalFuncCode iFuncCode);
	~CGeneralParam();

	bool SetInt(int iVal);
	int GetInt();
	bool SetArrayInt(int* pArrInt, int iLen);
	bool GetArrayInt(int* pArrInt, int& iLen);

	bool SetBool(bool bVal);
	bool GetBool();
	bool SetArrayBool(bool* pArrBool, int iLen);
	bool GetArrayBool(bool* pArrBool, int& iLen);

	bool SetDouble(double lfVal);
	double GetDouble();
	bool SetArrayDouble(double* plfVal, int iLen);
	bool GetArrayDouble(double* plfVal, int& iLen);

	bool SetPTCHAR(TCHAR* lpData);//**[1/10/2018 ConanSteve]**:  Ĭ�ϳ���256
	TCHAR* GetPTCHAR();
	bool SetArrayPTCHAR(TCHAR** lppData, int iLen);//**[1/10/2018 ConanSteve]**:  ����ַ�������256
	bool GetArrayPTCHAR(TCHAR** lppData, int &iLen);//**[1/10/2018 ConanSteve]**:  ������鳤��256
													//CGeneralParam& operator=(const CGeneralParam&);
};
// {
// 	CExtenInfo extenInfo;
// 	CGeneralParam generalParam(e_GeneralParamType_AlgCheckErrorDesc);
// 	TCHAR buf[MAX_PATH];
// 	generalParam.SetPTCHAR(buf);
// 	extenInfo.push_back(&generalParam);
// 	iReserve4 = &extenInfo;
// }

class EXTENINFO_API CExtenInfo
{
public:
	CExtenInfo();
	~CExtenInfo();
	COptionalFunc* GetFuncAddr(e_OptionalFuncCode iFuncCode);//**[9/25/2017 ConanSteve]**:  ���ָ�����ŵ����ַ
	CGeneralParam& GetGeneralParam(e_OptionalFuncCode iFuncCode);
	void push_back(COptionalFunc*);//**[9/25/2017 ConanSteve]**:  ��һ��COptionalFunc��������ӽ�CExtenInfo��
	bool remove_last();//**[9/25/2017 ConanSteve]**:  �Ƴ����һ�����������ö���ʹ����֮�󣬵��ö�������Ƴ����һ����������ܷ����쳣
	void clear();
	int GetFuncNum();//**[9/22/2017 ConanSteve]**:  �����չ�����Ŀ
	CExtenInfo& operator=(const CExtenInfo&);
protected:
	void* pInterior;
};


/*****************************************************************************************/

class EXTENINFO_API CMultiObjSaveImgPara :public COptionalFunc//**[7/18/2017 ConanSteve]**:  ������ͼ��ز���
{
public:
	CMultiObjSaveImgPara();
	~CMultiObjSaveImgPara();
	void SetImgNum(int iImgNum);//**[7/26/2017 ConanSteve]**:  ����ͼ������
	int GetImgNum();//**[7/26/2017 ConanSteve]**:  ���ͼ������
	void SetImgWidth(int iImgWidth);//**[7/26/2017 ConanSteve]**:  ����ͼ��߶�
	int GetImgWidth();//**[7/26/2017 ConanSteve]**:  ���ͼ����
	void SetImgHeight(int iImgHeight);//**[7/26/2017 ConanSteve]**:  ����ͼ��߶�
	int GetImgHeight();
	void SetImgChannel(int iImgChannel);
	int GetImgChannel();
	void SetIsCirCleSaveImg(bool bIsCirCleSaveImg);
	bool GetIsCirCleSaveImg();
	void SetSaveImgFolderPath(LPCTSTR lpszSaveImgFolderPath, int iLen);
	TCHAR* GetAddrOfSaveImgFolderPath();
};
 
 class EXTENINFO_API CAlgSaveImgPara :public COptionalFunc//**[7/18/2017 ConanSteve]**:  �㷨��ͼ��ز���
 {
 public:
	 CAlgSaveImgPara();
	 ~CAlgSaveImgPara();
 	void SetImgSN(int iImgSN);
 	int GetImgSN();//**[9/19/2017 ConanSteve]**:  ��õ�ǰͼ���
 	void SetImgSavedCnt(int iImgSavedCnt);
 	int GetImgSavedCnt();//**[9/19/2017 ConanSteve]**:  ����Ѵ�ͼ�������
 	void SetSaveImgFolderPath(LPCTSTR lpszAddr, int iLen =MAX_PATH);
 	TCHAR* GetAddrOfSaveImgFolderPath();
 };
 
 class EXTENINFO_API CSetObjStatusWnd :public COptionalFunc
 {
 public:
	 CSetObjStatusWnd();
	 ~CSetObjStatusWnd();
 	void SetIsShow(bool bIsShow);
 	bool GetIsShow();
 };
 
 class EXTENINFO_API CSetAlgObject :public COptionalFunc//**[7/26/2017 ConanSteve]**:  ��ģ�崫��ģ�����
 {
 public:
	 CSetAlgObject();
	 ~CSetAlgObject();
 	void SetAddrOfModelObj(void* pAddrOfModelObject);
 	void * GetAddrOfModelObj();
 	void SetLoadModelCtrlPara(int iLoadModelCtrlPara);
 	int GetLoadModelCtrlPara();
	void SetAlgObjIndex(int iAlgObjIndex);//**[10/9/2017 ConanSteve]**:  �����㷨�������ڶ�����������е�����
	int GetAlgObjIndex();
 };

class EXTENINFO_API CAlgDetectPara :public COptionalFunc//**[9/22/2017 ConanSteve]**:  ���������㷨Detect������Ϣ��
{
public:
	CAlgDetectPara();
	~CAlgDetectPara();
	void SetDetectTimes(int iDetectTimes);//**[9/25/2017 ConanSteve]**:  ����Detect������0��ʾһ��Detect��1��2�ֱ��ʾ����Detect�ĵ�1�κ͵�2�ε���
	int GetDetectTimes();//**[9/25/2017 ConanSteve]**:  ��ö�����DetectTimesֵ
};

class EXTENINFO_API CAlgCopyWindow :public COptionalFunc
{
public:
	CAlgCopyWindow();
	~CAlgCopyWindow();
	void SetTimeOfWait(int iTimeOfWait);
	int GetTimeOfWait();//**[10/9/2017 ConanSteve]**:  ��������ڻ�������еĵȴ�ʱ��
	void SetTimeOfFirstDetect(int iTimeOfFirstDetect);
	int GetTimeOfFirstDetect();//**[10/9/2017 ConanSteve]**:  ���1��Detectʱ��
	void SetTimeOfSecondDetect(int iTimeOfSecondDetect);
	int GetTimeOfSecondDetect();//**[10/9/2017 ConanSteve]**:  ���2��Detectʱ��
	void SetTickOfPushQueue(LONGLONG llTickOfPushQueue);
	LONGLONG GetTickOfPushQueue();
	CAlgCopyWindow& operator=(const CAlgCopyWindow&);
};



/*************************************<>*************************************/


