#pragma once
/************************************************************************/
/**               [5/3/2017 ConanSteve]**/
enum e_MachineType
{
	e_MachineType_Capsule,//**[7/14/2017 ConanSteve]**:  ����
	e_MachineType_Liquor
};
/**               [5/3/2017 ConanSteve]**/
enum e_WriteDataStatus
{
	e_WriteDataStatus_Leisure,
	e_WriteDataStatus_Busy,
	e_WriteDataStatus_Complete
};
enum e_DetectStatus {
	e_DetectStatus_NoChecking,
	e_DetectStatus_Checking
};

enum e_TimeType//**[10/9/2017 ConanSteve]**:  ��ⳬʱ�ж�����
{
	e_TimeType_MilliSecond,
	e_TimeType_Frame
};
enum e_WhetherDraw {//**[10/9/2017 ConanSteve]**:  ������ж��Ƿ�ͼ
	e_WhetherDraw_Draw,
	e_WhetherDraw_NotDraw
};
enum e_CheckOutWhetherDraw {//**[10/9/2017 ConanSteve]**:  checkOut���ص�ֵ�Ƿ�ͼ
	e_CheckOutWhetherDraw_NotDraw,
	e_CheckOutWhetherDraw_Draw
};


enum e_SaveImgObj {
	e_SaveImgObj_Alg= 0,//**[10/9/2017 ConanSteve]**:  �㷨��ͼ
	e_SaveImgObj_MultiObj= 1,//**[10/9/2017 ConanSteve]**:  ������ͼ
	e_SaveImgObj_System=2//**[11/23/2017 ConanSteve]**:  ϵͳ��ͼ
};

enum e_NamingRegulation {
	e_NamingRegulation_ImgSN,
	e_NamingRegulation_ProductSN
};

enum e_SysDetectMode {
	e_SysDetectMode_Multi=0,//**[7/19/2017 ConanSteve]**:  һ��һ��ͼ(�п�����һ��ͼҲ�п����Ƕ���ͼ)���
	e_SysDetectMode_Single//**[7/19/2017 ConanSteve]**:  һ�ε���ͼ���
};
enum e_ObjDetectMode//**[7/26/2017 ConanSteve]**:  �㷨���ģʽ
{
	e_ObjDetectMode_1,
	e_ObjDetectMode_2
};



/************************************************************************/