#pragma once

#include <event2/event.h>
#include <event2/event_struct.h>


struct ServerTimerInfo
{
	unsigned int elapse;	// ��ʱ���������λ���룩
	long long starttime;	// ��ʼʱ�䣨��λ���룩
	ServerTimerInfo()
	{
		elapse = 10;
		starttime = 0;
	}
};

class CDataLine;
class CSignedLock;
class CServerTimer
{
public:
	CServerTimer();
	~CServerTimer();
	bool Start(CDataLine* pDataLine);
	bool Stop();
	bool SetTimer(unsigned int uTimerID, unsigned int uElapse); //uElapse�Ǻ��뵥λ
	bool KillTimer(unsigned int uTimerID);
	bool ExistsTimer(unsigned int uTimerID);

private:
	// ��ʱ��ִ�к���
	static void TimeoutCB(evutil_socket_t fd, short event, void* arg);
	// ��ʱ���̺߳���
	static void * ThreadCheckTimer(void* pThreadData);
private:
	volatile bool m_bRun;
	std::unordered_map<unsigned int, ServerTimerInfo> m_timerMap;
	CDataLine* m_pDataLine;	// �����dataline����
	CSignedLock* m_pLock; // �߳���
};