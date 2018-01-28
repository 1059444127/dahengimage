#pragma once
class CImageOfSys
{
public:
	CImageOfSys();
	~CImageOfSys();
};

/*************************************<ͼ������>************************************[9/6/2017 ConanSteve]*/
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
enum e_ShowModeFlip	//��ת��ʾ��ʽ
{
	NoFlip = 0,					//����ת��ʾ
	FlipVertical,				//��ֱ��ת��ʾ
	FlipHorizintal,				//ˮƽ��ת��ʾ
	Flip						//ȫ��ת��ʾ
};	//��ת��ʾ��ʽ

enum e_ShowModeStretch	//������ʾ��ʽ
{
	NoStretch = 0,				//��������ʾ
	Stretch						//������ʾ
};	//������ʾ��ʽ

enum eAutoSaveErrorInfo
{
	GrabAllFaultError = -9,
	ParamTypeError,				// ����2�������ʹ������ [8/19/2009 GZ]
	ParamLPElementError,			// ����1����Ԫ��ָ��Ϊ�� [8/19/2009 GZ]
	ListCountError,					// ��õ�Ԫ���е��������Ϊ����� [8/19/2009 GZ]
	SaveBMPError,					// ����SaveBMPFile()δ�ɹ� [8/19/2009 GZ]
	CreatePathError,				// ����·������
	FileNameError,					// �ļ�������
	ImageWHBITError,				// ͼƬ����λ������
	ImageBufferError				// ͼ������������
};
class CImgHandle
{
	//**[9/6/2017 ConanSteve]**:  ��ͼ��������
	static bool MirrorSingleImage(BYTE* pImageBuff, int lImageWidth, int lImageHeight, int lImageBitCount, e_ShowModeFlip  eShowMode);
};

