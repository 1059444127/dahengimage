#include "stdafx.h"
#include "DemoProjectDlg.h"

void CDemoProjectDlg::IntegrateCtrls()
{
	m_MainCtrl.push_back(IDC_TAB1);					//����ҳ
	m_MainCtrl.push_back(IDC_STATIC_CMODEL);		//��ǰģ������	Static
	m_MainCtrl.push_back(IDC_STATIC_INFO);			//��ʾģ�������Ϣ Static
	m_MainCtrl.push_back(IDC_BUTTON_FoldPage);
	m_MainCtrl.push_back(IDC_BTN_PARKOUR);			//һ����� PushButton
	m_MainCtrl.push_back(IDC_BTN_SAVESINGLEMODEL);	//���浱ǰ·ģ�� PushButton
	m_MainCtrl.push_back(IDC_BTN_SAVEMODEL);		//����ȫ��ģ��	PushButton
	//m_MainCtrl.push_back(IDC_STATIC_SPLIT);			//
	m_MainCtrl.push_back(IDC_BTN_EXIT2);			//�˳�  PushButton
	m_MainCtrl.push_back(IDC_BTN_UPDMDL);			//����ģ��	PushButton
	m_MainCtrl.push_back(IDC_BTN_MLIST);			//ģ���б���ļ��б��л�	PushButton
	m_MainCtrl.push_back(IDC_STATIC_1);
	m_pMainCtrlPR = new s_CtrlPosRatio[m_MainCtrl.size()];

	m_SideCtrl.push_back(IDC_STATIC_BCK);
	m_SideCtrl.push_back(IDC_STATIC_MLIST);		//ģ���б� static
	m_SideCtrl.push_back(IDC_STATIC_NEWNAME);	//������ģ������ static
	m_SideCtrl.push_back(IDC_MODELLIST);		//ģ���б� listbox
	m_SideCtrl.push_back(IDC_STATIC_KJ);		//�½�ģ�� groupbox
	m_SideCtrl.push_back(IDC_EDIT1);			//ģ������ edittext
	m_SideCtrl.push_back(IDC_NEWMODEL);			//�½�ģ�� pushbutton
	m_SideCtrl.push_back(IDC_BTN_DELETE);		//ɾ��ģ�� pushbutton
	m_SideCtrl.push_back(IDC_BTN_SETD);			//��ΪĬ��ģ��	pushbutton
	m_SideCtrl.push_back(IDC_COMBO_STOPPERTYPE);//��Ͽ� ����ʹ�� δ��
	m_SideCtrl.push_back(IDC_STATIC_FILEBACK);	//����ͼ������� groupbox
	m_SideCtrl.push_back(IDC_STATIC_FILE);		//File ��δʹ��
	m_SideCtrl.push_back(IDC_STATIC_DISK);		//Ӳ�� ��δʹ��
	m_SideCtrl.push_back(IDC_BTN_ERRIMG);		//��λ������ͼ�� PushButton
	m_SideCtrl.push_back(IDC_BTN_GRABIMG);		//��λ������ͼ�� PushButton
	m_SideCtrl.push_back(IDC_BTN_CAPIMG);		//��λ���н���ͼ�� PushButton
	m_SideCtrl.push_back(IDC_BTN_CLRST);		//���	PuchButton
	m_SideCtrl.push_back(IDC_LIST_RESULT);		//�㷨����� ListBox
	m_SideCtrl.push_back(IDC_EDIT_CURPATH);		//��ǰ����ҳ��ͼ��·�� EditText
	m_SideCtrl.push_back(IDC_STATIC_VERSION);	//�汾��Ϣ	Static
	m_SideCtrl.push_back(IDC_EDIT_ModelLog);
	m_SideCtrl.push_back(IDC_CHECK_ShowModelLog);
	m_pSideCtrlPR = new s_CtrlPosRatio[m_SideCtrl.size()];
}
