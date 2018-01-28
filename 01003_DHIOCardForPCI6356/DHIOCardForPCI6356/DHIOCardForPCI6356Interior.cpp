#include "stdafx.h"
#include "DHIOCardForPCI6356Interior.h"


CDHIOCardForPCI6356Interior::CDHIOCardForPCI6356Interior()
{
}


CDHIOCardForPCI6356Interior::~CDHIOCardForPCI6356Interior()
{
}

bool CDHIOCardForPCI6356Interior::InitCard(CIOCardInitInfo& rIOCardInitInfo)
{	

	m_IOCardInitInfo = rIOCardInitInfo;
	m_iniCfgOfInit.Init(m_IOCardInitInfo.lpszPathFileOfInit);
	m_iniCfgOfConnection.Init(m_IOCardInitInfo.lpszPathFileOfConnection);
	if (FALSE == PathFileExists(m_IOCardInitInfo.lpszPathFileOfInit))
	{
		m_sLastErrorInfo.Format(_T("�ӿڿ������ļ���%d�������ڣ�"), m_IOCardInitInfo.lpszPathFileOfInit);
		return false;
	}
	if (FALSE == PathFileExists(m_IOCardInitInfo.lpszPathFileOfConnection))
	{
		m_sLastErrorInfo.Format(_T("�ӿڿ������ļ���%d�������ڣ�"), m_IOCardInitInfo.lpszPathFileOfConnection);
		return false;
	}

	/*************************************<��ʼ��ϵͳ��ӿڿ�IO����>************************************[12/18/2017 ConanSteve]*/
	CString sModuleName{ _T("") }, sKeyName{ _T("") };
	int iKeyValue{ -1 };
	sModuleName = _T("��������");
	m_iGrabNum = m_iniCfgOfConnection.GetIntEx(sModuleName, _T("�������"), 0);
	for (int i = 0; i < m_iGrabNum; ++i)
	{
		sModuleName.Format(_T("���%d"), i + 1);
		sKeyName.Format(_T("�ܿش�������˿ں�"));
		iKeyValue = m_iniCfgOfConnection.GetIntEx(sModuleName, sKeyName, -1);
		if (-1 != iKeyValue)
			m_map_OutportOfGrabRelated.insert(make_pair(i, iKeyValue));
		s_OutputCtrlInfo sOutputCtrlInfo;
		sOutputCtrlInfo.iChannel = m_iniCfgOfConnection.GetIntEx(sModuleName, _T("�ܿش������к�"), -1);
		sOutputCtrlInfo.iLogicalOutport = m_iniCfgOfConnection.GetIntEx(sModuleName, _T("�ܿش����߼��˿ں�"), -1);
		if(-1 != sOutputCtrlInfo.iChannel && -1!= sOutputCtrlInfo.iLogicalOutport)
			m_map_GrabSN2LogicalOutport.insert(make_pair(i, sOutputCtrlInfo));
	}

	//**[12/13/2017 ConanSteve]**:  �߷��ź����
	m_iKickQueueNum = m_iniCfgOfConnection.GetIntEx(_T("��������"), _T("�߷Ѷ�����"), 1);
	for (int i = 0; i < m_iKickQueueNum; ++i)
	{
		sModuleName.Format(_T("�߷Ѷ���%d"), i);
		int iKickNumOfCurQueue = m_iniCfgOfConnection.GetIntEx(sModuleName, _T("�߷ѿ���"), 1);//**[12/13/2017 ConanSteve]**:  ��ǰ�߷Ѷ��е��߷ѿڸ���
		int iSysKickSN = i;
		//iSysKickSN = m_iniCfgOfConnection.GetIntEx(sModuleName, _T("��ǰ�ӿڿ��߷Ѷ��ж�Ӧϵͳ�߷Ѷ���SN"), i);
		m_map_SysKickSN2CardKickId.insert(make_pair(iSysKickSN, i));
		m_arr_sKickCntInfo[i].iKickSN = i;
		m_arr_sKickCntInfo[i].iKickPortNum = iKickNumOfCurQueue;
		for (int j = 0; j < iKickNumOfCurQueue; ++j)
		{
			sKeyName.Format(_T("�߷Ѷ�������%d��ӦOutportId"), j);
			int iPhysicalPort = m_iniCfgOfConnection.GetIntEx(sModuleName, sKeyName, -1);
			if(iPhysicalPort<0||iPhysicalPort>23)
				AfxMessageBox(sKeyName + _T("����ȷ������Connection�ļ����ã�"));
			m_arr_sKickCntInfo[i].arr_iKickPortId[j] = iPhysicalPort;
			sKeyName.Format(_T("�߷Ѷ�������%d��Ӧ�߼��˿ں�"), j);
			int iLogicalPortId = m_iniCfgOfConnection.GetIntEx(sModuleName, sKeyName, -1);
			if (iLogicalPortId < 0 || iLogicalPortId>23)
				AfxMessageBox(sKeyName + _T("����ȷ������Connection�ļ����ã�"));
			if (-1!=iPhysicalPort && -1 != iLogicalPortId)
			{
				//**[12/18/2017 ConanSteve]**:  ��ʱ�߷Ѷ��кź�ͨ����һ��ֵ
				m_map_KickPortDict.insert(make_pair(i*100+j, s_KickKeyValue(i, iLogicalPortId)));
			}
		}
	}
	return true;
}

bool CDHIOCardForPCI6356Interior::OpenCard()
{
	m_iDevSN = m_iniCfgOfInit.GetIntEx(_T("����ģ��"), _T("�豸���"), 0);
	//**[12/15/2017 ConanSteve]**:  ����ӿڿ��Ѿ��򿪣���رսӿڿ�
	if (TRUE == m_IOCard.IsOpen())
	{
		m_IOCard.Close();
	}
	m_bInitSuccess = TRUE == m_IOCard.Open(m_iDevSN, NULL);
	if (!m_bInitSuccess)
	{
		m_sLastErrorInfo = _T("�򿪽ӿڿ�ʧ�ܣ�");
		return false;
	}

	s_InitParam sInit;
	sInit.pContext = this;
	sInit.pFunc = GlobalIOCallback;
	sInit.strPath = m_IOCardInitInfo.lpszPathFileOfInit;
	bool bRet =  TRUE == m_IOCard.ReadParameterFile(&sInit);
	if (!bRet)//**[12/18/2017 ConanSteve]**:  �����ʧ��
	{
		m_IOCard.Close();
		return false;
	}
	m_bNeedReadCardParam = FALSE;

	ResetIOCard();
	return bRet;
}

bool CDHIOCardForPCI6356Interior::CloseCard()
{
	BOOL bRet = false;
	if (m_bInitSuccess)
	{
		try
		{
			bRet = m_IOCard.PauseInCntWork(-1);
			for (int i = 0; i < 24; ++i)
			{
				//bRet = m_IOCard.SetOutIOStatus(i, FALSE);
			}
			bRet == m_IOCard.Close();
		}
		catch (...)
		{
			m_sLastErrorInfo = _T("�رսӿڿ��쳣");
			return false;
		}
	}
	return true;
}

bool CDHIOCardForPCI6356Interior::ResetIOCard()
{
	m_IOCard.ResetALL();
	CString sPathFolderOfLog{ _T("") }, sVirtualDisk{ _T("") }, sPathFileOfLog{_T("")};
	CINICfg iniAlgCfg(CExFn::GetCurrentAppPath() + _T("\\Config\\AlgConfig.ini"));
	sVirtualDisk = iniAlgCfg.GetString(_T("Demo Controls"), _T("VirtualDisk"), sVirtualDisk);
	sPathFolderOfLog.Format(_T(":\\LogFiles\\PCI6356_%d\\"), m_iDevSN);
	sPathFolderOfLog = sVirtualDisk + sPathFolderOfLog;
	CExFn::CreateMultiDirectory(sPathFolderOfLog);
	m_bEnableLog = iniAlgCfg.GetBool(_T("Other Parameters"), _T("bWriteLog"), false);
	for (int i = 0; i < m_iKickQueueNum; ++i)
	{
		sPathFileOfLog.Format(_T("%sKickQueueInfo_%d.txt"), sPathFolderOfLog, i);
		m_arr_fileKick[i].Close();
		m_arr_fileKick[i].Open(sPathFileOfLog, &m_bEnableLog);
	}
	return false;
}

bool CDHIOCardForPCI6356Interior::OpenTools(CWnd * pParent)
{
	m_IOCard.ShowDialog();
	return true;
}

bool CDHIOCardForPCI6356Interior::Enable(bool bEnable)
{
	EnableQueue(bEnable);
	return true;
}

bool CDHIOCardForPCI6356Interior::EnableQueue(bool bEnable)
{
	m_bEnableWorking = bEnable;
	if (m_bInitSuccess)
	{
		if (m_bEnableWorking)
		{
			m_IOCard.StartWork(FALSE);
		}
		else
		{
			m_IOCard.StopWork(FALSE);
		}
	}
	return false;
}

bool CDHIOCardForPCI6356Interior::GetParam(s_IOCardParaInfo & para)
{
	return false;
}

bool CDHIOCardForPCI6356Interior::SetParam(s_IOCardParaInfo & para)
{
	return false;
}

bool CDHIOCardForPCI6356Interior::WriteKickResult(s_sRsltInfoWritten & rslt)
{
	if (m_bInitSuccess&&m_bEnableWorking)
	{
		auto itera_find = m_map_SysKickSN2CardKickId.find(rslt.iKickSN);
		if (m_map_SysKickSN2CardKickId.end() != itera_find)
		{
			int iChannel = 0;//**[12/18/2017 ConanSteve]**:  �߷Ѷ������
			int iKickIdOfIOCard = itera_find->second;
			int iResult{ 0 };//**[12/18/2017 ConanSteve]**:  ����д����ֵ
			int iLogicalPort{ 0 };
			CString sInfo{ _T("") }, sTemp{_T("")};
			sInfo.Format(_T("[ChannelID:%02d][ImageSN:%08d][KickRsltNum:%d]"), iChannel, rslt.iImgSN, rslt.iRsltNum);
			for (int i = 0; i < rslt.iRsltNum; ++i)
			{
				auto itera_KickKeyValue = m_map_KickPortDict.find(iKickIdOfIOCard+i);
				if (m_map_KickPortDict.end() != itera_KickKeyValue)
				{
					iChannel = itera_KickKeyValue->second.iChannel;
					iLogicalPort = itera_KickKeyValue->second.iLogicalOutport;
					if (-1 != iLogicalPort)//**[12/18/2017 ConanSteve]**:  ����߼��˿ں���ȷ
					{
						if (rslt.arr_bKick[i])//**[12/18/2017 ConanSteve]**:  �����Ҫ�߷ѣ�λ��1
						{
							iResult |= 1 << iLogicalPort;
						}
						else//**[12/18/2017 ConanSteve]**:  ����λ��0
						{
							iResult &= ~(1 << iLogicalPort);
						}
					}
				}
			}
			TCHAR bufTemp[20];
			memset(bufTemp, 0, sizeof(bufTemp));
			_itot(iResult, bufTemp, 2);
			sTemp.Format(_T("%024s"), bufTemp);
			sInfo += sTemp;
			m_arr_fileKick[iChannel].Write(sInfo);
			m_IOCard.SetOutCtrlByResultEx(iChannel, (rslt.iImgSN - 1) % 256, iResult);
			return true;
		}
	}
	return false;
}

bool CDHIOCardForPCI6356Interior::NotifyCardOfGrabCallBack(s_NotifyInfoOfGrabCallback & notifyInfo)
{
	return false;
}

e_IOCardType CDHIOCardForPCI6356Interior::GetIOCardType()
{
	return e_IOCardType();
}

CString CDHIOCardForPCI6356Interior::GetLastError()
{
	return CString();
}

bool CDHIOCardForPCI6356Interior::GetInputIndex(int iInputport, int &iValue)
{
	if (m_bInitSuccess)
	{
		iValue = m_IOCard.ReadInputCount(iInputport);
		return true;
	}
	return false;
}

bool CDHIOCardForPCI6356Interior::GetGrabImgSN(int iGrabSN, int& iImageSN)
{
	auto itera_find = m_map_GrabSN2LogicalOutport.find(iGrabSN);
	if (m_bInitSuccess&& m_map_GrabSN2LogicalOutport.end()!= itera_find)
	{
		iImageSN =  m_IOCard.ReadOutputIndex(itera_find->second.iChannel, itera_find->second.iLogicalOutport);
		return true;
	}
	return false;
}

bool CDHIOCardForPCI6356Interior::GetKickCountInfo(int iKickSN, s_KickCountInfo& sKickCntInfo)
{
	auto itera_find = m_map_SysKickSN2CardKickId.find(iKickSN);//**[12/18/2017 ConanSteve]**:  ����ϵͳKickSN�ҵ��ӿڿ����߷Ѷ��б��
	if (m_map_SysKickSN2CardKickId.end() != itera_find)//**[12/18/2017 ConanSteve]**:  
	{
		int iKickSNOfIOCard = itera_find->second;
		for (int i = 0; i < m_arr_sKickCntInfo[iKickSNOfIOCard].iKickPortNum; ++i)//**[12/18/2017 ConanSteve]**:  ������ѯÿ��out�ڵ��߷�ֵ
		{
			int iPhysicalPortId = m_arr_sKickCntInfo[iKickSNOfIOCard].arr_iKickPortId[i];
			m_arr_sKickCntInfo[iKickSNOfIOCard].arr_iKickPortCount[i] = m_IOCard.ReadOutputCount(iPhysicalPortId);
			m_arr_sKickCntInfo[iKickSNOfIOCard].arr_iAutoKickCnt[i] = m_IOCard.ReadOutputBTCnt(iPhysicalPortId);
		}
		sKickCntInfo = m_arr_sKickCntInfo[iKickSNOfIOCard];
		return true;
	}
	return false;
}

bool CDHIOCardForPCI6356Interior::SetOutIOStatus(int iOutport, bool bStatus)
{
	if (m_bInitSuccess)
	{
		return TRUE == m_IOCard.SetOutIOStatus(iOutport, bStatus);
	}
	return false;
}

bool CDHIOCardForPCI6356Interior::GetOutputCount(int iOutport, int &iValue)
{
	if (m_bInitSuccess)
	{
		iValue =  m_IOCard.ReadOutputCount(iOutport);
		return true;
	}
	return false;
}

void CDHIOCardForPCI6356Interior::GlobalIOCallback(const s_IOParam * SigInfo)
{

}

void CDHIOCardForPCI6356Interior::GlobalIOCallbackFunc(const s_IOParam * SigInfo)
{
}
