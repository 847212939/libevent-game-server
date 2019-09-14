#pragma once

#define GSERVER_SOCKET_SEND_BUF		(128 * 1024)		// ���ͻ�������С
#define GSERVER_SOCKET_RECV_BUF		(64 * 1024)			// ���ջ�������С
#define MAX_RECONNECT_COUNT			5					// �������������0����������

class CDataLine;
class CGServerConnect;
class CSignedLock;

// ����socket
class CGServerClient
{
public:
	CGServerClient();
	~CGServerClient();

public:
	bool Init(CDataLine* pDataLine, CGServerConnect* pCGServerConnect, int index, char ip[24], int port);
	bool Connect();

	// ������
	bool OnRead();

	// ������
	bool Send(const void* pData, int size);

	// socket�ر�
	bool OnClose();

	// ��ȡsocket����
	bool IsConnected();
	int GetSocket() { return m_socket; }
	int GetSocketIdx() { return m_index; }
	int GetRemainSendBufSize() { return m_remainSendBytes; }

	// ��շ���������(��ʼ��������������һ���������)
	void Clear();

	// ������غ���
	bool IsNeedReConnect();
	bool ReConnect();
private:
	int m_socket;
	volatile bool m_isConnected;
	volatile int m_ReConnectCount;

	char m_recvBuf[GSERVER_SOCKET_RECV_BUF];
	int  m_remainRecvBytes;

	char m_sendBuf[GSERVER_SOCKET_SEND_BUF];
	int m_remainSendBytes;

	CDataLine* m_pDataLine;			//�����dataline����
	CSignedLock m_lock;
	CGServerConnect* m_pCGServerConnect;		//���������
	int m_index;
	char m_ip[24];
	int m_port;
};

// socket����
class CGServerConnect
{
public:
	CGServerConnect();
	~CGServerConnect();

public:
	bool Start(CDataLine* pDataLine, int roomID);
	bool Stop();

public:
	// �������ݺ���
	bool SendData(int idx, void* pData, int size, int mainID, int assistID, int handleCode, unsigned int uIdentification);

	// �ӿ�
public:
	int GetRoomID();
	const std::vector<CGServerClient*>& GetSocketVec();
	void GetIndexByThreadID(unsigned int uThreadID, size_t& uMin, size_t& uMax);

	// �̺߳���
private:
	// SOCKET ��������߳�
	static void* ThreadCheckConnect(void* pThreadData);
	// SOCKET ���ݽ����߳�
	static void* ThreadRSSocket(void* pThreadData);

private:
	std::vector<CGServerClient*> m_socketVec;
	std::map<pthread_t, int> m_threadIDToIndexMap;
	CDataLine* m_pDataLine;
	int m_roomID;
	volatile bool m_running;
	pthread_t	m_hThreadCheckConnect;
};