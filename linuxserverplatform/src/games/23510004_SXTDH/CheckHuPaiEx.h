#ifndef CHECKHUPAIEX_H
#define CHECKHUPAIEX_H
#include "CheckHuPai.h"
///�¼������ࣨ���أ�
class CheckHuPaiEx :public CheckHuPai
{
public:
	int   m_byTingMaxFan;//���ƿ��ܵ����
	int   m_iHuFenType[PLAY_COUNT][MAX_HU_FEN_KIND];
public:
	CheckHuPaiEx(void);
	~CheckHuPaiEx(void);
	//��ʼ������
	virtual void InitData();

	/////////����Ƿ��ܺ���////////
	virtual bool CanHu(GameDataEx *pGameData, BYTE station, BYTE byBeStation, BYTE lastpai, bool zimo);

	///////////////�齫���//////////////////
	void CountHuFen(GameDataEx *pGameData);

	//���ݺ������ͣ���ȡ���Ʊ���
	int GetHuTypeFans(GameDataEx *pGameData, BYTE m_byHuType[MAX_HUPAI_TYPE]);

	//����ܷ�����
	virtual bool CanTing(GameDataEx *pGameData, BYTE station, BYTE CanOut[]);

	//��ȡһ�ſ�������
	BYTE GetCanTingCard(GameDataEx *pGameData, BYTE station);

	//��ȡ����
	virtual void ExtractHu(GameDataEx *pGameData, BYTE station, BYTE byHuCard[]);

	//�������г�һ���ƣ�Ȼ����ȡ�ܺ�����
	bool OutCardExtractHu(GameDataEx *pGameData, BYTE station, BYTE byOutCanHuCard[HAND_CARD_NUM],
		BYTE byHuCard[HAND_CARD_NUM][MAX_CANHU_CARD_NUM], BYTE byCardRemainNum[MAX_MJ_PAI]);

	//������
	bool CheckAllJiang(GameDataEx *pGameData);

	///�����߶��������Ƶ�����
	int CountLongQi();

	///�����߶��������Ƶ�����
	bool CheckHuGen(BYTE lastpai);

	//����Ƿ��۾��߶�
	bool CheckYaoJiuQiDui(GameDataEx *pGameData);

	//���߶Լ��
	bool CheckQiDuiEx(GameDataEx *pGameData);

	//����������Ƶĺ����Ƿ�ı�
	bool IsTingKouChange(GameDataEx *pGameData, BYTE station, BYTE byChangeCard[HAND_CARD_NUM]);
};
#endif