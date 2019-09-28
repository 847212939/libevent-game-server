#include "stdafx.h"
#include "GameDataEx.h"

GameDataEx::GameDataEx(void)
{
	GameData::GameData();
	//memset(m_byDingQue, 255, sizeof(m_byDingQue));
	//memset(m_iGangScore, 0, sizeof(m_iGangScore));
	memset(m_bIsHu, false, sizeof(m_bIsHu));
	memset(m_bQiHu, 0, sizeof(m_bQiHu));//����Ƿ�����״̬
	m_iHuangZhuangCount = 0;
	LoadIni();
	InitData();
}

GameDataEx::~GameDataEx(void)
{
}

//virtual ��ʼ������
void GameDataEx::InitData()
{
	GameData::InitData();
	//memset(m_byDingQue, 255, sizeof(m_byDingQue));
	memset(m_iGangScore, 0, sizeof(m_iGangScore));
	memset(m_bIsHu, false, sizeof(m_bIsHu));
	memset(m_bQiHu, 0, sizeof(m_bQiHu));//����Ƿ�����״̬
	memset(m_byOutCanHuCard, 255, sizeof(m_byOutCanHuCard));
	memset(m_byHuCard, 255, sizeof(m_byHuCard));
	memset(m_byOutAfterHuCard, 255, sizeof(m_byOutAfterHuCard));
	memset(m_byCardRemainNum, 0, sizeof(m_byCardRemainNum));

	m_byCurrentOperatorUser = 255;	//��ǰ�ɲ��������ܵ����
	//���ֲ���
	for (int i = 0;i < PLAY_COUNT;++i)
	{
		m_bIsBuHua[i] = false;
		m_bIsBuAllHua[i] = false;
		m_vHuapaiTemp[i].clear();
		m_vecQiPengCard[i].clear();
	}
	memset(m_byFangWei, 255, sizeof(m_byFangWei));
	memset(m_byHuaPaiType, 255, sizeof(m_byHuaPaiType));
	memset(m_bIsGangShangGang, 0, sizeof(m_bIsGangShangGang));
	m_byNowOutStation = 255;
	m_tMatchMJ.Init();
	m_byQiHuTurn = 255;
	m_byPengTurn = 255;
	memset(m_bQiPeng, false, sizeof(m_bQiPeng));
	memset(m_iHuaScore, 0, sizeof(m_iHuaScore)); //����
	for (int i = 0;i < PLAY_COUNT;++i)
	{
		m_countHuFen[i].Init();     //����
		m_countGangFen[i].Init();   //�ܷ�
		//m_countGenZhuang[i].Init(); //��ׯ��
		//m_countHorseFen[i].Init();  //���
	}
}

///��ʼ������
void GameDataEx::LoadIni()
{
	GameData::LoadIni();
}

///����Ƿ����ָ��������
bool GameDataEx::IsHaveAMenPai(BYTE pai)
{
	for (int i = 0;i < MAX_MJ_PAI_COUNT;i++)
	{
		if (m_MenPai.byMenPai[i] != 255 && m_MenPai.byMenPai[i] == pai)
			return true;
	}
	return false;
}

///�õ�ĳ�ֻ�ɫ�ƣ�0�� 1Ͳ 2��)
int GameDataEx::GetAKindPai(BYTE station, BYTE kind)
{
	kind %= 3;
	int count = 0;
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (m_byArHandPai[station][i] / 10 == kind)
			return m_byArHandPai[station][i];
	}
	return 0;
}

///���ĳ�ֻ�ɫ�Ƶĸ�����0�� 1Ͳ 2��)
int GameDataEx::GetAKindPaiCount(BYTE station, BYTE kind)
{
	kind %= 3;
	int count = 0;
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (m_byArHandPai[station][i] == 255)
			continue;
		if (m_byArHandPai[station][i] / 10 == kind)
			count++;
	}
	return count;
}

//ChangePai����
//byNewCard����
//�������л�����һ����
//��ָ����һ������
bool	GameDataEx::ChangeAHandPai(BYTE station, BYTE ChangePai, BYTE byNewCard)
{
	if (station < 0 || station >= PLAY_COUNT)
	{
		return false;
	}
	if (0 == ChangePai || 255 == ChangePai || 0 == byNewCard || 255 == byNewCard)
	{
		return false;
	}
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (m_byArHandPai[station][i] == ChangePai && ChangeAMenPai(byNewCard, ChangePai))
		{
			m_byArHandPai[station][i] = byNewCard;
			return true;
		}
	}
	return false;
}

//��ָ��һ������
bool	GameDataEx::ChangeAMenPai(BYTE ChangePai, BYTE byNewCard)
{
	if (0 == ChangePai || 255 == ChangePai || 0 == byNewCard || 255 == byNewCard)
	{
		return false;
	}
	for (int i = 0;i < MAX_MJ_PAI_COUNT;i++)
	{
		if (m_MenPai.byMenPai[i] != 255 && m_MenPai.byMenPai[i] == ChangePai)
		{
			m_MenPai.byMenPai[i] = byNewCard;
			return true;
		}
	}
	return false;
}

///���ƣ�IsOutPaiPeople
///�������Ƿ�������
///@param 
///@return  true �� ,false ����
bool GameDataEx::IsOutPaiPeople(BYTE station)
{
	if (m_byNowOutStation < 0 || m_byNowOutStation >= PLAY_COUNT)
	{
		return false;
	}
	if (m_byArHandPaiCount[station] % 3 != 2)
	{
		return false;
	}
	return m_byNowOutStation == station ? true : false;
}

///��ȡ���Ƶ���ɫ��
int GameDataEx::GetPaiSeCount(BYTE station)
{
	int count = 0;
	bool se[3];
	memset(se, 0, sizeof(se));
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (m_byArHandPai[station][i] < 0 || m_byArHandPai[station][i] > CMjEnum::MJ_TYPE_B9)
			continue;
		se[m_byArHandPai[station][i] / 10] = true;
	}
	for (int i = 0;i < 3;i++)
	{
		if (se[i])
			count++;
	}
	return count;
}
///�Ƿ���ȱ�ŵ���
bool GameDataEx::IsHaveQueMen(BYTE station, BYTE type)
{
	if (type > 2 || type < 0)
		return false;
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (m_byArHandPai[station][i] < 0 || m_byArHandPai[station][i] > CMjEnum::MJ_TYPE_B9)
			continue;
		if (m_byArHandPai[station][i] / 10 == type)
			return true;
	}
	return false;
}
//��ȡһ��data��û�е��ƣ���������
BYTE GameDataEx::GetChangePai(BYTE station, BYTE data[])
{
	BYTE pai = 255;
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		bool have = false;
		for (int j = 0;j < HAND_CARD_NUM;++j)
		{
			if (data[j] == m_byArHandPai[station][i])
				have = true;//���������д��ڸ��ƣ�������ȥ�滻
		}
		if (!have)
		{
			return m_byArHandPai[station][i];
		}
	}
	return pai;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///���ƣ�AddToGCP
///���������һ�����ݵ��ܳ���������
///@param gcpData Ҫ��ӵĳ���������
///@return 
void GameDataEx::AddToGCP(BYTE station, TCPGStruct *gcpData)
{
	//GameData::AddToGCP(station,gcpData);
	for (int i = 0;i < 5;i++)
	{
		if (ACTION_NO == m_UserGCPData[station][i].byType)
		{
			m_UserGCPData[station][i].byType = gcpData->byType;
			m_UserGCPData[station][i].iBeStation = gcpData->iBeStation;
			m_UserGCPData[station][i].iOutpai = gcpData->iOutpai;
			m_UserGCPData[station][i].iStation = gcpData->iStation;
			for (int j = 0;j < 4;j++)
			{
				m_UserGCPData[station][i].byData[j] = gcpData->byData[j];
			}
			break;
		}
	}
}

/////////////////////���Ʋ���////////////////////////////////////////////////////////
///���ĳ�����Ƿ����Ż���
bool GameDataEx::CheckIsHaveHuaPai(BYTE station, BYTE pai)
{
	for (int i = 0; i < HUA_CARD_NUM; i++)
	{
		if (pai != 255 && m_byArHuaPai[station][i] == pai)
		{
			return true;
		}
	}
	return false;
}

//������
bool GameDataEx::DisPatchCard()
{
	BYTE index = 0;
	memset(m_MenPai.byMenPai, 255, sizeof(m_MenPai.byMenPai));
	if (m_mjRule.bHaveWan)
	{
		for (int i = 0;i < 9;i++)
		{
			for (int j = 0;j < 4;j++)
			{
				m_MenPai.byMenPai[index] = stcArr_A_Mj_W[i];
				index++;
			}
		}
	}
	if (m_mjRule.bHaveTiao)
	{
		for (int i = 0;i < 9;i++)
		{
			for (int j = 0;j < 4;j++)
			{
				m_MenPai.byMenPai[index] = stcArr_A_Mj_T[i];
				index++;
			}
		}
	}
	if (m_mjRule.bHaveBing)
	{
		for (int i = 0;i < 9;i++)
		{
			for (int j = 0;j < 4;j++)
			{
				m_MenPai.byMenPai[index] = stcArr_A_Mj_B[i];
				index++;
			}
		}
	}
	if (m_mjRule.bHaveHongZhong)
	{
		for (int i = 0;i < 4;i++)
		{
			m_MenPai.byMenPai[index] = CMjEnum::MJ_TYPE_ZHONG;
			index++;
		}
	}
	if (m_mjRule.bHaveFaCai)
	{
		for (int i = 0;i < 4;i++)
		{
			m_MenPai.byMenPai[index] = CMjEnum::MJ_TYPE_FA;
			index++;
		}
	}
	if (m_mjRule.bHaveBaiBan)
	{
		for (int i = 0;i < 4;i++)
		{
			m_MenPai.byMenPai[index] = CMjEnum::MJ_TYPE_BAI;
			index++;
		}
	}
	if (m_mjRule.bHaveFengPai)
	{
		for (int i = 0;i < 4;i++)
		{
			for (int b = 0;b < 4;b++)
			{
				m_MenPai.byMenPai[index] = stcArr_A_Mj_FENG[i];;
				index++;
			}
		}
	}
	if (m_mjRule.bHaveFlower)
	{
		for (int i = 0;i < 8;i++)
		{
			m_MenPai.byMenPai[index] = stcArr_A_Mj_Other[i];
			index++;
		}
	}
	if (m_mjRule.byAllMjCount != index)
	{
		return false;
	}
	///������
	srand(GetTickCount());
	BYTE temp = 255, data = 0;
	int iCount = 2;
	while (--iCount)
	{
		for (int i = 0;i < index;i++)
		{
			temp = rand() % (index - i) + i;
			data = m_MenPai.byMenPai[temp];
			m_MenPai.byMenPai[temp] = m_MenPai.byMenPai[i];
			m_MenPai.byMenPai[i] = data;
		}
	}

	//���ݴ����������ȷ��ץ���ƿ�ʼ�����ͽ�������
	//���ݲ�ͬ���齫��ͬ
	//4����Ϊһ��
	int num = 0;
	for (int i = 0; i < 4; i++)
	{
		if (i >= m_MenPai.byGetPaiDir)
		{
			break;
		}
		num += m_mjRule.byMenPaiNum[i];
	}
	num += m_MenPai.byGetPai * 4;
	if (num >= index)
	{
		num = num - index;
	}
	m_MenPai.byStartIndex = num; //������ʼ����
	m_MenPai.byAllPaiNum = index;//�����Ƶ�����
	m_MenPai.byPaiNum = index;   //ʣ����������
	int iVirtualIndex = m_MenPai.byStartIndex;
	if (iVirtualIndex == 0)
	{
		iVirtualIndex = m_MenPai.byAllPaiNum - 1;
	}
	else
	{
		--iVirtualIndex;
	}
	m_MenPai.byEndIndex = iVirtualIndex;
	return true;
}
//������ץһ����
BYTE GameDataEx::GetPai(int iStation, bool bhead, BYTE *pIndex, BYTE byGetPai)
{
	BYTE paiIndex = 255;//��ץ�Ƶ�λ��
	BYTE card = 255;
	if (bhead)//��ǰץ
	{
		if (m_MenPai.byStartIndex >= m_MenPai.byAllPaiNum)
		{
			m_MenPai.byStartIndex = 0;
		}

		if (byGetPai != 255) //Ҫץָ���ƣ�
		{
			bool bIsOk = false;
			int iIndex = 0;
			for (int i = 0; i < m_MenPai.byAllPaiNum; i++)
			{
				if (m_MenPai.byMenPai[i] == byGetPai)
				{
					bIsOk = true;
					iIndex = i;
					break;
				}
			}
			if (bIsOk) //�ҵ��˵���λ��
			{
				BYTE byTemp = m_MenPai.byMenPai[m_MenPai.byStartIndex];
				m_MenPai.byMenPai[m_MenPai.byStartIndex] = m_MenPai.byMenPai[iIndex];
				m_MenPai.byMenPai[iIndex] = byTemp;
			}
		}

		paiIndex = m_MenPai.byStartIndex;//��ץ�Ƶ�λ��
		card = m_MenPai.byMenPai[m_MenPai.byStartIndex];
		m_MenPai.byMenPai[m_MenPai.byStartIndex] = 255;
		m_MenPai.byStartIndex++;
	}
	else//�Ӻ���ץ
	{
		if (byGetPai != 255) //Ҫץָ���ƣ�
		{
			bool bIsOk = false;
			int iIndex = 0;
			for (int i = 0; i < m_MenPai.byAllPaiNum; i++)
			{
				if (m_MenPai.byMenPai[i] == byGetPai)
				{
					bIsOk = true;
					iIndex = i;
					break;
				}
			}
			if (bIsOk) //�ҵ��˵���λ��
			{
				BYTE byTemp = m_MenPai.byMenPai[m_MenPai.byEndIndex];
				m_MenPai.byMenPai[m_MenPai.byEndIndex] = m_MenPai.byMenPai[iIndex];
				m_MenPai.byMenPai[iIndex] = byTemp;
			}
		}

		paiIndex = m_MenPai.byEndIndex;//��ץ�Ƶ�λ��
		card = m_MenPai.byMenPai[m_MenPai.byEndIndex];
		m_MenPai.byMenPai[m_MenPai.byEndIndex] = 255;
		if (m_MenPai.byEndIndex == 0)
		{
			m_MenPai.byEndIndex = m_MenPai.byAllPaiNum - 1;
		}
		else
		{
			m_MenPai.byEndIndex--;
		}
	}
	if (iStation != 255)
	{
		AddToHandPai(iStation, card);
	}
	if (pIndex != NULL)
	{
		*pIndex = paiIndex;//��ץ�Ƶ�λ��
	}
	return card;
}
BYTE GameDataEx::GetNextStation(BYTE station, bool shun)
{
	BYTE re = 255;
	if (shun)//˳ʱ��
	{
		re = (station + 1) % PLAY_COUNT;
	}
	else
	{
		re = (station + PLAY_COUNT - 1) % PLAY_COUNT;
	}
	return re;
}
//����
BYTE GameDataEx::MatchMJ()
{
	CString lw;
	if (!m_mjRule.bIsMatchMJ)
	{
		return 255;
	}

	m_tMatchMJ.byArHandPaiCount[0] = 14;//ׯ��
	m_tMatchMJ.byArHandPaiCount[1] = 13;
	m_tMatchMJ.byArHandPaiCount[2] = 13;
	m_tMatchMJ.byArHandPaiCount[3] = 13;

	//	m_GameData.m_byArHandPai[0][0]=2;
	//	m_GameData.m_byArHandPai[0][1]=5;
	//	m_GameData.m_byArHandPai[0][2]=9;
	//	m_GameData.m_byArHandPai[0][3]=15;
	//	m_GameData.m_byArHandPai[0][4]=21;
	//	m_GameData.m_byArHandPai[0][5]=21;
	//	m_GameData.m_byArHandPai[0][6]=22;
	//	m_GameData.m_byArHandPai[0][7]=25;
	//	m_GameData.m_byArHandPai[0][8]=26;
	//	m_GameData.m_byArHandPai[0][9]=31;
	//	m_GameData.m_byArHandPai[0][10]=34;
	//	m_GameData.m_byArHandPai[0][11]=31;
	//	m_GameData.m_byArHandPai[0][12]=36;
	//	m_GameData.m_byArHandPaiCount[0]=13;


	m_tMatchMJ.byArHandPai[0][0] = 1;
	m_tMatchMJ.byArHandPai[0][1] = 1;
	m_tMatchMJ.byArHandPai[0][2] = 1;
	m_tMatchMJ.byArHandPai[0][3] = 1;
	m_tMatchMJ.byArHandPai[0][4] = 2;
	m_tMatchMJ.byArHandPai[0][5] = 2;
	m_tMatchMJ.byArHandPai[0][6] = 2;
	m_tMatchMJ.byArHandPai[0][7] = 3;
	m_tMatchMJ.byArHandPai[0][8] = 3;
	m_tMatchMJ.byArHandPai[0][9] = 3;
	m_tMatchMJ.byArHandPai[0][10] = 34;
	m_tMatchMJ.byArHandPai[0][11] = 31;
	m_tMatchMJ.byArHandPai[0][12] = 36;
	m_tMatchMJ.byArHandPai[0][13] = 33;

	m_tMatchMJ.byArHandPai[1][0] = 32;
	m_tMatchMJ.byArHandPai[1][1] = 32;
	m_tMatchMJ.byArHandPai[1][2] = 32;
	m_tMatchMJ.byArHandPai[1][3] = 12;
	m_tMatchMJ.byArHandPai[1][4] = 14;
	m_tMatchMJ.byArHandPai[1][5] = 14;
	m_tMatchMJ.byArHandPai[1][6] = 15;
	m_tMatchMJ.byArHandPai[1][7] = 15;
	m_tMatchMJ.byArHandPai[1][8] = 17;
	m_tMatchMJ.byArHandPai[1][9] = 17;
	m_tMatchMJ.byArHandPai[1][10] = 19;
	m_tMatchMJ.byArHandPai[1][11] = 19;
	m_tMatchMJ.byArHandPai[1][12] = 33;

	m_tMatchMJ.byArHandPai[2][0] = 1;
	m_tMatchMJ.byArHandPai[2][1] = 2;
	m_tMatchMJ.byArHandPai[2][2] = 6;
	m_tMatchMJ.byArHandPai[2][3] = 7;
	m_tMatchMJ.byArHandPai[2][4] = 8;
	m_tMatchMJ.byArHandPai[2][5] = 23;
	m_tMatchMJ.byArHandPai[2][6] = 23;
	m_tMatchMJ.byArHandPai[2][7] = 16;
	m_tMatchMJ.byArHandPai[2][8] = 17;
	m_tMatchMJ.byArHandPai[2][9] = 18;
	m_tMatchMJ.byArHandPai[2][10] = 24;
	m_tMatchMJ.byArHandPai[2][11] = 25;
	m_tMatchMJ.byArHandPai[2][12] = 26;

	m_tMatchMJ.byArHandPai[3][0] = 1;
	m_tMatchMJ.byArHandPai[3][1] = 17;
	m_tMatchMJ.byArHandPai[3][2] = 18;
	m_tMatchMJ.byArHandPai[3][3] = 24;
	m_tMatchMJ.byArHandPai[3][4] = 25;
	m_tMatchMJ.byArHandPai[3][5] = 26;
	m_tMatchMJ.byArHandPai[3][6] = 14;
	m_tMatchMJ.byArHandPai[3][7] = 16;
	m_tMatchMJ.byArHandPai[3][8] = 29;
	m_tMatchMJ.byArHandPai[3][9] = 3;
	m_tMatchMJ.byArHandPai[3][10] = 3;
	m_tMatchMJ.byArHandPai[3][11] = 23;
	m_tMatchMJ.byArHandPai[3][12] = 24;
	int iLastIndex = m_MenPai.byStartIndex - 1;
	if (iLastIndex < 0)
	{
		iLastIndex = m_MenPai.byAllPaiNum - 1;
	}
	int iGetPaiNum = 0;
	for (int i = 0;i < PLAY_COUNT;i++)
	{
		if (0 == i)
		{
			for (int b = 0;b < 14;b++)
			{
				for (int c = 0;c < MAX_MJ_PAI_COUNT;c++)
				{
					if (m_tMatchMJ.byArHandPai[i][b] == m_MenPai.byMenPai[c])
					{
						m_MenPai.byMenPai[c] = 255;
						m_MenPai.byStartIndex++;
						break;
					}
				}
			}
		}
		else
		{
			for (int b = 0;b < 13;b++)
			{
				for (int c = 0;c < MAX_MJ_PAI_COUNT;c++)
				{
					if (m_tMatchMJ.byArHandPai[i][b] == m_MenPai.byMenPai[c])
					{
						m_MenPai.byMenPai[c] = 255;
						m_MenPai.byStartIndex++;
						break;
					}
				}
			}
		}
	}
	int index = 0;
	BYTE byTempMenPai[MAX_MJ_PAI_COUNT];
	memset(byTempMenPai, 255, sizeof(byTempMenPai));
	for (int i = 0;i < MAX_MJ_PAI_COUNT;i++)
	{
		if (255 != m_MenPai.byMenPai[i])
		{
			byTempMenPai[index] = m_MenPai.byMenPai[i];
			index++;
		}
	}
	memset(m_MenPai.byMenPai, 255, sizeof(m_MenPai.byMenPai));
	int iStartInddex = m_MenPai.byStartIndex;
	for (int i = 0;i < index;i++)
	{
		if (iStartInddex >= m_MenPai.byAllPaiNum)
		{
			iStartInddex = 0;
		}
		m_MenPai.byMenPai[iStartInddex] = byTempMenPai[i];
		iStartInddex++;
	}
	memcpy(m_byArHandPai[m_byNtStation], m_tMatchMJ.byArHandPai[0], sizeof(m_byArHandPai[m_byNtStation]));
	m_byArHandPaiCount[m_byNtStation] = 14;
	int copyindex = 1;
	for (int i = 0;i < PLAY_COUNT;i++)
	{
		if (m_byNtStation == i)
		{
			continue;
		}
		memcpy(m_byArHandPai[i], m_tMatchMJ.byArHandPai[copyindex], sizeof(m_byArHandPai[i]));
		m_byArHandPaiCount[i] = 13;
		copyindex++;
	}
	for (int i = 0;i < PLAY_COUNT;i++)
	{
		SortHandPai(i, false);
	}
	return m_tMatchMJ.byArHandPai[0][13];
}

//����
void GameDataEx::TurningString(BYTE &byPai0, BYTE &byPai1)
{
	BYTE byCard = GetPai(255, true, NULL);
	byPai0 = byCard;
	bool bFlag = false;
	if (CMjEnum::MJ_TYPE_W9 == byCard)
	{
		byCard = CMjEnum::MJ_TYPE_W1;
		bFlag = true;
	}
	if (CMjEnum::MJ_TYPE_T9 == byCard)
	{
		byCard = CMjEnum::MJ_TYPE_T1;
		bFlag = true;
	}
	if (CMjEnum::MJ_TYPE_B9 == byCard)
	{
		byCard = CMjEnum::MJ_TYPE_B1;
		bFlag = true;
	}
	if (CMjEnum::MJ_TYPE_BAI == byCard)
	{
		byCard = CMjEnum::MJ_TYPE_FD;
		bFlag = true;
	}
	if (!bFlag)
	{
		byCard = byCard + 1;
	}
	byPai1 = byCard;
}

//�ж��Ƿ������ׯ����
int GameDataEx::CheckIsShouZhangGen(bool bIsNext)
{
	int cout_ = 0;
	int index0 = 0, index1 = 0, index2 = 0, index3 = 0;
	for (int i = 0;i < OUT_CARD_NUM;i++)
	{
		int j = 0;
		for (j = index0; j < index0 + HUA_CARD_NUM;)
		{
			if (CheckIsHuaPai(m_byArOutPai[0][j]))
				j++;
			else
				break;
		}
		index0 = j;

		for (j = index1; j < index1 + HUA_CARD_NUM;)
		{
			if (CheckIsHuaPai(m_byArOutPai[1][j]))
				j++;
			else
				break;
		}
		index1 = j;

		for (j = index2; j < index2 + HUA_CARD_NUM;)
		{
			if (CheckIsHuaPai(m_byArOutPai[2][j]))
				j++;
			else
				break;
		}
		index2 = j;

		for (j = index3; j < index3 + HUA_CARD_NUM;)
		{
			if (CheckIsHuaPai(m_byArOutPai[3][j]))
				j++;
			else
				break;
		}
		index3 = j;
		if (m_byArOutPai[0][index0] == 255 || m_byArOutPai[1][index1] == 255 ||
			m_byArOutPai[2][index2] == 255 || m_byArOutPai[3][index3] == 255)
		{
			break;
		}

		if (m_byArOutPai[0][index0] == m_byArOutPai[1][index1] && m_byArOutPai[1][index1] == m_byArOutPai[2][index2]
			&& m_byArOutPai[2][index2] == m_byArOutPai[3][index3])
		{
			cout_++;
			index0++;
			index1++;
			index2++;
			index3++;
			if (!bIsNext)
			{
				break;
			}
		}
		else
			break;
	}
	return cout_;
}

//��ȡ������ҵ����λ�ù�ϵ
BYTE GameDataEx::GetPlayerStationRelation(BYTE byMeStation, BYTE byOtherStation)
{
	if (byMeStation < 0 || byMeStation >= PLAY_COUNT
		|| byOtherStation < 0 || byOtherStation >= PLAY_COUNT)
	{
		return MJ_STATION_NO;
	}
	if (byMeStation == byOtherStation)
	{
		return MJ_STATION_BEN_JIA;
	}
	int i = 0;
	for (i = 0; i < PLAY_COUNT;i++)
	{
		int j = (byMeStation + PLAY_COUNT - 1 - i) % PLAY_COUNT;
		if (j == byOtherStation)
			break;
	}
	if (i == 0)
	{
		return MJ_STATION_XIA_JIA;
	}
	else if (i == 1)
	{
		return MJ_STATION_DUI_JIA;
	}
	else if (i == 2)
	{
		return MJ_STATION_SHANG_JIA;
	}
	return MJ_STATION_NO;
}

//��ȡbyMeStation��Һͷ�byNoStation�Ĺ�ϵ
BYTE GameDataEx::GetPlayerStationRelationEx(BYTE byMeStation, BYTE byNoStation)
{
	if (PLAY_COUNT != 4)
	{
		return MJ_STATION_NO;
	}
	BYTE byPlayer1 = 255, byPlayer2 = 255;

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (i != byMeStation && i != byNoStation)
		{
			if (byPlayer1 != 255)
			{
				byPlayer1 = i;
			}
			else if (byPlayer2 != 255)
			{
				byPlayer2 = i;
			}
		}
	}
	return GetPlayerStationRelation(byMeStation, byPlayer1) | GetPlayerStationRelation(byMeStation, byPlayer2);
}

//���ʣ������
BYTE GameDataEx::GetRemainMenPai(BYTE byRemainMenPai[MAX_REMAIN_MEN_PAI_NUM])
{
	BYTE byNum = 0;
	for (int i = 0;i < MAX_MJ_PAI_COUNT;i++)
	{
		if (m_MenPai.byMenPai[i] != 255)
		{
			if (byNum < MAX_REMAIN_MEN_PAI_NUM)
			{
				byRemainMenPai[byNum] = m_MenPai.byMenPai[i];
			}
			byNum++;
		}
	}
	return byNum;
}

//BYTE byHuType[MAX_HUPAI_TYPE];		//�������� ��255Ĭ�����ֵ
//BYTE byNameType;					//��Ӧ�����֣� ö������MJ_HU_FEN_TYPE
//BYTE byXiangYingStation[MAX_COUNT_HUPAI_FEN]; //��Ӧ��ң�ö������MJ_STATION
//int iFanCount;						//�������͵ķ������������ϲ88������һɫ16��
//int iAllFen[MAX_COUNT_HUPAI_FEN];   //��Ӯ�ĺ���
bool GameDataEx::AddToHuFenStruct(BYTE byStation, BYTE byNameType, BYTE byXiangYingStation, BYTE byXiangYingStationEx, bool bBaoTing, int iAllFen)
{
	if (byStation >= PLAY_COUNT || byStation < 0 || m_countHuFen[byStation].byNums >= 3)
	{
		return false;
	}
	m_countHuFen[byStation].byNameType[m_countHuFen[byStation].byNums] = byNameType;
	m_countHuFen[byStation].byXiangYingStation[m_countHuFen[byStation].byNums] = byXiangYingStation;
	m_countHuFen[byStation].byXiangYingStationEx[m_countHuFen[byStation].byNums] = byXiangYingStationEx;
	//m_countHuFen[byStation].bBaoTing[m_countHuFen[byStation].byNums] = bBaoTing;
	m_countHuFen[byStation].iAllFen[m_countHuFen[byStation].byNums] = iAllFen;
	m_countHuFen[byStation].byNums++;
	return true;
}

//BYTE byGangType[MAX_COUNT_GANG_FEN]; //������ ��ö������MJ_GANG_FEN_TYPE
//BYTE byXiangYingStation[MAX_COUNT_GANG_FEN]; //��Ӧ��ң�ö������MJ_STATION
//int iAllFen[MAX_COUNT_GANG_FEN];//�ܷ�
bool GameDataEx::AddToGangFenStruct(BYTE byStation, BYTE byGangType, BYTE byXiangYingStation, bool bBaoTing, int iAllFen)
{
	if (m_countGangFen[byStation].byNums >= MAX_COUNT_GANG_FEN || byGangType == MJ_GANG_FEN_TYPE_NO)
	{
		return false;
	}
	bool bIsXiangTongGang = false;
	int iIndex = 0;
	for (int i = 0; i < m_countGangFen[byStation].byNums; i++)
	{
		if (m_countGangFen[byStation].byGangType[i] == byGangType && m_countGangFen[byStation].bBaoTing[i] == bBaoTing
			&& m_countGangFen[byStation].byXiangYingStation[i] == byXiangYingStation)
		{
			bIsXiangTongGang = true;
			iIndex = i;
			break;
		}
	}
	if (bIsXiangTongGang)
	{
		m_countGangFen[byStation].byGangNums[iIndex] ++;
		m_countGangFen[byStation].iAllFen[iIndex] += iAllFen;
	}
	else
	{
		m_countGangFen[byStation].byGangType[m_countGangFen[byStation].byNums] = byGangType;
		m_countGangFen[byStation].byXiangYingStation[m_countGangFen[byStation].byNums] = byXiangYingStation;
		m_countGangFen[byStation].bBaoTing[m_countGangFen[byStation].byNums] = bBaoTing;
		m_countGangFen[byStation].byGangNums[m_countGangFen[byStation].byNums] ++;
		m_countGangFen[byStation].iAllFen[m_countGangFen[byStation].byNums] = iAllFen;
		m_countGangFen[byStation].byNums++;
	}
	return true;
}

//��������б��
bool GameDataEx::DelBuGangFenStruct(BYTE byStation, BYTE byGangType, BYTE byXiangYingStation, bool bBaoTing, int iAllFen)
{
	if (m_countGangFen[byStation].byNums >= MAX_COUNT_GANG_FEN || byGangType == MJ_GANG_FEN_TYPE_NO)
	{
		return false;
	}
	bool bIsXiangTongGang = false;
	int iIndex = 0;
	for (int i = 0; i < m_countGangFen[byStation].byNums; i++)
	{
		if (m_countGangFen[byStation].byGangType[i] == byGangType && m_countGangFen[byStation].bBaoTing[i] == bBaoTing
			&& m_countGangFen[byStation].byXiangYingStation[i] == byXiangYingStation)
		{
			bIsXiangTongGang = true;
			iIndex = i;
			break;
		}
	}
	if (bIsXiangTongGang)
	{
		if (m_countGangFen[byStation].byGangNums[iIndex] > 1)
		{
			m_countGangFen[byStation].byGangNums[iIndex] --;
			m_countGangFen[byStation].iAllFen[iIndex] += iAllFen;
		}
		else
		{
			m_countGangFen[byStation].byGangType[iIndex] = MJ_GANG_FEN_TYPE_NO;
		}
		return true;
	}
	return false;
}
////��������
//bool GameDataEx::AddToHorseFenStruct(BYTE byStation,BYTE byBuyHorseType,BYTE byMaiStation,BYTE byBeiMaiStation,int iAllFen)
//{
//	if (m_countHorseFen[byStation].byNums >= MAX_COUNT_HORSE_FEN)
//	{
//		return false;
//	}
//	m_countHorseFen[byStation].byBuyHorseType[m_countHorseFen[byStation].byNums] = byBuyHorseType;
//	m_countHorseFen[byStation].byMaiStation[m_countHorseFen[byStation].byNums] = byMaiStation;
//	m_countHorseFen[byStation].byBeiMaiStation[m_countHorseFen[byStation].byNums] = byBeiMaiStation;
//	m_countHorseFen[byStation].iAllFen[m_countHorseFen[byStation].byNums] = iAllFen;
//	m_countHorseFen[byStation].byNums ++;
//	return true;
//}

//BYTE byGenZhuangType; //���� ��0��Ĭ�����ֵ����1,��ׯ����2������ׯ��
//BYTE byXiangYingStation; //��Ӧ��ң�ö������MJ_STATION
//int iAllFen;//��ׯ��
//bool GameDataEx::AddToGenZhuangFenStruct(BYTE byStation,BYTE byGenZhuangType,BYTE byXiangYingStation,int iAllFen)
//{
//	if (m_countGenZhuang[byStation].byNums >= MAX_COUNT_GEN_ZHUANG_FEN)
//	{
//		return false;
//	}
//	m_countGenZhuang[byStation].byGenZhuangType[m_countGenZhuang[byStation].byNums] = byGenZhuangType;
//	m_countGenZhuang[byStation].byXiangYingStation[m_countGenZhuang[byStation].byNums] = byXiangYingStation;
//	m_countGenZhuang[byStation].iAllFen[m_countGenZhuang[byStation].byNums] = iAllFen;
//	m_countGenZhuang[byStation].byNums ++;
//	return true;
//}