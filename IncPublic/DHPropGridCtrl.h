#if !defined(AFX_CUSTOMPROPERTIES_H__C3F44405_4572_4B2B_895B_E56C1D8CFEA0__INCLUDED_)
#define AFX_CUSTOMPROPERTIES_H__C3F44405_4572_4B2B_895B_E56C1D8CFEA0__INCLUDED_

#pragma once

#define DHPG_MSG_BTN_CLICKED		WM_USER+3001
#define DHPG_MSG_PROPERTY_CHANGED	WM_USER+3002
//#define DHPG_MSG_PARA_CLICKED      WM_USER+3003

#define MAX_SUB_ALG_NUM 15
#define MAX_PARA_NUM 200   //�����ڴ����Ĳ�������

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

//���б�������ݸ��µı������ͣ������ı�����������ת��Ϊ�������ٽ������ݸ���
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
//***�¼����ڴ�����
//�����ڴ����ƣ�Format(ShareMemory_%d",m_nCamIndex)
//д�ڴ�����¼����ƣ�Format("ShareEvent_%d",m_nCamIndex)
//�����ڴ��С:sizeof(SharedMemory)*MAX_PARA_NUM
//****//
struct SharedMemory//�����ڴ�ṹ��
{   
    int nChangedNum;  //�����ڴ��д˽ṹ����Ч�����ڵ�һ���ṹ����
	int nCamIndex;//�����
	int nParaIndex;//�б��
    char strDllName[32];//��̬����
	char strGroupName[32];//����
	char strCtrlName[32];//�ؼ���
	int nType;//�������� 0 ������  1�ַ���
	char strValueOld[256];//�ַ��� �ϱ���ֵ
	char strValueNew[256];//�ַ��� �±���ֵ
	double dValue[2]; //dValue[0]�������±���ֵ   dValue[1]�������ϱ���ֵ
};


class CLASS_DECLSPEC CDHPropGridCtrl
{
public:
	CDHPropGridCtrl(void);
	~CDHPropGridCtrl(void);

	
	//�����Ƿ�󶨲����ļ���־bIsOperateFile������Ҫ�󶨣��ѱ�־λ��Ϊ1��
	//����ָ�������ļ�·���Ľӿ�chFileSectionName����ʽӦΪ���£�"file1,section;file2,section;file3,section" 
	//�ֺŸ��ϵ��ǲ�ͬ�Ӷ�̬�⣬���Ÿ��ϵ���һ����̬���еĲ����ļ�����APP�ֶΡ�һ���Ӷ�̬��ֻ�ܶ�Ӧһ��file,section
	//INI�ļ���·����������,�ļ���ǰ��ĸ���Ŀ¼�������,����д�벻�ɹ�,�ú������� FALSE ֵ.
	//�ļ�����·���б���Ϊ \\ ,��Ϊ��VC++��, \\ �ű�ʾһ�� \ .
	//Ҳ�ɽ�INI�ļ����ڳ�������Ŀ¼,��ʱ file ����Ϊ: ".\\name.ini".
	//��ȡ�뱣����� ����ReadModel() WriteModel(),��ʼ��ʱ���Զ�����ReadModel()
	int InstallControl(CWnd* pParentDlg, const CRect rcPos, const char* chConfigMidName, const int nCameraIndex=0,const int nParaIndex=0,bool bIsOperateFile=false,const char* chFileSectionName="");
	/*
		pParentDlg:			������ָ��
		rcPos:				λ��
		strConfigMidName:	 �б������ļ����� ĩβ����Ҫ��".ini" 
		nCameraIndex:		�ڼ�· ��1��ʼ
		nParaIndex:       �ڼ��������б�  ��1��ʼ
		///ʧЧ  AppName��߼���VL �ͱ�ΪValueList ����[��λVL]
		bIsOperateFile:      �Ƿ���������ļ���־λ
		chFileSectionName:    �����ļ�·������������AppName�ӿڡ����ݷ�ʽΪ"file1,section;file2,section;file3,section" �ֺŸ��ϵ��ǲ�ͬ�Ӷ�̬�⣬���Ÿ��ϵ���һ����̬���еĲ����ļ�����APP�ֶΡ�һ���Ӷ�̬���Ӧ
		һ��file��sectiion

	//*******************************INI�ļ����ù���******************************	
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

		�󶨱�����ص������Ӳ���:
		[������Ϣ]������
		DefaultCtrlFileName=Default/realfilepath/NULL//      realfilepath��ʽ�磺 *\\*\\*.ini  
        DefaultCtrlSectionName=Default/realsectionname/NULL//                realsectionname��ʽ�磺������   
		
        ini�������������ӣ�    
		*_CtrlFileName=Default/Interface/realfilepath/NULL//
        *_CtrlSectionName=Default/Interface/realsectionname/NULL//
        *_CtrlKeyName=Default/realkeyname/NULL//                            realkeyname��ʽ�磺���
		
		//*****************************FileName*****************************
		*_CtrlFileNameΪDefaultʱ����ȡֵ��ʹ��DefaultCtrlFileName�ļ�ֵ����ʱ��DefaultCtrlFileNameΪDefaultʱ��*_CtrlFileName��ʹ��run\ModelInfo\($GridName)para.ini��Ϊ�����Ĳ����ļ�����             
		                                                                  ��DefaultCtrlFileNameΪrealfilepathʱ��*_CtrlFileName��ʹ��realfilepath��Ϊ�����ļ���
																		  ��DefaultCtrlFileNameΪNULLʱ������*_CtrlFileNameֵΪDefault�Ĳ��������б���������
		*_CtrlFileNameΪInterfaceʱ*_CtrlFileName��ʹ��InstallControl�ӿ���chFileSectionName�������������ļ�����
		*_CtrlFileNameΪrealfilepathʱ��*_CtrlFileName��ʹ��realfilepath��Ϊ�����ļ���
		*_CtrlFileNameΪNULLʱ���˿ؼ������б���������
		
		//****************************SectionName*********************************
		*_CtrlSectionNameΪDefaultʱ����ȡֵ��ʹ��DefaultCtrlSectionName�ļ�ֵ����ʱ��DefaultCtrlSectionNameΪDefaultʱ��*_CtrlSectionName��ʹ�õ�ǰ�������ڲ����б��е�������Ϊ�����������           
		                                                                  ��DefaultCtrlSectionNameΪrealsectionnameʱ��*_CtrlSectionName��ʹ��realsectionname��Ϊ����
																		  ��DefaultCtrlSectionNameΪNULLʱ������*_CtrlSectionNameֵΪDefault�Ĳ��������б���������
		*_CtrlSectionNameΪInterfaceʱ*_CtrlSectionName��ʹ��InstallControl�ӿ���chFileSectionName�������������ļ�����
		*_CtrlSectionNameΪrealsectionnameʱ��*_CtrlSectionName��ʹ��realsectionname��Ϊ�����ļ���
		*_CtrlSectionNameΪNULLʱ���˿ؼ������б���������
		
		//*****************************KeyName************************************
		*_CtrlKeyNameΪDefaultʱ��*_CtrlKeyName��ʹ�õ�ǰ�����ڲ����б��е�������Ϊ��������
		*_CtrlKeyNameΪrealkeynameʱ��*_CtrlKeyName��ʹ��realkeyname��Ϊ������
		*_CtrlKeyNameΪNULLʱ���˿ؼ������б���������
		
	
		�ؼ���ʾ��������Ӳ�����
		�����������   *_CtrlIsVisible=0/1 ��ʾ�˲����Ƿ������ʾ
		����߶�̬�������� IsVisible=0/1   ��ʾ�����̬���Ƿ������ʾ
		
		

	*/
	int SetColumnWidth(const int nVal);  //�趨�б����������еĿ��
	int SetRowHeight(const int nVal);    //�趨�б�ÿ�еĸ߶�
	int DisplayCtrl(const int nCmdShow); //�Ƿ���ʾ�б�
	int SetFont(LOGFONT& lFont);        //�趨�б�����������

	//EnableCtrl�ļ��ַ���
	int EnableCtrl(int nDllIndex, char* chGroupName, int nCtrlIndex, BOOL bEnable = TRUE);
	int EnableCtrl(int nDllIndex, char* chGroupName, char* chCtrlName, BOOL bEnable = TRUE);
	int EnableCtrl(int nDllIndex, const char* chGroupName, const char* chCtrlName, BOOL bEnable = TRUE);
	int EnableCtrl(int nDllIndex, int nGroupIndex, int nCtrlIndex, BOOL bEnable = TRUE);
	/*
	nDllIndex��Ϊ-1��������Ч
	chGroupName��Ϊ-1������DllIndex�¶���Ч
	nCtrlIndex��Ϊ-1������Group����Ķ���Ч �������ļ���CtrlName��ߵ����
	*/
	int MoveWindow(const CRect rect, BOOL bRepaint = 1);


	int ReadModel();//�Ӳ����ļ��ж�ȡ����ˢ�µ�������
	int WriteModel();//�����������ϵĲ���ֵд�뵽�����ļ���


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

