#if !defined(AFX_ALGSTOPPER_H__3D35A3B4_D261_9285_8EBD_714891F3AEF3__INCLUDED_)
#define AFX_ALGSTOPPER_H__3D35A3B4_D261_9285_8EBD_714891F3AEF3__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/*
* Copyright (c) 2017,�������ͼ���Ӿ����޹�˾ҽҩ�����ҵ��
* All rights reserved.
*
* �ļ����ƣ�alg_struct_stopper.h
* �ļ���ʶ��
* ժҪ���㷨��ϵͳ�ӿ���ʹ�õĽṹ��Ϊͨ�ýṹ�壬�ýṹ�������ǹ̶���ͨ�õġ�
		��ͬ����Ŀ���㷨��ϵͳ��Ա������Ҫ�����Լ���Ŀ��Ҫȥ�Զ���ṹ�壬�ýṹ���ļ�
		��Ϊҩ����ҵ��������Ŀ���Զ���ṹ�壬�ļ�������(��������)��������Ѫ��PP��Ŀ��
* ��ǰ�汾��1.0
* ���ߣ�
* ������ڣ�2017��3��16��
*
*/

//=========================�㷨�Զ���ṹ��===================================
namespace Stopper
{
	//��ʼ��	
	typedef struct _AlgInitParam
	{
		CDC	**pDC;		//���ھ��
		int	nBayer[30];	//�����任ģʽ��֧��30�����  0�ޱ����任 1gbģʽ 2grģʽ 3bgģʽ 4rgģʽ
		int	nRevert[30];//ͼ��תģʽ��֧��30�����  0����ת 1���ҷ�ת 2���·�ת 3�ԽǷ�ת
	}s_AlgInitParam;


	//�������
	typedef struct _AlgDetectInput
	{
		int		nInSig;			//ϵͳ��������Ϊ0 !!!����0״̬Ϊ�㷨�ڲ�ʹ�á�
		int		bRunCheck;		//�Ƿ��� 0���� 1���

		int		nImageWidth;	//ͼ���
		int		nImageHeight;	//ͼ���
		int		nChannel;		//ͨ����
				
		int		nDisplayMode;	//��ʾģʽ�� 0����ʾ 1��ʾ���� 2��ʾ��Ʒ 3��ʾ��Ʒ
		int		nSaveImageMode;	//��ͼģʽ�� 0����ͼ 1�������� 2�����ͼ 3�����м��Ŀ��ͼ
		char	chSaveImagePath[MAX_PATH];//��ͼ·��
	}s_AlgDetectInput;
	//������
	typedef struct _AlgDetectOutput
	{
		int	nResult;			//�����(����Ŀ��ʱʹ��)
		int	szResults[50];		//�����(���Ŀ��ʱʹ�ã�֧��50��Ŀ��)
	}s_AlgDetectOutput;


	//�㷨����
	typedef struct _AlgSetParam
	{
		char	chModelName[256];//ģ������

		//���²���ϵͳ�����ò���
		CWnd*	pParent;
		CRect	rect;
		bool	bTopWin;
	}s_AlgSetParam;
}
#endif // !defined(AFX_ALGSTOPPER_H__3D35A3B4_D261_9285_8EBD_714891F3AEF3__INCLUDED_)