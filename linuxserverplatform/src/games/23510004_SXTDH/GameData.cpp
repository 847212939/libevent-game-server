#include "stdafx.h"
#include "GameData.h"
GameData::GameData(void)
{
	memset(m_byArHandPai, 255, sizeof(m_byArHandPai));	/// ���Ʋ�����20�ţ�255Ϊ������־
	memset(m_byArOutPai, 255, sizeof(m_byArOutPai));		/// ���Ʋ�����160�ţ�255Ϊ������־
	memset(m_byArHuaPai, 255, sizeof(m_byArHuaPai));		/// ���Ʋ�����10�ţ�255Ϊ������־
	memset(m_iArMenPai, 255, sizeof(m_iArMenPai));		/// ���Ʋ�����40�ţ�255Ϊ������־	
	memset(m_byCanOutCard, 255, sizeof(m_byCanOutCard));	/// ͣ�ƻ������������ܴ������
	memset(m_byMenFeng, 255, sizeof(m_byMenFeng));		/// �ŷ�	
	memset(m_bHaveOut, 0, sizeof(m_bHaveOut));			///	��¼����Ƿ��������
	memset(m_byArHuaPaiCount, 0, sizeof(m_byArHuaPaiCount));

	m_StrJing.Init();
	//��ʼ����
	m_MenPai.Init();
	m_byQuanFeng = 255;			///Ȧ��
	m_bySelectIndex = 255;		///��ǰѡ��������
	m_byLastOutPai = 255;		///���������
	m_byEndZhuaPaiPalyer = 255;		///��ǰ������ץ��
	m_byNowOutStation = 255;	///��ǰ����λ��
	m_byLastOutStation = 255;	///�ϴγ���λ��
	///��ǰץ�Ʒ���true ˳ʱ�룬false ��ʱ��
	m_bZhuaPaiDir = true;
	InitData();
	LoadIni();
}
GameData::~GameData(void)
{
}
///��ʼ������
void GameData::InitData()
{
	//��¼����״̬��ӵ�ڸܿ��͸ܺ���
	memset(m_bGangState, 0, sizeof(m_bGangState));
	///�������¼�,��ʱ����
	temp_ChiPai.Init();
	///�������¼�,��ʱ����
	temp_PengPai.Init();
	///�������¼�,��ʱ����
	temp_GangPai.Init();
	///��2��ɫ��ȷ������λ�ã������ͣ������Ƶ㣨��С�㣩�¼�
	T_TwoSeziDirAndGetPai.Init();
	///�����¼�
	//T_SendPai.Init();
	///������Ҳ����¼�
	//T_AllBuHua.Init();
	///������Ҳ����¼�
	//T_OneBuHua.Init();
	///�����¼�
	T_OutPai.Init();
	///ץ�����¼�
	T_ZhuaPai.Init();
	/////�����ܺ���֪ͨ�¼����¼�
	for (int i = 0;i < PLAY_COUNT;++i)
	{
		T_CPGNotify[i].Init();//����
	}
	///�������¼�
	T_ChiPai.Init();
	///�������¼�
	T_PengPai.Init();
	///�������¼�
	T_GangPai.Init();
	///�������¼�
	T_TingPai.Init();
	///�������¼�
	T_HuPai.Init();
	///����¼�
	T_CountFen.Init();
	///��ʼ����֪ͨ�¼�
	//T_BeginOutPai.Init();
	memset(m_byArHandPai, 255, sizeof(m_byArHandPai));			//���Ʋ�����20�ţ�255Ϊ������־
	memset(m_byArHandPaiCount, 0, sizeof(m_byArHandPaiCount));	//��������
	memset(m_byArOutPai, 255, sizeof(m_byArOutPai));				//���Ʋ�����160�ţ�255Ϊ������־
	memset(m_byArOutPaiCount, 0, sizeof(m_byArOutPaiCount));		//��������
	memset(m_byArHuaPai, 255, sizeof(m_byArHuaPai));				//���Ʋ�����10�ţ�255Ϊ������־
	memset(m_iArMenPai, 255, sizeof(m_iArMenPai));				//���Ʋ�����40�ţ�255Ϊ������־
	memset(m_byMenPaiData, 255, sizeof(m_byMenPaiData));	//ת�������������
	m_StrJing.Init();//��ʼ������
	m_MenPai.Init();//��ʼ����
	for (int i = 0;i < PLAY_COUNT;++i)
	{
		m_bHaveOut[i] = false;
		temp_HuPai[i].Init();
	}
	for (int i = 0;i < PLAY_COUNT;i++)
	{
		for (int index = 0;index < 5;index++)
		{
			m_UserGCPData[i][index].Init();
		}
	}
	memset(m_bTing, 0, sizeof(m_bTing));				//����Ƿ�����
	memset(m_byTingType, 0, sizeof(m_byTingType));	//�����������
	memset(m_byCanOutCard, 255, sizeof(m_byCanOutCard));///ͣ�ƻ������������ܴ������
	memset(m_byMenFeng, 255, sizeof(m_byMenFeng));	///�ŷ�	
	memset(m_bHaveOut, 0, sizeof(m_bHaveOut));			///	��¼����Ƿ��������
	memset(m_byArHuaPaiCount, 0, sizeof(m_byArHuaPaiCount));
	m_bySelectIndex = 255;				///��ǰѡ��������
	m_byLastOutPai = 255;				///���������
	m_byEndZhuaPaiPalyer = 255;			///��ǰ������ץ��
	m_byNowOutStation = 255;			///��ǰ����λ��
	m_byLastOutStation = 255;			///�ϴγ���λ��
	///��ǰץ�Ʒ���true ˳ʱ�룬false ��ʱ��
	m_bZhuaPaiDir = true;

	memset(m_byRemainMenPai, 255, sizeof(m_byRemainMenPai));//ʣ������
	m_byRemainMenPaiCount = 0;
	memset(m_byTingOutCardIndex, 255, sizeof(m_byTingOutCardIndex));
	//�Ѿ����ȥ��ÿ��������
	memset(m_byAllOutCardNum, 0, sizeof(m_byAllOutCardNum));
}
/*--------------------------------------------------------------------------*/
///�������淨����
void GameData::LoadIni()
{
	//�淨����
	//GameData::LoadIni()��CServerGameDesk::InitDeskGameStation()֮ǰ����
	m_mjRule.byAllMjCount = 136;			//�齫������ 
	m_mjRule.bHaveWan = 1;			//�Ƿ�����
	m_mjRule.bHaveTiao = 1;			//�Ƿ�����
	m_mjRule.bHaveBing = 1;			//�����б�
	m_mjRule.bHaveHongZhong = 1;			//�Ƿ��к���
	m_mjRule.bHaveFaCai = 1;			//�Ƿ��з���
	m_mjRule.bHaveBaiBan = 1;			//�Ƿ��аװ�
	m_mjRule.bHaveFengPai = 1;			//�Ƿ��ж�������
	m_mjRule.bHaveFlower = 0;			//�Ƿ��л���
	m_mjRule.bHaveJing = 0;			//���޲���
	m_mjRule.byMenPaiNum[0] = 34;			//����������
	m_mjRule.byMenPaiNum[1] = 34;			//����������
	m_mjRule.byMenPaiNum[2] = 34;			//����������
	m_mjRule.byMenPaiNum[3] = 34;			//����������
	m_mjRule.byAutoOutTime = 1;			//�Զ�����ʱ��	
	m_mjRule.byOutTime = 15;			//����ʱ��		
	m_mjRule.byBlockTime = 15;			//����˼��ʱ��	
	m_mjRule.bNetCutTuoGuan = 1;			//�Ƿ�����й�
	m_mjRule.bForceTuoGuan = 1;			//�Ƿ�ǿ���й�
	m_mjRule.byGamePassNum = 0;			//��������
	m_mjRule.iGameBase = 1;			//������
	m_mjRule.bIsDuoXiang = 1;            //�Ƿ����һ�ڶ���
	m_mjRule.bIsSuanGangHu = 0;            //�Ƿ�������ܺ����øܷ�
	m_mjRule.bChaoShiTuoGuan = 1;           //��ҳ���ʱ�й�
	m_mjRule.bDuanXianTuoGuan = 1;          //�Ƿ������������Ƿ��й�
#ifdef _RELEASEEX //��Ӫ�治����
	m_mjRule.bIsMatchMJ = 0;                //�Ƿ�����
#else
	m_mjRule.bIsMatchMJ = 1;                //�Ƿ�����
#endif // _RELEASEEX

	//��������
	m_mjAction.bChi = 0;		//��
	m_mjAction.bChiJing = 0;        //�����
	m_mjAction.bChiFeng = 0;		//�Է���
	m_mjAction.bChiJian = 0;		//���з���
	m_mjAction.bPeng = 1;		//��
	m_mjAction.bKan = 0;		//��
	m_mjAction.bGang = 1;		//��
	m_mjAction.bBuGang = 1;		//����
	m_mjAction.bGangAfterTing = 0;
	m_mjAction.bTing = 1;		//��
	m_mjAction.bQiangChi = 0;		//����(�Ժ�����)
	m_mjAction.bQxHh = 1;        //�������ܺ�(һȦ��)
	m_mjAction.bQiPeng = 1;        //����������(һȦ��)
	m_mjAction.bQiangGangHu = 1;
	m_mjAction.bFangPao = 1;			//�Ƿ��ܵ���
	m_mjAction.bZiMo = 1;			//�Ƿ�������
	m_mjAction.bBuHua = 0;			//�Ƿ���Ҫ����
}
/*--------------------------------------------------------------------------*/


///���ƣ�IsHaveAHandPai
///����������Ƿ����ָ��������
///@param pai Ҫ������
///@return true �� false ��
bool GameData::IsHaveAHandPai(BYTE station, BYTE pai)
{
	if (station < 0 || station >= PLAY_COUNT)
		return false;
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (m_byArHandPai[station][i] == pai)
			return true;
	}
	return false;
}

///���ƣ�GetAHandPaiCount
///���������������ĳ���Ƶĸ���
///@param pai Ҫ������
///@return ������
int GameData::GetAHandPaiCount(BYTE station, BYTE pai)
{
	if (station < 0 || station >= PLAY_COUNT)
		return 0;
	int count = 0;
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (pai != 255 && m_byArHandPai[station][i] == pai)
			count++;
	}
	return count;
}

///���ƣ�GetHandPaiCount
///������������Ƹ���
///@param 
///@return ������Ƶ�����
int GameData::GetHandPaiCount(BYTE station)
{
	if (station < 0 || station >= PLAY_COUNT)
		return 0;
	int count = 0;
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (m_byArHandPai[station][i] != 255)
			count++;
	}
	return count;
}

///���ƣ�CopyHandPai
///������������ҵ�����
///@param pai[] ������, station λ��, show �Ƿ񿽱�����
///@return �����������
int GameData::CopyHandPai(BYTE pai[][HAND_CARD_NUM], BYTE station, bool show)
{
	if (station < 0 || station >= PLAY_COUNT)
		return 0;
	//memset(pai,255,sizeof(pai));
	for (int i = 0;i < PLAY_COUNT;++i)
	{
		int count = GetHandPaiCount(station);
		if (i == station)//�Լ�����
		{
			for (int j = 0;j < HAND_CARD_NUM;++j)
			{
				pai[i][j] = m_byArHandPai[station][j];
			}
		}
		else
		{
			if (!show)
			{
				for (int j = 0;j < HAND_CARD_NUM;++j)
				{
					if (m_byArHandPai[i][j] != 255)
					{
						pai[i][j] = 255;
					}
					else
					{
						pai[i][j] = m_byArHandPai[i][j];
					}
				}
			}
			else
			{
				for (int j = 0;j < HAND_CARD_NUM;++j)
				{
					pai[i][j] = m_byArHandPai[i][j];
				}
			}
		}
	}
	return 0;
}

///���ƣ�CopyOneUserHandPai
///����������ĳ����ҵ�����
///@param pai[] ������, station λ��
///@return �����������
int GameData::CopyOneUserHandPai(BYTE pai[HAND_CARD_NUM], BYTE station)
{
	if (station < 0 || station >= PLAY_COUNT)
		return 0;
	int count = 0;
	for (int j = 0;j < HAND_CARD_NUM;++j)
	{
		if (m_byArHandPai[station][j] != 255)
			count++;
		pai[j] = m_byArHandPai[station][j];
	}
	return count;
}

///���ƣ�SortHandPai
///��������������small�Ƿ��С������
///@param big �Ƿ�ӵ���С����Ĭ�ϴ�С��������
void GameData::SortHandPai(BYTE station, bool big)
{
	if (station < 0 || station >= PLAY_COUNT)
		return;
	///����
	CLogicBase::SelectSort(m_byArHandPai[station], HAND_CARD_NUM, big);
	/*BYTE HandPai[HAND_CARD_NUM];
	for(int j=0;j<HAND_CARD_NUM;++j)
	{
	HandPai[j] = m_byArHandPai[station][j];
	}
	int index=0;
	for(int j=0;j<HAND_CARD_NUM;++j)
	{
	if(m_StrJing.CheckIsJing(HandPai[j]))
	{
	m_byArHandPai[station][index] = HandPai[j];
	index++;
	}
	}
	for(int j=0;j<HAND_CARD_NUM;++j)
	{
	if(!m_StrJing.CheckIsJing(HandPai[j]) && index<HAND_CARD_NUM)
	{
	m_byArHandPai[station][index] = HandPai[j];
	index++;
	}
	}*/
}

///���ƣ�AddToHandPai
///���������һ���Ƶ�������
///@param pai ��ӵ�����ֵ
///@return 
void GameData::AddToHandPai(BYTE station, BYTE pai)
{
	if (station < 0 || station >= PLAY_COUNT)
		return;
	if (pai == 255)
	{
		return;
	}
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (m_byArHandPai[station][i] == 255)
		{
			m_byArHandPai[station][i] = pai;
			m_byArHandPaiCount[station]++;
			break;
		}
	}
}

///���ƣ�SetHandPaiData
///������������������
///@param  pai[] �������������
void GameData::SetHandPaiData(BYTE station, BYTE pai[], BYTE byCount)
{
	if (station < 0 || station >= PLAY_COUNT)
	{
		return;
	}
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		m_byArHandPai[station][i] = pai[i];
	}
	m_byArHandPaiCount[station] = byCount;
}

///���ƣ�DeleteAHandPai
///������ɾ��һ��ָ��������
///@param pai Ҫɾ������
BYTE GameData::DeleteAHandPai(BYTE station, BYTE pai)
{
	if (station < 0 || station >= PLAY_COUNT || !CMjRef::IsValidateMj(pai))
		return 255;
	BYTE byIndex = 255;
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (m_byArHandPai[station][i] == pai)
		{
			byIndex = i;
			m_byArHandPai[station][i] = 255;
			break;
		}
	}
	m_byArHandPaiCount[station]--;
	return byIndex;
	//CLogicBase::SelectSort(m_byArHandPai[station],HAND_CARD_NUM,false);
	//BYTE HandPai[HAND_CARD_NUM];
	//for(int j=0;j<HAND_CARD_NUM;++j)
	//{
	//	HandPai[j] = m_byArHandPai[station][j];
	//}
	//int index=0;
	//for(int j=0;j<HAND_CARD_NUM;++j)
	//{
	//	if(m_StrJing.CheckIsJing(HandPai[j]))
	//	{
	//		m_byArHandPai[station][index] = HandPai[j];
	//		index++;
	//	}
	//}
	//for(int j=0;j<HAND_CARD_NUM;++j)
	//{
	//	if(!m_StrJing.CheckIsJing(HandPai[j]) && 255!=HandPai[j])
	//	{
	//		m_byArHandPai[station][index] = HandPai[j];
	//		index++;
	//	}
	//}
	//m_byArHandPaiCount[station]=index;
}

///���ƣ�SetHandPaiBack
///���������������Ʊ�
///@param num���õ�����
///@return 
void GameData::SetHandPaiBack(BYTE station, BYTE num)
{
	if (station < 0 || station >= PLAY_COUNT)
		return;
	memset(m_byArHandPai[station], 255, sizeof(m_byArHandPai[station]));
	for (int i = 0;i < num;i++)
	{
		m_byArHandPai[station][i] = 0;
	}
}

///���ƣ�GetLastHandPai
///������������һ������
///@param 
BYTE GameData::GetLastHandPai(BYTE station)
{
	if (station < 0 || station >= PLAY_COUNT)
		return 255;
	for (int i = HAND_CARD_NUM - 1;i >= 0;i--)
	{
		if (m_byArHandPai[station][i] != 255)
		{
			return m_byArHandPai[station][i];
		}
	}
	return 255;
}

///���ƣ�GetSelectHandPai
///���������ѡ�е�һ������
///@param 
///@return 
BYTE GameData::GetSelectHandPai(BYTE station)
{
	if (station < 0 || station >= PLAY_COUNT)
		return 255;
	if (m_bySelectIndex < HAND_CARD_NUM && m_bySelectIndex != 255)
		return m_byArHandPai[station][m_bySelectIndex];
	return 255;
}

///���ƣ�IsOutPaiPeople
///�������Ƿ�������
///@param 
///@return  true �� ,false ����
bool GameData::IsOutPaiPeople(BYTE station)
{
	if (station < 0 || station >= PLAY_COUNT)
		return false;
	if (GetHandPaiCount(station) % 3 == 2)
	{
		return true;
	}
	return false;
}

///���ƣ�IsHuaPaiInHand
///����������������Ƿ���ڻ���
///@param 
///@return  true �� ,false ����
bool GameData::IsHuaPaiInHand(BYTE station)
{
	if (station < 0 || station >= PLAY_COUNT)
		return false;
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (CheckIsHuaPai(m_byArHandPai[station][i]))
		{
			return true;
		}
	}
	return false;
}

///���ƣ�MoveHuaPaiFormHand
///�������������еĻ����Ƶ�����������
///@param 
///@return  ���Ƶ�����
int GameData::MoveHuaPaiFormHand(BYTE station)
{
	if (station < 0 || station >= PLAY_COUNT)
		return 0;
	int hua = 0;
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (m_byArHandPai[station][i] == 255)
		{
			break;
		}
		if (CheckIsHuaPai(m_byArHandPai[station][i]))
		{
			hua++;
			AddToHuaPai(station, m_byArHandPai[station][i]);
			DeleteAHandPai(station, m_byArHandPai[station][i]);
		}
	}
	return hua;
}

///ͨ��������ȡһ������
BYTE GameData::GetHandPaiFromIndex(BYTE station, int index)
{
	if (station < 0 || station >= PLAY_COUNT)
		return 255;
	if (station < 0 || station>3)
		return 255;
	return m_byArHandPai[station][index];
}

///ͨ����������һ������
bool GameData::SetHandPaiFromIndex(BYTE station, int index, BYTE pai)
{
	if (station < 0 || station>3 || !CMjRef::IsValidateMj(pai))
		return false;
	if (m_byArHandPai[station][index] == 255)
		return false;
	m_byArHandPai[station][index] = pai;
	return true;
}

///�����Ƿ�����
bool GameData::IsHaveHandPaiFromIndex(BYTE station, int index)
{
	if (station < 0 || station>3 || index < 0 || index >= HAND_CARD_NUM)
	{
		return false;
	}
	if (m_byArHandPai[station][index] == 255)
	{
		return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///���ƣ�GetUserGangNum
///��������ȡ��Ҹ��Ƶĸ���(���ܣ����ܣ�����)
///@param &an ��������, &ming �������� ,  &bu ��������
///@return ���Ƶ��ܸ���
int GameData::GetUserGangNum(BYTE station, BYTE &an, BYTE &ming, BYTE &bu)
{
	if (station < 0 || station >= PLAY_COUNT)
		return 0;
	int all = 0;
	an = 0;
	ming = 0;
	bu = 0;
	for (int i = 0;i < 5;i++)
	{
		switch (m_UserGCPData[station][i].byType)
		{
		case ACTION_AN_GANG://����
			an++;
			break;
		case ACTION_BU_GANG://����
			bu++;
			break;
		case ACTION_MING_GANG://����
			ming++;
			break;
		}
	}
	all = ming + an + bu;
	return all;
}

///���ƣ�GetUserChiNum
///��������ҳ��ƴ���
///@param station ���λ��
///@return ���Ƶ��ܸ���
int GameData::GetUserChiNum(BYTE station)
{
	if (station < 0 || station >= PLAY_COUNT)
		return 0;
	int num = 0;
	for (int i = 0;i < 5;i++)
	{
		switch (m_UserGCPData[station][i].byType)
		{
		case ACTION_CHI://����
			num++;
			break;
		}
	}
	return num;
}

///���ƣ�GetUserPengNum
///��������Ҹ��ƴ���
///@param station ���λ��
///@return ���Ƶ��ܸ���
int GameData::GetUserPengNum(BYTE station)
{
	if (station < 0 || station >= PLAY_COUNT)
		return 0;
	int num = 0;
	for (int i = 0;i < 5;i++)
	{
		switch (m_UserGCPData[station][i].byType)
		{
		case ACTION_PENG://����
			num++;
			break;
		}
	}
	return num;
}

///���ƣ�IsUserHaveGangPai
///��������Ҹ���ĳ����
///@param pai ��, &type ����
///@return  true �� ,false ����
bool GameData::IsUserHaveGangPai(BYTE station, BYTE pai, BYTE &type)
{
	if (station < 0 || station >= PLAY_COUNT)
		return false;
	for (int i = 0;i < 5;i++)
	{
		if (m_UserGCPData[station][i].byType >= ACTION_AN_GANG && m_UserGCPData[station][i].byType <= ACTION_MING_GANG && m_UserGCPData[station][i].byData[0] == pai)
		{
			type = m_UserGCPData[station][i].byType;
			return true;
		}
	}
	return false;
}
///
///���ƣ�IsUserHavePengPai
///����������Ƿ�����ĳ����
///@param pai ��
///@return  true �� ,false ����
bool GameData::IsUserHavePengPai(BYTE station, BYTE pai)
{
	if (station < 0 || station >= PLAY_COUNT)
		return false;
	for (int i = 0;i < 5;i++)
	{
		if (ACTION_NO == m_UserGCPData[station][i].byType)
			continue;
		if (m_UserGCPData[station][i].byType == ACTION_PENG &&  m_UserGCPData[station][i].byData[0] == pai)
		{
			return true;
		}
	}
	return false;
}

///���ƣ�IsUserHaveChiPai
///����������Ƿ�Թ�ĳ����
///@param pai ��
///@return  true �� ,false ����
bool GameData::IsUserHaveChiPai(BYTE station, BYTE pai)
{
	if (station < 0 || station >= PLAY_COUNT)
		return false;
	for (int i = 0;i < 5;i++)
	{
		if (m_UserGCPData[station][i].byType == ACTION_CHI &&  m_UserGCPData[station][i].iOutpai == pai)
		{
			return true;
		}
	}
	return false;
}

///���ƣ�AddToGCP
///���������һ�����ݵ��ܳ���������
///@param gcpData Ҫ��ӵĳ���������
void GameData::AddToGCP(BYTE station, TCPGStruct *gcpData)
{
	if (station < 0 || station >= PLAY_COUNT)
		return;
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

///���ƣ�DeleteAGCPData
///������ɾ���ܳ���������ָ����������
///@param type ����, pai ��
void GameData::DeleteAGCPData(BYTE station, BYTE type, BYTE pai)
{
	if (station < 0 || station >= PLAY_COUNT)
		return;
	for (int i = 0;i < 5;i++)
	{
		if (m_UserGCPData[station][i].byType == type && (m_UserGCPData[station][i].byData[0] == pai || m_UserGCPData[station][i].byData[1] == pai || m_UserGCPData[station][i].byData[2] == pai))
		{
			m_UserGCPData[station][i].Init();
			break;
		}
	}
	ShortGCP(station);//����ܳ�������
}

///���ƣ�CopyGCPData
///�����������ܳ��������е�������
///@param gcpData ����������
///@return �����ܸ���
int GameData::CopyGCPData(BYTE station, TCPGStruct gcpData[])
{
	if (station < 0 || station >= PLAY_COUNT)
		return 0;
	//memcpy(gcpData,m_UserGCPData,sizeof(m_UserGCPData));
	for (int i = 0;i < 5;i++)
	{
		gcpData[i] = m_UserGCPData[station][i];
	}
	return GetGCPCount(station);
}

///���ƣ�SetGCPData
///���������øܳ��������е�������
///@param gcpData ����������
///@return 
void GameData::SetGCPData(BYTE station, TCPGStruct gcpData[])
{
	if (station < 0 || station >= PLAY_COUNT)
		return;
	for (int i = 0;i < 5;i++)
	{
		m_UserGCPData[station][i] = gcpData[i];
	}
}

///���ƣ�GetGCPCount
///��������ȡ�ܳ����Ľڵ����
///@param 
///@return ��ó������ܸ���
int GameData::GetGCPCount(BYTE station)
{
	if (station < 0 || station >= PLAY_COUNT)
		return 0;
	int count = 0;
	for (int i = 0;i < 5;i++)
	{
		if (ACTION_NO != m_UserGCPData[station][i].byType)
		{
			count++;
		}
	}
	return count;
}
///���ƣ�GetOnePaiGCPCount
///��������ȡ�ܳ�������ĳ���Ƶ�����
///@param 
///@return ĳ���Ƶ�����
int GameData::GetOnePaiGCPCount(BYTE station, BYTE pai)
{
	if (station < 0 || station >= PLAY_COUNT)
		return 0;
	int count = 0;
	for (int i = 0;i < 5;i++)
	{
		if (ACTION_NO == m_UserGCPData[station][i].byType)
			continue;
		for (int j = 0;j < 4;j++)
		{
			if (m_UserGCPData[station][i].byData[j] == pai && pai != 255)
			{
				count++;
			}
		}
	}
	return count;
}
///���ƣ�ShortGCP
///����������ܳ�������
///@param 
void GameData::ShortGCP(BYTE station)
{
	if (station < 0 || station >= PLAY_COUNT)
		return;
	int index = 0;
	TCPGStruct m_TempGCPData[5];
	for (int i = 0;i < 5;i++)
	{
		m_TempGCPData[i] = m_UserGCPData[station][i];
		m_UserGCPData[station][i].Init();
	}
	for (int i = 0;i < 5;i++)
	{
		if (ACTION_NO != m_TempGCPData[i].byType)
		{
			m_UserGCPData[station][index] = m_TempGCPData[i];
			index++;
		}
	}
}
///���ƣ�InitGCP
///��������ʼ���ܳ�������
///@param 
void GameData::InitGCP(BYTE station)
{
	if (station < 0 || station >= PLAY_COUNT)
		return;
	for (int i = 0;i < 5;i++)
	{
		m_UserGCPData[station][i].Init();
	}
}
//////////////////���Ʋ���///////////////////////////////////////////////////////////////////////////////

///���ƣ�AddToOutPai
///���������һ���Ƶ�����������
///@param pai ��
void GameData::AddToOutPai(BYTE station, BYTE pai, bool bTingCard)
{
	if (station < 0 || station >= PLAY_COUNT)
		return;
	for (int i = 0;i < OUT_CARD_NUM;i++)
	{
		if (m_byArOutPai[station][i] == 255)
		{
			if (bTingCard)
			{
				m_byTingOutCardIndex[station] = i;
			}
			else
			{
				m_byAllOutCardNum[pai] ++;
			}
			m_byArOutPai[station][i] = pai;
			break;
		}
	}
}
///���ƣ�DeleteLastOutPai
///������ɾ�����һ�ų���
///@param 
void GameData::DeleteLastOutPai(BYTE station)
{
	if (station < 0 || station >= PLAY_COUNT)
		return;
	for (int i = 0;i < OUT_CARD_NUM;i++)
	{
		if (m_byArOutPai[station][i] == 255 && i > 0)
		{
			m_byArOutPai[station][i - 1] = 255;
			break;
		}
	}
}

///���ƣ�SetOutPaiData
///���������ó�������
///@param pai[] ��������
///@return 
void GameData::SetOutPaiData(BYTE station, BYTE pai)
{
	if (station < 0 || station >= PLAY_COUNT)
	{
		return;
	}
	bool bAdd = false;
	for (int i = 0;i < OUT_CARD_NUM;i++)
	{
		if (255 == m_byArOutPai[station][i])
		{
			m_byArOutPai[station][i] = pai;
			m_byArOutPaiCount[station]++;
			break;
		}
	}
}

///���ƣ�CopyOutPai
///������������ҵĳ���
///@param pai[] ��������
///@return ���Ƶ�����
int GameData::CopyOutPai(BYTE station, BYTE pai[])
{
	if (station < 0 || station >= PLAY_COUNT)
		return 0;
	for (int i = 0;i < OUT_CARD_NUM;i++)
	{
		pai[i] = m_byArOutPai[station][i];
	}
	return CLogicBase::GetPaiNum(m_byArOutPai[station], OUT_CARD_NUM);
}

///���ƣ�GetOutPaiCount
///��������ȡ��ҵĳ�������
///@param 
///@return ���Ƶ�����
int GameData::GetOutPaiCount(BYTE station)
{
	if (station < 0 || station >= PLAY_COUNT)
		return 0;
	int num = 0;
	for (int i = 0;i < OUT_CARD_NUM;i++)
	{
		if (m_byArOutPai[station][i] != 255)
		{
			num++;
		}
	}
	return num;
}
///���ƣ�GetOutPaiCount
///��������ȡ���ĳ���Ƶĳ�������
///@param 
///@return ĳ���Ƴ��Ƶ�����
int GameData::GetOneOutPaiCount(BYTE station, BYTE pai)
{
	if (station < 0 || station >= PLAY_COUNT)
		return 0;
	int num = 0;
	for (int i = 0;i < OUT_CARD_NUM;i++)
	{
		if (m_byArOutPai[station][i] != 255 && m_byArOutPai[station][i] == pai)
		{
			num++;
		}
	}
	return num;
}
/////////////////////���Ʋ���////////////////////////////////////////////////////////

///���ƣ�AddToHuaPai
///���������һ�Ż��Ƶ�����������
///@param pai ����
void GameData::AddToHuaPai(BYTE station, BYTE pai)
{
	if (station < 0 || station >= PLAY_COUNT)
		return;
	for (int i = 0;i < HUA_CARD_NUM;i++)
	{
		if (m_byArHuaPai[station][i] == 255)
		{
			m_byArHuaPai[station][i] = pai;
			m_byArHuaPaiCount[station]++;
			break;
		}
	}
}

///���ƣ�SetHuaPaiData
///���������û�������
///@param pai[] ��������
///@return 
void GameData::SetHuaPaiData(BYTE station, BYTE pai[])
{
	if (station < 0 || station >= PLAY_COUNT)
		return;
	memcpy(m_byArHuaPai[station], pai, sizeof(m_byArHuaPai[station]));
}

///���ƣ�CopyHuaPai
///������������ҵĻ���
///@param pai[] ��������
///@return 
int GameData::CopyHuaPai(BYTE station, BYTE pai[])
{
	if (station < 0 || station >= PLAY_COUNT)
		return 0;
	int count = 0;
	for (int i = 0;i < HUA_CARD_NUM;i++)
	{
		pai[i] = m_byArHuaPai[station][i];
		if (m_byArHuaPai[station][i] != 255)
		{
			count++;
		}
	}
	return count;
}

///����Ƿ���
bool GameData::CheckIsHuaPai(BYTE pai)
{
	if (pai >= CMjEnum::MJ_TYPE_FCHUN && pai != 255)
		return true;
	return false;
}
///////////////////////���Ʋ���//////////////////////////////////////////////////////////////////

///���ƣ�SetMenPaiData
///������������������
///@param  pai[] ��������
///@return 
void GameData::SetMenPaiData(BYTE station, BYTE pai[])
{
	if (station < 0 || station >= 4)
		return;
	memcpy(m_iArMenPai[station], pai, sizeof(m_iArMenPai[station]));
}
///���ƣ�CopyMenPai
///������������ҵ�����
///@param  pai[][] ��������
///@return �����������
void GameData::CopyAllMenPai(BYTE pai[][MEN_CARD_NUM])
{
	for (int i = 0;i < 4;i++)
	{
		for (int j = 0;j < MEN_CARD_NUM;j++)
		{
			pai[i][j] = m_iArMenPai[i][j];
		}
	}
}
///���ƣ�CopyMenPai
///������������ҵ�����
///@param  pai[] ��������
///@return �����������
int GameData::CopyMenPai(BYTE station, BYTE pai[])
{
	if (station < 0 || station >= 4)
		return 0;
	for (int i = 0;i < MEN_CARD_NUM;i++)
	{
		pai[i] = m_iArMenPai[station][i];
	}
	return CLogicBase::GetPaiNum(m_iArMenPai[station], MEN_CARD_NUM);
}

///���ƣ�ChangeMenPai
///����������ת��
///@param  pai[] ��������
///@return �����������
void GameData::ChangeMenPai(int iIndex, BYTE byCard)
{
	int iCount_ = 0, i = 0;
	for (i = 0; i < 4; i++)
	{
		iCount_ += m_mjRule.byMenPaiNum[i];
		if (iIndex - iCount_ < 0)
		{
			break;
		}
	}
	m_iArMenPai[i][m_mjRule.byMenPaiNum[i] - (iCount_ - iIndex)] = byCard;
}

//��������������
void GameData::CreateMenPai()
{
	int iStart = 0;
	int iEnd = m_mjRule.byMenPaiNum[0];
	int index = 0;
	for (int i = iStart;i < iEnd;++i)//0λ�õ�����
	{
		if (m_MenPai.byMenPai[i] != 255)
		{
			m_iArMenPai[0][index++] = 0;
		}
		else
		{
			m_iArMenPai[0][index++] = 255;
		}
	}
	iStart = m_mjRule.byMenPaiNum[0];
	iEnd = m_mjRule.byMenPaiNum[0] + m_mjRule.byMenPaiNum[1];
	index = 0;
	for (int i = iStart;i < iEnd;++i)//1λ�õ�����
	{
		if (m_MenPai.byMenPai[i] != 255)
		{
			m_iArMenPai[1][index++] = 0;
		}
		else
		{
			m_iArMenPai[1][index++] = 255;
		}
	}
	iStart = m_mjRule.byMenPaiNum[0] + m_mjRule.byMenPaiNum[1];
	iEnd = m_mjRule.byMenPaiNum[0] + m_mjRule.byMenPaiNum[1] + m_mjRule.byMenPaiNum[2];
	index = 0;
	for (int i = iStart;i < iEnd;++i)//2λ�õ�����
	{
		if (m_MenPai.byMenPai[i] != 255)
		{
			m_iArMenPai[2][index++] = 0;
		}
		else
		{
			m_iArMenPai[2][index++] = 255;
		}
	}
	iStart = m_mjRule.byMenPaiNum[0] + m_mjRule.byMenPaiNum[1] + m_mjRule.byMenPaiNum[2];
	iEnd = m_mjRule.byMenPaiNum[0] + m_mjRule.byMenPaiNum[1] + m_mjRule.byMenPaiNum[2] + m_mjRule.byMenPaiNum[3];
	index = 0;
	for (int i = iStart;i < iEnd;++i)//3λ�õ�����
	{
		if (m_MenPai.byMenPai[i] != 255)
		{
			m_iArMenPai[3][index++] = 0;
		}
		else
		{
			m_iArMenPai[3][index++] = 255;
		}
	}
}

///��������ȡĳ��ҵ���������
int GameData::GetMenPaiCount(BYTE station)
{
	if (station < 0 || station >= 4)
		return false;
	int count = 0;
	for (int i = 0;i < MEN_CARD_NUM;++i)
	{
		if (m_iArMenPai[station][i] != 255)
		{
			count++;
		}
	}
	return count;
}

///���ƣ�ChangeAllHandPai
///����������
///@param  station λ��, pai[] ��, count������
void GameData::ChangeAllHandPai(BYTE station, BYTE pai[], BYTE count)
{
	if (station < 0 || station >= PLAY_COUNT)
		return;
	for (int i = 0;i < count;i++)
	{
		if (!CMjRef::IsValidateMj(pai[i]))
			continue;
		if (m_byArHandPai[station][i] == 255)
			break;
		m_byArHandPai[station][i] = pai[i];
	}
}

///���ƣ�ChangeAllHandPai
///��ָ������
///@param  station λ��, pai[] ��, handpai[]Ҫ����������, count������
void GameData::ChangeSomeHandPai(BYTE station, BYTE pai[], BYTE handpai[], BYTE count)
{
	if (station < 0 || station >= PLAY_COUNT)
		return;
	for (int i = 0;i < count;i++)
	{
		if (!CMjRef::IsValidateMj(pai[i]) || !CMjRef::IsValidateMj(handpai[i]))
			continue;
		if (IsHaveAHandPai(station, handpai[i]))
		{
			DeleteAHandPai(station, handpai[i]);
			AddToHandPai(station, pai[i]);
		}
	}
}

///���ƣ�SetGameNextPai
///������ǽ����һ����
///@param  pai ��
void GameData::SetGameNextPai(BYTE pai)
{
	if (m_MenPai.byStartIndex == 255)
		return;
	if (m_MenPai.byMenPai[m_MenPai.byStartIndex] == 255)
		return;
	m_MenPai.byMenPai[m_MenPai.byStartIndex] = pai;
}
