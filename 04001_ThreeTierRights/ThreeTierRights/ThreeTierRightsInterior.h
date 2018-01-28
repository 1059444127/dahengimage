#pragma once
#include "map"
#include "ExFn.h"
#include "ThreeTierRights.h"
using namespace std;

class CUserInfo
{
public:
	CUserInfo() {}
	CUserInfo(CString sUserName, CString sPassword, e_UserType eUserType, bool bEnable = true)
	{
		this->bEnable = bEnable;
		this->sUserName = sUserName;
		this->sPassword = sPassword;
		this->eUserType = eUserType;
	}
public:
	bool bEnable{ true };
	CString sUserName{_T("")};
	CString sPassword{_T("")};
	e_UserType eUserType{ e_UserType_Illegal };

};

class CThreeTierRightsInterior
{
public:
	CThreeTierRightsInterior();
	CThreeTierRightsInterior(CWnd* pParent);
	~CThreeTierRightsInterior();
public:
public:
	bool Login(CWnd* pParent = NULL, bool bEnbaleFromDisk = false, bool bHint = false);
	bool VerifyRights(e_UserType iUserType, bool bHint = false);//**[1/15/2018 ConanSteve]**:  ��֤Ȩ��
	bool VerifyRights(CWnd* pWnd, e_RightType iRightType = e_RightType_ThreeTierVerification, bool bHint = false);//**[1/15/2018 ConanSteve]**:  ��ȡָ���ؼ���Ȩ��
	void ManageUsers();
	bool GetCurUserInfo(s_UserInfoOfThreeTierRights& rUserInfo);
	bool AddCtrl(CWnd* pWnd, e_RightType iRightType = e_RightType_ThreeTierVerification);//**[1/15/2018 ConanSteve]**:  ��ӿؼ�ָ�뵽3��Ȩ�޹������
	bool ReadConfigFile();
	void SaveConfigFile();
	CString ParseUserType(int iUserType);
	e_UserType ParseUserType(CString sUserType);

public:
	CWnd* m_pParent{ nullptr };
	bool m_bEnable{ false };
	CString m_sCurUsername{ _T("") };//**[11/14/2017 ConanSteve]**:  ��ǰ��¼�û���
	e_UserType m_iCurUserType{ e_UserType_Illegal };
	CString m_sPathFileOfCfg{ _T("") };
	CMarkup m_xml;
	typedef struct s_CtrlInfo {
		e_UserType iUserType;
		TCHAR lpszText[MAX_PATH];
	}s_CtrlInfo;
	map<CString, CUserInfo> m_mapUserInfo;//**[1/15/2018 ConanSteve]**:  �û��б�
	map<CWnd*, s_CtrlInfo> m_map_PWnd2UserType;//**[1/15/2018 ConanSteve]**:  �ؼ�ID��ָָ����Ȩ��
	map<CWnd*, bool> map_PWnd2EnableShow;//**[1/26/2018 ConanSteve]**:	ֻ�ǿ��ƿؼ��Ƿ���ʾ 

// 	map<CString, CUserInfo> m_mapUserInfoOfDebugger;
// 	map<CString, CUserInfo> m_mapUserInfoOfOperator;

};

