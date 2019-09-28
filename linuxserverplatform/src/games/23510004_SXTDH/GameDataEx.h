#ifndef GAMEDATAEX_H
#define GAMEDATAEX_H
#include "GameData.h"
#include <vector>

///��Ϸ���ݹ�����
class GameDataEx : public GameData
{
public:
	bool		m_bIsHu[PLAY_COUNT];		//�Ƿ����
	bool		m_bQiHu[PLAY_COUNT];		//����Ƿ�����״̬
	bool		m_bQiPeng[PLAY_COUNT];		//����
	int			m_iGangScore[PLAY_COUNT];	//���Ƶ÷�
	int			m_iHuaScore[PLAY_COUNT];	//�����÷�
	int         m_iAddScore[PLAY_COUNT];	//ÿһ�ѵ�����
	int			m_iHuangZhuangCount;//��ׯ����
	int         m_iZhuangWinCount; //ׯ����Ӯ����
	//BYTE		m_byDingQue[PLAY_COUNT];	//����ȱ��0����  1���� 2��Ͳ
	BYTE		m_byQiHuTurn;//������
	BYTE		m_byPengTurn;//���Ƶ�
	std::vector<BYTE> m_vecQiPengCard[PLAY_COUNT];
	tagMatchMJ  m_tMatchMJ;
	tagActionTurn  m_tActionTurn[PLAY_COUNT];
	//��ǰ�ɲ��������ܺ������
	BYTE		m_byCurrentOperatorUser;

	//�������
	tagStructGameRuler m_tGameRuler;
	bool		m_bIsGangShangGang[PLAY_COUNT];//�Ƿ��и��ϸܣ�
	int			m_bGangZiMo[PLAY_COUNT];//��¼������������ɶҲûӴ 0��,���� 1�������������� 2��

	//����
	vector<BYTE>   m_vHuapaiTemp[PLAY_COUNT]; //����ÿ����һ��Ƶ�����
	bool		   m_bIsBuHua[PLAY_COUNT];	  //�Ƿ����ֲ�����
	bool           m_bIsBuAllHua[PLAY_COUNT]; //�Ƿ������л�
	BYTE		   m_byFangWei[PLAY_COUNT];	  //��λ����������
	BYTE		   m_byHuaPaiType[PLAY_COUNT][2];//�������ͣ������ﶬ�ƣ�÷������ơ�ÿ�������ֱ���÷�ʹ�

	//�µ����ͳ��
	CountHuFenStruct                m_countHuFen[PLAY_COUNT];     //����
	CountGangFenStruct              m_countGangFen[PLAY_COUNT];   //�ܷ�
	//CountGenZhuangFenStruct         m_countGenZhuang[PLAY_COUNT]; //��ׯ��
	//CountHorseFenStruct				m_countHorseFen[PLAY_COUNT];  //���

	//����������ơ��������ݣ���������ʹ��
	BYTE m_byOutCanHuCard[HAND_CARD_NUM];				//�������ƿɺ�
	BYTE m_byHuCard[HAND_CARD_NUM][MAX_CANHU_CARD_NUM];//�������ƺ�������
	BYTE m_byCardRemainNum[MAX_MJ_PAI];		//���������ʾʣ��������
	BYTE m_byOutAfterHuCard[PLAY_COUNT][MAX_CANHU_CARD_NUM]; //���½���ʾ����
	BYTE m_byCardType[MAX_MJ_KIND_NUM]; //�������е���
public:
	GameDataEx(void);
	~GameDataEx(void);
	//��ʼ������
	virtual void InitData();
	//��ʼ������
	virtual void LoadIni();

	//////////////////���Ʋ���/////////////////////////////////////
	///����Ƿ����ָ��������
	virtual	bool IsHaveAMenPai(BYTE pai);

	//��ָ����һ������
	bool	ChangeAHandPai(BYTE station, BYTE ChangePai, BYTE byNewCard);

	//��ָ��һ������
	bool	ChangeAMenPai(BYTE ChangePai, BYTE byNewCard);

	///�Ƿ�������
	virtual bool IsOutPaiPeople(BYTE station);

	///��ȡ���Ƶ���ɫ��
	virtual int GetPaiSeCount(BYTE station);

	///������ץһ����
	virtual BYTE GetPai(int iStation, bool bhead, BYTE *pIndex = NULL, BYTE byGetPai = 255);

	///�Ƿ���ȱ�ŵ���
	bool IsHaveQueMen(BYTE station, BYTE type);

	//��ȡһ��data��û�е��ƣ���������
	BYTE GetChangePai(BYTE station, BYTE data[]);

	///���ĳ�ֻ�ɫ�Ƶĸ�����0�� 1Ͳ 2��)
	int GetAKindPaiCount(BYTE station, BYTE kind);

	///�õ�ĳ�ֻ�ɫ�ƣ�0�� 1Ͳ 2��)
	int GetAKindPai(BYTE station, BYTE kind);

	//////////////////�������Ʋ���/////////////////////////////////////
	///���һ�����ݵ��ܳ���������
	virtual void AddToGCP(BYTE station, TCPGStruct *gcpData);

	//////////////////���Ʋ���///////////////////////////////////////////////////////
	///���ĳ�����Ƿ����Ż���
	virtual bool CheckIsHaveHuaPai(BYTE station, BYTE pai);

	///////////////////��չ����///////////////////////////////////////////////////////
	//������
	virtual bool DisPatchCard();

	///��ȡ�¼�λ��
	BYTE GetNextStation(BYTE station, bool shun);

	//����
	BYTE MatchMJ();

	//����
	void TurningString(BYTE &byPai0, BYTE &byPai1);

	//�ж��Ƿ������ׯ����
	int CheckIsShouZhangGen(bool bIsNext = false);

	//��ȡ������ҵ����λ�ù�ϵ
	BYTE GetPlayerStationRelation(BYTE byMeStation, BYTE byOtherStation);

	//��ȡbyMeStation��Һͷ�byNoStation�Ĺ�ϵ
	BYTE GetPlayerStationRelationEx(BYTE byMeStation, BYTE byNoStation);

	//���ʣ������
	BYTE GetRemainMenPai(BYTE m_byRemainMenPai[MAX_REMAIN_MEN_PAI_NUM]);

	//��Ӻ���
	//BYTE byNameType[3];					//��Ӧ�����֣� ö������MJ_HU_FEN_TYPE
	//BYTE byXiangYingStation[3];			//��Ӧ��ң�ö������MJ_STATION
	//BYTE byXiangYingStationEx[3];		//��Ӧ��ң�ö������MJ_STATION
	//bool bBaoTing[3];					//������ң�byXiangYingStationEx�Ƿ������ǵ�����ң�������ң���byXiangYingStation�Ƿ���
	//int iAllFen[3];						//��Ӯ�ĺ���
	bool AddToHuFenStruct(BYTE byStation, BYTE byNameType, BYTE byXiangYingStation, BYTE byXiangYingStationEx, bool bBaoTing, int iAllFen);

	//��Ӹܷ�
	bool AddToGangFenStruct(BYTE byStation, BYTE byGangType, BYTE byXiangYingStation, bool bBaoTing, int iAllFen);

	//��������б��
	bool DelBuGangFenStruct(BYTE byStation, BYTE byGangType, BYTE byXiangYingStation, bool bBaoTing, int iAllFen);

	//������
	bool AddToHorseFenStruct(BYTE byStation, BYTE byBuyHorseType, BYTE byMaiStation, BYTE byBeiMaiStation, int iAllFen);

	//��Ӹ�ׯ��
	bool AddToGenZhuangFenStruct(BYTE byStation, BYTE byGenZhuangType, BYTE byXiangYingStation, int iAllFen);
};
#endif