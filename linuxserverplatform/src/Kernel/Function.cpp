#include "CommonHead.h"
#include <sys/syscall.h>
#include "Function.h"

//获取动态数组指针大小
int GetNewArraySize(void* pArray)
{
	if (pArray == NULL)
	{
		return 0;
	}

	int* p = (int*)pArray;
	int iSize = *(p - 2);

	if (iSize < 0 || iSize > INT32_MAX)
	{
		return 0;
	}

	return iSize;
}

// 获取系统时间
void GetLocalTime(SYSTEMTIME* sysTime)
{
	if (!sysTime)
	{
		return;
	}

	struct timeval tv;
	gettimeofday(&tv, NULL);

	time_t tt = tv.tv_sec;
	tt = tt + 8 * 3600;  // transform the time zone
	tm* t = gmtime(&tt);

	if (!t)
	{
		return;
	}

	sysTime->wYear = t->tm_year + 1900;
	sysTime->wMonth = t->tm_mon + 1;
	sysTime->wDayOfWeek = t->tm_wday;
	sysTime->wDay = t->tm_mday;
	sysTime->wHour = t->tm_hour;
	sysTime->wMinute = t->tm_min;
	sysTime->wSecond = t->tm_sec;
	sysTime->wMilliseconds = tv.tv_usec / 1000;
	sysTime->lMicroseconds = tv.tv_usec;
}

//获取时间戳（单位：毫秒）
long long GetSysMilliseconds()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);

	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

//获取线程pid
pthread_t GetCurrentThreadId()
{
	return pthread_self();
}

pthread_t GetCurrentSysThreadId()
{
	return syscall(SYS_gettid);
}