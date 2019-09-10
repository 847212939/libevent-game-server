#include "CommonHead.h"
#include "Exception.h"
#include "Function.h"
#include <exception>

//��̬��������
bool CException::m_bShowError = false;					//�Ƿ���ʾ����

CException::CException(const char * szErrorMessage, unsigned int uErrorCode, bool bAutoDelete)
{
	m_uErrorCode = uErrorCode;
	m_bAutoDelete = bAutoDelete;
	if ((szErrorMessage != NULL) && (szErrorMessage[0] != 0))
	{
		if (strlen(szErrorMessage) < (sizeof(m_szMessage) / sizeof(m_szMessage[0])))
		{
			strcpy(m_szMessage, szErrorMessage);
		}
		else
		{
			strcpy(m_szMessage, "�쳣������Ϣ̫��");
		}
	}
	else
	{
		strcpy(m_szMessage, "û���쳣��Ϣ");
	}

	if (m_bShowError == true)
	{
		printf("%s\n", m_szMessage);
	}
}


CException::~CException()
{
}

//��ȡ�������
UINT CException::GetErrorCode() const
{
	if (this != NULL) return m_uErrorCode;
	return 0;
}

//ɾ������
bool CException::Delete()
{
	if ((this != NULL) && (m_bAutoDelete == true))
	{
		delete this;
		return true;
	}
	return false;
}

//��ȡ������Ϣ
TCHAR const * CException::GetErrorMessage(TCHAR * szBuffer, int iBufLength) const
{
	if (this != NULL)
	{
		//����������Ϣ
		if ((szBuffer != NULL) && (iBufLength > 0))
		{
			int iCopyLength = min(iBufLength - 1, strlen(m_szMessage));
			szBuffer[iCopyLength] = 0;
			memcpy(szBuffer, m_szMessage, iCopyLength * sizeof(TCHAR));
		}
		return m_szMessage;
	}
	return NULL;
}

//�����Ƿ���ʾ����
bool CException::ShowErrorMessage(bool bShowError)
{
	m_bShowError = bShowError;
	return m_bShowError;
}