#include "CommonHead.h"
#include "Exception.h"
#include "INIFile.h"
#include "log.h"
#include "PlatformMessage.h"
#include "configManage.h"
#include "Util.h"
#include "BaseMainManage.h"


//�����߳������ṹ
struct HandleThreadStartStruct
{
	//��������
	HANDLE								hEvent;						//�����¼�
	HANDLE								hCompletionPort;			//��ɶ˿�
	CBaseMainManage* pMainManage;				//���ݹ���ָ��
};

//���캯��
CBaseMainManage::CBaseMainManage()
{
	m_bInit = false;
	m_bRun = false;
	m_hHandleThread = 0;
	m_connectCServerHandle = 0;

	::memset(&m_DllInfo, 0, sizeof(m_DllInfo));
	::memset(&m_InitData, 0, sizeof(m_InitData));
	::memset(&m_KernelData, 0, sizeof(m_KernelData));

	m_pRedis = NULL;
	m_pRedisPHP = NULL;
	m_pTcpConnect = NULL;
	m_pGServerConnect = NULL;
	m_pServerTimer = NULL;
}

CBaseMainManage::~CBaseMainManage()
{
	SAFE_DELETE(m_pRedis);
	SAFE_DELETE(m_pRedisPHP);
	SAFE_DELETE(m_pTcpConnect);
	SAFE_DELETE(m_pGServerConnect);
	SafeDeleteArray(m_pServerTimer);
}

bool CBaseMainManage::Init(ManageInfoStruct* pInitData, IDataBaseHandleService* pDataHandleService)
{
	if (!pInitData || !pDataHandleService)
	{
		ERROR_LOG("invalid input params pInitData=%p pDataHandleService=%p", pInitData, pDataHandleService);
		return false;
	}

	if (m_bInit)
	{
		ERROR_LOG("already inited");
		return false;
	}

	m_InitData = *pInitData;

	bool ret = false;

	ret = PreInitParameter(&m_InitData, &m_KernelData);
	if (!ret)
	{
		throw new CException("CBaseMainManage::Init PreInitParameter �������ڴ���", 0x41A);
	}

	m_pRedis = new CRedisLoader;
	if (!m_pRedis)
	{
		return false;
	}

	ret = m_pRedis->Init();
	if (!ret)
	{
		throw new CException("CBaseMainManage::Init redis��ʼ��ʧ�ܣ�������redis������δ������", 0x401);
	}

	//vip�������Ҫ����php redis
	if (m_InitData.iRoomType != ROOM_TYPE_GOLD)
	{
		m_pRedisPHP = new CRedisPHP;
		if (!m_pRedisPHP)
		{
			return false;
		}

		ret = m_pRedisPHP->Init();
		if (!ret)
		{
			throw new CException("CBaseMainManage::Init redisPHP��ʼ��ʧ�ܣ�������redis PHP������δ������", 0x402);
		}
	}

	ret = pDataHandleService->SetParameter(this, &m_SQLDataManage, &m_InitData, &m_KernelData);
	if (!ret)
	{
		throw new CException("CBaseMainManage::Init pDataHandleService->SetParameterʧ��", 0x41C);
	}

	ret = m_SQLDataManage.Init(&m_InitData, &m_KernelData, pDataHandleService, this);
	if (!ret)
	{
		throw new CException("CBaseMainManage::Init m_SQLDataManage ��ʼ��ʧ��", 0x41D);
	}

	m_pTcpConnect = new CTcpConnect;
	if (!m_pTcpConnect)
	{
		throw new CException("CBaseMainManage::Init new CTcpConnect failed", 0x43A);
	}

	m_pGServerConnect = new CGServerConnect;
	if (!m_pGServerConnect)
	{
		throw new CException("CBaseMainManage::Init new CGServerConnect failed", 0x43A);
	}

	int iServerTimerNums = Min_(MAX_TIMER_THRED_NUMS, ConfigManage()->GetCommonConfig().TimerThreadNumber);
	iServerTimerNums = iServerTimerNums <= 0 ? 1 : iServerTimerNums;
	m_pServerTimer = new CServerTimer[iServerTimerNums];
	if (!m_pServerTimer)
	{
		throw new CException("CBaseMainManage::Init new CServerTimer failed", 0x43A);
	}

	ret = OnInit(&m_InitData, &m_KernelData);
	if (!ret)
	{
		throw new CException("CBaseMainManage::Init OnInit ��������", 0x41B);
	}

	m_bInit = true;

	return true;
}

//ȡ����ʼ������ 
bool CBaseMainManage::UnInit()
{
	//ֹͣ����
	if (m_bRun)
	{
		Stop();
	}

	//���ýӿ�
	OnUnInit();

	//ȡ����ʼ��
	m_bInit = false;
	m_SQLDataManage.UnInit();

	//��������
	memset(&m_DllInfo, 0, sizeof(m_DllInfo));
	memset(&m_InitData, 0, sizeof(m_InitData));
	memset(&m_KernelData, 0, sizeof(m_KernelData));

	//�ͷ�redis
	m_pRedis->Stop();
	SAFE_DELETE(m_pRedis);
	if (m_pRedisPHP)
	{
		m_pRedisPHP->Stop();
		SAFE_DELETE(m_pRedisPHP);
	}

	SAFE_DELETE(m_pTcpConnect);
	SAFE_DELETE(m_pGServerConnect);
	SafeDeleteArray(m_pServerTimer);

	return true;
}

//��������
bool CBaseMainManage::Start()
{
	if (!m_bInit || m_bRun)
	{
		return false;
	}

	m_bRun = true;

	////�����¼�
	//HANDLE StartEvent = CreateEvent(FALSE, TRUE, NULL, NULL);

	////������ɶ˿�
	//m_hCompletePort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	//if (m_hCompletePort == NULL)
	//{
	//	throw new CException(TEXT("CBaseMainManage::Start m_hCompletePort ����ʧ��"), 0x41D);
	//}
	//m_DataLine.SetCompletionHandle(m_hCompletePort);

	//���������߳�
	pthread_t uThreadID = 0;
	HandleThreadStartStruct	ThreadStartData;
	ThreadStartData.pMainManage = this;
	//ThreadStartData.hCompletionPort = m_hCompletePort;
	//ThreadStartData.hEvent = StartEvent;
	int err = pthread_create(&uThreadID, NULL, LineDataHandleThread, (void*)& ThreadStartData);
	if (err != 0)
	{
		SYS_ERROR_LOG("LineDataHandleThread failed");
		throw new CException("CBaseMainManage::Start LineDataHandleThread �߳�����ʧ��", 0x41E);
	}
	m_hHandleThread = uThreadID;

	// ������Ϸҵ���߼��߳����Ӧ��־�ļ�
	GameLogManage()->AddLogFile(uThreadID, THREAD_TYPE_LOGIC, m_InitData.uRoomID);

	//WaitForSingleObject(StartEvent, INFINITE);

	bool ret = true;
	ret = m_SQLDataManage.Start();
	if (!ret)
	{
		throw new CException("CBaseMainManage::m_SQLDataManage.Start ���ݿ�ģ������ʧ��", 0x420);
	}

	//////////////////////////////////���������ķ�������////////////////////////////////////////
	const CenterServerConfig& centerServerConfig = ConfigManage()->GetCenterServerConfig();
	ret = m_pTcpConnect->Start(&m_DataLine, centerServerConfig.ip, centerServerConfig.port, SERVICE_TYPE_LOADER, m_InitData.uRoomID);
	if (!ret)
	{
		throw new CException("CBaseMainManage::m_pTcpConnect.Start ����ģ������ʧ��", 0x433);
	}

	err = pthread_create(&m_connectCServerHandle, NULL, TcpConnectThread, (void*)this);
	if (err != 0)
	{
		SYS_ERROR_LOG("TcpConnectThread failed");
		throw new CException("CBaseMainManage::m_pTcpConnect.Start �����̺߳�������ʧ��", 0x434);
	}

	//////////////////////////////////�������¼��������////////////////////////////////////////
	ret = m_pGServerConnect->Start(&m_DataLine, m_InitData.uRoomID);
	if (!ret)
	{
		throw new CException("CBaseMainManage::m_pGServerConnect.Start ����ģ������ʧ��", 0x433);
	}

	//////////////////////////////////������ʱ��////////////////////////////////////////

	for (int i = 0; i < GetNewArraySize(m_pServerTimer); i++)
	{
		if (!m_pServerTimer[i].Start(&m_DataLine))
		{
			throw new CException("CBaseMainManage::m_pServerTimer.Start ��ʱ������ʧ��", 0x433);
		}
	}

	//////////////////////////////////////////////////////////////////////////

	ret = OnStart();
	if (!ret)
	{
		throw new CException("CBaseMainManage::Start OnStart ��������", 0x422);
	}

	//��Ϣһ��ʱ��
	usleep(THREAD_ONCE_HANDLE_MSG);

	return true;
}

//ֹͣ����
bool CBaseMainManage::Stop()
{
	if (m_bRun == false)
	{
		return true;
	}

	OnStop();

	m_bRun = false;

	//m_DataLine.SetCompletionHandle(NULL);

	m_SQLDataManage.Stop();

	if (m_pTcpConnect)
	{
		m_pTcpConnect->Stop();
	}

	if (m_pGServerConnect)
	{
		m_pGServerConnect->Stop();
	}

	////�ر���ɶ˿�
	//if (m_hCompletePort != NULL)
	//{
	//	::PostQueuedCompletionStatus(m_hCompletePort, 0, NULL, NULL);
	//	::CloseHandle(m_hCompletePort);
	//	m_hCompletePort = NULL;
	//}

	// �ر����ķ������߳�
	if (m_connectCServerHandle)
	{
		pthread_cancel(m_connectCServerHandle);
		m_connectCServerHandle = 0;
	}

	//�˳������߳�
	if (m_hHandleThread)
	{
		pthread_cancel(m_hHandleThread);
		m_hHandleThread = 0;
	}

	//�رն�ʱ��
	for (int i = 0; i < GetNewArraySize(m_pServerTimer); i++)
	{
		m_pServerTimer[i].Stop();
	}

	//�����������
	m_DataLine.CleanLineData();

	return true;
}

//ˢ�·���
bool CBaseMainManage::Update()
{
	return OnUpdate();
}

//�첽�߳̽������
bool CBaseMainManage::OnAsynThreadResultEvent(UINT uHandleKind, UINT uHandleResult, void* pData, UINT uResultSize, UINT uDataType, UINT uHandleID)
{
	AsynThreadResultLine resultData;

	//��װ����
	resultData.LineHead.uSize = uResultSize;
	resultData.LineHead.uDataKind = uDataType;

	resultData.uHandleKind = uHandleKind;
	resultData.uHandleResult = uHandleResult;
	resultData.uHandleID = uHandleID;

	//�������
	return (m_DataLine.AddData(&resultData.LineHead, sizeof(resultData), HD_ASYN_THREAD_RESULT, pData, uResultSize) != 0);
}

//�趨��ʱ��
bool CBaseMainManage::SetTimer(UINT uTimerID, UINT uElapse)
{
	if (!m_pServerTimer)
	{
		ERROR_LOG("no timer run");
		return false;
	}

	int iTimerCount = GetNewArraySize(m_pServerTimer);
	if (iTimerCount <= 0 || iTimerCount > MAX_TIMER_THRED_NUMS)
	{
		ERROR_LOG("timer error");
		return false;
	}

	m_pServerTimer[uTimerID % iTimerCount].SetTimer(uTimerID, uElapse);

	return true;
}

//�����ʱ��
bool CBaseMainManage::KillTimer(UINT uTimerID)
{
	if (!m_pServerTimer)
	{
		ERROR_LOG("no timer run");
		return false;
	}

	int iTimerCount = GetNewArraySize(m_pServerTimer);
	if (iTimerCount <= 0 || iTimerCount > MAX_TIMER_THRED_NUMS)
	{
		ERROR_LOG("timer error");
		return false;
	}

	m_pServerTimer[uTimerID % iTimerCount].KillTimer(uTimerID);

	return true;
}

//�������ݴ����߳�
void* CBaseMainManage::LineDataHandleThread(void* pThreadData)
{
	//���ݶ���
	HandleThreadStartStruct* pData = (HandleThreadStartStruct*)pThreadData;		//�߳���������ָ��
	CBaseMainManage* pMainManage = pData->pMainManage;						//���ݹ���ָ��
	CDataLine* m_pDataLine = &pMainManage->m_DataLine;				//���ݶ���ָ��
	HANDLE	hCompletionPort = pData->hCompletionPort;				//��ɶ˿�

	//�߳����ݶ�ȡ���
	//::SetEvent(pData->hEvent);

	//���ݻ���
	BYTE						szBuffer[LD_MAX_PART];
	DataLineHead* pDataLineHead = (DataLineHead*)szBuffer;

	while (pMainManage->m_bRun)
	{
		////�ȴ���ɶ˿�
		//bSuccess = ::GetQueuedCompletionStatus(hCompletionPort, &dwThancferred, &dwCompleteKey, (LPOVERLAPPED*)& OverData, INFINITE);
		//if (bSuccess == FALSE || dwThancferred == 0)
		//{
		//	continue;
		//}
		usleep(THREAD_ONCE_HANDLE_MSG);

		while (m_pDataLine->GetDataCount())
		{
			try
			{
				unsigned int size = m_pDataLine->GetData(pDataLineHead, sizeof(szBuffer));
				if (size == 0)
				{
					continue;
				}

				switch (pDataLineHead->uDataKind)
				{
				case HD_SOCKET_READ://SOCKET���ݶ�ȡ
				{
					SocketReadLine* pSocketRead = (SocketReadLine*)pDataLineHead;
					if (pMainManage->OnSocketRead(&pSocketRead->netMessageHead,
						pSocketRead->uHandleSize ? pSocketRead + 1 : NULL,
						pSocketRead->uHandleSize, pSocketRead->uAccessIP,
						pSocketRead->uIndex, pSocketRead->dwHandleID) == false)
					{
						//ERROR_LOG("OnSocketRead failed mainID=%d assistID=%d", pSocketRead->NetMessageHead.uMainID, pSocketRead->NetMessageHead.uAssistantID);
					}
					break;
				}
				case HD_ASYN_THREAD_RESULT://�첽�̴߳�����
				{
					AsynThreadResultLine* pDataRead = (AsynThreadResultLine*)pDataLineHead;
					void* pBuffer = NULL;
					unsigned int size = pDataRead->LineHead.uSize;

					if (size < sizeof(AsynThreadResultLine))
					{
						ERROR_LOG("AsynThreadResultLine data error !!!");
						break;
					}

					if (size > sizeof(AsynThreadResultLine))
					{
						pBuffer = (void*)(pDataRead + 1);			// �ƶ�һ��SocketReadLine
					}

					pMainManage->OnAsynThreadResult(pDataRead, pBuffer, size - sizeof(AsynThreadResultLine));

					break;
				}
				case HD_TIMER_MESSAGE://��ʱ����Ϣ
				{
					ServerTimerLine* pTimerMessage = (ServerTimerLine*)pDataLineHead;
					pMainManage->OnTimerMessage(pTimerMessage->uTimerID);
					break;
				}
				case HD_PLATFORM_SOCKET_READ:	// ���ķ���������������Ϣ
				{
					PlatformDataLineHead* pPlaformMessageHead = (PlatformDataLineHead*)pDataLineHead;
					int sizeCenterMsg = pPlaformMessageHead->platformMessageHead.MainHead.uMessageSize - sizeof(PlatformMessageHead);
					UINT msgID = pPlaformMessageHead->platformMessageHead.AssHead.msgID;
					int userID = pPlaformMessageHead->platformMessageHead.AssHead.userID;
					void* pBuffer = NULL;
					if (sizeCenterMsg > 0)
					{
						pBuffer = (void*)(pPlaformMessageHead + 1);
					}

					pMainManage->OnCenterServerMessage(msgID, &pPlaformMessageHead->platformMessageHead.MainHead, pBuffer, sizeCenterMsg, userID);
					break;
				}
				default:
					break;
				}
			}

			catch (int iCode)
			{
				CON_ERROR_LOG("[ LoaderServer ��ţ�%d ] [ �����������core�ļ�����鿴core�ļ� ] [ Դ����λ�ã�δ֪ ]", iCode);
				continue;
			}

			catch (...)
			{
				CON_ERROR_LOG("#### δ֪������####");
				continue;
			}
		}
	}

	//INFO_LOG("THREAD::thread LineDataHandleThread exit!!!");

	pthread_exit(NULL);
}

//////////////////////////////////////////////////////////////////////////
// ���ķ������߳�
void * CBaseMainManage::TcpConnectThread(void* pThreadData)
{
	CBaseMainManage* pThis = (CBaseMainManage*)pThreadData;
	if (!pThis)
	{
		CON_ERROR_LOG("pThis==NULL");
		pthread_exit(NULL);
	}

	CTcpConnect* pTcpConnect = (CTcpConnect*)pThis->m_pTcpConnect;
	if (!pTcpConnect)
	{
		pthread_exit(NULL);
	}

	while (pThis->m_bRun && pThis->m_pTcpConnect)
	{
		pTcpConnect->EventLoop();
		pTcpConnect->CheckConnection();
	}

	pthread_exit(NULL);
}