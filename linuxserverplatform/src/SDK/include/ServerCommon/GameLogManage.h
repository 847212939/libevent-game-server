#pragma once

#include <map>
#include <string>
#include "Function.h"

struct ThreadLogFiles
{
	std::string errorLog;
	std::string costLog;

	void Clear()
	{
		errorLog.clear();
		costLog.clear();
	}
};

// ��Ϸlog����
class CGameLogManage
{
private:
	CGameLogManage();
	~CGameLogManage();

public:
	static CGameLogManage* Instance();
	void Release();

	// �ⲿ�ӿ�����߳���־
	void AddLogFile(pthread_t threadID, int threadType, int roomID = 0);
	// ��ȡ��Ӧ�̵߳�errorlog
	std::string GetErrorLog(pthread_t threadID);
	// ��ȡ��Ӧ�̵߳�costLog
	std::string GetCostLog(pthread_t threadID);

	// ����ָ���ļ���fp
	bool AddLogFileFp(std::string strFile, FILE* fp);
	// ��ȡָ���ļ���fp
	FILE* GetLogFileFp(std::string&& strFile);

private:
	// ������ķ����������־�ļ�
	void AddCenterLogFile(pthread_t threadID, int threadType);

	// ��Ӵ��������������־�ļ�
	void AddLogonLogFile(pthread_t threadID, int threadType);

	// �����Ϸ�����������־�ļ�
	void AddLoaderLogFile(pthread_t threadID, int threadType, int roomID);

private:
	// ��Ϸ��־�ļ�map
	std::map<pthread_t /*threadID*/, ThreadLogFiles /*logFileName*/> m_loaderLogFilesMap;
	// ������־�ļ�map
	std::map<pthread_t /*threadID*/, ThreadLogFiles /*logFileName*/> m_logonLogFilesMap;
	// ���ķ���־�ļ�map
	std::map<pthread_t /*threadID*/, ThreadLogFiles /*logFileName*/> m_centerLogFilesMap;

	// �ļ�������map
	std::map<std::string, FILE*> m_filesFpMap;
};

#define GameLogManage()		CGameLogManage::Instance()