// MyListCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DHIOCardForRS232_XZXNYN.h"
#include "MyListCtrl.h"


// CMyListCtrl

IMPLEMENT_DYNAMIC(CMyListCtrl, CListCtrl)

CMyListCtrl::CMyListCtrl()
{

}

CMyListCtrl::~CMyListCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()



// CMyListCtrl ��Ϣ�������




void CMyListCtrl::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	char szFilePathName[_MAX_PATH + 1] = { 0 };

	//�õ��ļ�����      
	UINT nNumOfFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	for (UINT nIndex = 0; nIndex < nNumOfFiles; ++nIndex)
	{
		//���õ��ļ���   
		DragQueryFile(hDropInfo, nIndex, (LPTSTR)szFilePathName, _MAX_PATH);

		//��ȡListCtrl�Ѿ����ڵ�����
		int nCnt = this->GetItemCount();

		//�����ı�
		LVITEM lvi = { 0 };
		lvi.mask = LVIF_TEXT;
		lvi.iItem = nCnt;
		lvi.iSubItem = 0;
		lvi.pszText = szFilePathName;
		this->InsertItem(&lvi);
	}

	//����ϷŲ���
	DragFinish(hDropInfo);
	CListCtrl::OnDropFiles(hDropInfo);
}
