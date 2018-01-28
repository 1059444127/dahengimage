#pragma once

#ifdef THREETIERRIGHTS_EXPORTS
#define THREETIERRIGHTS_EXPORTS __declspec(dllexport)
#else
#define THREETIERRIGHTS_EXPORTS __declspec(dllimport)
#endif

enum e_UserType {
	e_UserType_Illegal,
	e_UserType_Operator,
	e_UserType_Debugger,
	e_UserType_Admin,
	e_UserType_SuperAdmin//**[1/15/2018 ConanSteve]**:  ������ԱȨ�ޣ�
};
enum e_DataStoreType {
	e_DataStoreType_Xml,//**[1/15/2018 ConanSteve]**:  ��xml�ļ���ʽ����Ҫ����ļ�����
	e_DataStoreType_Mysql//**[1/15/2018 ConanSteve]**:  �����ݿ�洢�û���Ϣ
};

typedef struct s_DatabaseInfo {
	TCHAR host[MAX_PATH];
	TCHAR database[MAX_PATH];
	TCHAR password[MAX_PATH];
	TCHAR tableName[MAX_PATH];
}s_DatabaseInfo;

class THREETIERRIGHTS_EXPORTS CThreeTierRights
{
public:
	CThreeTierRights(CWnd* pParent=NULL);
	~CThreeTierRights();
public:
	bool Init(e_DataStoreType  dataStoreType= e_DataStoreType_Xml, s_DatabaseInfo* pDatabaseInfo = NULL);//**[1/15/2018 ConanSteve]**:  ��ʼ����ʽ��Ĭ��
	bool Login(CWnd* pParent = NULL, bool bHint = false);//**[1/15/2018 ConanSteve]**:  �û���¼���л��û�ʹ��
	bool VerifyRights(e_UserType iUserType, bool bHint = false);//**[1/15/2018 ConanSteve]**:  ��֤Ȩ��
	bool VerifyRights(CWnd* pWnd, bool bHint = false);//**[1/15/2018 ConanSteve]**:  ��ȡָ���ؼ���Ȩ��
	void ManageUsers();//**[1/15/2018 ConanSteve]**:  �����û�
	bool AddCtrl(CWnd* pWnd);//**[1/15/2018 ConanSteve]**:  ��ӿؼ�ID��3��Ȩ�޹������

private:
	void* pInteroir;
};