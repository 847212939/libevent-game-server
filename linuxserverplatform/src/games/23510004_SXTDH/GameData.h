#ifndef GAMEDATA_H
#define GAMEDATA_H
#include "Mj_Message.h"
#include "mj.h"
#include "CLogicBase.h"
///��Ϸ���ݹ�����:�������ݣ���Ϸ�������ݵȣ������ݽ��в����޸ĵȶ���
class GameData
{
public:
	MjRuleSet				m_mjRule;		/// �齫��������	  
	ActionRuleSet			m_mjAction;		/// �齫��������        
	BYTE					m_byNtStation;	//	ׯ��λ��	    
	bool					m_bGangState[PLAY_COUNT];//��¼����״̬��ӵ�ڸܿ��͸ܺ���
	////////////�������ݶ���/////////////////////////////////////////////////////////

	///�������¼�,��ʱ����
	tagChiPai				temp_ChiPai;

	///�������¼�,��ʱ����
	tagPengPai				temp_PengPai;

	///�������¼�,��ʱ����
	tagGangPai				temp_GangPai;

	///�����¼�,��ʱ����(����˿��Ժ���)
	tagHuPai				temp_HuPai[PLAY_COUNT];
public:////////////�¼����ݶ�����//////////////////////
	///��2��ɫ��ȷ������λ�ã������ͣ������Ƶ㣨��С�㣩�¼�	
	tagTwoSeziDirAndGetPai	T_TwoSeziDirAndGetPai;

	///�����¼�	
	//tagSendPai			T_SendPai;

	///������Ҳ����¼�	
	//tagAllBuHua			T_AllBuHua;

	///�����¼�	
	tagOutPai			T_OutPai;

	///�����ܺ���֪ͨ�¼����¼�	  
	tagCPGNotify		T_CPGNotify[PLAY_COUNT];

	///ץ�����¼�		
	tagZhuaPai			T_ZhuaPai;

	///������Ҳ����¼�	
	//tagOneBuHua			T_OneBuHua;

	///�������¼�	
	tagChiPai			T_ChiPai;

	///�������¼�	
	tagPengPai			T_PengPai;

	///�������¼�	
	tagGangPai			T_GangPai;

	///�������¼�	
	tagTingPai			T_TingPai;

	///�������¼�	
	tagHuPai			T_HuPai;

	///����¼�		
	tagCountFen			T_CountFen;
public:
	//��������
	MjMenPaiData		m_MenPai;

	//������
	TCPGStruct			m_UserGCPData[PLAY_COUNT][5];

	///��������
	JingStruct			m_StrJing;

	//���Ʋ�����14�� 
	BYTE				m_byArHandPai[PLAY_COUNT][HAND_CARD_NUM];

	//��������	  
	BYTE				m_byArHandPaiCount[PLAY_COUNT];

	//���Ʋ�����40�ţ�0���˶������ƣ�255���˴������ƣ�2D�齫����Ҫ��ʾ���ƶ�����
	BYTE				m_iArMenPai[4][MEN_CARD_NUM]; //����Ϸ�����޹أ�����4������

	//���Ʋ�����8��
	BYTE				m_byArHuaPai[PLAY_COUNT][HUA_CARD_NUM];

	//���Ƹ���
	BYTE				m_byArHuaPaiCount[PLAY_COUNT];

	//���Ʋ�����40��
	BYTE				m_byArOutPai[PLAY_COUNT][OUT_CARD_NUM];

	//���Ƶ�����
	BYTE				m_byArOutPaiCount[PLAY_COUNT];

	//��ǰ�������	  
	BYTE				m_byNowOutStation;

	//��¼����Ƿ��������(���ڼ������غ�)
	bool				m_bHaveOut[PLAY_COUNT];

	//����Ƿ�����
	bool				m_bTing[PLAY_COUNT];

	//��������(��¥ ���ǰ�¥--ɽ���齫����)
	BYTE				m_byTingType[PLAY_COUNT];

	//ת�������������
	BYTE				m_byMenPaiData[4][MEN_CARD_NUM / 2][2];

	//���������
	BYTE				m_byLastOutPai;

	//ץ���һ���Ƶ����
	BYTE				m_byEndZhuaPaiPalyer;

	//��ǰץ�Ʒ���true ˳ʱ�룬false ��ʱ��	  
	bool				m_bZhuaPaiDir;

	//�ϴγ���λ��
	BYTE				m_byLastOutStation;

	//ͣ�ƻ������������ܴ������
	BYTE				m_byCanOutCard[PLAY_COUNT][HAND_CARD_NUM];

	//Ȧ��
	BYTE				m_byQuanFeng;

	//�ŷ�
	BYTE				m_byMenFeng[PLAY_COUNT];

	///��ǰѡ��������
	BYTE				m_bySelectIndex;

	//ʣ������
	BYTE                m_byRemainMenPai[MAX_REMAIN_MEN_PAI_NUM];

	//ʣ����������
	BYTE                m_byRemainMenPaiCount;

	//�����淨
	BYTE		m_byTingOutCardIndex[PLAY_COUNT];//���Ƶ��۵�������
	BYTE		m_byAllOutCardNum[MAX_MJ_PAI]; //�Ѿ��������ÿ���Ƶ�����
public:
	GameData(void);
	~GameData(void);
	//��ʼ������
	virtual void InitData();

	///��ʼ������
	virtual void LoadIni();

	//////////////////���Ʋ���/////////////////////////////////////
	//����Ƿ����ָ��������
	virtual bool IsHaveAHandPai(BYTE station, BYTE pai);

	//���������ĳ���Ƶĸ���
	virtual int GetAHandPaiCount(BYTE station, BYTE pai);

	//������Ƶ�����
	virtual int GetHandPaiCount(BYTE station);

	//������ҵ�����
	virtual int CopyHandPai(BYTE pai[][HAND_CARD_NUM], BYTE station, bool show);

	//����������ĳ����ҵ�����
	virtual int CopyOneUserHandPai(BYTE pai[HAND_CARD_NUM], BYTE station);

	//��������small�Ƿ��С������
	virtual void SortHandPai(BYTE station, bool big);

	//���һ���Ƶ�������
	virtual void AddToHandPai(BYTE station, BYTE pai);

	//������������
	virtual void SetHandPaiData(BYTE station, BYTE pai[], BYTE byCount);

	//ɾ��һ��ָ��������
	virtual BYTE DeleteAHandPai(BYTE station, BYTE pai);

	//���������Ʊ�
	virtual void SetHandPaiBack(BYTE station, BYTE num);

	//������һ������
	virtual BYTE GetLastHandPai(BYTE station);

	//���ѡ�е�һ������
	virtual BYTE GetSelectHandPai(BYTE station);

	//�Ƿ�������
	virtual bool IsOutPaiPeople(BYTE station);

	//����������Ƿ���ڻ���
	virtual bool IsHuaPaiInHand(BYTE station);

	//�������еĻ����Ƶ�����������
	virtual int MoveHuaPaiFormHand(BYTE station);

	//ͨ��������ȡһ������
	virtual BYTE GetHandPaiFromIndex(BYTE station, int index);

	//ͨ����������һ������
	virtual bool SetHandPaiFromIndex(BYTE station, int index, BYTE pai);

	//�����Ƿ�����
	virtual bool IsHaveHandPaiFromIndex(BYTE station, int index);

	//////////////////�������Ʋ���/////////////////////////////////////
	//��ȡ��Ҹ��Ƶĸ���(���ܣ����ܣ�����)
	virtual int GetUserGangNum(BYTE station, BYTE &an, BYTE &ming, BYTE &bu);

	//��ҳ��ƴ���
	virtual int GetUserChiNum(BYTE station);

	//������ƴ���
	virtual int GetUserPengNum(BYTE station);

	//��Ҹ���ĳ����
	virtual bool IsUserHaveGangPai(BYTE station, BYTE pai, BYTE &type);

	//����Ƿ�����ĳ����
	virtual bool IsUserHavePengPai(BYTE station, BYTE pai);

	//����Ƿ�Թ�ĳ����
	virtual bool IsUserHaveChiPai(BYTE station, BYTE pai);

	//���һ�����ݵ��ܳ���������
	virtual void AddToGCP(BYTE station, TCPGStruct *gcpData);

	//ɾ���ճ���������ָ����������
	virtual void DeleteAGCPData(BYTE station, BYTE type, BYTE pai);

	//�����ܳ��������е�������
	virtual int CopyGCPData(BYTE station, TCPGStruct gcpData[]);

	//���ó��������е�������
	virtual void SetGCPData(BYTE station, TCPGStruct gcpData[]);

	//��ȡ�ܳ����Ľڵ����
	virtual int GetGCPCount(BYTE station);

	//��������ȡ�ܳ�������ĳ���Ƶ�����
	virtual int GetOnePaiGCPCount(BYTE station, BYTE pai);

	//����ܳ�������
	virtual void ShortGCP(BYTE station);

	//��������ʼ���ܳ�������
	virtual void InitGCP(BYTE station);

	///////////////////���Ʋ���///////////////////////////////////////////////////////
	//���һ���Ƶ�����������
	virtual void AddToOutPai(BYTE station, BYTE pai, bool bTingCard = false);

	//ɾ�����һ�ų���
	virtual void DeleteLastOutPai(BYTE station);

	//���ó�������
	virtual void SetOutPaiData(BYTE station, BYTE pai);

	//������ҵĳ���
	virtual int CopyOutPai(BYTE station, BYTE pai[]);

	//��ȡ��ҵĳ�������
	virtual int GetOutPaiCount(BYTE station);

	//��������ȡ���ĳ���Ƶĳ�������
	virtual int GetOneOutPaiCount(BYTE station, BYTE pai);

	//////////////////���Ʋ���///////////////////////////////////////////////////////
	//���һ�Ż��Ƶ�����������
	virtual void AddToHuaPai(BYTE station, BYTE pai);

	//���û�������
	virtual void SetHuaPaiData(BYTE station, BYTE pai[]);

	//������ҵĻ���
	virtual int CopyHuaPai(BYTE station, BYTE pai[]);

	//����Ƿ���
	virtual bool CheckIsHuaPai(BYTE pai);

	///////////////////�����Ʋ���///////////////////////////////////////////////////////
	//������������
	virtual void SetMenPaiData(BYTE station, BYTE pai[]);

	//������������
	virtual void CopyAllMenPai(BYTE pai[][MEN_CARD_NUM]);

	//������ҵ�����
	virtual int CopyMenPai(BYTE station, BYTE pai[]);

	//����������ת��
	virtual void ChangeMenPai(int iIndex, BYTE byCard = 255);

	//��������������
	virtual void CreateMenPai();

	//��������ȡĳ��ҵ���������
	virtual int GetMenPaiCount(BYTE station);

	///////////////////�������//////////////////////////////////////////////
	/////���pai�Ƿ����
	//virtual bool CheckIsCaiShen(BYTE pai);
	/////��ղ���ṹ
	//virtual void InitCaiShen();
	/////////////////�����ͻ��������Ʋ���///////////////////////
	//����������
	virtual void ChangeAllHandPai(BYTE station, BYTE pai[], BYTE count);

	//��ָ������
	virtual void ChangeSomeHandPai(BYTE station, BYTE pai[], BYTE handpai[], BYTE count);

	///������ǽ����һ����
	virtual void SetGameNextPai(BYTE pai);
};
#endif