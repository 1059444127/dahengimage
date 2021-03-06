#include "stdafx.h"

#define VAR_DECLS 
#include "IDS.h" 

void CIDS::ResetDefault()
{
	m_mapIDS.clear();
	bDefault = true;
}

bool CIDS::Init(TCHAR* const tcFilePath)
{
	return Init(CString(tcFilePath));
}
bool CIDS::Init(CString const &sFilePath)
{
	m_mapIDS.clear();
	if (PathFileExists(sFilePath))
	{
		m_sLangFilePath = sFilePath;
		bPathFileExist = true;
		bDefault = false;
	}
	else
	{
		bPathFileExist = false;
		bDefault = true;
	}
	return bPathFileExist;
}
CString CIDS::Translate(TCHAR* const tchar)
{
	CString sRet{ tchar };
	//*[7/3/2017 ConanSteve]*:  如果路径存在
	if (!bDefault)
	{
		if (bPathFileExist)
		{
			auto itera_find = m_mapIDS.find(tchar);
			if (m_mapIDS.end() != itera_find)
			{
				sRet = itera_find->second;
			}
			else
			{
				GetPrivateProfileString(m_sModuleName, sRet, sRet, sRet.GetBuffer(1024), 1024, TEXT("D:\\Workspace\\Projects\DahengProjects\Demo\SRC\Language\Chinese(Simplified).ini"));
				sRet.ReleaseBuffer();
				if (TEXT("") == sRet)
					sRet = tchar;
				m_mapIDS.insert(std::make_pair((CString)tchar, sRet));
			}
		}
	}
	return sRet;

}
void CIDS::SetAppName(CString sModuleName)
{
	m_sModuleName = sModuleName;
}
//CIDS theIDS;