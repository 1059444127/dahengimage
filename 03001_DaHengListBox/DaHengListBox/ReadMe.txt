2015-08-31 �޸����ӶԵƼ���ļ��е�֧��
// �Ƽ��Circle�ļ��з���
// E:\SaveImages\Good\2015-08-21-22-22\Circle123 [8/31/2015 Administrator]
BOOL CDHFileBox::DirOrBmp(const CString& strFilePath)
{
	//////////////////////////////////////////////////////////////////////////
	int index = strFilePath.ReverseFind('\\');
	CString strAJDZ=strFilePath.Right(strFilePath.GetLength()-index-1);
	if (strAJDZ.Find("Circle") >=0)//�ļ��е������а���Circle
	{
		//�ж�����ļ��������Ƿ���bmp�ļ�
		CFileFind finder;
		BOOL bRet = finder.FindFile(strFilePath+"\\*.bmp",0);
		if (bRet)
		{
			//�Ƽ��Circle�ļ���
			return TRUE;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (strFilePath.Right(4)==".bmp" || strFilePath.Right(4)==".BMP")
		return TRUE;
	else
		return FALSE;
}

Date: 2017-06-08 version��2, 2, 1, 1 ModifiedBy:ConanSteve
//����ModelListBox������ʾ���� 

Date:2017-06-21		Version��2, 2, 1, 2		 ModifiedBy:�ŷ�
	//1���޸���һ��ģ�岻��ʹ���Ҽ������˵�������
	//2������������ģ�幦��