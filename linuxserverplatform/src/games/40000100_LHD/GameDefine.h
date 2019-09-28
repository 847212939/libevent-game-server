#pragma once

#include <list>

#define SYSTEMMONEY 88888888

// ������Ϸ�е�һЩ����
const int AREA_CARD_COUNT = 1;					// ÿ��λ���Ƶ�����
const int MAX_AREA_COUNT = 3;					// ������
const int MAX_SHANGZHUANG_COUNT = 5;			// ��ׯ�б��������
const int MAX_NOTE_SORT = 6;					// ����������
const int MAX_AREA_TREND_COUNT = 20;			// ���������������
const int MAX_GOLD_ROOM_COUNT = 5;				// ��ҳ��������
const int MAX_DESK_USER = 6;					// �����λ��
const int MAX_CHOU_MA_COUNT = 100;				// ���ĳ�������

// ��Ϸ���ýṹ
struct GameConfig
{
	BYTE randBankerCount;
	BYTE randCount;						//�����ׯ����
	BYTE CanSitCount;					//��������������¶�����
	BYTE GamePlayCount_1;			//������Ϸ�������жϻ�������������
	BYTE GamePlayCount_2;			//������Ϸ�������жϻ�������������
	BYTE GamePlayCount_3;			//������Ϸ�������жϻ�������������

	int waitBeginTime;					// �ȴ���ʼʱ��
	int noteKeepTime;					// ��ע�׶γ���ʱ��
	int sendCardKeepTime;				// ���ƽ׶γ���ʱ��
	int compareKeepTime;				// ���ƽ׶γ���ʱ��
	int waitSettleTime;					// �ȴ�����ʱ��
	int maxZhuangGameCount;				// ׯ�������еľ���
	int taxRate;						// ��ˮ
	int maxDeskCount;					// �����λ��
	long long shangZhuangLimitMoney;
	long long sitLimitMoney;			// ����������Ҫ���
	long long noteList[MAX_GOLD_ROOM_COUNT][MAX_NOTE_SORT];
	long long allUserMaxNote;			// �������(����������)���ܹ�����ע����

	GameConfig()
	{
		randBankerCount = 1;
		randCount = 1;
		CanSitCount = 4;
		waitBeginTime = 5;
		noteKeepTime = 15;
		sendCardKeepTime = 2;
		compareKeepTime = 8;
		waitSettleTime = 3;
		maxZhuangGameCount = 5;
		taxRate = 5;
		maxDeskCount = 0;

		shangZhuangLimitMoney = 100000;
		sitLimitMoney = 5000;

		noteList[0][0] = 100;
		noteList[0][1] = 200;
		noteList[0][2] = 500;
		noteList[0][3] = 1000;
		noteList[0][4] = 2000;
		noteList[0][5] = 4000;

		noteList[1][0] = 1000;
		noteList[1][1] = 5000;
		noteList[1][2] = 10000;
		noteList[1][3] = 20000;
		noteList[1][4] = 50000;
		noteList[1][5] = 100000;

		noteList[2][0] = 5000;
		noteList[2][1] = 10000;
		noteList[2][2] = 100000;
		noteList[2][3] = 500000;
		noteList[2][4] = 1000000;
		noteList[2][5] = 2000000;
		allUserMaxNote = 0;
	}
};

// �����Ϣ
struct DTUserInfo
{
	long userID;								// ���ID
	bool isNote;								// ����Ƿ���ע
	bool isContinueNote;						// �Ƿ���ѹ
	long long noteList[MAX_AREA_COUNT];			// ��¼��ǰѹע
	long long lastNoteList[MAX_AREA_COUNT];		// ��¼�Ͼ�ѹע, ������ѹ

	DTUserInfo()
	{
		Clear();
	}

	void ClearNoteInfo()
	{
		isContinueNote = false;
		isNote = false;
		memset(noteList, 0, sizeof(noteList));
	}

	void Clear()
	{
		memset(this, 0, sizeof(DTUserInfo));
	}
};

// ׯ����Ϣ
struct DealerInfo
{
	bool isSystem;					// �Ƿ�ϵͳ��ׯ	
	long userID;					// �û�ID
	long long shangzhuangMoney;		// ��ׯЯ�������

	DealerInfo()
	{
		memset(this, 0, sizeof(DealerInfo));
	}

	void Clear()
	{
		memset(this, 0, sizeof(DealerInfo));
	}
};

#pragma pack(1)
// ����
struct TrendData
{
	int type;			// ���ͣ�-1��Ĭ�ϣ�0������1������2����
	BYTE count;			// ����
	TrendData()
	{
		type = -1;
		count = 0;
	}
	TrendData(int type, BYTE count)
	{
		this->type = type;
		this->count = count;
	}
};
#pragma pack()

// ������Ϣ
struct AreaInfo
{
	long long areaNote;				//������ע��
	long long areaRealPeopleNote;	//������עֵ
	BYTE cardList[AREA_CARD_COUNT];
	int currWinRate;				// ��ǰ��õı��ʣ�������Ϊ����
	
	AreaInfo()
	{
		Clear();
	}

	void ClearInfo()
	{
		areaNote = 0;
		areaRealPeopleNote = 0;
		memset(cardList, 255, sizeof(cardList));
		currWinRate = 0;
	}

	void Clear()
	{
		areaNote = 0;
		areaRealPeopleNote = 0;
		memset(cardList, 255, sizeof(cardList));
		currWinRate = 0;
	}
};

// ��Ӯ�������
struct CtrlSystemParam
{
	int winCount;
	int allCount;
	CtrlSystemParam()
	{
		winCount = 5;
		allCount = 10;
	}
};

// ��Ϸ״̬
enum GameStatus
{
	GAME_STATUS_WAITNEXT = 1,		// �ȴ��¾ֿ�ʼ
	GAME_STATUS_NOTE,				// ��ע�׶�
	GAME_STATUS_SENDCARD,			// ���ƽ׶�
	GAME_STATUS_COMPARE,			// ���ƽ׶�
	GAME_STATUS_SETTLE,				// ����׶�
};

// ��ʱ��
enum GameTimer
{
	GAME_TIMER_DEFAULT = 40,
	GAME_TIMER_WAITNEXT,
	GAME_TIMER_NOTE,				// ��ע�׶ζ�ʱ��
	GAME_TIMER_NOTEINFO,			// ������ע��Ϣ��ʱ��
	GAME_TIMER_SENDCARD,			// ���ƶ�ʱ��
	GAME_TIMER_COMPARE,				// ���ƽ׶ζ�ʱ��
	GAME_TIMER_SETTLE,				// ����׶ζ�ʱ��
	GAME_TIMER_END,
};

#define SYSTEM_DEALER_DESKSTATION	254		// ϵͳ��ׯ