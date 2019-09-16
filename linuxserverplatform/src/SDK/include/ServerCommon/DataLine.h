#pragma once

#include "Lock.h"
#include "InternalMessageDefine.h"

/*
Struct		:ListItemData
Memo		:���������ݽṹ
Author		:Fred Huang
Add Data	:2008-3-4
Modify Data	:none
Parameter	:
	stDataHead	:���ݰ�ͷ
	pData		:ÿ�����������ݵ�ָ�룬ʹ����new��ʽ������ڴ棬ע�⣬�ڳ�����ʱ��Ҫ��ʽ��delete ���ڴ�
*/
struct ListItemData
{
	DataLineHead stDataHead;
	BYTE* pData;
};

//���ݶ�����
class CDataLine
{
private:
	std::list <ListItemData*> m_DataList;
	CSignedLock		m_csLock;				//ͬ����

	//HANDLE			m_hCompletionPort
public:
	CDataLine();
	virtual ~CDataLine();

public:
	//��ȡ��
	CSignedLock* GetLock() { return &m_csLock; }
	//������������
	bool CleanLineData();
	//������Ϣ����
	virtual UINT AddData(DataLineHead* pDataInfo, UINT uAddSize, UINT uDataKind, void* pAppendData = NULL, UINT uAppendAddSize = 0);
	//��ȡ��Ϣ����
	virtual UINT GetData(DataLineHead* pDataBuffer, UINT uBufferSize);

public:
	size_t GetDataCount();
};

