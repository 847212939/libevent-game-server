#pragma once

#include "Interface.h"
#include "basemessage.h"
#include "KernelDefine.h"

//�������Ͷ���
#define HD_SOCKET_READ					1							//SOCKET ��ȡ�¼�����			
#define HD_SOCKET_CLOSE					2							//SOCKET �ر��¼�����			
#define HD_ASYN_THREAD_RESULT			3							//�첽�߳̽������
#define HD_TIMER_MESSAGE				4							//��ʱ����Ϣ����
#define HD_PLATFORM_SOCKET_READ			5							//���ķ���ȡ�¼�����

//���ݶ�����Ϣͷ
struct DataLineHead
{
	UINT						uSize;								//���ݴ�С
	UINT						uDataKind;							//��������
};

///�첽�߳̽����Ϣ�ṹ����
struct AsynThreadResultLine
{
	DataLineHead						LineHead;					///����ͷ
	UINT								uHandleResult;				///������
	UINT								uHandleKind;				///��������
	UINT								uHandleID;					///�����ʶ
};

//SOCKET�ر�֪ͨ�ṹ����
struct SocketCloseLine
{
	DataLineHead						LineHead;					//����ͷ
	UINT								uIndex;						//SOCKT ����
	ULONG								uAccessIP;					//SOCKET IP
	UINT								uConnectTime;				//����ʱ��
};

//SOCKET��ȡ֪ͨ�ṹ����
struct SocketReadLine
{
	DataLineHead						LineHead;					//����ͷ
	NetMessageHead						netMessageHead;				//���ݰ�ͷ
	UINT								uHandleSize;				//���ݰ������С
	UINT								uIndex;						//SOCKET ����
	ULONG								uAccessIP;					//SOCKET IP
	UINT								dwHandleID;					//SOCKET ���� ID
};

//��ʱ����Ϣ�ṹ����
struct ServerTimerLine
{
	DataLineHead						LineHead;					//����ͷ
	UINT								uTimerID;					//��ʱ�� ID
};

//���ݿ����ݰ�ͷ�ṹ
struct DataBaseLineHead
{
	DataLineHead					dataLineHead;							///����ͷ
	UINT							uHandleKind;							///��������
	UINT							uIndex;									///��������
	UINT							dwHandleID;								///�����ʶ
};

//////////////////////////////////////////////////////////////////////////
// �ڲ�ͨ�����

// �ڲ�ͨ����֤Э��
const int COMMON_VERIFY_MESSAGE = 20190601;

// dataline ƽ̨����ͷ
struct PlatformDataLineHead
{
	DataLineHead lineHead;
	PlatformMessageHead platformMessageHead;
	int	socketIdx;
};

// ���ķ���֤
struct PlatformCenterServerVerify
{
	int serverType; // ����������   SERVICE_TYPE_LOGON//����    SERVICE_TYPE_LOADER//��Ϸ
	int serverID;	// ��������logonID����Ϸ����roomID

	PlatformCenterServerVerify()
	{
		memset(this, 0, sizeof(PlatformCenterServerVerify));
	}
};

// ��½����֤
struct PlatformLogonServerVerify
{
	int roomID;
	char passwd[128];

	PlatformLogonServerVerify()
	{
		memset(this, 0, sizeof(PlatformCenterServerVerify));
	}
};

///���ӳɹ���Ϣ 
struct MSG_S_ConnectSuccess
{
	BYTE						bMaxVer;							///���°汾����
	BYTE						bLessVer;							///��Ͱ汾����
	BYTE						bReserve[2];						///�����ֶ�
	UINT						iCheckCode;							///���ܺ��У���룬�ɿͻ��˽����ڰ�ͷ�з���

	MSG_S_ConnectSuccess()
	{
		memset(this, 0, sizeof(MSG_S_ConnectSuccess));
	}
};

//////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
#define DTK_GP_CHECK_DB_CONNECTION		487						// ������ݿ�������
#define DTK_GP_SQL_STATEMENT			488						// SQL���
#define DTK_GP_REGISTR_SEND             489                     // send uid to web
//////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
struct InternalSqlStatement
{
	DataBaseLineHead	head;
	char	sql[MAX_SQL_STATEMENT_SIZE];

	InternalSqlStatement()
	{
		memset(this, 0, sizeof(InternalSqlStatement));
	}
};

///////////////////////////////////////////////////////////////////////////////