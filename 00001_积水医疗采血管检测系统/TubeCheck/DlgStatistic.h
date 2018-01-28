#if !defined(AFX_DLGSTATISTIC_H__02FD4B4B_1892_4CAC_BC98_FFE999C4EA26__INCLUDED_)
#define AFX_DLGSTATISTIC_H__02FD4B4B_1892_4CAC_BC98_FFE999C4EA26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgStatistic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgStatistic dialog

class CDlgStatistic : public CDialog
{
// Construction
public:
	CDlgStatistic(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgStatistic)
	enum { IDD = IDD_DLG_STATISTIC };
	CListCtrl	m_listData;
	CComboBox	m_Model;
	CMonthCalCtrl	m_StopTime;
	CMonthCalCtrl	m_StartTime;
	//}}AFX_DATA

	CString m_sStartTime;		//����ʱ��
	CString m_sStopTime;		//ͣ��ʱ��
	CString m_sStartYear;		//����-��
	CString m_sStartMonth;		//����-��
	CString m_sStartDay;		//����-��
	CString m_sStopYear;		//ͣ��-��
	CString m_sStopMonth;		//ͣ��-��
	CString m_sStopDay;			//ͣ��-��
	CString m_sModelName;		//ģ������
	CString m_sAlgModelPath;	//ģ��·��

	BOOL m_bCheckDate;			//�Ƿ�ѡ������
	BOOL m_bCheckModel;			//�Ƿ�ѡ��ģ������

	_ConnectionPtr m_Connect;	// �������ݿ�
	_RecordsetPtr m_RecordSet;	// ��¼��

	int m_iTotalCount;			// �������
	int m_iTotalGood;			// ��Ʒ����
	int m_iTotalBad;			// ��Ʒ����
	int m_iFieldCount;			// �ֶθ���

	CPtrList m_PtrList;			// ��ż�¼������
	CStdioFile m_stdFile;		// ���ڴ򿪣�д�룬����.csv�ļ�

	COleDateTime m_DateTime;

public:
	void InitComboProduct();	//��ʼ��ģ����Ͽ�
	void Init(_ConnectionPtr pConn, _RecordsetPtr pRecord);	//��ȡ���ʱ���
	BOOL GetRecordSet(CString sSQL);		// ��ü�¼��
	BOOL OutputRecord();					// �����¼��
	void CloseCsvFile();					// �ر�Excel�ļ�
	BOOL OpenCsvFile(CString strFilePath);	// ��Excel�ļ�


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgStatistic)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgStatistic)
	afx_msg void OnSelectStartTime(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeStartTime(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnExcel();
	afx_msg void OnBtnQuery();
	afx_msg void OnSelchangeStopTime(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSTATISTIC_H__02FD4B4B_1892_4CAC_BC98_FFE999C4EA26__INCLUDED_)
