#pragma once

#include <map>
#include <string>

using namespace std;

#define CONFIGLEN           256 

enum INI_RES
{
	INI_SUCCESS,            //�ɹ�
	INI_ERROR,              //��ͨ����
	INI_OPENFILE_ERROR,     //���ļ�ʧ��
	INI_NO_ATTR             //�޶�Ӧ�ļ�ֵ
};

//���캯�� mode �ַ���˵��
//r	��ֻ����ʽ���ļ������ļ�������ڡ�
//r + �Զ� / д��ʽ���ļ������ļ�������ڡ�
//rb + �Զ� / д��ʽ��һ���������ļ���ֻ����� / д���ݡ�
//rt + �Զ� / д��ʽ��һ���ı��ļ����������д��
//w	��ֻд�ļ������ļ��������ļ�������Ϊ�㣬�����ļ����ݻ���ʧ�����ļ��������򴴽����ļ���
//w + �򿪿ɶ� / д�ļ������ļ��������ļ�������Ϊ�㣬�����ļ����ݻ���ʧ�����ļ��������򴴽����ļ���
//a	�Ը��ӵķ�ʽ��ֻд�ļ������ļ������ڣ���ᴴ�����ļ�������ļ����ڣ���д������ݻᱻ�ӵ��ļ�β�󣬼��ļ�ԭ�ȵ����ݻᱻ������EOF ����������
//a + �Ը��ӷ�ʽ�򿪿ɶ� / д���ļ������ļ������ڣ���ᴴ�����ļ�������ļ����ڣ���д������ݻᱻ�ӵ��ļ�β�󣬼��ļ�ԭ�ȵ����ݻᱻ������EOF������������

//�Ӽ����� �Ӽ�ֵ 
typedef map<std::string, std::string> KEYMAP;
//�������� ����ֵ  
typedef map<std::string, KEYMAP> MAINKEYMAP;

// config �ļ��Ļ���������
class CINIFile
{
public:
	// ���캯��
	CINIFile(string fileName, string mode = "r+");
	// ��������
	virtual ~CINIFile();
public:
	// �ж��ļ��Ƿ����
	static bool IsFileExist(string fileName);
	// �����ļ�
	static void CopyFileTo(string destFileName, string srcFileName);
	// ��ȡ��ǰĿ¼
	static string GetAppPath();
	// ��config �ļ�
	INI_RES OpenFile(const char* pathName, const char* mode);
	// �ر�config �ļ�
	INI_RES CloseFile();
	// ��ȡ���εļ�ֵ
	int  GetKeyVal(string secName, string keyName, int default_);
	// ��ȡ�޷������εļ�ֵ
	UINT GetKeyVal(string secName, string keyName, UINT default_);
	// ��ȡ�����εļ�ֵ
	long long GetKeyVal(string secName, string keyName, long long default_);
	// ��ȡdouble�ļ�ֵ
	double  GetKeyVal(string secName, string keyName, double default_);
	// ��ȡ��ֵ���ַ���
	string GetKeyVal(string secName, string keyName, string default_);
	// ���ü�ֵ���ַ���
	void SetKeyValString(string secName, string keyName, string str);

protected:
	// ��дconfig�ļ�
	INI_RES GetKey(string secName, string keyName, string &value);
	void SetKey(string secName, string keyName, string value);
protected:
	// ���򿪵��ļ��ֱ�
	FILE* m_fp;
	string  m_szKey;
	MAINKEYMAP m_Map;
	bool  m_bModify;
};

