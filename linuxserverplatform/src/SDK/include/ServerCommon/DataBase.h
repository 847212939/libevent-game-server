#pragma once

class CDataLine;
class CDataBaseManage;

// ���ݿ⴦����
class CDataBaseManage
{
public:
	KernelInfoStruct* m_pKernelInfo;	// �ں�����
	ManageInfoStruct* m_pInitInfo;		// ��ʼ������ָ��
	CDataLine	m_DataLine;				// ���ݶ���
	CMysqlHelper* m_pMysqlHelper;		// ���ݿ�ģ��
protected:
	pthread_t	m_hThread;			// �߳̾��
	//HANDLE	m_hCompletePort;	// ��ɶ˿�
	bool	m_bInit;			// ��ʼ����־
	bool	m_bRun;				// ���б�־
	IDataBaseHandleService* m_pHandleService;	// ���ݴ���ӿ�

public:
	CDataBaseManage();
	virtual ~CDataBaseManage();

public:
	//��ʼ������
	bool Init(ManageInfoStruct* pInitInfo, KernelInfoStruct* pKernelInfo, IDataBaseHandleService* pHandleService, IAsynThreadResultService* pResultService);
	//ȡ����ʼ��
	bool UnInit();
	//��ʼ����
	bool Start();
	//ֹͣ����
	bool Stop();
	//���봦�����
	bool PushLine(DataBaseLineHead* pData, UINT uSize, UINT uHandleKind, UINT uIndex, UINT dwHandleID);

public:
	//�����������
	bool CheckSQLConnect();
	//�������ݿ�
	bool SQLConnectReset();

	int		m_sqlClass;

	ushort	m_nPort;
	bool    m_bsqlInit;

	char	m_host[128];
	char	m_passwd[48];
	char	m_user[48];
	char	m_name[48];

private:
	//���ݿ⴦���߳�
	static void* DataServiceThread(void* pThreadData);
};

///***********************************************************************************************///
//���ݿ⴦��ӿ���
class CDataBaseHandle : public IDataBaseHandleService
{
public:
	CDataBaseHandle();
	virtual ~CDataBaseHandle();

protected:
	KernelInfoStruct* m_pKernelInfo;			//�ں�����
	ManageInfoStruct* m_pInitInfo;				//��ʼ������ָ��
	IAsynThreadResultService* m_pRusultService;	//�������ӿ�
	CDataBaseManage* m_pDataBaseManage;		//���ݿ����

public:
	//���ò���
	virtual bool SetParameter(IAsynThreadResultService* pRusultService, CDataBaseManage* pDataBaseManage, ManageInfoStruct* pInitData, KernelInfoStruct* pKernelData);
};