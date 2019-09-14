#pragma once

class CSignedLock;
class CSignedLockObject
{
private:
	int			m_iLockCount;		//��������
	CSignedLock* m_pLockObject;		//��������

public:
	CSignedLockObject(CSignedLock* pLockObject, bool bAutoLock);
	~CSignedLockObject();

public:
	inline void Lock();
	inline void UnLock();
};

// pthread_mutex_init  pthread_mutexattr_t �ĵڶ���������ϸ���� 
// PTHREAD_MUTEX_TIMED_NP������ȱʡֵ��Ҳ������ͨ������һ���̼߳����Ժ��������������߳̽��γ�һ���ȴ����У����ڽ��������ȼ�����������������Ա�֤����Դ����Ĺ�ƽ�ԡ�
// PTHREAD_MUTEX_RECURSIVE_NP��Ƕ����������ͬһ���̶߳�ͬһ�����ɹ���ö�Σ���ͨ�����unlock����������ǲ�ͬ�߳��������ڼ����߳̽���ʱ���¾�����
// PTHREAD_MUTEX_ERRORCHECK_NP������������ͬһ���߳�����ͬһ�������򷵻�EDEADLK��������PTHREAD_MUTEX_TIMED_NP���Ͷ�����ͬ��������֤���������μ���ʱ�������������µ�������
// PTHREAD_MUTEX_ADAPTIVE_NP����Ӧ����������򵥵������ͣ����ȴ����������¾�����

class CSignedLock
{
	friend class CSignedLockObject;

private:
	pthread_mutex_t	m_csLock;
	pthread_mutexattr_t m_attr;

public:
	CSignedLock();
	~CSignedLock();

private:
	inline void Lock() { pthread_mutex_lock(&m_csLock); }
	inline void UnLock() { pthread_mutex_unlock(&m_csLock); }
};