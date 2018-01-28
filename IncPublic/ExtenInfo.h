// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� EXTENINFO_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// EXTENINFO_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�

#pragma once
#include "ExtenInfoDef.h"
#include "iostream"
#include "string"
using namespace std;

/************************************************************************/
const GUID guID = { 0x15ac02db, 0x48c8, 0x499a,{ 0xa2, 0x96, 0x39, 0x62, 0xc7, 0xfa, 0x8e, 0x93 } };
class COptionalFunc
{
public:
	COptionalFunc();
	COptionalFunc(e_OptionalFuncCode iFuncCode);
	e_OptionalFuncCode GetFuncCode();//**[9/25/2017 ConanSteve]**:  ��õ�ǰ����Ĵ���
	COptionalFunc& SetFuncCode(e_OptionalFuncCode iFuncCode);

	COptionalFunc& SetBool(bool bVal);
	bool GetBool();

	COptionalFunc& SetInt(int iVal);
	int GetInt();

	COptionalFunc& SetPointer(void* ptr);
	void* GetPointer();

	COptionalFunc& SetDouble(double lfVal);
	double GetDouble();

	COptionalFunc& SetLonglong(LONGLONG ldVal);
	LONGLONG GetLonglong();

	COptionalFunc& SetArrayTCHAR(TCHAR* lpData);//**[1/10/2018 ConanSteve]**:  Ĭ�ϳ���256
	TCHAR* GetArrayTCHAR();
protected:
	e_OptionalFuncCode iFuncCode;//**[7/19/2017 ConanSteve]**:  �����룬ָ������ṹ�崫�ݵ�����Щ���ܵĲ���
	union {
		bool bData;
		int iData;
		void* pPtr;
		double lfData;
		LONGLONG ldData;
		TCHAR lpszData[MAX_PATH];
	}m_unionData;
};

class CExtenInfo
{
public:
	CExtenInfo();
	~CExtenInfo();
	COptionalFunc& GetFuncStruct(e_OptionalFuncCode iFuncCode);
	COptionalFunc& push_back(COptionalFunc Func);//**[9/25/2017 ConanSteve]**:  ��һ��COptionalFunc��������ӽ�CExtenInfo��
	COptionalFunc& push_back(e_OptionalFuncCode iFuncCode);
// 	bool remove_last();//**[9/25/2017 ConanSteve]**:  �Ƴ����һ�����������ö���ʹ����֮�󣬵��ö�������Ƴ����һ����������ܷ����쳣
// 	void clear();
// 	int GetFuncNum();//**[9/22/2017 ConanSteve]**:  �����չ�����Ŀ
	CExtenInfo& operator=(const CExtenInfo&);
protected:
	COptionalFunc * p_arrOptionalFunc;
	// {15AC02DB-48C8-499A-A296-3962C7FA8E93}
	GUID  m_guFlag;//**[1/23/2018 ConanSteve]**:  �����Ϊ��ֵ����ô���п��������ݴ��ݴ����£�Ӧ��������ʡ�p_arrOptionalFunc��
	int m_iSizeOfFunc;
	int iCapacityOfFunc;
};
/*************************************<>*************************************/
// 							CExtenInfo extenInfo;
// 							extenInfo.push_back(COptionalFunc(e_Fn_CheckOutPara_AlgCheckErrorDesc).SetArrayTCHAR(_T("����")));
// 							//iReserve4 = (int)&extenInfo;//**[1/12/2018 ConanSteve]**:	extenInfo����Ҫ��ȫ�ֵ�
// 
// 							//**[1/12/2018 ConanSteve]**:	 "CExtenInfoָ��"->GetFuncStruct("�����ĺ궨��").Get*****();
// 							int iDetectTimes = pExtenInfo->GetFuncStruct(e_Fn_CheckInParam_DetectTimes).GetIntEx();
// 							TCHAR* pBuf = pExtenInfo->GetFuncStruct(e_Fn_CheckOutPara_AlgCheckErrorDesc).GetArrayTCHAR();
// 
//							TCHAR(*lpszSaveImagePath)[MAX_PATH] = (TCHAR(*)[MAX_PATH])pExtenInfo->GetFuncStruct(e_Fn_AlgModelInput_SaveImagePath).GetPointer();
// 							CString sPath0 = lpszSaveImagePath[0];//**[1/12/2018 ConanSteve]**:	 ��ô�ͼ·��1
// 							CString sPath1 = lpszSaveImagePath[1];


