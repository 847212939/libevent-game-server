//////////////////////////////////////////////////////////////////////////
/// ������Ϣ��������Ϣ�����������
/// ���У�GameLog���ڵ��Ը���ʱ�����Ϣ������ʱ�����޸ĺ����壬��д�ļ�ֱ�ӷ���
#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include "../GameMessage/upgrademessage.h"

//��ӡ��־
void GameLog::OutputFile(const char *strOutputString, ...)
{
#ifdef _RELEASEEX
	//��Ӫ���ֹ����OutputFile��������Ϊ�ú�����ʱԼ1.5����
	return;
#endif // _RELEASEEX
	char szFilename[256];
	CString strPath = CINIFile::GetAppPath() + GAMENAME + "_log\\";
	SHCreateDirectoryEx(NULL, strPath, NULL);
	CTime time = CTime::GetCurrentTime();
	sprintf(szFilename, "%s%d__%d%02d%02d_log.txt", strPath.GetBuffer(), NAME_ID, time.GetYear(), time.GetMonth(), time.GetDay());
	FILE *fp = fopen(szFilename, "a");
	if (NULL == fp)
	{
		return;
	}
	//����ʱ��
	char cTime[30];
	::memset(cTime, 0, sizeof(cTime));
	sprintf(cTime, "[%d:%d:%d] ", time.GetHour(), time.GetMinute(), time.GetSecond());
	fprintf(fp, cTime);
	va_list arg;
	va_start(arg, strOutputString);
	vfprintf(fp, strOutputString, arg);
	fprintf(fp, "\n");
	fclose(fp);
}

void GameLog::SFile(const char * strOutputString, ...)
{
	char szFilename[256];
	CString strPath = CINIFile::GetAppPath() + GAMENAME + "_log\\";
	SHCreateDirectoryEx(NULL, strPath, NULL);
	CTime time = CTime::GetCurrentTime();
	sprintf(szFilename, "%s%d__%d%02d%02d_������־.txt", strPath.GetBuffer(), NAME_ID, time.GetYear(), time.GetMonth(), time.GetDay());
	FILE *fp = fopen(szFilename, "a");
	if (NULL == fp)
	{
		return;
	}
	//����ʱ��
	char cTime[30];
	::memset(cTime, 0, sizeof(cTime));
	sprintf(cTime, "[%d:%d:%d] ", time.GetHour(), time.GetMinute(), time.GetSecond());
	fprintf(fp, cTime);
	va_list arg;
	va_start(arg, strOutputString);
	vfprintf(fp, strOutputString, arg);
	fprintf(fp, "\n");
	fclose(fp);
}

void GameLog::OutputString(const char * strOutputString, ...)
{
#ifdef _RELEASEEX
	//��Ӫ���ֹ����OutputDebugString��������Ϊ�ú�����ʱ����
	return;
#endif // _RELEASEEX
	//����ʱ��
	CTime time = CTime::GetCurrentTime();
	char cTime[30];
	::memset(cTime, 0, sizeof(cTime));
	sprintf(cTime, "\n[%d:%d:%d] ", time.GetHour(), time.GetMinute(), time.GetSecond());
	OutputDebugString(cTime);
	char strBuffer[4096] = { 0 };
	va_list vlArgs;
	va_start(vlArgs, strOutputString);
	_vsnprintf(strBuffer, sizeof(strBuffer) - 1, strOutputString, vlArgs);
	va_end(vlArgs);
	OutputDebugString(strBuffer);
	OutputDebugString("\n");
}