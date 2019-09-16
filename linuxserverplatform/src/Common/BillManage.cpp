#include "CommonHead.h"
#include "MysqlHelper.h"
#include "Define.h"
#include "Util.h"
#include "log.h"
#include <stdarg.h>
#include "DataLine.h"
#include "BillManage.h"

CBillManage::CBillManage()
{
}

CBillManage::~CBillManage()
{
}

CBillManage* CBillManage::Instance()
{
	static CBillManage manage;
	return &manage;
}

void CBillManage::Release()
{
}

void CBillManage::WriteBill(CDataBaseManage* m_pDBManage, const char* pFormat, ...)
{
	if (m_pDBManage == NULL)
	{
		ERROR_LOG("���ݾ��m_pDBManage=NULL");
		return;
	}

	char sql[MAX_SQL_STATEMENT_SIZE] = "";

	va_list args;
	va_start(args, pFormat);

	vsprintf(sql, pFormat, args);

	va_end(args);

	InternalSqlStatement msg;
	memcpy(msg.sql, sql, sizeof(sql));

	if (!m_pDBManage->PushLine(&msg.head, sizeof(InternalSqlStatement), DTK_GP_SQL_STATEMENT, 0, 0))
	{
		ERROR_LOG("Ͷ�ݶ���ʧ�ܣ�%s", sql);
	}
}