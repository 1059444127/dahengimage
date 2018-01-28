#if !defined(AFX_DLGNEWMODEL_H__1FE194E1_3B52_4698_9E6C_5255C31C5B81__INCLUDED_)
#define AFX_DLGNEWMODEL_H__1FE194E1_3B52_4698_9E6C_5255C31C5B81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNewModel.h : header file
//
#define MAX_MEDICINE_TYPE 200  //ҩƷ���������Ŀ

/////////////////////////////////////////////////////////////////////////////
// CDlgNewModel dialog

class CDlgNewModel : public CDialog
{
// Construction
public:
	CDlgNewModel(CWnd* pParent = NULL);   // standard constructor


	CString m_sModelName;	//ģ������

	CString m_sNewName;		//��Ʒ����[2015-5-6]
	CString m_sNewSpec;		//��Ʒ���
	CString m_sNewPercent;		//��Ʒ�ٷֱ�

	CString m_sGroupName[MAX_MEDICINE_TYPE];	//��Ʒ���Ƽ�������
	CString m_sGroupSpec[MAX_MEDICINE_TYPE];	//��Ʒ��񼯺�����
	CString m_sGroupPercent[MAX_MEDICINE_TYPE];	//��Ʒ�ٷֱȼ�������

	int m_iName; //��Ʒ����[2015-5-6]
	int m_iSpec; //��Ʒ���[2015-5-6]
	int m_iPercent; //��Ʒ�ٷֱ�[2015-5-6]

	int m_iNameIndex; //��Ʒ����
	int m_iSpecIndex; //��Ʒ���
	int m_iPercentIndex; //��Ʒ�ٷֱ�

	int m_iLabelType;		//��ǩ���ͣ�������ʽ�����㷨ϵͳͨ��Э��涨��

	int m_iBodyShape; // 100Ϊ����ƿ 101Ϊ������
	int m_iHeadShape; // 100Ϊ���� 101Ϊ˫��


// Dialog Data
	//{{AFX_DATA(CDlgNewModel)
	enum { IDD = IDD_DLG_NEW_MODEL };
	CComboBox	m_cmbSpec;
	CComboBox	m_cmbPercent;
	CComboBox	m_cmbName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNewModel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgNewModel)
	virtual void OnOK();
	afx_msg void OnRadioBodyShape1();
	afx_msg void OnRadioBodyShape2();
	afx_msg void OnRadioHeadShape1();
	afx_msg void OnRadioHeadShape2();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboName();
	afx_msg void OnSelchangeComboPercent();
	afx_msg void OnSelchangeComboSpec();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNEWMODEL_H__1FE194E1_3B52_4698_9E6C_5255C31C5B81__INCLUDED_)
