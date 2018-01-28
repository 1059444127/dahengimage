#if !defined(AFX_CUSTOMPROPERTIES_H__C3F44405_4572_4B2B_895B_E56C1D8CFEA0__INCLUDED_)
#define AFX_CUSTOMPROPERTIES_H__C3F44405_4572_4B2B_895B_E56C1D8CFEA0__INCLUDED_

#pragma once

#define DHPG_MSG_BTN_CLICKED		WM_USER+3001
#define DHPG_MSG_PROPERTY_CHANGED	WM_USER+3002

typedef struct _DHPROPMSGSTRUCT
{
	int nDllIndex;
	char chGroupName[256];
	int nID;
	char chValue[256];
}s_DHPROGMSGSTRUCT, *P_DHPROGMSGSTRUCT;

/*
s_DHPROGMSGSTRUCT2˵��
�ð汾�ڷ������Ա������Ϣʱ���ѽ�s_DHPROGMSGSTRUCT�ṹ��Ϊs_DHPROGMSGSTRUCT2
�����Ǽ�����Group�����
ͬʱs_DHPROGMSGSTRUCT�ṹ���ɿ���ʹ��
*/
typedef struct _DHPROPMSGSTRUCT2
{
	int nDllIndex;
	char chGroupName[256];
	int nID;
	char chValue[256];

	int nGroupIndex;
	CWnd* pCtrlWnd;
}s_DHPROGMSGSTRUCT2, *P_DHPROGMSGSTRUCT2;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	_EXPORTING
#ifdef _EXPORTING
#define CLASS_DECLSPEC    __declspec(dllexport)  //�������ʱʹ��
#else
#define CLASS_DECLSPEC    __declspec(dllimport)	 //��̬��Ӧ��ʱʹ��
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CLASS_DECLSPEC DHPropGridData
{
public:
	DHPropGridData();
	~DHPropGridData();	

	DHPropGridData(CString*);	//1
	DHPropGridData(int*);		//2
	DHPropGridData(UINT*);		//3
	DHPropGridData(long*);		//4
	DHPropGridData(DWORD*);		//5
	DHPropGridData(float*);		//6
	DHPropGridData(double*);	//7
	DHPropGridData(BYTE*);		//8
	DHPropGridData(short*);		//9
	DHPropGridData(bool*);		//10
	DHPropGridData(LONGLONG*);	//11
	DHPropGridData(CTime*);		//12
	DHPropGridData(char*);		//13
	DHPropGridData(void*);		//99

	void ModifyData(CString);	
	void ModifyData(int);		
	void ModifyData(UINT);		
	void ModifyData(long);		
	void ModifyData(DWORD);		
	void ModifyData(float);		
	void ModifyData(double);	
	void ModifyData(BYTE);		
	void ModifyData(short);		
	void ModifyData(bool);		
	void ModifyData(LONGLONG);	
	void ModifyData(CTime);
	void ModifyData(char*);	
	void ModifyData(void*);		

	BOOL GetData(CString&);
	BOOL GetData(int&);
	BOOL GetData(UINT&);
	BOOL GetData(long&);
	BOOL GetData(DWORD&);
	BOOL GetData(float&);
	BOOL GetData(double&);
	BOOL GetData(BYTE&);
	BOOL GetData(short&);
	BOOL GetData(bool&);
	BOOL GetData(LONGLONG&);
	BOOL GetData(CTime&);
	BOOL GetData(char*);
	BOOL GetData(void*);

	int GetDataType();

protected:
	void* m_pData;
	int m_nDataType;
	void FreeData();
};

class CLASS_DECLSPEC CDHPropertyGridCtrl
{
public:
	CDHPropertyGridCtrl(void);
	~CDHPropertyGridCtrl(void);

	int InstallControl(CWnd* pParentDlg, const CRect rcPos, const char* chConfigMidName, const int nCameraIndex);
	/*
		pParentDlg:			������ָ��
		rcPos:				λ��
		strConfigMidName:	��ʾ���� - �����ļ����������Ϊ"DHPropGridLayoutConfig %s%d.ini";
		nCameraIndex:		�ڼ�· ��1��ʼ

		AppName��߼���VL �ͱ�ΪValueList ����[��λVL]
		AppName�������ظ������ߵĽ��޷����뵽�ؼ���

		������		       CtrlDesc
		EditCtrl0/1/2	:  CtrlRangeMin CtrlRangeMax CtrlPrecision/CtrlTextLen  int/double/char
			intĬ��-2147483647��2147483647
			doubleĬ��-99999.0��99999.0 С����λ������Ϊ2 ���Ϊ5
			charĬ���255
		Button			:  CtrlValue
		CheckButton		:  
		SliderCtrl		:  CtrlRangeMin CtrlRangeMax
		ComboBox		:  CtrlValue(�м��� ?*%? ����)
	*/
	int SetColumnWidth(const int nVal);
	int SetRowHeight(const int nVal);
	int DisplayCtrl(const int nCmdShow);
	int SetFont(LOGFONT& lFont);

	//EnableCtrl�ļ��ַ���
	int EnableCtrl(int nDllIndex, char* chGroupName, int nCtrlIndex, BOOL bEnable = TRUE);
	int EnableCtrl(int nDllIndex, char* chGroupName, char* chCtrlName, BOOL bEnable = TRUE);
	int EnableCtrl(int nDllIndex, const char* chGroupName, const char* chCtrlName, BOOL bEnable = TRUE);
	int EnableCtrl(int nDllIndex, int nGroupIndex, int nCtrlIndex, BOOL bEnable = TRUE);
	/*
	nDllIndex��Ϊ-1��������Ч
	chGroupName��ΪNULL������DllIndex�¶���Ч
	nCtrlIndex��Ϊ-1������Group����Ķ���Ч �������ļ���CtrlName��ߵ����
	*/
	int MoveWindow(const CRect rect, BOOL bRepaint = 1);
	int ReadModel(const char* chModelName);	//��ʱû��
	int WriteModel(const char* chModelName);//��ʱû��


	//Update�ļ��ַ���
	int UpdatePara(const int nDllIndex, const char* chGroupName, const int nCtrlIndex, DHPropGridData& Data, BOOL bGetValue = TRUE);
	int UpdatePara(const int nDllIndex, const int nGroupIndex, const int nCtrlIndex, DHPropGridData& Data, BOOL bGetValue = TRUE);
	/*
	nCtrlIndex �������ļ���CtrlName��ߵ����
	nGroupIndex ���������ļ������� ���û���� ����0��ʼ˳�����
	*/
	int UpdatePara(const int nDllIndex, const char* chGroupName, const int nCtrlIndex, int& nValue, BOOL bGetValue = TRUE);
	int UpdatePara(const int nDllIndex, const char* chGroupName, const int nCtrlIndex, double& dValue, BOOL bGetValue = TRUE);
	int UpdatePara(const int nDllIndex, const char* chGroupName, const int nCtrlIndex, char* chValue, BOOL bGetValue = TRUE);
	int UpdatePara(const int nDllIndex, const char* chGroupName, const int nCtrlIndex, float& fValue, BOOL bGetValue = TRUE);

	int Do(int, char*, void*, void*, void*);
	/*
	int = 1: �����ؼ� ����void*����Ϊ char* Color, int* nDllIndex, int* nGroupIndex, int* nCtrlIndex
		  2: ������� ����void*����Ϊ int* nDllIndex, int* nGroupIndex, int* nCtrlIndex
		  3: չ������ ����void*����Ϊ int* nDllIndex, int* nGroupIndex
		  4: �����༭��
		  5: ����CurrentSelection
		  6: ��ȡColumnWidth
	*/
protected:
	void* m_pPropGridCtrl;
};

#endif // !defined(AFX_CUSTOMPROPERTIES_H__C3F44405_4572_4B2B_895B_E56C1D8CFEA0__INCLUDED_)

