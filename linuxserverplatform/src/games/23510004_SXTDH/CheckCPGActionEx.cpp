#include "stdafx.h"
#include "CheckCPGActionEx.h"
CheckCPGActionEx::CheckCPGActionEx(void)
{
}

CheckCPGActionEx::~CheckCPGActionEx(void)
{
}
///��ʼ������
void CheckCPGActionEx::InitData()
{
	CheckCPGAction::InitData();
}
///�ܷ�����
bool CheckCPGActionEx::CanPeng(GameDataEx *pGameData, BYTE station, BYTE pai)
{
	if (!pGameData->m_mjAction.bPeng)
	{
		return false;
	}
	if (pGameData->m_mjAction.bQiPeng && pGameData->m_bQiPeng[station]) //������������Լ�ץ��ǰ��׼��
	{
		for (size_t i = 0; i < pGameData->m_vecQiPengCard[station].size(); i++)
		{
			if (pai == pGameData->m_vecQiPengCard[station][i])
			{
				return false;
			}
		}
	}
	if (pai != 255 && pGameData->GetAHandPaiCount(station, pai) >= 2)
	{
		return true;
	}
	return false;
}
///�ܷ����
bool CheckCPGActionEx::CanGang(GameDataEx *pGameData, BYTE station, BYTE pai, BYTE Result[][2], bool bMe, CheckHuPaiEx *pCheckHuData)
{
	if (!pGameData->m_mjAction.bGang)
	{
		return false;
	}

	/////////////////////////////���ƣ����ڲ��ܱ�/////////////////////////////////////////////
	BYTE byTempHandCard[HAND_CARD_NUM];
	bool bb = false;
	if (bMe)//���Լ����ƣ����ܺͲ���
	{
		int temp = 0, index = 0;
		for (int i = 0;i < HAND_CARD_NUM;i++)
		{
			if (pGameData->GetAHandPaiCount(station, pGameData->m_byArHandPai[station][i]) >= 4 && temp != pGameData->m_byArHandPai[station][i])
			{
				temp = pGameData->m_byArHandPai[station][i];
				memset(byTempHandCard, 255, sizeof(byTempHandCard));
				pGameData->CopyOneUserHandPai(byTempHandCard, station);
				if (pGameData->m_bTing[station]) //��������Ƿ�ı�
				{
					for (int i = 0; i < HAND_CARD_NUM; i++)
					{
						if (byTempHandCard[i] == temp)
						{
							byTempHandCard[i] = 255;
						}
					}
					if (pCheckHuData && pCheckHuData->IsTingKouChange(pGameData, station, byTempHandCard))
					{
						continue;
					}
				}
				Result[index][1] = pGameData->m_byArHandPai[station][i];
				Result[index][0] = ACTION_AN_GANG;
				bb = true;
				index++;
				continue;
			}
			if (pGameData->m_mjAction.bBuGang)
			{
				if (pGameData->IsUserHavePengPai(station, pGameData->m_byArHandPai[station][i]))
				{
					Result[index][1] = pGameData->m_byArHandPai[station][i];
					Result[index][0] = ACTION_BU_GANG;
					temp = pGameData->m_byArHandPai[station][i];
					bb = true;
					index++;
				}
			}
		}
		return bb;
	}
	else//����
	{
		if (pai == 255 || pai == 0)
			return false;
		memset(byTempHandCard, 255, sizeof(byTempHandCard));
		pGameData->CopyOneUserHandPai(byTempHandCard, station);
		if (pGameData->GetAHandPaiCount(station, pai) >= 3)
		{
			if (pGameData->m_bTing[station]) //��������Ƿ�ı�
			{
				for (int i = 0; i < HAND_CARD_NUM; i++)
				{
					if (byTempHandCard[i] == pai)
					{
						byTempHandCard[i] = 255;
					}
				}
				if (pCheckHuData && pCheckHuData->IsTingKouChange(pGameData, station, byTempHandCard))
				{
					return false;
				}
			}
			bb = true;
			Result[0][1] = pai;
			Result[0][0] = ACTION_MING_GANG;
		}
	}
	return bb;

}
//�������,byCard�Ǹܵ���
void CheckCPGActionEx::CountGangFen(GameDataEx *pGameData, int iStation, int iBeStation, BYTE byType)
{
	int iShuGang = 0;
	if (ACTION_AN_GANG == byType)//����ÿ��2��
	{
		for (int i = 0;i < PLAY_COUNT;i++)
		{
			int iFen = 0;
			if (iStation == i)
			{
				continue;
			}
			iFen = 2 * pGameData->m_mjRule.iGameBase;
			pGameData->m_iGangScore[i] -= iFen;
			iShuGang += iFen;
			pGameData->AddToGangFenStruct(i, MJ_GANG_FEN_TYPE_BEI_AN_GANG, pGameData->GetPlayerStationRelation(i, iStation), false, -iFen);
		}
		pGameData->m_iGangScore[iStation] += iShuGang;
		pGameData->AddToGangFenStruct(iStation, MJ_GANG_FEN_TYPE_AN_GANG, MJ_STATION_ALL_JIA, false, iShuGang);
	}
	else if (ACTION_BU_GANG == byType)//����ÿ��1��
	{
		for (int i = 0;i < PLAY_COUNT;i++)
		{
			int iFen = 0;
			if (iStation == i)
			{
				continue;
			}
			iFen = 1 * pGameData->m_mjRule.iGameBase;
			pGameData->m_iGangScore[i] -= iFen;
			iShuGang += iFen;
			pGameData->AddToGangFenStruct(i, MJ_GANG_FEN_TYPE_BEI_BU_GANG, pGameData->GetPlayerStationRelation(i, iStation), false, -iFen);
		}
		pGameData->m_iGangScore[iStation] += iShuGang;
		pGameData->AddToGangFenStruct(iStation, MJ_GANG_FEN_TYPE_BU_GANG, MJ_STATION_ALL_JIA, false, iShuGang);
	}
	else if (ACTION_MING_GANG == byType)//����һ��3��
	{
		if (pGameData->m_bTing[iStation]) //������ܣ����Ҷ�����
		{
			for (int i = 0;i < PLAY_COUNT;i++)
			{
				int iFen = 0;
				if (iStation == i)
				{
					continue;
				}
				iFen = 1 * pGameData->m_mjRule.iGameBase;
				pGameData->m_iGangScore[i] -= iFen;
				iShuGang += iFen;
				if (i == iBeStation)
				{
					pGameData->AddToGangFenStruct(i, MJ_GANG_FEN_TYPE_FANG_GANG, pGameData->GetPlayerStationRelation(i, iStation), true, -iFen);
				}
				else
				{
					pGameData->AddToGangFenStruct(i, MJ_GANG_FEN_TYPE_BEI_MING_GANG, pGameData->GetPlayerStationRelation(i, iStation), true, -iFen);
				}
			}
			pGameData->m_iGangScore[iStation] += iShuGang;
			pGameData->AddToGangFenStruct(iStation, MJ_GANG_FEN_TYPE_MING_GANG, MJ_STATION_ALL_JIA, true, iShuGang);
		}
		else
		{
			iShuGang = 3 * pGameData->m_mjRule.iGameBase;
			pGameData->m_iGangScore[iBeStation] -= iShuGang;
			pGameData->m_iGangScore[iStation] += iShuGang;

			pGameData->AddToGangFenStruct(iBeStation, MJ_GANG_FEN_TYPE_FANG_GANG, pGameData->GetPlayerStationRelation(iBeStation, iStation), false, -iShuGang);
			pGameData->AddToGangFenStruct(iStation, MJ_GANG_FEN_TYPE_MING_GANG, pGameData->GetPlayerStationRelation(iStation, iBeStation), false, iShuGang);
		}
	}
}

//������֣��������ȼ�����֣����ܼ���ܷ�
void CheckCPGActionEx::CountGangFenEx(GameDataEx *pGameData)
{
	//// ͳ������
	////WAUTOCOST("CountGangFenEx ��ʱͳ��");
	//memset(pGameData->m_iGangScore,0,sizeof(pGameData->m_iGangScore));
	//for (int j = 0;j<5;j++)
	//{
	//	for(int i=0;i<PLAY_COUNT;i++)
	//	{
	//		int iShuGang=0;
	//		if(ACTION_AN_GANG==pGameData->m_UserGCPData[i][j].byType)//����ÿ��2��
	//		{
	//			for(int k=0;k<PLAY_COUNT;k++)
	//			{
	//				int iFen = 0;
	//				if(i==k)
	//				{
	//					continue;
	//				}
	//				iFen = 2* pGameData->m_mjRule.iGameBase;
	//				pGameData->m_iGangScore[k]-=iFen;
	//				iShuGang+=iFen;
	//				pGameData->AddToGangFenStruct(k,MJ_GANG_FEN_TYPE_BEI_AN_GANG,pGameData->GetPlayerStationRelation(k,i),-iFen);
	//			}
	//			pGameData->m_iGangScore[i]+=iShuGang;
	//			pGameData->AddToGangFenStruct(i,MJ_GANG_FEN_TYPE_AN_GANG,MJ_STATION_ALL_JIA,iShuGang);
	//		}
	//		else if(ACTION_BU_GANG==pGameData->m_UserGCPData[i][j].byType)//����ÿ��1��(����)
	//		{
	//			for(int k=0;k<PLAY_COUNT;k++)
	//			{
	//				int iFen = 0;
	//				if(i==k)
	//				{
	//					continue;
	//				}
	//				iFen = 1* pGameData->m_mjRule.iGameBase;
	//				pGameData->m_iGangScore[k]-=iFen;
	//				iShuGang+=iFen;
	//				pGameData->AddToGangFenStruct(k,MJ_GANG_FEN_TYPE_BEI_BU_GANG,pGameData->GetPlayerStationRelation(k,i),-iFen);
	//			}
	//			pGameData->m_iGangScore[i]+=iShuGang;
	//			pGameData->AddToGangFenStruct(i,MJ_GANG_FEN_TYPE_BU_GANG,MJ_STATION_ALL_JIA,iShuGang);
	//		}
	//		else if(ACTION_MING_GANG==pGameData->m_UserGCPData[i][j].byType)//����һ��3�֣��Ըܣ�
	//		{
	//			iShuGang = 3* pGameData->m_mjRule.iGameBase;
	//			pGameData->m_iGangScore[pGameData->m_UserGCPData[i][j].iBeStation]-=iShuGang;
	//			pGameData->m_iGangScore[i]+=iShuGang;
	//			pGameData->AddToGangFenStruct(pGameData->m_UserGCPData[i][j].iBeStation,MJ_GANG_FEN_TYPE_FANG_GANG,
	//				pGameData->GetPlayerStationRelation(pGameData->m_UserGCPData[i][j].iBeStation,i),-iShuGang);
	//			pGameData->AddToGangFenStruct(i,MJ_GANG_FEN_TYPE_MING_GANG,
	//				pGameData->GetPlayerStationRelation(i,pGameData->m_UserGCPData[i][j].iBeStation),iShuGang);
	//		}
	//	}
	//}
}

//��ȥ���ܺ����øܷ�,�����ܷ�
void CheckCPGActionEx::MinusGangFen(GameDataEx *pGameData, int iStation, int iBeStation)
{
	if (pGameData->m_mjRule.bIsSuanGangHu)
	{
		return;
	}
	int iShuGang = 0;
	for (int i = 0;i < PLAY_COUNT;i++)//��Ӯ��
	{
		int iFen = 0;
		if (iStation == i)
		{
			continue;
		}
		iFen = 1 * pGameData->m_mjRule.iGameBase;
		pGameData->m_iGangScore[i] += iFen;
		iShuGang += iFen;
		pGameData->DelBuGangFenStruct(i, MJ_GANG_FEN_TYPE_BEI_BU_GANG, pGameData->GetPlayerStationRelation(i, iStation), false, iFen);
	}
	pGameData->m_iGangScore[iStation] -= iShuGang;
	pGameData->DelBuGangFenStruct(iStation, MJ_GANG_FEN_TYPE_BU_GANG, MJ_STATION_ALL_JIA, false, -iShuGang);
}