#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

#include "..\Dexter\Dexter.h"
// 1�ֽڶ���
#pragma pack(1)

////////////////////////////////////////////////////////////////////////// 
// ������

#define GAMENAME						TEXT("BOSS����")
#define GAME_DLL_AUTHOR                 TEXT("���ڻ�������Ƽ�")            //
#define GAME_DLL_NOTE                   TEXT("���� -- ��Ϸ���")

#define NAME_ID                         50000001
#define PLAY_COUNT                      4

// �汾����
#define GAME_MAX_VER					1								        // ������߰汾
#define GAME_LESS_VER					1								        // ������Ͱ汾
#define GAME_CHANGE_VER					0								        // �޸İ汾

// ֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME

// ��Ϸ��������
#define INVALID_CHAIR				0xFFFF								//��Ч����

////////////////////////////////////////////////////////////////////////// 

//
#define  GAME_STATUS_FREE     0     //����״̬
#define  GAME_STATUS_PLAY     1    //��Ϸ״̬

// ��Ϸ��Ϣ
#define WM_D3D_RENDER				(WM_USER + 300)					// ��Ⱦ��Ϣ
#define WM_D3D_LOAD					(WM_USER + 301)					// ������Ϣ
#define WM_SHOW_CHAT				(WM_USER + 302)					// ������Ϣ
#define WM_BEGIN_HELP				(WM_USER + 303)					// ��ʼ������Ϣ
#define IDM_CATCH_FISH				(WM_USER + 1000)				// ������Ϣ
#define IDM_FIRE					(WM_USER + 1001)				// ������Ϣ
#define IDM_UPDATE_CONTROLS			(WM_USER + 1004)				// ���¿ؼ���Ϣ
#define IDM_SET						(WM_USER + 1005)				// ������Ϣ
#define IDM_FIRE_SWIM_OVER			(WM_USER + 1007)				// �ζ����
#define IDM_TEMP_SET				(WM_USER + 1008)				// ��ʱ������Ϣ
#define IDM_COLOR					(WM_USER + 1009)				// ��ɫ��Ϣ
#define IDM_BEGIN_LASER				(WM_USER + 1013)				// ׼������
#define IDM_LASER					(WM_USER + 1014)				// ����
#define IDM_GOLD_SELETE				(WM_USER + 1018)				// ���ѡ��
#define IDM_SPEECH					(WM_USER + 1020)				// ������Ϣ
#define IDM_MULTIPLE				(WM_USER + 1021)				// ������Ϣ
#define IDM_OPEN_CONTROL			(WM_USER + 1023)				// �򿪿���

#define S_TOP_LEFT					0								// ������λ��
#define S_TOP_CENTER				1								// ������λ��
#define S_TOP_RIGHT					2								// ������λ��
#define S_BOTTOM_LEFT				3								// ������λ��
#define S_BOTTOM_CENTER				4								// ������λ��
#define S_BOTTOM_RIGHT				5								// ������λ��

#define C_TOP_LEFT					0								// ��ͼλ��
#define C_TOP_CENTER				1								// ��ͼλ��
#define C_TOP_RIGHT					2								// ��ͼλ��
#define C_BOTTOM_LEFT				3								// ��ͼλ��
#define C_BOTTOM_CENTER				4								// ��ͼλ��
#define C_BOTTOM_RIGHT				5								// ��ͼλ��

// ��Դ���
#define DEFAULE_WIDTH				(1280)							// �ͻ�����Կ�
#define DEFAULE_HEIGHT				(800)							// �ͻ�����Ը�	
#define OBLIGATE_LENGTH				(300)							// Ԥ�����

#define CAPTION_TOP_SIZE			25								// �����С
#define CAPTION_BOTTOM_SIZE			40								// �����С

// ����
#define  MAX_VOLUME					(3000)

////////////////////////////////////////////////////////////////////////// 
// ʱ��任
#define  SECOND_TO_MILLISECOND(A)	(A*1000)
#define  MINUTE_TO_MILLISECOND(A)	(A*1000*60)

// ���������
#define	RAND_INDEX					2
#define RAND_INITIAL( nValue, nMin, nMax )		{ nValue[0] = nMin; nValue[1] = nMax; }
#define RAND_MIN_MAX( nValue, nMin, nMax )		{ if( (nMin) >= (nMax) ) { nValue = nMin;} else { nValue = (rand()%((nMax) - (nMin))) + (nMin); } }
#define RAND_EQUAL_MIN_MAX( nMin, nMax )		( ( (rand() + GetTickCount()) % (nMax - nMin) ) + nMin )
#define RAND_TRUE_FALSE( True, False )			( ( (rand() + GetTickCount()) % ((True) + (False)) ) < (uint)(True) )

// ���·��
#define BEZIER_POINT_MAX			8

//������ߵ�key
#define BAG_HONG_BAO		"redBag"			// ���
#define BAG_PHONE_CARD1		"phoneBillCard1"	// 1Ԫ���ѿ�
#define BAG_PHONE_CARD5		"phoneBillCard5"	// 5Ԫ���ѿ�
#define BAG_BING_DONG		"skillFrozen"		// ��������
#define BAG_LOCK			"skillLocking"		// ��������

// ��Ϸ���
enum
{
	PlayChair_Max				= 4,
	PlayChair_Invalid			= 0xffff,
	PlayName_Len				= 32,
	QianPao_Bullet				= 1,							// ǧ������
	Multiple_Max				= 10,							// �����
};

// ������С��0-30
enum EnumFishTypeA
{
	FishType_BXiaoHuangYu		= 0,							// С����
	FishType_BXiaoQingYu	    = 1,							// С����
	FishType_BCaiBanYu			= 2,							// �ʰ���							
	FishType_BHaiMa			    = 3,							// ����
	FishType_BBXiaoChouYu	    = 4,							// С����
	FishType_BHongWeiYu		    = 5,							// ��β��
	FishType_BHaiLuo			= 6,							// ����
	FishType_BHaiLuoXie			= 7,							// ����з
	FishType_BShanHuYu		    = 8,							// ɺ����
	FishType_BHongJinYu			= 9,							// �����
	FishType_BLanWeiYu			= 10,							// ��β��
	FishType_BDengLongYu		= 11,							// ������

	FishType_BQiPaoYu			= 12,							// ������
	FishType_BLvQiPaoYu		    = 13,							// ��ɫ������
	FishType_BKuiJiaYu			= 14,							// ������
	FishType_BNianYu			= 15,							// ����
	FishType_BWuGui				= 16,							// �ڹ�
	FishType_BMoGuiYu			= 17,							// ħ����

	FishType_BJianYu			= 18,							// ����
	FishType_BHaiTun		    = 19,							// ����
	FishType_BDianYu			= 20,							// ����
	FishType_BJingYu			= 21,							// ����
	FishType_BShaYu				= 22,							// ����

	FishType_BChuiTouSha		= 23,							// ��ͷ��
	FishType_BJinJuChiSha		= 24,							// �ƽ�׾޳���
	FishType_BJinKuiJiaYu		= 25,							// �ƽ������
	FishType_BJinChuiTouSha	    = 26,							// �ƽ�ͷ��
	FishType_BJinShaYu		    = 27,							// �ƽ���
	FishType_BJinHuSha			= 28,							// �ƽ𻢾�

	FishType_BOSS			    = 29,							//BOSS

	FishType_Hong_Bao		    = 30,							//	��� keyֵ:redBag
	FishType_Hua_Fei1			= 31,							//  ����1Ԫ keyֵ:phoneBillCard1
	FishType_Hua_Fei2			= 32,							//  ����5Ԫ keyֵ:phoneBillCard5


	FishType_ShuiHuZhuan		= 35,							// ˮ䰴�
	FishType_ZhongYiTang		= 36,							// ������
	FishType_BaoZhaFeiBiao		= 37,							// ��ը����
	FishType_BaoXiang			= 38,							// ����
	FishType_General_Max		= 33,							// ��ͨ�����
	FishType_Normal_Max			= 33,							// ���������
	FishType_Max				= 33,							// �������
	FishType_Small_Max			= 11,							// С���������
	FishType_Moderate_Max		= 17,							// ������
	FishType_Moderate_Big_Max	= 22,							// �д�����
	FishType_Big_Max			= 29,							// ��������
	FishType_Invalid			= -1,							// ��Ч��
};
//����
enum EnumFishTypeS
{
	FishTypeS_One			    = 30,							//����

};
//����
enum EnumFishTypeSS
{
	FishTypeSS_One			   = 50,							//����
};
//����
enum EnumFishTypeSSS
{
	FishTypeSSS_One			  = 90,							   //����
};
// ��״̬
enum EnumFishState
{
	FishState_Normal,		// ��ͨ��
	FishState_King,			// ����
	FishState_Killer,		// ɱ����
	FishState_Aquatic,		// ˮ����
};

// ����
enum EnumHelpType
{
	HelpType_Operation,		
	HelpType_Fish,		
	HelpType_Box,		
	HelpType_Max,	
};

////////////////////////////////////////////////////////////////////////// 

// �ӵ�����
enum
{
	BulletTwo	= 0,			// ˫���ӵ�
	BulletThree = 1,			// �����ӵ�
	BulletFour	= 2,			// �����ӵ�
	BulletStyle = 3,			// �ӵ�����
};

// ��ȡ��������
enum EnumScoreType
{
	EST_Cold,					// ���
	EST_Laser,					// ����
	EST_Speed,					// ����
	EST_Gift,					// ����
	EST_Null,					// ��
};

// ��¼��Ϣ
enum EnumRecord
{
	Record_UserID		= 1,		
	Record_ServerID		= 2,			
	Record_MatchID		= 3,			
	Record_MatchNo		= 4,	
	Record_FireCount	= 5,
	Record_UseScore		= 6,	
};


// ������
struct CDoublePoint
{
	double				x;
	double				y;

	CDoublePoint() {  x = 0.0; y = 0.0; }
	CDoublePoint( POINT& Par ) {  x = Par.x; y = Par.y; }
	CDoublePoint( CDoublePoint& Par ) {  x = Par.x; y = Par.y; }
	CDoublePoint( double ParX, double ParY ) { x = ParX; y = ParY; }
	CDoublePoint& operator= (const CDoublePoint& point) { this->x = point.x; this->y = point.y; return *this; }
	void operator+= (CDoublePoint& point) { x += point.x; y += point.y; }
	bool operator!= (CDoublePoint& point) { return (x != point.x || y != point.y); }
	inline void SetPoint( double ParX, double ParY ) { x = ParX; y = ParY; }
};

struct CShortPoint
{
	short				x;
	short				y;

	CShortPoint() {  x = 0; y = 0; }
	CShortPoint( POINT& Par ) {  x = (short)Par.x; y = (short)Par.y; }
	CShortPoint( CShortPoint& Par ) {  x = Par.x; y = Par.y; }
	CShortPoint( short ParX, short ParY ) { x = ParX; y = ParY; }
	void operator+= (CShortPoint& point) { x += point.x; y += point.y; }
	inline void SetPoint( short ParX, short ParY ) { x = ParX; y = ParY; }

};

struct tagBezierPoint
{
	CDoublePoint			BeginPoint;
	CDoublePoint			EndPoint;
	CDoublePoint			KeyOne;
	CDoublePoint			KeyTwo;
	uint					Time;
};

// ������Ҫ��Ϣ
struct tagLoad
{
	void *							pGLDevice;
	void *							pCWnd;
	void *							pHdc;
	void *							pHrc;
	int								nIndex;
};

// RGB ��ɫ
struct COLORRGBM
{
	byte							R;									// ��ɫɫ��
	byte							G;									// ��ɫ����
	byte							B;									// ��ɫ����
};

// HSB ��ɫ
struct COLORHSBM
{
	WORD							H;									// ��ɫɫ��
	double							S;									// ��ɫ����
	double							B;									// ��ɫ����
};

// ��Ϸ����
struct GameScene
{
	byte				cbBackIndex;							// ��������
	LONGLONG			lPlayScore;								// ��һ���
	LONGLONG			lPlayCurScore[PlayChair_Max];				// ��һ���
	LONGLONG			lPlayStartScore[PlayChair_Max];			// ��һ���

	int					nBulletVelocity;						// �ӵ��ٶ�
	int					nBulletCoolingTime;						// �ӵ���ȴ
	int					nFishMultiple[FishType_Max][2];		// �㱶��

	LONGLONG			lBulletConsume[PlayChair_Max];			// �ӵ�����
	long				lPlayFishCount[PlayChair_Max][FishType_Max];	// ��Ҳ�������
	int					nMultipleValue[Multiple_Max];		// ���䱶��
	int					nMultipleIndex[PlayChair_Max];			// ��ǰ����
	bool				bUnlimitedRebound;						// ���޷���
//	tchar				szBrowseUrl[256];						// ��ֵ��ַ
};

////////////////////////////////////////////////////////////////////////// 
// ����������ṹ
enum
{
	SUB_S_SYNCHRONOUS = 101,				// ͬ����Ϣ
	SUB_S_FISH_CREATE,						// �㴴��
	SUB_S_FISH_CATCH,						// ������
	SUB_S_FIRE,								// ����
	SUB_S_EXCHANGE_SCENE,					// ת������
	SUB_S_OVER,								// ����
	SUB_S_DELAY_BEGIN,						// �ӳ�
	SUB_S_DELAY,							// �ӳ�
	SUB_S_BEGIN_LASER,						// ׼������
	SUB_S_LASER,							// ����
	SUB_S_BANK_TAKE,						// ����ȡ��
	SUB_S_SPEECH,							// ������Ϣ
	SUB_S_SYSTEM,							// ϵͳ��Ϣ
	SUB_S_MULTIPLE,							// ������Ϣ
	SUB_S_SUPPLY_TIP,						// ������ʾ
	SUB_S_SUPPLY,							// ������Ϣ
	SUB_S_AWARD_TIP,						// ������ʾ
	SUB_S_CONTROL,							// ������Ϣ
	SUB_S_UPDATE_GAME,						// ������Ϸ
	SUB_S_STAY_FISH,						// ͣ����
	SUB_S_GAME_SENCE,                       //���ͳ���
	SUB_S_GAME_BOSS,						//BOSSˢ��
};

#define  SUB_S_CREATE_FISHS           666    //��������

// ϵͳ��Ϣ
struct CMD_S_System
{
	double					dRoomStock;			// ������
	double					dTableStock;		// ������
	double					dPlayStock;			// ��ҿ��
	double					dDartStock;			// ���п��
	double					dGameDifficulty;	// �����Ѷ�
};

// �㴴��
struct CMD_S_FishCreate
{
	// ������Ϣ
	uint				nFishKey;				// ��ؼ�ֵ
	uint				unCreateTime;			// ����ʱ��
	WORD				wHitChair;				// ��ɱλ��
	byte				nFishType;				// ������
	EnumFishState		nFishState;				// ��״̬

	// ·��ƫ��
	float				fRotateAngle;			// �Ƕ�
	CShortPoint 		PointOffSet;			// ƫ��

	// ��ʼ�Ƕ�
	float				fInitialAngle;			// �Ƕ�

	// ���·��
	int					nBezierCount;			// ·������
	tagBezierPoint		TBezierPoint[BEZIER_POINT_MAX];		// ·����Ϣ
};

// ͬ����Ϣ
struct CMD_S_Synchronous
{						 
	uint				nOffSetTime;			// ƫ��ʱ��	
};

// ������
struct CMD_S_CatchFish
{
	uint				nFishKey;				// ������
	WORD				wChairID;				// ���λ��
	int					nMultipleCount;			// ��������
	LONGLONG			lScoreCount;			// �������
};

// ����
struct CMD_S_Fire
{
	uint				nBulletKey;				// �ӵ��ؼ�ֵ
	int					nBulletScore;			// �ӵ�����
	int					nMultipleIndex;			// ��������
	int					nTrackFishIndex;		// ׷��������
	WORD				wChairID;				// ���λ��
	CShortPoint			ptPos;					// λ��
	LONGLONG			llPlayScore;			// ���ʵʱ����
};


// ׼������
struct CMD_S_BeginLaser
{
	WORD				wChairID;				// ���λ��
	CShortPoint			ptPos;					// λ��
};

// ����
struct CMD_S_Laser
{
	WORD				wChairID;				// ���λ��
	CShortPoint			ptPos;					// λ��
};

// ���⽱��
struct CMD_S_LaserReward
{
	WORD				wChairID;				// ���λ��
	LONGLONG			lScore;					// ��������
};

// ת������
struct CMD_S_ExchangeScene
{
	byte				cbBackIndex;			// ��������
};

// ����
struct CMD_S_Over
{
	WORD				wChairID;				// ���λ��
};

// �ӳ�
struct CMD_S_Delay
{
	int					nDelay;					// �ӳ�
	WORD				wChairID;				// ���λ��
};

// ���в鿴
struct CMD_S_BankTake
{
	WORD				wChairID;				// ���λ��
	LONGLONG			lPlayScore;				// �û�����
};

// ������Ϣ
struct CMD_S_Speech
{
	WORD				wChairID;				// ���λ��
	int					nSpeechIndex;			// ��������
};

// ����ѡ��
struct CMD_S_Multiple
{
	WORD				wChairID;				// ���λ��
	int					nMultipleIndex;			// ��������
};

// ������ʾ
struct CMD_S_SupplyTip
{
	WORD				wChairID;				// ���λ��
};

// ������Ϣ
struct CMD_S_Supply
{
	WORD				wChairID;				// ���λ��
	LONGLONG			lSupplyCount;			// ��������
	EnumScoreType		nSupplyType;			// ��������
};

// ��ʾ��Ϣ
struct CMD_S_AwardTip
{
	WORD				wTableID;				// ����ID
	WORD				wChairID;				// λ��ID
	tchar				szPlayName[32];			// �û���
	byte				nFishType;				// ������
	int					nFishMultiple;			// �㱶��
	LONGLONG			lFishScore;				// �������
	EnumScoreType		nScoreType;				// ��������
};

// �ṹ��Ϣ
struct CMD_S_Control
{
	uint				nMessageID;
	uint				nMessageSize;
	byte				cbMessageInfo[1024];
};

// ������Ϸ
struct CMD_S_UpdateGame
{
	int					nMultipleValue[Multiple_Max];						// ���䱶��
	int					nCatchFishMultiple[FishType_Max][RAND_INDEX];		// ���㱶��
	int					nBulletVelocity;									// �ӵ��ٶ�
	int					nBulletCoolingTime;									// �ӵ���ȴ
};

// ͣ����
struct CMD_S_StayFish
{
	uint				nFishKey;							// ������
	uint				nStayStart;							// ͣ����ʼ
	uint				nStayTime;							// ͣ��ʱ��
};

////////////////////////////////////////////////////////////////////////// 
// �ͻ�������ṹ
enum
{
	SUB_C_CATCH_FISH = 101,							// ������Ϣ
	SUB_C_FIRE,										// ����
	SUB_C_DELAY,									// �ӳ�
	SUB_C_BEGIN_LASER,								// ׼������
	SUB_C_LASER,									// ����
	SUB_C_SPEECH,									// ������Ϣ
	SUB_C_MULTIPLE,									// ������Ϣ
	SUB_C_CONTROL,									// ������Ϣ
	SUB_C_SKILL,									// �ͷż���
	SUB_C_CHANGE_CONNON,							// �л���̨
};

// �����������
#define FishCatch_Max			5

// ������
struct CMD_C_CatchFish
{
	uint				nBulletKey;					// �ӵ��ؼ�ֵ
	uint				nFishKey[FishCatch_Max];	// ������
};

// ����
struct CMD_C_Fire
{
	int					nMultipleIndex;				// ��������
	int					nTrackFishIndex;			// ׷��������
	uint				nBulletKey;					// �ӵ��ؼ�ֵ
	CShortPoint			ptPos;						// λ��
};

// ׼������
struct CMD_C_BeginLaser
{
	CShortPoint			ptPos;						// λ��
};

// ����
struct CMD_C_Laser
{
	CShortPoint			ptBeginPos;					// ��ʼλ��
	CShortPoint			ptEndPos;					// ����λ��
	uint				unLossTime;					// �ѹ�ʱ��
};

// ������Ϣ
struct CMD_C_Speech
{
	int					nSpeechIndex;				// ��������
};

// ������Ϣ
struct CMD_C_Multiple
{
	int					nMultipleIndex;			// ��������
};

// �ṹ��Ϣ
struct CMD_C_Control
{
	uint				nMessageID;
	uint				nMessageSize;
	byte				cbMessageInfo[1024];
};

//����
struct CMD_C_Skill
{
	byte	SkillType; //�������� 2,����,3,����
};

//��̨�л�
struct CMD_C_CANNON
{
	BYTE desksTation;   //��λ��
	BYTE cannonIndex;	//��̨����
	CMD_C_CANNON()
	{
		desksTation = 255;
		cannonIndex = 255;
	}
};
////////////////////////////////////////////////////////////////////////// 

// �Ѷ�����
#define Difficulty_Type		3	
#define Difficulty_Max		10	

enum EnumExplosionConditionType
{
	ExplosionConditionType_Gun,
	ExplosionConditionType_Gold,
};

// �Զ�������
struct tagCustomRule
{
	// ��������
	int						nMultipleValue[Multiple_Max];		// ���䱶��

	// ��������
	int						nTaxRatio;								// ��ˮ����
	int						nInitialStock;							// ��ʼ���
	int						nRoomDifficultyCount[Difficulty_Max];	// �����Ѷ�
	int						nTableDifficultyCount[Difficulty_Max];	// �����Ѷ�
	int						nPlayDifficultyCount[Difficulty_Max];	// �����Ѷ�
	double					dRoomDifficultyValue[Difficulty_Max];	// �����Ѷ�
	double					dTableDifficultyValue[Difficulty_Max];	// �����Ѷ�
	double					dPlayDifficultyValue[Difficulty_Max];	// �����Ѷ�

	// ��������
	int						nCreateCount;							// ��������
	int						nSceneTime;								// ����ά��ʱ��

	// �ӵ�����
	int						nBulletVelocity;						// �ӵ��ٶ�
	int						nBulletCoolingTime;						// �ӵ���ȴ

	// �����
	int						nSupplyCondition[2];					// ��������
	int						nLaserTime;								// ����ʱ��
	int						nLaserChance;							// ���⼸��
	int						nSpeedTime;								// ����ʱ��
	int						nSpeedChance;							// ���ټ���
	int						nGiftScore[5];							// ���ͽ��
	int						nGiftChance[5];							// ���ͼ���
	int						nNullChance;							// ���伸��

	// ������
	int						nCatchFishMultiple[FishType_Max][RAND_INDEX];		// ���㱶��

	// 2.0 ��չ
	int						nDifficultyStart[Difficulty_Type];		// �Ѷ�����

	// 3.0 ��չ
	int						nExplosionProportion;					// ��ը����
	int						nExplosionStart;						// ��ը����
	LONGLONG				lExplosionCondition;					// ��ը����
	EnumExplosionConditionType nExplosionConditionType;				// ��������

	// 4.0 ��չ
	int						nAwardMinMultiple;
	BOOL					nAwardChatBox;

	tagCustomRule()
	{
		DefaultCustomRule();
	}

	// Ĭ������
	void DefaultCustomRule()
	{
		// ���䱶��
		int nTempMultipleValue[Multiple_Max] = { 1, 10, 100, 500, 1000, 5000 };
		CopyMemory( nMultipleValue, nTempMultipleValue, sizeof(nMultipleValue) );

		// ��������
		nInitialStock = 0;
		nTaxRatio = 3;

		// �Ѷȿ���
		nRoomDifficultyCount[0] = 50;
		nRoomDifficultyCount[1] = 100;
		nRoomDifficultyCount[2] = 200;
		nRoomDifficultyCount[3] = 400;
		nRoomDifficultyCount[4] = 800;
		nRoomDifficultyCount[5] = 1200;
		nRoomDifficultyCount[6] = 2000;
		nRoomDifficultyCount[7] = 3000;
		nRoomDifficultyCount[8] = 6000;
		nRoomDifficultyCount[9] = 10000;

		nTableDifficultyCount[0] = 50;
		nTableDifficultyCount[1] = 100;
		nTableDifficultyCount[2] = 200;
		nTableDifficultyCount[3] = 400;
		nTableDifficultyCount[4] = 800;
		nTableDifficultyCount[5] = 1200;
		nTableDifficultyCount[6] = 2000;
		nTableDifficultyCount[7] = 3000;
		nTableDifficultyCount[8] = 6000;
		nTableDifficultyCount[9] = 10000;

		nPlayDifficultyCount[0] = -10000;
		nPlayDifficultyCount[1] = -1000;
		nPlayDifficultyCount[2] = -200;
		nPlayDifficultyCount[3] = -100;
		nPlayDifficultyCount[4] = -50;
		nPlayDifficultyCount[5] = 0;
		nPlayDifficultyCount[6] = 50;
		nPlayDifficultyCount[7] = 200;
		nPlayDifficultyCount[8] = 5000;
		nPlayDifficultyCount[9] = 10000;

		dRoomDifficultyValue[0] = 0.05;
		dRoomDifficultyValue[1] = 0.15;
		dRoomDifficultyValue[2] = 0.25;
		dRoomDifficultyValue[3] = 0.50;
		dRoomDifficultyValue[4] = 0.75;
		dRoomDifficultyValue[5] = 1.00;
		dRoomDifficultyValue[6] = 1.25;
		dRoomDifficultyValue[7] = 1.70;
		dRoomDifficultyValue[8] = 2.50;
		dRoomDifficultyValue[9] = 3.50;

		dTableDifficultyValue[0] = 0.05;
		dTableDifficultyValue[1] = 0.15;
		dTableDifficultyValue[2] = 0.25;
		dTableDifficultyValue[3] = 0.50;
		dTableDifficultyValue[4] = 0.75;
		dTableDifficultyValue[5] = 1.00;
		dTableDifficultyValue[6] = 2.50;
		dTableDifficultyValue[7] = 2.75;
		dTableDifficultyValue[8] = 3.00;
		dTableDifficultyValue[9] = 3.50;

		dPlayDifficultyValue[0] = 2.50;
		dPlayDifficultyValue[1] = 2.15;
		dPlayDifficultyValue[2] = 1.75;
		dPlayDifficultyValue[3] = 1.15;
		dPlayDifficultyValue[4] = 0.80;
		dPlayDifficultyValue[5] = 0.60;
		dPlayDifficultyValue[6] = 0.40;
		dPlayDifficultyValue[7] = 0.20;
		dPlayDifficultyValue[8] = 0.15;
		dPlayDifficultyValue[9] = 0.10;

		// ��������
		nCreateCount = 12;
		nSceneTime = 60 * 8;

		// �ӵ�����
		nBulletVelocity = 15000;					
		nBulletCoolingTime = 150;

		// �����
		RAND_INITIAL(nSupplyCondition, 500, 700);
		nLaserTime = 30;
		nLaserChance = 10;
		nSpeedTime = 60;
		nSpeedChance = 60;
		nGiftScore[0] = 10;
		nGiftScore[1] = 30;
		nGiftScore[2] = 50;
		nGiftScore[3] = 80;
		nGiftScore[4] = 120;
		nGiftChance[0] = 5;
		nGiftChance[1] = 5;
		nGiftChance[2] = 5;
		nGiftChance[3] = 5;
		nGiftChance[4] = 5;
		nNullChance = 5;

		// ������
		int	nTempFishMultiple[FishType_Max][RAND_INDEX] = { { 2, 2 }, { 3, 3 }, { 4, 4 }, { 5, 5 }, { 6, 6 }, { 7, 7 }, { 8, 8 }, { 9, 9 }, { 10, 10 }, { 12, 12 }, { 15, 15 }, { 18, 18 }, { 20, 20 }, { 25, 25 }, { 30, 30 }, { 40, 40 }, { 150, 150 }, { 200, 200 }, { 300, 300 }, { 400, 400 }, { 400, 1000 }, { 20, 20 }, { 20, 20 }, { 20, 20 }, { 0, 0 } };
		CopyMemory(nCatchFishMultiple, nTempFishMultiple, sizeof(nCatchFishMultiple));

		// �Ѷȿ���
		nDifficultyStart[0] = TRUE;
		nDifficultyStart[1] = TRUE;
		nDifficultyStart[2] = FALSE;

		// ��ը��Ϣ
		nExplosionProportion = 50;
		nExplosionStart = 5000;
		lExplosionCondition = 3000;
		nExplosionConditionType = ExplosionConditionType_Gun;

		// �н�����
		nAwardMinMultiple = 50;
		nAwardChatBox = TRUE;
	}
};




// ��ԭ������
#pragma pack()

////////////////////////////////////////////////////////////////////////// 

#endif