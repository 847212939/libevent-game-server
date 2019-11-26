#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "GameLogic.h"
#include "PolygonShape.h"
#include <map>
using namespace std;

////////////////////////////////////////////////////////////////////////////////// 

// ����·��
struct tagFishPath
{
	CWHArray< tagBezierPoint * >		ArrayBezierPoint;
};

// ������Ϣ
struct tagFireInfo
{
	int							nMultipleIndex;		// ��������
	double						dBulletInvest;		// �ӵ�����
};

// ������Ϣ
struct tagCatchInfo
{
	UINT						nFishKey;
	int							nMultipleCount;
	LONGLONG					lScoreCount;

	tagCatchInfo()
	{
		nFishKey = 0;
		nMultipleCount = 0;
		lScoreCount = 0;
	}
	tagCatchInfo ( tagCatchInfo & TRight )
	{
		nFishKey = TRight.nFishKey;
		nMultipleCount = TRight.nMultipleCount;
		lScoreCount = TRight.lScoreCount;
	}
	void operator= ( const tagCatchInfo & TRight )
	{
		nFishKey = TRight.nFishKey;
		nMultipleCount = TRight.nMultipleCount;
		lScoreCount = TRight.lScoreCount;
	}
};

// ͣ����Ϣ
struct tagStayInfo
{
	UINT						nStayStart;			// ͣ����ʼ
	UINT						nStayTime;			// ͣ��ʱ��
};
typedef CWHArray< tagStayInfo >	CArrayStay;

// ����ṹ
struct tagFishInfo
{
	// ������Ϣ
	UINT				nFishKey;				// ������
	BYTE				nFishType;				// ������
	int					nFishScore;				// �����
	UINT				wHitChair;				// ��ɱλ��

	// ʱ����Ϣ
	UINT				unCreateTime;			// ����ʱ��
	UINT				unOverTime;				// ����ʱ��

	// ��ɱ��Ϣ
	int					nScoreChance;			// ��������
	double				dCurChance;				// ��ǰ����

	// ��״̬
	EnumFishState		nFishState;				// ��״̬

	// �ظ�����
	bool				bRepeatCreate;			// �ظ�����

	// ·��ƫ��
	float				fRotateAngle;			// �Ƕ�
	CShortPoint 		PointOffSet;			// ƫ��

	// ·����Ϣ
	int					nBezierCount;			// ·������
	tagBezierPoint		TBezierPoint[BEZIER_POINT_MAX];		// ·����Ϣ

	// ��ʼ���Ƕ�
	float				fInitialAngle;			// �Ƕ�

	// ͣ������
	CArrayStay			ArrayStayInfo;			// ͣ����ʼ

	tagFishInfo()
	{
		// ������Ϣ
		nFishKey = 0;
		nFishType = 0;			
		nFishScore = 0;	
		wHitChair = 0;	

		// ʱ����Ϣ
		unCreateTime = 0;
		unOverTime = 0;

		// ��ɱ��Ϣ
		nScoreChance = 0;
		dCurChance = 0.0;

		// ������
		nFishState = FishState_Normal;

		// �ظ�����
		bRepeatCreate = false;

		// ·��ƫ��
		fRotateAngle = 0.f;
		PointOffSet.SetPoint(0, 0);

		// ·����Ϣ
		nBezierCount = 0;
		ZeroMemory(TBezierPoint, sizeof(TBezierPoint));
	}

	// ��ֵ����
	void operator= ( const tagFishInfo & TRight )
	{
		// ������Ϣ
		nFishKey = TRight.nFishKey;
		nFishType = TRight.nFishType;
		nFishScore = TRight.nFishScore;	
		wHitChair = TRight.wHitChair;

		// ʱ����Ϣ
		unCreateTime = TRight.unCreateTime;
		unOverTime = TRight.unOverTime;

		// ��ɱ��Ϣ
		nScoreChance = TRight.nScoreChance;
		dCurChance = TRight.dCurChance;

		// ������
		nFishState = TRight.nFishState;

		// �ظ�����
		bRepeatCreate = TRight.bRepeatCreate;

		// ·��ƫ��
		fRotateAngle = TRight.fRotateAngle;
		PointOffSet = TRight.PointOffSet;

		// ·����Ϣ
		nBezierCount = TRight.nBezierCount;
		CopyMemory(&TBezierPoint, TRight.TBezierPoint, sizeof(TBezierPoint));

		// ͣ������
		ArrayStayInfo.Append( TRight.ArrayStayInfo );
	}
};

// ���ƶ�����
enum EnumFishMoveType
{
	FishMoveType_Next,					// ��һ��
	FishMoveType_Delete,				// �쳣��
	FishMoveType_Normal,				// ������
};

// ��������
enum EnumPlayCatchType
{
	EPCT_Bullet,				// �ӵ�
	EPCT_Laser,					// ����
	EPCT_Killer,				// ɱ����
	EPCT_Explosion,				// �ֲ���ը
	EPCT_FullScreen,			// ȫ����ը
};

// ɱ������
enum EnumKillerType
{
	KillerType_No,			// ��ɱ��
	KillerType_One,			// һ��
	KillerType_All,			// ȫ��
};

// ��Ϸ����
class CTableFrameSink : public CGameDesk
{
	// ��������
public:
	// ���캯��
	CTableFrameSink();
	// ��������
	virtual ~CTableFrameSink();

	// ��Ȩ����
protected:
	bool							m_bLegalOne;
	bool							m_bLagalTwo;
	//ICompilationSink *				m_pICompilationSink;			

	// ��Ϸ����
protected:
	bool							m_bCompetitionRoom;						// ��������
	bool							m_bTimedCompetitionRoom;				// ��ʱ������
	bool							m_bFirstTime;							// ��һ��
	bool							m_bFishSwimming;						// ���ζ�
	UINT							m_nAppearFishCount;						// ��������
	 int						    m_nFishMultiple[FishType_Max][2];		// ��ı���

	// ������Ϣ
protected:
	static int						m_nCreateCount;							// ��������

	// ������Ϣ
protected:
	byte							m_cbBackIndex;							// ��������
	bool							m_bBackExchange;						// ��������
	static int						m_nSceneTime;							// ����ά��ʱ��
	uint							m_nSceneBegin;							// ������ʼʱ��

	// ���䱶��
protected:
	 int						m_nMultipleValue[Multiple_Max];

	// �ӵ�
protected:
	static int						m_nBulletVelocity;						// �ӵ��ٶ�
	static int						m_nBulletCoolingTime;					// �ӵ���ȴ

	// �����Ϣ
protected:
	DWORD 							m_dwPlayID[PlayChair_Max];								// ���ID
	LONGLONG						m_lPlayScore[PlayChair_Max];							// ��һ���
	LONGLONG						m_lPlayStartScore[PlayChair_Max];						// ��һ���
	LONGLONG						m_lplayCutMoney[PlayChair_Max];							// ��ˮ���
	LONGLONG						m_lFireCount[PlayChair_Max];							// ��������
	LONGLONG						m_lBulletConsume[PlayChair_Max];						// �ӵ�����
	DWORD							m_nPlayDownTime[PlayChair_Max];							// �������ʱ��
	DWORD							m_nRobotPlayTime[PlayChair_Max];						// ��������Ϸʱ��
	DWORD							m_nPlayDelay[PlayChair_Max];							// ����ӳ�
	long							m_lPlayFishCount[PlayChair_Max][FishType_Max];			// ��Ҳ�������
	int								m_nMultipleIndex[PlayChair_Max];						// ��ұ���
	map< int, tagFireInfo >			m_ArrayBulletKey[PlayChair_Max];						// �ӵ��ؼ�ֵ

	// �����Ϣ
protected:
	CPoint							m_PointPlay[PlayChair_Max];							// ���λ��

	// ��Ҳ���
protected:
	static int						m_nSupplyCondition[2];					// �����
	int								m_nSupplyValue[PlayChair_Max];			// �ֵ
	int								m_nEnergyValue[PlayChair_Max];			// ����ֵ
	bool							m_bPlaySupply[PlayChair_Max];				// �������
	int								m_nLaserPlayTime[PlayChair_Max];			// ����ʱ��
	int								m_nSpeedPlayTime[PlayChair_Max];			// ����ʱ��

	// ����Ϣ
protected:
	CWHArray< tagFishInfo >			m_ArrayFishInfo;						// ������Ϣ
	DWORD							m_nStartTime;							// ͬ��ʱ��
	DWORD							m_FishKingTime;							//����ˢ����ʼʱ��
	DWORD                           m_FishKingIntervalTime;					//������ˢ�¼��
	bool							m_bFushBoss;							//����ˢ�¹���׶�
	bool							m_bFushSence;							//ˢ��BOSSʱ��������ˢ��
	bool							m_bIsFushBoss;						    //BOSSˢ����
	bool							m_bIsFushBegin;							//ˢ�µ�BOSS��ʧ�ڼ�

	// ����
protected:
	UINT							m_nFishTeamTime;							// �������
	int								m_nFishKingTime;							// ����ʱ��
	int								m_nFishKing[FishType_Small_Max];				// ������Ϣ

	// ��Ϣ����
protected:
	int								m_nAwardMinMultiple;
	BOOL							m_bAwardChatBox;

	// ���������
protected:
	static int						m_nLaserTime;							// ����ʱ��
	static int						m_nLaserChance;							// ���⼸��
	static int						m_nSpeedTime;							// ����ʱ��
	static int						m_nSpeedChance;							// ���ټ���
	static int						m_nGiftScore[5];						// ���ͽ��
	static int						m_nGiftChance[5];						// ���ͼ���
	static int						m_nNullChance;							// ���伸��

	// ��ը����
protected:
	static int						m_nExplosionProportion;					// ��ը����
	static int						m_nExplosionStart;						// ��ը����
	static LONGLONG					m_lExplosionCondition;					// ��ը����
	static EnumExplosionConditionType m_nExplosionConditionType;			// ��������
	static CMap<uint, uint, LONGLONG, LONGLONG> m_MapPlayExplosionCondition;// ��ը����

	// ���
protected:
	static double					m_dTaxRatio;							// ˰�ձ���
	static LONGLONG					m_nRoomWrite;							// ����ʵ��д��
	static double					m_dDartStock;						// ���ڿ��
	static double					m_dWholeTax[Multiple_Max];			// ȫ��˰��
	static double					m_dRoomStock[Multiple_Max];			// ������
	static double *					m_pdTableStock[Multiple_Max];		// ������
	double							m_dPlayStock[PlayChair_Max][Multiple_Max];// ��ҿ��
	
	// �Ѷ�
	static int						m_nDifficultyStart[Difficulty_Type];		// �Ѷ�����
	static int						m_nRoomDifficultyCount[Difficulty_Max];
	static int						m_nTableDifficultyCount[Difficulty_Max];
	static int						m_nPlayDifficultyCount[Difficulty_Max];
	static double					m_dRoomDifficultyValue[Difficulty_Max];
	static double					m_dTableDifficultyValue[Difficulty_Max];
	static double					m_dPlayDifficultyValue[Difficulty_Max];

	// �����Ѷȿ���
    CMapPersonalDifficulty	m_MapPersonalDifficulty;

	// �ܿ��
	static LONGLONG					m_lStockInitial;					// ��ʼ���
	static LONGLONG					m_lStockCurrent;					// ��ǰ���

	// ��·��
protected:
	static CWHArray< tagFishPath * >	m_ArrayFishPathPositive;			

	//��������
	int   m_iAddFinshCount;	   //�����������ӵ�����
	int   m_iFinshLeve;        //��Ⱥ��С�ȼ�
	int   m_iFinshGroup;       //��������
	bool  m_bOpenShoalFish;    //����Ⱥ�Ƿ���
	int   m_iFishShoalLev;     //����Ⱥ�����ȼ�
	int   m_iCreatJiLv;        //�������ʱ��С�㴴������
	int   m_iTickTime;		  //��Ҷ�ò������ᱻ�߳�
	bool  m_IsOpenTick;	      //�Ƿ����������߳����
	int	  m_ipropNum;		  //���ڶ�������������Ա����������
	int	  m_iBingDong;		  //�������ܸ���
	int	  m_iLocking;		  //1Ԫ���ѿ�����
	int	  m_iRate;			  //��ˮ��
	int   m_iRobotTickTime;   // �����˶�ò������ᱻ��

	uint   m_iOptionTime[PLAY_COUNT];//��Ҳ������ʱ��
	// �������
protected:
	CGameLogic						m_GameLogic;							// ��Ϸ�߼�
	static tagCustomRule			m_CustomRule;							// ���ù���

	// ����ӿ�
protected:
	//ITableFrame	*					m_pITableFrame;							// ��ܽӿ�
	//tagGameServiceOption *			m_pGameServiceOption;					// ��������

	// �������
public:
	HINSTANCE						m_hControlInst;
	//IServerControl *				m_pIServerControl;



	// �����ӿ�
public:
	// �ͷŶ���
	virtual VOID Release();
	// �ӿڲ�ѯ
	//virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	// ����ӿ�
public:
	// ��������
	//virtual bool Initialization(IUnknownEx * pIUnknownEx);
	// ��λ����
	virtual VOID RepositionSink();

	// ��ѯ�ӿ�
public:
	// ��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID) { return false; }



	// ��Ϸ�¼�
public:


	/// ��ʼ��Ϸ
	virtual bool InitDeskGameStation();
	//��Ϸ��ʼ
	//virtual bool OnStart();
	//��Ϸ��ʼ
	virtual bool GameBegin(BYTE bBeginFlag);
	//��Ϸ����
	virtual bool GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	//���ͳ���
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	//	// ������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);
	//�����Ϣ������
	virtual bool HandleFrameMessage(BYTE bDeskStation, unsigned int assistID, void* pData, int size, bool bWatchUser = false);
	// �������
	virtual bool UserSitDeskActionNotify(BYTE deskStation);

	// �¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool OnTimer(UINT uTimerID);
	//��Ϸ��Ϣ����
	virtual bool HandleNotifyMessage(BYTE deskStation, unsigned int assistID, void* pData, int size, bool bWatchUser = false);

	//�û�����
	virtual bool  UserNetCut(GameUserInfo* pUser);
	//�û�����
	virtual bool  UserLeftDesk(GameUserInfo* pUser);
	//��Ϸ״̬
	virtual bool IsPlayGame(BYTE bDeskStation);
	//�û�����
	//virtual void  UserBeKicked(BYTE deskStation);
public:
	//��������
	virtual void LoadDynamicConfig();

	// ���ƺ���
public:
	// ��������
	virtual void GetCustomRule( tagCustomRule & nConfigInfo );
	// ��������
	virtual void SetCustomRule( tagCustomRule & nConfigInfo, bool bSaveFile );
	// ���ظ����Ѷ�
	virtual void GetPersonalDifficulty( CMapPersonalDifficulty & MapPersonalDifficulty );
	// ���ø����Ѷ�
	virtual void SetPersonalDifficulty( unsigned long lPlayID, double dPersonalDifficulty );
	// ɾ�������Ѷ�
	virtual void DeletePersonalDifficulty( unsigned long lPlayID );
	// ��ո����Ѷ�
	virtual void ClearPersonalDifficulty();
	//��ȡ�����Ѷ�
	void GetUserDifficulty(unsigned long lPlayID, double& dPersonalDifficulty);
	// ��ȡ���
	
	// �������ӿ��
	virtual bool SetTableStock( unsigned short TableID, int nTableStock );
	//������Ҵ���
	virtual void UserBeKicked(BYTE deskStation);
	// ��������Ϸ,�ж������������Ƿ�ɱ䣨���������ˣ��Լ���ׯ�б���ң�
	virtual bool HundredGameIsInfoChange(BYTE deskStation);

	// ��Ϸ�¼�
protected:
	// �����¼�
	bool OnSubCatchFish(BYTE deskStation, const void * pBuffer, WORD wDataSize);
	// ����
	bool OnSubFire(BYTE deskStation, const void * pBuffer, WORD wDataSize);
	// ׼������
	bool OnSubBeginLaser(BYTE deskStation, const void * pBuffer, WORD wDataSize);
	// ����
	bool OnSubLaser(BYTE deskStation, const void * pBuffer, WORD wDataSize);
	// �ӳ�
	bool OnSubDelay(BYTE deskStation, const void * pBuffer, WORD wDataSize);
	// ����
	bool OnSubSpeech(BYTE deskStation, const void * pBuffer, WORD wDataSize);
	// ����ѡ��
	bool OnSubMultiple(BYTE deskStation, const void * pBuffer, WORD wDataSize);
	// ������Ϣ
	bool OnSubControl(BYTE deskStation, VOID * pData, WORD wDataSize );
	//�����ͷ�
	bool OnSubSkill(BYTE deskStation, VOID * pData, WORD wDataSize);

	void GetStock(LONGLONG & lStockInitial, LONGLONG & lStockCurrent);
	//�л���̨
	bool OnChangeCannon(BYTE deskStation, VOID * pData, WORD wDataSize);

	// ���ܺ���
public:
	//����ҷ�������
	bool SendTableData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize, unsigned int handleCode = 0);
	//����ҷ�������
	bool SendUserItemData(GameUserInfo * pIServerUserItem, WORD wSubCmdID, VOID * pData, WORD wDataSize);

	// ���ͽ�����ʾ
	bool SendAwardTip( WORD wChairID, byte nFishType, int nFishMultiple, LONGLONG lFishScore, EnumScoreType nScoreType );
	// ��ʼ�����
	void InitializationFishpond( uint nBeginTime = 0 );
	// ������
	uint CreateFish( byte cbCount, byte cbFishType = FishType_Max, uint nPathIndex = uint_max, uint unCreateTime = 0, uint unIntervalTime = 1000, float fRotateAngle = 0.f, bool bCanSpecial = true, bool bCanAquatic = true, EnumKillerType nKillerType = KillerType_No, WORD wHitChair = INVALID_CHAIR, CShortPoint & PointOffSet = CShortPoint(0,0), float fInitialAngle = 0.f, bool bRepeatCreate = true );
	// ������
	uint CreateFishEx( byte cbCount, byte cbFishType = FishType_Max, tagBezierPoint TBezierPoint[BEZIER_POINT_MAX] = NULL, int nBezierCount = 0, uint unCreateTime = 0, uint unIntervalTime = 1000, float fRotateAngle = 0.f, bool bCanSpecial = true, bool bCanAquatic = true, EnumKillerType nKillerType = KillerType_No, WORD wHitChair = INVALID_CHAIR, CShortPoint & PointOffSet = CShortPoint(0,0), float fInitialAngle = 0.f, bool bRepeatCreate = true );
	// ������
	uint ResetFish( tagFishInfo & TFishInfo );
	// ���·��
	void RandomPath( byte cbFishType, tagBezierPoint TBezierPoint[BEZIER_POINT_MAX], int & nBezierCount );
	// ��������Ϣ
	void SendFish( tagFishInfo & TFishInfo );
	// ����
	LONGLONG PlayCatchFish( WORD wChairID, int nBulletSplit, uint nFishCatchKey, int nMultipleIndex, EnumPlayCatchType EPlayCatchType );
	// �Ƚ���
	bool ISFishCatchKey( uint nFishKey, CWHArray< uint > & ArrayFishCatchKey );
	// ������
	LONGLONG CatchInFish( WORD wChairID, int nMultipleIndex, EnumPlayCatchType nPlayCatchType, tagFishInfo & TFishInfo, CWHArray< tagCatchInfo > & ArrayCatchFishInfo );
	// ���Ͳ�����Ϣ
	void SendCatchFish( WORD wChairID, CWHArray< tagCatchInfo > & ArrayCatchFishInfo );
	// ��Ϸ�Ѷ�
	double DifficultyGame( WORD wChairID, int nMultipleIndex );
	// ��Ϸ�Ѷ�
	double DifficultyRoom( int nMultipleIndex );
	// ��Ϸ�Ѷ�
	double DifficultyTable( int nMultipleIndex );
	// ��Ϸ�Ѷ�
	double DifficultyPlay( WORD wChairID, int nMultipleIndex );
	// �ڲ�����
private:
	// ��������
	uint SpecialFishMatrix( byte cbFishType, CDoublePoint PointFish );
	// ��Ⱥ����
	uint GroupOfFish( uint nBeginTime = 0 );
	// ԲȦ��
	uint CircleOfFish( byte cbFishType,	int nFishCount, int nPathIndex, uint unCreateTime, uint nRadius, uint unIntervalTime );
	// ��ȡ·��ʱ��
	uint PathTime( tagFishPath* pFishPath );
	// ���㵱ǰ��λ��
	EnumFishMoveType FishMove( CDoublePoint & ptPosition, tagFishInfo & TFish, uint nCustomLossTime = uint_max );
	// �������Ϣ
	void FishFastMove();
	// ��ȡ������
	uint GetFishCount( uint nType );
	// ��ȡ����������
	uint GetFishSpecialCount();
	// �����
	void FishFreeze( uint unLossTime );
	// ��������
	template< typename T > void SwapVariable( T & One, T & Two );
	// �жϻ�����
	bool IsAndroidUser( BYTE wChairID );
	// ��ʼ�����
	void InitializePlayer( WORD wChairID );

public:
	// ��ת��
	CDoublePoint Rotate( CDoublePoint & ptCircle, double dRadian, CDoublePoint & ptSome );
	// ��ת��
	CShortPoint Rotate( CShortPoint & ptCircle, double dRadian, CShortPoint & ptSome );
	// ��֪Բ��,����,�뾶 ��Բ������һ
	CShortPoint RotatePoint( CShortPoint & ptCircle, double dRadian, double dRadius );
	// ����������
	CDoublePoint PointOnCubicBezier( tagBezierPoint* cp, double t );
	// �������
	double DistanceOfPoint( CDoublePoint & PointOne, CDoublePoint & PointTwo );
	// �������
	int RandomArea(int nLen, ...);

public:
	// ʹ�ܹػ���Ȩ ���ػ�
	bool EnableShutdownPrivilege();

	// ��溯��
public:
	// �������
	void PlayerConsume( WORD wChairID, int nMultipleIndex, LONGLONG lConsumeCount ); 
	// �������
	void PlayerIncome( WORD wChairID, int nMultipleIndex, LONGLONG lIncomeCount );
	// ���ӿ��
	double & _TableStock( int nMultipleIndex );	
	// ���ӿ��
	double & _TableStock( unsigned short TableID, int nMultipleIndex );	
	// ��ҿ��
	double & _PlayerStock( WORD wChairID, int nMultipleIndex );

	//����
	//��ȡ�����Ϣ
	GameUserInfo * GetTableUserItem(WORD wChairID);
	//��ӡ��־�ļ�-������
	void DebugPrintf(const char *p, ...);
	//��ʼ����Ϸ����
	void IniConfig();

	//��������
	bool IsBegin;              //��Ϸ�Ƿ�ʼ��
};

////////////////////////////////////////////////////////////////////////////////// 

#endif