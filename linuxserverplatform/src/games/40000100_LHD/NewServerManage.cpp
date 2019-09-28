#include "Stdafx.h"
#include "NewServerManage.h"
#include "UpgradeMessage.h"

CNewServerManage::CNewServerManage() : CGameDesk(ALL_ARGEE)
{
	m_gameStatus = 0;
	m_dealerDeskStation = SYSTEM_DEALER_DESKSTATION;
	m_LastDealerDeskStation = 255;
	m_LastShangzhuangMoney = 0;
	m_xiaZhuangRightNow = false;
	m_currDealerGameCount = 0;
	m_startTime = 0;
	m_curCtrlWinCount = 0;
	m_runCtrlCount = 0;
	m_ctrlParamIndex = 0;
	memset(m_szLastCtrlParam, 0, sizeof(m_szLastCtrlParam));
}

CNewServerManage::~CNewServerManage()
{
}

bool CNewServerManage::InitDeskGameStation()
{
	m_userInfoVec.resize(PLAY_COUNT);
	m_dealerInfoVec.resize(MAX_SHANGZHUANG_COUNT);

	LoadDynamicConfig();

	ReSetGameState(0);

	m_allCount = 0;
	m_longWinCount = 0;
	m_iContinueZhuang = -1;

	return true;
}

bool CNewServerManage::OnStart()
{
	m_gameStatus = GAME_STATUS_WAITNEXT;
	SetTimer(GAME_TIMER_WAITNEXT, m_gameConfig.waitBeginTime * 1000);		// ������һ�׶εĶ�ʱ��

	ChangeSystemZhuangInfo();

	return true;
}

bool CNewServerManage::OnGetGameStation(BYTE deskStation, UINT socketID, bool bWatchUser)
{
	if (deskStation >= PLAY_COUNT)
	{
		return false;
	}

	GameUserInfo user;
	GetUserData(deskStation, user);
	m_bIsSuper[deskStation] = (user.userStatus&USER_IDENTITY_TYPE_SUPER) == USER_IDENTITY_TYPE_SUPER ? 1 : 0;

	NewGameMessageGameStation msg;

	msg.byIsSuper = m_bIsSuper[deskStation];

	// �������
	msg.cfgWaitBeginKeepTime = m_gameConfig.waitBeginTime;
	msg.cfgNoteKeepTime = m_gameConfig.noteKeepTime;
	msg.cfgSendCardKeepTime = m_gameConfig.sendCardKeepTime;
	msg.cfgCompareKeepTime = m_gameConfig.compareKeepTime;
	msg.cfgSettleKeepTime = m_gameConfig.waitSettleTime;
	msg.cfgShangZhuangLimitMoney = m_gameConfig.shangZhuangLimitMoney;
	msg.cfgSitLimitMoney = m_gameConfig.sitLimitMoney;
	int roomLevel = GetRoomLevel() >= 1 && GetRoomLevel() <= 3 ? GetRoomLevel() - 1 : 0;
	memcpy(msg.cfgNoteList, m_gameConfig.noteList[roomLevel], sizeof(msg.cfgNoteList));

	msg.gameStatus = m_gameStatus;
	msg.nextStageLeftSecs = (int)GetNextStageLeftSecs();
	const DTUserInfo& userInfo = m_userInfoVec[deskStation];
	memcpy(msg.myAreaNoteList, userInfo.noteList, sizeof(msg.myAreaNoteList));

	//����������ע
	if (msg.byIsSuper)
	{
		for (int i = 0; i < MAX_AREA_COUNT; i++)
		{
			msg.areaNoteList[i] = m_areaInfoArr[i].areaRealPeopleNote;
		}
	}
	else
	{
		for (int i = 0; i < MAX_AREA_COUNT; i++)
		{
			msg.areaNoteList[i] = m_areaInfoArr[i].areaNote;
		}
	}

	MakeSendCardInfo(msg.sendCardInfo);
	MakeZhuangInfo(msg.zhuangInfo);


	msg.curZhuangPlayCount = m_currDealerGameCount;
	msg.allZhuangPlayCount = m_gameConfig.maxZhuangGameCount;

	memset(msg.deskUser, 255, sizeof(msg.deskUser));
	for (size_t i = 0; i < m_gameDesk.size(); i++)
	{
		msg.deskUser[i] = m_gameDesk[i];
	}
	memcpy(msg.areaChouMaCount, m_areaChouMaCount, sizeof(msg.areaChouMaCount));

	SendGameStation(deskStation, socketID, bWatchUser, &msg, sizeof(msg));
	
	return true;
}

// ��ׯ����
bool SortShangZhuangByGold(const DealerInfo &v1, const DealerInfo &v2)
{
	return v1.shangzhuangMoney > v2.shangzhuangMoney;
}

bool CNewServerManage::HandleNotifyMessage(BYTE deskStation, unsigned int assistID, void * pData, int size, bool bWatchUser)
{
	if (bWatchUser == true)
	{
		return false;
	}

	if (deskStation >= (BYTE)m_userInfoVec.size())
	{
		return false;
	}
	switch (assistID)
	{
	case MSG_GAME_REQ_SHANGZHUANG:
	{
		OnHandleUserRequestShangZhuang(deskStation, pData, size);
		return true;
	}
	case MSG_GAME_REQ_XIAZHUANG:
	{
		OnHandleUserRequestXiaZhuang(deskStation);
		return true;
	}
	case MSG_GAME_REQ_NOTE:
	{
		OnHandleUserReuqestNote(deskStation, pData, size);
		return true;
	}
	case MSG_GAME_REQ_CONTINUE_NOTE:
	{
		OnHandleRequestContinueNote(deskStation);
		return true;
	}
	case MSG_GAME_REQ_AREA_TREND:
	{
		OnHandleRequestAreaTrend(deskStation);
		return true;
	}
	case MSG_GAME_REQ_SUPER_SET:
	{
		OnHandleRequestSuperSet(deskStation, pData, size);
		return true;
	}
	case MSG_GAME_REQ_CONTINUE_ZHUANG:
	{
		OnHandleRequestContinueZhuang(deskStation, pData, size);
		return true;
	}
	case MSG_GAME_REQ_SIT:
	{
		OnHandleRequestSit(deskStation, pData, size);
		return true;
	}
	default:
		break;
	}

	return __super::HandleNotifyMessage(deskStation, assistID, pData, size, bWatchUser);
}

bool CNewServerManage::OnTimer(unsigned int timerID)
{
	if (timerID != GAME_TIMER_NOTEINFO)
	{
		KillTimer(timerID);
	}

	switch (timerID)
	{
	case GAME_TIMER_WAITNEXT:
	{
		OnTimerWaitNext();
		return true;
	}
	case GAME_TIMER_NOTE:
	{
		OnTimerNote();
		return true;
	}
	case GAME_TIMER_NOTEINFO:
	{
		OnTimerSendNoteInfo();
		return true;
	}
	case GAME_TIMER_SENDCARD:
	{
		OnTimerSendCard();
		return true;
	}
	case GAME_TIMER_COMPARE:
	{
		OnTimerCompare();
		return true;
	}
	case GAME_TIMER_SETTLE:
	{
		OnTimerSettle();
		return true;
	}
	default:
		break;
	}

	return true;
}

bool CNewServerManage::ReSetGameState(BYTE bLastStation)
{
	// ��������¼
	m_tempChouMaDataCount = 0;
	memset(m_areaChouMaCount, 0, sizeof(m_areaChouMaCount));

	// ���������Ϣ
	for (size_t i = 0; i < m_areaInfoArr.size(); i++)
	{
		m_areaInfoArr[i].Clear();
	}

	// �����ҵ���ע��Ϣ
	for (size_t i = 0; i < m_userInfoVec.size(); i++)
	{
		DTUserInfo& userInfo = m_userInfoVec[i];
		userInfo.ClearNoteInfo();
	}
	if (!m_dealerInfoVec.empty())
	{
		for (auto it = m_dealerInfoVec.begin(); it != m_dealerInfoVec.end();)
		{
			long userID = it->userID;
			if (GetUserIDByDeskStation(GetDeskStationByUserID(userID)) != userID)
			{
				it = m_dealerInfoVec.erase(it);
				continue;
			}
			GameUserInfo* pUser = m_pDataManage->GetUser(userID);
			//������Ϸ����ʼ�����ӵ�ʱ��Ҳ��������ӿڣ������Ǹ�ʱ���û�δ��½���鲻���û���Ϣ�Ļᵼ�´�����Ҫ��ָ������пղ���
			if (NULL == pUser)
			{
				it = m_dealerInfoVec.erase(it);
				continue;
			}
			if (pUser->money < it->shangzhuangMoney)
			{
				it = m_dealerInfoVec.erase(it);
				continue;
			}
			if (pUser->deskStation == m_dealerDeskStation)
			{
				it++;
				continue;
			}
			if (pUser->IsOnline == false)
			{
				it = m_dealerInfoVec.erase(it);
				continue;
			}
			it++;
		}

		std::sort(m_dealerInfoVec.begin(), m_dealerInfoVec.end(), SortShangZhuangByGold);
	}

	// ���������б�
	ClearDeskUser();

	return true;
}

bool CNewServerManage::UserLeftDesk(GameUserInfo* pUser)
{
	if (!pUser)
	{
		ERROR_LOG("pUser is NULL");
		return false;
	}

	if (pUser->deskStation >= m_userInfoVec.size())
	{
		ERROR_LOG("user deskStation is invalid deskStation=%d", pUser->deskStation);
		return false;
	}

	bool bKickShangZhuangUser = false;

	// ���Ϊׯ
	if (pUser->deskStation == m_dealerDeskStation)
	{
		ERROR_LOG("ׯ�ұ���");
	}
	else
	{
		for (auto it = m_dealerInfoVec.begin(); it != m_dealerInfoVec.end(); ++it)
		{
			if (it->userID == pUser->userID)
			{
				it = m_dealerInfoVec.erase(it);
				bKickShangZhuangUser = true;
				break;
			}
		}
	}

	if (bKickShangZhuangUser)
	{
		std::sort(m_dealerInfoVec.begin(), m_dealerInfoVec.end(), SortShangZhuangByGold);
		BroadcastZhuangInfo();
	}

	DTUserInfo& userInfo = m_userInfoVec[pUser->deskStation];
	userInfo.Clear();

	// ɾ����λ��
	DelDeskUser(pUser->deskStation);

	return __super::UserLeftDesk(pUser);
}

bool CNewServerManage::UserNetCut(GameUserInfo* pUser)
{
	return __super::UserNetCut(pUser);
}

bool CNewServerManage::OnHandleUserRequestShangZhuang(BYTE deskStation, void* pData, int size)
{
	if (size != sizeof(NewGameMessageReqShangZhuang))
	{
		return false;
	}

	NewGameMessageReqShangZhuang* pMessage = (NewGameMessageReqShangZhuang*)pData;
	if (!pMessage)
	{
		return false;
	}

	long userID = GetUserIDByDeskStation(deskStation);
	if (userID <= 0)
	{
		ERROR_LOG("invalid userID = 0");
		return false;
	}

	GameUserInfo* pUser = m_pDataManage->GetUser(userID);
	if (!pUser)
	{
		ERROR_LOG("GetUser failed userID=%d", userID);
		return false;
	}

	if (deskStation == m_dealerDeskStation)
	{
		return false;
	}

	NewGameMessageRspShangZhuang msg;

	// ����ׯ�б����޷�������ׯ
	for (auto it = m_dealerInfoVec.begin(); it != m_dealerInfoVec.end(); ++it)
	{
		if (it->userID == userID)
		{
			ERROR_LOG("user:%d is already in shangzhuang list");
			SendGameMessage(deskStation, "���Ѿ���������ׯ�������ٴ����룡", SMT_EJECT);
			return false;
		}
	}

	// ��ׯ�б������޷���ׯ
	if (m_dealerInfoVec.size() >= MAX_SHANGZHUANG_COUNT)
	{
		SendGameMessage(deskStation, "��ׯ�б�����!", SMT_EJECT);
		return true;
	}

	// ����Ǯ����
	if (pUser->money < pMessage->shangzhuangMoney)
	{
		SendGameMessage(deskStation, "�ʽ𲻹�", SMT_EJECT);
		return true;
	}

	// Ǯ�����޷���ׯ
	if (pMessage->shangzhuangMoney < m_gameConfig.shangZhuangLimitMoney)
	{
		SendGameMessage(deskStation, "�ʽ𲻹���ׯ", SMT_EJECT);
		return true;
	}
	// ��������ׯ�����ж�
	if (IsVirtual(deskStation) && !IsVirtualCanShangXiazZhuang(deskStation, 0))
	{
		return false;
	}
	// ������ׯ�б�
	DealerInfo info;
	info.isSystem = false;
	info.userID = userID;
	info.shangzhuangMoney = pMessage->shangzhuangMoney;
	m_dealerInfoVec.push_back(info);

	// ��ׯ����
	std::sort(m_dealerInfoVec.begin(), m_dealerInfoVec.end(), SortShangZhuangByGold);

	// ֪ͨ�仯
	BroadcastZhuangInfo();

	SendGameMessage(deskStation, "��ׯ�ɹ���", SMT_EJECT);

	SendGameData(deskStation, &msg, sizeof(msg), MSG_MAIN_LOADER_GAME, MSG_GAME_RSP_SHANGZHUANG, 0);
	return true;
}

bool CNewServerManage::OnHandleUserRequestXiaZhuang(BYTE deskStation)
{
	long userID = GetUserIDByDeskStation(deskStation);
	if (userID <= 0)
	{
		ERROR_LOG("invalid userID = 0");
		return false;
	}

	GameUserInfo* pUser = m_pDataManage->GetUser(userID);
	if (!pUser)
	{
		ERROR_LOG("GetUser failed userID=%d", userID);
		return false;
	}

	bool bXiaZhuang = false;
	// ֻ�д�����ׯ�б�����Լ�Ϊׯ�Ҳ�����ׯ
	for (auto it = m_dealerInfoVec.begin(); it != m_dealerInfoVec.end(); ++it)
	{
		if (it->userID == userID)
		{
			bXiaZhuang = true;
			break;
		}
	}
	// ��������ׯ�����ж�
	if (IsVirtual(deskStation) && !IsVirtualCanShangXiazZhuang(deskStation, 1))
	{
		return false;
	}

	if (!bXiaZhuang && deskStation != m_dealerDeskStation)
	{
		return false;
	}

	if (deskStation == m_dealerDeskStation)
	{
		// ������ֽ���֮��������ׯ����ׯ
		SendGameMessage(deskStation, "�����Ѿ���ʼ�����ֽ�����������ׯ��");
		m_xiaZhuangRightNow = true;
	}
	else
	{
		// ����ׯ�б����Ƴ�
		for (auto it = m_dealerInfoVec.begin(); it != m_dealerInfoVec.end(); ++it)
		{
			if (it->userID == userID)
			{
				it = m_dealerInfoVec.erase(it);
				break;
			}
		}
		SendGameMessage(deskStation, "��ׯ�ɹ���");
	}


	// ��ׯ����
	std::sort(m_dealerInfoVec.begin(), m_dealerInfoVec.end(), SortShangZhuangByGold);

	BroadcastZhuangInfo();

	SendGameData(deskStation, NULL, 0, MSG_MAIN_LOADER_GAME, MSG_GAME_RSP_XIAZHUANG, 0);
	return true;
}

bool CNewServerManage::OnHandleUserReuqestNote(BYTE deskStation, void* pData, int size)
{
	if (size != sizeof(NewGameMessageReqNote))
	{
		return false;
	}
	if (m_gameStatus != GAME_STATUS_NOTE)
	{
		return false;
	}

	NewGameMessageReqNote* pMessage = (NewGameMessageReqNote*)pData;
	if (!pMessage)
	{
		return false;
	}

	if (pMessage->area < 0 || pMessage->area >= MAX_AREA_COUNT)
	{
		ERROR_LOG("area is invalid area=%d", pMessage->area);
		return false;
	}

	long userID = GetUserIDByDeskStation(deskStation);
	if (userID <= 0)
	{
		ERROR_LOG("invalid userID = 0");
		return false;
	}

	NewGameMessageRspNote msg;

	// ׯ�Ҳ�����ע
	if (deskStation == m_dealerDeskStation)
	{
		return false;
	}

	GameUserInfo* pUser = m_pDataManage->GetUser(userID);
	if (!pUser)
	{
		ERROR_LOG("GetUser failed userID=%d", userID);
		return false;
	}
	long long note = GetNoteByIndex(pMessage->noteIndex);
	if (note <= 0)
	{
		ERROR_LOG("note index is invalid noteIndex=%d", pMessage->noteIndex);
		return false;
	}

	long long llUserResNums = 0;
	if (!GetRoomResNums(deskStation, llUserResNums))
	{
		ERROR_LOG("GetUser failed userID=%d", userID);
		return false;
	}

	//��������עֵ
	long long llAllAreaBet = 0;
	for (int i = 0; i < MAX_AREA_COUNT; i++)
	{
		llAllAreaBet += m_areaInfoArr[i].areaNote;
	}

	//�����ע����
	long long iAllBet = 0;
	for (int i = 0; i < MAX_AREA_COUNT; i++)
	{
		iAllBet += m_userInfoVec[deskStation].noteList[i];
	}

	if ((iAllBet + note) > llUserResNums)
	{
		//���ϵ�Ǯ������ע
		SendGameMessage(deskStation, "��ע������������");
		return true;
	}

	if (m_gameConfig.allUserMaxNote > 0 && (llAllAreaBet + note) > m_gameConfig.allUserMaxNote)
	{
		SendGameMessage(deskStation, "��ע��������");
		return true;
	}

	//�����������ע
	if (pUser->isVirtual && !IsVirtualCanNote(deskStation, note))
	{
		return false;
	}

	//��������עֵ ��
	long long llDragonAreaBet = m_areaInfoArr[0].areaNote;
	//����������עֵ  ��
	long long llDrawAreaBet = m_areaInfoArr[1].areaNote;
	//��������עֵ ��
	long long llTigerAreaBet = m_areaInfoArr[2].areaNote;

	int iNoteArea = pMessage->area; //��ע����
	if (iNoteArea == 0)
	{
		llDragonAreaBet += note;
	}
	else if (iNoteArea == 1)
	{
		llDrawAreaBet += note;
	}
	else
	{
		llTigerAreaBet += note;
	}

	if (pMessage->area != 1)
	{
		//��ע���ܳ���ׯ��(��ϵͳ)��ׯ���
		if (abs(llDragonAreaBet - llTigerAreaBet) > m_dealerInfo.shangzhuangMoney)
		{
			SendGameMessage(deskStation, "��ע����ׯ������");
			return true;
		}
	}
	else
	{
		//��ע���ܳ���ׯ��(��ϵͳ)��ׯ���
		if (llDrawAreaBet * 8 > m_dealerInfo.shangzhuangMoney)
		{
			SendGameMessage(deskStation, "��ע����ׯ������");
			return true;
		}
	}

	DTUserInfo& userInfo = m_userInfoVec[deskStation];
	AreaInfo& areaInfo = m_areaInfoArr[pMessage->area];

	userInfo.isNote = true;
	userInfo.noteList[pMessage->area] += note;
	areaInfo.areaNote += note;

	//����������עֵ
	if (!pUser->isVirtual)
	{
		areaInfo.areaRealPeopleNote += note;
	}

	// ֪ͨ����Լ���ע�ɹ�
	for (int i = 0; i < MAX_AREA_COUNT; i++)
	{
		msg.noteList[i] = userInfo.noteList[i];
	}

	SendGameData(deskStation, &msg, sizeof(msg), MSG_MAIN_LOADER_GAME, MSG_GAME_RSP_NOTE, 0);

	//��¼����
	m_areaChouMaCount[pMessage->area][pMessage->noteIndex] ++;
	if (m_tempChouMaDataCount < MAX_CHOU_MA_COUNT && m_tempChouMaDataCount >= 0)
	{
		m_tempChouMaData[m_tempChouMaDataCount].deskStation = deskStation;
		m_tempChouMaData[m_tempChouMaDataCount].index = pMessage->noteIndex;
		m_tempChouMaData[m_tempChouMaDataCount].area = pMessage->area;
		m_tempChouMaData[m_tempChouMaDataCount].nums = note;
		m_tempChouMaDataCount++;
	}

	BrodcastNoteInfo();

	return true;
}

bool CNewServerManage::OnHandleRequestContinueNote(BYTE deskStation)
{
	if (m_gameStatus != GAME_STATUS_NOTE)
	{
		return false;
	}

	long userID = GetUserIDByDeskStation(deskStation);
	if (userID <= 0)
	{
		ERROR_LOG("invalid userID = 0");
		return false;
	}

	GameUserInfo* pUser = m_pDataManage->GetUser(userID);
	if (!pUser)
	{
		ERROR_LOG("GetUser failed userID=%d", userID);
		return false;
	}

	long long llUserResNums = 0;
	if (!GetRoomResNums(deskStation, llUserResNums))
	{
		ERROR_LOG("GetUser failed userID=%d", userID);
		return false;
	}

	//�Լ���ׯ�ң��޷���ѹ
	if (deskStation == m_dealerDeskStation)
	{
		SendGameMessage(deskStation, "ׯ�Ҳ�����ѹ", SMT_EJECT);
		return false;
	}

	// ��ע�˾Ͳ�����ѹ
	DTUserInfo& userInfo = m_userInfoVec[deskStation];
	if (userInfo.isNote == true)
	{
		SendGameMessage(deskStation, "�Ѿ���ע��������ѹ", SMT_EJECT);
		return false;
	}

	// �Ѿ���ѹ�˲����ٴ���ѹ
	if (userInfo.isContinueNote == true)
	{
		SendGameMessage(deskStation, "�Ѿ���ѹ����������ѹ", SMT_EJECT);
		return false;
	}

	//��������עֵ
	long long llAllAreaBet = 0;
	for (int i = 0; i < MAX_AREA_COUNT; i++)
	{
		llAllAreaBet += m_areaInfoArr[i].areaNote;
	}

	//�����ѹ����
	long long iAllbet = 0;
	for (int i = 0; i < MAX_AREA_COUNT; i++)
	{
		iAllbet += m_userInfoVec[deskStation].lastNoteList[i];
	}

	//�Ͼ�û����ע
	if (iAllbet <= 0)
	{
		SendGameMessage(deskStation, "��һ��û����ע��������ѹ", SMT_EJECT);
		return true;
	}

	//��ѹ���ܳ�����ǰ��ҵİٷ�֮10
	if (iAllbet > llUserResNums)
	{
		SendGameMessage(deskStation, "��ѹ���ܳ���������", SMT_EJECT);
		return true;
	}

	//��ע������ׯ��30%
	if ((llAllAreaBet + iAllbet) > m_dealerInfo.shangzhuangMoney * 3 / 10)
	{
		SendGameMessage(deskStation, "��ע����ׯ������", SMT_EJECT);
		return true;
	}

	if (m_gameConfig.allUserMaxNote > 0 && (llAllAreaBet + iAllbet) > m_gameConfig.allUserMaxNote)
	{
		SendGameMessage(deskStation, "��ע��������");
		return true;
	}

	userInfo.isContinueNote = true;
	userInfo.isNote = true;
	for (int i = 0; i < MAX_AREA_COUNT; i++)
	{
		userInfo.noteList[i] = userInfo.lastNoteList[i];
		m_areaInfoArr[i].areaNote += userInfo.lastNoteList[i];
		if (!pUser->isVirtual)
		{
			m_areaInfoArr[i].areaRealPeopleNote += userInfo.lastNoteList[i];
		}
	}

	NewGameMessageRspNote msg;

	for (int i = 0; i < MAX_AREA_COUNT; i++)
	{
		msg.noteList[i] = userInfo.noteList[i];
	}

	SendGameData(deskStation, &msg, sizeof(msg), MSG_MAIN_LOADER_GAME, MSG_GAME_RSP_CONTINUE_NOTE, 0);

	//��һ��Ҫ���ٸ�����
	for (int i = 0; i < MAX_AREA_COUNT; i++)
	{
		if (userInfo.noteList[i]>0)
		{
			long long noteNum = userInfo.noteList[i];
			int noteIndex = MAX_NOTE_SORT - 1;
			while (noteNum>0)
			{
				if (noteNum >= GetNoteByIndex(noteIndex))
				{
					noteNum -= GetNoteByIndex(noteIndex);
					//��¼����
					m_areaChouMaCount[i][noteIndex] ++;
					if (m_tempChouMaDataCount < MAX_CHOU_MA_COUNT && m_tempChouMaDataCount >= 0)
					{
						m_tempChouMaData[m_tempChouMaDataCount].deskStation = deskStation;
						m_tempChouMaData[m_tempChouMaDataCount].index = noteIndex;
						m_tempChouMaData[m_tempChouMaDataCount].area = i;
						m_tempChouMaData[m_tempChouMaDataCount].nums = GetNoteByIndex(noteIndex);
						m_tempChouMaDataCount++;
					}
					continue;
				}
				noteIndex--;
				if (noteIndex < 0) break;
			}
		}
	}




	BrodcastNoteInfo();

	return true;
}

bool CNewServerManage::OnHandleRequestAreaTrend(BYTE deskStation)
{
	NewGameMessageRspAreaTrend msg;

	if (m_allCount == 0)
	{
		msg.longWin = 100;
		msg.huWin = 100;
	}
	else
	{
		msg.longWin = m_longWinCount * 100 / m_allCount;
		msg.huWin = (m_allCount - m_longWinCount) * 100 / m_allCount;
	}

	int iCount = 0;

	for (auto itr = m_resultList.begin(); itr != m_resultList.end(); itr++)
	{
		msg.data[iCount].type = itr->type;
		msg.data[iCount].count = itr->count;
		iCount++;
	}

	iCount = 0;
	for (auto itr = m_resultSummaryList.begin(); itr != m_resultSummaryList.end(); itr++)
	{
		msg.result[iCount] = *itr;
		iCount++;
	}

	SendGameData(deskStation, &msg, sizeof(msg), MSG_MAIN_LOADER_GAME, MSG_GAME_RSP_AREA_TREND, 0);

	return true;
}

// ���󳬶�����
bool CNewServerManage::OnHandleRequestSuperSet(BYTE deskStation, void* pData, int size)
{
	if (size != sizeof(NewGameMessageReqSuperSet))
	{
		return false;
	}

	NewGameMessageReqSuperSet* pMessage = (NewGameMessageReqSuperSet*)pData;
	if (!pMessage)
	{
		return false;
	}

	GameUserInfo userData;
	if (!GetUserData(deskStation, userData))
	{
		return false;
	}

	if ((userData.userStatus&USER_IDENTITY_TYPE_SUPER) != USER_IDENTITY_TYPE_SUPER)
	{
		return false;
	}

	m_bySuperSetType = pMessage->bySuperSetType;

	SendGameMessage(deskStation, "���óɹ�", SMT_EJECT);

	return true;
}

// �����Ƿ������ׯ
bool CNewServerManage::OnHandleRequestContinueZhuang(BYTE deskStation, void* pData, int size)
{
	if (size != sizeof(NewGameMessageReqContinueZhuang))
	{
		return false;
	}

	NewGameMessageReqContinueZhuang* pMessage = (NewGameMessageReqContinueZhuang*)pData;
	if (!pMessage)
	{
		return false;
	}

	if (m_gameStatus != GAME_STATUS_WAITNEXT && m_gameStatus != GAME_STATUS_SETTLE)
	{
		SendGameMessage(deskStation, "��Ϣ�Ѿ���ʱ", SMT_EJECT);
		return false;
	}

	if (deskStation != m_dealerDeskStation)
	{
		SendGameMessage(deskStation, "����ׯ��", SMT_EJECT);
		return false;
	}


	m_iContinueZhuang = pMessage->byType;

	return true;
}

bool CNewServerManage::OnHandleRequestSit(BYTE deskStation, void* pData, int size)
{
	SAFECHECK_MESSAGE(pMessage, NewGameMessageReqSit, pData, size);

	long long llUserResNums = 0;
	if (!GetRoomResNums(deskStation, llUserResNums))
	{
		ERROR_LOG("GetUser failed userID=%d", GetUserIDByDeskStation(deskStation));
		return false;
	}
	//�����������������
	if (IsVirtual(deskStation) && !IsVirtualCanSit(deskStation, pMessage->type))
	{
		return false;
	}

	if (pMessage->type == 0) //����
	{
		// ���������жϣ�����Ƿ��㹻
		if (llUserResNums < m_gameConfig.sitLimitMoney)
		{
			char msg[128] = "";
			sprintf_s(msg, sizeof(msg), "���Ľ�Ҳ���%lld���޷�����", m_gameConfig.sitLimitMoney);
			SendGameMessage(deskStation, msg);
			return false;
		}

		if (deskStation == m_dealerDeskStation)
		{
			SendGameMessage(deskStation, "ׯ�Ҳ�������");
			return false;
		}

		if (IsDeskUser(deskStation))
		{
			SendGameMessage(deskStation, "���Ѿ�����λ��");
			return false;
		}

		if (IsDeskHaveUser(pMessage->index))
		{
			SendGameMessage(deskStation, "����λ�Ѿ�����");
			return false;
		}

		// ��ӵ���λ
		SetDeskUser(deskStation, pMessage->index);
	}
	else //վ��
	{
		if (!IsDeskUser(deskStation))
		{
			SendGameMessage(deskStation, "��û�����£�����վ��");
			return false;
		}

		// ɾ��λ��
		DelDeskUser(deskStation);
	}

	NewGameMessageRspSit msg;
	msg.index = pMessage->index;
	msg.result = 1;
	msg.type = pMessage->type;

	SendGameData(deskStation, &msg, sizeof(msg), MSG_MAIN_LOADER_GAME, MSG_GAME_RSP_SIT, 0);

	return true;
}

void CNewServerManage::OnTimerWaitNext()
{
	KillAllTimer();
	TransToWaitNextStage();
}

void CNewServerManage::OnTimerSendCard()
{
	KillAllTimer();
	TransToSendCardStage();
}

void CNewServerManage::OnTimerNote()
{
	KillAllTimer();
	TransToNoteStage();
}

void CNewServerManage::OnTimerSendNoteInfo()
{
	BrodcastNoteInfo();
}

void CNewServerManage::OnTimerCompare()
{
	KillAllTimer();
	TransToCompareStage();
}

void CNewServerManage::OnTimerSettle()
{
	KillAllTimer();
	TransToSettleStage();
}

long long CNewServerManage::GetNoteByIndex(int index)
{
	if (index < 0 || index >= MAX_NOTE_SORT)
	{
		return 0;
	}

	int roomLevel = GetRoomLevel() >= 1 && GetRoomLevel() <= 3 ? GetRoomLevel() - 1 : 0;

	return m_gameConfig.noteList[roomLevel][index];
}

BYTE CNewServerManage::PopCardFromDesk()
{
	if (m_deskCardQueue.empty())
	{
		// log todo
		return 0xff;
	}

	BYTE card = m_deskCardQueue.front();
	m_deskCardQueue.pop();

	return card;
}

bool CNewServerManage::SendAreaCards(BYTE* pCardList)
{
	if (!pCardList)
	{
		return false;
	}

	for (int i = 0; i < AREA_CARD_COUNT; i++)
	{
		BYTE card = PopCardFromDesk();
		if (card == 0xff)
		{
			ERROR_LOG("invalid card");
			return false;
		}

		pCardList[i] = card;
	}

	return true;
}

void CNewServerManage::KillAllTimer()
{
	for (unsigned int i = GAME_TIMER_DEFAULT + 1; i < GAME_TIMER_END; i++)
	{
		KillTimer(i);
	}
}

bool CNewServerManage::ConfirmDealer()
{
	bool bChangeZhuang = false;
	if (m_xiaZhuangRightNow)
	{
		bChangeZhuang = true;
	}
	else if (m_dealerDeskStation != SYSTEM_DEALER_DESKSTATION)
	{
		//��ׯ�б������Я����ׯ����������Ǹ�
		long long topDealerMoney = 0;
		if (!m_dealerInfoVec.empty())
		{
			for (auto it = m_dealerInfoVec.begin(); it != m_dealerInfoVec.end(); ++it)
			{
				topDealerMoney = (*it).shangzhuangMoney;
				break;
			}
		}

		//�����ׯЯ������������б��е���ҽ������ʱ����ׯ�����������ׯ
		if (m_currDealerGameCount >= m_gameConfig.maxZhuangGameCount && m_dealerInfo.shangzhuangMoney < topDealerMoney)
		{
			m_xiaZhuangRightNow = true;
			bChangeZhuang = true;
		}

		if (m_currDealerGameCount >= m_gameConfig.maxZhuangGameCount && m_dealerInfo.shangzhuangMoney >= topDealerMoney && m_iContinueZhuang != 1)
		{
			m_xiaZhuangRightNow = true;
			bChangeZhuang = true;
		}

		if (m_xiaZhuangRightNow == true)
		{
			m_currDealerGameCount = 0;
			m_dealerDeskStation = SYSTEM_DEALER_DESKSTATION;
		}
	}

	if (m_dealerDeskStation == INVALID_DESKSTATION || IsHundredGameSystemDealer())
	{
		// ��ʼ��Ϸ����ϵͳ��ׯ
		if (!m_dealerInfoVec.empty())
		{
			for (auto it = m_dealerInfoVec.begin(); it != m_dealerInfoVec.end(); ++it)
			{
				m_dealerInfo = *it;
				it = m_dealerInfoVec.erase(it);
				break;
			}
			if (m_dealerInfo.userID <= 0)
			{
				ERROR_LOG("m_shangZhuangList front an invalid userID userID=%d", m_dealerInfo.userID);
				return false;
			}

			BYTE deskStation = GetDeskStationByUserID(m_dealerInfo.userID);
			if (deskStation == INVALID_DESKSTATION)
			{
				ERROR_LOG("GetDeskStationByUserID failed userID=%d", m_dealerInfo.userID);
				return false;
			}

			m_dealerDeskStation = deskStation;
		}
		else
		{
			m_dealerDeskStation = SYSTEM_DEALER_DESKSTATION;
		}
	}
	else
	{
		if (!bChangeZhuang)
		{
			return true;
		}

		// ��һ��߻�������ׯ,Ҫ��һׯ����ׯ�󣬲����л�
		if (!m_dealerInfoVec.empty())
		{
			vector<DealerInfo>::iterator it;
			for (it = m_dealerInfoVec.begin(); it != m_dealerInfoVec.end(); ++it)
			{
				m_dealerInfo = *it;
				it = m_dealerInfoVec.erase(it);
				break;
			}

			if (m_dealerInfo.userID <= 0)
			{
				ERROR_LOG("m_shangZhuangList front an invalid userID userID=%d", m_dealerInfo.userID);
				return false;
			}

			BYTE deskStation = GetDeskStationByUserID(m_dealerInfo.userID);
			if (deskStation == INVALID_DESKSTATION)
			{
				ERROR_LOG("GetDeskStationByUserID failed userID=%d", m_dealerInfo.userID);
				return false;
			}
			m_dealerDeskStation = deskStation;
		}
		else
		{
			m_dealerDeskStation = SYSTEM_DEALER_DESKSTATION;
		}
	}

	std::sort(m_dealerInfoVec.begin(), m_dealerInfoVec.end(), SortShangZhuangByGold);

	return true;
}

time_t CNewServerManage::GetNextStageLeftSecs()
{
	time_t currTime = time(NULL);

	if (m_gameStatus == 0)
	{
		return 0;
	}
	else if (m_gameStatus == GAME_STATUS_WAITNEXT)
	{
		return m_startTime + m_gameConfig.waitBeginTime - currTime;
	}
	else if (m_gameStatus == GAME_STATUS_SENDCARD)
	{
		return m_startTime + m_gameConfig.waitBeginTime + m_gameConfig.sendCardKeepTime - currTime;
	}
	else if (m_gameStatus == GAME_STATUS_NOTE)
	{
		return m_startTime + m_gameConfig.waitBeginTime + m_gameConfig.sendCardKeepTime + m_gameConfig.noteKeepTime - currTime;
	}
	else if (m_gameStatus == GAME_STATUS_COMPARE)
	{
		return m_startTime + m_gameConfig.waitBeginTime + m_gameConfig.noteKeepTime + m_gameConfig.sendCardKeepTime + m_gameConfig.compareKeepTime - currTime;
	}
	else if (m_gameStatus == GAME_STATUS_SETTLE)
	{
		return m_startTime + m_gameConfig.waitBeginTime + m_gameConfig.noteKeepTime + m_gameConfig.sendCardKeepTime + m_gameConfig.compareKeepTime + m_gameConfig.waitSettleTime - currTime;
	}

	return 0;
}

void CNewServerManage::TransToWaitNextStage()
{
	KillAllTimer();
	m_gameStatus = GAME_STATUS_WAITNEXT;
	m_startTime = time(NULL);

	BroadcastGameStatus();

	// �ȿ�ʼ����
	SetTimer(GAME_TIMER_SENDCARD, (m_gameConfig.waitBeginTime + 1) * 1000);

	// ����ȷ��
	m_bySuperSetType = 0;
	memset(m_bIsSuper, 0, sizeof(m_bIsSuper));
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		long userID = GetUserIDByDeskStation(i);
		if (userID <= 0)
		{
			continue;
		}

		GameUserInfo * pUser = m_pDataManage->GetUser(userID);
		if (!pUser)
		{
			continue;
		}

		if ((pUser->userStatus&USER_IDENTITY_TYPE_SUPER) == USER_IDENTITY_TYPE_SUPER)
		{
			m_bIsSuper[i] = true;
		}
	}
}

void CNewServerManage::TransToSendCardStage()
{
	// ȷ��ׯ��
	if (ConfirmDealer() == false)
	{
		ERROR_LOG("ConfirmDealer failed");
		m_dealerDeskStation = SYSTEM_DEALER_DESKSTATION;
	}

	if (m_xiaZhuangRightNow == false && m_iContinueZhuang == 1)
	{
		m_currDealerGameCount = 0;
	}

	if (m_dealerDeskStation != m_LastDealerDeskStation)
	{
		m_currDealerGameCount = 0;
	}

	if (m_dealerDeskStation != m_LastDealerDeskStation)
	{
		m_currDealerGameCount = 0;
	}


	// ��ʼ��ׯ��ѡ��
	m_iContinueZhuang = -1;

	ChangeSystemZhuangInfo();

	if (m_dealerDeskStation != SYSTEM_DEALER_DESKSTATION)
	{
		m_currDealerGameCount++;
		DelDeskUser(m_dealerDeskStation);
	}

	GameBegin(0);
	m_xiaZhuangRightNow = false;

	// ֪ͨׯ��Ϣ
	BroadcastZhuangInfo();

	// ϴ��
	std::vector<BYTE> cardVec;
	m_logic.RandCard(cardVec, 1);

	std::queue<BYTE> tempQueue;
	m_deskCardQueue.swap(tempQueue);

	for (size_t i = 0; i < cardVec.size(); i++)
	{
		m_deskCardQueue.push(cardVec[i]);
	}

	// ��������� �� ��
	for (size_t areaIdx = 0; areaIdx < m_areaInfoArr.size(); areaIdx++)
	{
		if (areaIdx != 1)
		{
			AreaInfo& areaInfo = m_areaInfoArr[areaIdx];
			SendAreaCards(areaInfo.cardList);
		}
	}

	m_gameStatus = GAME_STATUS_SENDCARD;

	// ֪ͨǰ�˽׶α仯
	BroadcastGameStatus();

	// ��������ע
	SetTimer(GAME_TIMER_NOTE, m_gameConfig.sendCardKeepTime * 1000);
}

void CNewServerManage::TransToNoteStage()
{
	m_gameStatus = GAME_STATUS_NOTE;

	// ֪ͨ���������ע��ʼ
	BroadcastGameStatus();

	NewGameMessageHasXuYa msg;

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (GetUserIDByDeskStation(i) <= 0) continue;
		long long NoteData = 0;
		msg.bHasReuqestNote = false;
		for (int idx = 0; idx < MAX_AREA_COUNT; idx++)
		{
			NoteData += m_userInfoVec[i].lastNoteList[idx];
		}
		if (NoteData > 0)
		{
			msg.bHasReuqestNote = true;
		}
		SendGameData(i, &msg, sizeof(msg), MSG_MAIN_LOADER_GAME, MSG_GAME_NIF_XUYA, 0);
	}

	// ���÷�����ע��Ϣ��ʱ��
	//SetTimer(GAME_TIMER_NOTEINFO, 200);
	m_tempChouMaDataCount = 0;

	// ��ע�����
	SetTimer(GAME_TIMER_COMPARE, m_gameConfig.noteKeepTime * 1000);
}

void CNewServerManage::TransToCompareStage()
{
	KillTimer(GAME_TIMER_NOTEINFO);
	BrodcastNoteInfo();

	m_gameStatus = GAME_STATUS_COMPARE;

	if (!m_bySuperSetType)
	{
		//ִ�и��ʿ���
		AiWinAutoCtrl();
	}

	//ִ�г�������
	SuperSetChange();

	// ����������Ӯ��Ϣ
	CalcAreaRate();

	// �ٷ���һ������Ϣ
	// ���Ƶ���Ϣ���͸�����ǰ��
	NewGameMessageNtfSendCard msg;
	MakeSendCardInfo(msg);
	BroadcastDeskData(&msg, sizeof(msg), MSG_MAIN_LOADER_GAME, MSG_GAME_NTF_SENDCARD);

	// ֪ͨǰ�˽׶α仯
	BroadcastGameStatus();

	SetTimer(GAME_TIMER_SETTLE, m_gameConfig.compareKeepTime * 1000);
}

void CNewServerManage::TransToSettleStage()
{
	long dealerID = 0;
	if (m_dealerDeskStation != SYSTEM_DEALER_DESKSTATION)
	{
		dealerID = GetUserIDByDeskStation(m_dealerDeskStation);
		if (dealerID <= 0)
		{
			ERROR_LOG("invalid dealerID dealerID=%d, deskStation=%d��ׯ�Ҳ����ڣ�ׯ���Զ����ϵͳׯ��", dealerID, m_dealerDeskStation);

			// �ݴ���ׯ���Զ����ϵͳׯ��
			m_dealerDeskStation = SYSTEM_DEALER_DESKSTATION;
		}
	}

	m_gameStatus = GAME_STATUS_SETTLE;
	m_LastDealerDeskStation = m_dealerDeskStation;

	long long winMoneyList[PLAY_COUNT] = { 0 };
	memset(winMoneyList, 0, sizeof(winMoneyList));

	// ������Ӯ
	BYTE maxWinnerDeskStation = INVALID_DESKSTATION;
	long long maxWinMoney = 0;
	long long dealerWinMoney = 0;
	double shurate = double(10) / 100.0;
	for (size_t i = 0; i < m_userInfoVec.size(); i++)
	{
		DTUserInfo& userInfo = m_userInfoVec[i];

		//�����ϴ���ע��Ϣ
		for (int idx = 0; idx < MAX_AREA_COUNT; idx++)
		{
			userInfo.lastNoteList[idx] = userInfo.noteList[idx];
		}

		if (userInfo.isNote == false || i == m_dealerDeskStation)
		{
			// û��ע����Ϊׯ��
			continue;
		}

		long long winMoney = 0;
		if (m_currWinArea != 2)
		{
			for (size_t areaIdx = 0; areaIdx < m_areaInfoArr.size(); areaIdx++)
			{
				long long noteMoney = userInfo.noteList[areaIdx];
				if (noteMoney > 0)
				{
					//  ��ע��
					int areaRate = m_areaInfoArr[areaIdx].currWinRate;
					winMoney += noteMoney * areaRate;
				}
			}
		}
		else
		{
			//���ͣ���۳���������ע��10%����ȡ������8��
			long long tmpwinMoney0 = -userInfo.noteList[0] * m_areaInfoArr[0].currWinRate;
			long long tmpwinMoney2 = -userInfo.noteList[2] * m_areaInfoArr[2].currWinRate;
			winMoney -= (long long)floor((tmpwinMoney0 + tmpwinMoney2) * shurate);
			winMoney += userInfo.noteList[1] * m_areaInfoArr[1].currWinRate; //����ѹ�͵Ķ�ע��������Ӯ			
		}

		winMoneyList[i] = winMoney;
		dealerWinMoney -= winMoney;

		if (winMoney > maxWinMoney)
		{
			maxWinnerDeskStation = (BYTE)i;
			maxWinMoney = winMoney;
		}
	}

	// ׯ��
	if (m_dealerDeskStation != SYSTEM_DEALER_DESKSTATION)
	{
		if (m_dealerDeskStation < m_userInfoVec.size())
		{
			winMoneyList[m_dealerDeskStation] = dealerWinMoney;
		}
	}

	///////////////////////////////�����ˮ///////////////////////////////////////////
	double rate = double(m_gameConfig.taxRate) / 100.0;
	long long llRateMoney[PLAY_COUNT] = { 0 };
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		long long taxValue = 0;
		if (winMoneyList[i] == 0)
		{
			continue;
		}
		if (winMoneyList[i] > 0)
		{
			long long i64WinMoney = winMoneyList[i]; //��¼���Ӯ��Ǯ
			taxValue = (long long)floor(rate * i64WinMoney);
			i64WinMoney -= taxValue;
			llRateMoney[i] += taxValue; //�ۼӳ�ˮ�����
			winMoneyList[i] = i64WinMoney; //���Ӯ��Ǯ���۳�����ˮ���
		}
	}

	if (m_dealerDeskStation == SYSTEM_DEALER_DESKSTATION)
	{
		long long taxValue = 0;
		if (dealerWinMoney > 0)
		{
			taxValue = (long long)floor(rate * dealerWinMoney);
			dealerWinMoney -= taxValue;
		}
	}

	// ��ұ仯
	ChangeUserPoint(winMoneyList, NULL, llRateMoney);

	// ������������ϢTODO
	NewGameMessageNtfSettle msg;
	if (m_dealerDeskStation != SYSTEM_DEALER_DESKSTATION)
	{
		msg.dealerWinMoney = winMoneyList[m_dealerDeskStation];
		m_dealerInfo.shangzhuangMoney += winMoneyList[m_dealerDeskStation];
	}
	else
	{
		msg.dealerWinMoney = dealerWinMoney;
		m_dealerInfo.shangzhuangMoney += dealerWinMoney;
	}
	msg.dealerMoney = m_dealerInfo.shangzhuangMoney;  //��¼ׯ��Я�������
	if (maxWinnerDeskStation != INVALID_DESKSTATION)
	{
		long maxWinnerID = GetUserIDByDeskStation(maxWinnerDeskStation);
		if (maxWinnerID > 0)
		{
			GameUserInfo* pUser = m_pDataManage->GetUser(maxWinnerID);
			if (pUser)
			{
				msg.maxWinnerInfo.userID = pUser->userID;
				memcpy(msg.maxWinnerInfo.name, pUser->name, sizeof(msg.maxWinnerInfo.name));
				memcpy(msg.maxWinnerInfo.headURL, pUser->headURL, sizeof(msg.maxWinnerInfo.headURL));
				msg.maxWinnerInfo.winMoney = maxWinMoney;
			}
		}
	}

	// ��ȡ������ҵ���Ӯ����Լ����
	for (size_t i = 0; i < m_gameDesk.size(); i++)
	{
		BYTE sitDeskStation = m_gameDesk[i];
		if (sitDeskStation >= PLAY_COUNT)
		{
			continue;
		}

		msg.sitUserWinMoney[i] = winMoneyList[sitDeskStation];
		GameUserInfo* pUser = m_pDataManage->GetUser(GetUserIDByDeskStation(sitDeskStation));
		if (pUser)
		{
			msg.sitUserMoney[i] = pUser->money;
		}
	}

	for (BYTE i = 0; i < PLAY_COUNT; i++)
	{
		long userID = GetUserIDByDeskStation(i);
		if (userID > 0)
		{
			if (m_dealerDeskStation == i && m_currDealerGameCount >= m_gameConfig.maxZhuangGameCount && m_dealerInfo.shangzhuangMoney >= m_gameConfig.shangZhuangLimitMoney)
			{
				msg.isContinueZhuang = 1;
			}
			else
			{
				msg.isContinueZhuang = 0;
			}
			msg.myWinMoney = winMoneyList[i];
			SendGameData(i, &msg, sizeof(msg), MSG_MAIN_LOADER_GAME, MSG_GAME_NTF_SETTLE, 0);
		}
	}

	// ���ýӿ�
	HundredGameFinish();

	m_LastShangzhuangMoney = m_dealerInfo.shangzhuangMoney; //��ׯЯ����������
	// ��ǰׯ�һ��ܷ������ׯ
	if (m_dealerDeskStation != SYSTEM_DEALER_DESKSTATION)
	{
		GameUserInfo* pDealer = m_pDataManage->GetUser(dealerID);
		if (!pDealer)
		{
			ERROR_LOG("GetUser failed dealerID=%d", dealerID);
			m_xiaZhuangRightNow = true;
		}

		if (pDealer && pDealer->IsOnline == false)
		{
			m_xiaZhuangRightNow = true;
		}

		////��ׯ�б������Я����ׯ����������Ǹ�
		//long long topDealerMoney = 0;
		//if (!m_dealerInfoVec.empty())
		//{
		//	for (auto it = m_dealerInfoVec.begin(); it != m_dealerInfoVec.end(); ++it)
		//	{
		//		topDealerMoney = (*it).shangzhuangMoney;
		//		break;
		//	}
		//}

		////�����ׯЯ������������б��е���ҽ������ʱ����ׯ�����������ׯ
		//if (m_currDealerGameCount >= m_gameConfig.maxZhuangGameCount && m_dealerInfo.shangzhuangMoney < topDealerMoney)
		//{
		//	m_xiaZhuangRightNow = true;
		//}

		//��ׯЯ�������С�����ޣ��Զ���ׯ
		if (m_dealerInfo.shangzhuangMoney < m_gameConfig.shangZhuangLimitMoney)
		{
			m_xiaZhuangRightNow = true;
		}
	}

	if (m_xiaZhuangRightNow == true)
	{
		m_currDealerGameCount = 0;
		m_dealerDeskStation = SYSTEM_DEALER_DESKSTATION;
	}

	ReSetGameState(0);

	BroadcastGameStatus();

	// ֪ͨׯ��Ϣ
	//BroadcastZhuangInfo();

	//ÿ�ֿ��������Ļ�������
	UpSitPlayerCount();

	// �ȴ��¾ֽ׶�
	SetTimer(GAME_TIMER_WAITNEXT, m_gameConfig.waitSettleTime * 1000);
}

void CNewServerManage::BroadcastGameStatus()
{
	NewGameMessageNtfGameStatus msg;

	msg.gameStatus = m_gameStatus;

	BroadcastDeskData(&msg, sizeof(msg), MSG_MAIN_LOADER_GAME, MSG_GAME_NTF_GAMESTATUS);
}

void CNewServerManage::BroadcastZhuangInfo()
{
	NewGameMessageNtfZhuangInfo msg;
	//��ǰׯ�ҵ���Ϣ
	MakeZhuangInfo(msg);
	//��������ȡׯ���б��е�ׯ����Ϣ
	int i = 0;
	auto iter = m_dealerInfoVec.begin();
	for (; iter != m_dealerInfoVec.end(); iter++, i++)
	{
		long userID = iter->userID;
		GameUserInfo* pDealer = m_pDataManage->GetUser(userID);
		if (pDealer)
		{
			msg.shangZhuangList[i].userID = pDealer->userID;
			msg.shangZhuangList[i].money = pDealer->money;
			memcpy(msg.shangZhuangList[i].name, pDealer->name, sizeof(msg.zhuangInfo.name));
			memcpy(msg.shangZhuangList[i].headURL, pDealer->headURL, sizeof(msg.zhuangInfo.headURL));
			msg.shangZhuangList[i].shangzhuangMoney = iter->shangzhuangMoney;
		}
	}
	BroadcastDeskData(&msg, sizeof(msg), MSG_MAIN_LOADER_GAME, MSG_GAME_NTF_ZHUANGINFO);
}

void CNewServerManage::BrodcastNoteInfo(BYTE deskStation)
{
	NewGameMessageNtfNoteInfo msg;

	memcpy(msg.chouMaData, m_tempChouMaData, sizeof(msg.chouMaData));
	msg.chouMaDataCount = m_tempChouMaDataCount;

	int iSendSize = sizeof(msg.noteList) + sizeof(msg.chouMaDataCount) +
		msg.chouMaDataCount * sizeof(ChouMaInfo);

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (GetUserIDByDeskStation(i) <= 0)
		{
			continue;
		}

		if (m_bIsSuper[i])
		{
			for (size_t i = 0; i < m_areaInfoArr.size(); i++)
			{
				msg.noteList[i] = m_areaInfoArr[i].areaRealPeopleNote;
			}
		}
		else
		{
			for (size_t i = 0; i < m_areaInfoArr.size(); i++)
			{
				msg.noteList[i] = m_areaInfoArr[i].areaNote;
			}
		}

		SendGameData(i, &msg, iSendSize, MSG_MAIN_LOADER_GAME, MSG_GAME_NTF_NOTEINFO, 0);
	}

	// �����
	m_tempChouMaDataCount = 0;
}

bool CNewServerManage::MakeZhuangInfo(NewGameMessageNtfZhuangInfo& msg)
{
	// ׯ����Ϣ
	long dealerID = GetUserIDByDeskStation(m_dealerDeskStation);
	if (dealerID > 0)
	{
		GameUserInfo* pDealer = m_pDataManage->GetUser(dealerID);
		if (pDealer)
		{
			msg.isSystemZhuang = false;
			msg.zhuangInfo.shangzhuangMoney = m_dealerInfo.shangzhuangMoney;
			msg.zhuangInfo.userID = pDealer->userID;
			msg.zhuangInfo.money = m_dealerInfo.shangzhuangMoney;
			memcpy(msg.zhuangInfo.name, pDealer->name, sizeof(msg.zhuangInfo.name));
			memcpy(msg.zhuangInfo.headURL, pDealer->headURL, sizeof(msg.zhuangInfo.headURL));
		}
	}
	else
	{
		// ϵͳ��ׯ
		msg.isSystemZhuang = true;
		msg.zhuangInfo.money = m_dealerInfo.shangzhuangMoney;
		strcpy(msg.zhuangInfo.name, "ϵͳ��ׯ");
		strcpy(msg.zhuangInfo.headURL, "aa");
		msg.zhuangInfo.shangzhuangMoney = m_dealerInfo.shangzhuangMoney;
	}

	msg.curZhuangPlayCount = m_currDealerGameCount;
	msg.allZhuangPlayCount = m_gameConfig.maxZhuangGameCount;

	return true;
}

bool CNewServerManage::MakeSendCardInfo(NewGameMessageNtfSendCard& msg)
{
	//������ע�Ժ��״̬��������
	if (m_gameStatus == GAME_STATUS_NOTE || m_gameStatus == GAME_STATUS_SENDCARD)
	{
		return true;
	}

	for (size_t i = 0; i < m_areaInfoArr.size(); i++)
	{
		const AreaInfo& areaInfo = m_areaInfoArr[i];

		memcpy(msg.areasCardList[i], areaInfo.cardList, sizeof(msg.areasCardList[i]));

		if (areaInfo.currWinRate > 0)
		{
			msg.isWin[i] = true;
		}
	}

	return true;
}

void CNewServerManage::CalcAreaRate()
{
	if (m_areaInfoArr.size() >= 3)
	{
		//�������Ƶ���
		int isWin = m_logic.CompareCardValue(m_areaInfoArr[0].cardList, AREA_CARD_COUNT, m_areaInfoArr[2].cardList, AREA_CARD_COUNT);
		m_currWinArea = isWin;
		if (0 == isWin)  //��Ӯ
		{
			SetResult(1);
			for (size_t i = 0; i < m_areaInfoArr.size(); i++)
			{
				if (i == 2)
				{
					m_areaInfoArr[i].currWinRate = 1;
				}
				else
				{
					m_areaInfoArr[i].currWinRate = -1;
				}
			}
		}
		else if (1 == isWin) //��Ӯ
		{
			SetResult(0);
			for (size_t i = 0; i < m_areaInfoArr.size(); i++)
			{
				if (i == 0)
				{
					m_areaInfoArr[i].currWinRate = 1;
				}
				else
				{
					m_areaInfoArr[i].currWinRate = -1;
				}
			}
		}
		else if (2 == isWin) //��
		{
			SetResult(2);
			for (size_t i = 0; i < m_areaInfoArr.size(); i++)
			{
				if (i == 1)
				{
					m_areaInfoArr[i].currWinRate = 8;
				}
				else
				{
					m_areaInfoArr[i].currWinRate = -1;
				}
			}
		}
	}
}

int CNewServerManage::GetRandNum()
{
	if (g_iRandNum >= 1000)
	{
		g_iRandNum = 0;
	}
	srand((unsigned)time(NULL) + g_iRandNum); rand();
	g_iRandNum++;
	return rand();
}

void CNewServerManage::SetResult(int result)
{
	m_allCount++;
	if (result == 0)
	{
		m_longWinCount++;
	}

	m_resultSummaryList.push_front(result);
	if (m_resultSummaryList.size() > MAX_AREA_TREND_COUNT)
	{
		m_resultSummaryList.pop_back();
	}

	if (m_resultList.size() >= MAX_AREA_TREND_COUNT)
	{
		m_resultList.pop_back();
	}

	if (m_resultList.size() == 0)
	{
		m_resultList.push_back(TrendData(result, 1));
	}
	else
	{
		TrendData &data = m_resultList.front();
		if (data.type == result)
		{
			data.count++;
		}
		else
		{
			m_resultList.push_front(TrendData(result, 1));
		}
	}
}

bool CNewServerManage::IsHundredGameSystemDealer()
{
	if (m_dealerDeskStation != SYSTEM_DEALER_DESKSTATION)
	{
		return false;
	}
	return true;
}

bool CNewServerManage::HundredGameIsInfoChange(BYTE deskStation)
{
	if (deskStation == m_dealerDeskStation)
	{
		return false;
	}

	long userID = GetUserIDByDeskStation(deskStation);

	for (auto it = m_dealerInfoVec.begin(); it != m_dealerInfoVec.end(); ++it)
	{
		if (it->userID == userID)
		{
			return false;
		}
		if (IsDeskUser(deskStation))
		{
			return false;
		}
	}

	return true;
}

void CNewServerManage::UserBeKicked(BYTE deskStation)
{
	if (deskStation >= m_userInfoVec.size())
	{
		ERROR_LOG("user deskStation is invalid deskStation=%d", deskStation);
		return;
	}

	long userID = GetUserIDByDeskStation(deskStation);
	if (userID <= 0)
	{
		return;
	}

	bool bKickShangZhuangUser = false;

	// ���Ϊׯ
	if (deskStation == m_dealerDeskStation)
	{
		ERROR_LOG("ׯ�ұ���");
	}
	else
	{
		for (auto it = m_dealerInfoVec.begin(); it != m_dealerInfoVec.end(); ++it)
		{
			if (it->userID == userID)
			{
				it = m_dealerInfoVec.erase(it);
				bKickShangZhuangUser = true;
				break;
			}
		}
	}

	if (bKickShangZhuangUser)
	{
		std::sort(m_dealerInfoVec.begin(), m_dealerInfoVec.end(), SortShangZhuangByGold);
		BroadcastZhuangInfo();
	}

	DTUserInfo& userInfo = m_userInfoVec[deskStation];
	userInfo.Clear();

	// ɾ����λ��
	DelDeskUser(deskStation);
}

bool CNewServerManage::IsPlayGame(BYTE deskStation)
{
	if (deskStation >= m_userInfoVec.size())
	{
		return false;
	}
	DTUserInfo& userInfo = m_userInfoVec[deskStation];
	if (userInfo.isNote == false)
	{
		//ׯ��
		if (deskStation == m_dealerDeskStation)
		{
			return true;
		}

		return false;
	}

	return true;
}

// ��̬���������ļ�����
void CNewServerManage::LoadDynamicConfig()
{
	//////////////////////////////////��������////////////////////////////////////////
	CString nameID;
	nameID.Format("%d", NAME_ID);
	CINIFile f(CINIFile::GetAppPath() + nameID + "_s.ini");
	CString key = TEXT("game");
	m_gameConfig.waitBeginTime = f.GetKeyVal(key, "waitBeginTime", 5);
	m_gameConfig.noteKeepTime = f.GetKeyVal(key, "noteKeepTime", 15);
	m_gameConfig.sendCardKeepTime = f.GetKeyVal(key, "sendCardKeepTime", 2);
	m_gameConfig.compareKeepTime = f.GetKeyVal(key, "compareKeepTime", 8);
	m_gameConfig.waitSettleTime = f.GetKeyVal(key, "waitSettleTime", 3);
	m_gameConfig.maxZhuangGameCount = f.GetKeyVal(key, "maxZhuangGameCount", 5);
	m_gameConfig.taxRate = f.GetKeyVal(key, "taxRate", 5);
	m_gameConfig.randBankerCount = f.GetKeyVal(key, "randBankerCount", 1);
	m_gameConfig.randCount = f.GetKeyVal(key, "randCount", 60);
	m_gameConfig.CanSitCount = f.GetKeyVal(key, "CanSitCount", 4);
	if (m_gameConfig.CanSitCount < 0 || m_gameConfig.CanSitCount > 10)
	{
		m_gameConfig.CanSitCount = 4;
	}
	if (m_gameConfig.randCount <= 0 || m_gameConfig.randCount > 100)
	{
		m_gameConfig.randCount = 60;
	}

	//������Ϸ�������жϻ�������������
	m_gameConfig.GamePlayCount_1 = f.GetKeyVal(key, "GamePlayCount_1", 3);
	m_gameConfig.GamePlayCount_2 = f.GetKeyVal(key, "GamePlayCount_2", 5);
	m_gameConfig.GamePlayCount_3 = f.GetKeyVal(key, "GamePlayCount_3", 10);
	if (m_gameConfig.GamePlayCount_1 < 0 || m_gameConfig.GamePlayCount_1 >= MAX_PLAYER_GRADE)
	{
		m_gameConfig.GamePlayCount_1 = 3;
	}
	if (m_gameConfig.GamePlayCount_2 <= m_gameConfig.GamePlayCount_1 || m_gameConfig.GamePlayCount_2 >= MAX_PLAYER_GRADE)
	{
		m_gameConfig.GamePlayCount_1 = 3;
		m_gameConfig.GamePlayCount_2 = 5;
	}
	if (m_gameConfig.GamePlayCount_3 <= m_gameConfig.GamePlayCount_2 || m_gameConfig.GamePlayCount_2 >= MAX_PLAYER_GRADE && m_gameConfig.GamePlayCount_3 >= MAX_PLAYER_GRADE)
	{
		m_gameConfig.GamePlayCount_1 = 3;
		m_gameConfig.GamePlayCount_2 = 5;
		m_gameConfig.GamePlayCount_3 = 10;
	}

	//����������λ
	int maxDeskCount = f.GetKeyVal(key, "maxDeskCount", 6);
	if (maxDeskCount != m_gameConfig.maxDeskCount)
	{
		m_gameConfig.maxDeskCount = maxDeskCount;
		m_gameDesk.resize(m_gameConfig.maxDeskCount);
		for (size_t i = 0; i < m_gameDesk.size(); i++)
		{
			m_gameDesk[i] = 255;
		}
	}

	//���ݳ��и߽��м�������
	if (GetRoomLevel() == 1)
	{
		m_gameConfig.shangZhuangLimitMoney = f.GetKeyVal(key, "shangZhuangLimitMoney1", 200000)* GetPlatformMultiple();
		m_gameConfig.sitLimitMoney = f.GetKeyVal(key, "sitLimitMoney1", 5000)* GetPlatformMultiple();
		m_gameConfig.noteList[0][0] = f.GetKeyVal(key, "noteList_1_0", 100)* GetPlatformMultiple();
		m_gameConfig.noteList[0][1] = f.GetKeyVal(key, "noteList_1_1", 200)* GetPlatformMultiple();
		m_gameConfig.noteList[0][2] = f.GetKeyVal(key, "noteList_1_2", 500)* GetPlatformMultiple();
		m_gameConfig.noteList[0][3] = f.GetKeyVal(key, "noteList_1_3", 1000)* GetPlatformMultiple();
		m_gameConfig.noteList[0][4] = f.GetKeyVal(key, "noteList_1_4", 2000)* GetPlatformMultiple();
		m_gameConfig.noteList[0][5] = f.GetKeyVal(key, "noteList_1_5", 4000)* GetPlatformMultiple();
		m_gameConfig.allUserMaxNote = f.GetKeyVal(key, "allUserMaxNote1", 2000000)* GetPlatformMultiple();
	}
	else if (GetRoomLevel() == 2)
	{
		m_gameConfig.shangZhuangLimitMoney = f.GetKeyVal(key, "shangZhuangLimitMoney2", 500000)* GetPlatformMultiple();
		m_gameConfig.sitLimitMoney = f.GetKeyVal(key, "sitLimitMoney2", 5000)* GetPlatformMultiple();
		m_gameConfig.noteList[1][0] = f.GetKeyVal(key, "noteList_2_0", 1000)* GetPlatformMultiple();
		m_gameConfig.noteList[1][1] = f.GetKeyVal(key, "noteList_2_1", 5000)* GetPlatformMultiple();
		m_gameConfig.noteList[1][2] = f.GetKeyVal(key, "noteList_2_2", 10000)* GetPlatformMultiple();
		m_gameConfig.noteList[1][3] = f.GetKeyVal(key, "noteList_2_3", 20000)* GetPlatformMultiple();
		m_gameConfig.noteList[1][4] = f.GetKeyVal(key, "noteList_2_4", 50000)* GetPlatformMultiple();
		m_gameConfig.noteList[1][5] = f.GetKeyVal(key, "noteList_2_5", 100000)* GetPlatformMultiple();
		m_gameConfig.allUserMaxNote = f.GetKeyVal(key, "allUserMaxNote2", 2000000)* GetPlatformMultiple();
	}
	else if (GetRoomLevel() == 3)
	{
		m_gameConfig.shangZhuangLimitMoney = f.GetKeyVal(key, "shangZhuangLimitMoney3", 1000000)* GetPlatformMultiple();
		m_gameConfig.sitLimitMoney = f.GetKeyVal(key, "sitLimitMoney3", 5000)* GetPlatformMultiple();
		m_gameConfig.noteList[2][0] = f.GetKeyVal(key, "noteList_3_0", 5000)* GetPlatformMultiple();
		m_gameConfig.noteList[2][1] = f.GetKeyVal(key, "noteList_3_1", 10000)* GetPlatformMultiple();
		m_gameConfig.noteList[2][2] = f.GetKeyVal(key, "noteList_3_2", 100000)* GetPlatformMultiple();
		m_gameConfig.noteList[2][3] = f.GetKeyVal(key, "noteList_3_3", 500000)* GetPlatformMultiple();
		m_gameConfig.noteList[2][4] = f.GetKeyVal(key, "noteList_3_4", 1000000)* GetPlatformMultiple();
		m_gameConfig.noteList[2][5] = f.GetKeyVal(key, "noteList_3_5", 2000000)* GetPlatformMultiple();
		m_gameConfig.allUserMaxNote = f.GetKeyVal(key, "allUserMaxNote3", 2000000)* GetPlatformMultiple();
	}

	////////////////////////////////���ⷿ������//////////////////////////////////////////
	TCHAR szKey[20];
	wsprintf(szKey, "%s_%d", nameID, m_pDataManage->m_InitData.uRoomID);
	key = szKey;
	m_gameConfig.waitBeginTime = f.GetKeyVal(key, "waitBeginTime", m_gameConfig.waitBeginTime);


	//����������λ
	m_gameDesk.resize(m_gameConfig.maxDeskCount);
	for (size_t i = 0; i < m_gameDesk.size(); i++)
	{
		m_gameDesk[i] = 255;
	}
}

void CNewServerManage::AiWinAutoCtrl()
{
	if (!m_pDataManage->LoadPoolData())
	{
		return;
	}

	int iCtrlType = GetCtrlRet();

	m_runCtrlCount++;

	if (iCtrlType == 0)
	{
		return;
	}

	if (iCtrlType == 1)
	{
		m_curCtrlWinCount++;
	}

	int iRateValue = 1;

	if (IsHundredGameSystemDealer())
	{
		iRateValue = 1;
	}
	else
	{
		long dealerID = GetUserIDByDeskStation(m_dealerDeskStation);
		if (dealerID <= 0)
		{
			ERROR_LOG("����ʧ�ܣ�ׯ����λ�Ų�����,m_dealerDeskStation = %d", m_dealerDeskStation);
			return;
		}

		GameUserInfo* pDealer = m_pDataManage->GetUser(dealerID);
		if (!pDealer)
		{
			ERROR_LOG("����ʧ�ܣ�ׯ����λ�Ų�����,dealerID = %d", dealerID);
			return;
		}

		if (pDealer->isVirtual) //��������ׯ
		{
			iRateValue = 1;
		}
		else
		{
			iRateValue = -1;
		}
	}

	if (iCtrlType == 1)
	{
		if (CalcSystemWinMoney(iRateValue) >= 0)
		{
			return;
		}
	}
	else
	{
		if (CalcSystemWinMoney(iRateValue) <= 0)
		{
			return;
		}
	}

	for (int iCount = 0; iCount < 5; iCount++)
	{
		BYTE maxCardA = m_logic.GetMaxCardByNumber(m_areaInfoArr[0].cardList, AREA_CARD_COUNT);
		if (m_logic.GetCardBulk(maxCardA) == 1)
		{
			SendAreaCards(m_areaInfoArr[0].cardList);
		}
		else
		{
			break;
		}
	}

	m_countResultCardVec.clear();

	for (size_t i = 0; i < m_deskCardQueue.size(); i++)
	{
		// �ı仢������
		SendAreaCards(m_areaInfoArr[2].cardList);

		if (iCtrlType == 1 && CalcSystemWinMoney(iRateValue) >= 0 || iCtrlType == -1 && CalcSystemWinMoney(iRateValue) <= 0)
		{
			m_countResultCardVec.push_back(m_areaInfoArr[2].cardList[0]);
		}
	}

	if (m_countResultCardVec.size() > 0)
	{
		m_areaInfoArr[2].cardList[0] = m_countResultCardVec[CUtil::GetRandNum() % m_countResultCardVec.size()];
	}
}

int CNewServerManage::GetCtrlRet()
{
	// ���ո���ִ��
	if (m_pDataManage->m_rewardsPoolInfo.platformCtrlType == 1)
	{
		if (CUtil::GetRandNum() % 1000 < abs(m_pDataManage->m_rewardsPoolInfo.platformCtrlPercent))
		{
			if (m_pDataManage->m_rewardsPoolInfo.platformCtrlPercent > 0)
			{
				return 1;
			}
			else
			{
				return -1;
			}
		}

		return 0;
	}

	int pArray[20] = { 0 };
	int iArrayCount = 0;

	if (!m_pDataManage->GetPoolConfigInfoString("ctrl", pArray, 20, iArrayCount))
	{
		return 0;
	}

	if (iArrayCount <= 0)
	{
		return 0;
	}

	if (iArrayCount % 2 != 0)
	{
		ERROR_LOG("json���ô���");
		return 0;
	}

	// �жϲ����Ƿ�ı�
	char strTemp[256] = "";
	for (int i = 0; i < iArrayCount; i++)
	{
		sprintf(strTemp + strlen(strTemp), "%d,", pArray[i]);
	}
	if (strcmp(strTemp, m_szLastCtrlParam))
	{
		m_curCtrlWinCount = 0;
		m_runCtrlCount = 0;
		m_ctrlParam.clear();
		memcpy(m_szLastCtrlParam, strTemp, sizeof(m_szLastCtrlParam));

		for (int i = 0; i < iArrayCount; i += 2)
		{
			CtrlSystemParam param;
			param.winCount = pArray[i];
			param.allCount = pArray[i + 1];
			if (param.allCount <= 0)
			{
				ERROR_LOG("json���ô����ܾ�������С��0");
				return 0;
			}
			m_ctrlParam.push_back(param);
		}
	}

	if (m_ctrlParam.size() <= 0)
	{
		return 0;
	}

	// �׾�
	if (m_runCtrlCount == 0)
	{
		m_ctrlParamIndex = CUtil::GetRandNum() % m_ctrlParam.size();
	}

	// ���ο������ˣ�������������������
	if (m_runCtrlCount >= m_ctrlParam[m_ctrlParamIndex].allCount)
	{
		m_ctrlParamIndex = CUtil::GetRandNum() % m_ctrlParam.size();
		m_curCtrlWinCount = 0;
		m_runCtrlCount = 0;
	}

	// ��ֵ����ƽ̨
	char paramBuf[128] = "";
	sprintf(paramBuf, "[�ܹ�%d��,ҪӮ%d��],Ŀǰ��[��%d��,�Ѿ�Ӯ%d��]", m_ctrlParam[m_ctrlParamIndex].allCount,
		m_ctrlParam[m_ctrlParamIndex].winCount, m_runCtrlCount, m_curCtrlWinCount);
	m_ctrlParmRecordInfo = paramBuf;

	// ���ݺ�̨����ֵ�������㷨�ж�
	if (m_ctrlParam[m_ctrlParamIndex].winCount < 0) //ȫ��
	{
		return -1;
	}

	if (m_ctrlParam[m_ctrlParamIndex].winCount == 0) //������
	{
		return 0;
	}

	if (m_ctrlParam[m_ctrlParamIndex].winCount >= m_ctrlParam[m_ctrlParamIndex].allCount) //ȫӮ
	{
		return 1;
	}

	if (m_curCtrlWinCount >= m_ctrlParam[m_ctrlParamIndex].winCount) //Ӯ�������������
	{
		return -1;
	}

	if (CUtil::GetRandNum() % (m_ctrlParam[m_ctrlParamIndex].allCount - m_runCtrlCount) < (m_ctrlParam[m_ctrlParamIndex].winCount - m_curCtrlWinCount))
	{
		return 1;
	}
	else
	{
		return -1;
	}

	return 0;
}

//���㵱ǰϵͳӮǮ
long long CNewServerManage::CalcSystemWinMoney(int iRateValue)
{
	long long winMoney = 0;

	//�������Ƶ���
	int isWin = m_logic.CompareCardValue(m_areaInfoArr[0].cardList, AREA_CARD_COUNT, m_areaInfoArr[2].cardList, AREA_CARD_COUNT);

	if (iRateValue == 1)
	{
		if (0 == isWin)  //��Ӯ
		{
			winMoney += m_areaInfoArr[0].areaRealPeopleNote;
			winMoney += m_areaInfoArr[1].areaRealPeopleNote;
			winMoney -= m_areaInfoArr[2].areaRealPeopleNote;
		}
		else if (1 == isWin) //��Ӯ
		{
			winMoney -= m_areaInfoArr[0].areaRealPeopleNote;
			winMoney += m_areaInfoArr[1].areaRealPeopleNote;
			winMoney += m_areaInfoArr[2].areaRealPeopleNote;
		}
		else if (2 == isWin) //��
		{
			winMoney += m_areaInfoArr[0].areaRealPeopleNote;
			winMoney -= 8 * m_areaInfoArr[1].areaRealPeopleNote;
			winMoney += m_areaInfoArr[2].areaRealPeopleNote;
		}
	}
	else
	{
		if (0 == isWin)  //��Ӯ
		{
			winMoney += (m_areaInfoArr[0].areaNote - m_areaInfoArr[0].areaRealPeopleNote);
			winMoney += (m_areaInfoArr[1].areaNote - m_areaInfoArr[1].areaRealPeopleNote);
			winMoney -= (m_areaInfoArr[2].areaNote - m_areaInfoArr[2].areaRealPeopleNote);
		}
		else if (1 == isWin) //��Ӯ
		{
			winMoney -= (m_areaInfoArr[0].areaNote - m_areaInfoArr[0].areaRealPeopleNote);
			winMoney += (m_areaInfoArr[1].areaNote - m_areaInfoArr[1].areaRealPeopleNote);
			winMoney += (m_areaInfoArr[2].areaNote - m_areaInfoArr[2].areaRealPeopleNote);
		}
		else if (2 == isWin) //��
		{
			winMoney += (m_areaInfoArr[0].areaNote - m_areaInfoArr[0].areaRealPeopleNote);
			winMoney -= 8 * (m_areaInfoArr[1].areaNote - m_areaInfoArr[1].areaRealPeopleNote);
			winMoney += (m_areaInfoArr[2].areaNote - m_areaInfoArr[2].areaRealPeopleNote);
		}

		winMoney = -winMoney;
	}

	return winMoney;
}

// ����ϵͳ������Դ
void CNewServerManage::ChangeSystemZhuangInfo()
{
	// ׯ����Ϣ
	if (m_dealerDeskStation == SYSTEM_DEALER_DESKSTATION)
	{
		m_dealerInfo.isSystem = true;
		// ϵͳ��ׯ
		if (m_dealerDeskStation != m_LastDealerDeskStation)
		{
			m_dealerInfo.shangzhuangMoney = CUtil::GetRandRange((int)m_gameConfig.shangZhuangLimitMoney, 9 * (int)m_gameConfig.shangZhuangLimitMoney);
		}
		else
		{
			if (m_dealerInfo.shangzhuangMoney < m_gameConfig.shangZhuangLimitMoney)
			{
				m_dealerInfo.shangzhuangMoney = CUtil::GetRandRange((int)m_gameConfig.shangZhuangLimitMoney, 9 * (int)m_gameConfig.shangZhuangLimitMoney);
			}
			else
			{
				m_dealerInfo.shangzhuangMoney = m_LastShangzhuangMoney;
			}
		}
	}
}

//ִ�г�������
void CNewServerManage::SuperSetChange()
{
	if (!m_bySuperSetType)
	{
		return;
	}

	//�������Ƶ���
	int isWin = m_logic.CompareCardValue(m_areaInfoArr[0].cardList, AREA_CARD_COUNT, m_areaInfoArr[2].cardList, AREA_CARD_COUNT);

	if (0 == isWin && m_bySuperSetType == 2)  //��Ӯ
	{
		return;
	}
	else if (1 == isWin && m_bySuperSetType == 1) //��Ӯ
	{
		return;
	}
	else if (2 == isWin && m_bySuperSetType == 3) //��
	{
		return;
	}

	for (int iCount = 0; iCount < 5; iCount++)
	{
		BYTE maxCardA = m_logic.GetMaxCardByNumber(m_areaInfoArr[0].cardList, AREA_CARD_COUNT);
		if (m_logic.GetCardBulk(maxCardA) == 1)
		{
			SendAreaCards(m_areaInfoArr[0].cardList);
		}
		else
		{
			break;
		}
	}

	for (size_t i = 0; i < m_deskCardQueue.size(); i++)
	{
		//�ı仢������
		SendAreaCards(m_areaInfoArr[2].cardList);

		//�������Ƶ���
		int isWin = m_logic.CompareCardValue(m_areaInfoArr[0].cardList, AREA_CARD_COUNT, m_areaInfoArr[2].cardList, AREA_CARD_COUNT);

		if (0 == isWin && m_bySuperSetType == 2)  //��Ӯ
		{
			return;
		}
		else if (1 == isWin && m_bySuperSetType == 1) //��Ӯ
		{
			return;
		}
		else if (2 == isWin && m_bySuperSetType == 3) //��
		{
			return;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
bool CNewServerManage::IsDeskUser(BYTE deskStation)
{
	if (deskStation == 255)
	{
		return false;
	}

	for (size_t i = 0; i < m_gameDesk.size(); i++)
	{
		if (m_gameDesk[i] == deskStation)
		{
			return true;
		}
	}

	return false;
}

bool CNewServerManage::DelDeskUser(BYTE deskStation)
{
	if (deskStation == 255)
	{
		return false;
	}

	int iIndex = -1;
	for (size_t i = 0; i < m_gameDesk.size(); i++)
	{
		if (m_gameDesk[i] == deskStation)
		{
			iIndex = (int)i;
			break;
		}
	}

	if (iIndex == -1)
	{
		return false;
	}

	m_gameDesk[iIndex] = 255;

	// ������Ϣ
	NewGameMessageNtfUserSit msg;
	msg.deskStation = deskStation;
	msg.index = iIndex;
	msg.type = 1;
	for (size_t i = 0; i < m_gameDesk.size(); i++)
	{
		msg.deskUser[i] = m_gameDesk[i];
	}

	BroadcastDeskData(&msg, sizeof(msg), MSG_MAIN_LOADER_GAME, MSG_GAME_NTF_SIT);

	return true;
}

bool CNewServerManage::SetDeskUser(BYTE deskStation, BYTE byIndex)
{
	if (deskStation == 255 || byIndex >= m_gameDesk.size())
	{
		return false;
	}

	m_gameDesk[byIndex] = deskStation;

	// ������Ϣ
	NewGameMessageNtfUserSit msg;
	msg.deskStation = deskStation;
	msg.index = byIndex;
	msg.type = 0;
	for (size_t i = 0; i < m_gameDesk.size(); i++)
	{
		msg.deskUser[i] = m_gameDesk[i];
	}

	BroadcastDeskData(&msg, sizeof(msg), MSG_MAIN_LOADER_GAME, MSG_GAME_NTF_SIT);

	return true;
}

bool CNewServerManage::IsDeskHaveUser(BYTE byIndex)
{
	if (byIndex >= m_gameDesk.size())
	{
		return false;
	}

	if (m_gameDesk[byIndex] != 255)
	{
		return true;
	}

	return false;
}

void CNewServerManage::ClearDeskUser()
{
	for (size_t i = 0; i < m_gameDesk.size(); i++)
	{
		if (m_gameDesk[i] == 255)
		{
			continue;
		}
		long userID = GetUserIDByDeskStation(m_gameDesk[i]);
		if (userID <= 0)
		{
			DelDeskUser(m_gameDesk[i]);
			continue;
		}

		if (m_pDataManage->GetUser(userID) == NULL)
		{
			DelDeskUser(m_gameDesk[i]);
		}
	}
}

//������������
void CNewServerManage::UpSitPlayerCount()
{
	BYTE UserCount = GetRealPeople();
	if (UserCount > 0 && UserCount <= m_gameConfig.GamePlayCount_1)
	{
		m_gameConfig.CanSitCount = 4;
	}
	else if (UserCount > m_gameConfig.GamePlayCount_1 && UserCount <= m_gameConfig.GamePlayCount_2)
	{
		m_gameConfig.CanSitCount = 2;
	}
	else if (UserCount > m_gameConfig.GamePlayCount_3)
	{
		m_gameConfig.CanSitCount = 1;
	}
}

//////////////////////////////////////////////////////////////////////////
// �жϻ������Ƿ��������ׯ  0����ׯ   1����ׯ
bool CNewServerManage::IsVirtualCanShangXiazZhuang(BYTE deskStation, BYTE type)
{
	int iShangZhuangRobotCount = 0;
	for (auto it = m_dealerInfoVec.begin(); it != m_dealerInfoVec.end(); it++)
	{
		GameUserInfo * pUser = m_pDataManage->GetUser(it->userID);
		if (pUser && pUser->isVirtual)
		{
			iShangZhuangRobotCount++;
		}
	}

	int iMinCount = m_pDataManage->GetPoolConfigInfo("zMinC");
	int iMaxCount = m_pDataManage->GetPoolConfigInfo("zMaxC") + 1;

	if (type == 0) // ��ׯ
	{
		int iRandCount = CUtil::GetRandRange(iMinCount, iMaxCount);
		if (iRandCount <= 0)
		{
			iRandCount = CUtil::GetRandRange(1, 4);
		}

		if (iShangZhuangRobotCount >= iRandCount)
		{
			return false;
		}
	}
	else // ��ׯ
	{
		if (iShangZhuangRobotCount <= iMinCount)
		{
			return false;
		}
	}

	return true;
}

bool CNewServerManage::IsVirtualCanNote(BYTE deskStation, long long note)
{
	// �������������ע
	long long llAllRobotNote = 0;
	for (int i = 0; i < MAX_AREA_COUNT; i++)
	{
		llAllRobotNote += m_areaInfoArr[i].areaNote - m_areaInfoArr[i].areaRealPeopleNote;
	}

	// ��ȡռ��
	int iNoteRate = m_pDataManage->GetPoolConfigInfo("noteRate");
	iNoteRate = iNoteRate <= 0 ? 3 : iNoteRate;

	if (llAllRobotNote + note > m_LastShangzhuangMoney * iNoteRate / 100)
	{
		return false;
	}

	return true;
}

// �жϻ������Ƿ����������վ��  0������   1��վ��
bool CNewServerManage::IsVirtualCanSit(BYTE deskStation, BYTE type)
{
	int iSitRobotCount = 0;

	for (size_t i = 0; i < m_gameDesk.size(); i++)
	{
		if (IsVirtual(m_gameDesk[i]))
		{
			iSitRobotCount++;
		}
	}

	int iMinCount = m_pDataManage->GetPoolConfigInfo("sMinC");
	int iMaxCount = m_pDataManage->GetPoolConfigInfo("sMaxC") + 1;

	if (type == 0) // ����
	{
		int iRandCount = CUtil::GetRandRange(iMinCount, iMaxCount);
		if (iRandCount <= 0)
		{
			iRandCount = CUtil::GetRandRange(1, 4);
		}

		if (iSitRobotCount >= iRandCount)
		{
			return false;
		}
	}
	else // վ��
	{
		if (iSitRobotCount <= iMinCount)
		{
			return false;
		}
	}

	return true;
}

