#pragma once
#include "windows.h"
#include "afx.h"
#include "vector"
#include "array"
#include "memory"
#include "Markup.h"
#include "algorithm"
#include "numeric"
using namespace std;

#define MAX_BUF_LEN 1024

#ifndef tr(quote)
	#define tr(quote) _T(quote)
#endif

/*************************************<��չ������>*************************************/
class CExFn
{
public:
	CExFn();
	~CExFn();
public:
	//**[8/4/2017 ConanSteve]**:  �ӿ�
	static BOOL ShowInTaskbar(HWND hWnd, BOOL bShow);//**[8/4/2017 ConanSteve]**:  ���ô��ڵ�������ͼ���Ƿ���ʾ
	static BOOL SetWindowPosTop(HWND hwnd);//**[8/4/2017 ConanSteve]**:  ���ô����ö�
	static CString GetCurrentAppPath(); //**[8/7/2017 ConanSteve]**:  ��ȡ��ǰ��������·��	
	//**[9/5/2017 ConanSteve]**:  ���ҵ�ǰ·����������Ŀ¼��ָ����ʽ�ļ�
	//**[1/5/2018 ConanSteve]**:	iType=1,ֻ�����ļ���iType =2ֻ�����ļ��У�iType=3,������ 
	static bool FindSpecificFiles(LPCTSTR lpszPathFolder, LPCTSTR lpszFilter, vector<CString>& vec_sPathFile, vector<CString>& vec_sFileName, bool bSearchSub = true, int iType = 3, CString* pError = nullptr);
	static BOOL DeleteDirectory(LPCTSTR lpszPath, LPCTSTR lpszType = _T("*.*"), bool bIncludeFolders = true);//**[8/7/2017 ConanSteve]**:  ����ɾ����ǰ�ļ��м�������Ŀ¼�ļ�
	static bool CreateMultiDirectory(LPCTSTR lpszPath);//**[8/25/2017 ConanSteve]**:  �����༶Ŀ¼�ļ���
	static BOOL CopyChildFiles(LPCTSTR lpszExistingFileName, LPCTSTR lpszNewFileName, BOOL bFailIfExists, CString* psErrorDesc = NULL);//**[1/9/2018 ConanSteve]**:  ���������ļ����е������ļ�
	//**[1/11/2018 ConanSteve]**:  https://www.cnblogs.com/huhu0013/p/4617767.html
	static BOOL IsDirectory(LPCTSTR pstrPath);
	static BOOL CopyFolder(LPCTSTR pstrSrcFolder, LPCTSTR pstrDstFolder);

	//**[1/25/2018 ConanSteve]**:	��ȡֵ 
	static bool GetPrivateProfileBool(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool bDefaultValue, LPCTSTR lpszFileName, LPCTSTR lpszAnnotation = NULL);
	static int GetPrivateProfileInt(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int iDefaultValue, LPCTSTR lpszFileName, LPCTSTR lpszAnnotation = NULL);
	static CString& GetPrivateProfileString(_In_opt_ LPCTSTR lpAppName, _In_opt_ LPCTSTR lpKeyName, CString &sReturnString, _In_opt_ LPCTSTR lpFileName, _In_opt_ LPCTSTR lpDefault = NULL, LPCTSTR lpszAnnotation = NULL);//**[9/1/2017 ConanSteve]**: 
	//**[1/25/2018 ConanSteve]**:	��ȡֵ����д��ֵ 
	static bool GetPrivateProfileBoolEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool bDefaultValue , LPCTSTR lpszFileName, LPCTSTR lpszAnnotation = NULL);
	static int GetPrivateProfileIntEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int iDefaultValue, LPCTSTR lpszFileName, LPCTSTR lpszAnnotation = NULL);
	static CString& GetPrivateProfileStringEx(_In_opt_ LPCTSTR lpAppName,_In_opt_ LPCTSTR lpKeyName,CString &sReturnString,_In_opt_ LPCTSTR lpFileName,_In_opt_ LPCTSTR lpDefault=NULL, LPCTSTR lpszAnnotation = NULL);//**[9/1/2017 ConanSteve]**: 
	
	static bool WritePrivateProfileBool(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool bKeyValue, LPCTSTR lpszFileName, LPCTSTR lpszAnnotation = NULL);
	static bool WritePrivateProfileInt(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int iKeyValue, LPCTSTR lpszFileName,LPCTSTR lpszAnnotation=NULL);
	static bool WritePrivateProfileString(LPCTSTR lpszAppName, LPCTSTR lpKeyName, LPCTSTR lpszValue, LPCTSTR lpszFileName, LPCTSTR lpszAnnotation = NULL);
	static CString Int2CS(int iVal);
	static CString GetLastErrorMsg();//**[9/13/2017 ConanSteve]**:  ���GetLastError���صĴ������Ӧ���ַ�����Ϣ

	//**[1/25/2018 ConanSteve]**:	���ݿ�߱������������δ�С��λ�� 
	static CRect AutoResizeRect(int iWidth, int iHeight, CRect rect);
	//**[1/25/2018 ConanSteve]**:	 �����Ӵ��ڵĴ�Сλ��
	static vector<CRect> AutoLayoutRect(CRect rectParent, int iCntHorizon, int iCntVertical, int iGapHorizon,int iGapVertical, int iChildNum, int iType=1, CRect* pRect = NULL);
};
/*************************************<CStringConvert�ַ�ת����>************************************[9/14/2017 ConanSteve]*/
enum e_EncodeFormat {
	e_EncodeFormat_Error = 0,
	e_EncodeFormat_ANSI_GB2312 = 1,
	e_EncodeFormat_UTF8 = 2,
	e_EncodeFormat_UTF8_BOM,
	e_EncodeFormat_UCS2_BE_BOM,
	e_EncodeFormat_UCS2_LE_BOM
};

class CStringConvert {
private:
	CStringConvert() {

	}
	~CStringConvert() {

	}
public:
	//**[7/21/2017 ConanSteve]**:  ANSI�洢תUnicode
private:
	static wstring MBCS_UCS2(string& str, int iCodePage);
public:
	static wstring ANSI_UCS2(string& str);
	static wstring ANSI_UCS2(const char* ch);
	static wstring UTF8_UCS2(string& str);//**[7/21/2017 ConanSteve]**:  UTF8�洢תUnicode
	static wstring UTF8_UCS2(char* ch);
	static wstring GB2312_UCS2(string& str);

private:
	static string UCS2_MBCS(wstring& wstr, int codePage);
public:
	static string UCS2_ANSI(wstring& wstr);
	static string UCS2_ANSI(wchar_t* wch);
	static string UCS2_UTF8(wstring& wstr);
	static string UCS2_UTF8(wchar_t* wch);

public:
	static string ANSI_UTF8(string &str);
	static string ANSI_UTF8(char* ch);
	static string UTF8_ANSI(string &str);
	static string UTF8_ANSI(char* ch);
	//	static string Int2UTF8(int nVal);
	//	static string WCHAR_UTF8(wchar_t* pwchData, int nLen = -1);


public:
	static bool IsTextUTF8(const char* str, long length);
	static e_EncodeFormat GetEncodeFormat(wstring& wsFilePathName);
	static e_EncodeFormat GetEncodeFormat(wchar_t* pwszFilePathName);
	static e_EncodeFormat GetEncodeFormat(string& sFilePathName);
	static e_EncodeFormat GetEncodeFormat(char* pszFilePathName);
	//static string WCHAR_ANSI(CString csPara);
	//static string CS2UTF8(CString csData);
	//static string Double2UTF8(double dbVal);
	//static int CS2Int(CString csPara);
	//static double CS2Double(CString csPara);
};
//**[8/3/2017 ConanSteve]**:  GBK��code page��CP936 ��BIG5��code page��CP950��GB2312��code page��CP20936
/*************************************<��дXML�����ļ���>************************************[9/14/2017 ConanSteve]*/
enum  VarType {
	__no,
	__bool,
	__int,
	__float,
	__DWORD,
	__CString,
};
typedef struct NodeVar
{
	VarType varType;//**[9/14/2017 ConanSteve]**:  ��������
	CString sAppName;
	CString sKeyName;
	union {
		bool *pbBool;
		int *piInt;
		float* pfFloat;

		CString* psCString;
	}ptr;
	CString sAnnotation;
	NodeVar()
	{
		varType=VarType::__no;//**[9/14/2017 ConanSteve]**:  ��������
		sAppName= _T("") ;
		sKeyName= _T("") ;
		sAnnotation =_T("") ;
		memset(&ptr, 0, sizeof(ptr));
	}
}NodeVar;
class CXMLCfg
{
public:
	CXMLCfg(LPCTSTR lpszFilePath= NULL);
	~CXMLCfg();
	bool Load(LPCTSTR lpszFilePath);
	bool Init(LPCTSTR lpszFilePath);//**[9/14/2017 ConanSteve]**:  ��ʼ�������ļ�·��
	bool UpdateData();//**[9/14/2017 ConanSteve]**:  ����������д��xml�ļ�
	CString GetStringEx(_In_opt_ LPCTSTR lpszAppName, _In_opt_ LPCTSTR lpszKeyName, LPCTSTR lpszDefaultValue = NULL, LPCTSTR lpszAnnotation = NULL);//**[9/1/2017 ConanSteve]**:
	//CString& GetStringEx(_In_opt_ LPCTSTR lpszAppName, _In_opt_ LPCTSTR lpszKeyName, CString &sRetKeyValue, LPCTSTR lpszDefaultValue=NULL, LPCTSTR lpszAnnotation = NULL);//**[9/1/2017 ConanSteve]**:
	CString& GetStringEx(_In_opt_ LPCTSTR lpszAppName, _In_opt_ LPCTSTR lpszKeyName, CString &sRetKeyValue, LPCTSTR lpszDefaultValue = NULL, LPCTSTR lpszAnnotation = NULL);
	

	bool WriteBool(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool bKeyValue, LPCTSTR lpszAnnotation = NULL);
	bool WriteString(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszValue,LPCTSTR lpszAnnotation = NULL);
	int GetInt(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int iDefaultValue = 0, LPCTSTR lpszAnnotation = NULL);
	bool WriteInt(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int iKeyValue, LPCTSTR lpszAnnotation = NULL);
	bool GetBool(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool bDefaultValue = false, LPCTSTR lpszAnnotation = NULL);


	bool GetBoolEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool& bRetKeyValue, bool bDefaultValue = false, LPCTSTR lpszAnnotation = NULL);
	int& GetIntEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int& iKeyValue, int iDefaultValue, LPCTSTR lpszAnnotation = NULL);//**[9/14/2017 ConanSteve]**:  �󶨱�����������UpdateDate����ʱ�����Զ�д�뱾�ε��ð󶨵ı���
	CString GetStringEx(vector<CString>& vecNodePath, LPCTSTR lpszKeyName, LPCTSTR lpszDefaultValue=NULL);
	CString& GetStringEx(vector<CString>& vecNodePath, LPCTSTR lpszKeyName, CString& sRetKeyValue, LPCTSTR lpszDefaultValue = NULL);
	
	//**[9/14/2017 ConanSteve]**:  д���������ã�ע��Ҳ��һ������
	CString& GetAttrString(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszAttrName, CString &sAttrValue);
	CString& GetAttrStringEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszAttrName, CString &sAttrValue);
	bool WriteAttrString(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszAttrName, LPCTSTR lpszAttrValue);
	int& GetAttrInt(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszAttrName, int& iAttrValue);
	int& GetAttrStringEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszAttrName, int& iAttrValue);
	bool WriteAttrInt(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszAttrName, int iAttrValue);


protected:
	bool m_bOpenFile;
	CMarkup m_xml;
	CString m_sPathFile;
	vector<NodeVar> m_vecNodeVar;//**[9/14/2017 ConanSteve]**:  �󶨵ı�������������ʱд��

};
/*************************************<CINICfg>************************************[9/14/2017 ConanSteve]*/
class CINICfg
{
public:
	//**[1/25/2018 ConanSteve]**:	��ʼ���͹��캯�� 
	CINICfg(LPCTSTR lpszFilePath = NULL);
	~CINICfg();
	bool Load(LPCTSTR lpszFilePath = NULL);
	bool Init(LPCTSTR lpszFilePath = NULL);
	bool UpdateData();

	bool GetBool(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool bDefaultValue = false, LPCTSTR lpszAnnotation = NULL);
	int GetInt(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int iDefaultValue = -1, LPCTSTR lpszAnnotation = NULL);
	double GetDouble(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, double fDefaultValue = 0, LPCTSTR lpszAnnotation = NULL);
	CString& GetString(_In_opt_ LPCTSTR lpszAppName, _In_opt_ LPCTSTR lpszKeyName, CString &sReturnString, _In_opt_ LPCTSTR lpszDefault = NULL, LPCTSTR lpszAnnotation = NULL);//**[9/1/2017 ConanSteve]**: 


	//**[1/25/2018 ConanSteve]**:	дֵ 
	bool WriteBool(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool bKeyValue, LPCTSTR lpszAnnotation = NULL);
	bool WriteInt(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int iKeyValue, LPCTSTR lpszAnnotation = NULL);
	bool WriteDouble(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, double lfValue, LPCTSTR lpszAnnotation = NULL);
	bool WriteString(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszAnnotation = NULL);


	//**[1/25/2018 ConanSteve]**:	��ȡֵ����д��Ĭ��ֵ

	bool GetBoolEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool bDefaultValue = false, LPCTSTR lpszAnnotation = NULL);
	int GetIntEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int iDefaultValue = -1, LPCTSTR lpszAnnotation = NULL);
	double GetDoubleEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, double fDefaultValue = 0, LPCTSTR lpszAnnotation = NULL);
	CString& GetStringEx(_In_opt_ LPCTSTR lpszAppName, _In_opt_ LPCTSTR lpszKeyName, CString &sReturnString, _In_opt_ LPCTSTR lpszDefault = NULL, LPCTSTR lpszAnnotation = NULL);//**[9/1/2017 ConanSteve]**: 

	//**[9/14/2017 ConanSteve]**:  �󶨱�������������ʱ�����Զ�д�뱾�ε��ð󶨵ı���
	int& GetIntEx2(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int& iKeyValue, int iDefaultValue = -1, LPCTSTR lpszAnnotation = NULL);
	bool GetBoolEx2(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool& bRetKeyValue, bool bDefaultValue = false, LPCTSTR lpszAnnotation = NULL);
	CString& GetStringEx2(_In_opt_ LPCTSTR lpszAppName, _In_opt_ LPCTSTR lpszKeyName, CString &sReturnString, _In_opt_ LPCTSTR lpszDefault = NULL, LPCTSTR lpszAnnotation = NULL);

protected:
	bool m_bOpenFile;
	CString m_sPathFile;
	vector<NodeVar> m_vecNodeVar;//**[9/14/2017 ConanSteve]**:  �󶨵ı�������������ʱд��
};

/*************************************<��־��¼��>*************************************/
class CLogFile {
public:
	CLogFile(LPCTSTR lpFilePath = NULL);
	~CLogFile();
	//**[9/6/2017 ConanSteve]**:  bWrite:�Ƿ����־��¼
	bool Open(LPCTSTR lpFilePath,bool* pbWrite = nullptr, int iFlag = CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone);
	bool Open(LPCTSTR lpFilePath, int iFlag);
	bool Close();
	bool Write(LPCTSTR lpLogInfo);
	//bool Write(const TCHAR* fmt, ...);
	void Flush();
	bool SetMaxLine(int iMaxLine);
	bool SeekToBegin();
private:
	bool m_bOpenFile;
	bool* m_pbWrite;
	int m_iCurLine;//**[8/30/2017 ConanSteve]**:  ��ǰ��¼����
	int m_iMaxLine;//**[8/30/2017 ConanSteve]**:  ���ɼ�¼����
	unsigned __int64 m_iLastTick;//**[9/5/2017 ConanSteve]**:  ��һ��ʱ�Ӽ���
	unsigned __int64 m_iFrequency;//**[9/5/2017 ConanSteve]**:  �����CPUƵ��
	//CRITICAL_SECTION m_csWrite;
	CFile m_file;
};

/*************************************<CPUʹ������>*************************************/
class CCPUUsage
{
private:
	typedef long long          int64_t;
	typedef unsigned long long uint64_t;
	HANDLE _hProcess;
	int _processor;    //cpu����    
	int64_t _last_time;         //��һ�ε�ʱ��    
	int64_t _last_system_time;
	    
	uint64_t file_time_2_utc(const FILETIME* ftime);// ʱ��ת��	 
	int get_processor_number();// ���CPU�ĺ���   	  
	void init(); //��ʼ�� 
	void clear();//�رս��̾��


public:
	CCPUUsage(DWORD ProcessID);
	CCPUUsage();
	~CCPUUsage();
	//����ֵΪ���̾�������ж�OpenProcess�Ƿ�ɹ�  
	HANDLE setpid(DWORD ProcessID);
	//-1 ��Ϊʧ�ܻ�������˳��� ����ɹ����״ε��û᷵��-2����;��setpid������PID���״ε���Ҳ�᷵��-2��  
	float get_cpu_usage();
};

class CLayout 
{
public:
	CLayout() {}
	CLayout(LPCTSTR lpszInitFilePath, CRect& rectParent) { Init(lpszInitFilePath, rectParent); }
	~CLayout();
	bool Init(LPCTSTR lpszInitFilePath, CRect& rectParent);//**[10/10/2017 ConanSteve]**:  lpszInitFilePath��XML�����ļ�·��  rectParent:�����ڵ�λ����Ϣ
	bool GetPosition(LPCTSTR lpszWndName, CRect& rectWnd);//**[10/10/2017 ConanSteve]**:  ��õ�ǰ���ڵĲ���λ��
	CString GetAttrib(LPCTSTR lpszWndName, LPCTSTR lpszAttrName, CString& sRetVal);
	int GetAttrib(LPCTSTR lpszWndName, LPCTSTR lpszAttrName, int iDefaultVal = 0);
private:
	bool FindFixedWnd(LPCTSTR lpszWndName, CRect& rectWnd);//**[10/10/2017 ConanSteve]**:  Ѱ�ҹ̶����ڵ�λ����Ϣ
	bool FindDynamicWnd(LPCTSTR lpszWndName, CRect&	rectWnd);//**[10/10/2017 ConanSteve]**:  Ѱ�Ҷ�̬���ڵ�λ����Ϣ
	bool ParseCurNode(LPCTSTR lpszWndName,CRect rectParent, CRect& rectWnd);
	array<int, 4>& ParsePadding(LPCTSTR lpszPadding, array<int, 4>& arrPadding);
	bool m_bInitSuccess;
	CRect m_rectParent;//**[10/10/2017 ConanSteve]**:  �����ڵ�λ����Ϣ
	CString m_sInitFilePath;
	CMarkup m_xml;

};

