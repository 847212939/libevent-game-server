#include "stdafx.h"
#include "CheckHuPai.h"

CheckHuPai::CheckHuPai(void)
{
	InitData();
}

CheckHuPai::~CheckHuPai(void)
{

}
///���ƣ�InitData
///��������ʼ������
///@param 
///@return 
void CheckHuPai::InitData()
{
	///����Ƿ���������
	m_bZimo = false;
	m_byStation = 255;///��ǰ�����ҵ�λ��
	m_byBeStation = 255;
	m_tHuPaiStruct.Init();
	m_tHuTempData.Init();	//���Ƽ����ʱ����
	m_tNoJingHuTempData.Init();///���Ƽ����ʱ����(�޲���)	
	m_byJingNum = 0;  //��������
	//�����������ڵ���
	m_byLastPai = 255;
	ClearPingHuData();
	memset(m_byTypeFans, 0, sizeof(m_byTypeFans));
	m_iMaxFan = -1;
}
/*------------------------------------------------------------------------------*/
///���ƣ�ChangeHandPaiData
///������ת���������ݵĴ洢��ʽ��Ϊ�˷�����Ƽ��
///@param handpai[] ��������, pai ���˴������
///@return 
void CheckHuPai::ChangeHandPaiData(GameDataEx *pGameData, BYTE handpai[])
{
	m_tHuTempData.Init();
	m_tNoJingHuTempData.Init();///���Ƽ����ʱ����(�޲���)	
	m_byJingNum = 0;
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (handpai[i] != 255)
		{
			if (!pGameData->m_StrJing.CheckIsJing(handpai[i]))//������
			{
				m_tNoJingHuTempData.Add(handpai[i]);
			}
			else
			{
				m_byJingNum++;
			}
			m_tHuTempData.Add(handpai[i]);
		}
	}
}


///���ƣ�ChangeHandPaiData
///������ת���������ݵĴ洢��ʽ��Ϊ�˷�����Ƽ��
///@param handpai[] ��������, pai ���˴������
///@return 
void CheckHuPai::ChangeHandPaiDataEx(GameDataEx *pGameData, BYTE handpai[])
{
	m_tHuTempData.Init();
	m_tNoJingHuTempData.Init();///���Ƽ����ʱ����(�޲���)	
	m_byJingNum = 0;
	int iAddLaiZiToHand = 0;		//����ӵ�������
	bool bAddLaiZiFlag = false;
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (handpai[i] == 255)
			continue;
		//  �ǲ���		
		if ((iAddLaiZiToHand == 0 &&							//δ���� 
			m_bZimo == false &&								//������
			pGameData->m_StrJing.CheckIsJing(handpai[i]) &&//���
			handpai[i] == m_byLastPai))
		{
			bAddLaiZiFlag = true;
		}

		if (!pGameData->m_StrJing.CheckIsJing(handpai[i]) || bAddLaiZiFlag)//������
		{
			if (bAddLaiZiFlag)
			{
				iAddLaiZiToHand = 1;
				bAddLaiZiFlag = false;
			}
			m_tNoJingHuTempData.Add(handpai[i]);
		}
		else
		{
			m_byJingNum++;
		}
		m_tHuTempData.Add(handpai[i]);
	}
}

///���ƣ�AddAPaiToArr
///���������һ���Ƶ���ʱ������
///@param pai Ҫ�ӵ���
///@return 
void CheckHuPai::AddAPaiToArr(BYTE pai)
{
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (m_tHuTempData.data[i][0] == 255)
		{
			m_tHuTempData.data[i][0] = pai;
			m_tHuTempData.data[i][1] = 1;
			m_tHuTempData.count++;
			break;
		}
		else if (m_tHuTempData.data[i][0] == pai)
		{
			m_tHuTempData.data[i][1]++;
			break;
		}
	}
}

///���ƣ�SetJingPai
///���������þ��ƣ���������ֵ�����Ӿ�������
///@param 
///@return 
void CheckHuPai::SetJingPai()
{
}

///���ƣ�ReSetJingPai
///�������ָ����ƣ������ƻָ�����ʱ������
///@param 
///@return 
void CheckHuPai::ReSetJingPai()
{
}

///���ƣ�CanTing
///����������ܷ�����
///@param  handpai[] ��������, pai ���һ����, gcpData[] ����������, MainJing ����, LessJing ����, CanOut[] ����������Ƶ��ƣ������ж��ţ�
///@return true ������false ������
bool CheckHuPai::CanTing(GameDataEx *pGameData, BYTE station, BYTE CanOut[])
{
	if (!pGameData->m_mjAction.bTing)//��������
	{
		return false;
	}
	m_byStation = station;
	///����Ƿ���������
	m_bZimo = true;
	memset(m_byArHandPai, 255, sizeof(m_byArHandPai));
	memset(CanOut, 255, sizeof(CanOut));
	int num = 0;

	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		m_byArHandPai[i] = pGameData->m_byArHandPai[station][i];
	}
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (m_byArHandPai[i] == 255)
			break;
		BYTE pai = 255;
		pai = m_byArHandPai[i];
		m_byArHandPai[i] = 255;
		//��С��������
		CLogicBase::SelectSort(m_byArHandPai, HAND_CARD_NUM, false);
		///������ת��
		ChangeHandPaiData(pGameData, m_byArHandPai);

		if (CheckPingHu(pGameData))
		{
			CanOut[num] = pai;
			num++;
		}
		m_byArHandPai[HAND_CARD_NUM - 1] = pai;

	}
	if (num > 0)
		return true;

	return false;
}
///���ƣ�ChiTing
///����ܷ�����ƣ��Ժ�����
///@param  station ����λ��, CanOut[] ����������Ƶ��ƣ�ÿ�ų���ֻ����һ�ţ���paip[][3] �ܳԵ������
///@return true ������false ������
bool CheckHuPai::ChiTing(GameDataEx *pGameData, BYTE station, BYTE CanOut[], BYTE pai[][3])
{
	m_byStation = station;

	memset(CanOut, 255, sizeof(CanOut));

	int num = 0, num1 = 0;
	for (int i = 0;i < 3;i++)
	{
		if (pai[i][0] != 255)
		{
			num++;
		}
	}

	for (int i = 0;i < num;i++)
	{
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));
		for (int j = 0;j < HAND_CARD_NUM;j++)
		{
			m_byArHandPai[j] = pGameData->m_byArHandPai[station][j];
		}
		///ɾ������

		///��С��������
		CLogicBase::SelectSort(m_byArHandPai, HAND_CARD_NUM, false);

		for (int i = 0;i < HAND_CARD_NUM;i++)
		{
			if (m_byArHandPai[i] == 255 || CanOut[num] != 255)
				continue;
			BYTE pai = 255;
			pai = m_byArHandPai[i];
			m_byArHandPai[i] = 255;
			//��С��������
			CLogicBase::SelectSort(m_byArHandPai, HAND_CARD_NUM, false);
			///������ת��
			ChangeHandPaiData(pGameData, m_byArHandPai);

			if (CheckPingHu(pGameData))
			{
				CanOut[num] = pai;
				num1++;
			}
			m_byArHandPai[HAND_CARD_NUM - 1] = pai;
		}
	}
	if (num1 > 0)
		return true;
	return false;
}
///���ƣ�GangTing
///���ܺ��ܷ񱣴�����״̬
///@param  station ����λ��, CanTing[] ��Ӧ����ϸܺ��ܷ񱣴�����paip[][4] ���Ƶ����
///@return true ������false ������
bool CheckHuPai::GangTing(GameDataEx *pGameData, BYTE station, tagCPGNotify &cpg, BYTE ps, bool CanTing[])
{
	m_byStation = station;

	memset(m_byArHandPai, 255, sizeof(m_byArHandPai));
	memset(CanTing, 0, sizeof(CanTing));
	int num = 0, num1 = 0;
	for (int i = 0;i < 4;i++)
	{
		if (cpg.iGangData[i][1] != 255)
			num++;
	}
	for (int i = 0;i < num;i++)
	{
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));
		for (int j = 0;j < HAND_CARD_NUM;j++)
		{
			m_byArHandPai[j] = pGameData->m_byArHandPai[station][j];
		}
		///ɾ������

		///��С��������
		CLogicBase::SelectSort(m_byArHandPai, HAND_CARD_NUM, false);

		for (int i = 0;i < HAND_CARD_NUM;i++)
		{
			if (m_byArHandPai[i] == 255 || CanTing[num])
				continue;
			BYTE pai = 255;
			pai = m_byArHandPai[i];
			m_byArHandPai[i] = 255;
			//��С��������
			CLogicBase::SelectSort(m_byArHandPai, HAND_CARD_NUM, false);
			///������ת��
			ChangeHandPaiData(pGameData, m_byArHandPai);

			if (CheckPingHu(pGameData))
			{
				CanTing[num1] = true;
				num1++;
			}
			m_byArHandPai[HAND_CARD_NUM - 1] = pai;
		}
	}
	if (num1 > 0)
		return true;

	return false;
}
///���ƣ�CanHu
///��������ϸ������
///@param station ���Ƽ��λ�ã�hutype[] �ܺ��Ƶ����ͣ�zimo �Ƿ�����
///@return true  �ܺ��� false ����
bool CheckHuPai::CanHu(GameDataEx *pGameData, BYTE station, BYTE lastpai, bool zimo)
{
	//�����������ڵ���
	m_byLastPai = lastpai;
	m_byStation = station;
	memset(m_byArHandPai, 255, sizeof(m_byArHandPai));
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		m_byArHandPai[i] = pGameData->m_byArHandPai[station][i];
		if (pGameData->m_byArHandPai[station][i] == 255 && lastpai != 255 && !zimo)
		{
			m_byArHandPai[i] = lastpai;
			break;
		}
	}
	///��С��������
	CLogicBase::SelectSort(m_byArHandPai, HAND_CARD_NUM, false);
	bool bb = false;
	///������ת��
	ChangeHandPaiData(pGameData, m_byArHandPai);

	if (CheckPingHu(pGameData))
	{
		bb = true;
		//����Ͻ��
		///��ӳ���������
		//AddGCPData(pGameData->m_UserGCPData[station],&m_tHuPaiStruct );
		//if(zimo)
		//{
		//	SetAHupaiType(HUPAI_TYPE_ZiMo,hutype);///����
		//}
		//CheckPingHuData();
	}
	return bb;
}

///���ƣ�AddGCPData
///�����������������ݼ��뵽���ƽṹ��
///@param gcpData[] ��Ҹճ������� , tempstruct  �������ݽṹ
///@return 
void CheckHuPai::AddGCPData(TCPGStruct gcpData[], PingHuStruct * tempstruct)//m_tHuPaiStruct
{
	BYTE pai[4];
	bool jing[4];
	memset(jing, 0, sizeof(jing));
	for (int i = 0;i < 5;i++)
	{
		if (ACTION_NO == gcpData[i].byType)
			continue;
		for (int j = 0;j < 4;j++)
		{
			pai[j] = gcpData[i].byData[j];
		}
		tempstruct->AddData(gcpData[i].byType, pai, jing);
	}
}

///���ƣ�CheckPingHu
///������ƽ�����
///@param  jing �Ƿ����, FengShunZi �����ܷ���˳��, JianShunZi �����Ƿ�����˳��, AddJing �����һ�ž�������ʱ�ã�
///@return true �����ƽ���� false �������ƽ��
//ԭ��:ɾ��˳����->����->˳��
bool CheckHuPai::CheckPingHu(GameDataEx *pGameData)
{
	//// ͳ������
	//LARGE_INTEGER BegainTime ;     
	//LARGE_INTEGER EndTime ;     
	//LARGE_INTEGER Frequency ;     
	//QueryPerformanceFrequency(&Frequency);     
	//QueryPerformanceCounter(&BegainTime) ;

	//��ʼ������Ӧ����
	m_tTempHupaistruct.Init();
	m_tHuPaiStruct.Init();
	m_iMaxFan = -1;
	BYTE pai[4] = { 255 };
	bool caishen[4] = { 0 };
	memset(pai, 255, sizeof(pai));
	memset(caishen, 0, sizeof(caishen));
	//ӵ���������ϵĲ����ƣ�ѡ��������������
	if (m_byJingNum >= 2 && m_byJingNum != 255)//
	{
		m_tHuPaiStruct.Init();//��ʼ�����ƽṹ
		memset(caishen, 0, sizeof(caishen));
		memset(pai, 255, sizeof(pai));
		pai[0] = pGameData->m_StrJing.byPai[0];
		pai[1] = pGameData->m_StrJing.byPai[0];
		caishen[0] = true;
		caishen[1] = true;
		m_tHuPaiStruct.AddData(TYPE_JINGDIAO_JIANG, pai, caishen);//��ӽ�ͷ
		MakePingHu(pGameData, m_tNoJingHuTempData, m_tHuPaiStruct, m_byJingNum - 2);//���Ƽ��ݹ麯�����ڵݹ������ȫ�����
	}
	//1������ļ��,һ�������һ��������
	if (m_byJingNum >= 1 && m_byJingNum != 255)
	{
		for (int i = 0;i < m_tNoJingHuTempData.count;i++)
		{
			if (m_tNoJingHuTempData.data[i][1] < 1 || m_tNoJingHuTempData.data[i][1] == 255 || m_tNoJingHuTempData.data[i][0] == 255)
				continue;
			m_tHuPaiStruct.Init();//��ʼ�����ƽṹ
			m_tNoJingHuTempData.data[i][1] -= 1;
			memset(caishen, 0, sizeof(caishen));
			memset(pai, 255, sizeof(pai));
			pai[0] = m_tNoJingHuTempData.data[i][0];
			pai[1] = m_tNoJingHuTempData.data[i][0];
			caishen[0] = true;
			m_tHuPaiStruct.AddData(TYPE_JIANG_PAI, pai, caishen);
			MakePingHu(pGameData, m_tNoJingHuTempData, m_tHuPaiStruct, m_byJingNum - 1);//���Ƽ��ݹ麯�����ڵݹ������ȫ�����
			m_tNoJingHuTempData.data[i][1] += 1;
		}
	}
	//���������ļ��,(�޾����齫m_tHuTempData �� m_tNoJingHuTempData��һ��������)
	for (int i = 0;i < m_tNoJingHuTempData.count;i++)
	{
		if (m_tNoJingHuTempData.data[i][1] < 2 || m_tNoJingHuTempData.data[i][0] == 255)
			continue;
		m_tHuPaiStruct.Init();//��ʼ�����ƽṹ
		m_tNoJingHuTempData.data[i][1] -= 2;
		memset(caishen, 0, sizeof(caishen));
		memset(pai, 255, sizeof(pai));
		pai[0] = m_tNoJingHuTempData.data[i][0];
		pai[1] = m_tNoJingHuTempData.data[i][0];
		m_tHuPaiStruct.AddData(TYPE_JIANG_PAI, pai, caishen);
		MakePingHu(pGameData, m_tNoJingHuTempData, m_tHuPaiStruct, m_byJingNum);//���Ƽ��ݹ麯�����ڵݹ������ȫ�����
		m_tNoJingHuTempData.data[i][1] += 2;
	}

	////�������ʱ��
	//QueryPerformanceCounter(&EndTime);
	//GameLog::OutputFile("==CheckPingHu��ʱ��%.1f==",(double)( EndTime.QuadPart - BegainTime.QuadPart )/ Frequency.QuadPart * 1000000);

	if (m_iMaxFan >= 0)
	{
		m_tHuPaiStruct = m_tTempHupaistruct;
		return true;
	}
	return false;
}

///ƽ��������£���ȡ�����������
bool CheckHuPai::GetHuPaiTypes(GameDataEx *pGameData, bool zimo)
{
	//���ݲ�ͬ���齫������������˻������
	//��˷�����ʼ��Ϊ1�����1
	int fan = 0;
	BYTE byTempHuType[MAX_HUPAI_TYPE];
	ClearHupaiType(byTempHuType);
	bool bDaHu = false;
	CheckPingHuData(pGameData);//�Ȼ�ȡ�����

	if (CheckQingYiSe(pGameData))//��һɫ
	{
		bDaHu = true;
		fan = 9;
		SetAHupaiType(HUPAI_TYPE_QingYiSe, byTempHuType);
	}
	else if (CheckYiTiaoLong())//һ����
	{
		bDaHu = true;
		fan = 9;
		SetAHupaiType(MJ_HUPAI_TYPE_YI_TIAO_LONG, byTempHuType);
	}

	if (!bDaHu) //ƽ��������
	{
		SetAHupaiType(HUPAI_TYPE_PingHu, byTempHuType);
		fan = 1;
	}

	if (fan > m_iMaxFan)
	{
		m_iMaxFan = fan;
		m_tTempHupaistruct = m_tHuPaiStruct;
		memcpy(m_byHuType, byTempHuType, sizeof(m_byHuType));
	}
	return true;
}

//������ƽ������
//@param csnum ������, m_tHuPaiStruct ���ƽṹ, FengShunZi �����ܷ���˳��, JianShunZi �����Ƿ�����˳��
//@return true �����ƽ���� false �������ƽ��
bool CheckHuPai::MakePingHu(GameDataEx *pGameData, CheckHuStruct &PaiData, PingHuStruct &HuPaiStruct, int iJingnum)
{
	if (PaiData.GetAllPaiCount() <= 0)//�Ѿ�����
	{
		//������ͣ���¼�������
		return GetHuPaiTypes(pGameData, m_bZimo);
	}
	if (iJingnum > HAND_CARD_NUM)
	{
		iJingnum = 0;
	}
	//ȡ��
	BYTE pai = 255;
	int count = 0;
	for (int i = 0;i < PaiData.count;i++)
	{
		if (0 == PaiData.data[i][1])
		{
			continue;
		}
		pai = PaiData.data[i][0];   //��¼��ֵ
		count = PaiData.data[i][1]; //��¼����
		break;
	}
	BYTE data[4];
	bool caishen[4];
	memset(data, 255, sizeof(data));
	memset(caishen, 0, sizeof(caishen));
	data[0] = pai;
	data[1] = pai;
	data[2] = pai;
	//��ɾ��������ɾ��˳��
	if (iJingnum >= 2)//һ���ƺ� 2����
	{
		caishen[0] = true;
		caishen[1] = true;
		HuPaiStruct.AddData(TYPE_AN_KE, data, caishen);//�������
		PaiData.SetPaiCount(pai, count - 1);//������
		MakePingHu(pGameData, PaiData, HuPaiStruct, iJingnum - 2);//�ݹ�
		PaiData.SetPaiCount(pai, count);//�����ϻָ���
		HuPaiStruct.DeleteData(TYPE_AN_KE, data, caishen);//�ָ�����
	}
	if (iJingnum >= 1 && count >= 2)//һ�Ų���������������
	{
		memset(caishen, 0, sizeof(caishen));
		caishen[0] = true;
		HuPaiStruct.AddData(TYPE_AN_KE, data, caishen);//�������
		PaiData.SetPaiCount(pai, count - 2);//������
		MakePingHu(pGameData, PaiData, HuPaiStruct, iJingnum - 1);
		PaiData.SetPaiCount(pai, count);//�����ϻָ���
		HuPaiStruct.DeleteData(TYPE_AN_KE, data, caishen);//�ָ�����
	}

	if (count >= 3)//�����������
	{
		memset(caishen, 0, sizeof(caishen));
		HuPaiStruct.AddData(TYPE_AN_KE, data, caishen);//�������
		PaiData.SetPaiCount(pai, count - 3);//������
		MakePingHu(pGameData, PaiData, HuPaiStruct, iJingnum);
		PaiData.SetPaiCount(pai, count);//�����ϻָ���
		HuPaiStruct.DeleteData(TYPE_AN_KE, data, caishen);//�ָ�����
	}
	if (pai >= CMjEnum::MJ_TYPE_FD)//���ƣ�ֻ���ǿ���
		return false;
	//��ͬ��������˳��
	if (iJingnum >= 2 && CMjRef::GetCardPaiDian(pai) >= 3)//��� xx+pai ��˳�ӣ�x�ǲ���
	{
		memset(caishen, 0, sizeof(caishen));
		caishen[0] = true;
		caishen[1] = true;
		data[0] = pai - 2;
		data[1] = pai - 1;
		data[2] = pai;
		HuPaiStruct.AddData(TYPE_SHUN_ZI, data, caishen);//������ƣ�˳��
		PaiData.SetPaiCount(pai, count - 1);//������
		MakePingHu(pGameData, PaiData, HuPaiStruct, iJingnum - 2);
		PaiData.SetPaiCount(pai, count);//�����ϻָ���
		HuPaiStruct.DeleteData(TYPE_SHUN_ZI, data, caishen);//�ָ�����
	}

	if (iJingnum >= 1 && CMjRef::GetCardPaiDian(pai) >= 2 && CMjRef::GetCardPaiDian(pai) < 9 && PaiData.GetPaiCount(pai + 1) >= 1)//��� x+pai+(pai+1) ��˳�ӣ�x�ǲ���
	{
		memset(caishen, 0, sizeof(caishen));
		caishen[0] = true;
		data[0] = pai - 1;
		data[1] = pai;
		data[2] = pai + 1;
		HuPaiStruct.AddData(TYPE_SHUN_ZI, data, caishen);//������ƣ�˳��
		PaiData.SetPaiCount(pai, count - 1);//������
		PaiData.SetPaiCount(pai + 1, PaiData.GetPaiCount(pai + 1) - 1);//������
		MakePingHu(pGameData, PaiData, HuPaiStruct, iJingnum - 1);
		PaiData.SetPaiCount(pai, count);//�����ϻָ���
		PaiData.SetPaiCount(pai + 1, PaiData.GetPaiCount(pai + 1) + 1);//�����ϻָ���
		HuPaiStruct.DeleteData(TYPE_SHUN_ZI, data, caishen);//�ָ�����
	}

	if (iJingnum >= 2 && CMjRef::GetCardPaiDian(pai) >= 2 && CMjRef::GetCardPaiDian(pai) < 9)//��� x+pai+(x) ��˳�ӣ�x�ǲ���
	{
		memset(caishen, 0, sizeof(caishen));
		caishen[0] = true;
		caishen[2] = true;
		data[0] = pai - 1;
		data[1] = pai;
		data[2] = pai + 1;
		HuPaiStruct.AddData(TYPE_SHUN_ZI, data, caishen);//������ƣ�˳��
		PaiData.SetPaiCount(pai, count - 1);//������
		MakePingHu(pGameData, PaiData, HuPaiStruct, iJingnum - 2);
		PaiData.SetPaiCount(pai, count);//�����ϻָ���
		HuPaiStruct.DeleteData(TYPE_SHUN_ZI, data, caishen);//�ָ�����
	}

	if (CMjRef::GetCardPaiDian(pai) < 8 && PaiData.GetPaiCount(pai + 1) >= 1 && PaiData.GetPaiCount(pai + 2) >= 1)//��� pai+(pai+1)+(pai+2) ��˳��
	{
		memset(caishen, 0, sizeof(caishen));
		data[0] = pai;
		data[1] = pai + 1;
		data[2] = pai + 2;
		HuPaiStruct.AddData(TYPE_SHUN_ZI, data, caishen);//������ƣ�˳��
		PaiData.SetPaiCount(pai, count - 1);//������
		PaiData.SetPaiCount(pai + 1, PaiData.GetPaiCount(pai + 1) - 1);//������
		PaiData.SetPaiCount(pai + 2, PaiData.GetPaiCount(pai + 2) - 1);//������
		MakePingHu(pGameData, PaiData, HuPaiStruct, iJingnum);
		PaiData.SetPaiCount(pai, count);//�����ϻָ���
		PaiData.SetPaiCount(pai + 1, PaiData.GetPaiCount(pai + 1) + 1);//������
		PaiData.SetPaiCount(pai + 2, PaiData.GetPaiCount(pai + 2) + 1);//������
		HuPaiStruct.DeleteData(TYPE_SHUN_ZI, data, caishen);//�ָ�����	
	}

	if (iJingnum >= 1 && CMjRef::GetCardPaiDian(pai) < 8 && PaiData.GetPaiCount(pai + 1) < 1 && PaiData.GetPaiCount(pai + 2) >= 1)//��� pai+(x)+(pai+2) ��˳��
	{
		memset(caishen, 0, sizeof(caishen));
		caishen[1] = true;
		data[0] = pai;
		data[1] = pai + 1;
		data[2] = pai + 2;
		HuPaiStruct.AddData(TYPE_SHUN_ZI, data, caishen);//������ƣ�˳��
		PaiData.SetPaiCount(pai, count - 1);//������
		PaiData.SetPaiCount(pai + 2, PaiData.GetPaiCount(pai + 2) - 1);//������
		MakePingHu(pGameData, PaiData, HuPaiStruct, iJingnum - 1);
		PaiData.SetPaiCount(pai, count);//�����ϻָ���
		PaiData.SetPaiCount(pai + 2, PaiData.GetPaiCount(pai + 2) + 1);//������
		HuPaiStruct.DeleteData(TYPE_SHUN_ZI, data, caishen);//�ָ�����	
	}

	if (iJingnum >= 1 && CMjRef::GetCardPaiDian(pai) < 8 && PaiData.GetPaiCount(pai + 1) >= 1 && PaiData.GetPaiCount(pai + 2) < 1)//��� pai+(pai+1)+(x) ��˳��
	{
		memset(caishen, 0, sizeof(caishen));
		caishen[2] = true;
		data[0] = pai;
		data[1] = pai + 1;
		data[2] = pai + 2;
		HuPaiStruct.AddData(TYPE_SHUN_ZI, data, caishen);//������ƣ�˳��
		PaiData.SetPaiCount(pai, count - 1);//������
		PaiData.SetPaiCount(pai + 1, PaiData.GetPaiCount(pai + 1) - 1);//������
		MakePingHu(pGameData, PaiData, HuPaiStruct, iJingnum - 1);
		PaiData.SetPaiCount(pai, count);//�����ϻָ���
		PaiData.SetPaiCount(pai + 1, PaiData.GetPaiCount(pai + 1) + 1);//������
		HuPaiStruct.DeleteData(TYPE_SHUN_ZI, data, caishen);//�ָ�����	
	}

	if (iJingnum >= 2 && CMjRef::GetCardPaiDian(pai) < 8)//��� pai+(x)+(x) ��˳��
	{
		memset(caishen, 0, sizeof(caishen));
		caishen[1] = true;
		caishen[2] = true;
		data[0] = pai;
		data[1] = pai + 1;
		data[2] = pai + 2;
		HuPaiStruct.AddData(TYPE_SHUN_ZI, data, caishen);//������ƣ�˳��
		PaiData.SetPaiCount(pai, count - 1);//������
		MakePingHu(pGameData, PaiData, HuPaiStruct, iJingnum - 2);
		PaiData.SetPaiCount(pai, count);//�����ϻָ���
		HuPaiStruct.DeleteData(TYPE_SHUN_ZI, data, caishen);//�ָ�����	
	}
	return false;
}

///�������ָ�����,�Ѽ������ƻ�ԭ
///@param num[] ��ʱ�����ԭ������
///@return 
void CheckHuPai::ReSetPaiNum(BYTE num[], CheckHuStruct &TempPai)
{
	for (int i = 0;i < TempPai.count;i++)
	{
		TempPai.data[i][1] = num[i];
	}
}

///��������������Ƿ�ͬһ��ɫ
///@param pai1, pai2 Ҫ����������
///@return true �ǣ� false ����
bool CheckHuPai::CheckTongSe(BYTE pai1, BYTE pai2)
{
	int count = 0;
	if (pai2 % 10 == 0 || pai1 / 10 != pai2 / 10)
		return false;
	return true;
}

///���ƣ�CheckLianPai
///��������������Ƿ�����,num �����ţ�1���������ţ�2������������
///@param pai1, pai2 Ҫ�����ƣ� num ���������
///@return true �����ţ�flse ��������
bool CheckHuPai::CheckLianPai(GameDataEx *pGameData, BYTE pai1, BYTE pai2, BYTE num)
{
	if (!CheckTongSe(pai1, pai2))
		return false;
	if (num == 1)
	{
		if (pai1 == pai2 + 1 || pai1 == pai2 - 1)
		{
			return true;
		}
	}
	else if (num == 2)
	{
		if (pai1 == pai2 + 1 || pai1 == pai2 - 1 || pai1 == pai2 + 2 || pai1 == pai2 - 2)
		{
			if (!pGameData->m_mjAction.bChiFeng && (pai1 >= CMjEnum::MJ_TYPE_FD && pai1 <= CMjEnum::MJ_TYPE_FB || pai2 >= CMjEnum::MJ_TYPE_FD && pai2 <= CMjEnum::MJ_TYPE_FB))
				return false;
			if (!pGameData->m_mjAction.bChiJian && (pai1 >= CMjEnum::MJ_TYPE_ZHONG && pai1 <= CMjEnum::MJ_TYPE_BAI || pai2 >= CMjEnum::MJ_TYPE_ZHONG && pai2 <= CMjEnum::MJ_TYPE_BAI))
				return false;
			if (pai2 >= CMjEnum::MJ_TYPE_FD && pai2 <= CMjEnum::MJ_TYPE_FB)///��������
			{
				if (pai1 > CMjEnum::MJ_TYPE_FB || pai1 < CMjEnum::MJ_TYPE_FD)
					return false;
			}
			else if (pai2 >= CMjEnum::MJ_TYPE_ZHONG && pai2 <= CMjEnum::MJ_TYPE_BAI)///�з���
			{
				if (pai1 > CMjEnum::MJ_TYPE_BAI || pai1 < CMjEnum::MJ_TYPE_ZHONG)
					return false;
			}
			return true;
		}
	}
	return false;
}

///���ƣ�SetAHupaiType
///���������һ�ֺ�������
///@param   type Ҫ��ӵ����� , hupai[] ������������
///@return 
void CheckHuPai::SetAHupaiType(BYTE type, BYTE hupai[])
{
	if (CheckHupaiType(type, hupai))//���˾Ͳ������
		return;
	for (int i = 0;i < MAX_HUPAI_TYPE;i++)
	{
		if (hupai[i] == 255)
		{
			hupai[i] = type;
			break;
		}
	}
}

///���ƣ�ReSetAHupaiType
///����������һ�ֺ�������
///@param type Ҫɾ�������� , hupai[] ������������
///@return 
void CheckHuPai::ReSetAHupaiType(BYTE type, BYTE hupai[])
{
	int index = 0;
	for (int i = 0;i < MAX_HUPAI_TYPE;i++)
	{
		if (hupai[i] == type)
		{
			hupai[i] = 255;
			break;
		}
	}
	//ȥ���м���ЧԪ��
	BYTE temp[MAX_HUPAI_TYPE];
	memset(temp, 255, sizeof(temp));
	for (int i = 0;i < MAX_HUPAI_TYPE;i++)
	{
		if (hupai[i] != 255)
		{
			temp[index] = hupai[i];
			index++;
		}
	}
	for (int i = 0;i < MAX_HUPAI_TYPE;i++)
	{
		hupai[i] = temp[i];
	}
}

///���ƣ�ClearHupaiType
///������������к�������
///@param type Ҫɾ�������� , hupai[] ������������
///@return 
void CheckHuPai::ClearHupaiType(BYTE hupai[])
{
	for (int i = 0;i < MAX_HUPAI_TYPE;i++)
	{
		hupai[i] = 255;
	}
}

///���ƣ�CheckHupaiType
///����������Ƿ����ĳ�ֺ�������
///@param type Ҫ�������� , hupai[] ������������
///@return true ���ڣ�flse ������
bool CheckHuPai::CheckHupaiType(BYTE type, BYTE hupai[])
{
	int index = 0;
	for (int i = 0;i < MAX_HUPAI_TYPE;i++)
	{
		if (hupai[i] == type && type != 255)
		{
			return true;
		}
	}
	return false;
}
///���ƣ�ClearPingHuData
///�����������������
///@param 
///@return 
void CheckHuPai::ClearPingHuData()
{
	m_byJiang = 255;			//��������
	m_byShunNum = 0;			//˳�ӵĸ���
	memset(m_byShunData, 255, sizeof(m_byShunData));//˳�ӵ�����	
	m_byKeziNum = 0;			//���ӵĸ���	
	memset(m_byKeziData, 255, sizeof(m_byKeziData));//���ӵ�����
	m_byGangNum = 0;			//���Ƶĸ���
	memset(m_byGangData, 255, sizeof(m_byGangData));	//���Ƶ�����
	m_byChiNum = 0;				//���Ƶĸ���
	m_byPengNum = 0;			//���Ƶĸ���

	m_byHandShunNum = 0;			//����˳�ӵĸ�������������
	memset(m_byHandShunData, 255, sizeof(m_byHandShunData));//˳�ӵ����ݣ���������
}

///���ƣ�CheckPingHuData
///���������ƽ��������ϣ����ƣ��ԣ����ܣ�˳�ӣ����̵�
///@param 
///@return 
void CheckHuPai::CheckPingHuData(GameDataEx *pGameData)
{
	ClearPingHuData();
	BYTE num = 0;
	for (int i = 0;i < m_tHuPaiStruct.count;i++)
	{
		switch (m_tHuPaiStruct.byType[i])
		{
		case TYPE_JINGDIAO_JIANG:
		case TYPE_JIANG_PAI:
		{
			m_byJiang = m_tHuPaiStruct.data[i][0];
		}
		break;
		case TYPE_SHUN_ZI:
		{
			m_byShunData[m_byShunNum] = m_tHuPaiStruct.data[i][0];
			m_byShunNum++;

			m_byHandShunData[m_byHandShunNum] = m_tHuPaiStruct.data[i][0];
			m_byHandShunNum++;
		}
		break;
		case TYPE_AN_KE:
		{
			m_byKeziData[m_byKeziNum] = m_tHuPaiStruct.data[i][0];
			m_byKeziNum++;
		}
		break;
		default:
			break;
		}
	}
	BYTE pai[4];
	memset(pai, 255, sizeof(pai));
	for (int i = 0;i < 5;i++)
	{
		if (ACTION_NO == pGameData->m_UserGCPData[m_byStation][i].byType)
			continue;
		for (int j = 0;j < 4;++j)
		{
			pai[j] = pGameData->m_UserGCPData[m_byStation][i].byData[j];
		}
		switch (pGameData->m_UserGCPData[m_byStation][i].byType)
		{
		case ACTION_CHI://���ƶ���
		{
			m_byShunData[m_byShunNum] = pai[0];
			m_byShunNum++;
			m_byChiNum++;
		}
		break;
		case ACTION_PENG://���ƶ���
		{
			m_byKeziData[m_byKeziNum] = pai[0];
			m_byKeziNum++;
			m_byPengNum++;
		}
		break;
		case ACTION_AN_GANG://���ܶ���
		{
			m_byGangData[m_byGangNum][1] = pai[0];
			m_byGangData[m_byGangNum][0] = ACTION_AN_GANG;
			m_byGangNum++;
		}
		break;
		case ACTION_BU_GANG://���ܶ���
		{
			m_byGangData[m_byGangNum][1] = pai[0];
			m_byGangData[m_byGangNum][0] = ACTION_BU_GANG;
			m_byGangNum++;
		}
		break;
		case ACTION_MING_GANG://���ܶ���
		{
			m_byGangData[m_byGangNum][1] = pai[0];
			m_byGangData[m_byGangNum][0] = ACTION_MING_GANG;
			m_byGangNum++;
		}
		break;
		}
	}
}
/////////////////��ϸ�������ͼ��//////////////////////////////////////////////////////////
//�����
bool CheckHuPai::CheckTianHu(GameDataEx *pGameData)
{
	if (m_byStation != pGameData->m_byNtStation || !m_bZimo)
	{
		return false;
	}
	if (pGameData->m_bHaveOut[m_byStation] || pGameData->GetGCPCount(m_byStation) > 0)
	{
		return false;
	}
	return true;
}
//�غ���
bool CheckHuPai::CheckDiHu(GameDataEx *pGameData)
{
	if (m_byStation == pGameData->m_byNtStation)
	{
		return false;
	}
	if (pGameData->m_bHaveOut[m_byStation] || pGameData->GetGCPCount(m_byStation) > 0 || pGameData->GetOutPaiCount(m_byStation) > 0)
	{
		return false;
	}
	return true;
}
//���ϻ�������ץ�����Ƴɺ���(����������)�������� 
bool CheckHuPai::CheckGangKai(GameDataEx *pGameData)
{
	if (m_byStation == pGameData->m_byNowOutStation && pGameData->m_bGangState[m_byStation])
	{
		return true;
	}
	return false;
}
//�ܺ���:��һ����,�������Ժ�һ���Ƶ�����,��ʱ�򶪳�ȥ������������˺���
bool CheckHuPai::CheckGangHouPao(GameDataEx *pGameData)
{
	if (!m_bZimo && m_byBeStation != 255 && pGameData->m_bGangState[m_byBeStation])//���ڸ���״̬
	{
		return true;
	}
	return false;
}

//һ��ߣ���һ�ֻ�ɫ2����ͬ��˳����ɵ��� ,���������ƣ�
bool CheckHuPai::CheckYiBanGao()
{
	if (m_byShunNum >= 2)
	{
		for (int i = 0;i < m_byShunNum - 1;++i)
		{
			for (int j = i + 1;j < m_byShunNum;j++)
			{
				if (m_byShunData[i] == m_byShunData[j] && m_byShunData[i] != 255)
					return true;
			}
		}
	}
	return false;
}

//ϲ��꣺2�ֻ�ɫ2��������ͬ��˳��  ,���������ƣ�
bool CheckHuPai::CheckXiXiangFeng()
{
	BYTE shunzi[MAX_CPG_NUM], num = 0, count = 0;
	memset(shunzi, 255, sizeof(shunzi));
	for (int i = 0;i < MAX_CPG_NUM;++i)
	{
		shunzi[i] = m_byShunData[i];
	}
	if (m_byShunNum == 4)
	{
		for (int i = 0;i < m_byShunNum - 1;++i)
		{
			for (int j = i + 1;j < m_byShunNum;j++)
			{
				if (shunzi[i] == shunzi[j] && shunzi[i] != 0)
				{
					shunzi[i] = 0;
					shunzi[j] = 0;
					count++;
				}
			}
		}
		if (count >= 2)
		{
			return true;
		}
	}
	return false;
}

//������һ�ֻ�ɫ6�������ӵ������� 
bool CheckHuPai::CheckLianLiu()
{
	if (m_byShunNum >= 2)
	{
		for (int i = 0;i < m_byShunNum - 1;++i)
		{
			for (int j = i + 1;j < m_byShunNum;j++)
			{
				if (m_byShunData[i] == m_byShunData[j] + 3 && m_byShunData[i] != 255 && CMjRef::CheckIsTongSe(m_byShunData[i], m_byShunData[j]))
				{
					return true;
				}
			}
		}
	}
	return false;
}

//���ٸ���һ�ֻ�ɫ�Ƶ�123��789����˳��  ,���������ƣ�
bool CheckHuPai::CheckLaoShaoFu()
{
	if (m_byShunNum >= 2)
	{
		for (int i = 0;i < m_byShunNum - 1;++i)
		{
			for (int j = i + 1;j < m_byShunNum;j++)
			{
				if (m_byShunData[i] % 10 == 1 && m_byShunData[j] % 10 == 9 && CMjRef::CheckIsTongSe(m_byShunData[i], m_byShunData[j]))
				{
					return true;
				}
				if (m_byShunData[i] % 10 == 9 && m_byShunData[j] % 10 == 1 && CMjRef::CheckIsTongSe(m_byShunData[i], m_byShunData[j]))
				{
					return true;
				}
			}
		}
	}
	return false;
}

//�۾ſ̣�3����ͬ��һ���������Ƽ�������ɵĿ���(���) 
bool CheckHuPai::CheckYaoJiuKe()
{
	for (int i = 0;i < m_byKeziNum;++i)
	{
		if ((m_byKeziData[i] % 10 == 1 || m_byKeziData[i] % 10 == 9) && m_byKeziData[i] < CMjEnum::MJ_TYPE_FD)
			return true;
	}
	for (int i = 0;i < m_byGangNum;++i)
	{
		if ((m_byGangData[i][1] % 10 == 1 || m_byGangData[i][1] % 10 == 9) && m_byGangData[i][1] < CMjEnum::MJ_TYPE_FD)
			return true;
	}
	return false;
}

//���ܣ��Լ��а��̣������˴����һ����ͬ���ƿ��ܣ����Լ�ץ��һ��������������ͬ���ƿ��� 
bool CheckHuPai::CheckMingGang()
{
	for (int i = 0;i < m_byGangNum;++i)
	{
		if (m_byGangData[i][1] == ACTION_MING_GANG || m_byGangData[i][1] == ACTION_BU_GANG)
			return true;
	}
	return false;
}

//ȱһ�ţ�������ȱ��һ�ֻ�ɫ������ 
bool CheckHuPai::CheckQueYiMen()
{
	bool hua[4];
	BYTE num = 0;
	memset(hua, 0, sizeof(hua));
	if (m_byJiang != 255)
	{
		hua[CMjRef::GetHuaSe(m_byJiang)] = true;
	}
	for (int i = 0;i < m_byShunNum;++i)
	{
		if (m_byShunData[i] != 255 && m_byShunData[i] < CMjEnum::MJ_TYPE_FD)
		{
			hua[CMjRef::GetHuaSe(m_byShunData[i])] = true;
		}
	}
	for (int i = 0;i < m_byKeziNum;++i)
	{
		if (m_byKeziData[i] != 255 && m_byKeziData[i] < CMjEnum::MJ_TYPE_FD)
		{
			hua[CMjRef::GetHuaSe(m_byKeziData[i])] = true;
		}
	}
	for (int i = 0;i < m_byGangNum;++i)
	{
		if (m_byGangData[i][1] != 255 && m_byGangData[i][1] < CMjEnum::MJ_TYPE_FD)
		{
			hua[CMjRef::GetHuaSe(m_byGangData[i][1])] = true;
		}
	}
	for (int i = 0;i < 3;++i)
	{
		if (hua[i])
		{
			num++;
		}
	}
	if (num == 2)
	{
		return true;
	}
	return false;
}

//���֣�������û�з硢���� 
bool CheckHuPai::CheckWuZi()
{
	for (int i = 0;i < m_byKeziNum;++i)
	{
		if (m_byKeziData[i] != 255 && m_byKeziData[i] >= CMjEnum::MJ_TYPE_FD)
			return false;
	}
	for (int i = 0;i < m_byGangNum;++i)
	{
		if (m_byGangData[i][1] != 255 && m_byGangData[i][1] >= CMjEnum::MJ_TYPE_FD)
			return false;
	}
	return true;
}

//���ţ�����123��3��789��7��1233��3��77879��7��Ϊ�š�(δ����������12345��3��56789��6������� )
bool CheckHuPai::CheckBianZhang(GameDataEx *pGameData)
{
	if (pGameData->GetOutPaiCount(pGameData->m_byNtStation) == 0 && m_byChiNum == 0 && m_byPengNum == 0 && m_byGangNum == 0)
	{//�������
		return false;
	}
	if (m_byLastPai >= CMjEnum::MJ_TYPE_FD || (m_byLastPai % 10 != 3 && m_byLastPai % 10 != 7))
	{//���ץ�����������ƣ����3,7���ƣ����ǽ���
		return false;
	}
	for (int i = 0;i < m_tHuPaiStruct.count;++i)
	{
		if (m_tHuPaiStruct.byType[i] != TYPE_SHUN_ZI)
		{
			continue;
		}
		if (m_tHuPaiStruct.data[i][2] % 10 == 3 && m_tHuPaiStruct.data[i][2] == m_byLastPai)
		{
			return true;
		}
		if (m_tHuPaiStruct.data[i][0] % 10 == 7 && m_tHuPaiStruct.data[i][0] == m_byLastPai)
		{
			return true;
		}
	}
	return false;
}

//���ţ���2����֮����ơ�4556��5ҲΪ���š�δ����������45567��6���㿲�� 
bool CheckHuPai::CheckKanZhang(GameDataEx *pGameData)
{
	if (pGameData->GetOutPaiCount(pGameData->m_byNtStation) == 0 && m_byChiNum == 0 && m_byPengNum == 0 && m_byGangNum == 0)
	{//�������
		return false;
	}
	if (m_byLastPai >= CMjEnum::MJ_TYPE_FD)
	{//���ץ������������
		return false;
	}
	for (int i = 0;i < m_tHuPaiStruct.count;++i)
	{
		if (m_tHuPaiStruct.byType[i] != TYPE_SHUN_ZI)
		{
			continue;
		}
		if (m_tHuPaiStruct.data[i][1] == m_byLastPai)
		{
			return true;
		}
	}
	return false;
}

//���������������������ɺ� 
bool CheckHuPai::CheckDanDiaoJiang(GameDataEx *pGameData)
{
	if (pGameData->GetOutPaiCount(pGameData->m_byNtStation) == 0 && m_byChiNum == 0 && m_byPengNum == 0 && m_byGangNum == 0)
	{//�������
		return false;
	}
	for (int i = 0;i < m_tHuPaiStruct.count;++i)
	{
		if (m_tHuPaiStruct.byType[i] == TYPE_JIANG_PAI || m_tHuPaiStruct.byType[i] == TYPE_JINGDIAO_JIANG)
		{
			if (m_tHuPaiStruct.data[i][0] == m_byLastPai)
			{
				return true;
			}
		}
	}
	return false;
}

//���̣����С�������3����ͬ������ɵĿ��� ,���ܲ��㣩
bool CheckHuPai::CheckJianKe()
{
	for (int i = 0;i < m_byKeziNum;++i)
	{
		if (m_byKeziData[i] == CMjEnum::MJ_TYPE_ZHONG || m_byKeziData[i] == CMjEnum::MJ_TYPE_FA || m_byKeziData[i] == CMjEnum::MJ_TYPE_BAI)
		{
			return true;
		}
	}
	for (int i = 0;i < m_byGangNum;++i)
	{
		if (m_byGangData[i][1] == CMjEnum::MJ_TYPE_ZHONG || m_byGangData[i][1] == CMjEnum::MJ_TYPE_FA || m_byGangData[i][1] == CMjEnum::MJ_TYPE_BAI)
		{
			return true;
		}
	}
	return false;
}

//Ȧ��̣���Ȧ����ͬ�ķ�� �����ܲ��㣩
bool CheckHuPai::CheckQuanFengKe(GameDataEx *pGameData)
{
	if (pGameData->m_byQuanFeng == 255)
		return false;
	for (int i = 0;i < m_byKeziNum;++i)
	{
		if (m_byKeziData[i] == pGameData->m_byQuanFeng)
		{
			return true;
		}
	}
	for (int i = 0;i < m_byGangNum;++i)
	{
		if (m_byGangData[i][1] == pGameData->m_byQuanFeng)
		{
			return true;
		}
	}
	return false;
}

//�ŷ�̣��뱾�ŷ���ͬ�ķ�� ,���ܲ��㣩
bool CheckHuPai::CheckMenFengKe()
{

	return false;
}

//��ǰ�壺û�гԡ��������ܣ��ͱ��˴������ 
bool CheckHuPai::CheckMenQianQing(GameDataEx *pGameData)
{
	if (m_byStation == pGameData->m_byNowOutStation)
	{
		return false;
	}
	if (m_byChiNum > 0 || m_byPengNum > 0)
	{
		return false;
	}
	for (int i = 0;i < m_byGangNum;++i)
	{
		if (m_byGangData[i][0] != ACTION_AN_GANG)
		{
			return false;
		}
	}
	return true;
}

//�Ĺ�һ�� �����У���4����ͬ���ƹ���һ�ҵ�˳�����ӡ��ԡ�������(����������) 
bool CheckHuPai::CheckSiGuiYi(GameDataEx *pGameData)
{
	if (m_byStation == pGameData->m_byNowOutStation)//����
	{
		for (int i = 0;i < m_tHuPaiStruct.count;++i)
		{
			if (m_tHuPaiStruct.byType[i] == TYPE_JIANG_PAI || m_tHuPaiStruct.byType[i] == TYPE_JINGDIAO_JIANG || m_tHuPaiStruct.byType[i] == TYPE_AN_KE)
			{
				if (pGameData->GetAHandPaiCount(m_byStation, m_tHuPaiStruct.data[i][0]) == 4)
					return true;
			}
			else if (m_tHuPaiStruct.byType[i] == TYPE_SHUN_ZI)
			{
				if (pGameData->GetAHandPaiCount(m_byStation, m_tHuPaiStruct.data[i][0]) == 4)
					return true;
				if (pGameData->GetAHandPaiCount(m_byStation, m_tHuPaiStruct.data[i][1]) == 4)
					return true;
				if (pGameData->GetAHandPaiCount(m_byStation, m_tHuPaiStruct.data[i][2]) == 4)
					return true;
			}
		}
	}
	else
	{
		for (int i = 0;i < m_tHuPaiStruct.count;++i)
		{
			if (m_tHuPaiStruct.byType[i] == TYPE_JIANG_PAI || m_tHuPaiStruct.byType[i] == TYPE_JINGDIAO_JIANG || m_tHuPaiStruct.byType[i] == TYPE_AN_KE)
			{
				if (pGameData->GetAHandPaiCount(m_byStation, m_tHuPaiStruct.data[i][0]) == 4
					|| (pGameData->GetAHandPaiCount(m_byStation, m_tHuPaiStruct.data[i][0]) == 3 && m_byLastPai == m_tHuPaiStruct.data[i][0]))
					return true;
			}
			else if (m_tHuPaiStruct.byType[i] == TYPE_SHUN_ZI)
			{
				if (pGameData->GetAHandPaiCount(m_byStation, m_tHuPaiStruct.data[i][0]) == 4
					|| (pGameData->GetAHandPaiCount(m_byStation, m_tHuPaiStruct.data[i][0]) == 3 && m_byLastPai == m_tHuPaiStruct.data[i][0]))
					return true;
				if (pGameData->GetAHandPaiCount(m_byStation, m_tHuPaiStruct.data[i][1]) == 4
					|| (pGameData->GetAHandPaiCount(m_byStation, m_tHuPaiStruct.data[i][1]) == 3 && m_byLastPai == m_tHuPaiStruct.data[i][1]))
					return true;
				if (pGameData->GetAHandPaiCount(m_byStation, m_tHuPaiStruct.data[i][2]) == 4
					|| (pGameData->GetAHandPaiCount(m_byStation, m_tHuPaiStruct.data[i][2]) == 3 && m_byLastPai == m_tHuPaiStruct.data[i][2]))
					return true;
			}
		}
	}
	return false;
}

//˫ͬ�̣� 2��������ͬ�Ŀ��� ,�����������ƣ�
bool CheckHuPai::CheckShuangTongKe()
{
	if (m_byKeziNum < 2)
	{
		return false;
	}

	BYTE num[9];
	memset(num, 0, sizeof(num));
	for (int i = 0;i < m_byKeziNum;++i)
	{
		if (m_byKeziData[i] >= CMjEnum::MJ_TYPE_FD)
			continue;
		num[m_byKeziData[i] % 10 - 1]++;

		if (num[m_byKeziData[i] % 10 - 1] >= 2)
		{
			return true;
		}
	}
	return false;
}

//˫���̣�2������ ,����˵��ڵ���,�������������ƣ�
bool CheckHuPai::CheckShuangAnGang()
{
	if ((m_byKeziNum - m_byPengNum) == 2)
		return true;
	return false;
}

//���ܣ���ץ4����ͬ���ƿ��� 
bool CheckHuPai::CheckAnGang()
{
	for (int i = 0;i < m_byGangNum;++i)
	{
		if (m_byGangData[i][0] == ACTION_AN_GANG)
		{
			return true;
		}
	}
	return false;
}

//���ۣ�������û��һ���ż����� 
bool CheckHuPai::CheckDuanYao(GameDataEx *pGameData)
{
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (m_byArHandPai[i] == 255)
			continue;
		if (m_byArHandPai[i] >= CMjEnum::MJ_TYPE_FD || m_byArHandPai[i] % 10 == 1 || m_byArHandPai[i] == 9)
			return false;
	}
	for (int i = 0;i < 5;++i)
	{
		if (ACTION_NO == pGameData->m_UserGCPData[m_byStation][i].byType)
			continue;
		if (pGameData->m_UserGCPData[m_byStation][i].byData[0] >= CMjEnum::MJ_TYPE_FD || pGameData->m_UserGCPData[m_byStation][i].byData[0] % 10 == 1 || pGameData->m_UserGCPData[m_byStation][i].byData[0] % 10 == 9)
			return false;
	}
	return true;
}

//ȫ���ۣ�����ʱ��ÿ���ơ����ƶ������� 
bool CheckHuPai::CheckQuanDaiYao(GameDataEx *pGameData)
{
	for (int i = 0;i < m_tHuPaiStruct.count;++i)
	{
		if (m_tHuPaiStruct.byType[i] == TYPE_JIANG_PAI || m_tHuPaiStruct.byType[i] == TYPE_JINGDIAO_JIANG || m_tHuPaiStruct.byType[i] == TYPE_AN_KE)
		{
			if (m_tHuPaiStruct.data[i][0] < CMjEnum::MJ_TYPE_FD && m_tHuPaiStruct.data[i][0] % 10 != 1 && m_tHuPaiStruct.data[i][0] % 10 != 9)
				return false;
		}
		else if (m_tHuPaiStruct.byType[i] == TYPE_SHUN_ZI)
		{
			bool jiu = false;
			for (int j = 0;j < 3;j++)
			{
				if (m_tHuPaiStruct.data[i][j] % 10 == 1 || m_tHuPaiStruct.data[i][j] % 10 == 9)
				{
					jiu = true;
				}
			}
			if (!jiu)
			{
				return false;
			}
		}
	}
	for (int i = 0;i < 5;++i)
	{
		if (ACTION_NO == pGameData->m_UserGCPData[m_byStation][i].byType)
			continue;
		if (pGameData->m_UserGCPData[m_byStation][i].byData[0] >= CMjEnum::MJ_TYPE_FD)
			continue;
		bool jiu = false;
		for (int j = 0;j < 3;j++)
		{
			if (pGameData->m_UserGCPData[m_byStation][i].byData[j] % 10 == 1 || pGameData->m_UserGCPData[m_byStation][i].byData[j] % 10 == 9)
			{
				jiu = true;
			}
		}
		if (!jiu)
		{
			return false;
		}
	}
	return true;
}

//�����ˣ�4���Ƽ�����û�г��ơ�����(��������)���������� 
bool CheckHuPai::CheckBuQiuRen(GameDataEx *pGameData)
{
	if (m_byStation != pGameData->m_byNowOutStation)//����
	{
		return false;
	}
	if (m_byPengNum > 0)
	{
		return false;
	}
	for (int i = 0;i < m_byGangNum;++i)
	{
		if (m_byGangData[i][0] != ACTION_AN_GANG)
		{
			return false;
		}
	}
	return true;
}

//˫���ܣ�2������ 
bool CheckHuPai::CheckShuangMingGang()
{
	BYTE num = 0;
	for (int i = 0;i < m_byGangNum;++i)
	{
		if (m_byGangData[i][0] != ACTION_AN_GANG)
		{
			num++;
		}
	}
	if (num == 2)
	{
		return true;
	}
	return false;
}

//�;��ţ����Ƴء�������������3������ʣ�ĵ�4����(���ܺͲ��ƺ;���) 
bool CheckHuPai::CheckHuJueZhang()
{

	return false;
}

//�����ͣ� ��4������(���)��������ɵĺ��� 
bool CheckHuPai::CheckPengPengHu(GameDataEx *pGameData)
{
	if (m_byShunNum == 0 && 0 == pGameData->GetUserChiNum(m_byStation))//˳�Ӹ���Ϊ0(�����г���)
	{
		return true;
	}
	return false;
}

//һɫ�Ƽ�⣨������ƺͳ������ƣ����ﶼ���ã���0����һɫ�ƣ�1��һɫ��2��һɫ
int CheckHuPai::CheckYiSe(GameDataEx *pGameData)
{
	bool zi = false;
	BYTE type = 255, pai = 255;
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (m_byArHandPai[i] == 255)
			continue;
		pai = m_byArHandPai[i];
		if (pGameData->m_StrJing.CheckIsJing(pai))
		{
			continue;
		}
		if (CMjRef::GetHuaSe(pai) == 3)
		{
			zi = true;
			continue;
		}
		if (type == 255)
		{
			type = CMjRef::GetHuaSe(pai);
		}
		if (type != CMjRef::GetHuaSe(pai))
		{
			return 0;
		}
	}
	for (int i = 0;i < 5;++i)
	{
		if (ACTION_NO == pGameData->m_UserGCPData[m_byStation][i].byType)
			continue;
		pai = pGameData->m_UserGCPData[m_byStation][i].byData[0];
		if (CMjRef::GetHuaSe(pai) == 3)
		{
			zi = true;
			continue;
		}
		if (type == 255)
		{
			type = CMjRef::GetHuaSe(pai);
		}
		if (type != CMjRef::GetHuaSe(pai))
		{
			return 0;
		}
	}
	if (type == 255)//ȫ���ֲ�������һɫ
		return 0;

	if (zi)
	{
		return 1;
	}
	return 2;
}



//��ɫ�����ߣ� 3�ֻ�ɫ3�����ε���һλ������˳�ӣ����������ƣ�
bool CheckHuPai::CheckSanSeSanBuGao()
{
	if (m_byShunNum < 3)
	{
		return false;
	}
	bool shun[3][9];
	memset(shun, 0, sizeof(shun));
	CLogicBase::SelectSort(m_byShunData, 4, false);
	for (int i = 0;i < 4;++i)
	{
		if (m_byShunData[i] == 255)
			continue;
		shun[CMjRef::GetHuaSe(m_byShunData[i])][m_byShunData[i] % 10 - 1] = true;
	}
	for (int i = 0;i < 7;++i)
	{
		if (shun[0][i] && shun[1][i + 1] && shun[2][i + 2]
			|| shun[1][i] && shun[2][i + 1] && shun[0][i + 2]
			|| shun[2][i] && shun[1][i + 1] && shun[0][i + 2]
			)
		{
			return true;
		}
	}

	return false;
}

//�����룺����ʱ3�������ơ��硢������ȫ 
bool CheckHuPai::CheckWuMenQi(GameDataEx *pGameData)
{
	bool wbt[3], feng = false, jian = false;
	memset(wbt, 0, sizeof(wbt));
	BYTE pai = 255;

	for (int i = 0;i < m_tHuPaiStruct.count;++i)
	{
		if (m_tHuPaiStruct.data[i][0] == 255)
			continue;
		pai = m_tHuPaiStruct.data[i][0];
		if (CMjRef::GetHuaSe(pai) < 3)
		{
			wbt[CMjRef::GetHuaSe(pai)] = true;
		}
		else
		{
			if (pai >= CMjEnum::MJ_TYPE_ZHONG)
			{
				jian = true;
			}
			else if (pai >= CMjEnum::MJ_TYPE_FD)
			{
				feng = true;
			}
		}
	}
	for (int i = 0;i < 5;++i)
	{
		if (ACTION_NO == pGameData->m_UserGCPData[m_byStation][i].byType)
			continue;
		pai = pGameData->m_UserGCPData[m_byStation][i].byData[0];
		if (CMjRef::GetHuaSe(pai) < 3)
		{
			wbt[CMjRef::GetHuaSe(pai)] = true;
		}
		else
		{
			if (pai >= CMjEnum::MJ_TYPE_ZHONG)
			{
				jian = true;
			}
			else if (pai >= CMjEnum::MJ_TYPE_FD)
			{
				feng = true;
			}
		}
	}

	if (wbt[0] && wbt[1] && wbt[2] && feng && jian)
	{
		return true;
	}
	return false;
}

//ȫ���ˣ�ȫ�����ơ����ơ����������������ƺ��ơ����Ƶ��� 
bool CheckHuPai::CheckQuanQiuRen(GameDataEx *pGameData)
{
	if (m_byStation == pGameData->m_byNowOutStation)//����
	{
		return false;
	}
	if (pGameData->GetHandPaiCount(m_byStation) != 1)
	{
		return false;
	}
	/*for(int i=0;i<m_tHuPaiStruct.count;++i)
	{
	if(m_tHuPaiStruct.byType[i] == TYPE_SHUN_ZI || m_tHuPaiStruct.byType[i] == TYPE_AN_KE)
	{
	return false;
	}
	}
	for(int i=0;i<m_byGangNum;++i)
	{
	if(m_byGangData[i][0] == ACTION_AN_GANG)
	{
	return false;
	}
	}*/
	return true;
}

//˫���ܣ� 2������ 
bool CheckHuPai::CheckShuangAnKe()
{
	BYTE num = 0;
	for (int i = 0;i < m_byGangNum;++i)
	{
		if (m_byGangData[i][0] == ACTION_AN_GANG)
		{
			num++;
		}
	}
	if (num == 2)
	{
		return true;
	}
	return false;
}

//˫���̣�2������(���) 
bool CheckHuPai::CheckShuangJianKe()
{
	BYTE num = 0;
	for (int i = 0;i < m_byGangNum;++i)
	{
		if (m_byGangData[i][1] >= CMjEnum::MJ_TYPE_ZHONG && m_byGangData[i][1] <= CMjEnum::MJ_TYPE_BAI)
		{
			num++;
		}
	}
	for (int i = 0;i < m_byKeziNum;++i)
	{
		if (m_byKeziData[i] >= CMjEnum::MJ_TYPE_ZHONG && m_byKeziData[i] <= CMjEnum::MJ_TYPE_BAI)
		{
			num++;
		}
	}
	return false;
}

//������ 3�ֻ�ɫ��3��˳�����ӳ�1-9�������� ,�����������ƣ�
bool CheckHuPai::CheckHuaLong()
{
	bool shun[3][9];
	memset(shun, 0, sizeof(shun));
	for (int i = 0;i < m_byShunNum;++i)
	{
		if (m_byShunData[i] >= CMjEnum::MJ_TYPE_FD)
			continue;
		shun[CMjRef::GetHuaSe(m_byShunData[i])][m_byShunData[i] % 10 - 1] = true;
	}
	if (shun[0][1] && shun[1][4] && shun[2][7]
		|| shun[1][1] && shun[2][4] && shun[0][7]
		|| shun[2][1] && shun[0][4] && shun[1][7])
	{
		return true;
	}
	return false;
}

//�Ʋ����� ������ͼ��û���������������ɵĺ��ƣ�����1234589����245689�����װ塣����ȱһ�� 
bool CheckHuPai::CheckTuiBuDao(GameDataEx *pGameData)
{
	BYTE pai = 255;
	for (int i = 0;i < m_tHuPaiStruct.count;++i)
	{
		pai = m_tHuPaiStruct.data[i][0];
		if (pai <= CMjEnum::MJ_TYPE_W9 || pai >= CMjEnum::MJ_TYPE_FD && pai != CMjEnum::MJ_TYPE_BAI)
		{
			return false;
		}
		if (m_tHuPaiStruct.byType[i] == TYPE_SHUN_ZI)
		{
			if (pai > CMjEnum::MJ_TYPE_B3 && pai != CMjEnum::MJ_TYPE_T4)
			{
				return false;
			}
		}
		else
		{
			if (pai == CMjEnum::MJ_TYPE_B6 || pai == CMjEnum::MJ_TYPE_B7 || pai == CMjEnum::MJ_TYPE_T1 || pai == CMjEnum::MJ_TYPE_T3 || pai == CMjEnum::MJ_TYPE_T7)
			{
				return false;
			}
		}
	}
	for (int i = 0;i < 5;++i)
	{
		if (ACTION_NO == pGameData->m_UserGCPData[m_byStation][i].byType)
			continue;
		pai = pGameData->m_UserGCPData[m_byStation][i].byData[0];
		if (pai <= CMjEnum::MJ_TYPE_W9 || pai >= CMjEnum::MJ_TYPE_FD && pai != CMjEnum::MJ_TYPE_BAI)
		{
			return false;
		}
		if (pGameData->m_UserGCPData[m_byStation][i].byType == ACTION_CHI)
		{
			if (pai > CMjEnum::MJ_TYPE_B3 && pai != CMjEnum::MJ_TYPE_T4)
			{
				return false;
			}
		}
		else
		{
			if (pai == CMjEnum::MJ_TYPE_B6 || pai == CMjEnum::MJ_TYPE_B7 || pai == CMjEnum::MJ_TYPE_T1 || pai == CMjEnum::MJ_TYPE_T3 || pai == CMjEnum::MJ_TYPE_T7)
			{
				return false;
			}
		}
	}
	return true;
}

//��ɫ��ͬ˳������ʱ����3�ֻ�ɫ3��������ͬ��˳�� �����������ƣ�
bool CheckHuPai::CheckSanSeSanTongShun()
{
	bool shun[3][9];
	memset(shun, 0, sizeof(shun));
	for (int i = 0;i < m_byShunNum;++i)
	{
		if (m_byShunData[i] >= CMjEnum::MJ_TYPE_FD)
			continue;
		shun[CMjRef::GetHuaSe(m_byShunData[i])][m_byShunData[i] % 10 - 1] = true;
	}
	for (int i = 0;i < 9;++i)
	{
		if (shun[0][i] && shun[1][i] && shun[2][i])
		{
			return true;
		}
	}
	return false;
}

//��ɫ���ڸߣ�����ʱ����3�ֻ�ɫ3�����ε���һλ���Ŀ��ӣ����������ƣ�
bool CheckHuPai::CheckSanSeSanJieGao()
{
	bool shun[3][9];
	memset(shun, 0, sizeof(shun));
	for (int i = 0;i < m_byShunNum;++i)
	{
		if (m_byShunData[i] >= CMjEnum::MJ_TYPE_FD)
			continue;
		shun[CMjRef::GetHuaSe(m_byShunData[i])][m_byShunData[i] % 10 - 1] = true;
	}
	for (int i = 0;i < 7;++i)
	{
		if (shun[0][i] && shun[1][i + 1] && shun[2][i + 2]
			|| shun[1][i] && shun[2][i + 1] && shun[0][i + 2]
			|| shun[2][i] && shun[1][i + 1] && shun[0][i + 2])
		{
			return true;
		}
	}
	return false;
}

//�޷��ͣ����ƺ��������κη��ַ�(���Ʋ���������) 
bool CheckHuPai::CheckWuFanHe()
{

	return false;
}

//���ֻش���������ǽ�����һ���ƺ��ơ��������� 
bool CheckHuPai::CheckMiaoShouHuiChun(GameDataEx *pGameData)
{
	if (m_byStation == pGameData->m_byNowOutStation && pGameData->m_MenPai.GetMenPaiNum() == 0)//����
	{
		return true;
	}
	return false;
}

//�������£��ʹ�������һ���� 
bool CheckHuPai::CheckHaiDiLaoYue(GameDataEx *pGameData)
{
	if (m_byStation != pGameData->m_byNowOutStation && pGameData->m_MenPai.GetMenPaiNum() == 0)//����
	{
		return true;
	}
	return false;
}

//���ܺͣ��ͱ�����ץ�����ܵ��ơ����ƺ;���  
bool CheckHuPai::CheckQiangGangHe(GameDataEx *pGameData)
{
	if (m_byStation != pGameData->m_byNowOutStation && ACTION_BU_GANG == pGameData->T_GangPai.byType)
	{
		return true;
	}
	return false;
}

//�����壺��������6-9��˳�ӡ����ӡ�������ɵĺ��ơ��������� 
bool CheckHuPai::CheckDaYuWu(GameDataEx *pGameData)
{
	BYTE pai = 255;
	for (int i = 0;i < m_tHuPaiStruct.count;++i)
	{
		pai = m_tHuPaiStruct.data[i][0];
		if (pai >= CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}
		if (pai % 10 < 6)
		{
			return false;
		}
	}
	for (int i = 0;i < 5;++i)
	{
		if (ACTION_NO == pGameData->m_UserGCPData[m_byStation][i].byType)
			continue;
		pai = pGameData->m_UserGCPData[m_byStation][i].byData[0];
		if (pai >= CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}
		if (pai % 10 < 6)
		{
			return false;
		}
	}
	return true;
}

//С���壺��������1-4��˳�ӡ����ӡ�������ɵĺ��ơ��������� 
bool CheckHuPai::CheckXiaoYuWu(GameDataEx *pGameData)
{
	BYTE pai = 255;
	for (int i = 0;i < m_tHuPaiStruct.count;++i)
	{
		pai = m_tHuPaiStruct.data[i][0];
		if (pai >= CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}
		if (m_tHuPaiStruct.byType[i] == TYPE_SHUN_ZI)
		{
			if (pai % 10 > 2)
			{
				return false;
			}
		}
		else
		{
			if (pai % 10 > 4)
			{
				return false;
			}
		}
	}
	for (int i = 0;i < 5;++i)
	{
		if (ACTION_NO == pGameData->m_UserGCPData[m_byStation][i].byType)
			continue;
		pai = pGameData->m_UserGCPData[m_byStation][i].byData[0];
		if (pai >= CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}
		if (pGameData->m_UserGCPData[m_byStation][i].byType == ACTION_CHI)
		{
			if (pai % 10 > 2)
			{
				return false;
			}
		}
		else
		{
			if (pai % 10 > 4)
			{
				return false;
			}
		}
	}
	return true;
}

//����̣�3����� ��(����������)
bool CheckHuPai::CheckSanFengKe()
{
	BYTE num = 0;
	for (int i = 0;i < m_byKeziNum;++i)
	{
		if (m_byKeziData[i] >= CMjEnum::MJ_TYPE_FD && m_byKeziData[i] <= CMjEnum::MJ_TYPE_FB)
		{
			num++;
		}
	}
	for (int i = 0;i < m_byGangNum;++i)
	{
		if (m_byGangData[i][1] >= CMjEnum::MJ_TYPE_FD && m_byGangData[i][1] <= CMjEnum::MJ_TYPE_FB)
		{
			num++;
		}
	}
	if (num == 3)
	{
		return true;
	}
	return false;
}

//����������ʱ����һ�ֻ�ɫ1-9�����ӵ������� �����������ƣ�
bool CheckHuPai::CheckQingLong()
{
	if (m_byShunNum < 3)
	{
		return false;
	}
	CLogicBase::SelectSort(m_byShunData, 4, false);
	if (m_byShunData[0] == m_byShunData[1] - 3 == m_byShunData[2] - 6 && m_byShunData[0] % 10 == 1)
	{
		return true;
	}
	return false;
}

//��ɫ˫���᣺2�ֻ�ɫ2�����ٸ�����һ�ֻ�ɫ5�����ĺ��ơ�����ϲ��ꡢ���ٸ������֡�ƽ�� �����������ƣ�
bool CheckHuPai::CheckSanSeShuangLongHui()
{
	if (m_byShunNum < 4)
	{
		return false;
	}
	bool shun[3][9];
	memset(shun, 0, sizeof(shun));
	for (int i = 0;i < m_byShunNum;++i)
	{
		if (m_byShunData[i] >= CMjEnum::MJ_TYPE_FD)
			continue;
		shun[CMjRef::GetHuaSe(m_byShunData[i])][m_byShunData[i] % 10 - 1] = true;
	}
	if (shun[0][0] && shun[0][6] && shun[1][0] && shun[1][6] && m_byJiang == CMjEnum::MJ_TYPE_T5
		|| shun[0][0] && shun[0][6] && shun[2][0] && shun[2][6] && m_byJiang == CMjEnum::MJ_TYPE_B5
		|| shun[2][0] && shun[2][6] && shun[1][0] && shun[1][6] && m_byJiang == CMjEnum::MJ_TYPE_W5)
	{
		return true;
	}
	return false;
}

//һɫ�����ߣ� ����ʱ����һ�ֻ�ɫ3�����ε���һλ�����ε�����λ���ֵ�˳�� �����������ƣ�
bool CheckHuPai::CheckYiSeSanBuGao()
{
	if (m_byShunNum < 3)
	{
		return false;
	}
	bool shun[3][9];
	memset(shun, 0, sizeof(shun));
	for (int i = 0;i < m_byShunNum;++i)
	{
		if (m_byShunData[i] >= CMjEnum::MJ_TYPE_FD)
			continue;
		shun[CMjRef::GetHuaSe(m_byShunData[i])][m_byShunData[i] % 10 - 1] = true;
	}
	for (int i = 0;i < 7;++i)
	{
		if (shun[0][i] && shun[0][i + 1] && shun[0][i + 2]
			|| shun[1][i] && shun[1][i + 1] && shun[1][i + 2]
			|| shun[2][i] && shun[2][i + 1] && shun[2][i + 2])
		{
			return true;
		}
	}
	for (int i = 0;i < 5;++i)
	{
		if (shun[0][i] && shun[0][i + 2] && shun[0][i + 4]
			|| shun[1][i] && shun[1][i + 2] && shun[1][i + 4]
			|| shun[2][i] && shun[2][i + 2] && shun[2][i + 4])
		{
			return true;
		}
	}
	return false;
}

//ȫ���壺ÿ���Ƽ����Ʊ�����5�������ơ����ƶ��� 
bool CheckHuPai::CheckQuanDaiWu(GameDataEx *pGameData)
{
	BYTE pai = 255, type = 255;
	bool zi = false;
	for (int i = 0;i < m_tHuPaiStruct.count;++i)
	{
		pai = m_tHuPaiStruct.data[i][0];
		if (CMjRef::GetHuaSe(pai) == 3)
		{
			return false;
		}
		if (m_tHuPaiStruct.byType[i] == TYPE_SHUN_ZI)
		{
			if (pai % 10 > 7 || pai % 10 < 3)
			{
				return false;
			}
		}
		else
		{
			if (pai % 10 != 5)
			{
				return false;
			}
		}
	}
	for (int i = 0;i < 5;++i)
	{
		if (ACTION_NO == pGameData->m_UserGCPData[m_byStation][i].byType)
			continue;
		pai = pGameData->m_UserGCPData[m_byStation][i].byData[0];
		if (CMjRef::GetHuaSe(pai) == 3)
		{
			return false;
		}
		if (pGameData->m_UserGCPData[m_byStation][i].byType == ACTION_CHI)
		{
			if (pai % 10 > 7 || pai % 10 < 3)
			{
				return false;
			}
		}
		else
		{
			if (pai % 10 != 5)
			{
				return false;
			}
		}
	}
	return true;
}

//��ͬ�̣�3��������ͬ�Ŀ���(��) 
bool CheckHuPai::CheckSanTongKe()
{
	BYTE num[9];
	memset(num, 0, sizeof(num));
	for (int i = 0;i < m_byKeziNum;++i)
	{
		if (m_byKeziData[i] >= CMjEnum::MJ_TYPE_FD)
		{
			continue;
		}
		num[m_byKeziData[i] % 10 - 1]++;
	}
	for (int i = 0;i < m_byGangNum;++i)
	{
		if (m_byGangData[i][1] >= CMjEnum::MJ_TYPE_FD)
		{
			continue;
		}
		num[m_byGangData[i][1] % 10 - 1]++;
	}
	for (int i = 0;i < 9;++i)
	{
		if (num[i] == 3)
		{
			return true;
		}
	}
	return false;
}

//�����̣� 3������ 
bool CheckHuPai::CheckSanAnKe()
{
	BYTE num = 0;
	for (int i = 0;i < m_tHuPaiStruct.count;++i)
	{
		if (m_tHuPaiStruct.byType[i] == TYPE_AN_KE)
		{
			num++;
		}
	}
	if (num == 3)
	{
		return true;
	}
	return false;
}

//ȫ˫�̣� ��2��4��6��8�����ƵĿ��ˡ�������ɵĺ��ơ������������ƣ������������͡����� 
bool CheckHuPai::CheckQuanShuangKe()
{
	if (m_byShunNum > 0)
	{
		return false;
	}
	for (int i = 0;i < m_byKeziNum;++i)
	{
		if (m_byKeziData[i] >= CMjEnum::MJ_TYPE_FD || m_byKeziData[i] % 10 != 0)
		{
			return false;
		}
	}
	if (m_byJiang >= CMjEnum::MJ_TYPE_FD || m_byJiang % 10 != 0)
	{
		return false;
	}
	return true;
}

//��һɫ�� ��һ�ֻ�ɫ����������ɺ͸��ơ������� 
bool CheckHuPai::CheckQingYiSe(GameDataEx *pGameData)
{
	return CheckYiSe(pGameData) == 2;
}

//��һɫһ����
bool CheckHuPai::CheckQingYiSeYiTiaoLong(GameDataEx *pGameData)
{
	if (CheckQingYiSe(pGameData) && CheckQingLong())
	{
		return true;
	}
	else
	{
		return false;
	}
}

//��һɫ
bool CheckHuPai::CheckHunYiSe(GameDataEx *pGameData)
{
	return CheckYiSe(pGameData) == 1;
}

//һɫ��ͬ˳������ʱ��һ�ֻ�ɫ3��������ͬ��˳�ˡ�����һɫ���ڸ� �����������ƣ�
bool CheckHuPai::CheckYiSeSanTongShun()
{
	if (m_byShunNum < 3)
	{
		return false;
	}
	BYTE num[3][9];
	memset(num, 0, sizeof(num));
	for (int i = 0;i < m_byShunNum;++i)
	{
		if (m_byShunData[i] >= CMjEnum::MJ_TYPE_FD)
			continue;
		num[CMjRef::GetHuaSe(m_byShunData[i])][m_byShunData[i] % 10 - 1]++;
	}
	for (int i = 0;i < 7;++i)
	{
		if (num[0][i] == 3 || num[1][i] == 3 || num[2][i] == 3)
		{
			return true;
		}
	}
	return false;
}

//һɫ���ڸߣ�����ʱ��һ�ֻ�ɫ3�����ε���һλ���ֵĿ��ӡ�����һɫ��ͬ˳ �����������ƣ�
bool CheckHuPai::CheckYiSeSanJieGao()
{
	if (m_byKeziNum < 3)
	{
		return false;
	}
	bool kezi[3][9];
	memset(kezi, 0, sizeof(kezi));
	for (int i = 0;i < m_byKeziNum;++i)
	{
		if (m_byKeziData[i] >= CMjEnum::MJ_TYPE_FD)
			continue;
		kezi[CMjRef::GetHuaSe(m_byKeziData[i])][m_byKeziData[i] % 10 - 1] = true;
	}
	for (int i = 0;i < m_byGangNum;++i)
	{
		if (m_byGangData[i][1] >= CMjEnum::MJ_TYPE_FD)
			continue;
		kezi[CMjRef::GetHuaSe(m_byGangData[i][1])][m_byGangData[i][1] % 10 - 1] = true;
	}
	for (int i = 0;i < 7;++i)
	{
		if (kezi[0][i] && kezi[0][i + 1] && kezi[0][i + 2]
			|| kezi[1][i] && kezi[1][i + 1] && kezi[1][i + 2]
			|| kezi[2][i] && kezi[2][i + 1] && kezi[2][i + 2])
		{
			return true;
		}
	}
	return false;
}

//ȫ����������789��ɵ�˳�ˡ�����(��)�����Ƶĺ��ơ��������� 
bool CheckHuPai::CheckQuanDa(GameDataEx *pGameData)
{
	BYTE pai = 255;
	for (int i = 0;i < m_tHuPaiStruct.count;++i)
	{
		pai = m_tHuPaiStruct.data[i][0];
		if (pai >= CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}
		if (pai % 10 < 6)
		{
			return false;
		}
	}
	for (int i = 0;i < 5;++i)
	{
		if (ACTION_NO == pGameData->m_UserGCPData[m_byStation][i].byType)
			continue;
		pai = pGameData->m_UserGCPData[m_byStation][i].byData[0];
		if (pai >= CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}
		if (pai % 10 < 6)
		{
			return false;
		}
	}
	return true;
}

//ȫ�У� ��������456��ɵ�˳�ӡ�����(��)�����Ƶĺ��ơ����ƶ��� 
bool CheckHuPai::CheckQuanZhong(GameDataEx *pGameData)
{
	BYTE pai = 255;
	for (int i = 0;i < m_tHuPaiStruct.count;++i)
	{
		pai = m_tHuPaiStruct.data[i][0];
		if (pai >= CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}
		if (m_tHuPaiStruct.byType[i] == TYPE_SHUN_ZI)
		{
			if (pai % 10 != 4)
			{
				return false;
			}
		}
		else
		{
			if (pai % 10 > 6 || pai % 10 < 4)
			{
				return false;
			}
		}
	}
	for (int i = 0;i < 5;++i)
	{
		if (ACTION_NO == pGameData->m_UserGCPData[m_byStation][i].byType)
			continue;
		pai = pGameData->m_UserGCPData[m_byStation][i].byData[0];
		if (pai >= CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}
		if (pGameData->m_UserGCPData[m_byStation][i].byType == ACTION_CHI)
		{
			if (pai % 10 != 4)
			{
				return false;
			}
		}
		else
		{
			if (pai % 10 > 6 || pai % 10 < 4)
			{
				return false;
			}
		}
	}
	return true;
}

//ȫС����������123��ɵ�˳�ӡ�����(��)���Ƶĵĺ��ơ��������� 
bool CheckHuPai::CheckQuanXiao(GameDataEx *pGameData)
{
	BYTE pai = 255;
	for (int i = 0;i < m_tHuPaiStruct.count;++i)
	{
		pai = m_tHuPaiStruct.data[i][0];
		if (pai >= CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}
		if (m_tHuPaiStruct.byType[i] == TYPE_SHUN_ZI)
		{
			if (pai % 10 > 1)
			{
				return false;
			}
		}
		else
		{
			if (pai % 10 > 3)
			{
				return false;
			}
		}
	}
	for (int i = 0;i < 5;++i)
	{
		if (ACTION_NO == pGameData->m_UserGCPData[m_byStation][i].byType)
			continue;
		pai = pGameData->m_UserGCPData[m_byStation][i].byData[0];
		if (pai >= CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}
		if (pGameData->m_UserGCPData[m_byStation][i].byType == ACTION_CHI)
		{
			if (pai % 10 > 1)
			{
				return false;
			}
		}
		else
		{
			if (pai % 10 > 3)
			{
				return false;
			}
		}
	}
	return true;
}

//һɫ�Ĳ��ߣ� һ�ֻ�ɫ4�����ε���һλ�������ε�����λ����˳�� �����������ƣ�
bool CheckHuPai::CheckYiSeSiBuGao()
{
	if (m_byShunNum < 4)
	{
		return false;
	}
	bool shun[3][9];
	memset(shun, 0, sizeof(shun));
	for (int i = 0;i < m_byShunNum;++i)
	{
		if (m_byShunData[i] >= CMjEnum::MJ_TYPE_FD)
			continue;
		shun[CMjRef::GetHuaSe(m_byShunData[i])][m_byShunData[i] % 10 - 1] = true;
	}
	for (int i = 0;i < 6;++i)
	{
		if (shun[0][i] && shun[0][i + 1] && shun[0][i + 2] && shun[0][i + 3]
			|| shun[1][i] && shun[1][i + 1] && shun[1][i + 2] && shun[1][i + 3]
			|| shun[2][i] && shun[0][i + 1] && shun[2][i + 2] && shun[2][i + 3])
		{
			return true;
		}
	}
	for (int i = 0;i < 3;++i)
	{
		if (shun[0][i] && shun[0][i + 2] && shun[0][i + 4] && shun[0][i + 6]
			|| shun[1][i] && shun[1][i + 2] && shun[1][i + 4] && shun[1][i + 6]
			|| shun[2][i] && shun[0][i + 2] && shun[2][i + 4] && shun[2][i + 6])
		{
			return true;
		}
	}
	return false;
}

//���ܣ�3���� 
bool CheckHuPai::CheckSanGang()
{
	if (m_byGangNum == 3)
	{
		return true;
	}
	return false;
}

//���۾ţ������ƺ�������һ���ŵĿ����ý�����ɵĺ��ơ����������� 
bool CheckHuPai::CheckHunYaoJiu(GameDataEx *pGameData)
{
	if (m_byShunNum > 0)
	{
		return false;
	}
	BYTE pai = 255, type = 255;
	bool zi = false;
	for (int i = 0;i < m_tHuPaiStruct.count;++i)
	{
		pai = m_tHuPaiStruct.data[i][0];
		if (CMjRef::GetHuaSe(pai) == 3)
		{
			zi = true;
			continue;
		}
		if (pai % 10 != 1 && pai % 10 != 9)
		{
			return false;
		}
		if (type == 255)
		{
			type = CMjRef::GetHuaSe(pai);
		}
		if (type != CMjRef::GetHuaSe(pai))
		{
			return false;
		}
	}
	for (int i = 0;i < 5;++i)
	{
		if (ACTION_NO == pGameData->m_UserGCPData[m_byStation][i].byType)
			continue;
		pai = pGameData->m_UserGCPData[m_byStation][i].byData[0];
		if (CMjRef::GetHuaSe(pai) == 3)
		{
			zi = true;
			continue;
		}
		if (pai % 10 != 1 && pai % 10 != 9)
		{
			return false;
		}
		if (type == 255)
		{
			type = CMjRef::GetHuaSe(pai);
		}
		if (type != CMjRef::GetHuaSe(pai))
		{
			return false;
		}
	}
	if (zi)
	{
		return true;
	}
	return false;
}

//һɫ��ͬ˳��һ�ֻ�ɫ4��������ͬ��˳�ӣ�����һɫ���ڸߡ�һ��ߡ��Ĺ�һ �����������ƣ�
bool CheckHuPai::CheckYiSeSiTongShun()
{
	if (m_byShunNum < 4)
	{
		return false;
	}
	BYTE num[3][9];
	memset(num, 0, sizeof(num));
	for (int i = 0;i < m_byShunNum;++i)
	{
		if (m_byShunData[i] >= CMjEnum::MJ_TYPE_FD)
			continue;
		num[CMjRef::GetHuaSe(m_byShunData[i])][m_byShunData[i] % 10 - 1]++;
	}
	for (int i = 0;i < 7;++i)
	{
		if (num[0][i] == 4 || num[1][i] == 4 || num[2][i] == 4)
		{
			return true;
		}
	}
	return false;
}

//һɫ�Ľڸߣ�һ�ֻ�ɫ4�����ε���һλ���Ŀ��Ӳ���һɫ��ͬ˳�������� �����������ƣ�
bool CheckHuPai::CheckYiSeSiJieGao()
{
	if ((m_byKeziNum + m_byGangNum) < 4)
	{
		return false;
	}
	bool kezi[3][9];
	memset(kezi, 0, sizeof(kezi));
	for (int i = 0;i < m_byKeziNum;++i)
	{
		if (m_byKeziData[i] >= CMjEnum::MJ_TYPE_FD)
			continue;
		kezi[CMjRef::GetHuaSe(m_byKeziData[i])][m_byKeziData[i] % 10 - 1] = true;
	}
	for (int i = 0;i < m_byGangNum;++i)
	{
		if (m_byGangData[i][1] >= CMjEnum::MJ_TYPE_FD)
			continue;
		kezi[CMjRef::GetHuaSe(m_byGangData[i][1])][m_byGangData[i][1] % 10 - 1] = true;
	}
	for (int i = 0;i < 6;++i)
	{
		if (kezi[0][i] && kezi[0][i + 1] && kezi[0][i + 2] && kezi[0][i + 3]
			|| kezi[1][i] && kezi[1][i + 1] && kezi[1][i + 2] && kezi[1][i + 3]
			|| kezi[2][i] && kezi[2][i + 1] && kezi[2][i + 2] && kezi[2][i + 3])
		{
			return true;
		}
	}
	return false;
}

//���۾ţ���������һ���ſ�����ɵĺ��ơ����������͡�ͬ�̡����� 
bool CheckHuPai::CheckQingYaoJiu(GameDataEx *pGameData)
{
	if (m_byShunNum > 0)
	{
		return false;
	}
	BYTE pai = 255, type = 255;
	for (int i = 0;i < m_tHuPaiStruct.count;++i)
	{
		pai = m_tHuPaiStruct.data[i][0];
		if (pai % 10 != 1 && pai % 10 != 9)
		{
			return false;
		}
		if (type == 255)
		{
			type = CMjRef::GetHuaSe(pai);
		}
		if (type != CMjRef::GetHuaSe(pai))
		{
			return false;
		}
	}
	for (int i = 0;i < 5;++i)
	{
		if (ACTION_NO == pGameData->m_UserGCPData[m_byStation][i].byType)
			continue;
		pai = pGameData->m_UserGCPData[m_byStation][i].byData[0];
		if (pai % 10 != 1 && pai % 10 != 9)
		{
			return false;
		}
		if (type == 255)
		{
			type = CMjRef::GetHuaSe(pai);
		}
		if (type != CMjRef::GetHuaSe(pai))
		{
			return false;
		}
	}
	return true;
}

//С��ϲ������ʱ�з��Ƶ�3�����Ӽ����ơ���������� �������������ƣ�
bool CheckHuPai::CheckXiaoSiXi()
{
	if ((m_byGangNum + m_byKeziNum) < 3)
	{
		return false;
	}
	BYTE num = 0;
	for (int i = 0;i < m_byKeziNum;++i)
	{
		if (m_byKeziData[i] >= CMjEnum::MJ_TYPE_FD && m_byKeziData[i] <= CMjEnum::MJ_TYPE_FB)
		{
			num++;
		}
	}
	for (int i = 0;i < m_byGangNum;++i)
	{
		if (m_byGangData[i][1] >= CMjEnum::MJ_TYPE_FD && m_byGangData[i][1] <= CMjEnum::MJ_TYPE_FB)
		{
			num++;
		}
	}
	if (num == 3 && m_byJiang >= CMjEnum::MJ_TYPE_FD && m_byJiang <= CMjEnum::MJ_TYPE_FB)
	{
		return true;
	}
	return false;
}

//С��Ԫ�� ����ʱ�м��Ƶ��������Ӽ����ơ����Ƽ��� �������������ƣ�
bool CheckHuPai::CheckXiaoSanYuan()
{
	if ((m_byGangNum + m_byKeziNum) < 2)
		return false;
	BYTE num = 0;
	for (int i = 0;i < m_byKeziNum;++i)
	{
		if (m_byKeziData[i] >= CMjEnum::MJ_TYPE_ZHONG && m_byKeziData[i] <= CMjEnum::MJ_TYPE_BAI)
		{
			num++;
		}
	}
	for (int i = 0;i < m_byGangNum;++i)
	{
		if (m_byGangData[i][1] >= CMjEnum::MJ_TYPE_ZHONG && m_byGangData[i][1] <= CMjEnum::MJ_TYPE_BAI)
		{
			num++;
		}
	}
	if (num == 2 && m_byJiang >= CMjEnum::MJ_TYPE_ZHONG && m_byJiang <= CMjEnum::MJ_TYPE_BAI)
	{
		return true;
	}
	return false;
}

//��һɫ�������ƵĿ���(��)������ɵĺ��ơ����������� 
bool CheckHuPai::CheckZiYiSe(GameDataEx *pGameData)
{
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (m_byArHandPai[i] != 255 && m_byArHandPai[i] < CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}
	}
	for (int i = 0;i < 5;++i)
	{
		if (ACTION_NO == pGameData->m_UserGCPData[m_byStation][i].byType)
			continue;
		if (pGameData->m_UserGCPData[m_byStation][i].byData[0] < CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}
	}
	return true;
}

//�İ��̣�4������(����)���������˴�����ơ�������ǰ�塢������ 
bool CheckHuPai::CheckSiAnKe(GameDataEx *pGameData)
{

	BYTE num = 0;
	for (int i = 0;i < m_tHuPaiStruct.count;++i)
	{
		if (m_tHuPaiStruct.byType[i] == TYPE_AN_KE)
		{
			num++;
		}
	}
	for (int i = 0;i < m_byGangNum;++i)
	{
		if (m_byGangData[i][0] == ACTION_AN_GANG)
		{
			num++;
		}
	}
	if (num == 4)
	{
		return true;
	}

	CheckPingHuData(pGameData);

	return false;
}

//һɫ˫���᣺һ�ֻ�ɫ���������ٸ���5Ϊ���ơ�����ƽ�����߶ԡ���һɫ �����������ƣ�
bool CheckHuPai::CheckYiSeShuangLongHui()
{
	if (m_byShunNum < 4)
	{
		return false;
	}
	BYTE num[3][9];
	memset(num, 0, sizeof(num));
	for (int i = 0;i < m_byShunNum;++i)
	{
		if (m_byShunData[i] >= CMjEnum::MJ_TYPE_FD)
			continue;
		num[CMjRef::GetHuaSe(m_byShunData[i])][m_byShunData[i] % 10 - 1]++;
	}
	if (num[0][1] == 2 && num[0][7] == 2 && m_byJiang == CMjEnum::MJ_TYPE_W5
		|| num[1][1] == 2 && num[1][7] == 2 && m_byJiang == CMjEnum::MJ_TYPE_B5
		|| num[2][1] == 2 && num[2][7] == 2 && m_byJiang == CMjEnum::MJ_TYPE_T5)
	{
	}

	return false;
}

//����ϲ����4�����(��)��ɵĺ��ơ�����Ȧ��̡��ŷ�̡�����̡������� �����������ƣ�
bool CheckHuPai::CheckDaSiXi()
{
	if ((m_byGangNum + m_byKeziNum) < 4)
	{
		return false;
	}
	bool zfb[4];
	memset(zfb, 0, sizeof(zfb));
	for (int i = 0;i < m_byKeziNum;++i)
	{
		if (m_byKeziData[i] >= CMjEnum::MJ_TYPE_FD && m_byKeziData[i] <= CMjEnum::MJ_TYPE_FB)
		{
			zfb[m_byKeziData[i] - CMjEnum::MJ_TYPE_FD] = true;
		}
	}
	for (int i = 0;i < m_byGangNum;++i)
	{
		if (m_byGangData[i][1] >= CMjEnum::MJ_TYPE_FD && m_byGangData[i][1] <= CMjEnum::MJ_TYPE_FB)
		{
			zfb[m_byGangData[i][1] - CMjEnum::MJ_TYPE_FD] = true;
		}
	}
	if (zfb[0] && zfb[1] && zfb[2] && zfb[3])
	{
		return true;
	}
	return false;
}

//����Ԫ�������У����з���3�����ӣ����ơ����Ƽ��� �������������ƣ�
bool CheckHuPai::CheckDaSanYuan()
{
	if ((m_byGangNum + m_byKeziNum) < 3)
		return false;
	bool zfb[3];
	memset(zfb, 0, sizeof(zfb));
	for (int i = 0;i < m_byKeziNum;++i)
	{
		if (m_byKeziData[i] >= CMjEnum::MJ_TYPE_ZHONG && m_byKeziData[i] <= CMjEnum::MJ_TYPE_FA)
		{
			zfb[m_byKeziData[i] - CMjEnum::MJ_TYPE_ZHONG] = true;
		}
	}
	if (zfb[0] && zfb[1] && zfb[2])
	{
		return true;
	}
	return false;
}

//��һɫ����23468���������е��κ�����ɵ�˳�ӡ����ӡ����ĺ��ơ����ƻ�һɫ������"��"����ɵĸ��ƣ��ɼ���һɫ 
bool CheckHuPai::CheckLvYiSe(GameDataEx *pGameData)
{
	BYTE pai = 255;
	for (int i = 0;i < HAND_CARD_NUM;i++)//��������
	{
		if (m_byArHandPai[i] == 255)
			continue;
		pai = m_byArHandPai[i];
		if (pai != CMjEnum::MJ_TYPE_T2 && pai != CMjEnum::MJ_TYPE_T3 && pai != CMjEnum::MJ_TYPE_T4 && pai != CMjEnum::MJ_TYPE_T6 && pai != CMjEnum::MJ_TYPE_T8 && pai != CMjEnum::MJ_TYPE_FA)
			return false;
	}
	for (int i = 0;i < 5;++i)
	{
		if (ACTION_NO == pGameData->m_UserGCPData[m_byStation][i].byType)
			continue;
		pai = pGameData->m_UserGCPData[m_byStation][i].byData[0];
		if (pGameData->m_UserGCPData[m_byStation][i].byType == ACTION_CHI)
		{
			if (pai != CMjEnum::MJ_TYPE_T2)
				return false;
		}
		else
		{
			if (pai != CMjEnum::MJ_TYPE_T2 && pai != CMjEnum::MJ_TYPE_T3 && pai != CMjEnum::MJ_TYPE_T4 && pai != CMjEnum::MJ_TYPE_T6 && pai != CMjEnum::MJ_TYPE_T8 && pai != CMjEnum::MJ_TYPE_FA)
				return false;
		}
	}
	return true;
}



//�ĸܣ�ʮ���޺�����4���� 
bool CheckHuPai::CheckSiGang()
{
	if (m_byGangNum == 4)
		return true;
	return false;
}

//�������ƣ���һ�ֻ�ɫ�������Ӱ�1112345678999��ɵ��ض����ͣ���ͬ��ɫ�κ�1�������Ƽ��ɺ��ơ�������һɫ���������������ƣ�
bool CheckHuPai::CheckJiuLianBaoDeng(GameDataEx *pGameData)
{
	if (pGameData->GetGCPCount(m_byStation) > 0)//�����г�����
	{
		return false;
	}
	BYTE type = 255;
	BYTE num[9];
	memset(num, 0, sizeof(num));
	BYTE hand[HAND_CARD_NUM];
	memset(hand, 255, sizeof(hand));

	for (int i = 0;i < HAND_CARD_NUM;++i)
	{
		if (m_byArHandPai[i] == 255)
			continue;
		hand[i] = m_byArHandPai[i];
	}
	for (int i = 0;i < HAND_CARD_NUM;++i)
	{
		if (hand[i] == 255)
			continue;
		if (hand[i] == m_byLastPai)//���һ���Ʋ���
		{
			hand[i] = 255;
			break;
		}
	}
	///��С��������
	CLogicBase::SelectSort(hand, HAND_CARD_NUM, false);

	for (int i = 0;i < HAND_CARD_NUM;++i)
	{
		if (hand[i] == 255)
			continue;
		if (hand[i] >= CMjEnum::MJ_TYPE_FD)//����������
			return false;
		num[hand[i] % 10 - 1]++;
		if (type == 255)
		{
			type = hand[i] / 10;
		}
		if (type != hand[i] / 10)//��������һɫ
		{
			return false;
		}
	}
	for (int i = 0;i < 9;i++)
	{
		if (i == 0 || i == 8)
		{
			if (num[i] < 3)
			{
				return false;
			}
		}
		else
		{
			if (num[i] != 1)
			{
				return false;
			}
		}
	}
	return true;
}

/////////////////////////��������///////////////////////////////////////////////////////////////////////////////////////////////
//ȫ�������ɵ���3�ֻ�ɫ147��258��369���ܴ�λ�������Ƽ����������з����е��κ�14������ɵĺ��ƣ�ʮ���ã������������롢�����ˡ�����(��ƽ������)
bool CheckHuPai::CheckQuanBuKao(GameDataEx *pGameData)
{
	BYTE num = 0;
	for (int i = 0;i < 5;++i)
	{
		if (ACTION_NO == pGameData->m_UserGCPData[m_byStation][i].byType)
			continue;
		num++;
	}
	if (num > 0)//�����г�����
	{
		return false;
	}
	BYTE pai = 255;
	for (int i = 0;i < m_tHuTempData.count;++i)
	{
		if (m_tHuTempData.data[i][1] > 1)
			return false;
		if (m_tHuTempData.data[i][0] >= CMjEnum::MJ_TYPE_FD)
		{
			continue;
		}
		if (pai == 255 || pai / 10 != m_tHuTempData.data[i][0] / 10)
		{
			pai = m_tHuTempData.data[i][0];
		}
		else
		{
			if ((m_tHuTempData.data[i][0] - pai) < 3)
			{
				return false;
			}
		}
	}
	return true;
}

//�������3�ֻ�ɫ��147��258��369���ܴ�λ�������� ��
bool CheckHuPai::CheckZuHeLong(GameDataEx *pGameData)
{
	if (pGameData->GetGCPCount(m_byStation) > 0)//�����г�����
	{
		return false;
	}

	return false;
}

//���߶Կ��Դ����
//�߶ԣ���7��������ɺ��ơ����Ʋ����ˡ�����  (��ƽ������)
bool CheckHuPai::CheckQiDui(GameDataEx *pGameData)
{
	if (pGameData->GetGCPCount(m_byStation) > 0)//�����г�����
	{
		return false;
	}
	int byTempJingNum = m_byJingNum;
	for (int i = 0;i < m_tNoJingHuTempData.count;++i)
	{
		if ((m_tNoJingHuTempData.data[i][1] & 1) == 1)
		{
			byTempJingNum--;
			if (byTempJingNum < 0)
			{
				return false;
			}
		}
	}
	return true;
}

//���߶�
bool CheckHuPai::CheckLongQiDui(GameDataEx *pGameData)
{
	BYTE byPaiNums[50];
	memset(byPaiNums, 0, sizeof(byPaiNums));
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (255 == m_byArHandPai[i])
		{
			continue;
		}
		byPaiNums[m_byArHandPai[i]]++;
	}
	int iLongNums = 0;
	for (int i = 0;i < 50;i++)
	{
		if (4 == byPaiNums[i])
		{
			iLongNums++;
		}
	}
	if (CheckQiDui(pGameData) && iLongNums >= 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//��һɫ�߶�
bool CheckHuPai::CheckQingYiSeQiDui(GameDataEx *pGameData)
{
	if (CheckQiDui(pGameData) && CheckQingYiSe(pGameData))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//���ǲ�����������7�����ŵĶ����ϱ��з��ף�����3�ֻ�ɫ����λ��147��258��369�е�7�����������û�н��Ƶĺ��ơ����������롢�����ˡ����� (��ƽ������)
bool CheckHuPai::CheckQiXingBuKao(GameDataEx *pGameData)
{
	if (pGameData->GetGCPCount(m_byStation) > 0)//�����г�����
	{
		return false;
	}
	BYTE num = 0;
	BYTE pai = 255;
	for (int i = 0;i < m_tHuTempData.count;++i)
	{
		if (m_tHuTempData.data[i][1] > 1)
			return false;
		if (m_tHuTempData.data[i][0] >= CMjEnum::MJ_TYPE_FD)
		{
			num++;
			continue;
		}
		if (pai == 255 || pai / 10 != m_tHuTempData.data[i][0] / 10)
		{
			pai = m_tHuTempData.data[i][0];
		}
		else
		{
			if ((m_tHuTempData.data[i][0] - pai) < 3)
			{
				return false;
			}
		}
	}
	if (num == 7)
	{
		return true;
	}
	return false;
}

//���߶ԣ� ��һ�ֻ�ɫ�������������������7�����ӵĺ��ơ�������һɫ�������ˡ����� �������������ƣ�
bool CheckHuPai::CheckLianQiDui(GameDataEx *pGameData)
{
	if (!CheckQiDui(pGameData))//�������߶�
	{
		return false;
	}
	BYTE duizi[9], num = 0, pai = 255;
	memset(duizi, 255, sizeof(duizi));
	for (int i = 0;i < m_tHuTempData.count;++i)
	{
		if (m_tHuTempData.data[i][1] == 0)
			continue;
		if (m_tHuTempData.data[i][1] != 2)
			return false;
		duizi[num] = m_tHuTempData.data[i][0];
		num++;
	}
	pai = duizi[0];
	if (duizi[1] - 1 == pai && duizi[2] - 2 == pai && duizi[3] - 3 == pai && duizi[4] - 4 == pai && duizi[5] - 5 == pai && duizi[6] - 6 == pai)
	{
		return true;
	}

	return false;
}

//ʮ���ۣ���3�������Ƶ�һ�����ƣ�7�����Ƽ�����һ��������ɵĺ��ơ����������롢�����ˡ����� �������������ƣ�
bool CheckHuPai::CheckShiSanYao(GameDataEx *pGameData)
{
	if (pGameData->GetGCPCount(m_byStation) > 0)//�����г�����
	{
		return false;
	}
	int pai[38], duizi = 0;
	memset(pai, 0, sizeof(pai));
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (m_byArHandPai[i] == 255)
			continue;
		if (m_byArHandPai[i] < CMjEnum::MJ_TYPE_FD && m_byArHandPai[i] % 10 != 1 && m_byArHandPai[i] % 10 != 9)
		{
			return false;
		}
		pai[m_byArHandPai[i]]++;
	}
	for (int i = 0;i < CMjEnum::MJ_TYPE_BAI + 1;++i)
	{
		if (i % 10 == 0 || (i < CMjEnum::MJ_TYPE_FD && i % 10 != 1 && i % 10 != 9))
			continue;
		if (pai[i] <= 0)
		{
			return false;
		}
		if (pai[i] > 1)
		{
			duizi++;
		}
		if (duizi >= 2)
		{
			return false;
		}
	}
	return true;
}

//��Ʈ(Դ�Խ����齫)
bool CheckHuPai::CheckDaPiao(GameDataEx *pGameData)
{
	BYTE byPaiNums[50];
	memset(byPaiNums, 0, sizeof(byPaiNums));
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (255 == m_byArHandPai[i])
		{
			continue;
		}
		byPaiNums[m_byArHandPai[i]]++;
	}
	int iAnKeNums = 0;
	for (int i = 0;i < 50;i++)
	{
		if (3 == byPaiNums[i])
		{
			iAnKeNums++;
		}
	}
	if (CheckQiDui(pGameData) && 4 == iAnKeNums)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//��⽫�ԣ�258��������
bool CheckHuPai::CheckJiangDui(GameDataEx *pGameData)
{
	for (int i = 0;i < m_tHuPaiStruct.count;++i)
	{
		if (m_tHuPaiStruct.byType[i] == TYPE_SHUN_ZI)//˳��
		{
			return false;
		}
		if (m_tHuPaiStruct.data[i][0] % 10 != 2 && m_tHuPaiStruct.data[i][0] % 10 != 5 && m_tHuPaiStruct.data[i][0] % 10 != 8)
		{
			return false;
		}
	}
	for (int i = 0;i < 5;i++)
	{
		if (ACTION_NO == pGameData->m_UserGCPData[m_byStation][i].byType)
			continue;
		BYTE pai = 255;
		pai = pGameData->m_UserGCPData[m_byStation][i].byData[0];
		switch (pGameData->m_UserGCPData[m_byStation][i].byType)
		{
		case ACTION_CHI://���ƶ���
		{
			return false;
		}
		break;
		case ACTION_AN_GANG:
		case ACTION_MING_GANG:
		case ACTION_BU_GANG:
		case ACTION_PENG:
		{
			if (pai % 10 != 2 && pai % 10 != 5 && pai % 10 != 8)
			{
				return false;
			}
		}
		break;
		}
	}
	return true;
}

//���۾�
bool CheckHuPai::CheckYiJiuHu(GameDataEx *pGameData)
{
	//���Ƽ��
	bool bIsHaveYiJiu = false;
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (m_byArHandPai[i] == 255)
			continue;
		BYTE byDian = CMjRef::GetCardPaiDian(m_byArHandPai[i]);
		if (m_byArHandPai[i] < CMjEnum::MJ_TYPE_FD && byDian != 1 && byDian != 9)
		{
			return false;
		}
		if (CMjRef::IsYiJiuPai(m_byArHandPai[i]))
		{
			bIsHaveYiJiu = true;
		}
	}
	//�����ܼ��
	for (int i = 0;i < 5;++i)
	{
		if (ACTION_NO == pGameData->m_UserGCPData[m_byStation][i].byType)
			continue;
		for (int j = 0; j < 4; j++)
		{
			BYTE byPai = pGameData->m_UserGCPData[m_byStation][i].byData[j];
			if (byPai == 255)
			{
				continue;
			}
			BYTE byDian = CMjRef::GetCardPaiDian(byPai);
			if (byPai < CMjEnum::MJ_TYPE_FD && byDian != 1 && byDian != 9)
			{
				return false;
			}
			if (CMjRef::IsYiJiuPai(m_byArHandPai[i]))
			{
				bIsHaveYiJiu = true;
			}
		}
	}
	if (!bIsHaveYiJiu)
	{
		return false;
	}
	return true;
}

//���۾�
bool CheckHuPai::CheckQingYiJiuHu(GameDataEx *pGameData)
{
	//���Ƽ��
	bool bIsHaveYiJiu = false;
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (m_byArHandPai[i] == 255)
			continue;
		BYTE byDian = CMjRef::GetCardPaiDian(m_byArHandPai[i]);
		if (m_byArHandPai[i] >= CMjEnum::MJ_TYPE_FD || (byDian != 1 && byDian != 9))
		{
			return false;
		}
		if (CMjRef::IsYiJiuPai(m_byArHandPai[i]))
		{
			bIsHaveYiJiu = true;
		}
	}
	//�����ܼ��
	for (int i = 0;i < 5;++i)
	{
		if (ACTION_NO == pGameData->m_UserGCPData[m_byStation][i].byType)
			continue;
		for (int j = 0; j < 4; j++)
		{
			BYTE byPai = pGameData->m_UserGCPData[m_byStation][i].byData[j];
			if (byPai == 255)
				continue;
			BYTE byDian = CMjRef::GetCardPaiDian(byPai);
			if (byPai >= CMjEnum::MJ_TYPE_FD || (byDian != 1 && byDian != 9))
			{
				return false;
			}
			if (CMjRef::IsYiJiuPai(m_byArHandPai[i]))
			{
				bIsHaveYiJiu = true;
			}
		}
	}
	if (!bIsHaveYiJiu)
	{
		return false;
	}
	return true;
}

//������е����Ƿ���ȫ��258����һɫ
bool CheckHuPai::QiangQiangHu258(GameDataEx *pGameData)
{
	int iPaiValue = 0;
	for (int i = 0;i < HAND_CARD_NUM;i++)
	{
		if (m_byArHandPai[i] == 255 || m_byArHandPai[i] == 0 || pGameData->m_StrJing.CheckIsJing(m_byArHandPai[i]))
		{
			continue;
		}
		iPaiValue = m_byArHandPai[i] % 10;
		if (!((iPaiValue == 2) || (iPaiValue == 5) || (iPaiValue == 8)))
		{
			return false;
		}
	}
	for (int i = 0;i < 5;i++)
	{
		if (pGameData->m_UserGCPData[m_byStation][i].byType != 255)
		{
			if (ACTION_CHI == pGameData->m_UserGCPData[m_byStation][i].byType)		//��Ϊ2��5��8���������˳��
			{
				return false;
			}
			iPaiValue = pGameData->m_UserGCPData[m_byStation][i].byData[0] % 10;
			if (!((iPaiValue == 2) || (iPaiValue == 5) || (iPaiValue == 8)))
			{
				return false;
			}
		}
	}
	return true;
}

//һ������������ʱ�����в�ͬ��ɫ��123456789˳�ӳ�Ϊһ����
bool CheckHuPai::CheckYiTiaoLong()
{
	if (m_byHandShunNum < 3)
	{
		return false;
	}
	CLogicBase::SelectSort(m_byHandShunData, MAX_CPG_NUM, false);
	bool bHave1 = false, bHave4 = false, bHave7 = false;
	BYTE byHua1[MAX_CPG_NUM], byHua4[MAX_CPG_NUM], byHua7[MAX_CPG_NUM];
	memset(byHua1, 255, sizeof(byHua1));
	memset(byHua4, 255, sizeof(byHua4));
	memset(byHua7, 255, sizeof(byHua7));
	int iHua1Count = 0, iHua4Count = 0, iHua7Count = 0;
	for (int i = 0; i < m_byHandShunNum; i++)
	{
		int iSize = CMjRef::GetCardPaiDianEx(m_byHandShunData[i]);
		int iHua = CMjRef::GetHuaSe(m_byHandShunData[i]);
		if (iHua >= 3)
		{
			return false;
		}
		if (iSize == 1)
		{
			bHave1 = true;
			byHua1[iHua1Count++] = iHua;
		}
		else if (iSize == 4)
		{
			bHave4 = true;
			byHua4[iHua4Count++] = iHua;
		}
		else if (iSize == 7)
		{
			bHave7 = true;
			byHua7[iHua7Count++] = iHua;
		}
	}
	if (bHave1 && bHave4 && bHave7)
	{
		for (int i = 0; i < 3; i++)
		{
			int iCount = 0;
			for (int j = 0; j < iHua1Count; j++)
			{
				if (byHua1[j] == i)
				{
					iCount++;
					break;
				}
			}
			for (int j = 0; j < iHua4Count; j++)
			{
				if (byHua4[j] == i)
				{
					iCount++;
					break;
				}
			}
			for (int j = 0; j < iHua7Count; j++)
			{
				if (byHua7[j] == i)
				{
					iCount++;
					break;
				}
			}
			if (iCount == 3)
			{
				return true;
			}
		}
	}
	return false;
}