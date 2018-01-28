/************************************************************************/
/* Copyright (c) 2005, �������ͼ���Ӿ����޹�˾�Ӿ�ϵͳ��     
/* All rights reserved.													
/*																		
/* �ļ����ƣ� lLogManager.cpp												
/* �ļ���ʶ�� 
/* ժҪ�� ���漰��־�ļ������ĸ��ֺ������л���, 
/*    
/*
/* ��ǰ�汾�� 1.1
/* ���ߣ� ����
/* ������ڣ� 2009��8��17��
/************************************************************************/

#include "stdafx.h"
#include "LogManager.h"
#include <afxtempl.h>//Ϊ��ʹ��CList


CLogManager::CLogManager()
{	
	m_strNext="_next";
	m_strLastManagerError="";
}
CLogManager::~CLogManager()
{
	
}
//����һ��Ŀ¼��������ڲ�����
BOOL CLogManager::MyCreatePath(const CString &strPath)
{
	WIN32_FIND_DATA fd; 
	HANDLE hFind = FindFirstFile(strPath, &fd); 
	if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{ 
		FindClose(hFind);
		return true; 
	} 
	FindClose(hFind); 
	
	return CreateDirectory(strPath, NULL);
}



//д��־�ļ�
BOOL CLogManager::MyWriteLogFile(
						 const CString& strSession, 
						 const CString& strContent,
						 int   iLogType)
{
	//����Logs�ļ�Ŀ¼��ini�ļ���
	CString strIniPath, strIniFileName, strTime;
	CTime curTime = CTime::GetCurrentTime();
	DWORD dwTime = GetTickCount()%1000;
	CString strLogFilePath=GetCurrentAppliationPath();

	static int iCount_Operation=0;// ����д���������� [8/18/2009 GZ]
	static int iCount_Check=0;
	static int iCount_MendParam=0;
	static int iCount_Abnormity=0;// ����д���������� [8/18/2009 GZ]
	int tmpLogCount=0;//���ļ���д���¼��������	
	switch(iLogType)
	{
	case OperationLog:   //������־
		{
			strIniPath.Format("%s%s%4d%s%02d%s", strLogFilePath, "LogFiles\\OperationLog\\",
							curTime.GetYear(), "��",curTime.GetMonth(), "��");

			strIniFileName.Format("%s%s%4d%02d%02d%s", 
							strIniPath, "\\", 
							curTime.GetYear(), 
							curTime.GetMonth(), 
							curTime.GetDay(), ".txt");
			iCount_Operation++;
			tmpLogCount=iCount_Operation;
		}
		break;
	case CheckLog:       //�����־
		{
			strIniPath.Format("%s%s%4d%s%02d%s", strLogFilePath, "LogFiles\\CheckLog\\",
							curTime.GetYear(), "��",curTime.GetMonth(), "��");

			strIniFileName.Format("%s%s%4d%02d%02d%s", 
							strIniPath, "\\", 
							curTime.GetYear(), 
							curTime.GetMonth(), 
							curTime.GetDay(), ".txt");
			iCount_Check++;
			tmpLogCount=iCount_Check;
		}
		break;
	case MendParamLog:   //�޸Ĳ�����־
		{
			strIniPath.Format("%s%s%4d%s%02d%s", strLogFilePath, "LogFiles\\MendParamLog\\",
							curTime.GetYear(), "��",curTime.GetMonth(), "��");

			strIniFileName.Format("%s%s%4d%02d%02d%s", 
							strIniPath, "\\", 
							curTime.GetYear(), 
							curTime.GetMonth(), 
							curTime.GetDay(), ".txt");
			iCount_MendParam++;
			tmpLogCount=iCount_MendParam;
		}
		break;
	case AbnormityLog:   //�쳣��־
		{
			strIniPath.Format("%s%s%4d%s%02d%s", strLogFilePath, "LogFiles\\AbnormityLog\\",
							curTime.GetYear(), "��",curTime.GetMonth(), "��");

			strIniFileName.Format("%s%s%4d%02d%02d%s", 
							strIniPath, "\\", 
							curTime.GetYear(), 
							curTime.GetMonth(), 
							curTime.GetDay(), ".txt");
			iCount_Abnormity++;
			tmpLogCount=iCount_Abnormity;
		}
		break;
	default:
		{
			strIniPath.Format("%s%s%4d%s%02d%s", strLogFilePath, "LogFiles\\OperationLog\\",
							curTime.GetYear(), "��",curTime.GetMonth(), "��");

			strIniFileName.Format("%s%s%4d%02d%02d%s", 
							strIniPath, "\\", 
							curTime.GetYear(), 
							curTime.GetMonth(), 
							curTime.GetDay(), ".txt");
			iCount_Operation++;
			tmpLogCount=iCount_Operation;
		}
	}
		
	strTime.Format("%02d%s%02d%s%02d:%03d%03d", 
		curTime.GetHour(), ":", 
		curTime.GetMinute(), ":", 
		curTime.GetSecond(),dwTime, rand()%1000);
	
	if (!IsPathExist(strIniPath))// ·�������ڣ�����·�� [8/17/2009 GZ]
	{
		switch(iLogType)
		{
		case OperationLog:
			iCount_Operation=0;
			break;
		case CheckLog:
			iCount_Check=0;
			break;
		case MendParamLog:
			iCount_MendParam=0;
			break;
		case AbnormityLog:
			iCount_Abnormity=0;
			break;
		default:
			iCount_Operation=0;
			break;
		}
		if (CreateMultiLevelPath(strIniPath))
		{
			
			if (!WritePrivateProfileString(strSession, strTime, strContent, strIniFileName))
			{
				m_strLastManagerError.Format("%s","WritePrivateProfileString����������־д��δ�ɹ�");
				return FALSE;
			}
			
		}
		else
		{
			//AfxMessageBox("�ļ���·��û�д����ɹ�!");
			m_strLastManagerError="�ļ���·��û�д����ɹ�";
			return FALSE;
		}
	}
	else  //·�����ڣ����ж��ļ��Ƿ����
	{
		//����ļ����ڣ����ж��Ƿ������������ƣ���д������
		if (IsFileExist(strIniFileName))
		{

			if (tmpLogCount>MAXLOGCOUNT)// �������������ƣ����¿��ļ��洢 [8/14/2009 GZ]
			{
				
				CString tmpstrNext("");
				tmpstrNext=strIniFileName;
				tmpstrNext.Delete(tmpstrNext.GetLength()-4,4);
				CString strNextName("");
				strNextName.Format("%s_next%d.txt",tmpstrNext,tmpLogCount/MAXLOGCOUNT);
				
		
				if (!WritePrivateProfileString(strSession, strTime, strContent, strNextName))
				{
					m_strLastManagerError.Format("%s","WritePrivateProfileString����������־д��δ�ɹ�");
					return FALSE;
				}

			}
			else// ����������д [8/18/2009 GZ]
			{
						
				if (!WritePrivateProfileString(strSession, strTime, strContent, strIniFileName))
				{
					m_strLastManagerError.Format("%s","WritePrivateProfileString����������־д��δ�ɹ�");
					return FALSE;
				}
						
			}
			
			
		}
		else// �ļ����������� [8/18/2009 GZ]
		{
			switch(iLogType)
			{
			case OperationLog:
				iCount_Operation=0;
				break;
			case CheckLog:
				iCount_Check=0;
				break;
			case MendParamLog:
				iCount_MendParam=0;
				break;
			case AbnormityLog:
				iCount_Abnormity=0;
				break;
			default:
				iCount_Operation=0;
				break;
			}
			if (!WritePrivateProfileString(strSession, strTime, strContent, strIniFileName))
			{
				m_strLastManagerError.Format("%s","WritePrivateProfileString����������־д��δ�ɹ�");
				return FALSE;
			}
		}
	}

	return TRUE;
}

CString CLogManager::GetLastManagerError()
{
	return m_strLastManagerError;
}

// ��ȡ��ִ�г������ڵ�·��
CString CLogManager::GetCurrentAppliationPath()
{
	char str[MAX_PATH], drive[MAX_PATH], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
	CString AppPath;
	GetModuleFileName(NULL, str, sizeof(str));
	_splitpath(str, drive, dir, fname, ext);
	strcat(drive, dir);
	AppPath = drive;

	return AppPath;
}
#include <io.h>//ͷ�ļ�

//�ж�·���Ƿ����
BOOL CLogManager::IsPathExist(const CString &strPath)
{
	
#ifdef _UNICODE
#define IsDirectoryExist(path) ::_waccess(path,0) == 0
	//_waccess�ĵڶ���������ʾ���жϴ����ԣ�����0��ʾ���ڣ�����-1��ʾ������
#else
#define IsDirectoryExist(path) ::_access(path,0) == 0//_access��MBCS�汾
#endif

	if(IsDirectoryExist(strPath))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//�ж��ļ��Ƿ����
BOOL CLogManager::IsFileExist(const CString &strFileName)
{
	CFileFind searcher;

	if(searcher.FindFile(strFileName))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//����һ�����Ŀ¼��������ھͲ�����
//(strPath  �Ϸ�: "D:\\abc\\me" �� "me\\you" �� ""; 
//			���Ϸ�: "D:\\abc\\me\\" �� "me\\you\\" �� "\\")
BOOL CLogManager::CreateMultiLevelPath(const CString &strPath)
{
	CString tmpPath;// = strPath;
	tmpPath.Format("%s",strPath);
	CList<CString,CString&> FolderStrList;
	int nStrLen = 0;
	
	for(;;)
	{
		nStrLen = tmpPath.GetLength();
		if(nStrLen <= 0)
		{
			break;//��ͷ���ļ���
		}		
	
		BOOL bPathExist = FALSE;//��·���Ƿ��Ѿ�����

		if(tmpPath.Right(1) == ":")
		{
			bPathExist = IsPathExist(tmpPath + "\\");
		}
		else
		{
			bPathExist = IsPathExist(tmpPath);
		}

		if(bPathExist)//���·������
		{
			break;//���Ѵ��ڵ�·���½��ļ���
		}
		
		//���·��������		
		//ÿ�ν�����һ���ļ��������
		
		int nPos = 0;
		nPos = tmpPath.ReverseFind('\\');//�Ӻ���ǰȷ���ļ�����
		if(nPos == -1)//�ѵ����һ���ļ�����
		{
			nPos = 0;//����һ���ļ��������
		}
		
		//���ļ��������
		CString tmpStr = tmpPath.Right(nStrLen - nPos);
		FolderStrList.AddHead(tmpStr);
		tmpPath.Delete(nPos, nStrLen - nPos);		
	}	
	
	//�����ļ���������˳����
	while(FolderStrList.IsEmpty() == FALSE)
	{
		tmpPath += FolderStrList.RemoveHead();
		BOOL bRet = FALSE;
		bRet = CreateDirectory(tmpPath, NULL);
		if(bRet == FALSE)
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

