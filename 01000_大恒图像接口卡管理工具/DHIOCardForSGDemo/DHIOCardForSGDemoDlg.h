
// DHIOCardForSGDemoDlg.h : ͷ�ļ�
//

#pragma once

#include "DHIOCard.h"
#include "DHIOCardForSG.h"
#include "DHIOCardForPIO24C.h"//**[12/19/2017 ConanSteve]**:  C��
#include "DHIOCardForPCI6356.h"
 #include "DHIOCardForRS232_XZXNYN.h"
#include "ExFn.h"
#include "afxwin.h"
#include "array"
using namespace std;

#pragma comment(lib, "DHIOCardForPCI6356D.lib")
// CDHIOCardForSGDemoDlg �Ի���
class CDHIOCardForSGDemoDlg : public CDialogEx
{
// ����
public:
	CDHIOCardForSGDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DHIOCARDFORSGDEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpencard1();
	CComboBox m_comboIOcardSN;
	int m_iCurIOCardType; 
	int m_iIOCardNum;
	int m_iLastIOCardSN;//**[12/16/2017 ConanSteve]**:  ��һ��ѡ�нӿڿ������к�
	array<bool, 6> m_arr_bEnable{ false };//**[12/16/2017 ConanSteve]**:  ��¼ÿ�ֽӿڿ��Ƿ����
	unique_ptr<shared_ptr<CDHIOCard>[] > m_pIOCard{ nullptr };
	unique_ptr<e_IOCardType[]> m_p_eIOCardType{ nullptr };//**[12/16/2017 ConanSteve]**:  �ӿڿ�����
	unique_ptr<CString[]> m_p_sPathFileOfInit{ nullptr };//**[12/16/2017 ConanSteve]**:  �ӿڿ������ļ�
	unique_ptr<CString[]> m_p_sPathFileOfConnect{ nullptr };//**[12/16/2017 ConanSteve]**:  ϵͳ��ӿڿ����������ļ������ļ�
	CINICfg m_iniIOCardManageCfg;//**[12/16/2017 ConanSteve]**:  �ӿڿ������������ļ�
	afx_msg void OnCbnSelchangeComboIocardsn();
	afx_msg void OnBnClickedButtonOpeniocardcfgfile();
	afx_msg void OnBnClickedRadio1(UINT uID);
	afx_msg void OnBnClickedButtonOpenconnectionfile();
	afx_msg void OnBnClickedButtonOpenmanagetoolcfgfile();
	afx_msg void OnBnClickedButtonReinit();
	bool InitWindow();
};
