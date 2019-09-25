#include "CommonHead.h"
#include "configManage.h"
#include "log.h"
#include "DataBase.h"


//�����߳̽ṹ����
struct DBThreadParam
{
	int					hEvent;									//�˳��¼�
	//HANDLE					hCompletionPort;						//��ɶ˿�
	CDataBaseManage* pDataManage;							//���ݿ������ָ��
};

CDataBaseManage::CDataBaseManage()
{
	m_bInit = false;
	m_bRun = false;
	m_hThread = 0;
	//m_hCompletePort = NULL;
	m_pInitInfo = NULL;
	m_pKernelInfo = NULL;
	m_pHandleService = NULL;
	m_pMysqlHelper = NULL;

	m_sqlClass = 0;
	m_nPort = 0;
	m_bsqlInit = false;

	m_host[0] = '\0';
	m_user[0] = '\0';
	m_name[0] = '\0';
	m_passwd[0] = '\0';
}

CDataBaseManage::~CDataBaseManage()
{
	m_bInit = false;
	m_pInitInfo = NULL;
	m_hThread = 0;
	//m_hCompletePort = NULL;
	m_pKernelInfo = NULL;
	m_pHandleService = NULL;
}

//��ʼ����
bool CDataBaseManage::Start()
{
	INFO_LOG("DataBaseManage start begin ...");

	if (m_bRun == true || m_bInit == false)
	{
		ERROR_LOG("DataBaseManage already running or not inited m_bRun=%d m_bInit=%d", m_bRun, m_bInit);
		return false;
	}

	m_bRun = true;

	//�����¼�
	int StartEvent = 0;//CreateEvent(FALSE, true, NULL, NULL);

	////������ɶ˿�
	//m_hCompletePort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	//if (m_hCompletePort == NULL)
	//{
	//	ERROR_LOG("CreateIoCompletionPort failed err=%d", GetLastError());
	//	return false;
	//}
	//m_DataLine.SetCompletionHandle(m_hCompletePort);

	SQLConnectReset();
	//SQLConnect();

	//�������ݴ����߳�
	pthread_t threadID = 0;

	DBThreadParam ThreadStartData;
	ThreadStartData.pDataManage = this;
	ThreadStartData.hEvent = StartEvent;
	//ThreadStartData.hCompletionPort = m_hCompletePort;

	int roomID = 0;
	if (m_pInitInfo)
	{
		roomID = m_pInitInfo->uRoomID;
	}

	int err = pthread_create(&threadID, NULL, DataServiceThread, (void*)& ThreadStartData);
	if (err != 0)
	{
		SYS_ERROR_LOG("DataServiceThread failed");
		return false;
	}

	m_hThread = threadID;

	// ������־�ļ�
	GameLogManage()->AddLogFile(threadID, THREAD_TYPE_ASYNC, roomID);

	//// �ȴ����̶߳�ȡ�̲߳���
	//WaitForSingleObject(StartEvent, INFINITE);

	//ResetEvent(StartEvent);

	INFO_LOG("DataBaseManage start end.");

	return true;
}

bool CDataBaseManage::Stop()
{
	INFO_LOG("DataBaseManage stop begin...");

	if (!m_bRun)
	{
		ERROR_LOG("DataBaseManage is not running...");
		return false;
	}

	m_bRun = false;


	//// �ȹر���ɶ˿�
	//if (m_hCompletePort)
	//{
	//	PostQueuedCompletionStatus(m_hCompletePort, 0, NULL, NULL);
	//	CloseHandle(m_hCompletePort);
	//	m_hCompletePort = NULL;
	//}

	// ����dataline
	//m_DataLine.SetCompletionHandle(NULL);
	m_DataLine.CleanLineData();

	// �ر����ݿ⴦���߳̾��
	if (m_hThread)
	{
		pthread_cancel(m_hThread);
		m_hThread = 0;
	}

	//�ر����ݿ�����
	if (m_pMysqlHelper)
	{
		m_pMysqlHelper->disconnect();
		delete m_pMysqlHelper;
		m_pMysqlHelper = NULL;
	}

	INFO_LOG("DataBaseManage stop end.");

	return true;
}

//ȡ����ʼ��
bool CDataBaseManage::UnInit()
{
	return true;
}

//���봦�����
bool CDataBaseManage::PushLine(DataBaseLineHead* pData, UINT uSize, UINT uHandleKind, UINT uIndex, UINT dwHandleID)
{
	//��������
	pData->dwHandleID = dwHandleID;
	pData->uIndex = uIndex;
	pData->uHandleKind = uHandleKind;
	return m_DataLine.AddData(&pData->dataLineHead, uSize, 0) != 0;
}

//���ݿ⴦���߳�
void* CDataBaseManage::DataServiceThread(void* pThreadData)
{
	INFO_LOG("DataServiceThread starting...");

	//���ݶ���
	DBThreadParam* pData = (DBThreadParam*)pThreadData;		//�߳���������ָ��
	CDataBaseManage* pDataManage = pData->pDataManage;				//���ݿ����ָ��
	CDataLine* pDataLine = &pDataManage->m_DataLine;			//���ݶ���ָ��
	IDataBaseHandleService* pHandleService = pDataManage->m_pHandleService;	//���ݴ���ӿ�
	//HANDLE					hCompletionPort = pData->hCompletionPort;			//��ɶ˿�

	//�߳����ݶ�ȡ���
	//::SetEvent(pData->hEvent);

	//���ݻ���
	BYTE					szBuffer[LD_MAX_PART];

	while (pDataManage->m_bRun == true)
	{
		//�ȴ���ɶ˿�
		usleep(THREAD_ONCE_DATABASE);

		while (pDataLine->GetDataCount())
		{
			try
			{
				//������ɶ˿�����
				if (pDataLine->GetData((DataLineHead*)szBuffer, sizeof(szBuffer)) < sizeof(DataBaseLineHead))
				{
					continue;
				}

				pHandleService->HandleDataBase((DataBaseLineHead*)szBuffer);
			}
			catch (...)
			{
				ERROR_LOG("CATCH:%s with %s\n", __FILE__, __FUNCTION__);
				continue;
			}
		}
	}

	INFO_LOG("DataServiceThread exit.");

	pthread_exit(NULL);
}

//�������ݿ�
bool CDataBaseManage::SQLConnectReset()
{
	if (m_bsqlInit == false)
	{
		const DBConfig& dbConfig = ConfigManage()->GetDBConfig();

		m_nPort = dbConfig.port;

		strcpy(m_host, dbConfig.ip);
		strcpy(m_user, dbConfig.user);
		strcpy(m_name, dbConfig.dbName);
		strcpy(m_passwd, dbConfig.passwd);

		m_bsqlInit = true;
	}

	if (m_pMysqlHelper)
	{
		delete m_pMysqlHelper;
	}

	m_pMysqlHelper = new CMysqlHelper;

	//��ʼ��mysql���󲢽�������
	m_pMysqlHelper->init(m_host, m_user, m_passwd, m_name, "", m_nPort);
	try
	{
		m_pMysqlHelper->connect();
	}
	catch (MysqlHelper_Exception& excep)
	{
		ERROR_LOG("�������ݿ�ʧ��:%s", excep.errorInfo.c_str());
		return false;
	}

	return true;
}

bool CDataBaseManage::CheckSQLConnect()
{
	if (!m_pMysqlHelper)
	{
		return false;
	}

	char buf[128] = "";
	sprintf(buf, "select * from %s LIMIT 1", TBL_BASE_GAME);
	bool bConect = false;

	try
	{
		m_pMysqlHelper->sqlExec(buf);
	}
	catch (...)
	{
		bConect = true;
	}

	if (bConect)
	{
		try
		{
			m_pMysqlHelper->connect();
		}
		catch (MysqlHelper_Exception& excep)
		{
			ERROR_LOG("�������ݿ�ʧ��:%s", excep.errorInfo.c_str());
			return false;
		}
	}

	return true;
}

//***********************************************************************************************//
CDataBaseHandle::CDataBaseHandle()
{
	m_pInitInfo = NULL;
	m_pKernelInfo = NULL;
	m_pRusultService = NULL;
	m_pDataBaseManage = NULL;
}

CDataBaseHandle::~CDataBaseHandle()
{
}

bool CDataBaseHandle::SetParameter(IAsynThreadResultService* pRusultService, CDataBaseManage* pDataBaseManage, ManageInfoStruct* pInitData, KernelInfoStruct* pKernelData)
{
	m_pInitInfo = pInitData;
	m_pKernelInfo = pKernelData;
	m_pRusultService = pRusultService;
	m_pDataBaseManage = pDataBaseManage;

	return true;
}

//��ʼ������
bool CDataBaseManage::Init(ManageInfoStruct* pInitInfo, KernelInfoStruct* pKernelInfo, IDataBaseHandleService* pHandleService, IAsynThreadResultService* pResultService)
{
	if (!pInitInfo || !pKernelInfo || !pHandleService || !pResultService)
	{
		throw new CException("CDataBaseManage::Init ��������!", (UINT)0x407, true);
	}

	//Ч�����
	if (m_bInit == true || m_bRun == true)
	{
		throw new CException("CDataBaseManage::Init ״̬Ч��ʧ��", (UINT)0x408, true);
	}

	//��������
	m_pInitInfo = pInitInfo;
	m_pKernelInfo = pKernelInfo;
	m_pHandleService = pHandleService;
	m_DataLine.CleanLineData();

	//��������
	m_bInit = true;

	return true;
}
