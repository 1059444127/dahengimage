// ProgramLicense.h: interface for the CProgramLicense class.
//
//////////////////////////////////////////////////////////////////////
#include "..\\..\\IncPublic\\alg_struct.h"
//using namespace Alg;

#if !defined(AFX_PROGRAMLICENSE_H__A36FE32F_4A7E_435E_B3E1_D226AF592D80__INCLUDED_)
#define AFX_PROGRAMLICENSE_H__A36FE32F_4A7E_435E_B3E1_D226AF592D80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef GUID ver_code;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	_EXPORTING

#ifdef _EXPORTING
#define CLASS_DECLSPEC    __declspec(dllexport)  //�������ʱʹ��
#else
#define CLASS_DECLSPEC    __declspec(dllimport)	 //��̬��Ӧ��ʱʹ��
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define KV_NOLIMITATION			0		//���к�������
#define KV_NOTREACHLIMITATION	-1		//δ�ﵽ���ƣ������ֹ���ڻ���Զ
#define KV_CLOSETODEADLINE		-2		//δ�ﵽ���ƣ����ֹ���ںܽ�

#define KV_NOTFINDROCKEYDLL		1		//û�ҵ����ܹ���̬��
#define KV_NOTFINDDONGLE		2		//û�ҵ����ܹ�
#define KV_OPENDONGLEFAIL		3		//�򿪼��ܹ�ʧ��
#define KV_FAILLOADKEY			4		//������Կʧ��
#define KV_UNVALIDUSERID		5		//�û�������
#define KV_INVALIDKEY			6		//�Ƿ���Կ
#define KV_EXPIREDKEY			7		//��Կ����
#define KV_LICENSEFILENOTFOUND  8		//û�ҵ�License�ļ�
#define KV_INFOCHANGED			9		//ע����Ϣ������
#define KV_VCODEERROR			10		//UUID��֤ʧ��
#define KV_UNKNOWPROBLEM		99		//��������

typedef struct _KeyVerfResult
{
	int nError;				//��Ӧ�ϱߵ�define�Ķ���	
	int nExpireDate;		//-1: �޴�������  0: δ����ֹ����  1: �����ֹ��������15��
	int nDays;				//-1: �޴�������  0: δ�ﵽʹ����������  1: ����ʹ���������ƻ�ʣ����15��
	int nGlobalRuntime;		//-1: �޴�������  0: δ�ﵽʹ��ʱ������  1: ����ʹ��������������30����
	int nExecutions;		//-1: �޴�������  0: δ�ﵽʹ�ô�������  1: ����ʹ�ô������ƻ�ʣ����10��
	char chErrorDetail[1024];
}s_KeyVerfResult;

class CLASS_DECLSPEC CProgramLicense  
{
public:
	CProgramLicense();
	virtual ~CProgramLicense();
	
	s_KeyVerfResult CheckLicenseValid(BOOL bFirst);
	void GetVerCode(ver_code* vCode);
	int CheckDogTime();	//0: Good  1: �ز�����  2: �Ƿ����ڻ��/���ܴ���  >3: ���ܹ����ʹ���
	void ShowDongleErrorCode();

	int	ReadHardwareID(char* chHardwareID);
	void OpenDog(); //�����ã���
private:
	void* m_pCheck;
//	LONGLONG m_llFreq;
};

#endif // !defined(AFX_PROGRAMLICENSE_H__A36FE32F_4A7E_435E_B3E1_D226AF592D80__INCLUDED_)
