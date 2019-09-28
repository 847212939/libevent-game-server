#pragma once
#include "../Server/MJ/mj.h"

/*------------------------------------------------------------------------------------------------------*/
//֧������
///֧�����Ͷ���
#define SUP_NORMAL_GAME			0x01								///��ͨ��Ϸ
#define SUP_MATCH_GAME			0x02								///������Ϸ
#define SUP_MONEY_GAME			0x04								///�����Ϸ
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
/*------------------------------------------------------------------------------------------------------*/
#define MAIN_VERSION					710		// ���汾��,?��?��
#define ASS_VERSION						15		// �����汾��,?��
/*------------------------------------------------------------------------------------------------------*/
//�ļ����ֶ���
#define GAMENAME						TEXT("ɽ���Ƶ���")
#define NAME_ID						    23510004  		// ���� ID

/*------------------------------------------------------------------------------------------------------*/
static TCHAR szTempStr[MAX_PATH] =		{0};/** �ݴ��ַ�����������ȡ dll_name �ȵ�ʱ��ʹ�� */
#define GET_CLIENT_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".ico"))
#define GET_SERVER_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".dll"))
#define GET_SKIN_FOLDER(A)				(sprintf(A,"%d",NAME_ID),strcat(A,""))
/*------------------------------------------------------------------------------------------------------*/
#define GET_STRING(NUM)                 #NUM
#define INT_TO_STR(NUM)                 GET_STRING(NUM)
#define SKIN_FOLDER                     TEXT(INT_TO_STR(NAME_ID))          // ������Ŀ¼
/*------------------------------------------------------------------------------------------------------*/
// �汾����
#define GAME_MAX_VER					2									// ������߰汾
#define GAME_LESS_VER					2									// ������Ͱ汾
#define GAME_CHANGE_VER					0									// �޸İ汾		
#define PLAY_COUNT						4									// ��Ϸ����	
// ��Ϸ״̬����
#define GS_STATUS_FREE					0				//����״̬
#define GS_STATUS_PLAYING				1				//��Ϸ��״̬
///�����Ƶ�����
#define HAND_CARD_NUM			14				//�����������
#define OUT_CARD_NUM			40				//��������
#define MAX_REMAIN_MEN_PAI_NUM  92              //���ʣ����������
#define HUA_CARD_NUM			8				//��������
#define MEN_CARD_NUM			34				//��ǽ����(��������ǰ���17����)
#define MAX_HUPAI_TYPE			4				//���������������
#define MAX_CANHU_CARD_NUM      13              //һ��������ܺ���������
#define MAX_HU_FEN_KIND         2				//��������
#define MAX_MJ_PAI				38				//�����ֵ
/***********************���ʹ��******************/
#define MAX_COUNT_GANG_FEN       13              //�ֲܷ���������б���
#define MAX_COUNT_HORSE_FEN      8               //��������ɵ�����б���������2���ʱ��������8��
#define MAX_COUNT_GEN_ZHUANG_FEN 1               //����ׯ����
/***********************************/

//�齫��������
#define MJTABLE_CARDTYPE0				0									//����ʽ1
#define MJTABLE_CARDTYPE1				1									//����ʽ2
#pragma pack(1)
//����������ṹ
#define S_C_GAME_START			100									//��Ϸ��ʼ
#define S_C_OUT_CARD			101									//��������
#define S_C_SEND_CARD			102									//�����˿�
#define S_C_OPERATE_NOTIFY		103									//������ʾ
#define S_C_OPERATE_RESULT		104									//��������
#define S_C_GAME_END			105									//��Ϸ����
#define S_C_TRUSTEE				106									//�û��й�
#define S_C_GAME_END_ALL		107								    //�ܽ���
#define S_C_QIHU_NOTIFY		    108								    //����֪ͨ
#define S_C_OUT_CARD_RESULT		109								    //���ƽ��


/////////////////////////////////�����������淨�궨��////////////////////////
//�����淨�������RULE_��ͷ
#define RULE_HAVE_TING_PAI_NOTIFY //������ʾ 
#define RULE_HAVE_HU_PAI_NOTIFY   //������ʾ
#define DELAY_CHECK_ACTION   //��ʱ��⶯��
/////////////////////////////////////////////////////////////////////////////


//�齫����
enum MJ_ACTION
{
	ACTION_NO=0,//û�ж���
	ACTION_CHI,//���ƶ���
	ACTION_CHI_FRONT,//��ͷ
	ACTION_CHI_MID,//����
	ACTION_CHI_BACK,//��β
	ACTION_PENG,//���ƶ���
	ACTION_KAN,//�����ƶ���
	ACTION_GANG,//��
	ACTION_AN_GANG,//���ܶ���
	ACTION_BU_GANG,//���ܶ���
	ACTION_MING_GANG,//���ܶ���
	ACTION_CHI_TNG,//���ԣ��Ժ����ƣ�
	ACTION_TING_PAI,//������
	ACTION_HU,//������
	ACTION_PASS,//��
};

//�齫���λ��
enum MJ_STATION
{
	MJ_STATION_NO = 0,//������
	MJ_STATION_BEN_JIA = 1,//����
	MJ_STATION_XIA_JIA = 2,//�¼�
	MJ_STATION_DUI_JIA = 4,//�Լ�
	MJ_STATION_SHANG_JIA = 8,//�ϼ�
	MJ_STATION_SHANG_XIA_JIA = MJ_STATION_SHANG_JIA | MJ_STATION_XIA_JIA,//�ϼҺ��¼�
	MJ_STATION_SHANG_DUI_JIA = MJ_STATION_SHANG_JIA | MJ_STATION_DUI_JIA,//�ϼҺͶԼ�
	MJ_STATION_DUI_XIA_JIA = MJ_STATION_DUI_JIA | MJ_STATION_XIA_JIA,//�ԼҺ��¼�
	MJ_STATION_ALL_JIA = MJ_STATION_SHANG_JIA | MJ_STATION_XIA_JIA | MJ_STATION_DUI_JIA,//���ң��ϼң��ԼҺ��¼ң���3���齫�н�����2�ң�2���齫����1��
};

//�齫��ָ�����
//���ݲ�ͬ���齫�����в�ͬ�Ľз�
enum MJ_GANG_FEN_TYPE
{
	MJ_GANG_FEN_TYPE_NO = 0,//������
	MJ_GANG_FEN_TYPE_AN_GANG ,//����
	MJ_GANG_FEN_TYPE_MING_GANG,//����
	MJ_GANG_FEN_TYPE_BU_GANG,//����(��)
	MJ_GANG_FEN_TYPE_BEI_AN_GANG,//������
	MJ_GANG_FEN_TYPE_FANG_GANG,//���
	MJ_GANG_FEN_TYPE_BEI_BU_GANG,//�����ܣ����� ----��������
	MJ_GANG_FEN_TYPE_BEI_MING_GANG,//������
};

//�齫��������
//��ͬ�ĵط��齫�����в�ͬ�Ľз�
enum MJ_HU_FEN_TYPE
{
	MJ_HU_FEN_TYPE_NO = 0,//������
	MJ_HU_FEN_TYPE_ZI_MO,//����
	MJ_HU_FEN_TYPE_BEI_ZI_MO,//������
	MJ_HU_FEN_TYPE_DIAN_PAO,//���ڣ����ڣ�
	MJ_HU_FEN_TYPE_DIAN_PAO_HU,//�Ժ������ں���
};

//��������
enum HUPAI_TYPE_MJ//�齫��������ö�ٽṹ
{
	MJ_HUPAI_TYPE_None					=255,	//Ĭ�����ֵ	
	MJ_HUPAI_TYPE_PingHu				=3,		//ƽ��
	MJ_HUPAI_TYPE_QiangGang				=46,	//���ܺ�
	MJ_HUPAI_TYPE_QiDui					=58,	//��С��
	MJ_HUPAI_TYPE_QingYiSe				=61,	//��һɫ
	MJ_HUPAI_TYPE_ShiSanYao				=84,	//ʮ����
	MJ_HUPAI_TYPE_LongQiDui				=88,	//�����߶�
	MJ_HUPAI_TYPE_YI_TIAO_LONG			=89,	//һ����
};

//���ּ���
struct CountHuFenStruct
{
	BYTE byNameType[3];					//��Ӧ�����֣� ö������MJ_HU_FEN_TYPE
	BYTE byXiangYingStation[3];			//��Ӧ��ң�ö������MJ_STATION
	BYTE byXiangYingStationEx[3];		//��Ӧ��ң�ö������MJ_STATION
	int iAllFen[3];						//��Ӯ�ĺ���
	BYTE byNums;
	CountHuFenStruct()
	{
		Init();
	}
	void Init()
	{
		memset(byNameType,MJ_HU_FEN_TYPE_NO,sizeof(byNameType));
		memset(byXiangYingStation,MJ_STATION_NO,sizeof(byXiangYingStation));
		memset(byXiangYingStationEx,MJ_STATION_NO,sizeof(byXiangYingStationEx));
		memset(iAllFen,0,sizeof(iAllFen));
		byNums = 0;
	}
};

//�ֽܷ���
struct CountGangFenStruct
{
	BYTE byGangType[MAX_COUNT_GANG_FEN];		//������ ��ö������MJ_GANG_FEN_TYPE
	BYTE byXiangYingStation[MAX_COUNT_GANG_FEN];//��Ӧ��ң�ö������MJ_STATION
	bool bBaoTing[MAX_COUNT_GANG_FEN];			//������Ƿ���(��Ӧ����Ƿ���),true������
	BYTE byGangNums[MAX_COUNT_GANG_FEN];		//��ͬ�ܵ���������ָ��������ͬ����Ӧ���λ����ͬ
	int iAllFen[MAX_COUNT_GANG_FEN];//�ܷ�
	BYTE byNums;
	CountGangFenStruct()
	{
		Init();
	}
	void Init()
	{
		memset(byGangType,MJ_GANG_FEN_TYPE_NO,sizeof(byGangType));
		memset(byXiangYingStation,MJ_STATION_NO,sizeof(byXiangYingStation));
		memset(byGangNums,0,sizeof(byGangNums));
		memset(iAllFen,0,sizeof(iAllFen));
		byNums = 0;
	}
};

///�����������ݽṹ
struct TCPGStruct
{
	BYTE  byType;    //����������
	BYTE  iStation;  //���������λ��
	BYTE  iBeStation;//�����������λ��
	BYTE  iOutpai;   //���˳�����
	BYTE  byData[4]; //������������
	TCPGStruct()
	{
		Init();
	}
	void Init()
	{
		memset(byData,255,sizeof(byData));
		byType = ACTION_NO;    //����������
		iOutpai = 255;   //���˳�����
		iStation = 255;  //���������λ��
		iBeStation = 255;//�����������λ��
	};
};

//�˴����崴������Ĺ�����Ϸ����
struct tagStructGameRuler
{
	bool bDaHu;                  //�Ƿ�����bDaHu=true�Ǵ���淨��bDaHu=false��ƽ���淨
	bool bBaoTing;               //����
	int  iZiMoScore;			 //��������
	bool bZhiZiMo;               //ֻ����
	bool bFenPai;                //�Ƿ������
	bool bAlreadSet;			 //�����������
	tagStructGameRuler()
	{
		Init();
	}
	void Init()
	{
		bZhiZiMo = false;
		bDaHu = true;
		bFenPai = false;
		bBaoTing = true;
		iZiMoScore = 3;
		bAlreadSet = 0;
	}
};

//����״̬
struct CMD_S_StatusFree
{
	bool bAgree[PLAY_COUNT];//׼�����
	BYTE byOutTime;			//����ʱ��		
	BYTE byBlockTime;		//����˼��ʱ��
	BYTE byMJCardType;		//�齫����ʽ
	int iPlayingCount;		//���о���
	tagStructGameRuler tGameRuler;		//��Ϸ����
	bool bSuperFlag;		//�������
	CMD_S_StatusFree()
	{
		Init();
	}
	void Init()
	{
		bSuperFlag = false;
		byOutTime=15;
		byBlockTime=15;
		iPlayingCount=0;
		memset(bAgree,false,sizeof(bAgree));
		byMJCardType=MJTABLE_CARDTYPE0;
	}
};

//��Ϸ��״̬
struct CMD_S_StatusPlay
{
	bool				bTing[PLAY_COUNT];	//����
	//bool				bTinging;			//�Ƿ����ڽ������Ʋ���
	bool				bTuoGuan[PLAY_COUNT];//�й�
	BYTE				byOutTime;			//����ʱ��		
	BYTE				byBlockTime;		//����˼��ʱ��	
	//������Ϣ
	BYTE				byArHandPai[PLAY_COUNT][HAND_CARD_NUM];//���Ʋ�����14�� 	 
	BYTE				byArHandPaiCount[PLAY_COUNT];//��������	 
	//������Ϣ
	BYTE				byArOutPai[PLAY_COUNT][OUT_CARD_NUM];//���Ʋ�����40��
	BYTE				byArOutPaiCount[PLAY_COUNT];//���Ƶ�����
	BYTE				byMJCardType;//�齫����ʽ
	//�������
	BYTE				byNowOutStation;//����λ��
	//BYTE				byTingCanOut[HAND_CARD_NUM];//�����ܴ����
	BYTE				byNtStation;
	BYTE				iMenNums;//��������
	int					iPlayingCount;//���о���
	//��������Ϣ
	TCPGStruct			UserGCPData[PLAY_COUNT][5];//������
	tagStructGameRuler  tGameRuler;		//��Ϸ����
	bool                bQiHu;          //�Ƿ�����������״̬
	BYTE				byTingOutCardIndex[PLAY_COUNT];//�����б��У����Ƶ��۵�������
	bool				bSuperFlag;	//�������
#ifdef RULE_HAVE_TING_PAI_NOTIFY //�Ƿ���������ʾ
	//������Ϣ
	BYTE                byOutCanHuCard[HAND_CARD_NUM];  //����ǰ��������������
	BYTE				byHuCard[HAND_CARD_NUM][MAX_CANHU_CARD_NUM];    //����ǰ����Ӧ�����ܺ�����
	BYTE				byCardRemainNum[MAX_MJ_PAI];		//���������ʾʣ��������
#endif
#ifdef RULE_HAVE_HU_PAI_NOTIFY
	BYTE                byOutAfterHuCard[MAX_CANHU_CARD_NUM];   //���ƺ󣬺�������
#endif
	CMD_S_StatusPlay()
	{
		Init();
	}
	void Init()
	{
		bSuperFlag = false;
		memset(bTing,false,sizeof(bTing));
		//bTinging=false;
		byOutTime=20;
		memset(byArHandPai,255,sizeof(byArHandPai));
		memset(byArHandPaiCount,0,sizeof(byArHandPaiCount));
		memset(byArOutPai,255,sizeof(byArOutPai));
		memset(byArOutPaiCount,0,sizeof(byArOutPaiCount));
		byNowOutStation=255;
		iMenNums=0;
		//memset(byTingCanOut,255,sizeof(byTingCanOut));
		memset(bTuoGuan,false,sizeof(bTuoGuan));
		iPlayingCount=0;
		byNtStation=255;
		byMJCardType=MJTABLE_CARDTYPE0;
		//iBaseFen=1;
		bQiHu = false;
		memset(byTingOutCardIndex,255,sizeof(byTingOutCardIndex));
#ifdef RULE_HAVE_TING_PAI_NOTIFY
		memset(byOutCanHuCard,255,sizeof(byOutCanHuCard));
		memset(byHuCard,255,sizeof(byHuCard));
		memset(byCardRemainNum,0,sizeof(byCardRemainNum));
#endif
#ifdef RULE_HAVE_HU_PAI_NOTIFY
		memset(byOutAfterHuCard,255,sizeof(byOutAfterHuCard));
#endif
	}
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	BYTE							byNtStation;								//ׯ��
	BYTE							byCard[PLAY_COUNT][HAND_CARD_NUM];			//������
	BYTE							byCardCount[PLAY_COUNT];					//������
	BYTE							bySezi0;									//ɫ��0����
	BYTE							bySezi1;									//ɫ��1����
	BYTE							byGetPaiDir;								//�������Ʒ���
	BYTE							byGetPai;									//���ƶ���
	BYTE							byMJCardType;	                            //�齫����ʽ
	BYTE                            byNtCard;                                   //ׯ������ץ������
	BYTE							iMenNums;									//��������
	int								iPlayingCount;								//����
	bool							bSuperFlag[PLAY_COUNT];						//�������
#ifdef RULE_HAVE_TING_PAI_NOTIFY
	BYTE                            byOutCanHuCard[HAND_CARD_NUM];  //������������
	BYTE							byHuCard[HAND_CARD_NUM][MAX_CANHU_CARD_NUM];    //��Ӧ�����ܺ�����   
	BYTE							byCardRemainNum[MAX_MJ_PAI];		//���������ʾʣ��������
#endif
	CMD_S_GameStart()
	{
		memset(bSuperFlag,false,sizeof(bSuperFlag));
		byNtStation=255;
		memset(byCard,255,sizeof(byCard));
		memset(byCardCount,0,sizeof(byCardCount));
#ifdef RULE_HAVE_TING_PAI_NOTIFY
		memset(byOutCanHuCard,255,sizeof(byOutCanHuCard));
		memset(byHuCard,255,sizeof(byHuCard));
		memset(byCardRemainNum,0,sizeof(byCardRemainNum));
#endif
		iMenNums=0;
		iPlayingCount=0;
		bySezi0=255;
		bySezi1=255;
		byGetPaiDir=255;
		byGetPai=255;
		byMJCardType=MJTABLE_CARDTYPE0;
		/*byBaoGen = 255;
		byBaoCard=255;
		iGameBase = 1;*/
	}
};

//����
struct CMD_S_OutCard
{
	bool							bTing;
	BYTE							byOutCardUser;						//�����û�
	BYTE							byOutCardData;						//�����˿�	
	BYTE                            byCardData[HAND_CARD_NUM];          //������
	BYTE                            byCardCount;                        //����Ŀ
#ifdef RULE_HAVE_HU_PAI_NOTIFY //�Ƿ�Ҫ��������ʾ
	BYTE							byHuCard[MAX_CANHU_CARD_NUM];	    //�ܺ�����
#endif
	BYTE							byOutCardDataIndex;                 //��������
	CMD_S_OutCard()
	{
		bTing=false;
		byOutCardUser=255;
		byOutCardData=255;
		memset(byCardData,255,sizeof(byCardData));
#ifdef RULE_HAVE_HU_PAI_NOTIFY //�Ƿ�Ҫ��������ʾ
		memset(byHuCard,255,sizeof(byHuCard));
#endif
		byCardCount=0;
		byOutCardDataIndex=255;
	}
};

//���ƽ��
struct CMD_S_OutCard_Result
{
	bool							bResult;        //���ƽ�����ɹ�true��ʧ��false
	//�����룬1������Ϸ״̬��2:���ǳ�����ң�3��û���������ƣ�4�����Ѿ����꣬5���Ѿ����ƣ�6���г����ܺ��������ܳ���
	BYTE							byErrorCode;	
	CMD_S_OutCard_Result()
	{
		bResult = false;
		byErrorCode = 0;
	}
};

//ץ��
struct CMD_S_SendCard
{
	BYTE							byCardData;							//�˿�����
	BYTE							byCurrentUser;						//��ǰ�û�
	BYTE                            byCard[HAND_CARD_NUM];				//������
	BYTE                            byCardCount;                        //����Ŀ	
	BYTE							iMenNums;							//��������
#ifdef RULE_HAVE_TING_PAI_NOTIFY
	BYTE                            byOutCanHuCard[HAND_CARD_NUM];  //������������
	BYTE							byHuCard[HAND_CARD_NUM][MAX_CANHU_CARD_NUM];    //��Ӧ�����ܺ�����
	BYTE							byCardRemainNum[MAX_MJ_PAI];		//���������ʾʣ��������
#endif
	CMD_S_SendCard()
	{
		byCardData=255;
		byCurrentUser=255;
		memset(byCard,255,sizeof(byCard));
		byCardCount=0;
		iMenNums=0;
#ifdef RULE_HAVE_TING_PAI_NOTIFY
		memset(byOutCanHuCard,255,sizeof(byOutCanHuCard));
		memset(byHuCard,255,sizeof(byHuCard));
		memset(byCardRemainNum,0,sizeof(byCardRemainNum));
#endif
	}
};

//�����ܺ�������ʾ
struct CMD_S_OperateNotify
{
	bool							bChi;							
	bool							bPeng;
	bool							bGang;
	bool							bHu;
	bool							bTing;
	BYTE							byUsr;								//�������
	BYTE							byChi[3][3];						//�ܳԵ���
	BYTE							byPeng;								//��������
	BYTE							byGangData[4][2];					//�ܸܵ���	
	BYTE							byTingCanOut[HAND_CARD_NUM];		//�����ܴ����
	BYTE							byHuCard[HAND_CARD_NUM][MAX_CANHU_CARD_NUM];  //���ƣ��������ƺ�������
	BYTE							byCardRemainNum[MAX_MJ_PAI]; //ÿ���Ƶ�ʣ������
	CMD_S_OperateNotify()
	{
		Init();
	}
	void Init()
	{
		bChi=false;
		bPeng=false;
		bGang=false;
		bHu=false;
		bTing=false;
		byUsr=255;
		memset(byChi,255,sizeof(byChi));
		memset(byGangData,255,sizeof(byGangData));
		memset(byTingCanOut,255,sizeof(byTingCanOut));
		byPeng=255;
	}
};

//�����ܺ��������
struct CMD_S_OperateResult
{
	BYTE  byType;    //����������
	BYTE  iStation;  //���������λ��
	BYTE  iBeStation;//�����������λ��
	BYTE  iOutpai;   //���˳�����
	BYTE  byData[4]; //������������
	BYTE  byCard[HAND_CARD_NUM];//��������
	BYTE  byCardCount;//��������
#ifdef RULE_HAVE_TING_PAI_NOTIFY
	BYTE  byOutCanHuCard[HAND_CARD_NUM];                 //������������
	BYTE  byHuCard[HAND_CARD_NUM][MAX_CANHU_CARD_NUM];    //��Ӧ�����ܺ�����
	BYTE  byCardRemainNum[MAX_MJ_PAI];		//���������ʾʣ��������
#endif
	CMD_S_OperateResult()
	{
		byType=255;
		iStation=255;
		iBeStation=255;
		iOutpai=255;
		byCardCount=0;
		memset(byData,255,sizeof(byData));
		memset(byCard,255,sizeof(byCard));
#ifdef RULE_HAVE_TING_PAI_NOTIFY
		memset(byOutCanHuCard,255,sizeof(byOutCanHuCard));
		memset(byHuCard,255,sizeof(byHuCard));
		memset(byCardRemainNum,0,sizeof(byCardRemainNum));
#endif
	}
};

//��Ϸ����
struct CMD_S_GameEnd
{
	bool							bZimo;						//�Ƿ�����
	bool							bIsLiuJu;					//�Ƿ�����
	bool                            bIsHu[PLAY_COUNT];		//��Щ��Һ�����
	BYTE							byFangPao;				//�������λ��
	BYTE							byPs;						//������ 
	BYTE							byArHandPai[PLAY_COUNT][HAND_CARD_NUM];//��������
	BYTE							byArHandPaiCount[PLAY_COUNT];//��������
	///////////////////��������//////////////////
	//BYTE							byBaoCard;					//�����
	TCPGStruct						UserGCPData[PLAY_COUNT][5];	//������
	//BYTE							byArOutPai[PLAY_COUNT][OUT_CARD_NUM]; //��������
	//BYTE							byArOutPaiCount[PLAY_COUNT];	 //���Ƶ�����
	BYTE							byRemainMenPai[MAX_REMAIN_MEN_PAI_NUM];
	BYTE							byRemainMenPaiCount;//ʣ����������
	BYTE							byHuType[PLAY_COUNT][MAX_HUPAI_TYPE];//�������� ��255Ĭ�����ֵ
	int								iFanCount[PLAY_COUNT];				 //�������͵ķ������������ϲ88������һɫ16��
	bool							bTing[PLAY_COUNT];					 //����Ƿ���
	CountHuFenStruct                countHuFen[PLAY_COUNT];     //�����б�
	CountGangFenStruct              countGangFen[PLAY_COUNT];   //�ܷ��б�
	long long						llGameScore[PLAY_COUNT];	//�ܵ÷�
	int								iWinFailDouble[PLAY_COUNT]; //��ҳ���Ӯ���ٱ�
	CMD_S_GameEnd()
	{
		Init();
	}
	void Init()
	{
		bZimo = false;		//�Ƿ�����
		byFangPao=255;
		byPs		= 255;	//������ֵ
		bIsLiuJu	= true;	//�Ƿ�����
		memset(bIsHu,false,sizeof(bIsHu));
		memset(byArHandPai, 255, sizeof(byArHandPai));//��������	
		memset(byArHandPaiCount, 0, sizeof(byArHandPaiCount));
		memset(llGameScore,0,sizeof(llGameScore));
		memset(iFanCount,0,sizeof(iFanCount));
		memset(byHuType,255,sizeof(byHuType));
		memset(bTing,false,sizeof(bTing));
		//byBaoCard=255;
		/*memset(byArOutPai,255,sizeof(byArOutPai));
		memset(byArOutPaiCount,0,sizeof(byArOutPaiCount));*/
		memset(byRemainMenPai,255,sizeof(byRemainMenPai));
		byRemainMenPaiCount = 0;
		memset(iWinFailDouble,0,sizeof(iWinFailDouble));
	}
};
struct CMD_S_ZongResult
{
	int								iZimo[PLAY_COUNT];				//��������
	int								iJiePao[PLAY_COUNT];			//���ڴ���
	int								iDianPao[PLAY_COUNT];			//���ڴ���
	int								iDianGang[PLAY_COUNT];			//��ܴ���
	int								iGang[PLAY_COUNT];			    //���ƴ���
	long long						llGameScore[PLAY_COUNT];		//�ۻ��÷�
	CMD_S_ZongResult()
	{
		Init();
	}
	void Init()
	{
		memset(iZimo,0,sizeof(iZimo));
		memset(iJiePao,0,sizeof(iJiePao));
		memset(iDianPao,0,sizeof(iDianPao));
		memset(iDianGang,0,sizeof(iDianGang));
		memset(iGang,0,sizeof(iGang));
		memset(llGameScore,0,sizeof(llGameScore));
	}
};
//�û��й�
struct CMD_S_Trustee
{
	bool							bTrustee;					//�Ƿ��й�
	BYTE							byDeskStation;				//�й���λ��
	CMD_S_Trustee()
	{
		bTrustee=false;
		byDeskStation=255;
	}
};


////////////////////�ͻ��˷��͸�������
/////////////////////////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ
#define C_S_OUT_CARD				50									//��������
#define C_S_OPERATE_CARD			51									//�����˿�
#define C_S_TRUSTEE				    52									//�û��й�

/*------------------------------------------����------------------------------------*/
#define C_S_SUPER_REQUEST_LOOK							120				//���˿���
#define S_C_SUPER_REQUEST_LOOK_RESULT					121				//���˿��ƽ��
#define C_S_SUPER_REQUEST_CRAD							122				//����Ҫ��
#define S_C_SUPER_REQUEST_CRAD_RESULT					123				//����Ҫ�ƽ��


//���������
struct CMD_C_SuperLook
{
};

//������������
struct CMD_S_SuperLookResult
{
	BYTE    byRemainPaiDate[38];		//ʣ��������	�±�����Ϊ��ֵ	
	CMD_S_SuperLookResult()
	{
		memset(byRemainPaiDate,0,sizeof(byRemainPaiDate));
	}
};

//����Ҫ��
struct CMD_C_Super_Request_Card
{
	BYTE CardDate;                //������
	CMD_C_Super_Request_Card()
	{
		CardDate = 255;
	}
};

//����Ҫ�ƽ��
struct CMD_S_Super_Request_Card_Result
{
	BYTE	ECode;                 //�����룺0 �ɹ�  1 ���ǳ������  2 û��������
	CMD_S_Super_Request_Card_Result()
	{
		ECode = 0;
	}
};

//����
struct CMD_C_OutCard
{
	bool							bTing;								//����
	BYTE							byOutCardData;						//��������
	CMD_C_OutCard()
	{
		bTing=false;
		byOutCardData=255;
	}
};
//�����ܺ�����
struct CMD_C_OperateCard
{
	BYTE							byOperateType;						//��������
	BYTE							byActionData[4];					//����������
	CMD_C_OperateCard()
	{
		memset(byActionData,255,sizeof(byActionData));
		byOperateType=ACTION_NO;
	}
};

//���⣬�ӵ׷�
struct CMD_C_AddScore
{
	int							    iNums;			//����
	CMD_C_AddScore()
	{
		iNums = 0;
	}
};

//�û��й�
struct CMD_C_Trustee
{
	bool							bTrustee;							//�Ƿ��й�	
	CMD_C_Trustee()
	{
		bTrustee=false;
	}
};
#pragma pack()