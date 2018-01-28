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
	
    // ��ʼ��
	s_Status Init(s_CheckInitParam sInitParam);//�����ڳ�ʼ��+���㷨����ģ��+������

	// �ͷ���Դ
	s_Status Free();

	// �������߳� 
	s_Status Detect(s_CheckInParam sCheckInParam);
	
	// ��������
	s_Status ShowDetail(int nCheckNS);
	
	s_Status Set(s_AlgModelInput sAlgModelInput);//������
protected:
	void* m_pInspObj;
};

#endif // !defined(AFX_OBJALLOC_H__8642FBEB_F57C_4058_8912_DD030E209D81__INCLUDED_)
