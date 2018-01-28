/************************************************************************/
/* Copyright (c) 2017, �������ͼ���Ӿ����޹�˾  
/* All rights reserved.													

// 2017-04-05
//��Ѫ�ܱ�ǩ����ṹ�嶨��

2017.04.07
�����������±�д��Ѫ����������ṹ��
/************************************************************************/


#pragma once
#include "alg_struct.h"
#include "vector"
using namespace std;

namespace General 
{
	class CAlgSingleInitParam
	{
	public:
		int iSize;//**[10/10/2017 ConanSteve]**:  ����ڴ��С
		int iStation;//**[8/30/2017 ConanSteve]**:  �����λ��
		int	iCallType;//**[8/2/2017 ConanSteve]**:  0�����˶�����ֳ����ã�1��DEMO���������	2:���˶����ģ�����
		int iWindowNum;//**[10/18/2017 ConanSteve]**:  ������λ���ڵĸ�����
		int* p_iCount;//**[10/10/2017 ConanSteve]**:  ÿ�����ڶ�����ͼ
		int* p_iWidth;//**[8/30/2017 ConanSteve]**:  ͼ�����
		int* p_iHeight;//**[8/30/2017 ConanSteve]**:  ͼ��߶�
		int* p_iBayer;//**[8/30/2017 ConanSteve]**:  �����任����
		int* p_iRevert;//**[8/30/2017 ConanSteve]**:  ��ת����
		int* p_iChannel;//**[1/17/2018 ConanSteve]**:  ͼ�����ݵ�ͨ����,�ڰ�ͼΪ1����ɫͼΪ3
		HWND* hWndCamImg;//**[8/30/2017 ConanSteve]**:  ϵͳͼ���������׵�ַ
		HWND* hWndAlgImg;//**[8/30/2017 ConanSteve]**:  �㷨ͼ���������׵�ַ

		CAlgSingleInitParam()
		{ 
			iSize = sizeof(CAlgSingleInitParam);
			iStation = 0;
			iCallType = 0;
			iWindowNum = 0;
			p_iCount = NULL;
			p_iWidth = NULL;
			p_iHeight = NULL;
			p_iBayer = NULL;
			p_iRevert = NULL;
			p_iChannel = NULL;
			hWndCamImg = NULL;
			hWndAlgImg = NULL;
		}
	};

	/*************************************<�㷨��������>************************************[8/30/2017 ConanSteve]*/
	enum e_DrawImgType//**[11/21/2017 ConanSteve]**:   0������ʾ��1ֻ��ʾϵͳͼ��2ֻ��ʾ�㷨���ͼ��,3���߶���ʾ
	{
		e_DrawImgType_NONE,//**[11/21/2017 ConanSteve]**:  ������ʾ
		e_DrawImgType_Sys,//**[11/21/2017 ConanSteve]**:  ֻ��ʾϵͳͼ��
		e_DrawImgType_Alg,//**[11/21/2017 ConanSteve]**:  ֻ��ʾ�㷨���ͼ��
		e_DrawImgType_Both//**[11/21/2017 ConanSteve]**:  ���߶���ʾ
	};
	class CAlgInspInputParam
	{
	public:
		int iSize;
		bool	bExecuteAlgCheck;//**[10/10/2017 ConanSteve]**:  �㷨�Ƿ�ִ���������㷨��⣬���Ϊ0����ô�㷨��Detectʱ��ֱ��Ĭ�Ϸ��ؽ��0����ʾ��Ʒ
		int		iDisplayImgMode;			//**[10/10/2017 ConanSteve]**:  ��0 ��ʼ��ֻ������ֵ����Ҫ��ʾͼ���Ӧ�ļ����ֵ������ֵ�Ƕ����Լ���ʾ��ϵ����ȡParamGrid�����ļ�, ������ʾ��Ʒ/����ʾ/��Ʒ��
		e_DrawImgType		iDrawImgType;	//*[7/3/2017 ConanSteve]*:  0������ʾ��1ֻ��ʾϵͳͼ��2ֻ��ʾ�㷨���ͼ��,3���߶���ʾ
		int		iSaveImageMode;	//**[10/10/2017 ConanSteve]**:  ��ͼģʽ�� 0����ͼ ����ֵ������ParamGrid�����ļ�������
		/*************************************<����6��������Init���洫�ݵ�һ��>************************************[1/24/2018 ConanSteve]*/
		//**[1/24/2018 ConanSteve]**:  ��Ҫ��Ϊ�˼���û��ģ��ͼ����㷨��Demo���޷���Init�õ���Щ����
		int* p_iCount;//**[10/10/2017 ConanSteve]**:  ÿ�����ڶ�����ͼ
		int* p_iWidth;//**[8/30/2017 ConanSteve]**:  ͼ�����
		int* p_iHeight;//**[8/30/2017 ConanSteve]**:  ͼ��߶�
		int* p_iBayer;//**[8/30/2017 ConanSteve]**:  �����任����
		int* p_iRevert;//**[8/30/2017 ConanSteve]**:  ��ת����
		int* p_iChannel;//**[1/17/2018 ConanSteve]**:  ͼ�����ݵ�ͨ����,�ڰ�ͼΪ1����ɫͼΪ3
		TCHAR(*lpszSaveImagePath)[MAX_PATH] ;	//**[1/5/2018 ConanSteve]**:  ��ͼ·�����飬��֧�ֵ���λ�����:
														//**[1/5/2018 ConanSteve]**:  ���Ǹ�ָ�룬ָ��ָ��һ��������׵�ַ����������е�ÿ��Ԫ����������һ���ַ�����(�����Ĵ�ͼ·��)���׵�ַ��
														//**[1/5/2018 ConanSteve]**:  ���1ά����ĳ���Ϊ��ǰ��λ�Ĵ��ڸ�������ʵ������ĸ���
	public:
		CAlgInspInputParam()
		{ 
			iSize = sizeof(CAlgInspInputParam);
			bExecuteAlgCheck = true;
			iDrawImgType = e_DrawImgType_Both;
			iSaveImageMode = 0;
			p_iCount = NULL;
			p_iWidth = NULL;
			p_iHeight = NULL;
			p_iBayer = NULL;
			p_iRevert = NULL;
			p_iChannel = NULL;
			lpszSaveImagePath = NULL;
		}

	};

	enum e_AlgStatus {
		e_AlgStatus_Good,//**[12/5/2017 ConanSteve]**:  ��Ʒ
		e_AlgStatus_Empty,
		e_AlgStatus_Bad,//**[12/5/2017 ConanSteve]**:  ��Ʒ
		e_AlgStatus_AlgException,//**[12/5/2017 ConanSteve]**:  �㷨�쳣
		e_AlgStatus_LostImg,//**[12/5/2017 ConanSteve]**:  �����ͼ
		e_AlgStatus_Overtime//**[12/5/2017 ConanSteve]**:  ��ⳬʱ
	};

	class CAlgInspResult
	{
	public:
		//**[8/30/2017 ConanSteve]**:  �����Ӧʲô��˼�������ļ��������
		//**[12/4/2017 ConanSteve]**:  0�Ǻ�Ʒ��1�ǻ�Ʒ��2�ǿ�Ʒ��3���㷨�쳣
		e_AlgStatus	iAlgStatus;
		int iSize;
		int iTimeTotle; //**[8/30/2017 ConanSteve]**:  �����ʱ��
		int iRsltNum;//**[11/24/2017 ConanSteve]**:  ���������
		e_AlgStatus arrAlgStatus[128];//**[11/24/2017 ConanSteve]**: 
		int	iLenOfDefect;//**[10/10/2017 ConanSteve]**:  ȱ�������ĳ���
		int arrHeadOfDefectDesc[256];//**[8/30/2017 ConanSteve]**:  ȱ������
		bool bStopMachine[64];//**[1/9/2018 ConanSteve]**:  ���ͣ��
		bool bAlarm[64];//**[1/9/2018 ConanSteve]**:  ����
		TCHAR lpszCheckDesc[MAX_PATH];//**[1/17/2018 ConanSteve]**:  ���������Ϣ

	public:
		CAlgInspResult() 
		{
			iAlgStatus = e_AlgStatus_Good;
			iSize = sizeof(CAlgInspResult);
			iTimeTotle = 0;
			iRsltNum = 1;
			iLenOfDefect = 0;
			memset(arrAlgStatus, 0, sizeof(arrAlgStatus));
			memset(arrHeadOfDefectDesc, 0, sizeof(arrHeadOfDefectDesc)); 
			memset(bStopMachine, 0, sizeof(bStopMachine));
			memset(bAlarm, 0, sizeof(bAlarm));
			memset(lpszCheckDesc, 0, sizeof(lpszCheckDesc));
		}

	};
}




/*************************************<�㷨��ʼ��������>************************************[8/30/2017 ConanSteve]*/

