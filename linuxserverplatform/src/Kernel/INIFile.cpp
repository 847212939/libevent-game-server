#include "CommonHead.h"
#include "INIFile.h"

/******************************************************************************
* ��  �ܣ����캯��
* ��  ������
* ����ֵ����
* ��  ע��
******************************************************************************/
CINIFile::CINIFile(string fileName, string mode/* = "r+"*/)
{
	m_szKey = "";
	m_fp = NULL;
	m_bModify = false;

	OpenFile(fileName.c_str(), mode.c_str());
}

/******************************************************************************
* ��  �ܣ���������
* ��  ������
* ����ֵ����
* ��  ע��
******************************************************************************/
CINIFile::~CINIFile()
{
	CloseFile();
	m_Map.clear();
}

bool CINIFile::IsFileExist(string fileName)
{
	return access(fileName.c_str(), 0);
}

void CINIFile::CopyFileTo(string destFileName, string srcFileName)
{
	bool bb = IsFileExist(srcFileName);
	if (!bb)return;
	string command = "cp ";
	command += srcFileName;
	command += " ";
	command += destFileName;//cp /home/file1 /root/file2
	system((char*)command.c_str());
}

string CINIFile::GetAppPath()
{
	char current_absolute_path[PATH_MAX] = "";

	//��ȡ��ǰ�������·��
	int cnt = readlink("/proc/self/exe", current_absolute_path, PATH_MAX);
	if (cnt < 0 || cnt >= PATH_MAX)
	{
		return "/home/";
	}

	//��ȡ��ǰĿ¼����·������ȥ��������
	for (int i = cnt; i >= 0; --i)
	{
		if (current_absolute_path[i] == '/')
		{
			current_absolute_path[i + 1] = '\0';
			break;
		}
	}

	return current_absolute_path;
}

/******************************************************************************
* ��  �ܣ����ļ�����
* ��  ������
* ����ֵ��
* ��  ע��
******************************************************************************/
INI_RES CINIFile::OpenFile(const char* pathName, const char* mode)
{
	string szLine, szMainKey, szLastMainKey, szSubKey;
	char strLine[CONFIGLEN] = { 0 };
	KEYMAP mLastMap;
	int  nIndexPos = -1;
	int  nLeftPos = -1;
	int  nRightPos = -1;
	m_fp = fopen(pathName, mode);

	if (m_fp == NULL)
	{
		printf("open inifile %s error!\n", pathName);
		return INI_OPENFILE_ERROR;
	}

	m_Map.clear();

	while (fgets(strLine, CONFIGLEN, m_fp))
	{
		szLine.assign(strLine);
		//ɾ���ַ����еķǱ�Ҫ�ַ�
		nLeftPos = szLine.find("\n");
		if (string::npos != nLeftPos)
		{
			szLine.erase(nLeftPos, 1);
		}
		nLeftPos = szLine.find("\r");
		if (string::npos != nLeftPos)
		{
			szLine.erase(nLeftPos, 1);
		}
		//�ж��Ƿ�������
		nLeftPos = szLine.find("[");
		nRightPos = szLine.find("]");
		if (nLeftPos != string::npos && nRightPos != string::npos)
		{
			szLine.erase(nLeftPos, 1);
			nRightPos--;
			szLine.erase(nRightPos, 1);
			szLastMainKey = szLine;
			m_Map[szLastMainKey] = mLastMap;
			mLastMap.clear();

		}
		else
		{
			//�Ƿ����Ӽ�
			if (nIndexPos = szLine.find("="), string::npos != nIndexPos)
			{
				string szSubKey, szSubValue;
				szSubKey = szLine.substr(0, nIndexPos);
				szSubValue = szLine.substr(nIndexPos + 1, szLine.length() - nIndexPos - 1);
				//mLastMap[szSubKey] = szSubValue ;
				MAINKEYMAP::iterator it = m_Map.find(szLastMainKey);
				if (it != m_Map.end())
				{
					it->second[szSubKey] = szSubValue;
				}
			}
			else
			{
				//TODO:������ini��ֵģ������� ��ע�͵�

			}
		}

	}

	return INI_SUCCESS;
}

/******************************************************************************
* ��  �ܣ��ر��ļ�����
* ��  ������
* ����ֵ��
* ��  ע��
******************************************************************************/
INI_RES CINIFile::CloseFile()
{
	if (m_bModify)
	{
		fseek(m_fp, 0, SEEK_SET);
		//����
		for (MAINKEYMAP::iterator it = m_Map.begin(); it != m_Map.end(); ++it)
		{
			string AppName = "[";
			AppName.append(it->first);
			AppName.append("]\r\n");
			fwrite(AppName.c_str(), sizeof(char), AppName.length(), m_fp);
			for (KEYMAP::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
			{
				string KeyPair = it2->first;
				KeyPair.append("=");
				KeyPair.append(it2->second);
				KeyPair.append("\r\n");
				fwrite(KeyPair.c_str(), sizeof(char), KeyPair.length(), m_fp);
			}
		}
	}
	if (m_fp != NULL)
	{
		fclose(m_fp);
		m_fp = NULL;
	}
	return INI_SUCCESS;
}

/******************************************************************************
* ��  �ܣ���ȡ[SECTION]�µ�ĳһ����ֵ���ַ���
* ��  ����
*		string mAttr		�������   	����
*		string cAttr		�������	�Ӽ�
*		string value		�������	�Ӽ���ֵ
* ����ֵ��
* ��  ע��
******************************************************************************/
INI_RES CINIFile::GetKey(string secName, string keyName, string& value)
{
	MAINKEYMAP::iterator it = m_Map.find(secName);
	if (it == m_Map.end())
		return INI_NO_ATTR;
	KEYMAP::iterator it2 = it->second.find(keyName);
	if (it2 == it->second.end())
		return INI_NO_ATTR;

	value = it2->second;

	return INI_SUCCESS;
}

void CINIFile::SetKey(string secName, string keyName, string value)
{
	MAINKEYMAP::iterator it = m_Map.find(secName);
	if (it != m_Map.end())
	{
		it->second[keyName] = value;
	}
	else
	{
		KEYMAP keymp;
		keymp[keyName] = value;
		m_Map[secName] = keymp;
	}
}

int CINIFile::GetKeyVal(string secName, string keyName, int default_)
{
	int nRes = default_;

	m_szKey = "";

	if (INI_SUCCESS == GetKey(secName, keyName, m_szKey))
	{
		nRes = atoi(m_szKey.c_str());
	}
	return nRes;
}

UINT CINIFile::GetKeyVal(string secName, string keyName, UINT default_)
{
	UINT nRes = default_;

	m_szKey = "";

	if (INI_SUCCESS == GetKey(secName, keyName, m_szKey))
	{
		nRes = atoi(m_szKey.c_str());
	}
	return nRes;
}

long long CINIFile::GetKeyVal(string secName, string keyName, long long default_)
{
	long long nRes = default_;

	m_szKey = "";

	if (INI_SUCCESS == GetKey(secName, keyName, m_szKey))
	{
		nRes = atoll(m_szKey.c_str());
	}
	return nRes;
}

double CINIFile::GetKeyVal(string secName, string keyName, double default_)
{
	double nRes = default_;

	m_szKey = "";

	if (INI_SUCCESS == GetKey(secName, keyName, m_szKey))
	{
		nRes = atof(m_szKey.c_str());
	}
	return nRes;
}

string CINIFile::GetKeyVal(string secName, string keyName, string default_)
{
	m_szKey = "";

	if (INI_SUCCESS != GetKey(secName, keyName, m_szKey))
	{
		m_szKey = default_;
	}

	return m_szKey;
}

void CINIFile::SetKeyValString(string secName, string keyName, string str)
{
	SetKey(secName, keyName, str);
	m_bModify = true;
}

