#pragma once
#include   <stdarg.h>

//��־��
class GameLog
{
public:
	static void OutputFile(const char *strOutputString, ...);	 //�����־�ļ�
	static void OutputString(const char * strOutputString, ...); //���ַ������������̨
	static void SFile(const char * strOutputString, ...);		 //���ַ���������ļ����˺�����Ӫ��Ҳ�������
private:
};
