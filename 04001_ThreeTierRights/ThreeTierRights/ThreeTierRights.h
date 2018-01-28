#pragma once

#ifdef THREETIERRIGHTS_EXPORTS
#define THREETIERRIGHTS_EXPORTS __declspec(dllexport)
#else
#define THREETIERRIGHTS_EXPORTS __declspec(dllimport)
#endif

enum e_UserType {
	e_UserType_Illegal,
	/*************************************<�û�Ȩ��>************************************[12/28/2017 ConanSteve]*/
	e_UserType_Operator,//**[1/26/2018 ConanSteve]**:	��ͨ����ԱȨ�� 
	e_UserType_Debugger,//**[1/26/2018 ConanSteve]**:	������ԱȨ�� 
	e_UserType_Admin, //**[1/26/2018 ConanSteve]**:	 ����ԱȨ��
	/*************************************<��㼼����ԱȨ��>************************************[12/28/2017 ConanSteve]*/
	e_UserType_SuperAdmin,//**[1/15/2018 ConanSteve]**:  ��������ԱȨ�ޣ����������㷨������Աʹ��
	e_UserType_SystemDeveloper//**[1/26/2018 ConanSteve]**:	ϵͳ������ԱȨ�ޣ���߼�������ʹ��һ�й��� 
};
enum e_DataStoreType {
	e_DataStoreType_Xml,//**[1/15/2018 ConanSteve]**:  ��xml�ļ���ʽ����Ҫ����ļ�����
	e_DataStoreType_Mysql,//**[1/15/2018 ConanSteve]**:  �����ݿ�洢�û���Ϣ
	e_DataStoreType_SQLite3
};

typedef struct s_DatabaseInfo {//**[1/27/2018 ConanSteve]**:	���ݿ�������Ϣ 

	TCHAR host[MAX_PATH];
	TCHAR database[MAX_PATH];
	TCHAR password[MAX_PATH];
}s_DatabaseInfo;

typedef struct s_UserInfoOfThreeTierRights
{
	TCHAR lpszUserName[MAX_PATH];
	TCHAR lpszUserType[MAX_PATH];
	s_UserInfoOfThreeTierRights()
	{
		memset(lpszUserName, 0, sizeof(lpszUserName));
		memset(lpszUserType, 0, sizeof(lpszUserType));
	}
};

enum e_RightType
{
	e_RightType_ThreeTierVerification,//**[1/26/2018 ConanSteve]**:	��Ҫ����Ȩ����֤
	e_RightType_EnableShow//**[1/26/2018 ConanSteve]**:	�����Ŀ��ƿؼ��Ƿ���ʾ����Ҫ��������Ա������Ȩ�޲����޸� 
};

class THREETIERRIGHTS_EXPORTS CThreeTierRights
{
public:
	CThreeTierRights(CWnd* pParent = NULL);
	~CThreeTierRights();
public:
	bool Init(e_DataStoreType  dataStoreType= e_DataStoreType_Xml, s_DatabaseInfo* pDatabaseInfo = NULL);//**[1/15/2018 ConanSteve]**:  ��ʼ����ʽ��Ĭ��
	bool Login(CWnd* pParent = NULL, bool bEnbaleFromDisk = false, bool bHint = false);//**[1/15/2018 ConanSteve]**:  �û���¼���л��û�ʹ��
	bool VerifyRights(e_UserType iUserType, bool bHint = false);//**[1/15/2018 ConanSteve]**:  ��֤Ȩ��
	bool VerifyRights(CWnd* pWnd, e_RightType iRightType = e_RightType_ThreeTierVerification, bool bHint = false);//**[1/15/2018 ConanSteve]**:  ��ȡָ���ؼ���Ȩ��
	void ManageUsers();//**[1/15/2018 ConanSteve]**:  �����û�
	bool GetCurUserInfo(s_UserInfoOfThreeTierRights& rUserInfo);
	bool AddCtrl(CWnd* pWnd, e_RightType iRightType= e_RightType_ThreeTierVerification);//**[1/15/2018 ConanSteve]**:  ��ӿؼ�ID��3��Ȩ�޹������

private:
	void* pInteroir;
};