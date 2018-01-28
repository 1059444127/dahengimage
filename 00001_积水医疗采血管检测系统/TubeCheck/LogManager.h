/************************************************************************/
/* Copyright (c) 2005, �������ͼ���Ӿ����޹�˾�Ӿ�ϵͳ��     
/* All rights reserved.													
/*																		
/* �ļ����ƣ� LogManager.h												
/* �ļ���ʶ�� 
/* ժҪ�� ���漰��־�ļ������ĸ��ֺ������л���, �ṩ�ļ���·�����ò���,
/*    ���漰��ͼ���ļ�, �����ı��ļ��ȹ���
/*
/* ��ǰ�汾�� 2.0
/* ���ߣ� ����
/* ������ڣ� 2009��8��17��
/*�޸����ڣ�2009��8��18�� ��������������Ϊԭ����_nextN.txt N=1,2,3...
/************************************************************************/

#if !defined(LogManager_h_)
#define LogManager_h_

//�����������Ҫ�ı����ͽṹ��



enum e_SaveLogType	    //������־����
{
	OperationLog = 0,			//������־
	CheckLog,		            //�����־
	MendParamLog,			    //�޸Ĳ�����־
	AbnormityLog                //�쳣��־
};

#define MAXLOGCOUNT 5000	// ���ļ���־���д������ [8/14/2009 GZ]


class CLogManager 
{
public:
	// ��ȡ��ִ�г������ڵ�·��
	CString GetCurrentAppliationPath();
	
	//�ж�·���Ƿ����
	BOOL IsPathExist(const CString &strPath);
	
	//�ж��ļ��Ƿ����
	BOOL IsFileExist(const CString &strFileName);
	
	//����һ�����Ŀ¼��������ھͲ�����
	//(strPath  �Ϸ�: "D:\\abc\\me" �� "me\\you" �� ""; 
	//			���Ϸ�: "D:\\abc\\me\\" �� "me\\you\\" �� "\\")
	BOOL CreateMultiLevelPath(const CString &strPath);

	CString GetLastManagerError();//����
	CString m_strLastManagerError;
	CString m_strNext;// ��������MAXLOGCOUNTʱ�ļ�����ӵĺ�׺ [8/14/2009 GZ]
	CLogManager();
	virtual ~CLogManager(); 
	//����һ��Ŀ¼��������ڲ�����
	BOOL MyCreatePath(const CString &strPath);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//����MyWriteLogFile()
	//���ܣ�	д��־��
	//			1.�Զ����� �Զ�����־·�����£�
	//			Run\ (��ǰ�ļ�Ŀ¼)
	//				LogFiles\
	//					 OperationLog\                ��������־ ͨѶ�����ȣ�
	//					 AbnormityLog\               ���쳣��־��
	//					 CheckLog\                   �������־��
	//					 MendParamLog\              ���޸Ĳ�����־  ����Ĳ�����

	//			2.֧�ֳ������ļ���־���д���������½��ļ�д����־
	//������
	//				//����1��strLogFilePathΪһ�����Ŀ¼��������ھͲ����������������´���,�������ڴ�·���´��� ����1�е�·�� Ȼ��д��־
	//				//		(strLogFilePath  �Ϸ�: "D:\\abc\\me" �� "me\\you" �� ""; 
	//				//					���Ϸ�: "D:\\abc\\me\\" �� "me\\you\\" �� "\\")
	//			����1��strSessionΪ���࣬����־�����������
	//			����2����־��������
	//			����3����־���ͣ�������enum e_SaveLogType���ͣ�0Ϊ������־��1Ϊ�����־��2Ϊ�޸Ĳ�����־��3Ϊ�쳣��־
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL MyWriteLogFile(/*const CString& strLogFilePath, */ // ȥ��·����Ĭ��Ϊ��ǰ�ļ��� [8/17/2009 GZ]
		const CString& strSession, 
		const CString& strContent,
		int   iLogType);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

protected:
private:
};

#endif // !define(LogManager_h_)