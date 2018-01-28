
#pragma once
#include "typeinfo"
#include "iostream"
#include "string"
#include "vector"
#include "ExtenInfo.h"
using namespace std;


class CExtenInfoInterior
{
public:
	CExtenInfoInterior();
	~CExtenInfoInterior();
	void* GetFuncAddr(e_OptionalFuncCode iFuncCode);//**[9/26/2017 ConanSteve]**:  ���ص���COptionalFunczhizhen
	CGeneralParam& GetGeneralParam(e_OptionalFuncCode iFuncCode);
//	COptionalFunc* GetFuncAddr(const type_info & typeInfo);
	void push_back(COptionalFunc* );
	bool remove_last();
	void clear();
	int GetFuncNum() { return vecOptionalFunc.size(); };
	CExtenInfoInterior& operator=(const CExtenInfoInterior&);

protected: 
	vector<COptionalFunc*> vecOptionalFunc;//**[7/19/2017 ConanSteve]**:  ����һ��ָ�����飬���洢�����ܽṹ��ָ��
// 	COptionalFunc** pp_arrOptionalFunc;
// 	int iSizeOfFunc;
// 	int iCapacityOfFunc;
};

class COptionalFuncInterior:public COptionalFunc
{
public:
	virtual e_OptionalFuncCode GetFuncCode() override { return iFuncCode; }
	virtual bool SetFuncCode(e_OptionalFuncCode iFuncCode)override { this->iFuncCode = iFuncCode;return true; };
protected:
	e_OptionalFuncCode iFuncCode;//**[7/19/2017 ConanSteve]**:  �����룬ָ������ṹ�崫�ݵ�����Щ���ܵĲ���
	int iSize;//**[7/19/2017 ConanSteve]**:  �ṹ���С��������չ
};

/*****************************************************************************************/

 class CMultiObjSaveImgParaInterior :public COptionalFuncInterior//**[7/18/2017 ConanSteve]**:  ������ͼ��ز���
 {
 public:
 	CMultiObjSaveImgParaInterior(void* = nullptr);
 	~CMultiObjSaveImgParaInterior();
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
 
 protected:
 	int iImgNum;//*[7/7/2017 ConanSteve]*:  ÿ�����������Ʒ���ĵ�����ͼ�����
 	int iImgWidth;//*[7/7/2017 ConanSteve]*:  ����ͼ�Ŀ��
 	int iImgHeight;//*[7/7/2017 ConanSteve]*:  ����ͼ�ĸ߶�
 	int iImgChannel;//*[7/7/2017 ConanSteve]*:  ͨ����
 	int iLenOfImgFolderPathBuf;	//**[7/26/2017 ConanSteve]**:  ����ͼ��·������������
 	TCHAR lpszSaveImgFolderPath[MAX_PATH];//*[7/7/2017 ConanSteve]*:  ��ǰ���ͼ���ļ���·��
 								//	wchar_t* pwszSaveImgFolderPath;//**[7/26/2017 ConanSteve]**:  Ϊ֧��Unicode ������
 	bool bIsCirCleSaveImg;//*[7/10/2017 ConanSteve]*:  �Ƿ�ѭ����ͼ
 };
 
 class CAlgSaveImgParaInterior :public COptionalFuncInterior//**[7/18/2017 ConanSteve]**:  �㷨��ͼ��ز���
 {
 public:
 	CAlgSaveImgParaInterior(void* = nullptr);
 	~CAlgSaveImgParaInterior();
 	void SetImgSN(int iImgSN);
 	int GetImgSN();//**[9/19/2017 ConanSteve]**:  ��õ�ǰͼ���
 	void SetImgSavedCnt(int iImgSavedCnt);
 	int GetImgSavedCnt();//**[9/19/2017 ConanSteve]**:  ����Ѵ�ͼ�������
 	void SetSaveImgFolderPath(LPCTSTR lpszAddr, int iLen);
 	TCHAR* GetAddrOfSaveImgFolderPath();

 protected:
 	int iImgSN{ 0 };//**[7/18/2017 ConanSteve]**:  ��ǰͼ���
 	int iImgSaveCnt{ 0 };//**[7/18/2017 ConanSteve]**:  �Ѵ�ͼ����
 	int iLenOfImgFolderPathBuf;	//**[7/26/2017 ConanSteve]**:  ����ͼ��·������������
 	TCHAR lpszSaveImgFolderPath[MAX_PATH];//**[7/18/2017 ConanSteve]**:  ��ͼ�ļ���·����ֻ���㷨������ͼʱ����Ҫ�������
 								//wchar_t* pwszSaveImgFolderPath;
 };
 
 class CSetObjStatusWndInterior :public COptionalFuncInterior
 {
 public:
 	CSetObjStatusWndInterior(void* = nullptr);
 	void SetIsShow(bool bIsShow);
 	bool GetIsShow();
 protected:
 	bool bIsShow;
 };
 
 class CSetAlgObjectInterior :public COptionalFuncInterior//**[7/26/2017 ConanSteve]**:  ��ģ�崫��ģ�����
 {
 public:
 	CSetAlgObjectInterior(void* = nullptr);
 	~CSetAlgObjectInterior() {}
 	void SetAddrOfModelObj(void* pAddrOfModelObject);
 	void * GetAddrOfModelObj();
 	void SetLoadModelCtrlPara(int iLoadModelCtrlPara);
 	int GetLoadModelCtrlPara();
	void SetAlgObjIndex(int iAlgObjIndex);//**[10/9/2017 ConanSteve]**:  �����㷨�������ڶ�����������е�����
	int GetAlgObjIndex();
 protected:
 	void* pAddrOfModelObject;
 	int   iLoadModelCtrlPara;
	int iAlgObjIndex;
 };

class CAlgDetectParaInterior :public COptionalFuncInterior
{
public:
	CAlgDetectParaInterior(void* = nullptr);
	~CAlgDetectParaInterior() {}
	void SetDetectTimes(int iDetectTimes);
	int GetDetectTimes();
protected:
	int iDetectTimes{ 0 };

};

class CAlgCopyWindowInterior :public COptionalFuncInterior
{
public:
	CAlgCopyWindowInterior();
	~CAlgCopyWindowInterior() {}
	void SetTimeOfWait(int iTimeOfWait);
	int GetTimeOfWait();//**[10/9/2017 ConanSteve]**:  ��������ڻ�������еĵȴ�ʱ��
	void SetTimeOfFirstDetect(int iTimeOfFirstDetect);
	int GetTimeOfFirstDetect();//**[10/9/2017 ConanSteve]**:  ���1��Detectʱ��
	void SetTimeOfSecondDetect(int iTimeOfSecondDetect);
	int GetTimeOfSecondDetect();//**[10/9/2017 ConanSteve]**:  ���2��Detectʱ��
	void SetTickOfPushQueue(LONGLONG llTickOfPushQueue);
	LONGLONG GetTickOfPushQueue();
	CAlgCopyWindowInterior& operator=(const CAlgCopyWindowInterior&);
private:
	int iTimeOfWait;
	int iTimeOfFirstDetect;
	int iTimeOfSecondDetect;
	LONGLONG llTickOfPushQueue;//**[10/9/2017 ConanSteve]**:  ��¼ͼ�����ݵ�һ�ν���ͼ�����ݶ��е�ʱ��
};
/*************************************<>*************************************/


class  CGeneralParamInterior :public COptionalFuncInterior
{
public:
	CGeneralParamInterior();
	CGeneralParamInterior(e_OptionalFuncCode e_FuncCode);
	~CGeneralParamInterior();



	bool SetInt(int iVal);
	int GetInt();
	bool SetArrayInt(int* pArrInt, int iLen);
	bool GetArrayInt(int* pArrInt, int& iLen);

	bool SetBool(bool bVal);
	bool GetBool();
	bool SetArrayBool(bool* pArrBool, int iLen);
	bool GetArrayBool(bool* pArrBool, int& iLen);

	//double SetDouble(double lfVal);
	bool SetPTCHAR(TCHAR* lpData);//**[1/10/2018 ConanSteve]**:  Ĭ�ϳ���256
	TCHAR* GetPTCHAR();
	bool SetArrayPTCHAR(TCHAR** lppData, int iLen);//**[1/10/2018 ConanSteve]**:  ����ַ�������256
	bool GetArrayPTCHAR(TCHAR** lppData, int &iLen);//**[1/10/2018 ConanSteve]**:  ������鳤��256

private:
	union {
		int iData;
		int arr_iData[MAX_PATH];
		bool bData;
		bool arr_bData[MAX_PATH];
		TCHAR lpszData[MAX_PATH*MAX_PATH];
		TCHAR arr_lpszData[MAX_PATH][MAX_PATH];
	}m_unionData;
	int m_iLen{ 1 };
};

