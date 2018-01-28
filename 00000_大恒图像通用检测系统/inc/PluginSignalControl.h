#pragma once
//**[12/13/2017 ConanSteve]**:  ϵͳ��������,�������Ĳ�ѯ
#include "vector"
#include "array"
#include "DHIOCard.h"

#define MAX_IOCARD_NUM 5//**[12/28/2017 ConanSteve]**:  ���ӿڿ�����
#define MAX_ASSEMBLYLINE_NUM 5 //**[12/28/2017 ConanSteve]**:  �����ˮ�߸���
#define MAX_KICK_QUEUE_NUM 10//**[12/28/2017 ConanSteve]**:  ����޷������
#define MAX_CHECK_GROUP_NUM 20//**[12/28/2017 ConanSteve]**:  ����������
#define MAX_CAMER_COUNT	40//**[12/28/2017 ConanSteve]**:  ����������

using namespace std;
typedef struct s_SysDataInfoInit {//**[1/10/2018 ConanSteve]**:  ��ʼ������ṹ��
	int iSize;
	int iIOCardNum;//**[1/9/2018 ConanSteve]**:  �ӿڿ���Ŀ
	int iAssemblyLineNum;//**[12/13/2017 ConanSteve]**:  ϵͳ����ˮ������ ���5
	int iKickQueueNum;//**[12/13/2017 ConanSteve]**:  �޷϶��и���
	int iCheckGroupNum;//**[12/13/2017 ConanSteve]**:  ��������
	int iGrabNum;//**[12/13/2017 ConanSteve]**:  ������������20�����
	int arr_iKickQueueNumOfEachaAssembly[MAX_ASSEMBLYLINE_NUM];//**[1/10/2018 ConanSteve]**:  ÿ����ˮ�ߵ��޷϶��и���
	int arr_veciKickSNInAssemblyLine[MAX_ASSEMBLYLINE_NUM][MAX_KICK_QUEUE_NUM];//**[12/13/2017 ConanSteve]**:  ÿ����ˮ�߰������޷������к�
	int arr_iCheckGroupNumOfEachKickQueue[MAX_KICK_QUEUE_NUM];
	int arr_veciCheckSNInKickQueue[MAX_KICK_QUEUE_NUM][MAX_CHECK_GROUP_NUM];//**[12/13/2017 ConanSteve]**:  ÿ���޷϶��а����ļ��������,���֧��20���޷϶���
	int arr_iGrabNumOfEachGroup[MAX_CHECK_GROUP_NUM];
	int arr_veciGrabSNInCheckGroup[MAX_CHECK_GROUP_NUM][MAX_CAMER_COUNT];//**[12/13/2017 ConanSteve]**:  ÿ������������������������֧��20�������
	CDHIOCard * arr_pIOCard[MAX_IOCARD_NUM];//**[12/13/2017 ConanSteve]**:  �ӿڿ�ָ������	
}s_SysDataInfoInit;

typedef struct  s_SysDataInfoRealTime
{
	int iSize;
	int  arr_HardImgCnt[MAX_CAMER_COUNT];//**[12/13/2017 ConanSteve]**:  ���ͼ���
	s_KickCountInfo arr_KickCntInfo[MAX_KICK_QUEUE_NUM];//**[1/10/2018 ConanSteve]**:  �޷���Ϣ

}s_SysDataInfoRealTime;

class CSignaleCnt
{
public:
protected:
private:
};