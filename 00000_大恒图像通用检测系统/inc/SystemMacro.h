#pragma once
#define MAX_IOCARD_NUM 5//**[12/28/2017 ConanSteve]**:  ���ӿڿ�����
#define MAX_ASSEMBLYLINE_NUM 5 //**[12/28/2017 ConanSteve]**:  �����ˮ�߸���
#define MAX_KICK_QUEUE_NUM 10//**[12/28/2017 ConanSteve]**:  ����޷������
#define MAX_CHECK_GROUP_NUM 20//**[12/28/2017 ConanSteve]**:  ����������
#define MAX_CAMER_COUNT	40//**[12/28/2017 ConanSteve]**:  ����������

enum eSaveImageType
{
	SaveGrabImage = 0,                // ���������ɼ�ͼ�� [8/19/2009 GZ]
	SaveErrorImage,					// �������ͼ��
	SaveAbnormityImage,				// �����쳣ͼ��
	SaveCorrectImage,				// ������ȷͼ��
	SaveModelImage					// ����ģ��ͼ��
};

enum e_ProductLine
{
	e_ProductLine_NO,//**[11/15/2017 ConanSteve]**:  �ޣ�ϵͳ�˳�
	e_ProductLine_General,//**[11/21/2017 ConanSteve]**:  ͨ���㷨
	e_ProductLine_Stopper,//**[11/21/2017 ConanSteve]**:  �����㷨
	e_ProductLine_LampInsp,//**[11/24/2017 ConanSteve]**:  �Ƽ���㷨
	e_ProductLine_LableCheck,//**[12/12/2017 ConanSteve]**:  ��Ѫ���㷨
	e_ProductLine_BlisterOnline//**[12/14/2017 ConanSteve]**:  ���������㷨���ṹ����Generalһ���������ڼ����޷Ͻ��ʱ��һ���߷ϻص�������޷Ͻ��
};

enum e_KickType {
	e_KickType_Single,//**[12/14/2017 ConanSteve]**:  ��·�޷ϣ��ۺ϶�·�������ֻдһ���޷Ͻ�����ӿڿ�
	e_KickType_Multi_Vertical,//**[12/14/2017 ConanSteve]**:  ��·�޷ϣ������޷ϻص���ÿ����������ֱ�дһ���߷Ͽڣ������������ߣ�ƿ�Ƿ�ɫ,��������ֻ��һ����ʵ��Ч����ͬ��·�޷�
	e_KickType_Multi_Horizontal//**[12/14/2017 ConanSteve]**:  ��·�޷�ˮƽģʽ���Խ���Ϊ����ÿ����Ʒˮƽ8����⹤λ����ֱ4�����������ÿ���޷�Ҳд4�����
};

enum e_SaveImgType {
	e_SaveImgType_NO,//**[11/23/2017 ConanSteve]**:  ����ͼ
	e_SaveImgType_Consecutive,//**[11/23/2017 ConanSteve]**:  ������ͼ��
	e_SaveImgType_Error,//**[11/23/2017 ConanSteve]**:  ����ͼ��
	e_SaveImgType_ProductExist//**[11/23/2017 ConanSteve]**:  �в�Ʒ��ͼ
};


enum e_CheckTotleCntType {//**[12/4/2017 ConanSteve]**:  ���������ͳ������
	e_CheckTotleCntType_PE,//**[12/4/2017 ConanSteve]**:  �������
	e_CheckTotleCntType_Rlst//**[12/4/2017 ConanSteve]**:  1��λ������в�Ʒ��������������
};

enum e_AlgStatus {
	e_AlgStatus_Good,//**[12/5/2017 ConanSteve]**:  ��Ʒ
	e_AlgStatus_Empty,
	e_AlgStatus_Bad,//**[12/5/2017 ConanSteve]**:  ��Ʒ
	e_AlgStatus_AlgException,//**[12/5/2017 ConanSteve]**:  �㷨�쳣
	e_AlgStatus_LostImg,//**[12/5/2017 ConanSteve]**:  �����ͼ
	e_AlgStatus_Overtime//**[12/5/2017 ConanSteve]**:  ��ⳬʱ
};;
