#pragma once

/*************************************<��̬���û��������ؼ���ԴID>************************************[8/28/2017 ConanSteve]*/

#define IDR_USER 0x00010000

#define ID_STATIC_CAMERA_IMAGE IDR_USER+0
#define ID_STATIC_ALGORITHM_IMAGE IDR_USER+50


//**[12/8/2017 ConanSteve]**:  ��̬�ı���Դ
#define IDC_STATIC_CHECK_COUNT IDR_USER+100//**[12/1/2017 ConanSteve]**:  �������
#define IDC_STATIC_GOOD_COUNT IDR_USER+101//**[12/4/2017 ConanSteve]**:  ��Ʒ����
#define IDC_STATIC_ERROR_COUNT IDR_USER+102//**[12/4/2017 ConanSteve]**:  ��������
#define IDC_STATIC_GOOD_RATE IDR_USER+103//**[12/4/2017 ConanSteve]**:  ��Ʒ��


//**[12/27/2017 ConanSteve]**:  ��ť��Դ�����ǲ����¼���ֻ����Ϊ��ʾʹ��


//**[1/24/2018 ConanSteve]**:  ���֧��5����ˮ��
/*************************************<200>************************************[12/28/2017 ConanSteve]*/
#define		IDC_BUTTON_LABEL_TOTLE_COUNT_ASSEMBLY	IDR_USER +200 //**[1/24/2018 ConanSteve]**:  ��ˮ�߼������
#define		IDC_BUTTON_LABEL_GOOD_COUNT_ASSEMBLY		IDR_USER+205//**[12/27/2017 ConanSteve]**:  ��ˮ�ߺ�Ʒ����
#define		IDC_BUTTON_LABEL_BAD_COUNT_ASSEMBLY		IDR_USER+210//**[12/27/2017 ConanSteve]**:  ��ˮ�߻�Ʒ����
#define		IDC_BUTTON_LABEL_GOOD_RATIO_ASSEMBLY		IDR_USER+215//**[12/27/2017 ConanSteve]**:  ��ˮ�ߺϸ���
#define		IDC_BUTTON_LABEL_CHECK_SPEED_ASSEMBLY				IDR_USER +220//**[1/24/2018 ConanSteve]**:	������ˮ�ߵļ���ٶ� 

/*************************************<250>************************************[12/28/2017 ConanSteve]*/

#define		IDC_BUTTON_LABEL_BEGIN				IDR_USER+250//**[12/27/2017 ConanSteve]**:  ��ͷ
#define		IDC_BUTTON_LABEL_END				IDC_BUTTON_LABEL_BEGIN+1+7  //**[1/26/2018 ConanSteve]**:	���һ���������һ���ؼ��ĸ���ֵһ�� 

#define		IDC_BUTTON_LABEL_MODEL_NAME						IDC_BUTTON_LABEL_BEGIN+0//**[1/24/2018 ConanSteve]**:  ģ������
#define		IDC_BUTTON_LABEL_USER_INFO						IDC_BUTTON_LABEL_BEGIN+1
#define		IDC_BUTTON_LABEL_TIME							IDC_BUTTON_LABEL_BEGIN+2//**[1/26/2018 ConanSteve]**:	��ǰʱ�� 
#define		IDC_BUTTON_LABEL_CHECK_COUNT_TOTLE					IDC_BUTTON_LABEL_BEGIN+3//**[1/26/2018 ConanSteve]**:	������� 
#define		IDC_BUTTON_LABEL_GOOD_COUNT_TOTLE						IDC_BUTTON_LABEL_BEGIN+4//**[1/26/2018 ConanSteve]**:	��Ʒ���� 
#define		IDC_BUTTON_LABEL_ERROR_COUNT_TOTLE					IDC_BUTTON_LABEL_BEGIN+5//**[1/26/2018 ConanSteve]**:	��Ʒ���� 
#define		IDC_BUTTON_LABEL_GOOD_RATE_TOTLE						IDC_BUTTON_LABEL_BEGIN+6//**[1/26/2018 ConanSteve]**:	�ϸ��� 
#define		IDC_BUTTON_LABEL_CHECK_SPEED					IDC_BUTTON_LABEL_BEGIN+7//**[1/26/2018 ConanSteve]**:	����ٶ� 



//**[12/8/2017 ConanSteve]**:  �����ؼ���Դ
#define IDC_LISTCTRL_REALTIME_RUN_INFO   IDR_USER+280//**[12/8/2017 ConanSteve]**: ������ϢCListCtrl�ؼ� 





//**[12/8/2017 ConanSteve]**:  ��ť��Դ,����100����ť�¼�
#define IDC_BUTTON_USER WM_USER+300 //**[1/26/2018 ConanSteve]**:	��ΪҪ���¼������Ա����� WM_SUER 

#define IDC_BUTTON_BEGIN			IDC_BUTTON_USER
#define IDC_BUTTON_END				IDC_BUTTON_USER+1+11 //**[1/26/2018 ConanSteve]**:	���һ���������һ���ؼ��ĸ���ֵһ�� 

#define IDC_BUTTON_RESET_SYSTEM					 IDC_BUTTON_BEGIN+0  //**[12/27/2017 ConanSteve]**:  ϵͳ��������
#define IDC_BUTTON_OPEN_IOCARD_TOOLS			 IDC_BUTTON_BEGIN+1 //**[12/8/2017 ConanSteve]**:  �л�ģ�尴ť
#define IDC_BUTTON_SWITCH_MODEL					 IDC_BUTTON_BEGIN+2 //**[12/8/2017 ConanSteve]**:  �򿪽ӿڿ����߰�ť
#define IDC_BUTTON_RECORD  							IDC_BUTTON_BEGIN+3 //**[12/13/2017 ConanSteve]**:  ��¼��־��ť
#define IDC_BUTTON_DEFECT_STATISTICS					IDC_BUTTON_BEGIN+4//**[12/27/2017 ConanSteve]**:  ȱ��ͳ��
#define IDC_BUTTON_DATA_STATISTICS						IDC_BUTTON_BEGIN+5//**[1/26/2018 ConanSteve]**:	����ͳ�� 
#define IDC_BUTTON_OPEN_MULTI_OBJECT_RUN_INFO_WINDOW	IDC_BUTTON_BEGIN+6//**[1/2/2018 ConanSteve]**:	�����������Ϣ
#define IDC_BUTTON_USER_MANAGEMENT						IDC_BUTTON_BEGIN+7//**[1/26/2018 ConanSteve]**:	�û����� 
#define IDC_BUTTON_SUPER_ADMIN_SETTING					IDC_BUTTON_BEGIN+8//**[1/26/2018 ConanSteve]**:	��������Ա�������� 
#define IDC_BUTTON_SYSTEM_DEVELOPER_SETTING					IDC_BUTTON_BEGIN+9//**[1/26/2018 ConanSteve]**:	ϵͳ�з���Ա�������� 
#define IDC_BUTTON_RESIZE_LAYOUT						IDC_BUTTON_BEGIN+10//**[1/26/2018 ConanSteve]**:	���µ������沼�� 
#define IDC_BUTTON_SWITCH_USER							IDC_BUTTON_BEGIN+11//**[1/26/2018 ConanSteve]**:	�л��û� 






//**[12/4/2017 ConanSteve]**:  ��ʱ��
#define ID_EVENT_UPDATE_TIME IDR_USER+1000//**[12/4/2017 ConanSteve]**:  ʱ��
#define ID_EVENT_UPDATE_INFO IDR_USER+1001//**[12/4/2017 ConanSteve]**:  �����Ϣ����
#define ID_EVENT_UPDATE_CHECK_SPEED IDR_USER+1002//**[12/28/2017 ConanSteve]**:  ��Ʒ�ٶȸ���
#define ID_EVENT_START_GRABBER IDR_USER+1003


#define ID_INDICATOR_GRAB               61446
#define ID_INDICATOR_CHECK              61447
#define ID_INDICATOR_ERROR              61448
#define ID_INDICATOR_OPERATION          61449


#define WM_USER_CLOSE WM_USER+1
