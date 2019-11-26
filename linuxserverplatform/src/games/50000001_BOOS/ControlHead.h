#pragma once

#include "CMD_Game.h"
#include <afxtempl.h>

#define CreateInterface( Hinstance, InterfacePoint, InterfaceName, CallDllName, CallFunctionName )		\
InterfacePoint = NULL;																					\
Hinstance = LoadLibrary( _T( CallDllName ) );															\
if ( Hinstance )																						\
{																										\
	typedef void * (*CREATE)(); 																		\
	CREATE pFunction = (CREATE)GetProcAddress( Hinstance, CallFunctionName ); 							\
	if ( pFunction )																					\
	{																									\
		InterfacePoint = static_cast< InterfaceName * >( pFunction() );									\
	}																									\
}																										



#define ReleaseInterface(Hinstance, InterfacePoint)	\
if( InterfacePoint )									\
{														\
	InterfacePoint->Release();							\
	InterfacePoint = NULL;								\
}														\
if( Hinstance )											\
{														\
	FreeLibrary(Hinstance);								\
	Hinstance = NULL;									\
}								

// �����Ѷ�
typedef CMap< unsigned long, unsigned long, double, double > CMapPersonalDifficulty;

// �ӿ���Ϣ

// ���ƻص�
interface IClientControlCallback
{
	// ������Ϣ
	virtual bool OnControlInfo( uint nMessageID, void * pData, uint nSize ) = NULL;
};

// �ͻ��˽ӿ�
interface IClientControl
{
	// �ͷŽӿ�
	virtual void Release() = NULL;
	// ��������
	virtual bool Create( CWnd * pParentWnd, IClientControlCallback * pIClientControlCallback ) = NULL;
	// ��ʾ����
	virtual bool ShowWindow( bool bShow ) = NULL;
	// ��Ϣ����
	virtual bool OnControlMessage( uint nMessageID, void * pData, uint nSize ) = NULL;
};

// ���ƻص�
interface IServerControlCallback
{
	// ��������
	virtual void GetCustomRule( tagCustomRule & nConfigInfo ) = NULL;
	// ��������
	virtual void SetCustomRule( tagCustomRule & nConfigInfo, bool bSaveFile ) = NULL;
	// ���ظ����Ѷ�
	virtual void GetPersonalDifficulty( CMapPersonalDifficulty & MapPersonalDifficulty ) = NULL;
	// ���ø����Ѷ�
	virtual void SetPersonalDifficulty( unsigned long lPlayID, double dPersonalDifficulty ) = NULL;
	// ɾ�������Ѷ�
	virtual void DeletePersonalDifficulty( unsigned long lPlayID ) = NULL;
	// ��ո����Ѷ�
	virtual void ClearPersonalDifficulty() = NULL;
	// ��ȡ���
	virtual void GetStock( LONGLONG & lStockInitial, LONGLONG & lStockCurrent ) = NULL;
	// �������ӿ��
	virtual bool SetTableStock( unsigned short TableID, int nTableStock ) = NULL;
	// ������Ϣ
	virtual bool OnControlInfo( void * pIServerUserItem, uint nMessageID, void * pData, uint nSize ) = NULL;
};


// �������ӿ�
interface IServerControl
{
	// �ͷŽӿ�
	virtual void Release() = NULL;
	// ��������
	virtual bool Create( IServerControlCallback * pIServerControlCallback ) = NULL;
	// ��Ϣ����
	virtual bool OnControlMessage( void * pIServerUserItem, uint nMessageID, void * pData, uint nSize ) = NULL;
};


// �ͻ�����Ϣ
#define SUBC_C_GAME_QUERY				1			// ��Ϸ��ѯ
#define SUBC_C_GAME_SET					2			// ��Ϸ����

#define SUBC_C_SET_TABLE				3			// ��������
#define SUBC_C_SET_PERSONAL				4			// �����Ѷ�
#define SUBC_C_DELETE_PERSONAL			5			// ����ɾ��
#define SUBC_C_QUERY_PERSONAL			6			// ��ѯ�Ѷ�
#define SUBC_C_CLEAR_PERSONAL			7			// ����Ѷ�
#define SUBC_C_UPDATA_STOCK				8			// ���¿��

// ������Ϣ
struct CMDC_C_GameSet
{
	// ������Ϣ
	tagCustomRule			nConfigInfo;			// ������Ϣ

	// �Ƿ񱣴�
	bool					bSaveFile;				// �����ļ�
};

// ��������
struct CMDC_C_SetTable
{
	unsigned short			TableID;				// ����ID
	int						nTableStock;			// ���ӿ��
};

// ��������
struct CMDC_C_SetPersonal
{
	unsigned long			lPlayID;				// ����ID
	double					dPersonalDifficulty;	// �����Ѷ�
};

// ����ɾ��
struct CMDC_C_DeletePersonal
{
	unsigned long			lPlayID;				// ����ID
};

// ��������Ϣ
#define SUBC_S_GAME_QUERY				1			// ��Ϸ��ѯ
#define SUBC_S_TEXT						2			// ������ʾ
#define SUBC_S_QUERY_PERSONAL			3			// ��ѯ�Ѷ�
#define SUBC_S_UPDATA_STOCK				4			// ���¿��

// ��Ϸ��ѯ
struct CMDC_S_GameQuery
{
	// ������Ϣ
	tagCustomRule			nConfigInfo;			// ������Ϣ

	// ��ѯ��Ϣ
	enum EnumQuery
	{
		EnumQuery_Query,
		EnumQuery_Set,
		EnumQuery_SetSave,
	};
	EnumQuery				nEnumQuery;				// ��ѯ�ṹ
};

// ������ʾ
struct CMDC_S_Text
{
	// ������Ϣ
	TCHAR					szTextInfo[64];			// ��ʾ��Ϣ
};

// ��ѯ�Ѷ�
struct CMDC_S_QueryPersonal
{
	unsigned long			lPlayID;				// ����ID
	double					dPersonalDifficulty;	// �����Ѷ�
};

// ���¿��
struct CMDC_S_UpdataStock
{
	LONGLONG				lStockInitial;			// ��ʼ���
	LONGLONG				lStockCurrent;			// ��ǰ���
};