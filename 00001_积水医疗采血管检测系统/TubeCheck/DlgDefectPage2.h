#if !defined(AFX_DLGDEFECTPAGE2_H__E994ACC0_3277_4211_939A_6225DA9F676D__INCLUDED_)
#define AFX_DLGDEFECTPAGE2_H__E994ACC0_3277_4211_939A_6225DA9F676D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDefectPage2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDefectPage2 dialog

class CDlgDefectPage2 : public CDialog
{
// Construction
public:
	CDlgDefectPage2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDefectPage2)
	enum { IDD = IDD_DLG_DEFECT_PAGE_2 };
	CListCtrl	m_listData;
	CMonthCalCtrl	m_StopDate;
	CMonthCalCtrl	m_StartDate;
	CComboBox	m_cmbModelName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDefectPage2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDefectPage2)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnInquiry();
	afx_msg void OnBtnExcel();
	afx_msg void OnSelchangeStartDate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeStopDate(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

	// ����
	BOOL m_bCheckDate;			//�Ƿ�ѡ������
	BOOL m_bCheckModel;			//�Ƿ�ѡ��ģ������
	
	_ConnectionPtr m_Connect;	// �������ݿ�
	_RecordsetPtr m_RecordSet;	// ��¼��
	
	int m_iTotalCount;			// �������
	int m_iTotalGood;			// ��Ʒ����
	int m_iTotalBad;			// ��Ʒ����
	int m_iFieldCount;			// �ֶθ���
	
	CString m_sModelName;		// ģ������
	CString m_sAlgModelPath;	// �㷨�ļ�·��
	CString m_sStartTime;		//����ʱ��
	CString m_sStopTime;		//ͣ��ʱ��
	CString m_sStartYear;		//����-��
	CString m_sStartMonth;		//����-��
	CString m_sStartDay;		//����-��
	CString m_sStopYear;		//ͣ��-��
	CString m_sStopMonth;		//ͣ��-��
	CString m_sStopDay;			//ͣ��-��

	CPtrList m_PtrList;			// ��ż�¼������
	CStdioFile m_stdFile;		// ���ڴ򿪣�д�룬����.csv�ļ�
	CStringList m_listColName;	// �б��ÿ������
	
	// ����
	void InitComboProduct();				// ��ʼ��ģ����Ͽ�
	BOOL GetRecordSet(CString sSQL);		// ��ü�¼��
	BOOL OutputRecord();					// �����¼��
	BOOL CloseCsvFile();					// �ر�Excel�ļ�
	BOOL OpenCsvFile(CString strFilePath);	// ��Excel�ļ�


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDEFECTPAGE2_H__E994ACC0_3277_4211_939A_6225DA9F676D__INCLUDED_)
