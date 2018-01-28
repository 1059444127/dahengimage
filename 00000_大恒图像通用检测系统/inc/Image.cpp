/************************************************************************/
/* Copyright (c) 2005, �й���㼯�ű���ͼ���Ӿ������ֹ�˾�Ӿ�������     
/* All rights reserved.													
/*																		
/* �ļ����ƣ� Image.cpp												
/* �ļ���ʶ�� �����ù���ƻ���
/* ժҪ�� CImage��, �Ա���һ��ͼ��ĸ����ݽṹ���з�װ, ʵ����ͼ�񿽱�,
/*    ͼ����ʾ, ͼ�����, ͼ����еȹ���
/*
/* ��ǰ�汾�� 1.1
/* ���ߣ� ��׿��
/* ������ڣ� 2005��10��10��
/*�޸�: ����AutoSaveImageToPath()������  SaveMeToPath()�Զ�����ͼ���ȶ�·�� ����//  [8/19/2009 GZ]
/************************************************************************/

#include "stdafx.h"
#include "Image.h"
#include "FileOperate.h"
#include "LogManager.h"
#include "MainFrm.h"
#include "SpendTime.h"
#include "BottleCapView.h"

//////////////////////////////////////////////////////////////////////////
//
extern CMainFrame *g_pFrame;
CImage::CImage()
{
	int i(0);
	m_pImageBuff = NULL;	//ͼ�񻺳���
	m_lBuffSize = 0;		//ͼ�����ݴ�С

	m_lImageWidth = 0;		//ͼ����
	m_lImageHeight = 0;		//ͼ�񳤶�
	m_lImageBitCount = 0;	//����λ��
	m_lImagePixelSize = 0;	//���ش�С����λ���ֽڣ�
	m_lImageSize = 0;		//ͼ�����ݴ�С
	
	//�Լ��ڲ���
	m_pBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 256*sizeof(RGBQUAD)];
    m_pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_pBitmapInfo->bmiHeader.biPlanes = 1;
    m_pBitmapInfo->bmiHeader.biBitCount = 0;
    m_pBitmapInfo->bmiHeader.biCompression = BI_RGB;
    m_pBitmapInfo->bmiHeader.biSizeImage = 0;
    m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
    m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
    m_pBitmapInfo->bmiHeader.biClrUsed = 0;
    m_pBitmapInfo->bmiHeader.biClrImportant = 0;
    m_pBitmapInfo->bmiHeader.biWidth = 0 ;
    m_pBitmapInfo->bmiHeader.biHeight = 0 ;
	for (i = 0 ; i < 256 ; i++)
	{
		m_pBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
		m_pBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
		m_pBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
		m_pBitmapInfo->bmiColors[i].rgbReserved = 0;
	}
	
	//������ʾ�õ�
	m_pShowBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 256*sizeof(RGBQUAD)];
    m_pShowBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_pShowBitmapInfo->bmiHeader.biPlanes = 1;
    m_pShowBitmapInfo->bmiHeader.biBitCount = 0;
    m_pShowBitmapInfo->bmiHeader.biCompression = BI_RGB;
    m_pShowBitmapInfo->bmiHeader.biSizeImage = 0;
    m_pShowBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
    m_pShowBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
    m_pShowBitmapInfo->bmiHeader.biClrUsed = 0;
    m_pShowBitmapInfo->bmiHeader.biClrImportant = 0;
    m_pShowBitmapInfo->bmiHeader.biWidth = 0 ;
    m_pShowBitmapInfo->bmiHeader.biHeight = 0 ;
	for (i = 0 ; i < 256 ; i++)
	{
		m_pShowBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
		m_pShowBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
		m_pShowBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
		m_pShowBitmapInfo->bmiColors[i].rgbReserved = 0;
	}
	
	//���������õ�
	m_pSaveBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD)];
    m_pSaveBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_pSaveBitmapInfo->bmiHeader.biPlanes = 1;
    m_pSaveBitmapInfo->bmiHeader.biBitCount = 0;
    m_pSaveBitmapInfo->bmiHeader.biCompression = BI_RGB;
    m_pSaveBitmapInfo->bmiHeader.biSizeImage = 0;
    m_pSaveBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
    m_pSaveBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
    m_pSaveBitmapInfo->bmiHeader.biClrUsed = 0;
    m_pSaveBitmapInfo->bmiHeader.biClrImportant = 0;
    m_pSaveBitmapInfo->bmiHeader.biWidth = 0 ;
    m_pSaveBitmapInfo->bmiHeader.biHeight = 0 ;

	for (i = 0 ; i < 256 ; i++)
	{
		m_pSaveBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
		m_pSaveBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
		m_pSaveBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
		m_pSaveBitmapInfo->bmiColors[i].rgbReserved = 0;
	}
}

CImage::~CImage()
{
	if(m_pImageBuff != NULL)
	{
		delete []m_pImageBuff;
		m_pImageBuff = NULL;
		m_lBuffSize = 0;
	}

	if (m_pBitmapInfo != NULL) 
	{
		delete []m_pBitmapInfo;
		m_pBitmapInfo = NULL;
	}
	
	if (m_pShowBitmapInfo != NULL) 
	{
		delete []m_pShowBitmapInfo;
		m_pShowBitmapInfo = NULL;
	}
	
	if (m_pSaveBitmapInfo != NULL) 
	{
		delete []m_pSaveBitmapInfo;
		m_pSaveBitmapInfo = NULL;
	}
}

void CImage::Init(long lImageBuffSize,long lPicCount)
{
	ASSERT(lImageBuffSize >= 0);

	m_lImageWidth = 0;	//ͼ����
	m_lImageHeight = 0;	//ͼ�񳤶�
	m_lImageBitCount = 0;	//����λ��
	m_lImagePixelSize = 0;	//���ش�С����λ���ֽڣ�
	m_lImageSize = 0;	//ͼ�����ݴ�С
	
	m_pBitmapInfo->bmiHeader.biWidth = m_lImageWidth;
	m_pBitmapInfo->bmiHeader.biHeight = m_lImageHeight;
	m_pBitmapInfo->bmiHeader.biBitCount = (WORD)m_lImageBitCount;

	if(m_pImageBuff != NULL)
	{
		delete []m_pImageBuff;
		m_pImageBuff = NULL;
		m_lBuffSize = 0;
	}
	
	m_pImageBuff = new BYTE[lImageBuffSize*lPicCount];
	memset(m_pImageBuff,0,sizeof(m_pImageBuff));

	if(m_pImageBuff == NULL)
	{
		m_lBuffSize = 0;
		throw(CImageException(1, "�ڴ���䲻�ɹ�", "Init()"));
		return;
	}
	else
	{
		m_lBuffSize = lImageBuffSize*lPicCount;
	}

}

void CImage::SetImageSize(long lImageWidth, long lImageHeight, long lImageBitCount, long lPicCount)
{
	ASSERT(lImageWidth >= 0 && lImageHeight >= 0 && lImageBitCount > 0);
	
	long lImageSize = lImageWidth * lImageHeight * ((lImageBitCount+7)/8) * lPicCount;
	
	if(lImageSize > m_lBuffSize)
	{
		throw(CImageException(2, "�������Ϸ�", "SetImageSize()"));
		return;
	}
	
	m_lImageWidth = lImageWidth;				//ͼ����
	m_lImageHeight = lImageHeight;				//ͼ�񳤶�
	m_lImageBitCount = lImageBitCount;			//����λ��
	m_lImagePixelSize = (lImageBitCount+7)/8;	//���ش�С����λ���ֽڣ�
	m_lImageSize = lImageSize;					//ͼ�����ݴ�С
	
	m_pBitmapInfo->bmiHeader.biWidth = m_lImageWidth;
	m_pBitmapInfo->bmiHeader.biHeight = m_lImageHeight;
	m_pBitmapInfo->bmiHeader.biBitCount = (WORD)m_lImageBitCount;
}

void CImage::AcceptImage(const BYTE* pSrcImageBuff, long lSrcImageWidth, long lSrcImageHeight, long lSrcImageBitCount, long lPicCount)
{
	ASSERT(pSrcImageBuff != NULL && lSrcImageWidth >= 0 && lSrcImageHeight >= 0 && lSrcImageBitCount > 0);
	
	long lImageSize = lSrcImageWidth * lSrcImageHeight * (lSrcImageBitCount/8) * lPicCount;

	if(lImageSize > m_lBuffSize)
	{
		throw(CImageException(2, "�������Ϸ�", "AcceptImage()"));
	}

	memcpy(m_pImageBuff, pSrcImageBuff, lImageSize);
	
	m_lImageWidth = lSrcImageWidth;
	m_lImageHeight = lSrcImageHeight;
	m_lImageBitCount = lSrcImageBitCount;
	m_lImagePixelSize = ((lSrcImageBitCount+7)/8);
	m_lImageSize = lImageSize;

		
	m_pBitmapInfo->bmiHeader.biWidth = m_lImageWidth;
	m_pBitmapInfo->bmiHeader.biHeight = m_lImageHeight;
	m_pBitmapInfo->bmiHeader.biBitCount = (WORD)m_lImageBitCount;

}

BYTE* CImage::GetImageBuffAddr()
{
	return m_pImageBuff;
}
long CImage::GetImageBuffSize()
{
	return m_lBuffSize;
}
long CImage::GetImageWidth()
{
	return m_lImageWidth;
}
long CImage::GetImageHeight()
{
	return m_lImageHeight;
}
long CImage::GetImageBitCount()
{
	return m_lImageBitCount;
}
long CImage::GetImagePixelSize()
{
	return m_lImagePixelSize;
}
long CImage::GetImageSize()
{
	return m_lImageSize;
}
const BITMAPINFO* CImage::GetBitmapInfo()
{
	return m_pBitmapInfo;
}

void CImage::ShowDIBImage(CDC* pDC, CRect Rect, e_ShowModeFlip ShowModeFlip, e_ShowModeStretch ShowModeStretch,
		const BYTE* pImageBuff, long lImageWidth, long lImageHeight, long lImageBitCount)
{
	int XDest = 0;
	int YDest = 0;
	int nDestWidth = 0;
	int nDestHeight = 0;

	ASSERT(pImageBuff != NULL && lImageWidth >= 0 && lImageHeight >= 0 && lImageBitCount > 0);

	m_csShowBitmapInfoSection.Lock();
	
	m_pShowBitmapInfo->bmiHeader.biWidth = lImageWidth;
	m_pShowBitmapInfo->bmiHeader.biHeight = lImageHeight;
	m_pShowBitmapInfo->bmiHeader.biBitCount = (WORD)lImageBitCount;

	
	XDest = Rect.left;
	YDest = Rect.top;
	nDestWidth = Rect.Width();
	nDestHeight = Rect.Height();
		
	if(ShowModeStretch == NoStretch)
	{
		switch(ShowModeFlip) 
		{
		case NoFlip:			//����ת
			{
			}
			break;
		case FlipHorizintal:	//ˮƽ��ת
			{
			}
			break;
		case FlipVertical:		//��ֱ��ת
			{
			}
			break;
		case Flip:				//ȫ��ת
			{
			}
			break;
		default:				//����ת
			{
			}
		}
		
		SetDIBitsToDevice(pDC->GetSafeHdc(), 
			XDest, // destx 
			YDest, // desty 
			nDestWidth,				//��ʾ���ڿ��
			nDestHeight,			//��ʾ���ڸ߶�
			0, // srcx 
			0, // srcy 
			0, // nstartscan 
			lImageHeight, // nnumscans 
			pImageBuff, // lpbits 
			m_pShowBitmapInfo, // lpbitsinfo 
			DIB_RGB_COLORS); // wusage 
	}
	else
	{
		switch(ShowModeFlip) 
		{
		case NoFlip:			//����ת
			break;
		case FlipHorizintal:	//ˮƽ��ת
			XDest = Rect.right;
			nDestWidth = -nDestWidth;
			break;
		case FlipVertical:		//��ֱ��ת
			YDest = Rect.bottom;
			nDestHeight = -nDestHeight;
			break;
		case Flip:				//ȫ��ת
			XDest = Rect.right;
			YDest = Rect.bottom;
			nDestWidth = -nDestWidth;
			nDestHeight = -nDestHeight;
			break;
		default:				//����ת
			XDest = Rect.left;
			YDest = Rect.top;
		}
		
		SetStretchBltMode(pDC->m_hDC, COLORONCOLOR);
		
		StretchDIBits(pDC->GetSafeHdc(),
			XDest,
			YDest,	
			nDestWidth,				//��ʾ���ڿ��
			nDestHeight,			//��ʾ���ڸ߶�
			0,
			0,
			lImageWidth,			//ͼ����
			lImageHeight,			//ͼ��߶�
			pImageBuff,				//ͼ�񻺳���
			m_pShowBitmapInfo,			//BMPͼ��������Ϣ
			DIB_RGB_COLORS,
			SRCCOPY
			);
	}
	
	m_csShowBitmapInfoSection.Unlock();
}
/*
void CImage::SaveDIBImageToBMPFile(const CString& strFileName, const BYTE* pImageBuff, long lImageWidth, long lImageHeight, long lImageBitCount)
{
	BOOL bRet = FALSE;

	ASSERT(pImageBuff != NULL && lImageWidth >= 0 && lImageHeight >= 0 && lImageBitCount > 0);
	ASSERT((strFileName.GetLength() >= 5) && (strFileName.Right(4) == ".bmp"));

	m_csSaveBitmapInfoSection.Lock();
	
	m_pSaveBitmapInfo->bmiHeader.biWidth = lImageWidth;
	m_pSaveBitmapInfo->bmiHeader.biHeight = lImageHeight;
	m_pSaveBitmapInfo->bmiHeader.biBitCount = (WORD)lImageBitCount;

	bRet = MyWriteBMPFile(strFileName, m_pSaveBitmapInfo, pImageBuff);

	m_csSaveBitmapInfoSection.Unlock();

	if(!bRet)
	{
		throw(CImageException(3, "�ļ����治�ɹ�", "SaveDIBImageToBMPFile()"));
		return;
	}	
}//*/

//�ü�����(������DIBͼ������)(��ԭͼ�����Ͻ�Ϊԭ��, x����, y����)
//Ŀ�껺����//���о���
//����ֵ: TRUE: �ɹ�
void CImage::CutDIBImage(BYTE *pDest, CRect DestRect, 
						 const BYTE* pSrcImageBuff, 
						 long lSrcImageWidth, 
						 long lSrcImageHeight, 
						 long lSrcImageBitCount)
{
	//ע: DIBͼ�����ݵĴ洢��ʽ��: ԭͼ��ĵ�һ�д洢��DIBͼ�����ݻ����������һ��, �����������������д洢.
	
	ASSERT(pDest != NULL && pSrcImageBuff != NULL && lSrcImageWidth >= 0 && lSrcImageHeight >= 0 && lSrcImageBitCount > 0);

	int nOffset_Source = 0;//����ڻ�����ͷ��ƫ�Ƶ��ֽ���(Դ)
	int nOffset_Dest = 0;  //����ڻ�����ͷ��ƫ�Ƶ��ֽ���(Ŀ��)

	int CutPositionX = DestRect.left;
	int CutPositionY = DestRect.top;
	int CutWidth = DestRect.Width();
	int CutHeight = DestRect.Height();

	int nPixelByteCount = (lSrcImageBitCount+7)/8;//һ�������ü����ֽڱ�ʾ(RGB:3)
	
	//�������
	if((CutPositionX + CutWidth) > lSrcImageWidth || (CutPositionY + CutHeight) > lSrcImageHeight)
	{
		throw(CImageException(2, "�������Ϸ�", "CutDIBImage()"));
		return;
	}
	
	//���м�����ʼ�е�ת��
	int nBeginLine = lSrcImageHeight - CutPositionY - CutHeight;
	
	//ȥ��DIBͼ�����ݻ������ļ����е���ʼ��
	nOffset_Source = (lSrcImageWidth * nBeginLine + CutPositionX) * nPixelByteCount;
	
	for (int i = 0; i < CutHeight; i++)//�ж�����
	{
		memcpy(pDest + nOffset_Dest, pSrcImageBuff + nOffset_Source, CutWidth*nPixelByteCount);
		
		nOffset_Dest += (CutWidth * nPixelByteCount);  //��һ��(Ŀ��)
		
		nOffset_Source += (lSrcImageWidth*nPixelByteCount);  //��һ��(Դ)
	}
}

//////////////////////////////////////////////////////////////////////////
// end of all


int CImage::SaveMeToPath(LPVOID lpElementParam,int iSaveImageType,int iCircleSN)
{

	CString strIniPath, strIniFileName, strTime;
	CTime curTime = CTime::GetCurrentTime();
	DWORD dwTime = GetTickCount();
	CString strRunFilePath=inGetCurrentAppliationPath();

	CAllInformationElement *pElement=(CAllInformationElement *)lpElementParam;
	int iCircleCont=0;	//���ź� [8/19/2009 GZ]
	iCircleCont=pElement->m_iCircleCount;
// 	int iGrabSN=0;		// ��������� [8/19/2009 GZ]
// 	iGrabSN=pElement->m_listSingleImageData.
	int iCameraSN=0;	// ������ [9/1/2009 GZ]
	iCameraSN=pElement->m_iCamGroup;//[2012-1-19 by zhy]

	switch(iSaveImageType)
	{
	case SaveGrabImage:
		{
			strIniPath.Format("%s%s%4d%s%02d%s%02d%sCamera%d", strRunFilePath, "SaveImages\\GrabImages\\",
							curTime.GetYear(), "��",curTime.GetMonth(), "��",curTime.GetDay(),"��\\",iCameraSN);
			strIniFileName.Format("%s\\Grab%d_%d_%d_%02d%02d%02d_%d_%03d%s", 
							strIniPath, iCameraSN, 
							iCircleCont,
							iCircleSN,
							curTime.GetHour(),
							curTime.GetMinute(), 
							curTime.GetSecond(), 
							dwTime,	rand()%1000,
							".bmp");
		}	
		break;
	case SaveErrorImage:
		{
			strIniPath.Format("%s%s%4d%s%02d%s%02d%sCamera%d", strRunFilePath, "SaveImages\\ErrorImages\\",
							curTime.GetYear(), "��",curTime.GetMonth(), "��",curTime.GetDay(),"��\\",iCameraSN);
			strIniFileName.Format("%s\\Error%d_%d_%d_%02d%02d%02d_%d_%03d%s", 
							strIniPath, iCameraSN, 
							iCircleCont,
							iCircleSN,
							curTime.GetHour(),
							curTime.GetMinute(), 
							curTime.GetSecond(), 
							dwTime,	rand()%1000,
							".bmp");
		}	
	    break;
	case SaveAbnormityImage:
		{
			strIniPath.Format("%s%s%4d%s%02d%s%02d%sCamera%d", strRunFilePath, "SaveImages\\AbnormityImages\\",
							curTime.GetYear(), "��",curTime.GetMonth(), "��",curTime.GetDay(),"��\\",iCameraSN);
			strIniFileName.Format("%s\\Abnormity%d_%d_%d_%02d%02d%02d_%d_%03d%s", 
							strIniPath, iCameraSN, 
							iCircleCont,
							iCircleSN,
							curTime.GetHour(),
							curTime.GetMinute(),
							curTime.GetSecond(),
							dwTime,	rand()%1000,
							".bmp");
		}	
	    break;
	default:
		{
			strIniPath.Format("%s%s%4d%s%02d%s%02d%sCamera%d", strRunFilePath, "SaveImages\\GrabImages\\",
				curTime.GetYear(), "��",curTime.GetMonth(), "��",curTime.GetDay(),"��\\",iCameraSN);
			strIniFileName.Format("%s\\Grab%d_%d_%d_%02d%02d%02d_%d_%03d%s", 
				strIniPath, iCameraSN, 
				iCircleCont,
				iCircleSN,
				curTime.GetHour(),
				curTime.GetMinute(), 
				curTime.GetSecond(), 
				dwTime,	rand()%1000,
				".bmp");
		}	
		break;
	}
	//strIniFileName.Format("%s_%d.bmp",strIniPath,iCircleCont);
	
	int lImageWidth,lImageHeight,lImageBitCount;
	BYTE* pImageBuff=NULL;
	pImageBuff=GetImageBuffAddr();
	lImageWidth=GetImageWidth();
	lImageHeight=GetImageHeight();
	lImageBitCount=GetImageBitCount();
	
	if (pImageBuff == NULL)
	{
		return ImageBufferError;
	}
	if (lImageWidth<0||lImageHeight<0||lImageBitCount<0)
	{
		return ImageWHBITError;
	}
	if ((strIniFileName.GetLength() <5)|| (strIniFileName.Right(4) != ".bmp")||strIniFileName.GetLength()>255)
	{
		return FileNameError;
	}
	

	if (!inIsPathExist(strIniPath))
	{
		if (inCreateMultiLevelPath(strIniPath))
		{
			m_csSaveBitmapInfoSection.Lock();
			
			m_pSaveBitmapInfo->bmiHeader.biWidth = lImageWidth;
			m_pSaveBitmapInfo->bmiHeader.biHeight = lImageHeight;
			m_pSaveBitmapInfo->bmiHeader.biBitCount = (WORD)lImageBitCount;
			
			if (inSaveBMPFile(strIniFileName, m_pSaveBitmapInfo, pImageBuff))
			{
			}
			else
			{
				m_csSaveBitmapInfoSection.Unlock();
				return SaveBMPError;
			}
			
			m_csSaveBitmapInfoSection.Unlock();

		}
		else
			return CreatePathError;
	}
	else
	{
		m_csSaveBitmapInfoSection.Lock();
		
		m_pSaveBitmapInfo->bmiHeader.biWidth = lImageWidth;
		m_pSaveBitmapInfo->bmiHeader.biHeight = lImageHeight;
		m_pSaveBitmapInfo->bmiHeader.biBitCount = (WORD)lImageBitCount;
		
		if (!inSaveBMPFile(strIniFileName, m_pSaveBitmapInfo, pImageBuff))
		{
			m_csSaveBitmapInfoSection.Unlock();
			return SaveBMPError;
		}	
		m_csSaveBitmapInfoSection.Unlock();
	}
	return 1;
}

//����С��ͼ��  
// iSaveImageType  ��������
// iCircleCont   �ɼ���������  ��������
// iCircleSN     ��������� 
// iCameraSN     ������ 
int CImage::SaveSingleImage(const int iSaveImageType,const int iCircleCount, const int iCamSN, const int iCamGroup, const int iPicCount, const CString strSaveTime)
{
	CBottleCapView* pView=(CBottleCapView*)g_pFrame->GetActiveView();
	BYTE* pImageBuff=NULL;


	int iOriginNum=0,iCount=0;
	int iWidth=0,iHeight=0,iBitCount=0;
	CString strIniPath, strIniFileName,strVirtualDisk;
	CTime curTime= CTime::GetCurrentTime();
	strVirtualDisk.Format("%s:\\",g_pFrame->m_sVirtualDisk);

	/////////////////////////////////////////////////////////////////////////////////////////////
	//���Ʊ���ͼ�������

	iOriginNum = g_pFrame->m_iOriginSaveImageNum[iCamSN];

	g_pFrame->m_csImagePath[iCamSN].Lock();
	iCount = g_pFrame->m_ListImagePath[iCamSN].GetCount();
	if(iCount>=(g_pFrame->m_iMaxSaveImageCount) || iOriginNum>0)
	{
		CString strDeleteFilePath;	
		strDeleteFilePath=g_pFrame->m_ListImagePath[iCamSN].RemoveHead();
// 		DeleteFile(strDeleteFilePath);

		strDeleteFilePath+='\0';
		g_pFrame->OP_Directory(strDeleteFilePath,NULL,FO_DELETE,FOF_NOCONFIRMATION | FOF_SIMPLEPROGRESS |FOF_NOERRORUI);

		if(iOriginNum>0)
		{
			g_pFrame->m_iOriginSaveImageNum[iCamSN]--;
		}
	}
	g_pFrame->m_csImagePath[iCamSN].Unlock();

	/////////////////////////////////////////////////////////////////////////////////////////////

	switch(iSaveImageType)
	{
	case SaveGrabImage:
		{
			strIniPath.Format("%s%s%4d%s%02d%s%02d%sCamera%d\\%d_%s", strVirtualDisk, "SaveImages\\GrabImages\\",
							curTime.GetYear(), "��",curTime.GetMonth(), "��",curTime.GetDay(),"��\\",iCamSN+1,
							iCircleCount,strSaveTime);
			strIniFileName.Format("%s\\Grab_%d_%s%s", 
				strIniPath,
				iCircleCount,
				strSaveTime,
				".bmp");
		}	
		break;
	case SaveErrorImage:
		{
			strIniPath.Format("%s%s%4d%s%02d%s%02d%sCamera%d\\%d_%s", strVirtualDisk, "SaveImages\\ErrorImages\\",
				curTime.GetYear(), "��",curTime.GetMonth(), "��",curTime.GetDay(),"��\\",iCamSN+1,
							iCircleCount,strSaveTime);
			strIniFileName.Format("%s\\Error_%d_%s%s", 
				strIniPath, 
				iCircleCount,
				strSaveTime,
				".bmp");

		}

	    break;
	case SaveCorrectImage:
		{
			strIniPath.Format("%s%s%4d%s%02d%s%02d%sCamera%d\\%d_%s", strVirtualDisk, "SaveImages\\CorrectImages\\",
				curTime.GetYear(), "��",curTime.GetMonth(), "��",curTime.GetDay(),"��\\",iCamSN+1,
							iCircleCount,strSaveTime);
				strIniFileName.Format("%s\\Correct_%d_%s%s", 
				strIniPath, 
				iCircleCount,
				strSaveTime,
				".bmp");

		}
	    break;
	case SaveAbnormityImage:
		{
			strIniPath.Format("%s%s%4d%s%02d%s%02d%sCamera%d\\%d_%s", strVirtualDisk, "SaveImages\\AbnormityImages\\",
				curTime.GetYear(), "��",curTime.GetMonth(), "��",curTime.GetDay(),"��\\",iCamSN+1,
							iCircleCount,strSaveTime);
			strIniFileName.Format("%s\\Abnormity_%d_%s%s", 
				strIniPath, 
				iCircleCount,
				strSaveTime,
				".bmp");
		}	
	    break;
	case SaveModelImage:
		{
			strIniPath.Format("%s%s%4d%s%02d%s%02d%sCamera%d\\%d_%s", strVirtualDisk, "SaveImages\\ModelImages\\",
				curTime.GetYear(), "��",curTime.GetMonth(), "��",curTime.GetDay(),"��\\",iCamSN+1,
							iCircleCount,strSaveTime);
			strIniFileName.Format("%s\\Model_%d_%s%s", 
				strIniPath, 
				iCircleCount,
				strSaveTime,
				".bmp");
		}	
	    break;		

	default:
		{
			strIniPath.Format("%s%s%4d%s%02d%s%02d%sCamera%d\\%d_%s", strVirtualDisk, "SaveImages\\GrabImages\\",
				curTime.GetYear(), "��",curTime.GetMonth(), "��",curTime.GetDay(),"��\\",iCamSN+1,
							iCircleCount,strSaveTime);
			strIniFileName.Format("%s\\Grab_%d_%s%s", 
				strIniPath, 
				iCircleCount,
				strSaveTime,
				".bmp");
		}	
		break;
	}
	
	pImageBuff=GetImageBuffAddr();
	iWidth=GetImageWidth();
	iHeight=GetImageHeight();
	iBitCount=GetImageBitCount();
	
	if (pImageBuff == NULL)
	{
		return ImageBufferError;
	}
	if (iWidth<0||iHeight<0||iBitCount<0)
	{
		return ImageWHBITError;
	}
 	if ((strIniFileName.GetLength() <5)|| (strIniFileName.Right(4) != ".bmp")||strIniFileName.GetLength()>255)
	{
		return FileNameError;
	}
	
	if (!inIsPathExist(strIniPath))
	{
		if (inCreateMultiLevelPath(strIniPath))
		{
			m_csSaveBitmapInfoSection.Lock();
			
			m_pSaveBitmapInfo->bmiHeader.biWidth = iWidth;
			m_pSaveBitmapInfo->bmiHeader.biHeight = iHeight;
			m_pSaveBitmapInfo->bmiHeader.biBitCount = (WORD)iBitCount;

			int iImageSize = iWidth*iHeight*iBitCount/8;

			for(int i=0;i<iPicCount;i++)
			{
				strIniFileName.Format("%s\\%02d.bmp",strIniPath,i+1);
				if (!inSaveBMPFile(strIniFileName, m_pSaveBitmapInfo, (pImageBuff+i*iImageSize)))
				{
					m_csSaveBitmapInfoSection.Unlock();
					return SaveBMPError;
				}
			}
			m_csSaveBitmapInfoSection.Unlock();
		}
		else
		{
			return CreatePathError;
		}
	}
	else
	{
		m_csSaveBitmapInfoSection.Lock();
		
		m_pSaveBitmapInfo->bmiHeader.biWidth = iWidth;
		m_pSaveBitmapInfo->bmiHeader.biHeight = iHeight;
		m_pSaveBitmapInfo->bmiHeader.biBitCount = (WORD)iBitCount;

		int iImageSize = iWidth*iHeight*iBitCount/8;

		for(int i=0;i<iPicCount;i++)
		{
			strIniFileName.Format("%s\\%d.bmp",strIniPath,i+1);
			if (!inSaveBMPFile(strIniFileName, m_pSaveBitmapInfo, (pImageBuff+i*iImageSize)))
			{
				m_csSaveBitmapInfoSection.Unlock();
				return SaveBMPError;
			}

		}
		m_csSaveBitmapInfoSection.Unlock();
	}

	g_pFrame->m_csImagePath[iCamSN].Lock();
	g_pFrame->m_ListImagePath[iCamSN].AddTail(strIniPath);
	g_pFrame->m_iListImagePathCount[iCamSN] = g_pFrame->m_ListImagePath[iCamSN].GetCount();
	g_pFrame->m_csImagePath[iCamSN].Unlock();

	return 1;
}

BOOL CImage::inSaveBMPFile(LPCSTR lpFileName, const BITMAPINFO *pBmpInfo, const BYTE *pImageBuffer)
{

	BOOL bRVal				= TRUE;
	DWORD dwBytesRead		= 0;
	DWORD dwSize			= 0;
	BITMAPFILEHEADER bfh	= {0};
	int nTable				= 0;
	DWORD dwImageSize		= 0;
	
	if (pBmpInfo->bmiHeader.biBitCount > 8) 
	{
		nTable = 0;
	}
	else
	{
		nTable = 256;
	}

	dwImageSize =  (pBmpInfo->bmiHeader.biWidth * pBmpInfo->bmiHeader.biHeight) * ((pBmpInfo->bmiHeader.biBitCount + 7) / 
		
		8);
	
	if (dwImageSize <= 0) 
	{
		bRVal = FALSE;
	}
	else{
		bfh.bfType		= (WORD)'M' << 8 | 'B';
		bfh.bfOffBits	= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nTable * sizeof(RGBQUAD);
		bfh.bfSize		= bfh.bfOffBits + dwImageSize;			
		
		HANDLE hFile = ::CreateFile(lpFileName,
			GENERIC_WRITE ,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			);
		if (hFile == INVALID_HANDLE_VALUE) {
			bRVal = FALSE;
		}
		else{
			dwSize = sizeof(BITMAPFILEHEADER);
			::WriteFile(hFile, &bfh, dwSize, &dwBytesRead, NULL );
			
			dwSize = sizeof(BITMAPINFOHEADER) + nTable * sizeof(RGBQUAD);
			::WriteFile(hFile, pBmpInfo, dwSize, &dwBytesRead, NULL );
			
			dwSize = dwImageSize;
			WriteFile(hFile, pImageBuffer, dwSize, &dwBytesRead, NULL );
			
			CloseHandle(hFile);
		}
	}
	
	return bRVal;
}

#include <io.h>//ͷ�ļ�
BOOL CImage::inIsPathExist(const CString &strPath)
{
#ifdef _UNICODE
#define IsDirectoryExist(path) ::_waccess(path,0) == 0
	//_waccess�ĵڶ���������ʾ���жϴ����ԣ�����0��ʾ���ڣ�����-1��ʾ������
#else
#define IsDirectoryExist(path) ::_access(path,0) == 0//_access��MBCS�汾
#endif
	
	if(IsDirectoryExist(strPath))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

BOOL CImage::inCreateMultiLevelPath(const CString &strPath)
{

	//����һ�����Ŀ¼��������ھͲ�����
	//(strPath  �Ϸ�: "D:\\abc\\me" �� "me\\you" �� ""; 
	//			���Ϸ�: "D:\\abc\\me\\" �� "me\\you\\" �� "\\")
	CString tmpPath;// = strPath;
	tmpPath.Format("%s",strPath);
	CList<CString,CString&> FolderStrList;
	int nStrLen = 0;
	
	for(;;)
	{
		nStrLen = tmpPath.GetLength();
		if(nStrLen <= 0)
		{
			break;//��ͷ���ļ���
		}		
		
		BOOL bPathExist = FALSE;//��·���Ƿ��Ѿ�����
		
		if(tmpPath.Right(1) == ":")
		{
			bPathExist = inIsPathExist(tmpPath + "\\");
		}
		else
		{
			bPathExist = inIsPathExist(tmpPath);
		}
		
		if(bPathExist)//���·������
		{
			break;//���Ѵ��ڵ�·���½��ļ���
		}
		
		//���·��������		
		//ÿ�ν�����һ���ļ��������
		
		int nPos = 0;
		nPos = tmpPath.ReverseFind('\\');//�Ӻ���ǰȷ���ļ�����
		if(nPos == -1)//�ѵ����һ���ļ�����
		{
			nPos = 0;//����һ���ļ��������
		}
		
		//���ļ��������
		CString tmpStr = tmpPath.Right(nStrLen - nPos);
		FolderStrList.AddHead(tmpStr);
		tmpPath.Delete(nPos, nStrLen - nPos);		
	}	
	
	//�����ļ���������˳����
	while(FolderStrList.IsEmpty() == FALSE)
	{
		tmpPath += FolderStrList.RemoveHead();
		BOOL bRet = FALSE;
		bRet = CreateDirectory(tmpPath, NULL);
		if(bRet == FALSE)
		{
			return FALSE;
		}
	}
	
	return TRUE;
}
// ��ȡ��ִ�г������ڵ�·��
CString CImage::inGetCurrentAppliationPath()
{
	char str[MAX_PATH], drive[MAX_PATH], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
	CString AppPath;
	GetModuleFileName(NULL, str, sizeof(str));
	_splitpath(str, drive, dir, fname, ext);
	strcat(drive, dir);
	AppPath = drive;

	return AppPath;
}

int CImage::AutoSaveImageToPath(LPVOID lpElementParam,int iSaveImageType)
{
	int tmpCountSuccess=0;
	int tmpCountError=0;
	int tmpTotal=0;	
	if (iSaveImageType<0)
	{
		return ParamTypeError;
	}
	if (lpElementParam==NULL)
	{
		return ParamLPElementError;
	}
	CAllInformationElement *pElement=(CAllInformationElement *)lpElementParam;

	int iCircleTotal=pElement->m_listSingleImageData.GetCount();
	if (iCircleTotal<=0)
	{
		return ListCountError;
	}

	switch(iSaveImageType)
	{
	case SaveGrabImage:
		{
			for (int tmpInt=0;tmpInt<iCircleTotal;tmpInt++)
			{
				CGrabImageInfo *pGrabImageInfo=NULL;
				pGrabImageInfo=	(CGrabImageInfo *)pElement->m_listSingleImageData.GetAt(pElement->m_listSingleImageData.FindIndex(tmpInt));
				if (pGrabImageInfo==NULL)
				{
					tmpCountError++;
					continue; 
				}
				else
				{
					if (pGrabImageInfo->m_bHaveImage)
					{
						if (pGrabImageInfo->m_myImage.SaveMeToPath(lpElementParam,SaveGrabImage,tmpInt+1))
						{
							tmpCountSuccess++;
							tmpTotal=tmpTotal+tmpInt;
						}
						else
						{
							tmpCountError++;
						}						
					}					
				}
			}
			if (tmpCountSuccess==iCircleTotal)// ȫ������ɹ� [8/20/2009 GZ]
			{
				return tmpCountSuccess;//���سɹ�����
			}
			else if(tmpCountSuccess>0&&tmpCountSuccess<iCircleTotal)//���ֱ���ɹ�
			{
				return -(tmpCountSuccess*1000+tmpTotal);
			}
			else if(tmpCountSuccess==0)//һ��Ҳû����ɹ�
			{
				return GrabAllFaultError;
			}			
		}	
		break;
	case SaveErrorImage:
		{
			return 0;
		}	
	    break;
	case SaveAbnormityImage:
		{
			return 0;
		}	
	    break;
	}
	return tmpCountSuccess;
}

//��ͼ�����ݽ��о���
int CImage::MirrorSingleImage(e_ShowModeFlip  eShowMode) //ֻ��ˮƽ�ʹ�ֱ����
{
	int lImageWidth,lImageHeight,lImageBitCount;
	BYTE* pImageBuff=NULL;
	pImageBuff=GetImageBuffAddr();
	lImageWidth=GetImageWidth();
	lImageHeight=GetImageHeight();
	lImageBitCount=GetImageBitCount();
	
	if (pImageBuff == NULL)
	{
		return ImageBufferError;
	}
	if (lImageWidth<0||lImageHeight<0||lImageBitCount<0)
	{
		return ImageWHBITError;
	}

	if (lImageBitCount==8)
	{	
		lImageWidth=WIDTHBYTES(lImageWidth*8);
		if (eShowMode==FlipHorizintal) //ˮƽ��ת
		{
			BYTE  tem;
			for (int i=0;i<lImageHeight;i++)
			{
				for (int j=0;j<lImageWidth/2;j++)
				{
					tem=*(pImageBuff+i*lImageWidth+j);
					*(pImageBuff+i*lImageWidth+j)=*(pImageBuff+i*lImageWidth+lImageWidth-1-j);
					*(pImageBuff+i*lImageWidth+lImageWidth-1-j)=tem;
				}
			}
			return TRUE;
		}
		if (eShowMode==FlipVertical) //��ֱ��ת
		{
			BYTE  tem;
			for (int i=0;i<lImageHeight/2;i++)
			{
				for (int j=0;j<lImageWidth;j++)
				{
					tem=*(pImageBuff+i*lImageWidth+j);
					*(pImageBuff+i*lImageWidth+j)=*(pImageBuff+(lImageHeight-1-i)*lImageWidth+j);
					*(pImageBuff+(lImageHeight-1-i)*lImageWidth+j)=tem;
				}
			}
			return TRUE;
		}
	}
	if (lImageBitCount==24)//24λ����
	{
		lImageWidth=WIDTHBYTES(lImageWidth*8);

		if (eShowMode==FlipHorizintal) //ˮƽ��ת
		{
			BYTE  tem1,tem2,tem3;
			for (int i=0;i<lImageHeight;i++)
			{
				for (int j=0;j<lImageWidth/2;j++)
				{
					tem1=*(pImageBuff+i*lImageWidth*3+3*j);
					tem2=*(pImageBuff+i*lImageWidth*3+3*j+1);
					tem3=*(pImageBuff+i*lImageWidth*3+3*j+2);
					*(pImageBuff+i*lImageWidth*3+3*j)=*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j));
					*(pImageBuff+i*lImageWidth*3+3*j+1)=*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j)+1);
					*(pImageBuff+i*lImageWidth*3+3*j+2)=*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j)+2);
					*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j))=tem1;
					*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j)+1)=tem2;
					*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j)+2)=tem3;
				}
			}
			return TRUE;
		}
		if (eShowMode==FlipVertical) //��ֱ��ת
		{
			BYTE  tem1,tem2,tem3;
			for (int i=0;i<lImageHeight/2;i++)
			{
				for (int j=0;j<lImageWidth;j++)
				{
					tem1=*(pImageBuff+i*lImageWidth*3+3*j);
					tem2=*(pImageBuff+i*lImageWidth*3+3*j+1);
					tem3=*(pImageBuff+i*lImageWidth*3+3*j+2);
					*(pImageBuff+i*lImageWidth*3+3*j)=*(pImageBuff+(lImageHeight-1-i)*lImageWidth*3+3*j);
					*(pImageBuff+i*lImageWidth*3+3*j+1)=*(pImageBuff+(lImageHeight-1-i)*lImageWidth*3+3*j+1);
					*(pImageBuff+i*lImageWidth*3+3*j+2)=*(pImageBuff+(lImageHeight-1-i)*lImageWidth*3+3*j+2);
					*(pImageBuff+(lImageHeight-1-i)*lImageWidth*3+3*j)=tem1;
					*(pImageBuff+(lImageHeight-1-i)*lImageWidth*3+3*j+1)=tem2;
					*(pImageBuff+(lImageHeight-1-i)*lImageWidth*3+3*j+2)=tem3;
				}
			}
			return TRUE;
		}
	}	
	return TRUE;
}

BOOL CImage::OpenBMPFileX(LPCSTR lpFileName,long lPicCount)//[2007-11-21] by SGQ
{
	BOOL Ret = OpenBMPFile(lpFileName, m_pBitmapInfo, m_pImageBuff);
	if (!Ret)
	{
		
		CString str;
		str.Format("CImage::OpenBMPFileX Open %s ʧ��!",lpFileName);
		AfxMessageBox(str);
		
		return FALSE;
	}
	
	m_lImageWidth = m_pBitmapInfo->bmiHeader.biWidth; 
	m_lImageHeight = m_pBitmapInfo->bmiHeader.biHeight;
	m_lImageBitCount= (long)(m_pBitmapInfo->bmiHeader.biBitCount);
	long lImageSize = m_lImageWidth * m_lImageHeight * (m_lImageBitCount/8)*lPicCount;
	if(lImageSize > m_lBuffSize)
	{
		throw(CImageException(2, "�������Ϸ�", "AcceptImage()"));
	}
	m_lImageSize = lImageSize;
	
	return TRUE;
}