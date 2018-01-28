#include "stdafx.h"
#include "XDlgSysCfg.h"


XDlgSysCfg::XDlgSysCfg(HWND hParent)
{
	m_hParent = hParent;
}

XDlgSysCfg::XDlgSysCfg(HWND hParent, wchar_t* pwszXML)
{
	m_hParent = hParent;
}


XDlgSysCfg::~XDlgSysCfg()
{
}

bool XDlgSysCfg::Init(HWND hParent)
{
	m_hParent = hParent;
	return true;
}

bool XDlgSysCfg::Create(wchar_t* pwszXML)
{
	//m_hWindow = XWnd_CreateEx(NULL, NULL, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 600, 350, m_hParent, xc_window_style_default); //��������
	//m_hWindow =  XModalWnd_Create(500, 350, L"ϵͳ����", m_hParent);
	bool bRet = XC_LoadResource(L"D:\\Workspace\\Projects\\DahengProjects\\TubeCheck\\bin\\resource\\xml\\resource.res", L"D:\\Workspace\\Projects\\DahengProjects\\TubeCheck\\bin\\resource\\xml");
	bRet=XC_LoadStyle(L"D:\\Workspace\\Projects\\DahengProjects\\TubeCheck\\bin\\resource\\xml\\style_gray.css", L"D:\\Workspace\\Projects\\DahengProjects\\TubeCheck\\bin\\resource\\xml");
	m_hWindow = (HWINDOW)XC_LoadLayout(L"D:\\Workspace\\Projects\\DahengProjects\\TubeCheck\\bin\\resource\\xml\\layout_SystemConfig.xml", m_hParent);
	XWnd_AdjustLayout(m_hWindow);
	if (m_hWindow)
	{
// 		m_hBtnExit = XBtn_Create(10, 10, 80, 25, L"Close", m_hWindow); //������ť
// 		XBtn_SetType(m_hBtnExit, button_type_close);

		//HBKM hBkmExit = XEle_GetBkManager(m_hBtnExit);
		//XBkM_Clear(hBkmExit);
//		XEle_EnableBkTransparent(m_hBtnExit, true);
		//XWnd_SetTransparentType(m_hWindow, window_transparent_shaped);
		//HIMAGE hImgSysBtnCloseHover = XImage_LoadFile(TEXT("res/sysbtn_close_hover.png", image_draw_type_stretch));
		//XBtn_AddBkImage(m_hBtnExit, button_state_stay, hImgSysBtnCloseHover);
		//XBtn_AddBkImage(m_hBtnExit, button_state_leave, XImage_LoadFile(TEXT("res/sysbtn_close_normal.png", image_draw_type_stretch)));
// 		CRect rectBody{ 0 };
// 		XWnd_GetBodyRect(m_hWindow, &rectBody);
// 		XEle_SetRectEx(m_hBtnExit, 1920 - 30, 0, 30, 27, true);
// 		hBkmExit->unused;

		//HBKM hBkmExit = XEle_GetBkManager(m_hBtnExit);
		//XBkM_AddImage(hBkmExit, button_state_stay, XImage_LoadFile(TEXT("res/sysbtn_close_hover.png", true)));


		//HBKM hBkm = XWnd_GetBkManager(m_hWindow);
		//XBkM_AddImage(hBkm, window_state_flag_leave, XImage_LoadFile(L"res/bk.jpg", image_draw_type_stretch));

		//	m_btnStart.Create(_T("Start"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 100, 30), m_pParendWnd, 1000);
		//	SetParent(m_btnStart.m_hWnd, XWnd_GetHWND(m_hWindow));



		XWnd_ShowWindow(m_hWindow, SW_SHOW); //��ʾ����
		RegisterEvent();
		return true;
	}
	//return NULL;
}

int XDlgSysCfg::OnEventBtnClick(BOOL *pBool)
{
	//������Ϣ��
	::PostMessage(AfxGetMainWnd()->m_hWnd, WM_QUIT, 0, 0);
	//CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	//pFrame->OnClose();
	//MessageBoxW(XWnd_GetHWND(m_hWindow), L"�����˰�ť", L"��ʾ", MB_OK);
	//*pBool = false; //�Ը��¼���������
	return 0;    //�¼��ķ���ֵ
}

void XDlgSysCfg::RegisterEvent()
{

	//XEle_RegEventCPP(m_hBtnExit, XE_BNCLICK, &XDlgSysCfg::OnEventBtnClick); //ע�ᰴť����¼�
}

int XDlgSysCfg::DoModal()
{
	if(NULL !=m_hWindow)
		return XModalWnd_DoModal(m_hWindow);
}