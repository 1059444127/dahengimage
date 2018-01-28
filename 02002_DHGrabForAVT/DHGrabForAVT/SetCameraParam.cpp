// SetCameraParam.cpp : implementation file
//

#include "stdafx.h"
#include "AVTGrabber.h"
#include "SetCameraParam.h"
#include "Login.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetCameraParam dialog


CSetCameraParam::CSetCameraParam(CWnd* pParent /*=NULL*/)
	: CDialog(CSetCameraParam::IDD, pParent)
{

	//{{AFX_DATA_INIT(CSetCameraParam)
	m_nBrightness = 0;
	m_nGain = 0;
	m_nShutter = 0;
	m_nBrightnessSlider = 0;
	m_nGainSlider = 0;
	m_nShutterSlider = 0;
	m_nAOILeft = 0;
	m_nAOITop = 0;
	m_nAOIWidth = 0;
	m_nAOIHeight = 0;
	m_nCB = 0;
	m_nCR = 0;
	//}}AFX_DATA_INIT
	m_strProfileName = _T("");
	m_nOldBrightness = 0;
	m_nOldGain = 0;
	m_nOldShutter = 0;
	m_nTriggerMode = 0;
	m_nOldTriggerMode = 0;

	m_iOldCB = 0;  //�޸�ǰ�İ�ƽ��UBֵ
	m_iOldCR = 0;  //�޸�ǰ�İ�ƽ��VRֵ

	m_iMaxWhiteBalCB = 0;     //���İ�ƽ��CB���� 
	m_iMaxWhiteBalCR = 0;     //���İ�ƽ��CR����

	m_iMaxBrightness = 0;     //�������
	m_iMaxGain = 0;           //�������
	m_iMaxShutter = 0;        //����ع�

	m_iMinBrightness = 0;     //��С����
	m_iMinGain = 0;           //��С����
	m_iMinShutter = 0;        //��С�ع�

	m_OldFrameCnt = 0;

	m_strLanType = _T("English");
}


void CSetCameraParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetCameraParam)
	DDX_Control(pDX, IDC_COMBO_TRIGGER_MODE, m_TriggerMode);
	DDX_Text(pDX, IDC_EDIT_BRIGHTNESS, m_nBrightness);
	DDX_Text(pDX, IDC_EDIT_GAIN, m_nGain);
	DDX_Text(pDX, IDC_EDIT_SHUTTER, m_nShutter);
	DDX_Slider(pDX, IDC_SLIDER_BRIGHTNESS, m_nBrightnessSlider);
	DDX_Slider(pDX, IDC_SLIDER_GAIN, m_nGainSlider);
	DDX_Slider(pDX, IDC_SLIDER_SHUTTER, m_nShutterSlider);
	DDX_Text(pDX, IDC_EDIT_AOI_LEFT, m_nAOILeft);
	DDX_Text(pDX, IDC_EDIT_AOI_TOP, m_nAOITop);
	DDX_Text(pDX, IDC_EDIT_AOI_WIDTH, m_nAOIWidth);
	DDX_Text(pDX, IDC_EDIT_AOI_HEIGHT, m_nAOIHeight);
	DDX_Text(pDX, IDC_EDIT_CB, m_nCB);
	DDX_Text(pDX, IDC_EDIT_CR, m_nCR);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetCameraParam, CDialog)
	//{{AFX_MSG_MAP(CSetCameraParam)
	ON_EN_CHANGE(IDC_EDIT_BRIGHTNESS, OnChangeEditBrightness)
	ON_EN_CHANGE(IDC_EDIT_GAIN, OnChangeEditGain)
	ON_EN_CHANGE(IDC_EDIT_SHUTTER, OnChangeEditShutter)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_MODE, OnSelchangeComboTriggerMode)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	ON_BN_CLICKED(IDC_BUTTON_AOI_APPLY, OnButtonAoiApply)
	ON_BN_CLICKED(IDC_BUTTON_FRAMETEST, OnButtonFrametest)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, OnButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_AOI_APPLYWB, OnButtonAoiApplywb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetCameraParam message handlers
void CSetCameraParam::SetOwner(CAVTCamDH *pGrabber, const CString &strProfileName)
{
	m_pGrabber = pGrabber;
	m_strProfileName = strProfileName;
}

//��ʼ���Ի���
BOOL CSetCameraParam::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	SetDiscriptionInfo(m_strLanType);
	ASSERT(m_pGrabber != NULL);

	SetSliderRange();

	UINT32 Result=0;
	UINT32 Brightness=0;
	UINT32 Gain=0;
	UINT32 Shutter=0;
	UINT32 WhiteBalanceUB=0;
	UINT32 WhiteBalanceVR=0;
	UINT32 Trigger=0;

	//////////////////////////////////////////////////////////////////////////
	//��ȡAOI����
	if (m_pGrabber->m_CamParms.ImageFormat < 7)//��Format��ʽ
	{
		GetDlgItem(IDC_EDIT_AOI_LEFT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AOI_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AOI_TOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AOI_HEIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_AOI_APPLY)->EnableWindow(FALSE);
	}
	else
	{
		m_nAOILeft = m_pGrabber->m_CamParms.XPos;
		m_nAOIWidth = m_pGrabber->m_CamParms.XSize;
		m_nAOITop = m_pGrabber->m_CamParms.YPos;
		m_nAOIHeight = m_pGrabber->m_CamParms.YSize;
	}

	if (m_pGrabber->m_CameraActive)
	{
		GetDlgItem(IDC_EDIT_AOI_LEFT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AOI_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AOI_TOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AOI_HEIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_AOI_APPLY)->EnableWindow(FALSE);
	}
	//////////////////////////////////////////////////////////////////////////
	//��ȡ��ǰ֡������������֡��
	m_OldFrameCnt = m_pGrabber->m_FrameCnt;

	//////////////////////////////////////////////////////////////////////////
	//��ȡ�����ǰ���������ع�ֵ ����ģʽ
	Result = m_pGrabber->m_Camera.GetParameter(FGP_BRIGHTNESS, &Brightness); //��ȡ������������ֵ
	m_nBrightness = (int)Brightness;
	m_nBrightnessSlider = m_nBrightness;
	m_nOldBrightness = m_nBrightness;
	Result = m_pGrabber->m_Camera.GetParameter(FGP_GAIN, &Gain);  //��ȡ������������ֵ
	m_nGain = (int)Gain;
	m_nGainSlider = m_nGain;
	m_nOldGain = m_nGain;
	Result = m_pGrabber->m_Camera.GetParameter(FGP_SHUTTER, &Shutter); //��ȡ��������Shutterֵ
	m_nShutter = (int)Shutter;
	m_nShutterSlider = m_nShutter;
	m_nOldShutter = m_nShutter;

//	Result = m_pGrabber->m_Camera.GetParameter(FGP_TRIGGER, &Trigger); //��ȡ�������Ĵ���ģʽ
	Result = m_pGrabber->m_Camera.ReadRegister(0xF0F00830,&Trigger);
	//m_nTriggerMode = m_pGrabber->m_nTriggerMode;
	switch(Trigger) 
	{
	case 0x80100000:
		m_nTriggerMode = 0;  //AVTTriggerOff �ڴ���ģʽ  
		break;
	case 0x83100000:
		m_nTriggerMode = 1;  //AVTTriggerOn  //�����ش���//�ⴥ������
		break;
	case 0x82100000:
		m_nTriggerMode = 2;  //AVTTriggerMode2  //�½��ش���
		break;
	case 0x83110000:
		m_nTriggerMode = 3;  //AVTTriggerMode3 //�ߵ�ƽ����
		break;
	case 0x82110000:
		m_nTriggerMode = 4;  //AVTTriggerMode4  //�͵�ƽ����
		break;
	case 0x831F0000:
		m_nTriggerMode = 5;  //AVTTriggerMode5  //���ģʽ�����ش���
		break;
	case 0x821F0000:
		m_nTriggerMode = 6;  //AVTTriggerMode6  //���ģʽ�½��ش���
		break;
	case 0x82E00000:
		m_nTriggerMode = 7;  //AVTTriggerMode6  //���ģʽ�½��ش���
		break;
	case 0x82E10000:
		m_nTriggerMode = 8;  //AVTTriggerMode6  //���ģʽ�½��ش���
		break;
	case 0x82EF0000:
		m_nTriggerMode = 9;  //AVTTriggerMode6  //���ģʽ�½��ش���
		break;

	default:
		m_nTriggerMode = 0;  //AVTTriggerOff �ڴ���ģʽ
	}

	m_nOldTriggerMode = m_nTriggerMode;
	m_TriggerMode.SetCurSel(m_nTriggerMode);

	if (m_pGrabber->m_bIsColorCamera == TRUE) //����ǲ�ɫ�����򿪰�ƽ�� 
	{
		Result = m_pGrabber->m_Camera.GetParameter(FGP_WHITEBALCB, &WhiteBalanceUB); //��ȡ�������İ�ƽ��UB����
		if (Result == FCE_NOERROR)
		{
			m_nCB = (int)WhiteBalanceUB;

			
		}
		else if(Result == FCE_NOTAVAILABLE)
		{
			GetDlgItem(IDC_EDIT_CB)->EnableWindow(FALSE);
		}
		else
		{
			AfxMessageBox(_T("��ȡ��ƽ��CB����ʧ��!"));
			m_nCB = 0;
		}
	
//		m_ctrlWhiteBalUBSlider.SetPos(m_iWhiteBalanceUB);

		m_iOldCB = m_nCB;


		
		Result = m_pGrabber->m_Camera.GetParameter(FGP_WHITEBALCR, &WhiteBalanceVR); //��ȡ�������İ�ƽ��VR����
		if (Result == FCE_NOERROR)
		{
			m_nCR = (int)WhiteBalanceVR;
		}
		else if(Result == FCE_NOTAVAILABLE)
		{
			GetDlgItem(IDC_EDIT_CR)->EnableWindow(FALSE);
		}
		else
		{
			AfxMessageBox(_T("��ȡ��ƽ��CR����ʧ��!"));
			m_nCR = 0;
		}

		m_iOldCR = m_nCR;

//		m_iWhiteBalanceVR = (int)WhiteBalanceVR;
//		m_ctrlWhiteBalVRSlider.SetPos(m_iWhiteBalanceVR);
//		m_iOldWhiteBalanceVR = m_iWhiteBalanceVR;
	}
	else
	{
 		GetDlgItem(IDC_EDIT_CB)->EnableWindow(FALSE);
 		GetDlgItem(IDC_EDIT_CR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_AOI_APPLYWB)->EnableWindow(FALSE);
	}

	if (Result != 0)
	{
		MessageBox(_T("��ȡ����ڲ���ʧ��,����������!"));
		return FALSE;
	}
	
	UpdateData(FALSE);

	EnableShow(FALSE);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// �������� [4/8/2014 SJC]
void CSetCameraParam::SetDiscriptionInfo(CString strLanType)
{
	if (strLanType==_T("English"))
	{
		SetWindowText(_T("Camera Param Setting Dialog"));
		SetDlgItemText(IDC_STATIC_CAMERA_PARAM,_T("Camera Param"));
		SetDlgItemText(IDC_STATIC_LIGHT,_T("Light"));
		SetDlgItemText(IDC_STATIC_GAIN,_T("Gain"));
		SetDlgItemText(IDC_STATIC_SHUTTER,_T("Shutter"));
		SetDlgItemText(IDC_STATIC_TRIGGER_MODE,_T("Trigger Mode"));
		SetDlgItemText(IDC_STATIC_AOI_PARAM,_T("AOI Param"));
		SetDlgItemText(IDC_STATIC_LEFT,_T("Left"));
		SetDlgItemText(IDC_STATIC_WIDTH,_T("Width"));
		SetDlgItemText(IDC_STATIC_TOP,_T("Top"));
		SetDlgItemText(IDC_STATIC_HEIGHT,_T("Height"));
		SetDlgItemText(IDC_BUTTON_AOI_APPLY,_T("Apply"));
		SetDlgItemText(IDC_STATIC_AOI_INFO,_T("Stop grab first"));
		SetDlgItemText(IDC_STATIC_WB,_T("White balance(Color Camera)"));
		SetDlgItemText(IDC_BUTTON_AOI_APPLYWB,_T("Apply"));
		SetDlgItemText(IDC_STATIC_FPS_TEST,_T("FPS Test"));
		SetDlgItemText(IDC_BUTTON_FRAMETEST,_T("Test"));
		SetDlgItemText(IDC_BUTTON_LOGIN,_T("Login"));
		SetDlgItemText(IDC_BTN_APPLY,_T("Save Param"));
		SetDlgItemText(IDOK,_T("Exit"));

		m_TriggerMode.ResetContent();
		m_TriggerMode.AddString(_T("Internal Grab"));
		m_TriggerMode.AddString(_T("Rising Edge Trigger"));
		m_TriggerMode.AddString(_T("Falling Edge Trigger"));
		m_TriggerMode.AddString(_T("High Level Trigger"));
		m_TriggerMode.AddString(_T("Low Level Trigger"));
		m_TriggerMode.AddString(_T("Prog. High Level Trigger"));
		m_TriggerMode.AddString(_T("Prog. Low Level Trigger"));
		m_TriggerMode.AddString(_T("Edge Soft Trigger"));
		m_TriggerMode.AddString(_T("Level Soft Trigger"));
		m_TriggerMode.AddString(_T("Prog. Soft Trigger"));
	}
	if (strLanType==_T("Chinese"))
	{
		m_TriggerMode.ResetContent();
		m_TriggerMode.AddString(_T("�ڴ���"));
		m_TriggerMode.AddString(_T("�����ش���"));
		m_TriggerMode.AddString(_T("�½��ش���"));
		m_TriggerMode.AddString(_T("�ߵ�ƽ����"));
		m_TriggerMode.AddString(_T("�͵�ƽ����"));
		m_TriggerMode.AddString(_T("����������ⴥ��"));
		m_TriggerMode.AddString(_T("����½����ⴥ��"));
		m_TriggerMode.AddString(_T("������"));
		m_TriggerMode.AddString(_T("��ƽ����"));
		m_TriggerMode.AddString(_T("�������"));
	}
}

// ����SLIDER�ؼ��������Сֵ�ͻ�������
void CSetCameraParam::SetSliderRange()
{
	CSliderCtrl *pSC = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_BRIGHTNESS);
	pSC->SetRange(m_iMinBrightness, m_iMaxBrightness, TRUE);
	pSC = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_GAIN);
	pSC->SetRange(m_iMinGain, m_iMaxGain, TRUE);
	pSC = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_SHUTTER);
	pSC->SetRange(m_iMinShutter, m_iMaxShutter, TRUE);

	// �����ƽ��Ŀ���
//	m_ctrlWhiteBalUBSlider.SetRange(0,m_iMaxWhiteBalCB);
//	m_ctrlWhiteBalVRSlider.SetRange(0,m_iMaxWhiteBalCR);
}

void CSetCameraParam::OnChangeEditBrightness() //����
{
	UpdateData(TRUE);
	if (m_nBrightness > m_iMaxBrightness)
	{
		m_nBrightness = m_iMaxBrightness;
	}

	if (m_nBrightness < m_iMinBrightness)
	{
		m_nBrightness = m_iMinBrightness;
	}

	m_nBrightnessSlider = m_nBrightness;
	UpdateData(FALSE);
	UINT32 Result;
	Result = m_pGrabber->m_Camera.SetParameter(FGP_BRIGHTNESS, m_nBrightness);
}

void CSetCameraParam::OnChangeEditGain() //����
{
	UpdateData(TRUE);
	if (m_nGain >m_iMaxGain)
	{
		m_nGain = m_iMaxGain;
	}
	if (m_nGain < m_iMinGain)
	{
		m_nGain = m_iMinGain;
	}

	m_nGainSlider = m_nGain;
	UpdateData(FALSE);
	UINT32 Result;
	Result = m_pGrabber->m_Camera.SetParameter(FGP_GAIN, m_nGain);
}

void CSetCameraParam::OnChangeEditShutter()  //�ع�
{
	UpdateData(TRUE);
	if (m_nShutter>m_iMaxShutter)
	{
		m_nShutter = m_iMaxShutter;
	}
	if (m_nShutter < m_iMinShutter)
	{
		m_nShutter = m_iMinShutter;
	}
	m_nShutterSlider = m_nShutter;
	UpdateData(FALSE);
	UINT32 Result;
	Result = m_pGrabber->m_Camera.SetParameter(FGP_SHUTTER, m_nShutter);
}


LRESULT CSetCameraParam::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_HSCROLL) 
	{
		int nScrollCode = (int) LOWORD(wParam);
		int nPos = (short int) HIWORD(wParam);   
		HWND hwndScrollBar = (HWND) lParam;
		if (GetDlgItem(IDC_SLIDER_BRIGHTNESS)->GetSafeHwnd() == hwndScrollBar)  //���Ȼ�������Ӧ
		{
			UpdateData(TRUE);
			m_nBrightness = m_nBrightnessSlider;
			UpdateData(FALSE);
			UINT32 Result;
			Result = m_pGrabber->m_Camera.SetParameter(FGP_BRIGHTNESS, m_nBrightness);
		}
		if (GetDlgItem(IDC_SLIDER_GAIN)->GetSafeHwnd() == hwndScrollBar)    //���滬������Ӧ
		{
			UpdateData(TRUE);
			m_nGain = m_nGainSlider;
			UpdateData(FALSE);
			UINT32 Result;
			Result = m_pGrabber->m_Camera.SetParameter(FGP_GAIN, m_nGain);
		}
		if (GetDlgItem(IDC_SLIDER_SHUTTER)->GetSafeHwnd() == hwndScrollBar)  //�ع⻬������Ӧ
		{
			UpdateData(TRUE);
			m_nShutter = m_nShutterSlider;
			UpdateData(FALSE);
			UINT32 Result;
			Result = m_pGrabber->m_Camera.SetParameter(FGP_SHUTTER, m_nShutter);
		}
//		if (GetDlgItem(IDC_SLIDER_WHITEBAL_UB)->GetSafeHwnd() == hwndScrollBar)  //��ƽ��UB������������Ӧ
//		{
//			m_iWhiteBalanceUB = m_ctrlWhiteBalUBSlider.GetPos();
//			UpdateData(FALSE);
//			UINT32 Result;
//			Result = m_pGrabber->m_Camera.SetParameter(FGP_WHITEBALCB, m_iWhiteBalanceUB);
//		}
//		if (GetDlgItem(IDC_SLIDER_WHITEBAL_VR)->GetSafeHwnd() == hwndScrollBar) //��ƽ��VR������������Ӧ
//		{
//			m_iWhiteBalanceVR = m_ctrlWhiteBalVRSlider.GetPos();
//			UpdateData(FALSE);
//			UINT32 Result;
//			Result = m_pGrabber->m_Camera.SetParameter(FGP_WHITEBALCR, m_iWhiteBalanceVR);
//		}
	}	

	return CDialog::DefWindowProc(message, wParam, lParam);
}

//����ģʽ����
void CSetCameraParam::OnSelchangeComboTriggerMode() 
{
	UpdateData(TRUE);
	int nval = m_TriggerMode.GetCurSel();
	m_nTriggerMode = nval;

	int  eTriggerValue; 

	switch(m_nTriggerMode) 
	{
	case 0:
		eTriggerValue = 0;  //AVTTriggerOff �ڴ���ģʽ  
		break;
	case 1:
		eTriggerValue = 1;  //AVTTriggerOn  //�����ش���//�ⴥ������
		break;
	case 2:
		eTriggerValue = 2;  //AVTTriggerMode2  //�½��ش���
		break;
	case 3:
		eTriggerValue = 3;  //AVTTriggerMode3 //�ߵ�ƽ����
		break;
	case 4:
		eTriggerValue = 4;  //AVTTriggerMode4  //�͵�ƽ����
		break;
	case 5:
		eTriggerValue = 5;  //AVTTriggerMode5  //���ģʽ�����ش���
		break;
	case 6:
		eTriggerValue = 6;  //AVTTriggerMode6  //���ģʽ�½��ش���
		break;
	case 7:
		eTriggerValue = 7;  //AVTTriggerMode6  //���ģʽ�½��ش���
		break;
	case 8:
		eTriggerValue = 8;  //AVTTriggerMode6  //���ģʽ�½��ش���
		break;
	case 9:
		eTriggerValue = 9;  //AVTTriggerMode6  //���ģʽ�½��ش���
		break;
	default:
		eTriggerValue = 0;  //AVTTriggerOff �ڴ���ģʽ
	}

	m_pGrabber->SetTriggerParam(eTriggerValue);
	UpdateData(FALSE);
}

void CSetCameraParam::OnBtnApply() //����ǰ�������浽�����ļ���
{
	CString str;
	str.Format(_T("%d"), m_nBrightness);
	WritePrivateProfileString(_T("�������"),_T("Brightness"),str,m_strProfileName);
	str.Format(_T("%d"), m_nShutter);
	WritePrivateProfileString(_T("�������"),_T("Shutter"),str,m_strProfileName);
	str.Format(_T("%d"), m_nGain);
	WritePrivateProfileString(_T("�������"),_T("Gain"),str,m_strProfileName);
	str.Format(_T("%d"), m_nTriggerMode);
 	WritePrivateProfileString(_T("�������"),_T("Trigger"),str,m_strProfileName);

	str.Format(_T("%d"), m_nAOILeft);
	WritePrivateProfileString(_T("�������"),_T("Left"),str,m_strProfileName);
	str.Format(_T("%d"), m_nAOIWidth);
	WritePrivateProfileString(_T("�������"),_T("Width"),str,m_strProfileName);
	str.Format(_T("%d"), m_nAOITop);
	WritePrivateProfileString(_T("�������"),_T("Top"),str,m_strProfileName);
	str.Format(_T("%d"), m_nAOIHeight);
 	WritePrivateProfileString(_T("�������"),_T("Height"),str,m_strProfileName);

	str.Format(_T("%d"), m_nCB);
 	WritePrivateProfileString(_T("�������"),_T("Whitebalcb"),str,m_strProfileName);
	str.Format(_T("%d"), m_nCR);
 	WritePrivateProfileString(_T("�������"),_T("Whitebalcr"),str,m_strProfileName);


}

void CSetCameraParam::OnOK() 
{
	CString str;
	
	GetDlgItem(IDC_BUTTON_FRAMETEST)->GetWindowText(str);
	
	if (str == _T("ֹͣ����"))
	{
		KillTimer(100);
	}

	CDialog::OnOK();
}


void CSetCameraParam::OnClose() 
{
	UpdateData(TRUE);
	CDialog::OnClose();
}

void CSetCameraParam::OnButtonAoiApply() 
{
	if (m_pGrabber->m_CameraActive)
	{
		AfxMessageBox(_T("���ڲɼ�,��������AOI����!"));
		return;
	}

	if (m_pGrabber->m_CamParms.ImageFormat < 7)
	{
		AfxMessageBox(_T("AVTSetAOI ��Format7ģʽ"));
		return;
	}

	UINT32 Result;
	CString str;
	UpdateData(TRUE);

	if (m_nAOIWidth > m_pGrabber->m_CamParms.XSizeMax)
	{
		m_nAOIWidth = m_pGrabber->m_CamParms.XSizeMax;
	}
				
	if (m_nAOIWidth < m_pGrabber->m_CamParms.XSizeMin)
	{
		m_nAOIWidth = m_pGrabber->m_CamParms.XSizeMin;
	}

	if (m_nAOIWidth%m_pGrabber->m_CamParms.XSizeUnit != 0)
	{
		m_nAOIWidth-=m_nAOIWidth % m_pGrabber->m_CamParms.XSizeUnit;
	}

	if (m_nAOILeft > m_pGrabber->m_CamParms.XPosMax)
	{
		m_nAOILeft = m_pGrabber->m_CamParms.XPosMax;
	}
				
	if (m_nAOILeft < m_pGrabber->m_CamParms.XPosMin)
	{
		m_nAOILeft = m_pGrabber->m_CamParms.XPosMin;
	}

	if (m_nAOILeft%m_pGrabber->m_CamParms.XPosUnit != 0)
	{
		m_nAOILeft-=m_nAOILeft % m_pGrabber->m_CamParms.XPosUnit;
	}
				
	if (m_nAOIWidth + m_nAOILeft > m_pGrabber->m_CamParms.XSizeMax)
	{
		str.Format(_T("���+��� > %d �������趨!"),m_pGrabber->m_CamParms.XSizeMax);
		AfxMessageBox(str);
		return;
	}
				


	if (m_nAOITop > m_pGrabber->m_CamParms.YPosMax)
	{
		m_nAOITop = m_pGrabber->m_CamParms.YPosMax;
	}
	if (m_nAOITop < m_pGrabber->m_CamParms.YPosMin)
	{
		m_nAOITop = m_pGrabber->m_CamParms.YPosMin;
	}
	if (m_nAOITop%m_pGrabber->m_CamParms.YPosUnit != 0)
	{
		m_nAOITop-=m_nAOITop % m_pGrabber->m_CamParms.YPosUnit;
	}

	if (m_nAOIHeight > m_pGrabber->m_CamParms.YSizeMax)
	{
		m_nAOIHeight = m_pGrabber->m_CamParms.YSizeMax;
	}
				
	if (m_nAOIHeight < m_pGrabber->m_CamParms.YSizeMin)
	{
		m_nAOIHeight = m_pGrabber->m_CamParms.YSizeMin;
	}

	if (m_nAOIHeight%m_pGrabber->m_CamParms.YSizeUnit != 0)
	{
		m_nAOIHeight-=m_nAOIHeight % m_pGrabber->m_CamParms.YSizeUnit;
	}
				
	if (m_nAOIHeight + m_nAOITop > m_pGrabber->m_CamParms.YSizeMax)
	{
		str.Format(_T("�ϱ�+�߶� > %d �������趨!"),m_pGrabber->m_CamParms.YSizeMax);
		AfxMessageBox(str);
		return;
	}


	// Now we resize
	m_pGrabber->m_Camera.SetParameter(FGP_RESIZE,1);
				
	//////////////////////////////////////////////////////////////////////////
	//����AOI����
	if (m_nAOIWidth != m_pGrabber->m_CamParms.XSize 
		|| m_nAOILeft != m_pGrabber->m_CamParms.XPos)
	{
		if(m_nAOIWidth + m_pGrabber->m_CamParms.XPos <= m_pGrabber->m_CamParms.XSizeMax)
		{
			if (m_nAOIWidth != m_pGrabber->m_CamParms.XSize)
			{
				Result = m_pGrabber->m_Camera.SetParameter(FGP_XSIZE,m_nAOIWidth);
				if(Result==FCE_NOERROR)
				{
					m_pGrabber->m_CamParms.XSize = m_nAOIWidth;			
				}
				else
				{
					str.Format(_T("����AOI���%dʧ��!"),m_nAOIWidth);
					AfxMessageBox(str);
					return;
				}
			}


			if (m_nAOILeft != m_pGrabber->m_CamParms.XPos)
			{
				Result = m_pGrabber->m_Camera.SetParameter(FGP_XPOSITION,m_nAOILeft);
				if(Result==FCE_NOERROR)
				{
					m_pGrabber->m_CamParms.XPos = m_nAOILeft;
				}
				else
				{
					str.Format(_T("����AOI���%dʧ��!"),m_nAOILeft);
					AfxMessageBox(str);
					return;
				}
			}

		}
		else
		{
			if (m_nAOILeft != m_pGrabber->m_CamParms.XPos)
			{
				Result = m_pGrabber->m_Camera.SetParameter(FGP_XPOSITION,m_nAOILeft);
				if(Result==FCE_NOERROR)
				{
					m_pGrabber->m_CamParms.XPos = m_nAOILeft;
				}
				else
				{
					str.Format(_T("����AOI���%dʧ��!"),m_nAOILeft);
					AfxMessageBox(str);
					return;
				}
			}


			if (m_nAOIWidth != m_pGrabber->m_CamParms.XSize)
			{
				Result = m_pGrabber->m_Camera.SetParameter(FGP_XSIZE,m_nAOIWidth);
				if(Result==FCE_NOERROR)
				{
					m_pGrabber->m_CamParms.XSize = m_nAOIWidth;			
				}
				else
				{
					str.Format(_T("����AOI���%dʧ��!"),m_nAOIWidth);
					AfxMessageBox(str);
					return;
				}
			}
		}
	}

	if (m_nAOIHeight != m_pGrabber->m_CamParms.YSize 
		|| m_nAOITop != m_pGrabber->m_CamParms.YPos)
	{
		if(m_nAOIHeight + m_pGrabber->m_CamParms.YPos <= m_pGrabber->m_CamParms.YSizeMax)
		{

			if (m_nAOIHeight != m_pGrabber->m_CamParms.YSize)
			{
				Result = m_pGrabber->m_Camera.SetParameter(FGP_YSIZE,m_nAOIHeight);
				if(Result==FCE_NOERROR)
				{
					m_pGrabber->m_CamParms.YSize = m_nAOIHeight;
				}
				else
				{
					str.Format(_T("����AOI�߶�%dʧ��!"),m_nAOIHeight);
					AfxMessageBox(str);
					return;
				}
			}

			
			if (m_nAOITop != m_pGrabber->m_CamParms.YPos)
			{
				Result = m_pGrabber->m_Camera.SetParameter(FGP_YPOSITION,m_nAOITop);
				if(Result==FCE_NOERROR)
				{
					m_pGrabber->m_CamParms.YPos = m_nAOITop;
				}
				else
				{
					str.Format(_T("����AOI�ϱ�%dʧ��!"),m_nAOITop);
					AfxMessageBox(str);
					return;
				}
			}			
		}
		else
		{
			if (m_nAOITop != m_pGrabber->m_CamParms.YPos)
			{
				Result = m_pGrabber->m_Camera.SetParameter(FGP_YPOSITION,m_nAOITop);
				if(Result==FCE_NOERROR)
				{
					m_pGrabber->m_CamParms.YPos = m_nAOITop;
				}
				else
				{
					str.Format(_T("����AOI�ϱ�%dʧ��!"),m_nAOITop);
					AfxMessageBox(str);
					return;
				}
			}


			if (m_nAOIHeight != m_pGrabber->m_CamParms.YSize)
			{
				Result = m_pGrabber->m_Camera.SetParameter(FGP_YSIZE,m_nAOIHeight);
				if(Result==FCE_NOERROR)
				{
					m_pGrabber->m_CamParms.YSize = m_nAOIHeight;
				}
				else
				{
					str.Format(_T("����AOI�߶�%dʧ��!"),m_nAOIHeight);
					AfxMessageBox(str);
					return;
				}
			}		
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//F7M0_BYTE_PER_PACKET

	UINT32 nTemp = m_pGrabber->m_CamParms.ImageFormat-7;
	UINT32 nTemp1 = nTemp<<12;
	UINT32 Address = m_pGrabber->m_CamParms.Format7ModeAdress+0x44+nTemp1;//0xF0F08044+nTemp1;
	
	UINT32 Value;
	Result = m_pGrabber->m_Camera.ReadRegister(Address,&Value);//F7M0_BYTE_PER_PACKET
	if(Result==FCE_NOERROR)
	{
		//////////////////////////////////////////////////////////////////////////
		UINT32  Value1 = 0xffff0000;
		UINT32  Value2 = 0x0;
		Value2 = Value&Value1;
		Value1 = Value2>>16;
		
		UINT32 Value3 = 0xffff;
		UINT32 Value4 = 0x0;
		Value4= Value&Value3;

		if(Value1 != Value4)
		{
			//////////////////////////////////////////////////////////////////////////
			Value1 = Value4<<16;
			Value1 +=Value4;	
			Result = m_pGrabber->m_Camera.WriteRegister(Address,Value1);//F7M0_BYTE_PER_PACKET
	
			if(Result==FCE_NOERROR)
			{
			}
			else
			{
				AfxMessageBox(_T("���ð���ʧ��!"));
				return;
			}	
		}
	}
	else
	{
		AfxMessageBox(_T("��ȡ����ʧ��!"));
		return;
	}


	m_pGrabber->m_Camera.SetParameter(FGP_RESIZE,0);
	//////////////////////////////////////////////////////////////////////////
	//��������
	if ((UINT32)(m_pGrabber->m_nImageWidth) != m_pGrabber->m_CamParms.XSize
		||(UINT32)(m_pGrabber->m_nImageHeight) != m_pGrabber->m_CamParms.YSize)
	{
		m_pGrabber->m_nImageWidth  = m_pGrabber->m_CamParms.XSize;
		m_pGrabber->m_nImageHeight = m_pGrabber->m_CamParms.YSize;	
		m_pGrabber->m_nImageBufferSize = m_pGrabber->m_nImageWidth * m_pGrabber->m_nImageHeight * m_pGrabber->m_nImageByteCount; 
		
		if(m_pGrabber->m_pbImageBufferAdd != NULL)
		{
			delete [](m_pGrabber->m_pbImageBufferAdd);
			m_pGrabber->m_pbImageBufferAdd = NULL;
		}
//		m_pGrabber->m_pbImageBufferAdd = new BYTE[m_pGrabber->m_nImageWidth*m_pGrabber->m_nImageHeight*3];

	}

// 	str.Format(_T("%d"), m_nAOILeft);
// 	WritePrivateProfileString(_T("�������"),_T("Left"),str,m_strProfileName);
// 	str.Format(_T("%d"), m_nAOIWidth);
// 	WritePrivateProfileString(_T("�������"),_T("Width"),str,m_strProfileName);
// 	str.Format(_T("%d"), m_nAOITop);
// 	WritePrivateProfileString(_T("�������"),_T("Top"),str,m_strProfileName);
// 	str.Format(_T("%d"), m_nAOIHeight);
//  WritePrivateProfileString(_T("�������"),_T("Height"),str,m_strProfileName);

	UpdateData(FALSE);

}

void CSetCameraParam::OnButtonFrametest() 
{
	CString str;

	GetDlgItem(IDC_BUTTON_FRAMETEST)->GetWindowText(str);

	if (str == _T("��ʼ����"))
	{
		//////////////////////////////////////////////////////////////////////////
		//��ȡ��ǰ֡������������֡��
		m_OldFrameCnt = m_pGrabber->m_FrameCnt;
		SetTimer(100,1000,NULL);

		GetDlgItem(IDC_BUTTON_FRAMETEST)->SetWindowText(_T("ֹͣ����"));
	}
	else if (str == _T("ֹͣ����"))
	{
		KillTimer(100);
		GetDlgItem(IDC_STATIC_FRAME)->SetWindowText(_T(""));
		GetDlgItem(IDC_BUTTON_FRAMETEST)->SetWindowText(_T("��ʼ����"));
	}
	
}

void CSetCameraParam::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 100)
	{
		CString str;
		str.Format(_T("%d֡/��"),m_pGrabber->m_FrameCnt-m_OldFrameCnt);
		m_OldFrameCnt = m_pGrabber->m_FrameCnt;
		GetDlgItem(IDC_STATIC_FRAME)->SetWindowText(str);
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CSetCameraParam::OnButtonLogin() 
{
	// TODO: Add your control notification handler code here
//	UpdateData(TRUE);
//	Login login(m_strProfileName);
//	login.DoModal();

//	if (login.m_bLogin)
//	{
		EnableShow(TRUE);
		GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(FALSE);
//	}

}

void CSetCameraParam::EnableShow(BOOL bEnable)
{
	if (bEnable)
	{
		GetDlgItem(IDC_SLIDER_BRIGHTNESS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_BRIGHTNESS)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLIDER_GAIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLIDER_SHUTTER)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SHUTTER)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_TRIGGER_MODE)->EnableWindow(TRUE);
		if (m_pGrabber->m_CameraActive)
		{
			GetDlgItem(IDC_EDIT_AOI_HEIGHT)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_AOI_WIDTH)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_AOI_LEFT)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_AOI_TOP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_AOI_APPLY)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_EDIT_AOI_HEIGHT)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_AOI_WIDTH)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_AOI_LEFT)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_AOI_TOP)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_AOI_APPLY)->EnableWindow(TRUE);
		}

	
		GetDlgItem(IDC_BUTTON_FRAMETEST)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_APPLY)->EnableWindow(TRUE);


//////////////////////////////////////////////////////////////////////////
		if (m_pGrabber->m_bIsColorCamera == FALSE) //����ǲ�ɫ�����򿪰�ƽ�� 
		{
			GetDlgItem(IDC_EDIT_CB)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_CR)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_AOI_APPLYWB)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_EDIT_CB)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_CR)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_AOI_APPLYWB)->EnableWindow(TRUE);
		}

//////////////////////////////////////////////////////////////////////////

	}
	else
	{
		GetDlgItem(IDC_SLIDER_BRIGHTNESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BRIGHTNESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_GAIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_SHUTTER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SHUTTER)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_TRIGGER_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AOI_LEFT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AOI_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AOI_TOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AOI_HEIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_AOI_APPLY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_FRAMETEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_APPLY)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CB)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_AOI_APPLYWB)->EnableWindow(FALSE);


	}
}



void CSetCameraParam::OnButtonAoiApplywb() 
{
	UINT32 Result;
	CString str;
	UpdateData(TRUE);


	if (m_nCB > m_pGrabber->m_CamParms.WhiteBalanceMaxCB)
	{
		m_nCB = m_pGrabber->m_CamParms.WhiteBalanceMaxCB;
	}
				
	if (m_nCB < m_pGrabber->m_CamParms.WhiteBalanceMinCB)
	{
		m_nCB = m_pGrabber->m_CamParms.WhiteBalanceMinCB;
	}

	if(m_nCB != m_iOldCB)
	{
		Result=m_pGrabber->m_Camera.SetParameter(FGP_WHITEBALCB,m_nCB);
		if (Result==FCE_NOERROR)
		{
			m_pGrabber->m_CamParms.WhiteBalanceCB=m_nCB;
			m_iOldCB = m_nCB;
		}
		else
		{
			AfxMessageBox(_T("���ð�ƽ��CBʧ��!"));
		}
	}


	if (m_nCR > m_pGrabber->m_CamParms.WhiteBalanceMaxCR)
	{
		m_nCR = m_pGrabber->m_CamParms.WhiteBalanceMaxCR;
	}
				
	if (m_nCR < m_pGrabber->m_CamParms.WhiteBalanceMinCR)
	{
		m_nCR = m_pGrabber->m_CamParms.WhiteBalanceMinCR;
	}

	if(m_nCR != m_iOldCR)
	{
		Result=m_pGrabber->m_Camera.SetParameter(FGP_WHITEBALCR,m_nCR);
		if (Result==FCE_NOERROR)
		{
			m_pGrabber->m_CamParms.WhiteBalanceCR=m_nCR;
			m_iOldCR = m_nCR;
		}
		else
		{
			AfxMessageBox(_T("���ð�ƽ��CRʧ��!"));
		}
	}

	UpdateData(FALSE);

}
