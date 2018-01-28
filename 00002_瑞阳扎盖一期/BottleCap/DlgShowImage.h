#if !defined(AFX_DLGSHOWIMAGE_H__513E4BAC_4BA3_4AA1_9A27_53D990295AAD__INCLUDED_)
#define AFX_DLGSHOWIMAGE_H__513E4BAC_4BA3_4AA1_9A27_53D990295AAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgShowImage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgShowImage dialog
#include "MainFrm.h"

class CDlgShowImage : public CDialog
{
// Construction
public:
	CDlgShowImage(CWnd* pParent = NULL);   // standard constructor

public:

	int m_iWidth;								//��
	int m_iHeight;								//�߶�
	int m_iBytes;								//λ��
	int m_iCameraSN;							//��¼��ǰ��λ��

	CString m_sSaveImagePath[CAMERA_COUNT];		//����ͼ��·��
	CString m_sErrorDes[CAMERA_COUNT];			//��������
	CImage *m_pImage[CAMERA_COUNT];				//ͼ���ڴ�
	CRect m_lRect[CAMERA_COUNT];				//��ʾͼ������
	CDC *m_pDC[CAMERA_COUNT];					//��ʾDC
	

	BOOL m_bFirstInit[CAMERA_COUNT];			//�Ƿ��Ѿ���ʼ��

	// ��ʾԭͼ��
	void ShowImage();

	// ��ͼ��
	BOOL OpenImage(CString strPath, CDC *pDC, CRect lRect, int iCameraSN);


// Dialog Data
	//{{AFX_DATA(CDlgShowImage)
	enum { IDD = IDD_DLG_SHOW_IMAGE };
	CStatic	m_imgPic1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgShowImage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgShowImage)
	afx_msg void OnBtnImage();
	afx_msg void OnBtnParam();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSHOWIMAGE_H__513E4BAC_4BA3_4AA1_9A27_53D990295AAD__INCLUDED_)
