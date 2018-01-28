
#if !defined(AFX_CINSPCLASS_H__B99533A4_69F3_46D1_8C50_DE5054773748__INCLUDED_)
#define AFX_CINSPCLASS_H__B99533A4_69F3_46D1_8C50_DE5054773748__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "alg_struct.h"
using namespace Alg;

class CInspBase
{
public:
	CInspBase(){}														//����
	virtual ~CInspBase(){}												//����
	// ��ʼ��
	virtual s_Status Init(s_CheckInitSingleParam sInitParam) = 0;//�����ڳ�ʼ��+������

	// �ͷ���Դ
	virtual s_Status Free() = 0;

	// ��⺯��
	virtual s_Status Detect(s_CheckInParam sCheckInParam, s_CheckOutParam* sCheckOutParam) = 0;

	// ��������
	virtual s_Status ShowDetail() = 0;

	virtual s_Status Set(s_AlgModelInput sAlgModelInput) = 0;//��ʾͼ�񣬶����������⹦��				
protected:
	void* m_pDetect;
};

#endif // !defined(AFX_CINSPCLASS_H__B99533A4_69F3_46D1_8C50_DE5054773748__INCLUDED_)
//����ָ��
//typedef CInspBase* (WINAPI* create_instance)(int nNumObj);
//typedef void (WINAPI* destroy_instance)(CInspBase* pCheck);