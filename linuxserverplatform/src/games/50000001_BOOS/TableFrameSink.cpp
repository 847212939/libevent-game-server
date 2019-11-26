#include "StdAfx.h"
#include "TableFrameSink.h"
#include <mmsystem.h>
#include "Util.h"
#pragma comment(lib,"winmm.lib")

////////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////////// 
//��ʱ��
// ����ʱ��
#define IDI_FISH_CREATE				10									// ����ʱ��
#define IDI_DELAY					11									// �ӳ�ʱ��
#define IDI_SECOND					12									// ���ʱ
#define TIME_FUSH_BOSS				13									//BOSSˢ�¹���
#define TIME_WRITE_SCORE            14                                 //��ʱд�ֶ�ʱ��

#define TIME_DELAY					10000								// �ӳ�ʱ��






// ��Ϣ����
#define	IPC_PACKAGE					4096 

// ��λ����Ϣ
#define FISH_POS_NEXT				0									// ��һ��
#define FISH_POS_DELETE				1									// ɾ����ǰ
#define FISH_POS_NORMAL				2									// ����

// ����ת����
#define AngleToRadian( Angle )     ( (float)(Angle) / 360.f ) * ( GL_PI * 2 )

// ��¼ת��
#define NumberToString( String, nSize, Number )	 tchar String[nSize] = { _T("") }; _sntprintf( String, nSize, TEXT("%I64d"), (LONGLONG)Number );	

////////////////////////////////////////////////////////////////////////////////// 

CWHArray< tagFishPath * >	CTableFrameSink::m_ArrayFishPathPositive;			// ��·������(��)

// ������
LONGLONG					CTableFrameSink::m_nRoomWrite = 0;
double						CTableFrameSink::m_dDartStock = 0.0;
double						CTableFrameSink::m_dWholeTax[Multiple_Max] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };	// ȫ��˰��
double						CTableFrameSink::m_dRoomStock[Multiple_Max] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };	// ������
double *					CTableFrameSink::m_pdTableStock[Multiple_Max] = { NULL, NULL, NULL, NULL, NULL, NULL };
//CMapPersonalDifficulty		CTableFrameSink::m_MapPersonalDifficulty;

double CTableFrameSink::m_dTaxRatio;
// ���伸��
int CTableFrameSink::m_nNullChance;
// ���ͼ���
int CTableFrameSink::m_nGiftChance[5];
// ���ͽ��
int CTableFrameSink::m_nGiftScore[5];
// ���ټ���
int CTableFrameSink::m_nSpeedChance;
// ����ʱ��
int CTableFrameSink::m_nSpeedTime;
// ���⼸��
int CTableFrameSink::m_nLaserChance;
// ����ʱ��
int CTableFrameSink::m_nLaserTime;
// ��������
int CTableFrameSink::m_nSupplyCondition[2];
// �ӵ���ȴ
int CTableFrameSink::m_nBulletCoolingTime;
// �ӵ��ٶ�
int CTableFrameSink::m_nBulletVelocity;
// ����ά��ʱ��
int CTableFrameSink::m_nSceneTime;
// ����������
int CTableFrameSink::m_nCreateCount;
//������(����)
int CTableFrameSink::m_nDifficultyStart[Difficulty_Type];
int CTableFrameSink::m_nRoomDifficultyCount[Difficulty_Max];
int CTableFrameSink::m_nTableDifficultyCount[Difficulty_Max];
int CTableFrameSink::m_nPlayDifficultyCount[Difficulty_Max];
double CTableFrameSink::m_dRoomDifficultyValue[Difficulty_Max];
double CTableFrameSink::m_dTableDifficultyValue[Difficulty_Max];
double CTableFrameSink::m_dPlayDifficultyValue[Difficulty_Max];
tagCustomRule CTableFrameSink::m_CustomRule;
// ��ը����
int CTableFrameSink::m_nExplosionProportion;
// ��ը����
int CTableFrameSink::m_nExplosionStart;
// ��ը����
LONGLONG CTableFrameSink::m_lExplosionCondition;
// ��������
EnumExplosionConditionType CTableFrameSink::m_nExplosionConditionType;
CMap<uint, uint, LONGLONG, LONGLONG> CTableFrameSink::m_MapPlayExplosionCondition;
LONGLONG CTableFrameSink::m_lStockInitial;
LONGLONG CTableFrameSink::m_lStockCurrent;
  




// ��־��¼
char g_szStockFileName[MAX_PATH] = { "" };

//��ʼ����Ϸ
bool CTableFrameSink::InitDeskGameStation()
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	IsBegin = false;
	IniConfig();
	return true;
}

// ���캯��
CTableFrameSink::CTableFrameSink() : CGameDesk(ALL_ARGEE)
{

	// ��Ϸ����
	m_bFirstTime = true;
	m_bFishSwimming = false;
	m_nAppearFishCount = 0;
	ZeroMemory( m_nFishMultiple, sizeof(m_nFishMultiple) );
	
	// ������Ϣ
	m_nCreateCount = 0;

	// ������Ϣ
	m_bCompetitionRoom = false;
	m_bTimedCompetitionRoom = false;
	m_cbBackIndex = 0;
	m_bBackExchange = false;
	m_nSceneTime = 0;
	m_nSceneBegin = 0;

	// ���䱶��
	ZeroMemory(m_nMultipleValue, sizeof(m_nMultipleValue));

	// �ӵ�
	m_nBulletVelocity = 10000;
	m_nBulletCoolingTime = 100;
	
	// �����Ϣ
	ZeroMemory(m_dwPlayID,sizeof(m_dwPlayID));
	ZeroMemory(m_lPlayScore,sizeof(m_lPlayScore));
	ZeroMemory(m_lPlayStartScore,sizeof(m_lPlayStartScore));
	ZeroMemory(m_lplayCutMoney,sizeof(m_lplayCutMoney));
	ZeroMemory(m_lFireCount, sizeof(m_lFireCount));
	ZeroMemory(m_lBulletConsume,sizeof(m_lBulletConsume));
	ZeroMemory(m_nPlayDownTime,sizeof(m_nPlayDownTime));
	ZeroMemory(m_nRobotPlayTime, sizeof(m_nRobotPlayTime));
	ZeroMemory(m_nPlayDelay,sizeof(m_nPlayDelay));
	ZeroMemory(m_lPlayFishCount,sizeof(m_lPlayFishCount));
	ZeroMemory(m_nMultipleIndex,sizeof(m_nMultipleIndex));

	// ��Ҳ���
	ZeroMemory(m_nSupplyCondition,sizeof(m_nSupplyCondition));
	ZeroMemory(m_nSupplyValue,sizeof(m_nSupplyValue));
	ZeroMemory(m_nEnergyValue,sizeof(m_nEnergyValue));
	ZeroMemory(m_bPlaySupply,sizeof(m_bPlaySupply));
	ZeroMemory(m_nLaserPlayTime,sizeof(m_nLaserPlayTime));
	ZeroMemory(m_nSpeedPlayTime,sizeof(m_nSpeedPlayTime));

	// ����Ϣ
	m_nStartTime = 0;

	// ����
	m_nFishTeamTime = 0;
	m_nFishKingTime = 30;
	ZeroMemory(m_nFishKing, sizeof(m_nFishKing));

	// ���������
	m_nLaserTime = 0;
	m_nLaserChance = 0;
	m_nSpeedTime = 0;
	m_nSpeedChance = 0;
	ZeroMemory(m_nGiftScore, sizeof(m_nGiftScore));
	ZeroMemory(m_nGiftChance, sizeof(m_nGiftChance));
	m_nNullChance = 0;

	// ���
	m_dTaxRatio = 0.0;
	ZeroMemory(m_dPlayStock, sizeof(m_dPlayStock));


	return;
}

// ��������
CTableFrameSink::~CTableFrameSink()
{
	// �ͷŽӿ�
//	ReleaseInterface( m_hControlInst, m_pIServerControl );
	for ( int nIndex = 0; nIndex < Multiple_Max; ++nIndex )
		SafeDeleteArray( m_pdTableStock[nIndex] );
	
}

// �ͷŶ���
VOID CTableFrameSink::Release()
{
	for( INT_PTR i = 0; i < m_ArrayFishPathPositive.GetCount(); ++i )
	{
		if( m_ArrayFishPathPositive[i] )
		{
			for( INT_PTR j = 0; j < m_ArrayFishPathPositive[i]->ArrayBezierPoint.GetCount(); ++j )
			{

				if( m_ArrayFishPathPositive[i]->ArrayBezierPoint[j] )
				{
					delete m_ArrayFishPathPositive[i]->ArrayBezierPoint[j];
					m_ArrayFishPathPositive[i]->ArrayBezierPoint[j] = NULL;
				}
			}
			m_ArrayFishPathPositive[i]->ArrayBezierPoint.RemoveAll();
			delete m_ArrayFishPathPositive[i];
			m_ArrayFishPathPositive[i] = NULL;
		}
	}

	m_ArrayFishPathPositive.RemoveAll();

	delete this;
}

void CTableFrameSink::IniConfig()
{
	m_FishKingTime = 0;
	m_bFushBoss = false;
	m_bIsFushBoss = false;
	m_bIsFushBegin = false;
	//���䱶��
	m_nMultipleValue[0] = 1;
	m_nMultipleValue[1] = 10;
	m_nMultipleValue[2] = 100;
	m_nMultipleValue[3] = 500;
	m_nMultipleValue[4] = 1000;
	m_nMultipleValue[5] = 5000;

	//������ ��ʱû��
	for (int nIndex = 0; nIndex < Multiple_Max; ++nIndex)
	{
		m_dRoomStock[nIndex] =(10 *(double)m_nMultipleValue[nIndex]);
		m_lStockInitial += (10* (LONGLONG)m_nMultipleValue[nIndex]);
		m_lStockCurrent += (10* (LONGLONG)m_nMultipleValue[nIndex]);
	}

	//�Ѷ�����
	m_nDifficultyStart[0] = TRUE;   //�����Ѷȿ���
	m_nDifficultyStart[1] = false;  //�����Ѷ�
	m_nDifficultyStart[1] = false;  //�����Ѷ�

	//�����Ѷ�
	m_nRoomDifficultyCount[0] = 50;
	m_nRoomDifficultyCount[1] = 100;
	m_nRoomDifficultyCount[2] = 200;
	m_nRoomDifficultyCount[3] = 400;
	m_nRoomDifficultyCount[4] = 800;
	m_nRoomDifficultyCount[5] = 1200;
	m_nRoomDifficultyCount[6] = 2000;
	m_nRoomDifficultyCount[7] = 3000;
	m_nRoomDifficultyCount[8] = 6000;
	m_nRoomDifficultyCount[9] = 10000;

	//�����Ѷ�
	m_nTableDifficultyCount[0] = 50;
	m_nTableDifficultyCount[1] = 100;
	m_nTableDifficultyCount[2] = 200;
	m_nTableDifficultyCount[3] = 400;
	m_nTableDifficultyCount[4] = 800;
	m_nTableDifficultyCount[5] = 1200;
	m_nTableDifficultyCount[6] = 2000;
	m_nTableDifficultyCount[7] = 3000;
	m_nTableDifficultyCount[8] = 6000;
	m_nTableDifficultyCount[9] = 10000;

	//�����Ѷ�
	m_nPlayDifficultyCount[0] = -10000;
	m_nPlayDifficultyCount[1] = -1000;
	m_nPlayDifficultyCount[2] = -200;
	m_nPlayDifficultyCount[3] = -100;
	m_nPlayDifficultyCount[4] = -50;
	m_nPlayDifficultyCount[5] = 0;
	m_nPlayDifficultyCount[6] = 50;
	m_nPlayDifficultyCount[7] = 200;
	m_nPlayDifficultyCount[8] = 5000;
	m_nPlayDifficultyCount[9] = 10000;

	//�����Ѷ�ϵ��(�õ�����)
	m_dRoomDifficultyValue[0] = 0.05;
	m_dRoomDifficultyValue[1] = 0.15;
	m_dRoomDifficultyValue[2] = 0.25;
	m_dRoomDifficultyValue[3] = 0.50;
	m_dRoomDifficultyValue[4] = 0.75;
	m_dRoomDifficultyValue[5] = 1.00;
	m_dRoomDifficultyValue[6] = 1.25;
	m_dRoomDifficultyValue[7] = 1.70;
	m_dRoomDifficultyValue[8] = 2.50;
	m_dRoomDifficultyValue[9] = 3.50;

	//�����Ѷ�ϵ��
	m_dTableDifficultyValue[0] = 0.05;
	m_dTableDifficultyValue[1] = 0.15;
	m_dTableDifficultyValue[2] = 0.25;
	m_dTableDifficultyValue[3] = 0.50;
	m_dTableDifficultyValue[4] = 0.75;
	m_dTableDifficultyValue[5] = 1.00;
	m_dTableDifficultyValue[6] = 2.50;
	m_dTableDifficultyValue[7] = 2.75;
	m_dTableDifficultyValue[8] = 3.00;
	m_dTableDifficultyValue[9] = 3.50;

	//�����Ѷ�ϵ��
	m_dPlayDifficultyValue[0] = 2.50;
	m_dPlayDifficultyValue[1] = 2.15;
	m_dPlayDifficultyValue[2] = 1.75;
	m_dPlayDifficultyValue[3] = 1.15;
	m_dPlayDifficultyValue[4] = 0.80;
	m_dPlayDifficultyValue[5] = 0.60;
	m_dPlayDifficultyValue[6] = 0.40;
	m_dPlayDifficultyValue[7] = 0.20;
	m_dPlayDifficultyValue[8] = 0.15;
	m_dPlayDifficultyValue[9] = 0.10;

	//��ˮ����
	m_dTaxRatio = 0.3;

	//��������
	m_nCreateCount = 12; 
	m_nSceneTime = 60 * 8;

	// �ӵ���Ϣ
	m_nBulletCoolingTime = 150;
	m_nBulletVelocity = 15000;

	// �����
	m_nSupplyCondition[0] = 500;
	m_nSupplyCondition[1] = 700;
	m_nLaserTime = 30;
	m_nLaserChance = 10;
	m_nSpeedTime = 60;
	m_nSpeedChance = 60;
	m_nGiftScore[0] = 10;
	m_nGiftScore[1] = 30;
	m_nGiftScore[2] = 50;
	m_nGiftScore[3] = 80;
	m_nGiftScore[4] = 120;
	m_nGiftChance[0] = 5;
	m_nGiftChance[1] = 5;
	m_nGiftChance[2] = 5;
	m_nGiftChance[3] = 5;
	m_nGiftChance[4] = 5;
	m_nNullChance = 5;

     //������
	m_nFishMultiple[0][0] = 2;
	m_nFishMultiple[0][1] = 2;
	m_nFishMultiple[1][0] = 3;
	m_nFishMultiple[1][1] = 3;
	m_nFishMultiple[2][0] = 4;
	m_nFishMultiple[2][1] = 4;
	m_nFishMultiple[3][0] = 5;
	m_nFishMultiple[3][1] = 5;
	m_nFishMultiple[4][0] = 6;
	m_nFishMultiple[4][1] = 6;
	m_nFishMultiple[5][0] = 7;
	m_nFishMultiple[5][1] = 7;
	m_nFishMultiple[6][0] = 8;
	m_nFishMultiple[6][1] = 8;
	m_nFishMultiple[7][0] = 9;
	m_nFishMultiple[7][1] = 9;
	m_nFishMultiple[8][0] = 10;
	m_nFishMultiple[8][1] = 10;
	m_nFishMultiple[9][0] = 12;
	m_nFishMultiple[9][1] = 12;
	m_nFishMultiple[10][0] = 15;
	m_nFishMultiple[10][1] = 15;
	m_nFishMultiple[11][0] = 18;
	m_nFishMultiple[11][1] = 18;
	m_nFishMultiple[12][0] = 20;
	m_nFishMultiple[12][1] = 20;
	m_nFishMultiple[13][0] = 25;
	m_nFishMultiple[13][1] = 25;
	m_nFishMultiple[14][0] = 30;
	m_nFishMultiple[14][1] = 30;
	m_nFishMultiple[15][0] = 40;
	m_nFishMultiple[15][1] = 40;
	m_nFishMultiple[16][0] = 150;
	m_nFishMultiple[16][1] = 150;
	m_nFishMultiple[17][0] = 200;
	m_nFishMultiple[17][1] = 200;
	m_nFishMultiple[18][0] = 300;
	m_nFishMultiple[18][1] = 300;
	m_nFishMultiple[19][0] = 400;
	m_nFishMultiple[19][1] = 400;
	m_nFishMultiple[20][0] = 400;
	m_nFishMultiple[20][1] = 1000;
	m_nFishMultiple[21][0] = 20;
	m_nFishMultiple[21][1] = 20;
	m_nFishMultiple[22][0] = 20;
	m_nFishMultiple[22][1] = 20;
	m_nFishMultiple[23][0] = 20;
	m_nFishMultiple[23][1] = 20;
	m_nFishMultiple[24][0] = 0;
	m_nFishMultiple[24][1] = 0;

	// ��ը����
	m_nExplosionProportion = 50;
	m_nExplosionStart = 5000;
	m_lExplosionCondition = 3000;
	m_nExplosionConditionType = ExplosionConditionType_Gun;
	m_nExplosionProportion = max(m_nExplosionProportion, 0);
	m_nExplosionProportion = min(m_nExplosionProportion, 1000);

	// �н�����
	m_nAwardMinMultiple = 50;
	m_bAwardChatBox = true;

	// ������ӿ��
	for (int nIndex = 0; nIndex < Multiple_Max; ++nIndex)
	{
		if (m_pdTableStock[nIndex] == NULL)
		{
			m_pdTableStock[nIndex] = new double[6];
			ZeroMemory(m_pdTableStock[nIndex], sizeof(double) * 6);
		}
	}

	

#ifdef _DEBUG

	// ���Ը���ը
	int nMultipleMax = 0;
	for (int nMultipleIndex = 0; nMultipleIndex < Multiple_Max; ++nMultipleIndex)
	{
		nMultipleMax = max(nMultipleMax, m_nMultipleValue[nMultipleIndex]);
	}

	// ��������
	m_dDartStock = nMultipleMax * 5000;

#endif

	// ��ȡ��ǰ·��
	TCHAR szDirectoryPath[MAX_PATH + 1] = { _T("") };
	GetModuleFileName(NULL, szDirectoryPath, MAX_PATH);
	PathRemoveFileSpec(szDirectoryPath);

	CRedisLoader* pRedis = m_pDataManage->GetRedis();
	RoomBaseInfo roomBasekInfo;
	RoomBaseInfo* pRoomBaseInfo = NULL;
	if (pRedis->GetRoomBaseInfo(m_pDataManage->GetRoomID(), roomBasekInfo))
	{
		pRoomBaseInfo = &roomBasekInfo;
	}
	else
	{
		pRoomBaseInfo = ConfigManage()->GetRoomBaseInfo(m_pDataManage->GetRoomID());
	}
	if (!pRoomBaseInfo)
	{
		return;
	}
	// ��ȡ��·��
	if (m_ArrayFishPathPositive.GetCount() == 0)
	{
		// ������
		tchar szFishMermaidPath[MAX_PATH] = _T("");
		_sntprintf(szFishMermaidPath, MAX_PATH, TEXT("%s\\FishLKConfig\\FishLKPath.pat"), szDirectoryPath);

		// ����·��
		CStdioFile myFile;
		BOOL bOpen = myFile.Open(szFishMermaidPath, CFile::modeNoTruncate | CFile::modeReadWrite);
		if (bOpen == FALSE)
		{
			AfxMessageBox(TEXT("�ļ� FishLKConfig\\FishLKPath.pat ��ȡʧ�ܣ�"));
			return;
		}

		// ��ȡ��Ϣ
		CString strReadPrior;
		int nStrCount = 10;
		int nPathCount = 0;
		BOOL bHand = FALSE;
		BOOL bBeging = TRUE;
		int jD = 0;
		while (myFile.ReadString(strReadPrior))
		{
			// �ж��Ƿ�����
			if (bBeging && strReadPrior.GetLength() == 9)
			{
				// ͷ��Ϣ�Ƚ�
				TCHAR cHand[9] = { 1, 9, 8, 9, 7, 3, 2, 2, 'S' };
				for (; jD < strReadPrior.GetLength() && jD < 9; ++jD)
				{
					if (strReadPrior.GetAt(jD) != cHand[jD])
					{
						break;
					}
				}

				if (jD == 9)
				{
					bHand = TRUE;
					bBeging = FALSE;
					continue;
				}
			}
			bBeging = FALSE;

			// �����Ϣ
			CString strRead;

			// ������
			if (bHand)
			{
				int nPriorCount = strReadPrior.GetLength();
				TCHAR* pData = new TCHAR[nPriorCount + 1];
				for (int jD = 0; jD < nPriorCount; ++jD)
				{
					pData[jD] = strReadPrior.GetAt(jD);
					if (pData[jD] != '\n')
						pData[jD] -= nStrCount % 99;

					nStrCount++;
				}
				pData[nPriorCount] = '\0';
				nStrCount++;
				strRead = pData;
				delete[] pData;
			}
			else
			{
				strRead = strReadPrior;
			}


			// ת�����ֽ�
			CHAR szReadByte[1024];
			strcpy(szReadByte, (LPCTSTR)strRead);

			if (szReadByte[0] == 'B')
			{
				int nPathIndexT = 0;
				int nPathCountT = 0;
				sscanf(szReadByte, "B PathIndex:%d PathCount:%d", &nPathIndexT, &nPathCountT);

				// ����ɽڵ�
				tagFishPath* pNode = new tagFishPath;
				m_ArrayFishPathPositive.Add(pNode);
			}
			else if (szReadByte[0] == 'S' && nPathCount < (int)m_ArrayFishPathPositive.GetCount())
			{
				CPoint BeginPoint;
				CPoint EndPoint;
				CPoint KeyOne;
				CPoint KeyTwo;
				int	   nTime;
				sscanf(szReadByte, "S BeginPoint:%d,%d EndPoint:%d,%d KeyOne:%d,%d KeyTwo:%d,%d Time:%d",
					&BeginPoint.x, &BeginPoint.y, &EndPoint.x, &EndPoint.y,
					&KeyOne.x, &KeyOne.y, &KeyTwo.x, &KeyTwo.y, &nTime);

				// ����ɽڵ�
				tagBezierPoint* pFishPath = new tagBezierPoint;
				pFishPath->BeginPoint = BeginPoint;
				pFishPath->EndPoint = EndPoint;
				pFishPath->KeyOne = KeyOne;
				pFishPath->KeyTwo = KeyTwo;
				pFishPath->Time = nTime;
				m_ArrayFishPathPositive[nPathCount]->ArrayBezierPoint.Add(pFishPath);
			}
			else if (strRead == TEXT("End"))
			{
				// ��������
				nPathCount++;
			}
			else
			{
				ASSERT(FALSE);
			}
		}

		myFile.Flush();
		myFile.Close();
	}




	// ���λ��
	m_PointPlay[S_TOP_LEFT].x = 230;
	m_PointPlay[S_TOP_LEFT].y = 50;
	m_PointPlay[S_TOP_CENTER].x = DEFAULE_WIDTH / 2;
	m_PointPlay[S_TOP_CENTER].y = 50;
	m_PointPlay[S_TOP_RIGHT].x = DEFAULE_WIDTH - 230;
	m_PointPlay[S_TOP_RIGHT].y = 50;
	m_PointPlay[S_BOTTOM_LEFT].x = 230;
	m_PointPlay[S_BOTTOM_LEFT].y = DEFAULE_HEIGHT - 23;
	//m_PointPlay[S_BOTTOM_CENTER].x = DEFAULE_WIDTH / 2;
	//m_PointPlay[S_BOTTOM_CENTER].y = DEFAULE_HEIGHT - 50;
//	m_PointPlay[S_BOTTOM_RIGHT].x = DEFAULE_WIDTH - 230;
//	m_PointPlay[S_BOTTOM_RIGHT].y = DEFAULE_HEIGHT - 50;
	return;
}

// ��������
void CTableFrameSink::LoadDynamicConfig()
{
	CString nameID;
	nameID.Format("%d", NAME_ID);
	CINIFile f(CINIFile::GetAppPath() + nameID + "_s.ini");
	CString key = TEXT("game");
	//��ȡ����
	int Multiple = GetPlatformMultiple();

	//�ӵ����
	m_nBulletCoolingTime = f.GetKeyVal(key, "nBulletCoolingTime", 150);
	//�ӵ��ٶ�
	m_nBulletVelocity = f.GetKeyVal(key, "nBulletVelocity", 15000);
	//����������
	m_nCreateCount = f.GetKeyVal(key, "nCreateCount", 12);
	//����ά��ʱ��
	m_nSceneTime = f.GetKeyVal(key, "nSceneTime", 480);
	//������������������
	m_iAddFinshCount = f.GetKeyVal(key, "AddFinshCount", 6);
	//��Ⱥ����
	m_iFinshLeve = f.GetKeyVal(key, "FinshLeve", 3);
	//�Ƿ����������ᱻ�߳�
	m_IsOpenTick = f.GetKeyVal(key, "IsOpenTick", 1);
	//��Ҷ�ò������ᱻ�߳�����(��)
	m_iTickTime = f.GetKeyVal(key, "TickTime", 120);
	//���ڶ�������������Ա����������
	m_ipropNum = f.GetKeyVal(key, "propNum", 23);
	m_iBingDong = f.GetKeyVal(key, "BingDong", 20);
	m_iLocking = f.GetKeyVal(key, "Locking", 20);
	m_iRate	= f.GetKeyVal(key, "Rate", 5);
	m_iRobotTickTime = f.GetKeyVal(key, "RobotTickTime", 60);

	if (m_iFinshLeve > 3 || m_iFinshLeve < 0)
	{
		m_iFinshLeve = 3;
	}
	//����Ⱥ�Ƿ���
	m_bOpenShoalFish= f.GetKeyVal(key, "OpenFishShoal", 1);
	//����Ⱥ����
	m_iFishShoalLev = f.GetKeyVal(key, "FishShoalLev", 10);
	//С�ڴα���������
	m_nAwardMinMultiple = f.GetKeyVal(key, "AwardMinMultiple", 30);

	//��������
	m_iFinshGroup = f.GetKeyVal(key, "FinshGroup", 255);
	if (m_iFinshGroup < 0 || m_iFinshGroup > 4)
	{
		m_iFinshGroup = 255;
	}
	//С����Ⱥ��������
	m_iCreatJiLv = f.GetKeyVal(key, "CreatJiLv", 10);
	if (m_iCreatJiLv < 0 || m_iCreatJiLv >= 90)
	{
		m_iCreatJiLv = 60;
	}

	m_FishKingIntervalTime = f.GetKeyVal(key, "FishKingIntervalTime", 120);
	if (m_FishKingIntervalTime > 0)
	{
		m_FishKingIntervalTime = m_FishKingIntervalTime * 1000;
	}
	else
	{
		m_FishKingIntervalTime = 120 * 1000;
	}


	m_dRoomDifficultyValue[0] = f.GetKeyVal(key, "dRoomDifficultyValue_1_0", 5);
	m_dRoomDifficultyValue[1] = f.GetKeyVal(key, "dRoomDifficultyValue_1_1", 15);
	m_dRoomDifficultyValue[2] = f.GetKeyVal(key, "dRoomDifficultyValue_1_2", 25);
	m_dRoomDifficultyValue[3] = f.GetKeyVal(key, "dRoomDifficultyValue_1_3", 50);
	m_dRoomDifficultyValue[4] = f.GetKeyVal(key, "dRoomDifficultyValue_1_4", 75);
	m_dRoomDifficultyValue[5] = f.GetKeyVal(key, "dRoomDifficultyValue_1_5", 100);
	m_dRoomDifficultyValue[6] = f.GetKeyVal(key, "dRoomDifficultyValue_1_6", 125);
	m_dRoomDifficultyValue[7] = f.GetKeyVal(key, "dRoomDifficultyValue_1_7", 170);
	m_dRoomDifficultyValue[8] = f.GetKeyVal(key, "dRoomDifficultyValue_1_8", 250);
	m_dRoomDifficultyValue[9] = f.GetKeyVal(key, "dRoomDifficultyValue_1_9", 350);

	//�����Ѷ�ϵ��
	m_dPlayDifficultyValue[9] = f.GetKeyVal(key, "dPlayDifficultyValue_1_9", 250);
	m_dPlayDifficultyValue[8] = f.GetKeyVal(key, "dPlayDifficultyValue_1_8", 215);
	m_dPlayDifficultyValue[7] = f.GetKeyVal(key, "dPlayDifficultyValue_1_7", 175);
	m_dPlayDifficultyValue[6] = f.GetKeyVal(key, "dPlayDifficultyValue_1_6", 115);
	m_dPlayDifficultyValue[5] = f.GetKeyVal(key, "dPlayDifficultyValue_1_5", 80);
	m_dPlayDifficultyValue[4] = f.GetKeyVal(key, "dPlayDifficultyValue_1_4", 60);
	m_dPlayDifficultyValue[3] = f.GetKeyVal(key, "dPlayDifficultyValue_1_3", 40);
	m_dPlayDifficultyValue[2] = f.GetKeyVal(key, "dPlayDifficultyValue_1_2", 20);
	m_dPlayDifficultyValue[1] = f.GetKeyVal(key, "dPlayDifficultyValue_1_1", 15);
	m_dPlayDifficultyValue[0] = f.GetKeyVal(key, "dPlayDifficultyValue_1_0", 10);
	for (int i = 0;i<10;i++)
	{
		if (m_dRoomDifficultyValue[i] > 1000 && m_dRoomDifficultyValue[i] <= 0)
		{
			m_dRoomDifficultyValue[i] = 1;
			continue;
		}
		m_dRoomDifficultyValue[i] = m_dRoomDifficultyValue[i] / 100;
	}
	for (int i = 0; i < 10; i++)
	{
		if (m_dPlayDifficultyValue[i] > 1000 && m_dPlayDifficultyValue[i] <= 0)
		{
			m_dPlayDifficultyValue[i] = 1;
			continue;
		}
		m_dPlayDifficultyValue[i] = m_dPlayDifficultyValue[i] / 100;
	}
	if (GetRoomLevel() == 1)
	{
		m_nMultipleValue[0] = f.GetKeyVal(key, "MultipleValue_1_0", 1) * Multiple;
		m_nMultipleValue[1] = f.GetKeyVal(key, "MultipleValue_1_1", 10) * Multiple;
		m_nMultipleValue[2] = f.GetKeyVal(key, "MultipleValue_1_2", 100) * Multiple;
		m_nMultipleValue[3] = f.GetKeyVal(key, "MultipleValue_1_3", 500) * Multiple;
		m_nMultipleValue[4] = f.GetKeyVal(key, "MultipleValue_1_4", 1000) * Multiple;
		m_nMultipleValue[5] = f.GetKeyVal(key, "MultipleValue_1_5", 5000) * Multiple;
		m_nMultipleValue[6] = f.GetKeyVal(key, "MultipleValue_1_6", 6000) * Multiple;
		m_nMultipleValue[7] = f.GetKeyVal(key, "MultipleValue_1_7", 7000) * Multiple;
		m_nMultipleValue[8] = f.GetKeyVal(key, "MultipleValue_1_8", 8000) * Multiple;
		m_nMultipleValue[9] = f.GetKeyVal(key, "MultipleValue_1_9", 10000) * Multiple;

		m_nFishMultiple[0][0] = f.GetKeyVal(key, "m_nFishMultiple_1_0_0", 2);
		m_nFishMultiple[0][1] = f.GetKeyVal(key, "m_nFishMultiple_1_0_1", 2);
		m_nFishMultiple[1][0] = f.GetKeyVal(key, "m_nFishMultiple_1_1_0", 2);
		m_nFishMultiple[1][1] = f.GetKeyVal(key, "m_nFishMultiple_1_1_1", 2);
		m_nFishMultiple[2][0] = f.GetKeyVal(key, "m_nFishMultiple_1_2_0", 3);
		m_nFishMultiple[2][1] = f.GetKeyVal(key, "m_nFishMultiple_1_2_1", 3);
		m_nFishMultiple[3][0] = f.GetKeyVal(key, "m_nFishMultiple_1_3_0", 3);
		m_nFishMultiple[3][1] = f.GetKeyVal(key, "m_nFishMultiple_1_3_1", 3);
		m_nFishMultiple[4][0] = f.GetKeyVal(key, "m_nFishMultiple_1_4_0", 4);
		m_nFishMultiple[4][1] = f.GetKeyVal(key, "m_nFishMultiple_1_4_1", 4);
		m_nFishMultiple[5][0] = f.GetKeyVal(key, "m_nFishMultiple_1_5_0", 4);
		m_nFishMultiple[5][1] = f.GetKeyVal(key, "m_nFishMultiple_1_5_1", 4);
		m_nFishMultiple[6][0] = f.GetKeyVal(key, "m_nFishMultiple_1_6_0", 5);
		m_nFishMultiple[6][1] = f.GetKeyVal(key, "m_nFishMultiple_1_6_1", 5);
		m_nFishMultiple[7][0] = f.GetKeyVal(key, "m_nFishMultiple_1_7_0", 5);
		m_nFishMultiple[7][1] = f.GetKeyVal(key, "m_nFishMultiple_1_7_0", 5);
		m_nFishMultiple[8][0] = f.GetKeyVal(key, "m_nFishMultiple_1_8_0", 6);
		m_nFishMultiple[8][1] = f.GetKeyVal(key, "m_nFishMultiple_1_8_1", 6);
		m_nFishMultiple[9][0] = f.GetKeyVal(key, "m_nFishMultiple_1_9_0", 6);
		m_nFishMultiple[9][1] = f.GetKeyVal(key, "m_nFishMultiple_1_9_1", 6);
		m_nFishMultiple[10][0] = f.GetKeyVal(key, "m_nFishMultiple_1_10_0", 6);
		m_nFishMultiple[10][1] = f.GetKeyVal(key, "m_nFishMultiple_1_10_1", 6);
		m_nFishMultiple[11][0] = f.GetKeyVal(key, "m_nFishMultiple_1_11_0", 7);
		m_nFishMultiple[11][1] = f.GetKeyVal(key, "m_nFishMultiple_1_11_1", 7);
		m_nFishMultiple[12][0] = f.GetKeyVal(key, "m_nFishMultiple_1_12_0", 8);
		m_nFishMultiple[12][1] = f.GetKeyVal(key, "m_nFishMultiple_1_12_1", 8);
		m_nFishMultiple[13][0] = f.GetKeyVal(key, "m_nFishMultiple_1_13_0", 8);
		m_nFishMultiple[13][1] = f.GetKeyVal(key, "m_nFishMultiple_1_13_1", 8);
		m_nFishMultiple[14][0] = f.GetKeyVal(key, "m_nFishMultiple_1_14_0", 9);
		m_nFishMultiple[14][1] = f.GetKeyVal(key, "m_nFishMultiple_1_14_1", 9);
		m_nFishMultiple[15][0] = f.GetKeyVal(key, "m_nFishMultiple_1_15_0", 10);
		m_nFishMultiple[15][1] = f.GetKeyVal(key, "m_nFishMultiple_1_15_1", 10);
		m_nFishMultiple[16][0] = f.GetKeyVal(key, "m_nFishMultiple_1_16_0", 12);
		m_nFishMultiple[16][1] = f.GetKeyVal(key, "m_nFishMultiple_1_16_1", 12);
		m_nFishMultiple[17][0] = f.GetKeyVal(key, "m_nFishMultiple_1_17_0", 15);
		m_nFishMultiple[17][1] = f.GetKeyVal(key, "m_nFishMultiple_1_17_1", 15);
		m_nFishMultiple[18][0] = f.GetKeyVal(key, "m_nFishMultiple_1_18_0", 20);
		m_nFishMultiple[18][1] = f.GetKeyVal(key, "m_nFishMultiple_1_18_1", 20);
		m_nFishMultiple[19][0] = f.GetKeyVal(key, "m_nFishMultiple_1_19_0", 25);
		m_nFishMultiple[19][1] = f.GetKeyVal(key, "m_nFishMultiple_1_19_1", 25);
		m_nFishMultiple[20][0] = f.GetKeyVal(key, "m_nFishMultiple_1_20_0", 30);
		m_nFishMultiple[20][1] = f.GetKeyVal(key, "m_nFishMultiple_1_20_1", 30);
		m_nFishMultiple[21][0] = f.GetKeyVal(key, "m_nFishMultiple_1_21_0", 30);
		m_nFishMultiple[21][1] = f.GetKeyVal(key, "m_nFishMultiple_1_21_1", 30);
		m_nFishMultiple[22][0] = f.GetKeyVal(key, "m_nFishMultiple_1_22_0", 30);
		m_nFishMultiple[22][1] = f.GetKeyVal(key, "m_nFishMultiple_1_22_1", 30);
		m_nFishMultiple[23][0] = f.GetKeyVal(key, "m_nFishMultiple_1_23_0", 50);
		m_nFishMultiple[23][1] = f.GetKeyVal(key, "m_nFishMultiple_1_23_1", 50);
		m_nFishMultiple[24][0] = f.GetKeyVal(key, "m_nFishMultiple_1_24_0", 50);
		m_nFishMultiple[24][1] = f.GetKeyVal(key, "m_nFishMultiple_1_24_1", 50);
		m_nFishMultiple[25][0] = f.GetKeyVal(key, "m_nFishMultiple_1_25_0", 80);
		m_nFishMultiple[25][1] = f.GetKeyVal(key, "m_nFishMultiple_1_25_1", 100);
		m_nFishMultiple[26][0] = f.GetKeyVal(key, "m_nFishMultiple_1_26_0", 80);
		m_nFishMultiple[26][1] = f.GetKeyVal(key, "m_nFishMultiple_1_26_1", 100);
		m_nFishMultiple[27][0] = f.GetKeyVal(key, "m_nFishMultiple_1_27_0", 120);
		m_nFishMultiple[27][1] = f.GetKeyVal(key, "m_nFishMultiple_1_27_1", 150);
		m_nFishMultiple[28][0] = f.GetKeyVal(key, "m_nFishMultiple_1_28_0", 120);
		m_nFishMultiple[28][1] = f.GetKeyVal(key, "m_nFishMultiple_1_28_1", 150);
		m_nFishMultiple[29][0] = f.GetKeyVal(key, "m_nFishMultiple_1_29_0", 400);
		m_nFishMultiple[29][1] = f.GetKeyVal(key, "m_nFishMultiple_1_29_1", 600);

		m_nFishMultiple[30][0] = f.GetKeyVal(key, "m_nFishMultiple_1_30_0", 400);
		m_nFishMultiple[30][1] = f.GetKeyVal(key, "m_nFishMultiple_1_30_1", 600);
		m_nFishMultiple[31][0] = f.GetKeyVal(key, "m_nFishMultiple_1_31_0", 400);
		m_nFishMultiple[31][1] = f.GetKeyVal(key, "m_nFishMultiple_1_31_1", 600);
		m_nFishMultiple[32][0] = f.GetKeyVal(key, "m_nFishMultiple_1_32_0", 400);
		m_nFishMultiple[32][1] = f.GetKeyVal(key, "m_nFishMultiple_1_32_1", 600);
	}
	else if (GetRoomLevel() == 2)
	{
		m_nMultipleValue[0] = f.GetKeyVal(key, "MultipleValue_2_0", 1) * Multiple;
		m_nMultipleValue[1] = f.GetKeyVal(key, "MultipleValue_2_1", 10) * Multiple;
		m_nMultipleValue[2] = f.GetKeyVal(key, "MultipleValue_2_2", 100) * Multiple;
		m_nMultipleValue[3] = f.GetKeyVal(key, "MultipleValue_2_3", 500) * Multiple;
		m_nMultipleValue[4] = f.GetKeyVal(key, "MultipleValue_2_4", 1000) * Multiple;
		m_nMultipleValue[5] = f.GetKeyVal(key, "MultipleValue_2_5", 5000) * Multiple;
		m_nMultipleValue[6] = f.GetKeyVal(key, "MultipleValue_2_6", 6000) * Multiple;
		m_nMultipleValue[7] = f.GetKeyVal(key, "MultipleValue_2_7", 7000) * Multiple;
		m_nMultipleValue[8] = f.GetKeyVal(key, "MultipleValue_2_8", 8000) * Multiple;
		m_nMultipleValue[9] = f.GetKeyVal(key, "MultipleValue_2_9", 10000) * Multiple;

		m_nFishMultiple[0][0] = f.GetKeyVal(key, "m_nFishMultiple_2_0_0", 2);
		m_nFishMultiple[0][1] = f.GetKeyVal(key, "m_nFishMultiple_2_0_1", 2);
		m_nFishMultiple[1][0] = f.GetKeyVal(key, "m_nFishMultiple_2_1_0", 2);
		m_nFishMultiple[1][1] = f.GetKeyVal(key, "m_nFishMultiple_2_1_1", 2);
		m_nFishMultiple[2][0] = f.GetKeyVal(key, "m_nFishMultiple_2_2_0", 3);
		m_nFishMultiple[2][1] = f.GetKeyVal(key, "m_nFishMultiple_2_2_1", 3);
		m_nFishMultiple[3][0] = f.GetKeyVal(key, "m_nFishMultiple_2_3_0", 3);
		m_nFishMultiple[3][1] = f.GetKeyVal(key, "m_nFishMultiple_2_3_1", 3);
		m_nFishMultiple[4][0] = f.GetKeyVal(key, "m_nFishMultiple_2_4_0", 4);
		m_nFishMultiple[4][1] = f.GetKeyVal(key, "m_nFishMultiple_2_4_1", 4);
		m_nFishMultiple[5][0] = f.GetKeyVal(key, "m_nFishMultiple_2_5_0", 4);
		m_nFishMultiple[5][1] = f.GetKeyVal(key, "m_nFishMultiple_2_5_1", 4);
		m_nFishMultiple[6][0] = f.GetKeyVal(key, "m_nFishMultiple_2_6_0", 5);
		m_nFishMultiple[6][1] = f.GetKeyVal(key, "m_nFishMultiple_2_6_1", 5);
		m_nFishMultiple[7][0] = f.GetKeyVal(key, "m_nFishMultiple_2_7_0", 5);
		m_nFishMultiple[7][1] = f.GetKeyVal(key, "m_nFishMultiple_2_7_0", 5);
		m_nFishMultiple[8][0] = f.GetKeyVal(key, "m_nFishMultiple_2_8_0", 6);
		m_nFishMultiple[8][1] = f.GetKeyVal(key, "m_nFishMultiple_2_8_1", 6);
		m_nFishMultiple[9][0] = f.GetKeyVal(key, "m_nFishMultiple_2_9_0", 6);
		m_nFishMultiple[9][1] = f.GetKeyVal(key, "m_nFishMultiple_2_9_1", 6);
		m_nFishMultiple[10][0] = f.GetKeyVal(key, "m_nFishMultiple_2_10_0", 6);
		m_nFishMultiple[10][1] = f.GetKeyVal(key, "m_nFishMultiple_2_10_1", 6);
		m_nFishMultiple[11][0] = f.GetKeyVal(key, "m_nFishMultiple_2_11_0", 7);
		m_nFishMultiple[11][1] = f.GetKeyVal(key, "m_nFishMultiple_2_11_1", 7);
		m_nFishMultiple[12][0] = f.GetKeyVal(key, "m_nFishMultiple_2_12_0", 8);
		m_nFishMultiple[12][1] = f.GetKeyVal(key, "m_nFishMultiple_2_12_1", 8);
		m_nFishMultiple[13][0] = f.GetKeyVal(key, "m_nFishMultiple_2_13_0", 8);
		m_nFishMultiple[13][1] = f.GetKeyVal(key, "m_nFishMultiple_2_13_1", 8);
		m_nFishMultiple[14][0] = f.GetKeyVal(key, "m_nFishMultiple_2_14_0", 9);
		m_nFishMultiple[14][1] = f.GetKeyVal(key, "m_nFishMultiple_2_14_1", 9);
		m_nFishMultiple[15][0] = f.GetKeyVal(key, "m_nFishMultiple_2_15_0", 10);
		m_nFishMultiple[15][1] = f.GetKeyVal(key, "m_nFishMultiple_2_15_1", 10);
		m_nFishMultiple[16][0] = f.GetKeyVal(key, "m_nFishMultiple_2_16_0", 12);
		m_nFishMultiple[16][1] = f.GetKeyVal(key, "m_nFishMultiple_2_16_1", 12);
		m_nFishMultiple[17][0] = f.GetKeyVal(key, "m_nFishMultiple_2_17_0", 15);
		m_nFishMultiple[17][1] = f.GetKeyVal(key, "m_nFishMultiple_2_17_1", 15);
		m_nFishMultiple[18][0] = f.GetKeyVal(key, "m_nFishMultiple_2_18_0", 20);
		m_nFishMultiple[18][1] = f.GetKeyVal(key, "m_nFishMultiple_2_18_1", 20);
		m_nFishMultiple[19][0] = f.GetKeyVal(key, "m_nFishMultiple_2_19_0", 25);
		m_nFishMultiple[19][1] = f.GetKeyVal(key, "m_nFishMultiple_2_19_1", 25);
		m_nFishMultiple[20][0] = f.GetKeyVal(key, "m_nFishMultiple_2_20_0", 30);
		m_nFishMultiple[20][1] = f.GetKeyVal(key, "m_nFishMultiple_2_20_1", 30);
		m_nFishMultiple[21][0] = f.GetKeyVal(key, "m_nFishMultiple_2_21_0", 30);
		m_nFishMultiple[21][1] = f.GetKeyVal(key, "m_nFishMultiple_2_21_1", 30);
		m_nFishMultiple[22][0] = f.GetKeyVal(key, "m_nFishMultiple_2_22_0", 30);
		m_nFishMultiple[22][1] = f.GetKeyVal(key, "m_nFishMultiple_2_22_1", 30);
		m_nFishMultiple[23][0] = f.GetKeyVal(key, "m_nFishMultiple_2_23_0", 50);
		m_nFishMultiple[23][1] = f.GetKeyVal(key, "m_nFishMultiple_2_23_1", 50);
		m_nFishMultiple[24][0] = f.GetKeyVal(key, "m_nFishMultiple_2_24_0", 50);
		m_nFishMultiple[24][1] = f.GetKeyVal(key, "m_nFishMultiple_2_24_1", 50);
		m_nFishMultiple[25][0] = f.GetKeyVal(key, "m_nFishMultiple_2_25_0", 80);
		m_nFishMultiple[25][1] = f.GetKeyVal(key, "m_nFishMultiple_2_25_1", 100);
		m_nFishMultiple[26][0] = f.GetKeyVal(key, "m_nFishMultiple_2_26_0", 80);
		m_nFishMultiple[26][1] = f.GetKeyVal(key, "m_nFishMultiple_2_26_1", 100);
		m_nFishMultiple[27][0] = f.GetKeyVal(key, "m_nFishMultiple_2_27_0", 120);
		m_nFishMultiple[27][1] = f.GetKeyVal(key, "m_nFishMultiple_2_27_1", 150);
		m_nFishMultiple[28][0] = f.GetKeyVal(key, "m_nFishMultiple_2_28_0", 120);
		m_nFishMultiple[28][1] = f.GetKeyVal(key, "m_nFishMultiple_2_28_1", 150);
		m_nFishMultiple[29][0] = f.GetKeyVal(key, "m_nFishMultiple_2_29_0", 400);
		m_nFishMultiple[29][1] = f.GetKeyVal(key, "m_nFishMultiple_2_29_1", 600);

		m_nFishMultiple[30][0] = f.GetKeyVal(key, "m_nFishMultiple_2_30_0", 400);
		m_nFishMultiple[30][1] = f.GetKeyVal(key, "m_nFishMultiple_2_30_1", 600);
		m_nFishMultiple[31][0] = f.GetKeyVal(key, "m_nFishMultiple_2_31_0", 400);
		m_nFishMultiple[31][1] = f.GetKeyVal(key, "m_nFishMultiple_2_31_1", 600);
		m_nFishMultiple[32][0] = f.GetKeyVal(key, "m_nFishMultiple_2_32_0", 400);
		m_nFishMultiple[32][1] = f.GetKeyVal(key, "m_nFishMultiple_2_32_1", 600);
	}
	else if (GetRoomLevel() == 3)
	{
		m_nMultipleValue[0] = f.GetKeyVal(key, "MultipleValue_3_0", 1) * Multiple;
		m_nMultipleValue[1] = f.GetKeyVal(key, "MultipleValue_3_1", 10) * Multiple;
		m_nMultipleValue[2] = f.GetKeyVal(key, "MultipleValue_3_2", 100) * Multiple;
		m_nMultipleValue[3] = f.GetKeyVal(key, "MultipleValue_3_3", 500) * Multiple;
		m_nMultipleValue[4] = f.GetKeyVal(key, "MultipleValue_3_4", 1000) * Multiple;
		m_nMultipleValue[5] = f.GetKeyVal(key, "MultipleValue_3_5", 5000) * Multiple;
		m_nMultipleValue[6] = f.GetKeyVal(key, "MultipleValue_3_6", 6000) * Multiple;
		m_nMultipleValue[7] = f.GetKeyVal(key, "MultipleValue_3_7", 7000) * Multiple;
		m_nMultipleValue[8] = f.GetKeyVal(key, "MultipleValue_3_8", 8000) * Multiple;
		m_nMultipleValue[9] = f.GetKeyVal(key, "MultipleValue_3_9", 10000) * Multiple;

		m_nFishMultiple[0][0] = f.GetKeyVal(key, "m_nFishMultiple_3_0_0", 2);
		m_nFishMultiple[0][1] = f.GetKeyVal(key, "m_nFishMultiple_3_0_1", 2);
		m_nFishMultiple[1][0] = f.GetKeyVal(key, "m_nFishMultiple_3_1_0", 2);
		m_nFishMultiple[1][1] = f.GetKeyVal(key, "m_nFishMultiple_3_1_1", 2);
		m_nFishMultiple[2][0] = f.GetKeyVal(key, "m_nFishMultiple_3_2_0", 3);
		m_nFishMultiple[2][1] = f.GetKeyVal(key, "m_nFishMultiple_3_2_1", 3);
		m_nFishMultiple[3][0] = f.GetKeyVal(key, "m_nFishMultiple_3_3_0", 3);
		m_nFishMultiple[3][1] = f.GetKeyVal(key, "m_nFishMultiple_3_3_1", 3);
		m_nFishMultiple[4][0] = f.GetKeyVal(key, "m_nFishMultiple_3_4_0", 4);
		m_nFishMultiple[4][1] = f.GetKeyVal(key, "m_nFishMultiple_3_4_1", 4);
		m_nFishMultiple[5][0] = f.GetKeyVal(key, "m_nFishMultiple_3_5_0", 4);
		m_nFishMultiple[5][1] = f.GetKeyVal(key, "m_nFishMultiple_3_5_1", 4);
		m_nFishMultiple[6][0] = f.GetKeyVal(key, "m_nFishMultiple_3_6_0", 5);
		m_nFishMultiple[6][1] = f.GetKeyVal(key, "m_nFishMultiple_3_6_1", 5);
		m_nFishMultiple[7][0] = f.GetKeyVal(key, "m_nFishMultiple_3_7_0", 5);
		m_nFishMultiple[7][1] = f.GetKeyVal(key, "m_nFishMultiple_3_7_0", 5);
		m_nFishMultiple[8][0] = f.GetKeyVal(key, "m_nFishMultiple_3_8_0", 6);
		m_nFishMultiple[8][1] = f.GetKeyVal(key, "m_nFishMultiple_3_8_1", 6);
		m_nFishMultiple[9][0] = f.GetKeyVal(key, "m_nFishMultiple_3_9_0", 6);
		m_nFishMultiple[9][1] = f.GetKeyVal(key, "m_nFishMultiple_3_9_1", 6);
		m_nFishMultiple[10][0] = f.GetKeyVal(key, "m_nFishMultiple_3_10_0", 6);
		m_nFishMultiple[10][1] = f.GetKeyVal(key, "m_nFishMultiple_3_10_1", 6);
		m_nFishMultiple[11][0] = f.GetKeyVal(key, "m_nFishMultiple_3_11_0", 7);
		m_nFishMultiple[11][1] = f.GetKeyVal(key, "m_nFishMultiple_3_11_1", 7);
		m_nFishMultiple[12][0] = f.GetKeyVal(key, "m_nFishMultiple_3_12_0", 8);
		m_nFishMultiple[12][1] = f.GetKeyVal(key, "m_nFishMultiple_3_12_1", 8);
		m_nFishMultiple[13][0] = f.GetKeyVal(key, "m_nFishMultiple_3_13_0", 8);
		m_nFishMultiple[13][1] = f.GetKeyVal(key, "m_nFishMultiple_3_13_1", 8);
		m_nFishMultiple[14][0] = f.GetKeyVal(key, "m_nFishMultiple_3_14_0", 9);
		m_nFishMultiple[14][1] = f.GetKeyVal(key, "m_nFishMultiple_3_14_1", 9);
		m_nFishMultiple[15][0] = f.GetKeyVal(key, "m_nFishMultiple_3_15_0", 10);
		m_nFishMultiple[15][1] = f.GetKeyVal(key, "m_nFishMultiple_3_15_1", 10);
		m_nFishMultiple[16][0] = f.GetKeyVal(key, "m_nFishMultiple_3_16_0", 12);
		m_nFishMultiple[16][1] = f.GetKeyVal(key, "m_nFishMultiple_3_16_1", 12);
		m_nFishMultiple[17][0] = f.GetKeyVal(key, "m_nFishMultiple_3_17_0", 15);
		m_nFishMultiple[17][1] = f.GetKeyVal(key, "m_nFishMultiple_3_17_1", 15);
		m_nFishMultiple[18][0] = f.GetKeyVal(key, "m_nFishMultiple_3_18_0", 20);
		m_nFishMultiple[18][1] = f.GetKeyVal(key, "m_nFishMultiple_3_18_1", 20);
		m_nFishMultiple[19][0] = f.GetKeyVal(key, "m_nFishMultiple_3_19_0", 25);
		m_nFishMultiple[19][1] = f.GetKeyVal(key, "m_nFishMultiple_3_19_1", 25);
		m_nFishMultiple[20][0] = f.GetKeyVal(key, "m_nFishMultiple_3_20_0", 30);
		m_nFishMultiple[20][1] = f.GetKeyVal(key, "m_nFishMultiple_3_20_1", 30);
		m_nFishMultiple[21][0] = f.GetKeyVal(key, "m_nFishMultiple_3_21_0", 30);
		m_nFishMultiple[21][1] = f.GetKeyVal(key, "m_nFishMultiple_3_21_1", 30);
		m_nFishMultiple[22][0] = f.GetKeyVal(key, "m_nFishMultiple_3_22_0", 30);
		m_nFishMultiple[22][1] = f.GetKeyVal(key, "m_nFishMultiple_3_22_1", 30);
		m_nFishMultiple[23][0] = f.GetKeyVal(key, "m_nFishMultiple_3_23_0", 50);
		m_nFishMultiple[23][1] = f.GetKeyVal(key, "m_nFishMultiple_3_23_1", 50);
		m_nFishMultiple[24][0] = f.GetKeyVal(key, "m_nFishMultiple_3_24_0", 50);
		m_nFishMultiple[24][1] = f.GetKeyVal(key, "m_nFishMultiple_3_24_1", 50);
		m_nFishMultiple[25][0] = f.GetKeyVal(key, "m_nFishMultiple_3_25_0", 80);
		m_nFishMultiple[25][1] = f.GetKeyVal(key, "m_nFishMultiple_3_25_1", 100);
		m_nFishMultiple[26][0] = f.GetKeyVal(key, "m_nFishMultiple_3_26_0", 80);
		m_nFishMultiple[26][1] = f.GetKeyVal(key, "m_nFishMultiple_3_26_1", 100);
		m_nFishMultiple[27][0] = f.GetKeyVal(key, "m_nFishMultiple_3_27_0", 120);
		m_nFishMultiple[27][1] = f.GetKeyVal(key, "m_nFishMultiple_3_27_1", 150);
		m_nFishMultiple[28][0] = f.GetKeyVal(key, "m_nFishMultiple_3_28_0", 120);
		m_nFishMultiple[28][1] = f.GetKeyVal(key, "m_nFishMultiple_3_28_1", 150);
		m_nFishMultiple[29][0] = f.GetKeyVal(key, "m_nFishMultiple_3_29_0", 400);
		m_nFishMultiple[29][1] = f.GetKeyVal(key, "m_nFishMultiple_3_29_1", 600);

		m_nFishMultiple[30][0] = f.GetKeyVal(key, "m_nFishMultiple_3_30_0", 400);
		m_nFishMultiple[30][1] = f.GetKeyVal(key, "m_nFishMultiple_3_30_1", 600);
		m_nFishMultiple[31][0] = f.GetKeyVal(key, "m_nFishMultiple_3_31_0", 400);
		m_nFishMultiple[31][1] = f.GetKeyVal(key, "m_nFishMultiple_3_31_1", 600);
		m_nFishMultiple[32][0] = f.GetKeyVal(key, "m_nFishMultiple_3_32_0", 400);
		m_nFishMultiple[32][1] = f.GetKeyVal(key, "m_nFishMultiple_3_32_1", 600);
	}
	else
	{
		m_nMultipleValue[0] = f.GetKeyVal(key, "MultipleValue_1_0", 1);
		m_nMultipleValue[1] = f.GetKeyVal(key, "MultipleValue_1_1", 10);
		m_nMultipleValue[2] = f.GetKeyVal(key, "MultipleValue_1_2", 100);
		m_nMultipleValue[3] = f.GetKeyVal(key, "MultipleValue_1_3", 500);
		m_nMultipleValue[4] = f.GetKeyVal(key, "MultipleValue_1_4", 1000);
		m_nMultipleValue[5] = f.GetKeyVal(key, "MultipleValue_1_5", 5000);
		m_nMultipleValue[6] = f.GetKeyVal(key, "MultipleValue_1_6", 6000);
		m_nMultipleValue[7] = f.GetKeyVal(key, "MultipleValue_1_7", 7000);
		m_nMultipleValue[8] = f.GetKeyVal(key, "MultipleValue_1_8", 8000);
		m_nMultipleValue[9] = f.GetKeyVal(key, "MultipleValue_1_9", 10000);

		m_nFishMultiple[0][0] = f.GetKeyVal(key, "m_nFishMultiple_3_0_0", 2);
		m_nFishMultiple[0][1] = f.GetKeyVal(key, "m_nFishMultiple_3_0_1", 2);
		m_nFishMultiple[1][0] = f.GetKeyVal(key, "m_nFishMultiple_3_1_0", 2);
		m_nFishMultiple[1][1] = f.GetKeyVal(key, "m_nFishMultiple_3_1_1", 2);
		m_nFishMultiple[2][0] = f.GetKeyVal(key, "m_nFishMultiple_3_2_0", 3);
		m_nFishMultiple[2][1] = f.GetKeyVal(key, "m_nFishMultiple_3_2_1", 3);
		m_nFishMultiple[3][0] = f.GetKeyVal(key, "m_nFishMultiple_3_3_0", 3);
		m_nFishMultiple[3][1] = f.GetKeyVal(key, "m_nFishMultiple_3_3_1", 3);
		m_nFishMultiple[4][0] = f.GetKeyVal(key, "m_nFishMultiple_3_4_0", 4);
		m_nFishMultiple[4][1] = f.GetKeyVal(key, "m_nFishMultiple_3_4_1", 4);
		m_nFishMultiple[5][0] = f.GetKeyVal(key, "m_nFishMultiple_3_5_0", 4);
		m_nFishMultiple[5][1] = f.GetKeyVal(key, "m_nFishMultiple_3_5_1", 4);
		m_nFishMultiple[6][0] = f.GetKeyVal(key, "m_nFishMultiple_3_6_0", 5);
		m_nFishMultiple[6][1] = f.GetKeyVal(key, "m_nFishMultiple_3_6_1", 5);
		m_nFishMultiple[7][0] = f.GetKeyVal(key, "m_nFishMultiple_3_7_0", 5);
		m_nFishMultiple[7][1] = f.GetKeyVal(key, "m_nFishMultiple_3_7_0", 5);
		m_nFishMultiple[8][0] = f.GetKeyVal(key, "m_nFishMultiple_3_8_0", 6);
		m_nFishMultiple[8][1] = f.GetKeyVal(key, "m_nFishMultiple_3_8_1", 6);
		m_nFishMultiple[9][0] = f.GetKeyVal(key, "m_nFishMultiple_3_9_0", 6);
		m_nFishMultiple[9][1] = f.GetKeyVal(key, "m_nFishMultiple_3_9_1", 6);
		m_nFishMultiple[10][0] = f.GetKeyVal(key, "m_nFishMultiple_3_10_0", 6);
		m_nFishMultiple[10][1] = f.GetKeyVal(key, "m_nFishMultiple_3_10_1", 6);
		m_nFishMultiple[11][0] = f.GetKeyVal(key, "m_nFishMultiple_3_11_0", 7);
		m_nFishMultiple[11][1] = f.GetKeyVal(key, "m_nFishMultiple_3_11_1", 7);
		m_nFishMultiple[12][0] = f.GetKeyVal(key, "m_nFishMultiple_3_12_0", 8);
		m_nFishMultiple[12][1] = f.GetKeyVal(key, "m_nFishMultiple_3_12_1", 8);
		m_nFishMultiple[13][0] = f.GetKeyVal(key, "m_nFishMultiple_3_13_0", 8);
		m_nFishMultiple[13][1] = f.GetKeyVal(key, "m_nFishMultiple_3_13_1", 8);
		m_nFishMultiple[14][0] = f.GetKeyVal(key, "m_nFishMultiple_3_14_0", 9);
		m_nFishMultiple[14][1] = f.GetKeyVal(key, "m_nFishMultiple_3_14_1", 9);
		m_nFishMultiple[15][0] = f.GetKeyVal(key, "m_nFishMultiple_3_15_0", 10);
		m_nFishMultiple[15][1] = f.GetKeyVal(key, "m_nFishMultiple_3_15_1", 10);
		m_nFishMultiple[16][0] = f.GetKeyVal(key, "m_nFishMultiple_3_16_0", 12);
		m_nFishMultiple[16][1] = f.GetKeyVal(key, "m_nFishMultiple_3_16_1", 12);
		m_nFishMultiple[17][0] = f.GetKeyVal(key, "m_nFishMultiple_3_17_0", 15);
		m_nFishMultiple[17][1] = f.GetKeyVal(key, "m_nFishMultiple_3_17_1", 15);
		m_nFishMultiple[18][0] = f.GetKeyVal(key, "m_nFishMultiple_3_18_0", 20);
		m_nFishMultiple[18][1] = f.GetKeyVal(key, "m_nFishMultiple_3_18_1", 20);
		m_nFishMultiple[19][0] = f.GetKeyVal(key, "m_nFishMultiple_3_19_0", 25);
		m_nFishMultiple[19][1] = f.GetKeyVal(key, "m_nFishMultiple_3_19_1", 25);
		m_nFishMultiple[20][0] = f.GetKeyVal(key, "m_nFishMultiple_3_20_0", 30);
		m_nFishMultiple[20][1] = f.GetKeyVal(key, "m_nFishMultiple_3_20_1", 30);
		m_nFishMultiple[21][0] = f.GetKeyVal(key, "m_nFishMultiple_3_21_0", 30);
		m_nFishMultiple[21][1] = f.GetKeyVal(key, "m_nFishMultiple_3_21_1", 30);
		m_nFishMultiple[22][0] = f.GetKeyVal(key, "m_nFishMultiple_3_22_0", 30);
		m_nFishMultiple[22][1] = f.GetKeyVal(key, "m_nFishMultiple_3_22_1", 30);
		m_nFishMultiple[23][0] = f.GetKeyVal(key, "m_nFishMultiple_3_23_0", 50);
		m_nFishMultiple[23][1] = f.GetKeyVal(key, "m_nFishMultiple_3_23_1", 50);
		m_nFishMultiple[24][0] = f.GetKeyVal(key, "m_nFishMultiple_3_24_0", 50);
		m_nFishMultiple[24][1] = f.GetKeyVal(key, "m_nFishMultiple_3_24_1", 50);
		m_nFishMultiple[25][0] = f.GetKeyVal(key, "m_nFishMultiple_3_25_0", 80);
		m_nFishMultiple[25][1] = f.GetKeyVal(key, "m_nFishMultiple_3_25_1", 100);
		m_nFishMultiple[26][0] = f.GetKeyVal(key, "m_nFishMultiple_3_26_0", 80);
		m_nFishMultiple[26][1] = f.GetKeyVal(key, "m_nFishMultiple_3_26_1", 100);
		m_nFishMultiple[27][0] = f.GetKeyVal(key, "m_nFishMultiple_3_27_0", 120);
		m_nFishMultiple[27][1] = f.GetKeyVal(key, "m_nFishMultiple_3_27_1", 150);
		m_nFishMultiple[28][0] = f.GetKeyVal(key, "m_nFishMultiple_3_28_0", 120);
		m_nFishMultiple[28][1] = f.GetKeyVal(key, "m_nFishMultiple_3_28_1", 150);
		m_nFishMultiple[29][0] = f.GetKeyVal(key, "m_nFishMultiple_3_29_0", 400);
		m_nFishMultiple[29][1] = f.GetKeyVal(key, "m_nFishMultiple_3_29_1", 600);

		m_nFishMultiple[30][0] = f.GetKeyVal(key, "m_nFishMultiple_3_30_0", 400);
		m_nFishMultiple[30][1] = f.GetKeyVal(key, "m_nFishMultiple_3_30_1", 600);
		m_nFishMultiple[31][0] = f.GetKeyVal(key, "m_nFishMultiple_3_31_0", 400);
		m_nFishMultiple[31][1] = f.GetKeyVal(key, "m_nFishMultiple_3_31_1", 600);
		m_nFishMultiple[32][0] = f.GetKeyVal(key, "m_nFishMultiple_3_32_0", 400);
		m_nFishMultiple[32][1] = f.GetKeyVal(key, "m_nFishMultiple_3_32_1", 600);
	}
	
	return;
}

// ��λ����
VOID CTableFrameSink::RepositionSink()
{
	return;
}


// ��Ϸ����
bool CTableFrameSink::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	// ��ɢд��
	if (bCloseFlag == GF_NORMAL)
	{
		//����״̬
		SetGameStation(GAME_STATUS_FREE);

		// ������
		for ( int nSite = 0; nSite < PlayChair_Max; ++nSite )
		{
			// �ж��û�
			long userID = GetUserIDByDeskStation(nSite);
			if( userID > 0 )
			{
				// ���ָ��
				GameUserInfo UserInfo;
				GetUserData(nSite,UserInfo);

				// д��
				LONGLONG lScore = ( m_lPlayScore[nSite] - m_lPlayStartScore[nSite] );
				if ( lScore != 0 )
				{
					// ����Ϊ���� ���ܳ���������
					if ( lScore < 0 && (-lScore) > UserInfo.money)
					{
						lScore = -UserInfo.money;
					}
				}

				// ��Ϸʱ��
				DWORD dwEndTime = (DWORD)time(NULL) - m_nPlayDownTime[nSite];

				ChangeUserPoint(nSite, lScore, m_lplayCutMoney[nSite]);

				m_lplayCutMoney[nSite] = 0;

				// ����д��
				m_nRoomWrite += (-lScore);

				// ����
				InitializePlayer(nSite);
			}
		}

		// ȫ�ֱ�ը��ԭ
		if ( GetFishCount(FishType_BaoZhaFeiBiao) > 0 )
		{
			// �����
			int nMultipleMax = 0;
			for ( int nMultipleIndex = 0; nMultipleIndex < Multiple_Max; ++nMultipleIndex )
			{
				nMultipleMax = max( nMultipleMax, m_nMultipleValue[nMultipleIndex] );
			}

			// ��������
			m_dDartStock += nMultipleMax * m_nExplosionStart;
		}

		// ��¼���
		//FishMermaidStock();

		// ɾ����ʱ��
		KillTimer(IDI_FISH_CREATE);
		KillTimer(IDI_DELAY);
		KillTimer(IDI_SECOND);
		KillTimer(TIME_WRITE_SCORE);
		//�����ҵ����и��˿���
		//ɾ��ȫ�������Ѷ�
		ClearPersonalDifficulty();

		// ����Ϣ
		m_ArrayFishInfo.RemoveAll();
		m_cbBackIndex = 0;
		m_nAppearFishCount = 0;
		m_bFishSwimming = FALSE;
		m_bBackExchange = FALSE;
		m_nStartTime = 0;

		// ����
		m_nFishTeamTime = 0;
		m_nFishKingTime = 30;
		ZeroMemory(m_nFishKing, sizeof(m_nFishKing));

		IsBegin = false;
		return __super::GameFinish(bDeskStation, bCloseFlag);
	}
	// ������
	for (int nSite = 0; nSite < PlayChair_Max; ++nSite)
	{
		// ����
		InitializePlayer(nSite);
	}

	IsBegin = false;
	return __super::GameFinish(bDeskStation, bCloseFlag);
}

 //���ͳ���
bool CTableFrameSink::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{

	BYTE cbGameStatus = GetGameStation();

	switch(cbGameStatus)
	{
	case GAME_STATUS_FREE:		
	case GAME_STATUS_PLAY:
		{



			// ������Ϣ
			GameScene Scene;
			ZeroMemory(&Scene, sizeof(Scene));
			Scene.lPlayScore = m_lPlayScore[bDeskStation];
			Scene.cbBackIndex = m_cbBackIndex;
			CopyMemory( Scene.lPlayCurScore, m_lPlayScore, sizeof(Scene.lPlayCurScore) );
			CopyMemory( Scene.lPlayStartScore, m_lPlayStartScore, sizeof(Scene.lPlayStartScore) );
			Scene.nBulletVelocity = m_nBulletVelocity;
			Scene.nBulletCoolingTime = m_nBulletCoolingTime;
			CopyMemory( Scene.nFishMultiple, m_nFishMultiple, sizeof(Scene.nFishMultiple) );
			CopyMemory( Scene.lBulletConsume, m_lBulletConsume, sizeof(Scene.lBulletConsume) );
			CopyMemory( Scene.lPlayFishCount, m_lPlayFishCount, sizeof(Scene.lPlayFishCount) );
			CopyMemory( Scene.nMultipleValue, m_nMultipleValue, sizeof(Scene.nMultipleValue) );
			CopyMemory( Scene.nMultipleIndex, m_nMultipleIndex, sizeof(Scene.nMultipleIndex) );
			Scene.bUnlimitedRebound = false;
			//_sntprintf(Scene.szBrowseUrl, CountArray(Scene.szBrowseUrl), TEXT("%s/Pay/PayIndex.aspx"),szPlatformLink);
			SendGameStation(bDeskStation, uSocketID, bWatchUser, &Scene, sizeof(Scene));
			//printf("�ӵ��ٶ�:%d,��ȴ:%d", Scene.nBulletVelocity, Scene.nBulletCoolingTime);
			// ��������Ϣ
			DWORD dwTime = timeGetTime();
			DWORD Time = dwTime - m_nStartTime;
		//	printf("%d,%d,%d\n", Time, dwTime, m_nStartTime);
			// ʱ��ͬ��
			CMD_S_Synchronous CMDSSynchronous;
			CMDSSynchronous.nOffSetTime = dwTime - m_nStartTime;
			//printf("%d,%d,%d\n", CMDSSynchronous.nOffSetTime, dwTime, m_nStartTime);
		     SendTableData(bDeskStation, SUB_S_SYNCHRONOUS, &CMDSSynchronous, sizeof(CMDSSynchronous));

			//// ������
			//for( int nIndex = 0; nIndex < m_ArrayFishInfo.GetCount(); ++nIndex )
			//{
			//	 ������Ϣ
			//	SendFish( m_ArrayFishInfo.GetAt( nIndex) );
			//}
			//
			return true;
		}
	}

	return true;
}

// ��ʱ���¼�
bool CTableFrameSink::OnTimer(UINT uTimerID)
{
	switch (uTimerID)
	{
		// �����㶨ʱ
	case IDI_FISH_CREATE:
		{
			// �رն�ʱ��
		KillTimer(IDI_FISH_CREATE);
			
			// ����Ƿ�����
			bool bHavePeople = false;
			for ( WORD i = 0; i < PlayChair_Max; ++i )
			{
				long  GameUser = GetUserIDByDeskStation(i);
				if (GameUser <= 0) continue;
				//if(IsAndroidUser(i)) continue;
				
				bHavePeople = true;
				
				break;
			}
			
			// ���˷���
			if ( !bHavePeople )
			{
				// ȫ�ֱ�ը��ԭ
				if ( GetFishCount(FishType_BaoZhaFeiBiao) > 0 )
				{
					// �����
					int nMultipleMax = 0;
					for ( int nMultipleIndex = 0; nMultipleIndex < Multiple_Max; ++nMultipleIndex )
					{
						nMultipleMax = max( nMultipleMax, m_nMultipleValue[nMultipleIndex] );
					}

					// ��������
					m_dDartStock += nMultipleMax * m_nExplosionStart;
				}

				// ����Ϣ
				m_ArrayFishInfo.RemoveAll();

				// ������Ϣ
				m_cbBackIndex = 0;
				m_nAppearFishCount = 0;
				m_bFishSwimming = FALSE;
				m_bBackExchange = FALSE;

				GameFinish(0, GF_NORMAL);

				return true;
			}

			// ���㵱ǰ��
			int nFishIndex = 0;
			while ( nFishIndex < m_ArrayFishInfo.GetCount() )
			{
				// ��ȡ����Ϣ
				tagFishInfo & TFishInfo = m_ArrayFishInfo.GetAt(nFishIndex);

				// ������λ��
				CDoublePoint PointFish;
				if( FishMove(PointFish, TFishInfo) == FishMoveType_Delete )
				{
					// ���´���
					if( TFishInfo.bRepeatCreate && !m_bBackExchange )
					{
						// ����������
						ResetFish( TFishInfo );

						// ��һ��
						++nFishIndex;
					}
					else
					{

						// ���ӱ�ը
						if( TFishInfo.nFishType == FishType_BaoXiang && TFishInfo.wHitChair != INVALID_CHAIR )
						{
							// �����
							m_bPlaySupply[TFishInfo.wHitChair] = false;
						}

						// ȫ�ֱ�ը��ԭ
						if ( TFishInfo.nFishType == FishType_BaoZhaFeiBiao )
						{
							// �����
							int nMultipleMax = 0;
							for ( int nMultipleIndex = 0; nMultipleIndex < Multiple_Max; ++nMultipleIndex )
							{
								nMultipleMax = max( nMultipleMax, m_nMultipleValue[nMultipleIndex] );
							}

							// ��������
							m_dDartStock += nMultipleMax * m_nExplosionStart;
						}

						// ɾ����
						m_ArrayFishInfo.RemoveAt( nFishIndex );

						// ����
						continue;
					}
				}

				// ��һ��
				++nFishIndex;
			}

			// ��ǰʱ��
			const uint nCurrentTime = timeGetTime();

			// ����ͼ
			if( !m_bBackExchange && nCurrentTime - m_nSceneBegin > (uint)m_nSceneTime * 1000 && !m_bFushSence)
			{
				m_bBackExchange = TRUE;
				m_nSceneBegin = nCurrentTime;
			}

			// �ȴ�����ͼ �������
			if ( !m_bBackExchange )
			{
				// �����
				if ( m_nAppearFishCount == 0 && !m_bBackExchange && !m_bFirstTime )
				{
					// ��������
					DWORD GroupTime = GroupOfFish();
					m_nFishTeamTime = GetTickCount() + GroupTime;
					//BOSSʱ��ȥ������ʱ��
					m_FishKingTime += GroupTime;
				}
				else if ( !m_bBackExchange && ( GetTickCount() > m_nFishTeamTime || m_ArrayFishInfo.GetCount() == 0 ) )
				{
					// ���˵�һ��
					m_bFirstTime = false;

					// ������Ⱥ
					InitializationFishpond();

					// �����
					int nMultipleMax = 0;
					for ( int nMultipleIndex = 0; nMultipleIndex < Multiple_Max; ++nMultipleIndex )
					{
						nMultipleMax = max( nMultipleMax, m_nMultipleValue[nMultipleIndex] );
					}

					// ��������
					if( m_dDartStock >= nMultipleMax * m_nExplosionStart && GetFishCount(FishType_BaoZhaFeiBiao) == 0 )
					{
						// ��������
						m_dDartStock -= nMultipleMax * m_nExplosionStart;

						// ��������
						CreateFish( 1, FishType_BaoZhaFeiBiao );
					}
				}
			}
	
			// ������������Ϣ
			BOOL bChangeScene = FALSE;
			if ( m_bBackExchange && m_ArrayFishInfo.GetCount() < 5 )
			{
				m_cbBackIndex = (m_cbBackIndex + 1)%3;
				m_bBackExchange = FALSE;
				bChangeScene = TRUE;
				m_nAppearFishCount = 0;

				// �������Ϣ
				FishFastMove();

				// ��ͼ��Ϣ
				CMD_S_ExchangeScene ExchangeScene;
				ExchangeScene.cbBackIndex = m_cbBackIndex;

				// ������Ϣ
				SendTableData(INVALID_CHAIR, SUB_S_EXCHANGE_SCENE, &ExchangeScene, sizeof(ExchangeScene) );
			}

			// ������ʱ��
			if ( bChangeScene )
			{
				SetTimer(IDI_FISH_CREATE, 5000);
			}
			else
			{
				SetTimer(IDI_FISH_CREATE, 200);
			}
		}
		return true;
		// �ӳٶ�ʱ
	case IDI_DELAY:
		{
			// �رն�ʱ��
			KillTimer(IDI_DELAY);

			// ����Ƿ�����
			bool bHavePeople = false;
			GameUserInfo *GameUser = NULL;
			for ( WORD i = 0; i < PlayChair_Max; ++i )
			{
				GameUser = GetTableUserItem(i);
				if (GameUser == NULL) continue;

				// ������ʱ��
				bHavePeople = true;
				SetTimer(IDI_DELAY, TIME_DELAY);
				break;
			}

			// ������Ϣ
			for (WORD i = 0; i < PlayChair_Max && bHavePeople; ++i )
			{
				//IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem( i );
				GameUser = GetTableUserItem(i);
				if (GameUser == NULL ) continue;
				//if ( !pIServerUserItem->IsClientReady() ) continue;

				m_nPlayDelay[i] = timeGetTime();
				SendUserItemData(GameUser, SUB_S_DELAY_BEGIN, NULL, NULL);
			}
		}
		return true;

		// ���Ӷ�ʱ
	case IDI_SECOND:
		{
			// �رն�ʱ��
			KillTimer(IDI_SECOND);

			// ����Ƿ�����
			bool bHavePeople = false;
			WORD wSite = INVALID_CHAIR;
			GameUserInfo *GameUser = NULL;
			for ( WORD i = 0; i < PlayChair_Max; ++i )
			{
				GameUser = GetTableUserItem(i);
				if (GameUser == NULL) continue;

				// ������ʱ��
				bHavePeople = true;
				SetTimer(IDI_SECOND, 1000);
				wSite = i;
				break;
			}
			
			// �����ޱ���
			if( !bHavePeople )
				return true;

			// ��������ʱ��
			if ( m_nFishKingTime > 0 )
			{
				m_nFishKingTime--;
			}

			// ��������ʱ��
			for( int nIndex = 0; nIndex < CountArray(m_nFishKing); ++nIndex )
			{
				if ( m_nFishKing[nIndex] > 0 )
				{
					m_nFishKing[nIndex]--;
				}
			}

			// ��Ϸʱ��
			DWORD dwCurrentime = (DWORD)time(NULL);

			uint NowTime = timeGetTime();
			//�ж���Ҷ೤ʱ�䲻����
			//if (m_IsOpenTick)
			//{
			//	for (int i = 0; i < PLAY_COUNT; i++)
			//	{
			//		if (GetUserIDByDeskStation(i) <= 0) continue;

			//		if (NowTime - m_iOptionTime[i] > m_iTickTime * 1000)
			//		{
			//			//�����߳������
			//			KickOutUser(i);
			//		}
			//	}
			//}

			for (int i = 0; i < PLAY_COUNT; i++)
			{
				if (GetUserIDByDeskStation(i) <= 0) continue;
				if (IsAndroidUser(i))
				{


				}
			}
			// ��������Ϸʱ��
			for( int nSite = 0; nSite < PlayChair_Max; ++nSite )
			{
				// ��ҽӿ�
				//IServerUserItem * pServerRobot = m_pITableFrame->GetTableUserItem(nSite);

				if (IsAndroidUser(nSite))
				{
					//��ʱ�������߳�
					if (NowTime - m_iOptionTime[nSite] > m_iRobotTickTime * 1000)
					{
						printf("�����˲������߳�\n");
						//�����߳������
						KickOutUser(nSite, REASON_KICKOUT_STAND_MINLIMIT);
					}

					if (dwCurrentime - m_nPlayDownTime[nSite] > m_nRobotPlayTime[nSite])
					{
						printf("��������Ϸ��ʱ���߳�:%d\n", m_nRobotPlayTime[nSite]);
						//�����߳������
						KickOutUser(nSite, REASON_KICKOUT_STAND_MINLIMIT);
					}
					else if(m_lPlayScore[nSite] < m_nMultipleValue[m_nMultipleIndex[nSite]])
					{
						printf("������Ǯ���㱻�߳�������ȼ���%d**:%lld", GetRoomLevel(), m_lPlayScore[nSite]);
						printf(",%d,%d\n", m_nMultipleIndex[nSite], m_nMultipleValue[m_nMultipleIndex[nSite]]);
						//�����߳������
						KickOutUser(nSite, REASON_KICKOUT_STAND_MINLIMIT);
					}
				}
			}
			return true;
		}
	case TIME_WRITE_SCORE: //��ʱд��
		{
			//�رն�ʱ��
			KillTimer(TIME_WRITE_SCORE);
			// ����Ƿ�����
			for (WORD i = 0; i < PlayChair_Max; ++i)
			{
				long  userid = GetUserIDByDeskStation(i);
				if (userid <= 0) continue;

				//������ô�ͽ���һ��
				GameUserInfo UserInfo;
				GetUserData(i, UserInfo);

				// д��
				LONGLONG lScore = (m_lPlayScore[i] - m_lPlayStartScore[i]);
				if (lScore != 0)
				{
					// ����Ϊ���� ���ܳ���������
					if (lScore < 0 && (-lScore) > UserInfo.money)
					{
						lScore = -UserInfo.money;
					}
				}

				// ��Ϸʱ��
				DWORD dwEndTime = (DWORD)time(NULL) - m_nPlayDownTime[i];
				ChangeUserPoint(i, lScore, m_lplayCutMoney[i]);
				m_lplayCutMoney[i] = 0;

				//��ʼ����
				GetUserData(i, UserInfo);
				m_lPlayScore[i] = UserInfo.money;
				m_lPlayStartScore[i] = UserInfo.money;

				//������ҵĸ����Ѷ�
				//������Ҹ����Ѷ�
				long Userid = GetUserIDByDeskStation(i);
				if (Userid > 0)
				{
					
					// �����Ѷ�
					int UserDifficulty = GetUserControlParam(i);
					//printf("��ң�%d,�����Ѷȣ�%d\n", Userid, UserDifficulty);
					double dGameDifficulty = 0;
					if (UserDifficulty >= 0 && UserDifficulty < 1000)
					{
						int Lev = UserDifficulty / 100;
						dGameDifficulty = m_dPlayDifficultyValue[Lev];
					}
					else if (UserDifficulty >= 1000)
					{
						dGameDifficulty = m_dPlayDifficultyValue[9];
					}
				//	printf("���ø����Ѷȣ�%llf", dGameDifficulty);
					SetPersonalDifficulty(Userid, dGameDifficulty);
				}

				// ����д��
				m_nRoomWrite += (-lScore);		
			}
			SetTimer(TIME_WRITE_SCORE, 30000);
			return true;
		}
	case TIME_FUSH_BOSS:
	{
		KillTimer(TIME_FUSH_BOSS);
		m_bFushBoss = true;
		return true;
	}
		return true;
	}
	return false;
}


//�����Ϣ������
bool CTableFrameSink::HandleFrameMessage(BYTE bDeskStation, unsigned int assistID, void* pData, int size, bool bWatchUser)
{
	return __super::HandleFrameMessage(bDeskStation, assistID, pData, size, bWatchUser);
}


// ��Ϸ��Ϣ
bool CTableFrameSink::HandleNotifyMessage(BYTE deskStation, unsigned int assistID, void* pData, int size, bool bWatchUser)
{
	// �쳣���
	try
	{
		//��¼��Ҳ���ʱ��
		m_iOptionTime[deskStation] = timeGetTime();

		// ��Ϣ����
		bool bSuccess = false;
		switch(assistID)
		{
		case SUB_C_CATCH_FISH:
			bSuccess = OnSubCatchFish(deskStation, pData, size);
			break;
		case SUB_C_FIRE:
			bSuccess = OnSubFire(deskStation, pData, size);
			break;
		case SUB_C_BEGIN_LASER:
			bSuccess = OnSubBeginLaser(deskStation, pData, size);
			break;
		case SUB_C_LASER:
			bSuccess = OnSubLaser(deskStation, pData, size);
			break;
		case SUB_C_DELAY:
			bSuccess = OnSubDelay(deskStation, pData, size);
			break;
		case SUB_C_SPEECH:
			bSuccess = OnSubSpeech(deskStation, pData, size);
			break;
		case SUB_C_MULTIPLE:
			bSuccess = OnSubMultiple(deskStation, pData, size);
			break;
		case SUB_C_CONTROL:
			bSuccess = OnSubControl(deskStation, pData, size);
			break;
		case SUB_C_SKILL:
			bSuccess = OnSubSkill(deskStation, pData, size);
			break;
		case SUB_C_CHANGE_CONNON:
			bSuccess = OnChangeCannon(deskStation, pData, size);
			break;
		}

		// �ж�ִ��״��
		if( !bSuccess )
			DebugPrintf( " %dִ����Ϣ [ %d ] ʧ�ܣ�", GetUserIDByDeskStation(deskStation),assistID);

		return __super::HandleNotifyMessage(deskStation, assistID, pData, size, bWatchUser);
	}
	catch ( CException * pException )
	{
		// ��ȡ������Ϣ
		tchar szErrorMessage[1024] = { _T("") };
		pException->GetErrorMessage(szErrorMessage, 1024);
		pException->Delete();


		return true;
	}
	catch ( ... )
	{
		DebugPrintf("���ش��� ִ����Ϣ [ %d ] ʧ�ܣ�", assistID);
		return true;
	}
	return __super::HandleNotifyMessage(deskStation, assistID, pData, size, bWatchUser);
}

 
 //�û�����
bool CTableFrameSink::UserSitDeskActionNotify(BYTE deskStation)
{

	// ��¼���
	//FishMermaidStock();
	bool bHavePeople = false;
	for (int i = 0;i<PlayChair_Max;i++)
	{
		long  GameUser = GetUserIDByDeskStation(i);
		if (GameUser <= 0) continue;
		//if (IsAndroidUser(i)) continue;

		bHavePeople = true;

		break;
	}
	//û�˻��߻�����ֱ�ӽ���
	//if (!bHavePeople)
	//{
	//	GameFinish(0, GF_NORMAL);
	//}

	SetGameStation(GAME_STATUS_PLAY);
	// ��������
	if( !m_bFishSwimming)
	{
		// ��������
		srand(GetTickCount());

		// ��һ��
		m_bFirstTime = TRUE;

		// ����ʱ��
		m_bFishSwimming = TRUE;

		// ������ʱ��
		SetTimer(IDI_FISH_CREATE, 100);

		// �ӳٶ�ʱ
		SetTimer(IDI_DELAY, TIME_DELAY);

		// �붨ʱ
		SetTimer(IDI_SECOND, 1000);
	
		// ����ʱ��
		m_nStartTime = timeGetTime();
		m_nSceneBegin = m_nStartTime;
	}
	//��¼�������ʱ��
	m_iOptionTime[deskStation] = timeGetTime();

	//��ȡ�����Ϣ
	GameUserInfo UserData;
	GetUserData(deskStation, UserData);
	// ��������
	if( m_dwPlayID[deskStation] == 0 )
	{
		// ��ʼ�����
		InitializePlayer(deskStation);

		// ���������Ϣ
		m_dwPlayID[deskStation] = GetUserIDByDeskStation(deskStation);
		m_lPlayScore[deskStation] = (LONGLONG)UserData.money;
		m_lPlayStartScore[deskStation] = m_lPlayScore[deskStation];
		m_nPlayDownTime[deskStation] = (DWORD)time(NULL);
		m_nRobotPlayTime[deskStation] = ( (rand()%15) + 15 ) * 60;

		//������Ҹ����Ѷ�
		long Userid = GetUserIDByDeskStation(deskStation);
		if (Userid > 0)
		{
			// �����Ѷ�
			int UserDifficulty = GetUserControlParam(deskStation);
			double dGameDifficulty = 0;
			if (UserDifficulty >= 0 && UserDifficulty < 1000)
			{
				int Lev = UserDifficulty / 100;
				dGameDifficulty = m_dPlayDifficultyValue[Lev];
				//printf("�����Ѷ�:%d,�Ѷȵȼ�:%d,�Ѷ�ϵ��%lf\n", UserDifficulty, Lev, RoomDifficultyCount[Lev]);
			}
			else if (UserDifficulty >= 1000)
			{
				dGameDifficulty = m_dPlayDifficultyValue[9];
			}
			SetPersonalDifficulty(Userid, dGameDifficulty);
		}
	}
	// ����״̬
	if (!IsBegin)
	{
		IsBegin = true;

		// ��ʼ��Ϸ
		GameBegin(0);
	}
	return true;
}

// �û�����
bool CTableFrameSink::UserLeftDesk(GameUserInfo* pUser)
{
		
	// ����Ƿ�����
	for ( WORD i = 0; i < PlayChair_Max; ++i )
	{
	   if (pUser->deskStation == i) continue;
		long  userid = GetUserIDByDeskStation(i);
		if (userid <= 0) continue;

		//������ô�ͽ���һ��
		// �ж��û�
		long PlayeruserID = pUser->userID;
		if (PlayeruserID > 0)
		{
			// ���ָ��
			//IServerUserItem * pIServerDismiss = m_pITableFrame->GetTableUserItem(nSite);
			GameUserInfo UserInfo;
			GetUserData(pUser->deskStation, UserInfo);

			// д��
			LONGLONG lScore = (m_lPlayScore[pUser->deskStation] - m_lPlayStartScore[pUser->deskStation]);
			if (lScore != 0)
			{
				// ����Ϊ���� ���ܳ���������
				if (lScore < 0 && (-lScore) > UserInfo.money)
				{
					lScore = -UserInfo.money;
				}
			}

			// ��Ϸʱ��
			DWORD dwEndTime = (DWORD)time(NULL) - m_nPlayDownTime[pUser->deskStation];

			ChangeUserPoint(pUser->deskStation, lScore, m_lplayCutMoney[i]);
			m_lplayCutMoney[i] = 0;

			// ����д��
			m_nRoomWrite += (-lScore);
			// ��������Ϣ
			InitializePlayer(pUser->deskStation);

		}
		return __super::UserLeftDesk(pUser);
	}

	// ɾ����ʱ��
	KillTimer(IDI_FISH_CREATE);
	KillTimer(IDI_DELAY);
	KillTimer(IDI_SECOND);
	//ɾ�������Ѷ�
	DeletePersonalDifficulty(pUser->userID);

	// ȫ�ֱ�ը��ԭ
	if ( GetFishCount(FishType_BaoZhaFeiBiao) > 0 )
	{
		// �����
		int nMultipleMax = 0;
		for ( int nMultipleIndex = 0; nMultipleIndex < Multiple_Max; ++nMultipleIndex )
		{
			nMultipleMax = max( nMultipleMax, m_nMultipleValue[nMultipleIndex] );
		}

		// ��������
		m_dDartStock += nMultipleMax * m_nExplosionStart;
	}

	// ����Ϣ
	m_ArrayFishInfo.RemoveAll();
	m_cbBackIndex = 0;
	m_nAppearFishCount = 0;
	m_bFishSwimming = FALSE;
	m_bBackExchange = FALSE;
	m_nStartTime = 0;

	// ����
	m_nFishTeamTime = 0;
	m_nFishKingTime = 30;
	ZeroMemory(m_nFishKing, sizeof(m_nFishKing));

	if (IsBegin)
	{
		// ������Ϸ
		GameFinish(pUser->deskStation, GF_NORMAL);
	}

	return __super::UserLeftDesk(pUser);
}

// ��������
void CTableFrameSink::GetCustomRule( tagCustomRule & nConfigInfo )
{
	// ������ֵ
	CopyMemory( &nConfigInfo, &m_CustomRule, sizeof(tagCustomRule) );
}

// ��������
void CTableFrameSink::SetCustomRule( tagCustomRule & nConfigInfo, bool bSaveFile )
{
	// ������ֵ
	CopyMemory( &m_CustomRule, &nConfigInfo, sizeof(tagCustomRule) );
	CRedisLoader* pRedis = m_pDataManage->GetRedis();
	RoomBaseInfo roomBasekInfo;
	RoomBaseInfo* pRoomBaseInfo = NULL;
	if (pRedis->GetRoomBaseInfo(m_pDataManage->GetRoomID(), roomBasekInfo))
	{
		pRoomBaseInfo = &roomBasekInfo;
	}
	else
	{
		pRoomBaseInfo = ConfigManage()->GetRoomBaseInfo(m_pDataManage->GetRoomID());
	}
	if (!pRoomBaseInfo)
	{
		return;
	}
	// ������Ϣ
	if ( bSaveFile )
	{
		// ��ȡ��ǰ·��
		TCHAR szDirectoryPath[MAX_PATH + 1] = { _T("") }; 
		GetModuleFileName(NULL, szDirectoryPath, MAX_PATH); 
		PathRemoveFileSpec(szDirectoryPath);

		// ������
		tchar szConfigPath[MAX_PATH] = _T("");
		_sntprintf(szConfigPath, MAX_PATH, TEXT("%s\\FishLKConfig\\%s.fdx"), szDirectoryPath, pRoomBaseInfo->name);
		file * pFile = NULL;
		pFile = fopen( CT2A(szConfigPath), "wb+" );
		if ( pFile != NULL )
		{
			// ��������
			fwrite( &m_CustomRule, sizeof(m_CustomRule), 1, pFile );
			fflush( pFile );
			fclose( pFile );
		}
	}

	// ��������
	CopyMemory( m_nMultipleValue, nConfigInfo.nMultipleValue, sizeof(m_nMultipleValue) ); 

	// ������
	LONGLONG lStockSetAmount = 0;
	for( int nIndex = 0; nIndex < Multiple_Max; ++nIndex )
	{
		m_dRoomStock[nIndex] = (double)((double)nConfigInfo.nInitialStock * (double)m_nMultipleValue[nIndex]);
		lStockSetAmount += (LONGLONG)(((LONGLONG)nConfigInfo.nInitialStock) * ((LONGLONG)m_nMultipleValue[nIndex]));
	}
	CopyMemory(m_nDifficultyStart, nConfigInfo.nDifficultyStart, sizeof(m_nDifficultyStart));
	CopyMemory(m_nRoomDifficultyCount, nConfigInfo.nRoomDifficultyCount, sizeof(m_nRoomDifficultyCount)); 
	CopyMemory(m_nTableDifficultyCount, nConfigInfo.nTableDifficultyCount, sizeof(m_nTableDifficultyCount)); 
	CopyMemory(m_nPlayDifficultyCount, nConfigInfo.nPlayDifficultyCount, sizeof(m_nPlayDifficultyCount)); 
	CopyMemory(m_dRoomDifficultyValue, nConfigInfo.dRoomDifficultyValue, sizeof(m_dRoomDifficultyValue)); 
	CopyMemory(m_dTableDifficultyValue, nConfigInfo.dTableDifficultyValue, sizeof(m_dTableDifficultyValue)); 
	CopyMemory(m_dPlayDifficultyValue, nConfigInfo.dPlayDifficultyValue, sizeof(m_dPlayDifficultyValue)); 

	// ������
	m_lStockInitial = m_lStockInitial + (lStockSetAmount - m_lStockCurrent);
	m_lStockCurrent = lStockSetAmount;

	// ��������
	m_dTaxRatio = min( (double)nConfigInfo.nTaxRatio / 1000.0, 0.90 );

	// ��������
	m_nCreateCount = nConfigInfo.nCreateCount;
	m_nSceneTime = nConfigInfo.nSceneTime;

	// �ӵ���Ϣ
	m_nBulletCoolingTime = nConfigInfo.nBulletCoolingTime;
	m_nBulletVelocity = nConfigInfo.nBulletVelocity;

	// �����
	CopyMemory(m_nSupplyCondition, nConfigInfo.nSupplyCondition, sizeof(m_nSupplyCondition)); 
	m_nLaserTime = nConfigInfo.nLaserTime;
	m_nLaserChance = nConfigInfo.nLaserChance;
	m_nSpeedTime = nConfigInfo.nSpeedTime;
	m_nSpeedChance = nConfigInfo.nSpeedChance;
	CopyMemory(m_nGiftScore, nConfigInfo.nGiftScore, sizeof(m_nGiftScore)); 
	CopyMemory(m_nGiftChance, nConfigInfo.nGiftChance, sizeof(m_nGiftChance)); 
	m_nNullChance = max(nConfigInfo.nNullChance, 2);

	// ������
	CopyMemory(m_nFishMultiple, nConfigInfo.nCatchFishMultiple, sizeof(m_nFishMultiple)); 

	// ��ը����
	m_nExplosionProportion = nConfigInfo.nExplosionProportion;
	m_nExplosionStart = nConfigInfo.nExplosionStart;
	m_lExplosionCondition = nConfigInfo.lExplosionCondition;
	m_nExplosionConditionType = nConfigInfo.nExplosionConditionType;
	m_nExplosionProportion = max( m_nExplosionProportion, 0 );
	m_nExplosionProportion = min( m_nExplosionProportion, 1000 );

	// ������Ϣ
	CMD_S_UpdateGame CMDSUpdateGame;
	CopyMemory( CMDSUpdateGame.nMultipleValue, m_nMultipleValue, sizeof(CMDSUpdateGame.nMultipleValue) );
	CopyMemory( CMDSUpdateGame.nCatchFishMultiple, m_nFishMultiple, sizeof(CMDSUpdateGame.nCatchFishMultiple) );
	CMDSUpdateGame.nBulletVelocity = m_nBulletVelocity;
	CMDSUpdateGame.nBulletCoolingTime = m_nBulletCoolingTime;

	// ȫ���䷢��
	SendTableData(INVALID_CHAIR, SUB_S_UPDATE_GAME, &CMDSUpdateGame, sizeof(CMD_S_UpdateGame) );
}

// ���ظ����Ѷ�
void CTableFrameSink::GetPersonalDifficulty( CMapPersonalDifficulty & MapPersonalDifficulty )
{
	POSITION Pos = m_MapPersonalDifficulty.GetStartPosition();
	while ( Pos )
	{
		unsigned long lPlayID = 0;
		double dPersonalDifficulty = 0.0;
		m_MapPersonalDifficulty.GetNextAssoc( Pos, lPlayID, dPersonalDifficulty );
		MapPersonalDifficulty.SetAt( lPlayID, dPersonalDifficulty );
	}
}
//��ȡ��ҵ��Ѷ�
void CTableFrameSink::GetUserDifficulty(unsigned long lPlayID, double& dPersonalDifficulty)
{
	m_MapPersonalDifficulty.Lookup(lPlayID, dPersonalDifficulty);
}
// ���ø����Ѷ�
void CTableFrameSink::SetPersonalDifficulty( unsigned long lPlayID, double dPersonalDifficulty )
{
	m_MapPersonalDifficulty.SetAt( lPlayID, dPersonalDifficulty );
}

// ɾ�������Ѷ�
void CTableFrameSink::DeletePersonalDifficulty( unsigned long lPlayID )
{
	m_MapPersonalDifficulty.RemoveKey( lPlayID );
}

// ��ո����Ѷ�
void CTableFrameSink::ClearPersonalDifficulty()
{
	m_MapPersonalDifficulty.RemoveAll();
}

// ��ȡ���
void CTableFrameSink::GetStock( LONGLONG & lStockInitial, LONGLONG & lStockCurrent )
{
	lStockInitial = m_lStockInitial;
	lStockCurrent = m_lStockCurrent;
}

// �������ӿ��
bool CTableFrameSink::SetTableStock( unsigned short TableID, int nTableStock )
{
	// �ж���Ч
	//if( TableID >= m_pGameServiceOption->wTableCount )
	//{
	//	return false;
	//}

	// ���ÿ��
	for ( int nIndex = 0; nIndex < Multiple_Max; ++nIndex )
	{
		// ���ӿ��
		double & dTableStock = _TableStock(TableID, nIndex);

		// ���ÿ��
		dTableStock = nTableStock * m_nMultipleValue[nIndex];
	}

	return true;
}



// �����¼�
bool CTableFrameSink::OnSubCatchFish(BYTE deskStation, const void * pBuffer, WORD wDataSize )
{
	// Ч������
	ASSERT(wDataSize==sizeof(CMD_C_CatchFish));
	if (wDataSize!=sizeof(CMD_C_CatchFish)) return false;
	if (GetUserIDByDeskStation(deskStation) <= 0)
	{
		return true;
	}
	// ��Ϣ����
	CMD_C_CatchFish * pCatchFish = (CMD_C_CatchFish *)pBuffer;
	if (!pCatchFish)
	{
		DebugPrintf("�������:%d,OnSubCatchFish�������¼��������ݰ�",GetUserIDByDeskStation(deskStation));
		return false;
	}
	// �������
	//WORD wChairID = pIServerUserItem->GetChairID();

	// �����ӵ�
	map<int, tagFireInfo>::iterator itr = m_ArrayBulletKey[deskStation].find(pCatchFish->nBulletKey);

	if( itr == m_ArrayBulletKey[deskStation].end() )
	{
		ASSERT(FALSE);

		return true;
	}

	// ��ȡ�ӵ���Ϣ
	tagFireInfo & TFireInfo = itr->second;

	// �ӵ����
	int nBulletSplit = 0;
	for ( int nIndex = 0; nIndex < FishCatch_Max; ++nIndex )
	{
		// ��Ч����
		if ( pCatchFish->nFishKey[nIndex] != 0 )
		{
			
			nBulletSplit++;
		}
	}

	// ���㲶��
	for ( int nIndex = 0; nIndex < FishCatch_Max; ++nIndex )
	{
		// ��Ч����
		if ( pCatchFish->nFishKey[nIndex] != 0 )
		{
			// ����Ϣ
			bool				bRealKill = false;
			byte				nFishType = FishType_Max;				
			EnumFishState		nFishState = FishState_Normal;

			// ��ȡ����Ϣ
			for( int nDeathIndex = 0; nDeathIndex < m_ArrayFishInfo.GetCount(); nDeathIndex++ )
			{
				// ��ȡ����Ϣ
				tagFishInfo & TFishInfo = m_ArrayFishInfo.GetAt(nDeathIndex);

				// ������
				if ( TFishInfo.nFishKey == pCatchFish->nFishKey[nIndex] )
				{
					//printf("�������key:%d\n", pCatchFish->nFishKey[nIndex]);
					// ������Ϣ
					nFishType = TFishInfo.nFishType;
					nFishState = TFishInfo.nFishState;

					// ��Ч��ɱ
					if( TFishInfo.wHitChair == deskStation || TFishInfo.wHitChair == INVALID_CHAIR )
					{
						bRealKill = true;
					}

					break;
				}
			}

			// ��Ч��ɱ
			if ( bRealKill )
			{
				// ���㲶������
				LONGLONG lCatchScore = PlayCatchFish(deskStation, nBulletSplit, pCatchFish->nFishKey[nIndex], TFireInfo.nMultipleIndex, EPCT_Bullet);

				// �ж�����Ϣ
				if(nFishState >= FishType_BChuiTouSha /*nFishState == FishState_Killer || nFishType == FishType_BaoZhaFeiBiao || nFishType == FishType_ZhongYiTang */)
				{
					
					// �󱶷�����Ϣ
					int nCatchMultiple = (int)(lCatchScore / m_nMultipleValue[TFireInfo.nMultipleIndex]);
					SendAwardTip(deskStation, nFishType, nCatchMultiple, lCatchScore, EST_Cold);
					//printf("��Ч��ɱ����:%d\n", nCatchMultiple);
				}
			}
		}
	}

	// ɾ���ӵ�
	m_ArrayBulletKey[deskStation].erase(itr);

	return true;
}


// ����
bool CTableFrameSink::OnSubFire(BYTE deskStation, const void * pBuffer, WORD wDataSize)
{
	// Ч������
	static int paoCount;
	if (IsAndroidUser(deskStation))
	{
		printf("�����˿���:%d\n", paoCount);
		paoCount++;
	}
	if (paoCount > 999)
	{
		paoCount = 0;
	}
	ASSERT(wDataSize==sizeof(CMD_C_Fire));
	if (wDataSize!=sizeof(CMD_C_Fire)) return false;
	if (GetUserIDByDeskStation(deskStation) <= 0)
	{
		DebugPrintf("���ID������");
		return true;
	}
	// ��Ϣ����
	CMD_C_Fire * pFire = (CMD_C_Fire *)pBuffer;
	if (!pFire)
	{
		DebugPrintf("�������%d,OnSubFire���Ϳ����ݰ�", GetUserIDByDeskStation(deskStation));
		return false;
	}

	// �������
	//WORD wChairID = pIServerUserItem->GetChairID();

	// ��Ч����
	if ( pFire->nMultipleIndex != m_nMultipleIndex[deskStation] )
	{
		ASSERT(FALSE);

		return true;
	}

	// �ӵ�������Ϣ
	int nBulletScore = 0;
	byte cbBulletIndex = 0;
	LONGLONG lBulletInvest = 0;
	lBulletInvest = m_nMultipleValue[m_nMultipleIndex[deskStation]] * QianPao_Bullet;
	//printf("����ȼ�:%d,���ڱ���:%d,%d,%lld\n",GetRoomLevel() ,m_nMultipleIndex[deskStation], m_nMultipleValue[m_nMultipleIndex[deskStation]], lBulletInvest);
	// �۳����
	if ( m_lPlayScore[deskStation] < lBulletInvest )
	{
	//	FishMermaidLogo( _T("��ͨ�ӵ���ҿ۳�ʧ�� %d, [P %I64d] [L %I64d]"), ( pIServerUserItem->IsAndroidUser() ? 1 : 0 ), m_lPlayScore[wChairID], lBulletInvest );

		ASSERT( FALSE );

		return true;
	}

	// �޸Ŀ���
	LONGLONG lPlayExplosionCondition = 0;
	m_MapPlayExplosionCondition.Lookup(GetUserIDByDeskStation(deskStation), lPlayExplosionCondition );
	
	// δ��������
	if ( lPlayExplosionCondition < m_lExplosionCondition )
	{
		if( m_nExplosionConditionType == ExplosionConditionType_Gun )
		{
			lPlayExplosionCondition += 1;
			m_MapPlayExplosionCondition.SetAt(GetUserIDByDeskStation(deskStation), lPlayExplosionCondition );
		}
		else
		{
			lPlayExplosionCondition += lBulletInvest;
			m_MapPlayExplosionCondition.SetAt(GetUserIDByDeskStation(deskStation), lPlayExplosionCondition );
		}
	}

	// �޸Ľ��
	m_lPlayScore[deskStation] -= lBulletInvest;

	// ��¼����
	nBulletScore = (int)lBulletInvest;
	m_lFireCount[deskStation] += 1;
	m_lBulletConsume[deskStation] += (long)lBulletInvest;

	// �������
	if ( !m_bPlaySupply[deskStation] )
		m_nEnergyValue[deskStation] += 1;
	
	// �Ϸ�����
	PlayerConsume(deskStation, m_nMultipleIndex[deskStation], lBulletInvest );

	// ����ӵ�
	tagFireInfo FireInfo;
	FireInfo.nMultipleIndex = m_nMultipleIndex[deskStation];
	FireInfo.dBulletInvest = (double)lBulletInvest;
	m_ArrayBulletKey[deskStation].insert( map<int, tagFireInfo>::value_type(pFire->nBulletKey, FireInfo) );
	//printf("�ӵ��ؼ�ֵ:%d,��������:%d,�ӵ�����:%d", pFire->nBulletKey, FireInfo.nMultipleIndex, FireInfo.dBulletInvest);
	// ������Ϣ
	CMD_S_Fire CMDSFire;
	CMDSFire.wChairID = deskStation;
	CMDSFire.nBulletScore = nBulletScore;
	CMDSFire.nMultipleIndex = m_nMultipleIndex[deskStation];
	CMDSFire.nTrackFishIndex = pFire->nTrackFishIndex;
	CMDSFire.ptPos = pFire->ptPos;

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (GetUserIDByDeskStation(i) <= 0) continue;
		CMDSFire.llPlayScore = m_lPlayScore[i];
		SendTableData(i, SUB_S_FIRE, &CMDSFire, sizeof(CMDSFire));
	}

	// �������ж�(��ʱδ����)
	if ( false && !m_bPlaySupply[deskStation] && m_nSupplyValue[deskStation] != 0 && m_nEnergyValue[deskStation] >= m_nSupplyValue[deskStation] && !m_bBackExchange )
	{
		// ����·��
		uint nPathIndex = deskStation;

		// ����������
		CreateFish( 1, FishType_BaoXiang, nPathIndex, 0, 0, 0.f, false, false, KillerType_No, deskStation, CShortPoint(0, 0), 0.f, false );

		// �������
		m_nEnergyValue[deskStation] = 0;

		// ��������
		m_bPlaySupply[deskStation] = true;

		// ��������
		m_nSupplyValue[deskStation] = RAND_EQUAL_MIN_MAX(m_nSupplyCondition[0], m_nSupplyCondition[1]);

		// ������ʾ��Ϣ
		CMD_S_SupplyTip CMDSSupplyTip;
		CMDSSupplyTip.wChairID = deskStation;
		SendTableData(INVALID_CHAIR, SUB_S_SUPPLY_TIP, &CMDSSupplyTip, sizeof(CMDSSupplyTip) );
	}

	return true;
}

// ׼������
bool CTableFrameSink::OnSubBeginLaser(BYTE deskStation, const void * pBuffer, WORD wDataSize)
{
	// Ч������
	ASSERT(wDataSize==sizeof(CMD_C_BeginLaser));
	if (wDataSize!=sizeof(CMD_C_BeginLaser) ) return false;

	if (GetUserIDByDeskStation(deskStation) <= 0)
	{
		DebugPrintf("������Ҳ�����");
		return false;
	}
	// ������ֵ�ж�
	//if ( m_nLaserPlayTime[pIServerUserItem->GetChairID()] <= 0 )
	//{
	//	FishMermaidLogo( _T("����ʹ�ó�ʱ - 1��") );

	//	ASSERT(FALSE);

	//	return true;
	//}

	// ��Ϣ����
	CMD_C_BeginLaser * pBeginLaser = (CMD_C_BeginLaser *)pBuffer;
	if (!pBeginLaser)
	{
		DebugPrintf("����:���%d��OnSubBeginLaser���ڿ�ָ��", GetUserIDByDeskStation(deskStation));
		return false;
	}
	// ������Ϣ
	CMD_S_BeginLaser BeginLaser;
	BeginLaser.wChairID = deskStation;
	BeginLaser.ptPos = pBeginLaser->ptPos;

	// ������Ϣ
	SendTableData(INVALID_CHAIR, SUB_S_BEGIN_LASER, &BeginLaser, sizeof(BeginLaser) );

	return true;
}

// ����
bool CTableFrameSink::OnSubLaser(BYTE deskStation, const void * pBuffer, WORD wDataSize)
{
	// Ч������
	ASSERT(wDataSize==sizeof(CMD_C_Laser));
	if (wDataSize!=sizeof(CMD_C_Laser)) return false;

	// ������ֵ�ж�
	//if ( m_nLaserPlayTime[pIServerUserItem->GetChairID()] <= 0 )
	//{
	//	FishMermaidLogo( _T("����ʹ�ó�ʱ - 2��") );

	//	ASSERT(FALSE);

	//	return true;
	//}
	if (GetUserIDByDeskStation(deskStation) <= 0)
	{
		DebugPrintf("�����Ҳ������%d", deskStation);
		return false;
	}
	// ��Ϣ����
	CMD_C_Laser * pLaser = (CMD_C_Laser *)pBuffer;

	if (!pLaser)
	{
		DebugPrintf("������� :%d,OnSubLaser��ָ��", GetUserIDByDeskStation(deskStation));
		return false;
	}
	// �������
	CMD_S_Laser Laser;
	WORD wChairID = deskStation;

	// ������Ϣ
	Laser.wChairID = deskStation;
	Laser.ptPos = pLaser->ptEndPos;

	// �������
	m_nLaserPlayTime[deskStation] = 0;

	// ������Ϣ
	SendTableData(INVALID_CHAIR, SUB_S_LASER, &Laser, sizeof(Laser) );

	// �Ǽ���
	FLOAT fRadian = atan2((FLOAT)( pLaser->ptEndPos.y - pLaser->ptBeginPos.y ) , (FLOAT)( pLaser->ptBeginPos.x - pLaser->ptEndPos.x ));
	fRadian = -fRadian - GL_PI / 2;

	// ���ɼ��ⷶΧ
	CFloatVector2 FVBullet;
	CFloatRotate  FRBullet;
	CPolygonShape SPBullet;
	FRBullet.Set(fRadian);
	FVBullet.Set(pLaser->ptBeginPos.x, pLaser->ptBeginPos.y);
	CFloatVector2 FVLaser[4];
	FVLaser[0].Set( -50, -(max(DEFAULE_WIDTH, DEFAULE_HEIGHT)) );
	FVLaser[1].Set(  50, -(max(DEFAULE_WIDTH, DEFAULE_HEIGHT)) );
	FVLaser[2].Set(  50, 0 );
	FVLaser[3].Set( -50, 0 );
	SPBullet.Set( FVLaser, 4 );

	// ���㲶��
	CWHArray< uint > ArrayFishCatchKey;
	for( int nFishIndex = 0; nFishIndex < m_ArrayFishInfo.GetCount(); ++nFishIndex )
	{
		// ��ȡ��Ϣ
		tagFishInfo & TFishInfo = m_ArrayFishInfo.GetAt( nFishIndex );

		// ������λ��
		CDoublePoint PointFish;
		if( FishMove( PointFish, TFishInfo, pLaser->unLossTime ) == FishMoveType_Normal )
		{
			// ��Чλ��
			if ( PointFish.x < 0 || PointFish.x > DEFAULE_WIDTH || PointFish.y < 0 || PointFish.y > DEFAULE_HEIGHT )
			{
				continue;
			}

			// �Ƚ�λ��
			if( SPBullet.PointInShape( CFloatTransform( FVBullet, FRBullet ), CFloatVector2(PointFish.x, PointFish.y) ) )
			{
				ArrayFishCatchKey.Add( TFishInfo.nFishKey );
			}
		}
	}

	// ���㲶������
	LONGLONG lCatchScore = 0;

	// ���㲶��
	for ( int nFishCatchKeyIndex = 0; nFishCatchKeyIndex < ArrayFishCatchKey.GetCount(); nFishCatchKeyIndex++ )
	{
		// ���㲶��
		lCatchScore += PlayCatchFish(deskStation, (int)ArrayFishCatchKey.GetCount(), ArrayFishCatchKey[nFishCatchKeyIndex], m_nMultipleIndex[deskStation], EPCT_Laser);
	}

	// �󱶷�����Ϣ
	int nCatchMultiple = (int)(lCatchScore / m_nMultipleValue[m_nMultipleIndex[deskStation]]);
	SendAwardTip(deskStation, 0, nCatchMultiple, lCatchScore, EST_Laser);
	//printf("�󱶷���:%d\n", nCatchMultiple);
	return true;
}

// �ӳ�
bool CTableFrameSink::OnSubDelay(BYTE deskStation, const void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize == 0);
	if (wDataSize != 0) return false;

	// �������
	WORD wChairID = deskStation;

	// ������Ϣ
	CMD_S_Delay Delay;

	// �����ӳ�
	DWORD dwTime = timeGetTime();
	if ( dwTime < m_nPlayDelay[deskStation] )
		Delay.nDelay = ULONG_MAX - m_nPlayDelay[deskStation] + dwTime;
	else
		Delay.nDelay = dwTime - m_nPlayDelay[deskStation];

	// ����λ��
	Delay.wChairID = deskStation;

	// �����������ʱ
	if( IsAndroidUser(deskStation) )
	{
		Delay.nDelay = rand()%50 + 50;
	}

	// ������Ϣ
	SendTableData(INVALID_CHAIR, SUB_S_DELAY, &Delay, sizeof(CMD_S_Delay) );

	return true;
}


// ����
bool CTableFrameSink::OnSubSpeech(BYTE deskStation, const void * pBuffer, WORD wDataSize)
{
	// Ч������
	ASSERT(wDataSize==sizeof(CMD_C_Speech));
	if (wDataSize!=sizeof(CMD_C_Speech)) return false;

	//// ��Ϣ����
	//CMD_C_Speech * pSpeech = (CMD_C_Speech *)pBuffer;

	//// �ж���Ϣ
	//if ( pSpeech->nSpeechIndex >= SPEECH_INDEX_MAX )
	//	return true;
	//
	//// ������Ϣ
	//CMD_S_Speech Speech;
	//Speech.wChairID = pIServerUserItem->GetChairID();
	//Speech.nSpeechIndex = pSpeech->nSpeechIndex;
	//SendTableData(SUB_S_SPEECH, &Speech, sizeof(Speech));

	return true;
}

// ����ѡ��
bool CTableFrameSink::OnSubMultiple(BYTE deskStation, const void * pBuffer, WORD wDataSize)
{
	// Ч������
	ASSERT(wDataSize==sizeof(CMD_C_Multiple));
	if (wDataSize!=sizeof(CMD_C_Multiple)) return false;
	if (GetUserIDByDeskStation(deskStation) <= 0)
	{
		DebugPrintf("����:���%d������!", deskStation);
		return false;
	}
	// ��Ϣ����
	CMD_C_Multiple * pMultiple = (CMD_C_Multiple *)pBuffer;
	if (!pMultiple)
	{
		DebugPrintf("����:���%d,����ѡ��OnSubMultiple��ָ��", GetUserIDByDeskStation(deskStation));
		return false;
	}
	// �������
	WORD wChairID = deskStation;

	// �ж���Ϣ
	if ( pMultiple->nMultipleIndex >= Multiple_Max || pMultiple->nMultipleIndex < 0 )
	{
		DebugPrintf("���%d���ñ���ʧ��, ������Чֵ��", deskStation);

		ASSERT(FALSE);

		return false;
	}

	// ����ͱ������
	if ( m_nLaserPlayTime[deskStation] > 0 || m_bPlaySupply[deskStation] )
	{
		DebugPrintf("��ڼ䣬���%d,ID:%d���ñ�����Ч,���⣺%d,������:%d��", deskStation, GetUserIDByDeskStation(deskStation), m_nLaserPlayTime[deskStation], m_bPlaySupply[deskStation]);

		ASSERT(FALSE);

		return true;
	}

	// ���ñ���
	m_nMultipleIndex[deskStation] = pMultiple->nMultipleIndex;

	// ������Ϣ
	CMD_S_Multiple Multiple;
	Multiple.wChairID = deskStation;
	Multiple.nMultipleIndex = pMultiple->nMultipleIndex;
	SendTableData(INVALID_CHAIR, SUB_S_MULTIPLE, &Multiple, sizeof(Multiple));

	return true;
}

// ������Ϣ
bool CTableFrameSink::OnSubControl(BYTE deskStation, VOID * pData, WORD wDataSize )
{
	// Ч������
	ASSERT(wDataSize==sizeof(CMD_C_Control));
	if (wDataSize!=sizeof(CMD_C_Control)) return false;

	// �������
	CMD_C_Control * pControl = (CMD_C_Control*)pData;
	if (GetUserIDByDeskStation(deskStation) <= 0)
	{
		DebugPrintf("����:���%d������!", deskStation);
		return false;
	}
	if (!pControl)
	{
		DebugPrintf("����:���%d,������ϢOnSubControl��ָ��", GetUserIDByDeskStation(deskStation));
		return false;
	}
	// �ж�Ȩ��
	//if( !CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) )
	return true;

}

// ������
uint CTableFrameSink::CreateFishEx( byte cbCount, 
								   byte cbFishType /*= FishType_Max*/, 
								   tagBezierPoint TBezierPoint[BEZIER_POINT_MAX] /*= NULL*/, 
								   int nBezierCount /*= 0*/, 
								   uint unCreateTime /*= 0*/, 
								   uint unIntervalTime /*= 1000*/, 
								   float fRotateAngle /*= 0.f*/, 
								   bool bCanSpecial /*= true*/, 
								   bool bCanAquatic /* = true */, 
								   EnumKillerType nKillerType /*= KillerType_No*/, 
								   WORD wHitChair /*= INVALID_CHAIR*/, 
								   CShortPoint & PointOffSet /*= CShortPoint(0,0)*/, 
								   float fInitialAngle /*= 0.f*/, 
								   bool bRepeatCreate /*= true */ )
{
	// ��Ч·��
	if ( TBezierPoint == NULL || nBezierCount == 0 )
	{
		//_Assert( false );

		return 0;
	}

	// �������
	int nPlayCount = 0;
	for( int nSite = 0; nSite < PlayChair_Max; ++nSite )
	{
		if( GetUserIDByDeskStation(nSite) > 0 )
			nPlayCount++;
	}

	// ��������
	if ( cbFishType == FishType_Max )
	{
		// ���㼸��
		int ChuiTouSha = 4;
		int JinJuChiSha = 4;
		int JinKuiJiaYu = 4;
		int JinChuiTouSha = 4;
		int JinShaYu = 4;
		int JinHuSha = 4;
		int BOSS = 4;
		int HongBao = 40;
		int HuaFei1 = 40;
		int HuaFei2 = 40;

		if( GetFishCount(FishType_BChuiTouSha) >= 1 ) ChuiTouSha = 0;
		if (GetFishCount(FishType_BJinJuChiSha) >= 1) JinJuChiSha = 0;
		if (GetFishCount(FishType_BJinKuiJiaYu) >= 1) JinKuiJiaYu = 0;
		if (GetFishCount(FishType_BJinChuiTouSha) >= 1) JinChuiTouSha = 0;
		if (GetFishCount(FishType_BJinShaYu) >= 1) JinShaYu = 0;
		if (GetFishCount(FishType_BJinHuSha) >= 1) JinHuSha = 0;
		if (GetFishCount(FishType_BOSS) >= 1) BOSS = 0;

		if (GetFishCount(FishType_BChuiTouSha) + GetFishCount(FishType_BJinJuChiSha) + GetFishCount(FishType_BJinKuiJiaYu) + GetFishCount(FishType_BJinChuiTouSha) + GetFishCount(FishType_BJinShaYu)
			+ GetFishCount(FishType_BJinHuSha) > 2)
		{
			int ChuiTouSha = 0;
			int JinJuChiSha = 0;
			int JinKuiJiaYu = 0;
			int JinChuiTouSha = 0;
			int JinShaYu = 0;
			int JinHuSha = 0;
		}

		if (GetFishCount(FishType_BOSS) >= 1)
		{
			int ChuiTouSha = 0;
			int JinJuChiSha = 0;
			int JinKuiJiaYu = 0;
			int JinChuiTouSha = 0;
			int JinShaYu = 0;
			int JinHuSha = 0;
		}
		if (GetFishCount(FishType_Hong_Bao)  >= 1)
		{
			HongBao = 0;
		}
		if (GetFishCount(FishType_Hua_Fei1) + GetFishCount(FishType_Hua_Fei2) >= 1)
		{
			HuaFei1 = 0;
			HuaFei2 = 0;
		}
		// �����
		cbFishType = RandomArea(FishType_Normal_Max, 2, 5, 5, 20, 20, 20, 20, 20, 10, 10, 10, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, ChuiTouSha, JinJuChiSha, JinKuiJiaYu, JinChuiTouSha, JinShaYu, JinHuSha, 0,HongBao, HuaFei1,HuaFei2);
		//����ˢ��ʱ��
		if (m_FishKingTime == 0)
		{
			m_FishKingTime = timeGetTime();
			m_bFushBoss = false;
			m_bFushSence = false;
		}
		else if (!m_bIsFushBoss && m_FishKingTime > 0 && (timeGetTime() - m_FishKingTime) >m_FishKingIntervalTime && GetFishCount(FishType_BOSS) <= 0)
		{
			m_bFushSence = true;
			m_bIsFushBoss = true;
			SetTimer(TIME_FUSH_BOSS, 5000);
			SendTableData(INVALID_CHAIR, SUB_S_GAME_BOSS, 0, 0);
		}

		if (m_bFushBoss)
		{
			m_bFushBoss = false;
			m_bFushSence = true;
			m_bIsFushBegin = true;
			cbFishType = FishType_BOSS;
			m_FishKingTime = timeGetTime();
		}

		if (GetFishCount(FishType_BOSS) <= 0 && m_bIsFushBegin && cbFishType!= FishType_BOSS)
		{
			m_bFushSence = false;
			m_bIsFushBoss = false;
			m_bIsFushBegin = false;
		}
	}

	// �����
	int nFishScore = 0;
	RAND_MIN_MAX(nFishScore, m_nFishMultiple[cbFishType][0], m_nFishMultiple[cbFishType][1] );

	//  ���䱣�浱ǰ����
	if ( cbFishType == FishType_BaoXiang && wHitChair != INVALID_CHAIR )
	{
		nFishScore = m_nMultipleValue[m_nMultipleIndex[wHitChair]];
	}

	// �ζ�ʱ��
	uint unSwimmiTime = 0;
	for( int nBezierIndex = 0; nBezierIndex < nBezierCount; ++nBezierIndex )
		unSwimmiTime += TBezierPoint[nBezierIndex].Time;

	// ɱ��������
	int nKillerIndex = int_max;
	if( nKillerType == KillerType_One && cbFishType < FishType_Small_Max )
	{
		nKillerIndex = rand()%cbCount;
	}

	// ��������
	double dCurChance = 0.0;

	// ѭ������
	int nIndex = 0;
	const uint unCurrentTime = timeGetTime();
	ASSERT( unCurrentTime >= m_nStartTime );
	while ( nIndex < cbCount )
	{
		// ������
		bool bSpecial = false;				
		if( GetFishSpecialCount() < 2 && bCanSpecial && m_nFishKingTime == 0 && m_nFishKing[cbFishType] == 0 && cbFishType < FishType_Small_Max && nFishScore > 0 && nFishScore <= 10 && RandomArea(2, 3, 19) == 0 )
		{
			m_nFishKing[cbFishType] = 30;
			m_nFishKingTime = 45 - (nPlayCount * 5);
			bSpecial = true;
		}

		// ͣ������
		bool bAquatic = false;	
		if( bCanAquatic && nFishScore < 20 && RandomArea(2, 1, 30) == 0 && cbFishType != FishType_BaoXiang )
		{
			bAquatic = true;
		}

		// ������Ϣ
		tagFishInfo FishInfo;
		FishInfo.nFishKey = max( 1, m_nAppearFishCount + 1);
		FishInfo.nFishType = cbFishType;
		FishInfo.unCreateTime = (unCurrentTime - m_nStartTime) + unCreateTime + nIndex * unIntervalTime;
		FishInfo.unOverTime = unSwimmiTime;
		FishInfo.nScoreChance = nFishScore;
		FishInfo.dCurChance	= dCurChance;
		FishInfo.fRotateAngle = fRotateAngle;
		FishInfo.PointOffSet = PointOffSet;
		FishInfo.wHitChair = wHitChair;
		FishInfo.nFishScore = nFishScore;
		FishInfo.nBezierCount = nBezierCount; 
		FishInfo.bRepeatCreate = bRepeatCreate;
		FishInfo.fInitialAngle = fInitialAngle;
		CopyMemory( FishInfo.TBezierPoint, TBezierPoint, sizeof(FishInfo.TBezierPoint) );

		if ( nIndex == nKillerIndex || nKillerType == KillerType_All )
		{
			FishInfo.nFishState = FishState_Killer;
		}
		else if ( bSpecial )
		{
			FishInfo.nFishState = FishState_King;
		}
		else if ( bAquatic )
		{
			FishInfo.nFishState = FishState_Aquatic;
		}
		else
		{
			FishInfo.nFishState = FishState_Normal;
		}

		// ��������
		m_nAppearFishCount++;

		// �������
		m_ArrayFishInfo.Add(FishInfo);

		// ��������Ϣ
		SendFish(FishInfo);

		// ��һ��
		nIndex++;
	}

	return unCreateTime + nIndex * unIntervalTime + unSwimmiTime;
}


// ������
uint CTableFrameSink::CreateFish( byte cbCount, 
								  byte cbFishType		/* = FishType_Max */, 
								  uint nPathIndex		/* = uint_max */,
								  uint unCreateTime		/* = 0 */,
								  uint unIntervalTime	/* = 1000 */,
								  float fRotateAngle	/* = 0.f */, 
								  bool bCanSpecial		/* = true */, 
								  bool bCanAquatic		/* = true */, 
								  EnumKillerType nKillerType /* = KillerType_No */,
								  WORD wHitChair		/* = INVALID_CHAIR */,
								  CShortPoint & PointOffSet /*= CShortPoint(0,0)*/, 
								  float fInitialAngle /*= 0.f*/, 
								  bool bRepeatCreate /*= true*/)
{

	// ���·��
	int nBezierCount = 0;
	tagBezierPoint TBezierPoint[BEZIER_POINT_MAX];
	if ( (int64)nPathIndex >= m_ArrayFishPathPositive.GetCount() )
	{
		// ��ָ��
		RandomPath( cbFishType, TBezierPoint, nBezierCount );
	}
	else
	{
		// ��ȡ�̶�·��
		ASSERT(nPathIndex < (uint)m_ArrayFishPathPositive.GetCount() );
		CWHArray< tagBezierPoint * > * pArrayBezierPoint = &(m_ArrayFishPathPositive[nPathIndex]->ArrayBezierPoint);

		// ��ֵ·��
		ASSERT( pArrayBezierPoint->GetCount() > 0 && pArrayBezierPoint->GetCount() <= BEZIER_POINT_MAX );
		nBezierCount = (int)pArrayBezierPoint->GetCount();
		for ( int nIndex = 0; nIndex < nBezierCount; ++nIndex )
		{
			// ��ȡ·��
			tagBezierPoint * pBezierPoint = pArrayBezierPoint->GetAt(nIndex);
			if (!pBezierPoint)
			{
				DebugPrintf("����:��ȡ��·������");
				return false;
			}
			// ��ֵ��Ϣ
			CopyMemory( &TBezierPoint[nIndex], pBezierPoint, sizeof(tagBezierPoint) );
		}
	}

	return CreateFishEx( cbCount, cbFishType, TBezierPoint, nBezierCount, unCreateTime, unIntervalTime, fRotateAngle, bCanSpecial, bCanAquatic, nKillerType, wHitChair, PointOffSet, fInitialAngle, bRepeatCreate );
}

// ������
uint CTableFrameSink::ResetFish( tagFishInfo & TFishInfo )
{
	// ��ǰʱ��
	int nIndex = 0;
	const uint nCurrentTime = timeGetTime();
	ASSERT( nCurrentTime >= m_nStartTime );

	// ������Ϣ
	tagFishInfo TFishCreateInfo;
	TFishCreateInfo.nFishKey = max( 1, m_nAppearFishCount + 1);
	TFishCreateInfo.nFishType = TFishInfo.nFishType;
	TFishCreateInfo.unCreateTime = nCurrentTime - m_nStartTime;
	TFishCreateInfo.unOverTime = TFishInfo.unOverTime;
	TFishCreateInfo.nScoreChance = TFishInfo.nFishScore;
	TFishCreateInfo.dCurChance	= TFishInfo.dCurChance;
	TFishCreateInfo.fRotateAngle = TFishInfo.fRotateAngle;
	TFishCreateInfo.PointOffSet = TFishInfo.PointOffSet;
	TFishCreateInfo.nFishState = TFishInfo.nFishState;
	TFishCreateInfo.wHitChair = TFishInfo.wHitChair;
	TFishCreateInfo.nFishScore = TFishInfo.nFishScore;
	TFishCreateInfo.nBezierCount = TFishInfo.nBezierCount;
	TFishCreateInfo.fInitialAngle = TFishInfo.fInitialAngle;
	//TFishCreateInfo.PSFish.Set( MonsterVec[TFishInfo.nFishType], MONSTER_VEC );

	// �޸�·��
	for ( int nIndex = TFishInfo.nBezierCount - 1, nAntiIndex = 0; nIndex >= 0; --nIndex, ++nAntiIndex )
	{
		// �޸�·����Ϣ
		TFishCreateInfo.TBezierPoint[nAntiIndex].BeginPoint = TFishInfo.TBezierPoint[nIndex].EndPoint;
		TFishCreateInfo.TBezierPoint[nAntiIndex].EndPoint = TFishInfo.TBezierPoint[nIndex].BeginPoint;
		TFishCreateInfo.TBezierPoint[nAntiIndex].KeyOne = TFishInfo.TBezierPoint[nIndex].KeyTwo;
		TFishCreateInfo.TBezierPoint[nAntiIndex].KeyTwo = TFishInfo.TBezierPoint[nIndex].KeyOne;
		TFishCreateInfo.TBezierPoint[nAntiIndex].Time = TFishInfo.TBezierPoint[nIndex].Time;
	}

	// ������
	TFishInfo = TFishCreateInfo;

	// ��������
	m_nAppearFishCount++;

	// ��������Ϣ
	SendFish(TFishCreateInfo);

	return nCurrentTime - m_nStartTime + TFishInfo.unOverTime;
}


// ���·��
void CTableFrameSink::RandomPath( byte cbFishType, tagBezierPoint TBezierPoint[BEZIER_POINT_MAX], int & nBezierCount )
{
	// BOSS
	if (m_bIsFushBoss && cbFishType >= FishType_Moderate_Max)
	{
		// ����·��
		TBezierPoint[0].BeginPoint.x = -200;
		TBezierPoint[0].BeginPoint.y = RAND_EQUAL_MIN_MAX(100, 700);
		TBezierPoint[0].EndPoint.x = 1480;
		TBezierPoint[0].EndPoint.y = RAND_EQUAL_MIN_MAX(100, 700);
		TBezierPoint[0].KeyOne.x = RAND_EQUAL_MIN_MAX(100, 1280);
		TBezierPoint[0].KeyOne.y = RAND_EQUAL_MIN_MAX(100, 800);
		TBezierPoint[0].KeyTwo.x = RAND_EQUAL_MIN_MAX(100, 1280);
		TBezierPoint[0].KeyTwo.y = RAND_EQUAL_MIN_MAX(100, 800);
		TBezierPoint[0].Time = 30000;

		// ·������
		nBezierCount = 1;
	}
	else if( cbFishType >= FishType_Moderate_Max )
	{
		// ����·��
		TBezierPoint[0].BeginPoint.x = -200;
		TBezierPoint[0].BeginPoint.y = RAND_EQUAL_MIN_MAX( 100, 700 );
		TBezierPoint[0].EndPoint.x = 1480;
		TBezierPoint[0].EndPoint.y = RAND_EQUAL_MIN_MAX( 100, 700 );
		TBezierPoint[0].KeyOne.x = RAND_EQUAL_MIN_MAX( 0, 1280 );
		TBezierPoint[0].KeyOne.y = RAND_EQUAL_MIN_MAX( 0, 800 );
		TBezierPoint[0].KeyTwo.x = RAND_EQUAL_MIN_MAX( 0, 1280 );
		TBezierPoint[0].KeyTwo.y = RAND_EQUAL_MIN_MAX( 0, 800 );
		TBezierPoint[0].Time = 30000;

		// ·������
		nBezierCount = 1;
	}
	// ����
	else if( cbFishType >= FishType_Small_Max )
	{
		// ����·��
		if ( RAND_TRUE_FALSE(50, 50) )
		{
			TBezierPoint[0].BeginPoint.x = -200;
			TBezierPoint[0].BeginPoint.y = RAND_EQUAL_MIN_MAX( 100, 700 );
			TBezierPoint[0].EndPoint.x = 1480;
			TBezierPoint[0].EndPoint.y = RAND_EQUAL_MIN_MAX( 100, 700 );
		}
		else
		{
			TBezierPoint[0].BeginPoint.x = RAND_EQUAL_MIN_MAX( 100, 1180 );
			TBezierPoint[0].BeginPoint.y = -200;
			TBezierPoint[0].EndPoint.x = RAND_EQUAL_MIN_MAX( 100, 1180 );
			TBezierPoint[0].EndPoint.y = 1000;
		}

		TBezierPoint[0].KeyOne.x = RAND_EQUAL_MIN_MAX( 200, 1080 );
		TBezierPoint[0].KeyOne.y = RAND_EQUAL_MIN_MAX( 200, 600 );
		TBezierPoint[0].KeyTwo.x = RAND_EQUAL_MIN_MAX( 200, 1080 );
		TBezierPoint[0].KeyTwo.y = RAND_EQUAL_MIN_MAX( 200, 600 );
		TBezierPoint[0].Time = RAND_EQUAL_MIN_MAX( 15000, 23000 );

		// ·������
		nBezierCount = 1;
	}
	// С��
	else
	{

		// ��һ��ģʽ
		if ( RAND_TRUE_FALSE(20, 80) )
		{
			// ����·��
			if ( RAND_TRUE_FALSE(50, 50) )
			{
				TBezierPoint[0].BeginPoint.x = -200;
				TBezierPoint[0].BeginPoint.y = RAND_EQUAL_MIN_MAX( 0, 800 );
				TBezierPoint[0].EndPoint.x = 1480;
				TBezierPoint[0].EndPoint.y = RAND_EQUAL_MIN_MAX( 0, 800 );
			}
			else
			{
				TBezierPoint[0].BeginPoint.x = RAND_EQUAL_MIN_MAX( 0, 1280 );
				TBezierPoint[0].BeginPoint.y = -200;
				TBezierPoint[0].EndPoint.x = RAND_EQUAL_MIN_MAX( 0, 1280 );
				TBezierPoint[0].EndPoint.y = 1000;
			}

			TBezierPoint[0].KeyOne.x = RAND_EQUAL_MIN_MAX( 200, 1080 );
			TBezierPoint[0].KeyOne.y = RAND_EQUAL_MIN_MAX( 200, 600 );
			TBezierPoint[0].KeyTwo.x = RAND_EQUAL_MIN_MAX( 200, 1080 );
			TBezierPoint[0].KeyTwo.y = RAND_EQUAL_MIN_MAX( 200, 600 );
			TBezierPoint[0].Time = RAND_EQUAL_MIN_MAX( 10000, 15000 );

			// ·������
			nBezierCount = 1;
		}
		// �ڶ���ģʽ
		else
		{
			// ����·��
			if ( RAND_TRUE_FALSE(50, 50) )
			{
				TBezierPoint[0].BeginPoint.x = -200;
				TBezierPoint[0].BeginPoint.y = RAND_EQUAL_MIN_MAX( 0, 800 );
				TBezierPoint[1].EndPoint.x = 1480;
				TBezierPoint[1].EndPoint.y = RAND_EQUAL_MIN_MAX( 0, 800 );
			}
			else
			{
				TBezierPoint[0].BeginPoint.x = RAND_EQUAL_MIN_MAX( 0, 1280 );
				TBezierPoint[0].BeginPoint.y = -200;
				TBezierPoint[1].EndPoint.x = RAND_EQUAL_MIN_MAX( 0, 1280 );
				TBezierPoint[1].EndPoint.y = 1000;
			}

			TBezierPoint[0].EndPoint.x = RAND_EQUAL_MIN_MAX( 200, 1208 );
			TBezierPoint[0].EndPoint.y = RAND_EQUAL_MIN_MAX( 100, 800 );
			TBezierPoint[1].BeginPoint.x = TBezierPoint[0].EndPoint.x;
			TBezierPoint[1].BeginPoint.y = TBezierPoint[0].EndPoint.y;

			TBezierPoint[0].KeyOne.x = RAND_EQUAL_MIN_MAX( 200, 1080 );
			TBezierPoint[0].KeyOne.y = RAND_EQUAL_MIN_MAX( 200, 600 );
			TBezierPoint[0].KeyTwo.x = RAND_EQUAL_MIN_MAX( 200, 1080 );
			TBezierPoint[0].KeyTwo.y = RAND_EQUAL_MIN_MAX( 200, 600 );
			TBezierPoint[0].Time = RAND_EQUAL_MIN_MAX( 7000, 9000 );

			TBezierPoint[1].KeyOne.x = RAND_EQUAL_MIN_MAX( 0, 1280 );
			TBezierPoint[1].KeyOne.y = RAND_EQUAL_MIN_MAX( 0, 800 );
			TBezierPoint[1].KeyTwo.x = RAND_EQUAL_MIN_MAX( 0, 1280 );
			TBezierPoint[1].KeyTwo.y = RAND_EQUAL_MIN_MAX( 0, 800 );
			TBezierPoint[1].Time = RAND_EQUAL_MIN_MAX( 7000, 15000 );

			// ·������
			nBezierCount = 2;
		}
	}

	// �����ת·��
	if( RAND_TRUE_FALSE(50, 50) )
	{
		// ����·��
		tagBezierPoint TBezierPointAnti[BEZIER_POINT_MAX];
		for ( int nIndex = nBezierCount - 1, nAntiIndex = 0; nIndex >= 0; --nIndex, ++nAntiIndex )
		{
			// �޸�·����Ϣ
			TBezierPointAnti[nAntiIndex].BeginPoint = TBezierPoint[nIndex].EndPoint;
			TBezierPointAnti[nAntiIndex].EndPoint = TBezierPoint[nIndex].BeginPoint;
			TBezierPointAnti[nAntiIndex].KeyOne = TBezierPoint[nIndex].KeyTwo;
			TBezierPointAnti[nAntiIndex].KeyTwo = TBezierPoint[nIndex].KeyOne;
			TBezierPointAnti[nAntiIndex].Time = TBezierPoint[nIndex].Time;
		}
		CopyMemory(TBezierPoint, TBezierPointAnti, sizeof(tagBezierPoint) * nBezierCount);
	}
}

// ��������Ϣ
void CTableFrameSink::SendFish( tagFishInfo & TFishInfo )
{
	// ������Ϣ
	CMD_S_FishCreate FishCreate;
	FishCreate.nFishKey = TFishInfo.nFishKey;
	FishCreate.unCreateTime = TFishInfo.unCreateTime;
	FishCreate.wHitChair = TFishInfo.wHitChair;
	FishCreate.nFishType = TFishInfo.nFishType;
	FishCreate.nFishState = TFishInfo.nFishState;
	FishCreate.fRotateAngle = TFishInfo.fRotateAngle;
	FishCreate.PointOffSet = TFishInfo.PointOffSet;
	FishCreate.nBezierCount = TFishInfo.nBezierCount;
	FishCreate.fInitialAngle = TFishInfo.fInitialAngle;
	CopyMemory( &FishCreate.TBezierPoint, &TFishInfo.TBezierPoint, sizeof(FishCreate.TBezierPoint) );
	//printf("������ʱ��:%d", FishCreate.unCreateTime);
	// ������Ϣ
	SendTableData(INVALID_CHAIR, SUB_S_CREATE_FISHS, &FishCreate, sizeof(CMD_S_FishCreate) );

	// ����ͣ����Ϣ
	for( int nIndex = 0; nIndex < TFishInfo.ArrayStayInfo.GetCount(); ++nIndex )
	{
		// ��ȡͣ����Ϣ
		tagStayInfo & TStayInfo = TFishInfo.ArrayStayInfo.GetAt( nIndex );

		// ������Ϣ
		CMD_S_StayFish CMDSStayFish;
		CMDSStayFish.nFishKey = TFishInfo.nFishKey;
		CMDSStayFish.nStayStart = TStayInfo.nStayStart;
		CMDSStayFish.nStayTime = TStayInfo.nStayTime;

		// ������Ϣ
		SendTableData(INVALID_CHAIR, SUB_S_STAY_FISH, &CMDSStayFish, sizeof(CMD_S_StayFish));
	}
}

// ��������
uint CTableFrameSink::SpecialFishMatrix( byte cbFishType, CDoublePoint PointFish )
{
	// ��λ��
	int nPathIndex = RAND_TRUE_FALSE(50, 50) ? 6 : 7;

	// ƫ��λ��
	CShortPoint PointOffSet;
	tagBezierPoint * pBezierPoint = m_ArrayFishPathPositive.GetAt(nPathIndex)->ArrayBezierPoint.GetAt(0);
	if (!pBezierPoint)
	{
		DebugPrintf("����:���������ȡ����");
		return false;
	}
	PointOffSet.x = (short)(PointFish.x - pBezierPoint->BeginPoint.x);
	PointOffSet.y = (short)(PointFish.y - pBezierPoint->BeginPoint.y);

	// ɱ����
	bool bCreateKiller = false;

	// 36��
	int nCountMax = 36;
	if (m_iFishShoalLev > 0 && m_iFishShoalLev <50)
	{
		nCountMax = m_iFishShoalLev;
	}
	for( int nCount = 0; nCount < nCountMax; ++nCount )
	{
		// ����ɱ��
		bool bKiller = !bCreateKiller && (RandomArea( 2, 2, nCountMax - nCount - 1 ) == 0);
		if ( bKiller ) bCreateKiller = true;

		// ����3Ȧ��
		CreateFish( 3, cbFishType, nPathIndex, 0, 1000, GL_PI * 2.f / nCountMax * nCount, false, false, bKiller ? KillerType_One : KillerType_No, INVALID_CHAIR, PointOffSet, 0.f, false );
	}

	return 0;
}
// ��Ⱥ����
uint CTableFrameSink::GroupOfFish( uint nBeginTime /* = 0 */ )
{
	BYTE Grop = rand() % 5;
	if (m_iFinshGroup >=0 && m_iFinshGroup<5)
	{
		Grop = m_iFinshGroup;
	}
	//DebugPrintf("�����������ͣ�%d,����ȼ�:%d", Grop, m_iFinshLeve);
	// �ж�����
	switch(Grop)
	{
	case 0:
		{
			// ������
			int nFishTime[10] = {0, 3000, 6000, 9000, 12000, 15500, 18000, 22000, 27500, 32000 };
			for ( int nFishIndex = FishType_BDengLongYu; nFishIndex <= FishType_BMoGuiYu; ++nFishIndex )
			{
				CreateFish( 1, nFishIndex, 8, nFishTime[nFishIndex - FishType_BDengLongYu], 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(0, 50), 0.f, false );
				CreateFish( 1, nFishIndex, 9, nFishTime[nFishIndex - FishType_BDengLongYu], 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(0, -50), 0.f, false );
			}
			int nFishCount = 32;
			// ������
			if (m_iFinshLeve == 3)
			{
				nFishCount = 32;
			}
			else if (m_iFinshLeve == 2)
			{
				nFishCount = 20;
			}
			else if (m_iFinshLeve == 1)
			{
				nFishCount = 12;
			}

			for( int nIndex = 1; nIndex < nFishCount; ++nIndex )
			{
				CreateFish( 1, FishType_BHaiLuo, 16, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(DEFAULE_WIDTH * nIndex / nFishCount, -40), 0.f, false );
				CreateFish( 1, FishType_BHaiLuo, 17, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(DEFAULE_WIDTH * nIndex / nFishCount, 40), 0.f, false );
			}

			nFishCount = 16;
			if (m_iFinshLeve == 3)
			{
				nFishCount = 16;
			}
			else if (m_iFinshLeve == 2)
			{
				nFishCount = 12;
			}
			else if (m_iFinshLeve == 1)
			{
				nFishCount = 7;
			}
			for( int nIndex = 1; nIndex < nFishCount; ++nIndex )
			{
				CreateFish( 1, FishType_BHongWeiYu, 16, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(DEFAULE_WIDTH * nIndex / nFishCount, 70), 0.f, false );
				CreateFish( 1, FishType_BHongWeiYu, 17, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(DEFAULE_WIDTH * nIndex / nFishCount, -70), 0.f, false );
			}
		}
		return 52000;
	case 1:
		{

			// ������
			CreateFish( 1, FishType_BMoGuiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(640, 400), 0.f, false );
			CreateFish( 1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(440, 400), 0.f, false );
			CreateFish( 1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(840, 400), GL_PI, false );

			CreateFish( 1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(415, 199), 0.f, false );

			CreateFish( 1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(537, 199), 0.f, false );

			CreateFish( 1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(661, 199), 0.f, false );

			CreateFish( 1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(784, 199), 0.f, false );

			CreateFish( 1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(866, 199), 0.f, false );


			CreateFish( 1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(415, 600), GL_PI, false );

			CreateFish( 1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(537, 600), GL_PI, false );

			CreateFish( 1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(661, 600), GL_PI, false );

			CreateFish( 1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(784, 600), GL_PI, false );

			CreateFish( 1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(866, 600), GL_PI, false );
			CreateFish( 1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(364, 206), AngleToRadian(-15), false );

			CreateFish( 1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(242, 298), AngleToRadian(-60), false );

			CreateFish( 1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(222, 451), AngleToRadian(-105), false );

			CreateFish( 1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(317, 573), AngleToRadian(-150), false );



			CreateFish( 1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(918, 206), AngleToRadian(15), false );

			CreateFish( 1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1039, 298), AngleToRadian(60), false );

			CreateFish( 1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1059, 451), AngleToRadian(105), false );

			CreateFish( 1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(966, 573), AngleToRadian(150), false );


			//�±�һ�ŵ���
			//CreateFish( 1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(413, 125), 0.f, false );

			CreateFish( 1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(518, 125), 0.f, false );	

			CreateFish( 1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(641, 125), 0.f, false );	

			CreateFish( 1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(770, 125), 0.f, false );	

			//CreateFish( 1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(869, 125), 0.f, false );	

			//�ϱ�һ��
		//	CreateFish( 1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(413, 672), GL_PI, false );

			CreateFish( 1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(490, 672), GL_PI, false );	
	
			CreateFish( 1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(641, 672), GL_PI, false );	
	
			CreateFish( 1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(800, 672), GL_PI, false );	
	
		//	CreateFish( 1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(869, 672), GL_PI, false );	
			//����
			CreateFish( 1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(365, 129), AngleToRadian(-10), false );

			CreateFish( 1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(237, 190), AngleToRadian(-40), false );

			CreateFish( 1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(156, 305), AngleToRadian(-70), false );

			CreateFish( 1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(144, 446), AngleToRadian(-100), false );

			CreateFish( 1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(203, 575), AngleToRadian(-130), false );

			CreateFish( 1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(319, 656), AngleToRadian(-160), false );

			CreateFish( 1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(916, 129), AngleToRadian(10), false );

			CreateFish( 1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1044, 190), AngleToRadian(40), false );

			CreateFish( 1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1125, 305), AngleToRadian(70), false );

			CreateFish( 1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1137, 446), AngleToRadian(100), false );

			CreateFish( 1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1078, 575), AngleToRadian(130), false );

			CreateFish( 1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(962, 656), AngleToRadian(160), false );

		



			if (m_iFinshLeve == 3)
			{
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(456, 199), 0.f, false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(497, 199), 0.f, false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(579, 199), 0.f, false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(620, 199), 0.f, false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(702, 199), 0.f, false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(743, 199), 0.f, false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(825, 199), 0.f, false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(456, 600), GL_PI, false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(497, 600), GL_PI, false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(579, 600), GL_PI, false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(620, 600), GL_PI, false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(702, 600), GL_PI, false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(743, 600), GL_PI, false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(825, 600), GL_PI, false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(317, 226), AngleToRadian(-30), false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(274, 257), AngleToRadian(-45), false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(222, 346), AngleToRadian(-75), false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(215, 399), AngleToRadian(-90), false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(242, 500), AngleToRadian(-120), false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(274, 540), AngleToRadian(-135), false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(364, 593), AngleToRadian(-165), false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(966, 226), AngleToRadian(30), false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1008, 257), AngleToRadian(45), false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1059, 346), AngleToRadian(75), false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1067, 399), AngleToRadian(90), false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1039, 500), AngleToRadian(120), false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1008, 540), AngleToRadian(135), false);
				CreateFish(1, FishType_BCaiBanYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(918, 593), AngleToRadian(165), false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(451, 125), 0.f, false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(489, 125), 0.f, false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(565, 125), 0.f, false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(603, 125), 0.f, false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(679, 125), 0.f, false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(717, 125), 0.f, false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(793, 125), 0.f, false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(831, 125), 0.f, false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(451, 672), GL_PI, false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(489, 672), GL_PI, false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(565, 672), GL_PI, false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(603, 672), GL_PI, false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(679, 672), GL_PI, false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(717, 672), GL_PI, false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(793, 672), GL_PI, false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(831, 672), GL_PI, false);

				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(319, 142), AngleToRadian(-20), false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(276, 162), AngleToRadian(-30), false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(204, 223), AngleToRadian(-50), false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(176, 262), AngleToRadian(-60), false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(144, 352), AngleToRadian(-80), false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(139, 399), AngleToRadian(-90), false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(156, 492), AngleToRadian(-110), false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(176, 535), AngleToRadian(-120), false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(237, 608), AngleToRadian(-140), false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(276, 636), AngleToRadian(-150), false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(365, 668), AngleToRadian(-170), false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(962, 142), AngleToRadian(20), false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1005, 162), AngleToRadian(30), false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1078, 223), AngleToRadian(50), false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1105, 262), AngleToRadian(60), false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1137, 352), AngleToRadian(80), false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1141, 399), AngleToRadian(90), false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1125, 492), AngleToRadian(110), false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1105, 535), AngleToRadian(120), false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1044, 608), AngleToRadian(140), false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1005, 636), AngleToRadian(150), false);
				CreateFish(1, FishType_BBXiaoChouYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(916, 668), AngleToRadian(170), false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(487, 36), 0.f, false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(548, 36), 0.f, false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(670, 36), 0.f, false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(731, 36), 0.f, false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(853, 36), 0.f, false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(487, 760), GL_PI, false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(548, 760), GL_PI, false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(670, 760), GL_PI, false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(731, 760), GL_PI, false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(853, 760), GL_PI, false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(303, 58), AngleToRadian(-20), false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(245, 85), AngleToRadian(-30), false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(149, 166), AngleToRadian(-50), false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(113, 217), AngleToRadian(-60), false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(70, 335), AngleToRadian(-80), false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(64, 399), AngleToRadian(-90), false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(86, 523), AngleToRadian(-110), false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(113, 580), AngleToRadian(-120), false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(194, 676), AngleToRadian(-140), false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(245, 712), AngleToRadian(-150), false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(364, 755), AngleToRadian(-170), false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(977, 58), AngleToRadian(20), false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1034, 85), AngleToRadian(30), false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1131, 166), AngleToRadian(50), false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1167, 217), AngleToRadian(60), false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1210, 335), AngleToRadian(80), false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1216, 399), AngleToRadian(90), false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1194, 523), AngleToRadian(110), false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1167, 580), AngleToRadian(120), false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1086, 676), AngleToRadian(140), false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1034, 712), AngleToRadian(150), false);
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(916, 755), AngleToRadian(170), false);



				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(426, 36), 0.f, false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(609, 36), 0.f, false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(792, 36), 0.f, false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(426, 760), GL_PI, false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(609, 760), GL_PI, false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(792, 760), GL_PI, false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(364, 42), AngleToRadian(-10), false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(194, 121), AngleToRadian(-40), false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(86, 275), AngleToRadian(-70), false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(70, 462), AngleToRadian(-100), false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(149, 631), AngleToRadian(-130), false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(303, 739), AngleToRadian(-160), false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(916, 42), AngleToRadian(10), false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1086, 121), AngleToRadian(40), false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1194, 275), AngleToRadian(70), false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1210, 462), AngleToRadian(100), false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1131, 631), AngleToRadian(130), false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(977, 739), AngleToRadian(160), false);
			}
			else if (m_iFinshLeve == 2)
			{
				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(426, 36), 0.f, false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(609, 36), 0.f, false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(792, 36), 0.f, false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(426, 760), GL_PI, false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(609, 760), GL_PI, false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(792, 760), GL_PI, false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(364, 42), AngleToRadian(-10), false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(194, 121), AngleToRadian(-40), false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(86, 275), AngleToRadian(-70), false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(70, 462), AngleToRadian(-100), false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(149, 631), AngleToRadian(-130), false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(303, 739), AngleToRadian(-160), false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(916, 42), AngleToRadian(10), false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1086, 121), AngleToRadian(40), false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1194, 275), AngleToRadian(70), false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1210, 462), AngleToRadian(100), false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(1131, 631), AngleToRadian(130), false);

				CreateFish(1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(977, 739), AngleToRadian(160), false);
			}
			else if (m_iFinshLeve == 1)
			{
				
			}
		}
		return 45000;
	case 2:
		{
			// ����С��
			byte cbFishSmallIndex[3] = { FishType_BXiaoHuangYu, FishType_BHaiMa, FishType_BCaiBanYu };
			uint unFishCount[3] = { 72, 48, 32 };
			uint unFishSmallCreateTime[3] = { 0, 1000, 2000 };
			uint fFishSmallRadius[3] = { 300, 240, 170 };
			uint fFishSmallIntervalTime[3] = { 9000, 7000, 5000 };

			if (m_iFinshLeve == 2)
			{
				unFishCount[0] = 35;
				unFishCount[1] = 24;
				unFishCount[2] = 16;
			}
			else if (m_iFinshLeve == 1)
			{
				unFishCount[0] = 20;
				unFishCount[1] = 18;
				unFishCount[2] = 12;
			}
			// ������
			for( int nIndex = 0; nIndex < 3; ++nIndex )
			{
				CircleOfFish( cbFishSmallIndex[nIndex], unFishCount[nIndex], 8, unFishSmallCreateTime[nIndex], fFishSmallRadius[nIndex], fFishSmallIntervalTime[nIndex] );
				CircleOfFish( cbFishSmallIndex[nIndex], unFishCount[nIndex], 9, unFishSmallCreateTime[nIndex], fFishSmallRadius[nIndex], fFishSmallIntervalTime[nIndex] );
			}

			// ��������
			byte cbFishBigIndex = ( rand() % 6 ) + FishType_BNianYu;
			CreateFish( 1, cbFishBigIndex, 8, 4500, 0, 0.f, false, false );
			CreateFish( 1, cbFishBigIndex, 9, 4500, 0, 0.f, false, false );
		}
		return 17000 + 6000;
	case 3:
		{
			// ����BOSS
			CreateFish( 1, ( rand() % 4 ) + FishType_BMoGuiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(DEFAULE_WIDTH / 2, DEFAULE_HEIGHT / 2), 0.f, false );

			// ��λ��
			int nPathIndex6 = 6;
			int nPathIndex7 = 7;

			// ƫ��λ��
			CShortPoint PointOffSet6;
			CShortPoint PointOffSet7;
			tagBezierPoint * pBezierPoint6 = m_ArrayFishPathPositive.GetAt(nPathIndex6)->ArrayBezierPoint.GetAt(0);
			tagBezierPoint * pBezierPoint7 = m_ArrayFishPathPositive.GetAt(nPathIndex7)->ArrayBezierPoint.GetAt(0);
			PointOffSet6.x = (short)(DEFAULE_WIDTH / 2 - pBezierPoint6->BeginPoint.x);
			PointOffSet6.y = (short)(DEFAULE_HEIGHT / 2 - pBezierPoint6->BeginPoint.y);
			PointOffSet7.x = (short)(DEFAULE_WIDTH / 2 - pBezierPoint7->BeginPoint.x);
			PointOffSet7.y = (short)(DEFAULE_HEIGHT / 2 - pBezierPoint7->BeginPoint.y);

			// 36��
			int nCountMax = 36;
			if (m_iFinshLeve == 2)
			{
				nCountMax = 20;
			}
			else if (m_iFinshLeve == 1)
			{
				nCountMax = 12;
			}
			for( int nCount = 0; nCount < nCountMax; ++nCount )
			{
				// ����5Ȧ��
				CreateFish( 1, FishType_BBXiaoChouYu, nPathIndex7, 0, 0, GL_PI * 2.f / nCountMax * nCount, false, false, KillerType_No, INVALID_CHAIR, PointOffSet7, 0.f, false );
				CreateFish( 1, FishType_BHaiLuo, nPathIndex6, 3000, 0, GL_PI * 2.f / nCountMax * nCount, false, false, KillerType_No, INVALID_CHAIR, PointOffSet6, 0.f, false );
				CreateFish( 1, FishType_BHongWeiYu, nPathIndex7, 6000, 0, GL_PI * 2.f / nCountMax * nCount, false, false, KillerType_No, INVALID_CHAIR, PointOffSet7, 0.f, false );
				CreateFish( 1, FishType_BHaiLuo, nPathIndex6, 9000, 0, GL_PI * 2.f / nCountMax * nCount, false, false, KillerType_No, INVALID_CHAIR, PointOffSet6, 0.f, false );
				CreateFish( 1, FishType_BHongWeiYu, nPathIndex7, 12000, 0, GL_PI * 2.f / nCountMax * nCount, false, false, KillerType_No, INVALID_CHAIR, PointOffSet7, 0.f, false );
			}

		}
		return 45000;
	case 4:
		{
			// ����BOSS
			byte cbFishBigIndex = ( rand() % 4 ) + FishType_BMoGuiYu;
			CreateFish( 1, cbFishBigIndex, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(318, 400), 0.f, false );
			CreateFish( 1, cbFishBigIndex, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, CShortPoint(957, 400), 0.f, false );

			// ������
			int nCountMax = 18;
			if (m_iFinshLeve == 2)
			{
				nCountMax = 14;
			}
			else if (m_iFinshLeve == 1)
			{
				nCountMax = 10;
			}
			for( int nIndex = 0; nIndex < nCountMax; ++nIndex )
			{
				float fAngle = GL_PI * 2.f / (float)nCountMax * (float)nIndex;
				CreateFish( 1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, Rotate( CShortPoint(318, 400), fAngle, CShortPoint(318, 400 + 140) ), fAngle + GL_PI, false );
				CreateFish( 1, FishType_BHongWeiYu, 15, 0, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, Rotate( CShortPoint(957, 400), fAngle, CShortPoint(957, 400 + 140) ), fAngle + GL_PI, false );
			}

			// ������
			nCountMax = 36;
			if (m_iFinshLeve == 2)
			{
				nCountMax = 22;
			}
			else if (m_iFinshLeve == 1)
			{
				nCountMax = 11;
			}
			for( int nIndex = 0; nIndex < nCountMax; ++nIndex )
			{
				float fAngle = GL_PI * 2.f / (float)nCountMax * (float)nIndex;
				CreateFish( 1, FishType_BCaiBanYu, 15, 500, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, Rotate( CShortPoint(318, 400), fAngle, CShortPoint(318, 400 + 225) ), fAngle + GL_PI, false );
				CreateFish( 1, FishType_BCaiBanYu, 15, 500, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, Rotate( CShortPoint(957, 400), fAngle, CShortPoint(957, 400 + 225) ), fAngle + GL_PI, false );
			}

			// ������
			nCountMax = 72;
			if (m_iFinshLeve == 2)
			{
				nCountMax = 32;
			}
			else if (m_iFinshLeve == 1)
			{
				nCountMax = 20;
			}
			for( int nIndex = 0; nIndex < nCountMax; ++nIndex )
			{
				float fAngle = GL_PI * 2.f / (float)nCountMax * (float)nIndex;
				CreateFish( 1, FishType_BXiaoHuangYu, 15, 1000, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, Rotate( CShortPoint(318, 400), fAngle, CShortPoint(318, 400 + 295) ), fAngle + GL_PI, false );
				CreateFish( 1, FishType_BXiaoHuangYu, 15, 1000, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, Rotate( CShortPoint(957, 400), fAngle, CShortPoint(957, 400 + 295) ), fAngle + GL_PI, false );
			}
		}
		return 45000;
	}

	return 0;
}


// ԲȦ��
uint CTableFrameSink::CircleOfFish( byte cbFishType, int nFishCount, int nPathIndex, uint unCreateTime, uint nRadius, uint unIntervalTime )
{
	// ������
	for( int nIndex = 0; nIndex < nFishCount; ++nIndex )
	{
		float fRadius = (float)nRadius;
		CShortPoint PointOffset(0, (short)(fRadius * sin( GL_PI * 2.f / (float)nFishCount * (float)nIndex )));
		uint unCreateOffsetTime = (uint)(( fRadius - (fRadius * cos( GL_PI * 2.f / (float)nFishCount * (float)nIndex )) ) / (fRadius * 2.f) * unIntervalTime);
		CreateFish( 1, cbFishType, nPathIndex, unCreateTime + unCreateOffsetTime, 0, 0.f, false, false, KillerType_No, INVALID_CHAIR, PointOffset );
	}

	return nFishCount;
}


// ��ȡ·��ʱ��
uint CTableFrameSink::PathTime( tagFishPath * pFishPath )
{
	uint unTime = 0;
	for( int i = 0; i < pFishPath->ArrayBezierPoint.GetCount(); ++i )
	{
		unTime += pFishPath->ArrayBezierPoint[i]->Time;
	}
	return unTime;
}

// ���㵱ǰ��λ��
EnumFishMoveType CTableFrameSink::FishMove( CDoublePoint & ptPos, tagFishInfo & TFish, uint nCustomLossTime /*= uint_max*/ )
{
	// �жϵ�ǰʱ��
	uint nCurrentTime = timeGetTime();

	// ����ʱ��
	uint nLossTime = nCurrentTime - m_nStartTime;

	// ����ʱ��
	if ( nCustomLossTime != uint_max )
	{
		nLossTime = nCustomLossTime;
	}

	// δ������ʱ��
	if ( nLossTime < TFish.unCreateTime )
	{
		return FishMoveType_Next;
	}

	// ��ǰͣ��ʱ��
	uint nBeforeStayTime = 0;

	// ͣ��ʱ��
	for( int nStayIndex = 0; nStayIndex < TFish.ArrayStayInfo.GetCount(); ++nStayIndex )
	{
		// ��ȡ��Ϣ
		tagStayInfo & nStayInfo = TFish.ArrayStayInfo.GetAt(nStayIndex);

		// ��ȡʱ��
		uint nStayStart = nStayInfo.nStayStart;				
		uint nStayTime = nStayInfo.nStayTime;

		// �Ƚ���Ϣ
		if( nLossTime <= nStayStart )
		{
			break;
		}
		else if( nLossTime > nStayStart && nLossTime < nStayStart + nStayTime )
		{
			nLossTime = nStayStart;

			break;
		}
		else if ( nLossTime >= nStayStart + nStayTime )
		{
			nBeforeStayTime += nStayTime;
		}
	}

	// ��ȥʱ��
	nBeforeStayTime -= nBeforeStayTime;

	// �ζ�ʱ��
	uint unSwimTime = nLossTime - TFish.unCreateTime;
	uint unSwimCurTime = unSwimTime;
	uint unAllTime = 0;
	int  nMoveIndex = 0;
	bool bSuccess = false;

	// ��ȡ�ζ�����
	for ( int nBezierIndex = 0; nBezierIndex < TFish.nBezierCount ; ++nBezierIndex )
	{
		// ��ȡ��Ϣ
		tagBezierPoint * pBezierPoint = &TFish.TBezierPoint[nBezierIndex];
		if (!pBezierPoint)
		{
			DebugPrintf("����:FishMove��ȡ����Ϣ����");
		}
		// ·����ʱ
		unAllTime += pBezierPoint->Time;

		// ���ҵ�ǰ·��
		if ( unSwimTime <= unAllTime )
		{
			nMoveIndex = nBezierIndex;
			bSuccess = true;
			break;
		}

		// ���㵱ǰʱ��
		unSwimCurTime -= pBezierPoint->Time;
	}

	// ·������
	if ( !bSuccess )
	{
		return FishMoveType_Delete;
	}

	// ���㱴����
	double dProportion = (double)(unSwimCurTime)/(double)(TFish.TBezierPoint[nMoveIndex].Time);
	CDoublePoint ptPosition(0.0, 0.0);
	ptPosition = PointOnCubicBezier( &TFish.TBezierPoint[nMoveIndex], dProportion );

	// ������ת
	ptPos = Rotate(TFish.TBezierPoint[0].BeginPoint, TFish.fRotateAngle, ptPosition );

	// ƫ��
	ptPos.x += TFish.PointOffSet.x;
	ptPos.y += TFish.PointOffSet.y;

	return FishMoveType_Normal;
}

// �������Ϣ
void CTableFrameSink::FishFastMove()
{
	// ȫ�ֱ�ը��ԭ
	if ( GetFishCount(FishType_BaoZhaFeiBiao) > 0 )
	{
		// �����
		int nMultipleMax = 0;
		for ( int nMultipleIndex = 0; nMultipleIndex < Multiple_Max; ++nMultipleIndex )
		{
			nMultipleMax = max( nMultipleMax, m_nMultipleValue[nMultipleIndex] );
		}

		// ��������
		m_dDartStock += nMultipleMax * m_nExplosionStart;
	}

	// �����
	m_nFishTeamTime = 0;
	m_ArrayFishInfo.RemoveAll();

	// ��ձ�����Ϣ
	for ( int nSite = 0; nSite < PlayChair_Max; ++nSite )
	{
		// ����������
		if( m_bPlaySupply[nSite] )
		{
			m_nEnergyValue[nSite] = 0;
			m_bPlaySupply[nSite] = false;
		}
	}

	// ����ʱ��
	m_nStartTime = timeGetTime();
}

// ��ȡ������
uint CTableFrameSink::GetFishCount( uint nType )
{
	// �ж�����
	if( nType >= FishType_Max )
		return 0;

	// ������
	uint nFishCount = 0;
	for ( int nIndex = 0; nIndex < m_ArrayFishInfo.GetCount(); ++nIndex )
	{
		if( m_ArrayFishInfo.GetAt(nIndex).nFishType == nType )
		{
			nFishCount++;
		}
	}

	return nFishCount;
}

// ��ȡ����������
uint CTableFrameSink::GetFishSpecialCount()
{
	// ������
	uint nFishCount = 0;
	for ( int nIndex = 0; nIndex < m_ArrayFishInfo.GetCount(); ++nIndex )
	{
		if( m_ArrayFishInfo.GetAt(nIndex).nFishState == FishState_King )
		{
			nFishCount++;
		}
	}

	return nFishCount;
}

// �����
void CTableFrameSink::FishFreeze( uint unLossTime )
{
	// ����ͣ��
	uint nStayStart = unLossTime;
	uint nStayTime = 5000;

	// ����Ϣ
	for( int nIndex = 0; nIndex < m_ArrayFishInfo.GetCount(); ++nIndex )
	{
		// ��ȡ����Ϣ
		tagFishInfo & TFish = m_ArrayFishInfo.GetAt(nIndex);

		// δ��ʱ��
		if ( nStayStart < TFish.unCreateTime )
		{
			continue;
		}

		// ��ȡ������
		CArrayStay & ArrayStayInfo = TFish.ArrayStayInfo;

		// �Ա����һ��
		bool bUpLast = false;
		if ( ArrayStayInfo.GetCount() > 0 )
		{
			// ��ȡ���һ��
			tagStayInfo & nStaylast = ArrayStayInfo.GetAt( ArrayStayInfo.GetCount() - 1 );

			// ��ͬʱ��
			if ( nStaylast.nStayStart + nStaylast.nStayTime >= nStayStart )
			{
				// ����ʱ��
				nStaylast.nStayTime = nStayStart - nStaylast.nStayStart + 5000;

				// ������Ϣ
				bUpLast = true;
				nStayStart = nStaylast.nStayStart;
				nStayTime = nStaylast.nStayTime;
			}
		}
	
		// �����Ϣ
		if( !bUpLast )
		{
			// ������Ϣ
			tagStayInfo nStayInfo;
			nStayInfo.nStayStart = nStayStart;
			nStayInfo.nStayTime = nStayTime;

			// �����Ϣ
			ArrayStayInfo.Add(nStayInfo);
		}

		// ������Ϣ
		CMD_S_StayFish CMDSStayFish;
		CMDSStayFish.nFishKey = TFish.nFishKey;
		CMDSStayFish.nStayStart = nStayStart;
		CMDSStayFish.nStayTime = nStayTime;

		// ������Ϣ
		SendTableData(INVALID_CHAIR, SUB_S_STAY_FISH, &CMDSStayFish, sizeof(CMD_S_StayFish));
	}
}

// ������Ϣ
bool CTableFrameSink::SendTableData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize, unsigned int handleCode/* = 0*/)
{
	// �ж�����
	if (wChairID == INVALID_CHAIR)
	{
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			if (GetUserIDByDeskStation(i) <= 0)
			{
				continue;
			}
			SendGameData(i, pData, wDataSize, MSG_MAIN_LOADER_GAME, wSubCmdID, handleCode);
		}
		return true;
	}
	return SendGameData(wChairID, pData, wDataSize, MSG_MAIN_LOADER_GAME, wSubCmdID, handleCode);
}

// ���ͽ�����ʾ
bool CTableFrameSink::SendAwardTip( WORD wChairID, byte nFishType, int nFishMultiple, LONGLONG lFishScore, EnumScoreType nScoreType )
{
	// ��Ч�����ŷ���
	if ( nFishMultiple <= 0 || lFishScore <= 0 )
		return false;

	// ��ȡ���
	//IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
	//if( pIServerUserItem == NULL )
	//	return false;
	CRedisLoader* pRedis = m_pDataManage->GetRedis();
	RoomBaseInfo roomBasekInfo;
	RoomBaseInfo* pRoomBaseInfo = NULL;
	if (pRedis->GetRoomBaseInfo(m_pDataManage->GetRoomID(), roomBasekInfo))
	{
		pRoomBaseInfo = &roomBasekInfo;
	}
	else
	{
		pRoomBaseInfo = ConfigManage()->GetRoomBaseInfo(m_pDataManage->GetRoomID());
	}
	if (!pRoomBaseInfo)
	{
		return false;
	}
	// �������䲻����
	if ( m_bCompetitionRoom )
		return true;

	// ����̫С
//	printf("������С:%d,%d\n", nFishMultiple, m_nAwardMinMultiple);
	if (nFishMultiple < m_nAwardMinMultiple)
		return true;

	// ������Ϣ
	//CMD_S_AwardTip CMDSAwardTip;
	//CMDSAwardTip.wTableID = /*pIServerUserItem->GetTableID()*/ 1;
	//CMDSAwardTip.wChairID = wChairID;
	//_sntprintf( CMDSAwardTip.szPlayName, CountArray(CMDSAwardTip.szPlayName), TEXT("%s"), pRoomBaseInfo->name);
	//CMDSAwardTip.nFishType = nFishType;
	//CMDSAwardTip.nFishMultiple = nFishMultiple;
	//CMDSAwardTip.lFishScore = lFishScore;
	//CMDSAwardTip.nScoreType = nScoreType;

	//// ������Ϣ
	//SendTableData(INVALID_CHAIR, SUB_S_AWARD_TIP, &CMDSAwardTip, sizeof(CMD_S_AwardTip) );

	// ����������Ϣ
	if( m_bAwardChatBox && nFishMultiple >= m_nAwardMinMultiple && lFishScore >= 0 )
	{
		// ����Ϣ
		tchar szFishName[FishType_Max][11] = {
			_T("С����"), _T("С����"), _T("�ʰ���"), _T("����"), _T("С����"),
			_T("��β��"), _T("����"), _T("����з"), _T("ɺ����"), _T("�����"),
			_T("��β��"), _T("������"), _T("������"), _T("��ɫ������"), _T("������"),
			_T("����"), _T("�ڹ�"), _T("ħ����"), _T("����"), _T("����"),
			_T("����"), _T("����"), _T("����"), _T("�ƽ�ͷ��"), _T("�ƽ�޳���"),
			_T("�ƽ������"), _T("�ƽ�ͷ��"), _T("�ƽ���"), _T("�ƽ𻢾�"), _T("ʷǰ����") };


		GameUserInfo data;
		
		if (!GetUserData(wChairID, data))
		{
			return false;
		}


		// ������Ϣ
		CString StrTable;

		// ������Ϣ
		CString StrAwardTip;
		//if (GetRoomLevel() <= 1)
		//{
		//	StrAwardTip.Format(_T("��ϲ���#FF0000 | ��%s��#00FAFA | �ڡ�BOOS���˳�������������#FF12FA | ��%s��#FFFAFA | ���#FFFA12 | ���#I1 | %d #C1��"),
		//		data.name, szFishName[nFishType], /*nFishMultiple,*/ lFishScore);
		//}
		//else if (GetRoomLevel() == 2)
		//{
		//	StrAwardTip.Format(_T("��ϲ���#FF0000 | ��%s��#00FAFA | �ڡ�BOOS�����м�����������#FF12FA | ��%s��#FFFAFA | ���#FFFA12 | ���#I1 | %d #C1��"),
		//		data.name, szFishName[nFishType], /*nFishMultiple,*/ lFishScore);
		//}
		//else if (GetRoomLevel() == 3)
		//{
		//	StrAwardTip.Format(_T("��ϲ���#FF0000 | ��%s��#00FAFA | �ڡ�BOOS���˸߼�����������#FF12FA | ��%s��#FFFAFA | ���#FFFA12 | ���#I1 | %d #C1��"),
		//		data.name, szFishName[nFishType], /*nFishMultiple,*/ lFishScore);
		//}
		//else
		//{
		//��ȡ����
		int Multiple = 1;
		if (Multiple <= 0)
		{
			Multiple = 1;
		}
			StrAwardTip.Format(_T("��ϲ���#FF0000|��%s��#00FAFA|�ڡ���BOOS��������#FF12FA|��%s��#FFFAFA|���#FFFA12|���#I1|%2f #C1"),
				data.name, szFishName[nFishType], /*nFishMultiple,*/ (double)(lFishScore)/ Multiple);
		//}

		
		// ��Ҳ���
		//if( nScoreType == EST_Cold )
		//{
		//	StrAwardTip.Format( _T("%s��� %s ������%s����� %d�� %I64d������"), pRoomBaseInfo->name, data.name, szFishName[nFishType], nFishMultiple, lFishScore);
		//}
		//else if ( nScoreType == EST_Laser )
		//{
		//	StrAwardTip.Format( _T("%s��� %s ʹ�ü��⣬��� %d�� %I64d������"), StrTable, CMDSAwardTip.szPlayName, nFishMultiple, lFishScore);
		//}

		//// ĩβ��ʾ
		//if ( nFishMultiple >= 500 )
		//{
		//	StrAwardTip += _T("�����ˣ�����");
		//}
		//else if ( nFishType == FishType_BaoXiang )
		//{
		//	StrAwardTip += _T("������������");
		//}
		//else
		//{
		//	StrAwardTip += _T("ʵ����Ⱥ������");
		//}
		m_pDataManage->SendRewardActivityNotify(StrAwardTip);
		//printf("%s\n", StrAwardTip);
		// ������Ϣ
	//	m_pITableFrame->SendRoomMessage( NULL, StrAwardTip, SMT_CHAT );
	}

	return true;
}

// ��ʼ�����
void CTableFrameSink::InitializationFishpond( uint nBeginTime /*= 0*/ )
{

	// �������
	int nPlayCount = 0;
	for( int nSite = 0; nSite < PlayChair_Max; ++nSite )
	{
		if( GetUserIDByDeskStation(nSite) != NULL )
			nPlayCount++;
	}

	// ��������
	int nCreateCount = 0;
	if (nPlayCount > 1)
	{
		nCreateCount = min(nPlayCount-1, 4) * m_iAddFinshCount + m_nCreateCount;
	}
	else
	{
		nCreateCount = min(nPlayCount, 4) * m_iAddFinshCount + m_nCreateCount;
	}
	
	
	// ��ȡ��Ч����
	int nValidCount = 0;
	for( int nFishIndex = 0; nFishIndex < m_ArrayFishInfo.GetCount(); ++nFishIndex )
	{
		nValidCount += (m_ArrayFishInfo.GetAt(nFishIndex).bRepeatCreate) ? 1 : 0;
	}

	// �ж�����
	if( nValidCount > nCreateCount )
	{
		return;
	}

	// ����ʱ��
	uint unCreateTime = nBeginTime;

	// С��Ⱥ
	if( (m_nAppearFishCount % 16) == 0 )
	{
		// С������
		int nNumber = (rand() % 3) + 3;
		byte cbFishType = RandomArea(6, 21, 19, 18, 16,13,12);

		// ������Ⱥ
		if ( rand() % 2 == 0 )
		{
			CreateFish( nNumber, cbFishType, uint_max, unCreateTime, 1000, 0.f, true, true, KillerType_No, INVALID_CHAIR, CShortPoint(0, 0), 0.f, false );
		}
		// ��ը��Ⱥ
		else
		{
			// ���·��
			int nBezierCount = 0;
			tagBezierPoint TBezierPoint[BEZIER_POINT_MAX];
			RandomPath( cbFishType, TBezierPoint, nBezierCount );
			CreateFishEx( 1, cbFishType, TBezierPoint, nBezierCount, unCreateTime, 0, 0.f, true, false, KillerType_All, INVALID_CHAIR, CShortPoint(-50, 0), 0.f, false );
			CreateFishEx( 1, cbFishType, TBezierPoint, nBezierCount, unCreateTime, 0, 0.f, true, false, KillerType_All, INVALID_CHAIR, CShortPoint(50, 0), 0.f, false );
			CreateFishEx( 1, cbFishType, TBezierPoint, nBezierCount, unCreateTime, 0, 0.f, true, false, KillerType_All, INVALID_CHAIR, CShortPoint(0, 50), 0.f, false );
			CreateFishEx( 1, cbFishType, TBezierPoint, nBezierCount, unCreateTime, 0, 0.f, true, false, KillerType_All, INVALID_CHAIR, CShortPoint(0, -50), 0.f, false );
		}
	}
	// ������
	else
	{
		if (rand()%100 < m_iCreatJiLv)
		{
			CreateFish(1, RandomArea(7, 30, 30,25,20,10,10,10), uint_max, unCreateTime, 0, 0.f, true, true, (RandomArea(2, 1, 99) == 0) ? KillerType_One : KillerType_No);
		}
		else
		{
			CreateFish(1, FishType_Max, uint_max, unCreateTime, 0, 0.f, true, true, (RandomArea(2, 1, 99) == 0) ? KillerType_One : KillerType_No);
		}

	}
}

// ������
LONGLONG CTableFrameSink::PlayCatchFish( WORD wChairID, int nBulletSplit, uint nFishCatchKey, int nMultipleIndex, EnumPlayCatchType nPlayCatchType )
{
	// ���ޱ���
	if( nMultipleIndex == Multiple_Max )
	{
		_Assert( false && "�����㱶���쳣��" );

		return 0;
	}
	if (GetUserIDByDeskStation(wChairID) <= 0)
	{
		return 0;
	}
	// ������Ϣ
	int nDeathScore = 0;
	int nDeathCount = 0;

	// ���·�������
	for( int nFishIndex = 0; nFishIndex < m_ArrayFishInfo.GetCount(); ++nFishIndex )
	{
		// ��ȡ��Ϣ
		tagFishInfo & TFishInfo = m_ArrayFishInfo.GetAt(nFishIndex);

		// ���·�������
		TFishInfo.nScoreChance = TFishInfo.nFishScore;
	}

	// ������
	for( int nFishIndex = 0; nFishIndex < m_ArrayFishInfo.GetCount(); ++nFishIndex )
	{
		// ��ȡ��Ϣ
		tagFishInfo & TFishInfo = m_ArrayFishInfo.GetAt(nFishIndex);

		// �ǲ�����
		if ( TFishInfo.nFishKey != nFishCatchKey )
		{
			continue;
		}

		// �޸���Ϣ
		nDeathScore += TFishInfo.nFishScore;
		nDeathCount += 1;

		// ���������
		if ( TFishInfo.nFishState != FishState_Killer /*&& TFishInfo.nFishType != FishType_ZhongYiTang*/ )
		{
			// ����
			break;
		}

		// ��Чλ��
		CDoublePoint PointFish;
		if ( FishMove( PointFish, TFishInfo ) != FishMoveType_Normal )
		{
			// ����
			break;
		}

		// �����ܱ���
		for( int nFishPeripheryIndex = 0; nFishPeripheryIndex < m_ArrayFishInfo.GetCount(); ++nFishPeripheryIndex )
		{
			// ��ȡ��Ϣ
			tagFishInfo & TPeripheryInfo = m_ArrayFishInfo.GetAt(nFishPeripheryIndex);

			// �ظ��Ƚ�
			if ( TPeripheryInfo.nFishKey == nFishCatchKey )
			{
				continue;
			}

			// ��Чλ��
			CDoublePoint PointPeriphery;
			if ( FishMove( PointPeriphery, TPeripheryInfo ) != FishMoveType_Normal )
			{
				continue;
			}

			// ������
			bool bRelatedFish = false;

			// ɱ����
			if ( TFishInfo.nFishState == FishState_Killer )
			{
				// ͬ����
				bRelatedFish = TFishInfo.nFishType == TPeripheryInfo.nFishType;
			}
			//else if ( TFishInfo.nFishType == FishType_ZhongYiTang )
			//{
			//	// �̾���
			//	bRelatedFish = DistanceOfPoint(PointFish, PointPeriphery) < 300.0;
			//}

			// ��Ч����
			if( bRelatedFish )
			{
				// �޸ļ���
				TFishInfo.nScoreChance += TPeripheryInfo.nFishScore;

				// ��ӷ���
				nDeathScore += TPeripheryInfo.nFishScore;
				nDeathCount += 1;
			}
		}

		// ����
		break;
	}

	// ��Ч����
	if( nDeathScore == 0 || nDeathCount == 0 )
	{
		return 0;
	}

	// ���ָ��
	//IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem( wChairID );
	
	// �Ѷ�ֵ
	double dModifyChance = DifficultyGame( wChairID, nMultipleIndex );
	//printf("������:����%d,���Ѷ�%llf\n", nMultipleIndex, dModifyChance);
	// ������Ϣ
	double dBulletScore = static_cast<double>(m_nMultipleValue[nMultipleIndex] * QianPao_Bullet);
	double dDeathScore = static_cast<double>(m_nMultipleValue[nMultipleIndex] * nDeathScore);

	// ������Ϣ
	nBulletSplit = max( 1, nBulletSplit );
	double dAppendChance = dBulletScore / (double)nBulletSplit / dDeathScore;
	double dBulletScale = (double)QianPao_Bullet / (double)nBulletSplit / (double)nDeathCount;

	// �����ܷ���
	LONGLONG lTotalCatchScore = 0;

	// ɱ����
	bool bFishKiller = false;
	byte nFishTypeKiller = FishType_Invalid;

	// ������
	bool bFishSpecial = false;
	byte cbFishTypeSpecial = 0;
	CDoublePoint PointSpecial;

	// ͣ��������
	bool bStayFish = false;

	// ȫ������
	bool bFishFullScreen = false;

	// ��ը������
	bool bFishExplosion = false;
	CDoublePoint PointExplosion;

	// ����������
	Static CWHArray< tagCatchInfo > ArrayCatchFishInfo;	
	ArrayCatchFishInfo.RemoveAll();

	// ��ֹ����, �����Ѷ�
	//if ( m_dRoomStock[nMultipleIndex] < dDeathScore )
	//{
	//	dModifyChance = min( 0.15, dModifyChance );
	//}

	//// ����̫����
	//if ( m_dRoomStock[nMultipleIndex] < -100 * m_nMultipleValue[nMultipleIndex] )
	//{
	//	dModifyChance = min( 0.05, dModifyChance );
	//}

	// ������Ĭ���Ѷ�
	if ( IsAndroidUser(wChairID) )
	{
		// Ĭ���Ѷ�
		dModifyChance = 1.0;

		// �����
		int nMultipleMax = 0;
		for ( int nIndex = 0; nIndex < Multiple_Max; ++nIndex )
		{
			nMultipleMax = max( nMultipleMax, m_nMultipleValue[nIndex] );
		}

		// ������Ӯ��̫��
		if ( m_lPlayScore[wChairID] - m_lPlayStartScore[wChairID] > 2000 * nMultipleMax )
		{
			dModifyChance = 0.5;
		}
	}

	// ��������ͳһ�Ѷ�
	if( m_bCompetitionRoom )
	{
		dModifyChance = max(1.2, dModifyChance);
	}

	// ������
	for( int nDeathIndex = 0; nDeathIndex < m_ArrayFishInfo.GetCount(); nDeathIndex++ )
	{
		// ��ȡ����Ϣ
		tagFishInfo & TFishInfo = m_ArrayFishInfo.GetAt(nDeathIndex);

		// �ǲ�����
		if ( TFishInfo.nFishKey != nFishCatchKey )
		{
			continue;
		}

		// ȫ�ֱ�ը�ж�
		LONGLONG lPlayExplosionCondition = 0;
		m_MapPlayExplosionCondition.Lookup( GetUserIDByDeskStation(wChairID), lPlayExplosionCondition );
		if( TFishInfo.nFishType == FishType_BaoZhaFeiBiao && lPlayExplosionCondition < m_lExplosionCondition )
		{
			continue;
		}

		// �������
		bool bSpecialDie = false;

		// ����Ǳ���, ���伸��ֱ��20%
		if ( TFishInfo.nFishType == FishType_BaoXiang && RAND_TRUE_FALSE(20, 80) )
		{
			bSpecialDie = true;
		}

		// ����Ǽ������
		if ( nPlayCatchType == EPCT_Laser )
		{
			bSpecialDie = true;
		}

		// ������
		int nCaptureChance = (int)(dBulletScale * 100.0 * dModifyChance);
		double dWillBeIn = 1.2 / dModifyChance;

		// �Ż�����
		nCaptureChance = max( nCaptureChance, 1 );

		// �����⴦��, ϵͳ��ֹ����
		//if ( !bSpecialDie && m_dRoomStock[nMultipleIndex] < dDeathScore && !m_bCompetitionRoom )
		//{
		//	dWillBeIn = max( 2.0, dWillBeIn );
		//	nCaptureChance = 1;
		//}

		// ��ӻ���
		TFishInfo.dCurChance += dAppendChance;

		// ������
		if ( bSpecialDie || TFishInfo.dCurChance > dWillBeIn || ( RAND_TRUE_FALSE( nCaptureChance, TFishInfo.nScoreChance * 100 ) ) )
		{
			// ����ը��
			if( TFishInfo.nFishType == FishType_BaoZhaFeiBiao )
			{
				// �������
				m_MapPlayExplosionCondition.SetAt(GetUserIDByDeskStation(wChairID), 0 );
			}

			// ��������
			lTotalCatchScore += CatchInFish( wChairID, nMultipleIndex, nPlayCatchType, TFishInfo, ArrayCatchFishInfo );

			// ��¼ɱ����
			if ( TFishInfo.nFishState == FishState_Killer && !bFishKiller )
			{
				// ��¼��Ϣ
				CDoublePoint PointKeller;
				bFishKiller = (FishMove( PointKeller, TFishInfo ) == FishMoveType_Normal);
				nFishTypeKiller = TFishInfo.nFishType;
			}

			// ��¼����
			if ( TFishInfo.nFishState == FishState_King && !bFishSpecial )
			{
				// ��¼��Ϣ
				bFishSpecial = (FishMove( PointSpecial, TFishInfo ) == FishMoveType_Normal);
				cbFishTypeSpecial = TFishInfo.nFishType;
			}

			// ��¼ͣ����
			//if ( TFishInfo.nFishType == FishType_ShuiHuZhuan && !bStayFish )
			//{
			//	// ��¼��Ϣ
			//	CDoublePoint PointStay;
			//	bStayFish = (FishMove( PointStay, TFishInfo ) == FishMoveType_Normal);
			//}

			//// ��¼ȫ����ը
			//if( TFishInfo.nFishType == FishType_BaoZhaFeiBiao && !bFishFullScreen )
			//{
			//	// ��¼��Ϣ
			//	CDoublePoint PointFullScreen;
			//	bFishFullScreen = (FishMove( PointFullScreen, TFishInfo ) == FishMoveType_Normal);
			//}

			//// ��¼��ը��
			//if ( TFishInfo.nFishType == FishType_ZhongYiTang && !bFishExplosion )
			//{
			//	// ��¼��Ϣ
			//	bFishExplosion = (FishMove( PointExplosion, TFishInfo ) == FishMoveType_Normal);
			//}

			// ɾ����
			m_ArrayFishInfo.RemoveAt(nDeathIndex);
		}

		// �˳�
		break;
	}

	// ɱ�������
	if ( bFishKiller )
	{
		// ������
		int nDeathIndex = 0; 
		while( nDeathIndex < m_ArrayFishInfo.GetCount() )
		{
			// ��ȡ��Ϣ
			tagFishInfo & TPeripheryInfo = m_ArrayFishInfo.GetAt(nDeathIndex);

			// �ܱ������
			CDoublePoint PointPeriphery;
			if( FishMove( PointPeriphery, TPeripheryInfo ) == FishMoveType_Normal && nFishTypeKiller == TPeripheryInfo.nFishType )
			{
				// ��������
				lTotalCatchScore += CatchInFish( wChairID, nMultipleIndex, EPCT_Killer, TPeripheryInfo, ArrayCatchFishInfo );

				// ɾ����
				m_ArrayFishInfo.RemoveAt(nDeathIndex);

				// ����
				continue;
			}

			// ��һ��
			++nDeathIndex;
		}
	}

	// ���������
	if ( bFishSpecial && m_bOpenShoalFish)
	{
		// ��������
		SpecialFishMatrix( cbFishTypeSpecial, PointSpecial );
	}

	// ȫ����ը
	if ( bFishFullScreen )
	{
		// ������
		LONGLONG lFishFullScreenScore = 0;
		int nDeathIndex = 0; 
		while( nDeathIndex < m_ArrayFishInfo.GetCount() )
		{
			// ��ȡ��Ϣ
			tagFishInfo & TPeripheryInfo = m_ArrayFishInfo.GetAt(nDeathIndex);

			// �ܱ������
			CDoublePoint PointPeriphery;
			if( FishMove( PointPeriphery, TPeripheryInfo ) == FishMoveType_Normal && PointPeriphery.x > 0 && PointPeriphery.x < DEFAULE_WIDTH && PointPeriphery.y > 0 && PointPeriphery.y < DEFAULE_HEIGHT )
			{
				// ��������
				LONGLONG lCatchScore = CatchInFish( wChairID, nMultipleIndex, EPCT_FullScreen, TPeripheryInfo, ArrayCatchFishInfo );
				lTotalCatchScore += lCatchScore;
				lFishFullScreenScore += lCatchScore;

				// ɾ����
				m_ArrayFishInfo.RemoveAt(nDeathIndex);

				// ����
				continue;
			}

			// ��һ��
			++nDeathIndex;
		}

		// �����
		int nMultipleMax = 0;
		for ( int nIndex = 0; nIndex < Multiple_Max; ++nIndex )
		{
			nMultipleMax = max( nMultipleMax, m_nMultipleValue[nIndex] );
		}

		// ��ԭ����
		if( !IsAndroidUser(wChairID) )
		{
			m_dDartStock += nMultipleMax * m_nExplosionStart - lFishFullScreenScore;
			//m_dRoomStock[nMultipleIndex] += lFishFullScreenScore;
		}
		else
		{
			m_dDartStock += nMultipleMax * m_nExplosionStart;
		}
	}

	// �ֲ���ը
	if( bFishExplosion )
	{
		// ������
		int nDeathIndex = 0; 
		while( nDeathIndex < m_ArrayFishInfo.GetCount() )
		{
			// ��ȡ��Ϣ
			tagFishInfo & TPeripheryInfo = m_ArrayFishInfo.GetAt(nDeathIndex);

			// �ܱ������
			CDoublePoint PointPeriphery;
			if( FishMove( PointPeriphery, TPeripheryInfo ) == FishMoveType_Normal && DistanceOfPoint(PointExplosion, PointPeriphery) < 300.0 && TPeripheryInfo.nFishType != FishType_BaoZhaFeiBiao )
			{
				// ��������
				lTotalCatchScore += CatchInFish( wChairID, nMultipleIndex, EPCT_Explosion, TPeripheryInfo, ArrayCatchFishInfo );

				// ɾ����
				m_ArrayFishInfo.RemoveAt(nDeathIndex);

				// ����
				continue;
			}

			// ��һ��
			++nDeathIndex;
		}
	}

	// ���Ͳ�����Ϣ
	SendCatchFish( wChairID, ArrayCatchFishInfo );

	// ͣ����Ϣ
	if ( bStayFish )
	{
		//FishFreeze( timeGetTime() - m_nStartTime );
	}

	// ������Ϣ
	return lTotalCatchScore;
}

// �Ƚ���
bool CTableFrameSink::ISFishCatchKey( uint nFishKey, CWHArray< uint > & ArrayFishCatchKey )
{
	// ��������
	for ( int nCatchIndex = 0; nCatchIndex < ArrayFishCatchKey.GetCount(); ++nCatchIndex )
	{
		// �ж���Ч����
		if ( ArrayFishCatchKey[nCatchIndex] != 0 && ArrayFishCatchKey[nCatchIndex] == nFishKey )
		{
			return true;
		}
	}

	return false;
}

// ������
LONGLONG CTableFrameSink::CatchInFish( WORD wChairID, int nMultipleIndex, EnumPlayCatchType nPlayCatchType, tagFishInfo & TFishInfo, CWHArray< tagCatchInfo > & ArrayCatchFishInfo )
{
	// �����
	int nFishScore = TFishInfo.nFishScore;

	// ���ˮ��
	if( TFishInfo.nFishState == FishState_Aquatic )
	{
		nFishScore += 8;
	}

	// ���ӱ�ը
	if( TFishInfo.nFishType == FishType_BaoXiang )
	{
		nFishScore = 0;
	}

	//����ǵ���ֱ�Ӽ��뱳��
	if (TFishInfo.nFishType == FishType_Hong_Bao)
	{
		nFishScore = 0;
		ChangeUserBage(wChairID,BAG_HONG_BAO);
	}

	if (TFishInfo.nFishType == FishType_Hua_Fei1)
	{
		nFishScore = 0;
		ChangeUserBage(wChairID, BAG_PHONE_CARD1);
	}

	if (TFishInfo.nFishType == FishType_Hua_Fei2)
	{
		nFishScore = 0;
		ChangeUserBage(wChairID, BAG_PHONE_CARD5);
	}

	if (TFishInfo.nFishType > m_ipropNum)
	{
		int Tmp = CUtil::GetRandRange(0, 100);
		if (Tmp > 50)
		{
			if (CUtil::GetRandRange(1, 1000) < m_iBingDong)
			{
				ChangeUserBage(wChairID, BAG_BING_DONG);
			}
		}
		else
		{
			if (CUtil::GetRandRange(1, 1000) < m_iLocking)
			{
				ChangeUserBage(wChairID, BAG_LOCK);
			}
		}	
	}
	// ������Ϣ
	LONGLONG lCatchScore = (LONGLONG)((LONGLONG)nFishScore * (LONGLONG)m_nMultipleValue[nMultipleIndex]);

	// �������
	PlayerIncome( wChairID, nMultipleIndex, lCatchScore );

	// ���ӱ�ը
	if( TFishInfo.nFishType == FishType_BaoXiang )
	{
		// �����
		m_bPlaySupply[wChairID] = false;

		// ����ǻ����ˣ��Ͳ�Ҫ���ּ���
		int nLaserChance = m_nLaserChance;
		if ( IsAndroidUser(wChairID) )
		{
			nLaserChance = 0;
		}

		// �����
		int nBoxActivity = RandomArea(8, nLaserChance, m_nSpeedChance, m_nGiftChance[0], m_nGiftChance[1], m_nGiftChance[2], m_nGiftChance[3], m_nGiftChance[4], m_nNullChance);

		// ������Ϊ���ͣ�ֻ��ǿ����
		//if( m_dRoomStock[nMultipleIndex] < m_nMultipleValue[nMultipleIndex] * 100.0 )
		//{
		//	nBoxActivity = RandomArea(8, 0, m_nSpeedChance, 0, 0, 0, 0, 0, m_nNullChance);
		//}

		// �жϻ
		switch (nBoxActivity)
		{
		case 0:	// ����
			{
				// ���¼���ʱ�䣨5�����ʱ�䣩
				m_nLaserPlayTime[wChairID] = m_nLaserTime + 5;

				// ���ͼ�����Ϣ
				CMD_S_Supply CMDSSupply;
				CMDSSupply.wChairID = wChairID;
				CMDSSupply.lSupplyCount = m_nLaserTime;
				CMDSSupply.nSupplyType = EST_Laser;
				SendTableData(INVALID_CHAIR, SUB_S_SUPPLY, &CMDSSupply, sizeof(CMDSSupply) );
			}
			break;
		case 1:	// ����
			{
				// ���¼���ʱ�䣨2�����ʱ�䣩
				m_nSpeedPlayTime[wChairID] = m_nSpeedTime + 2;

				// ���ͼ�����Ϣ
				CMD_S_Supply CMDSSupply;
				CMDSSupply.wChairID = wChairID;
				CMDSSupply.lSupplyCount = m_nSpeedTime;
				CMDSSupply.nSupplyType = EST_Speed;
				SendTableData(INVALID_CHAIR, SUB_S_SUPPLY, &CMDSSupply, sizeof(CMDSSupply) );
			}
			break;
		case 2:	// ���ͽ��
		case 3:
		case 4:
		case 5:
		case 6:
			{
				// ���²������
				lCatchScore = m_nGiftScore[ nBoxActivity - 2 ] * TFishInfo.nFishScore;

				// ����������Ϣ
				CMD_S_Supply CMDSSupply;
				CMDSSupply.wChairID = wChairID;
				CMDSSupply.lSupplyCount = lCatchScore;
				CMDSSupply.nSupplyType = EST_Gift;
				SendTableData(INVALID_CHAIR, SUB_S_SUPPLY, &CMDSSupply, sizeof(CMDSSupply) );

				// �������
				PlayerIncome( wChairID, nMultipleIndex, lCatchScore );

				// �󱶷�����Ϣ
				SendAwardTip(wChairID, FishType_BaoXiang, m_nGiftScore[ nBoxActivity - 2 ], lCatchScore, EST_Cold);
			//	printf("���ӷ���:%d\n", m_nGiftScore[nBoxActivity - 2]);
			}
			break;
		case 7:	// �ձ���
			{
				// ���Ϳձ�����Ϣ
				CMD_S_Supply CMDSSupply;
				CMDSSupply.wChairID = wChairID;
				CMDSSupply.lSupplyCount = 0;
				CMDSSupply.nSupplyType = EST_Null;
				SendTableData(INVALID_CHAIR, SUB_S_SUPPLY, &CMDSSupply, sizeof(CMDSSupply) );
			}
			break;
		}
	}
	else
	{
		
		// �󱶷�����Ϣ
		if (m_nFishMultiple[TFishInfo.nFishScore][0] >= m_nAwardMinMultiple && nPlayCatchType == EPCT_Bullet )
		{	
			SendAwardTip( wChairID, TFishInfo.nFishType, m_nFishMultiple[TFishInfo.nFishType][0], lCatchScore, EST_Cold );
			//printf("��������:%d,������:%d,��С���汶��:%d\n", m_nFishMultiple[TFishInfo.nFishType][0], TFishInfo.nFishType, m_nAwardMinMultiple);
		}
	}

	// ��ӳ�ˮ
	if (lCatchScore > 0)
	{
		m_lplayCutMoney[wChairID] += lCatchScore * m_iRate / 100;
		lCatchScore -= lCatchScore * m_iRate / 100;
	}


	// ��ӷ���
	m_lPlayScore[wChairID] += lCatchScore;

	// ��Ҳ�������
	m_lPlayFishCount[wChairID][TFishInfo.nFishType]++;

	// ���ò���
	tagCatchInfo TCatchInfo;
	TCatchInfo.nMultipleCount = TFishInfo.nFishScore;
	TCatchInfo.nFishKey = TFishInfo.nFishKey;
	TCatchInfo.lScoreCount = lCatchScore;

	// ��¼����
	ArrayCatchFishInfo.Add(TCatchInfo);

	return lCatchScore;
}

// ���Ͳ�����Ϣ
void CTableFrameSink::SendCatchFish( WORD wChairID, CWHArray< tagCatchInfo > & ArrayCatchFishInfo )
{
	// ����������Ϣ
	WORD wBufferSize = 0;
	byte cbBuffer[IPC_PACKAGE];
	int nSendIndex = 0;
	while( nSendIndex < ArrayCatchFishInfo.GetCount() )
	{
		// ��������
		if ( (wBufferSize + sizeof(CMD_S_CatchFish) + 1) > sizeof(cbBuffer) )
		{
			// ������Ϣ
			SendTableData(INVALID_CHAIR, SUB_S_FISH_CATCH, cbBuffer, wBufferSize);

			// ������Ϣ
			wBufferSize = 0;
		}

		// ��ȡ��Ϣ
		tagCatchInfo & TCatchInfo = ArrayCatchFishInfo.GetAt(nSendIndex);

		// ������Ϣ
		CMD_S_CatchFish CMDCatchFish;
		CMDCatchFish.wChairID = wChairID;
		CMDCatchFish.nFishKey = TCatchInfo.nFishKey;
		CMDCatchFish.nMultipleCount = TCatchInfo.nMultipleCount;
		CMDCatchFish.lScoreCount = TCatchInfo.lScoreCount;

		// ������Ϣ
		CopyMemory(cbBuffer + wBufferSize, &CMDCatchFish, sizeof(CMD_S_CatchFish) );
		wBufferSize += sizeof(CMD_S_CatchFish);

		// ��һ��
		nSendIndex++;
	}

	// ��Ч��Ϣ
	if ( wBufferSize > 0 ) 
	{
		// ������Ϣ
		SendTableData(INVALID_CHAIR, SUB_S_FISH_CATCH, cbBuffer, wBufferSize);
	}

	return ;
}

// ��Ϸ�Ѷ�
double CTableFrameSink::DifficultyGame( WORD wChairID, int nMultipleIndex )
{
	// �����Ѷ�
	double dRoomDifficulty = DifficultyRoom( nMultipleIndex );

	// �����Ѷ�
	//double dTableDifficulty = DifficultyTable( nMultipleIndex );

	// �����Ѷ�
	//double dPlayDifficulty = DifficultyPlay( wChairID, nMultipleIndex );

	// ��Ϸ�Ѷ�
	double dGameDifficulty = 0.0;
	int nDifficultyCount = 0;
	//�����Ѷ�
	double dUserdifficulty = 1;
	long userID = GetUserIDByDeskStation(wChairID);
	if (userID > 0)
	{
		GetUserDifficulty(userID, dUserdifficulty);
	}
	//printf("�����Ѷ�:%llf\n", dUserdifficulty);
	// ��Ч�Ѷ�
	if (m_nDifficultyStart[0])
	{
		dGameDifficulty += dUserdifficulty;
		nDifficultyCount += 1;
	}

	// ��Ч�Ѷ�
	if( m_nDifficultyStart[0] )
	{
		dGameDifficulty += dRoomDifficulty;
		nDifficultyCount += 1;
	}


	// ��Ч����
	if ( nDifficultyCount == 0 )
	{
		dGameDifficulty = 0.5;
	}
	else
	{
		dGameDifficulty /= nDifficultyCount;
	}


	// �����Ѷ�
	//int UserDifficulty = GetUserControlParam(wChairID);
	//if (UserDifficulty >=0 && UserDifficulty<1000)
	//{
	//	int Lev = UserDifficulty / 100;
	//	dGameDifficulty = m_dPlayDifficultyValue[Lev];
	//	//printf("�����Ѷ�:%d,�Ѷȵȼ�:%d,�Ѷ�ϵ��%lf\n", UserDifficulty, Lev, RoomDifficultyCount[Lev]);
	//}
	//else if (UserDifficulty >= 1000)
	//{
	//	dGameDifficulty = m_dPlayDifficultyValue[9];
	//}
	// �Ѷ�ֵ
	//printf("�Ѷ�ֵ%lf\n", dGameDifficulty);
	return dGameDifficulty;
}

// ��Ϸ�Ѷ�
double CTableFrameSink::DifficultyRoom( int nMultipleIndex )
{
	// �����Ѷ�
	double dRoomDifficulty = 1.0;
	//�Ѷ�ϵ��
	//double m_dRoomDifficultyValue[10] = { 0.05,0.15,0.25,0.5,0.75,1,1.25,1.7,2.5,3.5 };
	//printf("��Ϸ�Ѷ�:%lf\n", m_pooInfo.platformCtrlPercent);
	if (m_pDataManage->LoadPoolData())
	{
		if (m_pDataManage->m_rewardsPoolInfo.platformCtrlPercent <= 0)
		{
			dRoomDifficulty  == 1;
		}
		else
		{
			int Lev = 5;
			if (m_pDataManage->m_rewardsPoolInfo.platformCtrlPercent >= 1000 )
			{
				m_pDataManage->m_rewardsPoolInfo.platformCtrlPercent = 999;
			}
			else
			{
				Lev = m_pDataManage->m_rewardsPoolInfo.platformCtrlPercent / 100;
			}
			if (Lev >= 0 && Lev < Difficulty_Max)
			{
				dRoomDifficulty = m_dRoomDifficultyValue[Lev];
			}
			//printf("�����Ѷ�:%d,�Ѷȵȼ�:%d,�Ѷ�ϵ��%lf\n", m_pooInfo.platformCtrlPercent, Lev, RoomDifficultyCount[Lev]);
		}
	}
	
	// �Ѷ�����
	//int nDifficultyIndex = 0;

	//// ���㷿���Ѷ�
	//for ( nDifficultyIndex = 0; nDifficultyIndex < Difficulty_Max; ++nDifficultyIndex )
	//{
	//	// �Ƚ���Ϣ
	//	if( m_dRoomStock[nMultipleIndex] < m_nRoomDifficultyCount[nDifficultyIndex] * m_nMultipleValue[nMultipleIndex] )
	//	{
	//		// �����Ѷ�
	//		dRoomDifficulty = m_dRoomDifficultyValue[nDifficultyIndex];

	//		break;
	//	}
	//}

	//// ��Ч�Ѷ�
	//if( nDifficultyIndex == Difficulty_Max )
	//{
	//	// �����Ѷ�
	//	dRoomDifficulty = m_dRoomDifficultyValue[Difficulty_Max - 1];
	//}

	return dRoomDifficulty;
}

// ��Ϸ�Ѷ�
double CTableFrameSink::DifficultyTable( int nMultipleIndex )
{
	// �����Ѷ�
	double dTableDifficulty = 1.0;

	// �Ѷ�����
	int nDifficultyIndex = 0;

	// ���������Ѷ�
	for ( nDifficultyIndex = 0; nDifficultyIndex < Difficulty_Max; ++nDifficultyIndex )
	{
		// �Ƚ���Ϣ
		if( _TableStock(nMultipleIndex) < m_nTableDifficultyCount[nDifficultyIndex] * m_nMultipleValue[nMultipleIndex] )
		{
			// �����Ѷ�
			dTableDifficulty = m_dTableDifficultyValue[nDifficultyIndex];

			break;
		}
	}

	// ��Ч�Ѷ�
	if( nDifficultyIndex == Difficulty_Max )
	{
		// �����Ѷ�
		dTableDifficulty = m_dTableDifficultyValue[Difficulty_Max - 1];
	}

	return dTableDifficulty;
}

// ��Ϸ�Ѷ�
double CTableFrameSink::DifficultyPlay( WORD wChairID, int nMultipleIndex )
{
	// �����Ѷ�
	double dPlayDifficulty = 1.0;

	// �Ѷ�����
	int nDifficultyIndex = 0;

	// ��������Ѷ�
	for ( nDifficultyIndex = 0; nDifficultyIndex < Difficulty_Max; ++nDifficultyIndex )
	{
		// �Ƚ���Ϣ
		if( _PlayerStock(wChairID, nMultipleIndex) < m_nPlayDifficultyCount[nDifficultyIndex] * m_nMultipleValue[nMultipleIndex] )
		{
			// �����Ѷ�
			dPlayDifficulty = m_dPlayDifficultyValue[nDifficultyIndex];

			break;
		}
	}

	// ��Ч�Ѷ�
	if( nDifficultyIndex == Difficulty_Max )
	{
		// �����Ѷ�
		dPlayDifficulty = m_dPlayDifficultyValue[Difficulty_Max - 1];
	}

	return dPlayDifficulty;
}


// ��������
template< typename T > void CTableFrameSink::SwapVariable( T & One, T & Two )
{
	T Temp = One;
	One = Two;
	Two = Temp;
}

// ��ת��
CDoublePoint CTableFrameSink::Rotate( CDoublePoint & ptCircle, double dRadian, CDoublePoint & ptSome ) 
{ 
	CDoublePoint temp; 
	ptSome.x -= ptCircle.x; 
	ptSome.y -= ptCircle.y; 
	temp.x = ptSome.x*cos(dRadian) - ptSome.y*sin(dRadian) + ptCircle.x; 
	temp.y = ptSome.y*cos(dRadian) + ptSome.x*sin(dRadian) + ptCircle.y; 
	return temp; 
} 

// ��ת��
CShortPoint CTableFrameSink::Rotate( CShortPoint & ptCircle, double dRadian, CShortPoint & ptSome ) 
{ 
	CShortPoint temp; 
	ptSome.x -= ptCircle.x; 
	ptSome.y -= ptCircle.y; 
	temp.x = (short)(ptSome.x*cos(dRadian) - ptSome.y*sin(dRadian) + ptCircle.x); 
	temp.y = (short)(ptSome.y*cos(dRadian) + ptSome.x*sin(dRadian) + ptCircle.y); 
	return temp; 
} 

// ��֪Բ��,����,�뾶 ��Բ������һ
CShortPoint CTableFrameSink::RotatePoint( CShortPoint & ptCircle, double dRadian, double dRadius )
{
	CShortPoint temp;
	temp.x = (short)(dRadius*cos(dRadian) + ptCircle.x);
	temp.y = (short)(dRadius*sin(dRadian) + ptCircle.y);
	return temp;
}

// ����������
CDoublePoint CTableFrameSink::PointOnCubicBezier( tagBezierPoint * cp, double t )
{
	double   ax = 0.f, bx = 0.f, cx = 0.f;
	double   ay = 0.f, by = 0.f, cy = 0.f;
	double   tSquared = 0.f, tCubed = 0.f;
	CDoublePoint result;

	// ����ϵ��
	cx = 3.f * (cp->KeyOne.x - cp->BeginPoint.x);
	bx = 3.f * (cp->KeyTwo.x - cp->KeyOne.x) - cx;
	ax = cp->EndPoint.x - cp->BeginPoint.x - cx - bx;

	cy = 3.f * (cp->KeyOne.y - cp->BeginPoint.y);
	by = 3.f * (cp->KeyTwo.y - cp->KeyOne.y) - cy;
	ay = cp->EndPoint.y - cp->BeginPoint.y - cy - by;

	// �������ߵ�
	tSquared = t * t;
	tCubed = tSquared * t;

	result.x = (ax * tCubed) + (bx * tSquared) + (cx * t) + cp->BeginPoint.x;
	result.y = (ay * tCubed) + (by * tSquared) + (cy * t) + cp->BeginPoint.y;

	return result;
}

// �������
double CTableFrameSink::DistanceOfPoint( CDoublePoint & PointOne, CDoublePoint & PointTwo )
{
	return sqrt( pow( abs(PointOne.x - PointTwo.x), 2 ) + pow( abs(PointOne.y - PointTwo.y), 2 ) );
}

// �������
int CTableFrameSink::RandomArea( int nLen, ... )
{ 
	ASSERT( nLen > 0 );
	if ( nLen <= 0 )
		return 0;

	va_list argptr;
	va_start(argptr, nLen);
	Static CWHArray<int> Array;
	Array.RemoveAll();
	for ( byte i = 0; i < nLen; ++i )
	{
		int nTemp = va_arg( argptr, int );
		if( nTemp >= 0 && nTemp < 100000 )
		{
			Array.Add(nTemp);
		}
		else
		{
			ASSERT(FALSE);
			break;
		}
	}
	va_end(argptr);


	int nIndex = 0;
	int nACTotal = 0;
	for (int i = 0; i < Array.GetCount(); i++)
		nACTotal += Array[i];

	ASSERT( nACTotal > 0  );
	if ( nACTotal > 0 )
	{
		int nRandNum = ( rand() + GetTickCount() + m_nAppearFishCount ) % nACTotal;
		for (int i = 0; i < Array.GetCount(); i++)
		{
			nRandNum -= Array[i];
			if (nRandNum < 0)
			{
				nIndex = i;
				break;
			}
		}
	}
	else
	{
		nIndex = rand()%nLen;
	}


	ASSERT( nIndex < nLen );
	return nIndex;
}


// ʹ�ܹػ���Ȩ ���ػ�
bool CTableFrameSink::EnableShutdownPrivilege()
{
	HANDLE hProcess = NULL;
	HANDLE hToken = NULL;
	LUID uID = {0};
	TOKEN_PRIVILEGES stToken_Privileges = {0};

	// ��ȡ��ǰӦ�ó�����̾��
	hProcess = ::GetCurrentProcess(); 

	// �򿪵�ǰ���̵ķ������ƾ��(OpenProcessToken��������ʧ�ܷ���ֵΪ��)
	if(!::OpenProcessToken(hProcess,TOKEN_ADJUST_PRIVILEGES,&hToken)) 
		return false;

	// ��ȡȨ������Ϊ"SeShutdownPrivilege"��LUID(LookupPrivilegeValue��������ʧ�ܷ���ֵΪ��)
	if(!::LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&uID)) 
		return false;

	stToken_Privileges.PrivilegeCount = 1;			// ��������Ȩ�޸���
	stToken_Privileges.Privileges[0].Luid = uID;	// Ȩ�޵�LUID
	stToken_Privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; // Ȩ�޵�����,SE_PRIVILEGE_ENABLEDΪʹ�ܸ�Ȩ��

	// ���������������ָ��Ȩ��(AdjustTokenPrivileges��������ʧ�ܷ���ֵΪ��)
	if(!::AdjustTokenPrivileges(hToken,FALSE,&stToken_Privileges,sizeof stToken_Privileges,NULL,NULL)) 
		return false;

	// �鿴Ȩ���Ƿ�����ɹ�
	if(::GetLastError() != ERROR_SUCCESS) 
		return false;

	// �رվ��
	::CloseHandle(hToken);

	// �ػ���
	ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE,0);

	return true;
}

// �жϻ�����
bool CTableFrameSink::IsAndroidUser(BYTE wChairID )
{
	//IServerUserItem * pIAndroidUserItem = m_pITableFrame->GetTableUserItem( wChairID );
	//return pIAndroidUserItem && pIAndroidUserItem->IsAndroidUser();

	GameUserInfo user;
	if (GetUserData(wChairID, user))
	{
		return user.isVirtual;
	}

	return false;
}

// ��ʼ�����
void CTableFrameSink::InitializePlayer( WORD wChairID )
{
	// ��ʼ����Ϣ
	m_dwPlayID[wChairID] = 0;
	m_lPlayScore[wChairID] = 0;
	m_lPlayStartScore[wChairID] = 0;
	m_lplayCutMoney[wChairID] = 0;
	m_lBulletConsume[wChairID] = 0;
	m_lFireCount[wChairID] = 0;
	m_nPlayDownTime[wChairID] = 0;
	m_nRobotPlayTime[wChairID] = 0;
	m_nPlayDelay[wChairID] = 0;
	ZeroMemory(m_lPlayFishCount[wChairID], sizeof(m_lPlayFishCount[wChairID]));
	m_nMultipleIndex[wChairID] = 0;
	m_ArrayBulletKey[wChairID].clear();

	m_nEnergyValue[wChairID] = 0;
	m_nSupplyValue[wChairID] = RAND_EQUAL_MIN_MAX(m_nSupplyCondition[0], m_nSupplyCondition[1]);
	m_bPlaySupply[wChairID] = false;
	m_nLaserPlayTime[wChairID] = 0;
	m_nSpeedPlayTime[wChairID] = 0;

	// ɾ���ض���
	int nFishIndex = 0;
	while ( nFishIndex < m_ArrayFishInfo.GetCount() )
	{
		// ��ȡ����Ϣ
		tagFishInfo & TFishInfo = m_ArrayFishInfo.GetAt(nFishIndex);

		// �Ա�λ��
		if ( TFishInfo.wHitChair == wChairID )
		{
			// ɾ����
			m_ArrayFishInfo.RemoveAt( nFishIndex );

			continue;
		}

		// ��һ��
		nFishIndex++;
	}
}

// �������
void CTableFrameSink::PlayerConsume( WORD wChairID, int nMultipleIndex, LONGLONG lConsumeCount )
{
	// �����˲�����
	if( IsAndroidUser(wChairID) )
	{
		return ;
	}

	// ��Ч����
	if( nMultipleIndex == Multiple_Max )
	{
		return ;
	}

	// �޷��ۼ�
	if( lConsumeCount <= 0 )
	{
		return ;
	}

	// ����¼
	m_lStockCurrent += lConsumeCount;

	// ת������
	double dConsumption = static_cast<double>(lConsumeCount);

	// ��ˮ��ȡ
	double dExtractTax = dConsumption * m_dTaxRatio;
	dConsumption -= dExtractTax;
	m_dWholeTax[nMultipleIndex] += dExtractTax;

	// �����ۼ�
	double dExtractDart = dConsumption * ( (double)m_nExplosionProportion / 1000.0 );
	dConsumption -= dExtractDart;
	m_dDartStock += dExtractDart;

	// �����ۼ�
	//m_dRoomStock[nMultipleIndex] += dConsumption;

	// �����ۼ�
	double & dTableStock  = _TableStock( nMultipleIndex );
	dTableStock += dConsumption;

	// ��������
	double & dPlayStock = _PlayerStock( wChairID, nMultipleIndex );
	dPlayStock -= static_cast<double>(lConsumeCount);

	// ������Ϣ
	GameUserInfo * pIServerUserItem = GetTableUserItem(wChairID);
	// �ж�Ȩ��
	if( pIServerUserItem != NULL &&((pIServerUserItem->userStatus&USER_IDENTITY_TYPE_SUPER) == USER_IDENTITY_TYPE_SUPER))
	{
		// ������Ϣ
		CMD_S_System CMDSSystem;
		CMDSSystem.dRoomStock = m_dRoomStock[nMultipleIndex];
		CMDSSystem.dTableStock = dTableStock;
		CMDSSystem.dPlayStock = dPlayStock;
		CMDSSystem.dDartStock = m_dDartStock;
		CMDSSystem.dGameDifficulty = DifficultyGame( wChairID, nMultipleIndex );

		// ������Ϣ
		SendUserItemData( pIServerUserItem, SUB_S_SYSTEM, &CMDSSystem, sizeof(CMD_S_System) );
	}

	return ;
}

// �������
void CTableFrameSink::PlayerIncome( WORD wChairID, int nMultipleIndex, LONGLONG lIncomeCount )
{
	// �����˲�����
	if( IsAndroidUser(wChairID) )
	{
		return ;
	}

	// ��Ч����
	if( nMultipleIndex == Multiple_Max )
	{
		return ;
	}

	// �޷��ۼ�
	if( lIncomeCount <= 0 )
	{
		return ;
	}

	// ����¼
	m_lStockCurrent -= lIncomeCount;

	// ת������
	double dIncomeCount = static_cast<double>(lIncomeCount);

	// ��������
	//m_dRoomStock[nMultipleIndex] -= dIncomeCount;

	// ��������
	double & dTableStock = _TableStock( nMultipleIndex );
	dTableStock -= dIncomeCount;

	// �����ۼ�
	double & dPlayStock = _PlayerStock( wChairID, nMultipleIndex );
	dPlayStock += dIncomeCount;

	return ;
}

// ���ӿ��
double & CTableFrameSink::_TableStock( int nMultipleIndex )
{
	return m_pdTableStock[nMultipleIndex][/*m_pITableFrame->GetTableID()*/1];
}

// ���ӿ��
double & CTableFrameSink::_TableStock( unsigned short TableID, int nMultipleIndex )
{
	return m_pdTableStock[nMultipleIndex][TableID];
}

// ��ҿ��
double & CTableFrameSink::_PlayerStock( WORD wChairID, int nMultipleIndex )
{
	return m_dPlayStock[wChairID][nMultipleIndex];
}


//��ȡ�����Ϣ
GameUserInfo * CTableFrameSink::GetTableUserItem(WORD wChairID)
{
	if (!m_pDataManage)
	{
		return NULL;
	}

	long userID = GetUserIDByDeskStation(wChairID);
	if (userID <= 0)
	{
		return NULL;
	}

	return m_pDataManage->GetUser(userID);
}


//����ҷ�������
bool CTableFrameSink::SendUserItemData(GameUserInfo * pIServerUserItem, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	if (!pIServerUserItem)
	{
		INFO_LOG("pIServerUserItem=NULL");
		return true;
	}

	return SendGameData(pIServerUserItem->deskStation, pData, wDataSize, MSG_MAIN_LOADER_GAME, wSubCmdID, 0);
}
////////////////////////////////////////////////////////////////////////////////// 

bool CTableFrameSink::ReSetGameState(BYTE bLastStation)
{
	return true;
}

bool CTableFrameSink::GameBegin(BYTE bBeginFlag)
{
	SetTimer(TIME_WRITE_SCORE, 30000);
	IniConfig();
	LoadDynamicConfig();
	return __super::GameBegin(bBeginFlag);
}

//��Ӧ��־
void CTableFrameSink::DebugPrintf(const char *strOutputString, ...)
{
	CRedisLoader* pRedis = m_pDataManage->GetRedis();
	RoomBaseInfo roomBasekInfo;
	RoomBaseInfo* pRoomBaseInfo = NULL;
	if (pRedis->GetRoomBaseInfo(m_pDataManage->GetRoomID(), roomBasekInfo))
	{
		pRoomBaseInfo = &roomBasekInfo;
	}
	else
	{
		pRoomBaseInfo = ConfigManage()->GetRoomBaseInfo(m_pDataManage->GetRoomID());
	}
	if (!pRoomBaseInfo)
	{
		return;
	}

	char szFilename[256];
	CString strPath = CINIFile::GetAppPath() + GAMENAME + "_log\\";
	SHCreateDirectoryEx(NULL, strPath, NULL);
	CTime time = CTime::GetCurrentTime();
	sprintf(szFilename, "%s%d__%d%02d%02d_��������%d.txt", strPath, NAME_ID, time.GetYear(), time.GetMonth(), time.GetDay(), pRoomBaseInfo->gameID);
	FILE *fp = fopen(szFilename, "a");
	if (NULL == fp)
	{
		return;
	}
	//����ʱ��
	char cTime[30];
	::memset(cTime, 0, sizeof(cTime));
	sprintf(cTime, "[%d:%d:%d] ", time.GetHour(), time.GetMinute(), time.GetSecond());
	fprintf(fp, cTime);
	va_list arg;
	va_start(arg, strOutputString);
	vfprintf(fp, strOutputString, arg);
	fprintf(fp, "\n");
	fclose(fp);
}

//�û�����
bool CTableFrameSink::UserNetCut(GameUserInfo* pUser)
{
	//UserLeftDesk(pUser);
	//�����߳������
	KickOutUser(pUser->deskStation, REASON_KICKOUT_STAND_MINLIMIT);

	return true;
}

bool CTableFrameSink::IsPlayGame(BYTE bDeskStation)
{

	return false;
}


void CTableFrameSink::UserBeKicked(BYTE deskStation)
{
	// ����Ƿ�����
	for (WORD i = 0; i < PlayChair_Max; ++i)
	{
		if (deskStation == i) continue;
		long  userid = GetUserIDByDeskStation(i);
		if (userid <= 0) continue;

		//������ô�ͽ���һ��
		// �ж��û�
		long PlayeruserID = GetUserIDByDeskStation(deskStation);
		if (PlayeruserID > 0)
		{
			// ���ָ��
			//IServerUserItem * pIServerDismiss = m_pITableFrame->GetTableUserItem(nSite);
			GameUserInfo UserInfo;
			GetUserData(deskStation, UserInfo);

			// д��
			LONGLONG lScore = (m_lPlayScore[deskStation] - m_lPlayStartScore[deskStation]);
			if (lScore != 0)
			{
				// ����Ϊ���� ���ܳ���������
				if (lScore < 0 && (-lScore) > UserInfo.money)
				{
					lScore = -UserInfo.money;
				}
			}

			// ��Ϸʱ��
			DWORD dwEndTime = (DWORD)time(NULL) - m_nPlayDownTime[deskStation];

			ChangeUserPoint(deskStation, lScore, m_lplayCutMoney[i]);
			m_lplayCutMoney[i] = 0;

			// ����д��
			m_nRoomWrite += (-lScore);
			// ��������Ϣ
			InitializePlayer(deskStation);

		}

		return;
	}
	//ֻ����һ���� ��ô��Ϸ��ֱ�ӽ�����

	// ɾ����ʱ��
	KillTimer(IDI_FISH_CREATE);
	KillTimer(IDI_DELAY);
	KillTimer(IDI_SECOND);
	//ɾ�������Ѷ�
	long userID = GetUserIDByDeskStation(deskStation);
	DeletePersonalDifficulty(userID);

	// ȫ�ֱ�ը��ԭ
	if (GetFishCount(FishType_BaoZhaFeiBiao) > 0)
	{
		// �����
		int nMultipleMax = 0;
		for (int nMultipleIndex = 0; nMultipleIndex < Multiple_Max; ++nMultipleIndex)
		{
			nMultipleMax = max(nMultipleMax, m_nMultipleValue[nMultipleIndex]);
		}

		// ��������
		m_dDartStock += nMultipleMax * m_nExplosionStart;
	}

	// ����Ϣ
	m_ArrayFishInfo.RemoveAll();
	m_cbBackIndex = 0;
	m_nAppearFishCount = 0;
	m_bFishSwimming = FALSE;
	m_bBackExchange = FALSE;
	m_nStartTime = 0;

	// ����
	m_nFishTeamTime = 0;
	m_nFishKingTime = 30;
	ZeroMemory(m_nFishKing, sizeof(m_nFishKing));

	if (IsBegin)
	{
		// ������Ϸ
		GameFinish(deskStation, GF_NORMAL);
	}
}

// ����
bool CTableFrameSink::OnSubSkill(BYTE deskStation, VOID * pData, WORD wDataSize)
{
	if (sizeof(CMD_C_Skill) != wDataSize) return true;

	CMD_C_Skill *Skill = (CMD_C_Skill*)pData;
	if (!Skill)
	{
		DebugPrintf("����:���%d�����ܴ���", GetUserIDByDeskStation(deskStation));
		return false;
	}


	if (Skill->SkillType == 2)
	{
		int skillCount = GetUserBagDataByKey(deskStation, BAG_BING_DONG);
		if (skillCount > 0)
		{
			ChangeUserBage(deskStation, BAG_BING_DONG, -1);
			FishFreeze(5 * 1000);
		}
		else
		{
			SendGameMessage(deskStation, "���ı������ܵ��߲��㣡");
			return true;
		}
	}

	if (Skill->SkillType == 3)
	{
		int skillCount = GetUserBagDataByKey(deskStation, BAG_LOCK);
		if (skillCount > 0)
		{
			ChangeUserBage(deskStation, BAG_LOCK, -1);
		}
		else
		{
			SendGameMessage(deskStation, "�����������ܵ��߲��㣡");
			return true;
		}
	}

	SendTableData(INVALID_CHAIR, SUB_C_SKILL, pData, wDataSize);
	
	return true;
}


bool CTableFrameSink::HundredGameIsInfoChange(BYTE deskStation)
{
	return false;
}

bool CTableFrameSink::OnChangeCannon(BYTE deskStation, VOID * pData, WORD wDataSize)
{
	if (sizeof(CMD_C_CANNON) != wDataSize) return false;

	CMD_C_CANNON *cannon = (CMD_C_CANNON*)pData;

	long userID = GetUserIDByDeskStation(deskStation);
	if (userID <= 0) return false;

	SendTableData(INVALID_CHAIR, SUB_C_CHANGE_CONNON, pData, wDataSize);
	return true;
}