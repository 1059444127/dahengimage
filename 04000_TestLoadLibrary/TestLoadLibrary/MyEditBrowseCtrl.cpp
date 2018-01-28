// MyEditBrowseCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestLoadLibrary.h"
#include "MyEditBrowseCtrl.h"


// CMyEditBrowseCtrl

IMPLEMENT_DYNAMIC(CMyEditBrowseCtrl, CMFCEditBrowseCtrl)

CMyEditBrowseCtrl::CMyEditBrowseCtrl()
{

}

CMyEditBrowseCtrl::~CMyEditBrowseCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyEditBrowseCtrl, CMFCEditBrowseCtrl)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()



// CMyEditBrowseCtrl ��Ϣ�������




void CMyEditBrowseCtrl::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	TCHAR szFilePathName[_MAX_PATH + 1] = { 0 };

	//�õ��ļ�����      
	UINT nNumOfFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	for (UINT nIndex = 0; nIndex < nNumOfFiles; ++nIndex)
	{
		//���õ��ļ���   
		DragQueryFile(hDropInfo, nIndex, (LPTSTR)szFilePathName, _MAX_PATH);
		this->SetWindowText(szFilePathName);		
	}

	//����ϷŲ���
	DragFinish(hDropInfo);
	CMFCEditBrowseCtrl::OnDropFiles(hDropInfo);
}
