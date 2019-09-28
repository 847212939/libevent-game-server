#pragma once
#include "StdAfx.h"
#include "MJ/GameDataEx.h"
#include "MJ/CheckHuPaiEx.h"
#include "MJ/CheckCPGActionEx.h"
#include "iostream"
#include "string"
#include "json/json.h"
#include "fstream"
#pragma comment(lib,"json_vc71_libmt.lib")
#pragma warning(disable:4305)
#pragma warning(disable:4309)

//�˴������Ϸ��ʱ������Ϸ��ʱ��
//�ر�ע�ⶨʱ����ID��Χ1-50
#define TIME_ACTION				    30  //�齫������ʱ��
#define TIME_THINK				    31  //˼����ʱ��
#define TIME_TING_OUTCARD			32	//���Ƴ���
#define TIME_TUOGUAN			    33	//�йܳ���
#define TIME_HUAPAI                 34  //���ֻ���
#define TIME_BUHUA                  35  //����
#define TIME_CHECK_ACTION           36  //��⶯��
#define TIME_DELAY_SEND_ACTION      38  //��ʱ���Ͷ���
#define TIME_TUOGUAN_ACTION			39	//�йܶ����������ܺ����ȵȣ�

/// ��Ϸ������
class CServerGameDesk : public CGameDesk
{
public:
	GameDataEx						m_GameData;							//��Ϸ����
	CheckCPGActionEx				m_CPGData;							//����������
	CheckHuPaiEx					m_CheckHuData;						//��������
	bool							m_bAgree[PLAY_COUNT];				//ͬ���ʾ
	bool							m_bTrustee[PLAY_COUNT];				//�Ƿ��й�
	BYTE                            m_byMJCardType;						//�齫����ʽ
	int								m_iPlayingCount;					//��Ϸ���о���
	CMD_S_ZongResult				m_tZongResult;						//��ս��
	int								m_iVideoIndex;                      //ս���ط�����
	Json::Value						m_element;                          //ս���ط�Ԫ��
	Json::Value						m_root;                             //ս���طŵĸ�
	bool							m_bNetCut[PLAY_COUNT];				//����
	BYTE                            m_byBeginTempCard;					//��ʱ�������
	int                             m_iOutCardChaoShiCount[PLAY_COUNT]; //��ʱ���ƴ���

	////////////////////////////////��̬����///////////////////////////////
	static bool                     gOutMessageSize; //���Э���С

	////////////////////////////////����///////////////////////////////
	bool							m_bSuperFlag[PLAY_COUNT];			//������ҽ��
	BYTE							m_ChangeCard[PLAY_COUNT];			//��������´�ץ����ֵ

public:
	/// ���캯��
	CServerGameDesk();
	/// ��������
	virtual ~CServerGameDesk();
public:
	/// ��ʼ��Ϸ
	virtual bool InitDeskGameStation();

	/// ��Ϸ��ʼ
	virtual bool GameBegin(BYTE bBeginFlag);

	/// ��Ϸ����
	virtual bool GameFinish(BYTE bDeskStation, BYTE bCloseFlag);

	/// �ж��Ƿ�������Ϸ
	virtual bool IsPlayGame(BYTE bDeskStation);

	/// ��Ϸ���ݰ�������
	virtual bool HandleNotifyMessage(BYTE bDeskStation, unsigned int assistID, void* pData, int size, bool bWatchUser = false);

	/// �û��뿪��Ϸ��
	virtual bool UserLeftDesk(GameUserInfo* pUser);

	//��Ҷ��ߴ���
	virtual bool UserNetCut(GameUserInfo *pUser);

	/// ��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);

	/// ������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);

	/// ��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);

	//���ӳɹ���ɢ
	virtual void OnDeskSuccessfulDissmiss(bool isDismissMidway = true);

	// ��������й�
	virtual bool OnHandleUserRequestAuto(BYTE deskStation);

	// ���ȡ���й�
	virtual bool OnHandleUserRequestCancelAuto(BYTE deskStation);

	//��ʼ����Ϸ���ݣ�������������ʼ��
	virtual void InitDeskGameData();

	// ��̬���������ļ�����
	virtual void LoadDynamicConfig();
public:
	///������station��ǰ������Щ����
	bool CheckAction(int iStation, BYTE byCard, BYTE byType);

	//ץ��
	void ZhuaPai(BYTE byCurrentStation, bool bHead);

	//�û�����
	bool OnUserOutCard(BYTE byDeskStation, CMD_C_OutCard *pOutCard);

	//�û�����
	bool OnUserOperate(BYTE byDeskStation, CMD_C_OperateCard *pAction);

	//����
	bool OnChi(BYTE byDeskStation, CMD_C_OperateCard *pAction);

	//����
	bool OnPeng(BYTE byDeskStation, CMD_C_OperateCard *pAction);

	//����
	bool OnGang(BYTE byDeskStation, CMD_C_OperateCard *pAction);

	//����
	bool OnHu(BYTE byDeskStation, CMD_C_OperateCard *pAction);

	//��
	bool OnPass(BYTE byDeskStation, CMD_C_OperateCard *pAction);

	///������ж�������������Ϣ�����
	bool HaveUserCanDoAction(BYTE byIgnoreStation);

	//���ͳ�������
	void SendChiPengGangHuMsg(int iDesk);

	////��¼����״̬��ӵ�ڸܿ��͸ܺ���
	void RecordGangStatus();

	//json�洢��ս���ط�
	void OnJson(void *pData, int iType);

	//����
	bool HandCard();

	//��ȡ�������
	void GetSpecialRule();

	//����tagStructGameRuler������Ӧ����
	void SetSpecialRule();

	//�Զ�ѡ��һ���Ƴ�
	BYTE AutoOutCard(BYTE byStation);

	//�Զ�����
	bool AutoCanAction(BYTE byStation, BYTE byActionType);

	//���ͳ��Ƴ�����Ϣ
	void SendOutCardError(BYTE byDeskStation, int iErrorCode);

	//���������������
	bool SuperUserLookMenPai(BYTE byDeskStation, CMD_C_SuperLook *SuperLook);

	//�����������Ҫ��
	bool SuperUserChangePai(BYTE byDeskStation, CMD_C_Super_Request_Card *SuperChangeCard);
};