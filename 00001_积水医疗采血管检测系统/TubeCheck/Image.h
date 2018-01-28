/************************************************************************/
/* Copyright (c) 2005, �й���㼯�ű���ͼ���Ӿ������ֹ�˾�Ӿ�������     
/* All rights reserved.													
/*																		
/* �ļ����ƣ� Image.h												
/* �ļ���ʶ�� �����ù���ƻ���
/* ժҪ�� CImage��, �Ա���һ��ͼ��ĸ����ݽṹ���з�װ, ʵ����ͼ�񿽱�,
/*    ͼ����ʾ, ͼ�����, ͼ����еȹ���
/*
/* ��ǰ�汾�� 1.1
/* ���ߣ� ��׿��
/* ������ڣ� 2005��10��10��
/*�޸�: ����AutoSaveImageToPath()������  SaveMeToPath()�Զ�����ͼ���ȶ�·�� ����//  [8/19/2009 GZ]
/* ���뱣��С��ͼ����SaveSingleImage //  [2009-8-23-yangfs]
/* // SaveSingleImage()�����м��������Ų���������·�����ļ����ж����������� [9/1/2009 GZ]
/************************************************************************/

#if !defined(Image_h_)
#define Image_h_


//////////////////////////////////////////////////////////////////////////
//

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

enum eSaveImageType
{
	SaveGrabImage=0,                // ���������ɼ�ͼ�� [8/19/2009 GZ]
	SaveErrorImage,					// �������ͼ��
	SaveAbnormityImage,				// �����쳣ͼ��
	SaveCorrectImage,				// ������ȷͼ��
	SaveModelImage					// ����ģ��ͼ��
}; 

enum eAutoSaveErrorInfo
{
	GrabAllFaultError=-9,
	ParamTypeError,				// ����2�������ʹ������ [8/19/2009 GZ]
	ParamLPElementError,			// ����1����Ԫ��ָ��Ϊ�� [8/19/2009 GZ]
	ListCountError,					// ��õ�Ԫ���е��������Ϊ����� [8/19/2009 GZ]
	SaveBMPError,					// ����SaveBMPFile()δ�ɹ� [8/19/2009 GZ]
	CreatePathError,				// ����·������
	FileNameError,					// �ļ�������
	ImageWHBITError,				// ͼƬ����λ������
	ImageBufferError				// ͼ������������
};

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)


class CImage
{	
//����
protected:	
	// ��ȡ��ִ�г������ڵ�·��
	CString inGetCurrentAppliationPath();
	
	BYTE* m_pImageBuff;	//ͼ�񻺳���
	long m_lBuffSize;	//ͼ�񻺳�����С
	
	long m_lImageWidth;		//ͼ����
	long m_lImageHeight;	//ͼ�񳤶�
	long m_lImageBitCount;	//����λ��
	long m_lImagePixelSize;	//���ش�С����λ���ֽڣ�
	long m_lImageSize;		//ͼ�����ݴ�С

	BITMAPINFO* m_pBitmapInfo;
	
	BITMAPINFO* m_pShowBitmapInfo;
	CCriticalSection m_csShowBitmapInfoSection;
public:
 	BITMAPINFO* m_pSaveBitmapInfo;
	CCriticalSection m_csSaveBitmapInfoSection;

//����
public:
	BOOL inCreateMultiLevelPath(const CString &strPath);
	BOOL inIsPathExist(const CString &strPath);
	//AutoSaveImageToPath������iSaveImageTypeΪ1~3��1Ϊ�ɼ��洢 [8/20/2009 GZ]
	int AutoSaveImageToPath(LPVOID lpElementParam,int iSaveImageType);
	int SaveMeToPath(LPVOID lpElementParam,int iSaveImageType,int iCircleSN);
	BOOL inSaveBMPFile(LPCSTR lpFileName, const BITMAPINFO *pBmpInfo, const BYTE *pImageBuffer);

	//����С��ͼ�� 
	//iSaveImageType ����ͼ������
	//iCircleCountΪ��Ӧ�Ĵ��ź�
	//iCamSNΪ������
	//iGroupSNΪ��λ��
	//iPicCountΪÿ�δ����ɼ���ͼ������
	int SaveSingleImage(const int iSaveImageType,const int iCircleCount, const int iCamSN, const int iGroupSN, const int iPicCount, const CString strSaveTime);

	//��ͼ�����ݽ��о���
	int MirrorSingleImage(e_ShowModeFlip  eShowMode=FlipHorizintal); //ֻ��ˮƽ�ʹ�ֱ���� //  [2009-8-24-yangfs]

	CImage();
	virtual ~CImage();
	
	void Init(long lImageBuffSize, long lPicCount);
	void SetImageSize(long lImageWidth, long lImageHeight, long lImageBitCount, long lPicCount);
	void AcceptImage(const BYTE* pSrcImageBuff, long lSrcImageWidth, long lSrcImageHeight, long lSrcImageBitCount, long lPicCount);

	BYTE* GetImageBuffAddr();
	long GetImageBuffSize();
	long GetImageSize();
	long GetImageWidth();
	long GetImageHeight();
	long GetImageBitCount();
	long GetImagePixelSize();
	const BITMAPINFO* GetBitmapInfo();
	
	void ShowDIBImage(CDC* pDC, CRect Rect, e_ShowModeFlip ShowModeFlip, e_ShowModeStretch ShowModeStretch,
		const BYTE* pImageBuff, long lImageWidth, long lImageHeight, long lImageBitCount);
//	void SaveDIBImageToBMPFile(const CString& strFileName, const BYTE* pImageBuff, long lImageWidth, long lImageHeight, long lImageBitCount);
	void CutDIBImage(BYTE *pDest, CRect DestRect, const BYTE* pSrcImageBuff, long lSrcImageWidth, long lSrcImageHeight, long lSrcImageBitCount);
	BOOL OpenBMPFileX(LPCSTR lpFileName,long lPicCount);
};

class CImageException
{
//����
protected:	
	int m_nExceptionType;				//�쳣������
	CString m_strExceptionDescription;	//�쳣������
	CString m_strExceptionContext;		//�쳣�ĸ�����Ϣ
	
//����
public:
	CImageException(int nExceptionType, const CString &strExceptionContext)
		: m_nExceptionType(nExceptionType), 
		m_strExceptionContext(strExceptionContext)
	{
		m_strExceptionDescription.Format("Error Type(%d)");
	};
	
	CImageException(int nExceptionType, const CString &strExceptionDescription, const CString &strExceptionContext)
		: m_nExceptionType(nExceptionType), 
		m_strExceptionDescription(strExceptionDescription),
		m_strExceptionContext(strExceptionContext)
	{
	};
	
	~CImageException()
	{
	};

	//�õ�����
	int GetType() const
	{
		return m_nExceptionType;
	}

	//�õ������͵�����
	CString GetDescription() const
	{
		return m_strExceptionDescription;
	}
	
	//�õ�������Ϣ
	CString GetContext() const
	{
		return m_strExceptionContext;
	}
	
	void ShowMe() const
	{
		CString str;
		str.Format("�������ͣ�%d\n������%s\n������Ϣ��%s", 
			m_nExceptionType,
			m_strExceptionDescription,
			m_strExceptionContext);

		AfxMessageBox(str);
	}
};


#endif // !define(Image_h_)