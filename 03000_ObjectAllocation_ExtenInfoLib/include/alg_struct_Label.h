/************************************************************************/
/* Copyright (c) 2017, �������ͼ���Ӿ����޹�˾  
/* All rights reserved.													

// 2017-04-05
//��Ѫ�ܱ�ǩ����ṹ�嶨��

2017.04.07
�����������±�д��Ѫ����������ṹ��
/************************************************************************/


#if !defined(AFX_CHECK_H__3D35A3B4_D261_4116_8EBD_714892F3AEF3__INCLUDED_)
#define AFX_CHECK_H__3D35A3B4_D261_4116_8EBD_714892F3AEF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STATUS_STRING_MAXLENGTH	256			//����������Ϣ������ַ���
//��������״̬����
//2017.4.07 ȫɾ��
#include <vector>
using namespace std;



namespace LabelCheck
{
	// ����ִ��״̬��Ϣ
	typedef struct _Status
	{
		int		nStatusType;									//����״̬����
		char	chErrorInfo[STATUS_STRING_MAXLENGTH]{0};			//������Ϣ����
		char    chErrorContext[STATUS_STRING_MAXLENGTH]{0};		//������ϸ��Ϣ����	
		void*   Empty;
	}s_Status;

	typedef struct _AlgSingleInit
	{
		int		nWidth;         //ͼ����
		int		nHeight;	    //ͼ��߶�
		CDC		**pDC;			//���ھ��
		int		nCameraIndex;	//�����λ��
		int	nBayer;
		int	nRevert;
	}s_AlgSingleInit;

	//�㷨���������Զ���ṹ�� // �ӿ� ��
	typedef struct _DemoAlgSetParam
	{
		char chModelName[STATUS_STRING_MAXLENGTH]{0};
		CWnd*	pParent;
		CRect	rect;
		bool	bTopWin;
		int	iCheckSN;
	}s_DemoAlgSetParam;


	// ���������� algorithm check input parameter
	typedef struct _AlgInspectionInputParam
	{
		int		nWidth;			//ͼ���
		int		nHeight;		//ͼ���
		int		nChannel;		//ͨ����			<0ʱ ΪHalcon��ͼ������
		int		nCamIndex;		//�����			
		char	*pcImageData;	//ͼ������	
		//void    *pImageInfo;	// pImageInfo����s_CheckCameraInfo�ṹ��ָ���׵�ַ

		int		bRunCheck;		//�Ƿ��� 0���� 1��
		int		nInSig;			//����Ϊ0 !!!��ϵͳ���� 1���ڲ�����	2ֻ��ʾͼ3
		int		bDsp;			//0����ʾ 1ȫ��ʾ��Ե 3��ʾ����ͼ���Ե	101ȫ��ʾ��� 102������� 
		int		bDisplaySys;	//*[7/3/2017 ConanSteve]*:  1ֻ��ʾϵͳͼ��0ֻ��ʾ�㷨���ͼ��
		int     bDetectAlg;     //*[7/3/2017 ConanSteve]*:  1 ��ʾ���㷨���м��  0��ʾ�����㷨��⣬ͬʱҲ������Ƿ��в�Ѫ�ܣ���������ʾϵͳͼ��
		int     nDispFirstorLast; //*[7/3/2017 ConanSteve]*:  1 ��ʾ�㷨��ʾ���㷨��ϵͳ��ʾ��һ��ͼ��2��ʾ�㷨��ʾ���㷨��ϵͳ��ʾ���һ��ͼ

		int	    nDisplayMode;	// ��ʾģʽ(0:����ʾ,1:��ʾ����,2:��ʾ�в�Ѫ��,3:��ʾ��Ʒ,4:��ʾ��Ʒ,5:��ʾ��ƿ)

		int		nSaveImageMode{-1};	//��ͼģʽ�� -1����ͼ 0�������� 1�����ͼ 2�����в�Ѫ�ܵ�ͼ
		int		iRunMode{ 0 };//**[8/2/2017 ConanSteve]**:  0���ֳ�ϵͳ���ã�1��ģ�����
		int     iCount;       //����
		int     iCircleIndex;  //������  �ڼ�����Ѫ��
		char	chSaveImagePath[MAX_PATH]{0}; // ��ͼ·��
		//��ͼ·��ָ�����ļ��У�����M:\SaveImages\AllImages\2016-10-14 09-38. �㷨����iCricleIndex��nSaveImageMode�Լ�iCheckSN����ͼ��
			// ����ͼƬ��������format("%s\\Camera%d\\ProductID(%06d)\\Type(%d)_%02d.bmp",
											//cSavePath,iCheckSN+1,iCircleIndex,iSaveType,���);
	}s_AlgInspInPara;

	// ���������� algorithm check output parameter
	typedef struct _CapsuleStatus
	{ 
		int		nStatus;     
//      �������ͣ�1��λ
//  	sRightResult0="��Ʒ";//"��Ʒ"
// 		sErrorResult0="��Ʒ";//"��Ʒ"
// 		sErrorResult1="�޲�Ѫ��";
// 		sErrorResult2="����ͼ���";


//      �������ͣ�2��λ
//  	sRightResult0="��Ʒ";//"��Ʒ"
// 		sErrorResult0="��Ʒ";//"��Ʒ"
// 		sErrorResult1="�޲�Ѫ��";
// 		sErrorResult2="����ͼ���";


		char	chInfo[255];	//�Ƕ���Ϣ
	}s_CapsuleStatus;





	typedef struct _AlgInspectionReslut
	{
		int		nAlgStatus;		//0:�� 1:һ��ͼ�����ţ���Ʒ 2:�㷨�쳣 -1:Ĭ�� 3:�չ�
		char	chExpInfo[255];
		s_CapsuleStatus		sCapules[20];
		void*   Empty;
		int		iReserve1; ////������ŷ���   m_nObjIndex
		int		iDefectCount;// ȱ�ݸ���
		int     nTimeTotle; //��ʱ��
		int     nStopMachineType[20]{ 0 };// 0���ޱ�ǩ(���Ӳ�ת)  1����ǩ   ֵ0��ͣ�� 1ͣ�� [12/20/2017 NeverMore]
	}s_AlgInspResult;
}

#endif // !defined(AFX_CHECK_H__3D35A3B4_D261_4116_8EBD_714892F3AEF3__INCLUDED_)