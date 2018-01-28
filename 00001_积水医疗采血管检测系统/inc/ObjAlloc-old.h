// ObjAlloc.h: interface for the CObjAlloc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJALLOC_H__8642FBEB_F57C_4058_8912_DD030E209D81__INCLUDED_)
#define AFX_OBJALLOC_H__8642FBEB_F57C_4058_8912_DD030E209D81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "alg_struct.h"
using namespace Alg;

#define	_EXPORTING

#ifdef _EXPORTING
#define CLASS_DECLSPEC    __declspec(dllexport)  //�������ʱʹ��
#else
#define CLASS_DECLSPEC    __declspec(dllimport)	 //��̬��Ӧ��ʱʹ��
#endif

class CLASS_DECLSPEC CObjAlloc  
{
public:
	CObjAlloc();
	virtual ~CObjAlloc();
	
	s_Status Initialize(s_AlgInitPara sInitPara);								//��ʼ��
	s_Status ReadModel(s_AlgModelInput sModelIn);								//ģ������
	s_Status Inspect(s_AlgInspInPara& sInspectIn, s_AlgInspResult &sInspectOut);//��⺯��
	s_Status Free();															//�ͷ���Դ���ɲ����ã������������Զ����ã�
	void     ShowInfoDlg(int nCmdShow);											//SW_SHOW or SW_HIDE
	/*
	nCameraIndex һ�ɴ�0��ʼ
	*/
protected:
	void* m_pInspObj;
};

#endif // !defined(AFX_OBJALLOC_H__8642FBEB_F57C_4058_8912_DD030E209D81__INCLUDED_)
