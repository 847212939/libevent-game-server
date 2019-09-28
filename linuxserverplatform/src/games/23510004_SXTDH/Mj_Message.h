#ifndef MJ_MESSAGE_H
#define MJ_MESSAGE_H
#include "mj.h"
#include "../../GameMessage/upgrademessage.h"
#include "CLogicBase.h"
#pragma pack(1) //1�ֽڶ��� 

/*--------------------------------------------------------------------*/
///�齫��������
struct MjRuleSet
{
	bool	bHaveWan;			//�Ƿ�����		
	bool	bHaveTiao;			//�Ƿ�����		
	bool	bHaveBing;			//�����б�		
	bool    bHaveHongZhong;		//�Ƿ��к���	
	bool    bHaveFaCai;			//�Ƿ��з���	
	bool    bHaveBaiBan;		//�Ƿ��аװ�	
	bool    bHaveFengPai;		//�Ƿ��ж�������
	bool    bHaveFlower;		//�Ƿ��л���	
	bool	bHaveJing;			//���޲���		
	bool    bNetCutTuoGuan;		//�Ƿ�����й�
	bool	bForceTuoGuan;		//�Ƿ�ǿ���й�
	bool	bAutoBegin;			//ʱ�䵽���Ƿ��Զ���ʼ	  
	BYTE    byAllMjCount;		//�齫������ 
	BYTE	byHandCardNum;		//��������
	BYTE	byGamePassType;		//����ʣ������
	BYTE	byGamePassNum;		//��������
	BYTE	byNextNtType;		//�¾�ׯ�ҵ�ȷ������	
	BYTE    byMenPaiNum[4];		//����������
	BYTE    byAutoOutTime;		//�Զ�����ʱ��	
	BYTE    byOutTime;			//����ʱ��		
	BYTE    byBlockTime;		//����˼��ʱ��	
	BYTE    byBeginTime;		//��ʼ�ȴ�ʱ��	
	int		iGameBase;			//�׷�
	bool	bIsDuoXiang;        //�Ƿ�һ�ڶ���
	bool	bIsSuanGangHu;      //�Ƿ�������ܺ����øܷ�
	bool    bChaoShiTuoGuan;    //��ҳ���ʱ�й�
	bool    bDuanXianTuoGuan;   //�Ƿ������������Ƿ��й�
	bool	bIsMatchMJ;			//�Ƿ�����
	MjRuleSet()
	{
		memset(this, 0, sizeof(MjRuleSet));
	}

};
/*--------------------------------------------------------------------*/
///��������
struct ActionRuleSet
{
	bool bChi;			//��
	bool bChiFeng;		//�Է���
	bool bChiJian;		//���з���
	bool bChiJing;		//�Բ���
	bool bPeng;			//��
	bool bKan;			//��
	bool bGang;			//��
	bool bBuGang;		//����
	bool bTing;			//��
	bool bQiangChi;		//����(�Ժ�����)
	bool bGangAfterTing;//���ƺ��ܸ���
	bool bSeziAfterGang;//�ܺ��ɫ��ץ��
	bool bQxHh;			//�������Ⱥ���һȦ�ڣ�
	bool bQiPeng;       //����������(һȦ��)
	bool bQxHhBut;		//�������Ⱥ���һȦ�ڣ�������ͬ���ƿ��ԣ����ø������
	bool bOutLianZhang;	//���ƺ��ܳ����ţ������Ʋ�����������Լ������������˳�ӣ�
	bool bQiangGangHu;	//���ܺ�
	bool bFangPao;		//�Ƿ��ܵ���
	bool bZiMo;			//�Ƿ�������
	bool bBuHua;        //�Ƿ���Ҫ����
	ActionRuleSet()
	{
		memset(this, 0, sizeof(ActionRuleSet));
	}
};

///���Ƽ����ʱ����
struct CheckHuStruct
{
	BYTE  data[HAND_CARD_NUM][2];   //�����ݣ�0��ֵ��1����
	BYTE  count;
	CheckHuStruct()
	{
		Init();
	}
	void Init()
	{
		for (int i = 0;i < HAND_CARD_NUM;i++)
		{
			data[i][0] = 255;
			data[i][1] = 0;
		}
		count = 0;
	};
	void Add(BYTE pai)
	{
		if (GetPaiCount(pai) > 0)
		{
			for (int i = 0;i < HAND_CARD_NUM;i++)
			{
				if (data[i][0] == pai && pai != 255)
					data[i][1]++;
			}
		}
		else
		{
			data[count][0] = pai;
			data[count][1] = 1;
			count++;
		}
	};
	BYTE GetPaiCount(BYTE pai)
	{
		for (int i = 0;i < HAND_CARD_NUM;i++)
		{
			if (data[i][0] == pai && pai != 255 && data[i][1] != 0)
				return data[i][1];
		}
		return 0;
	};
	BYTE GetDataCount()
	{
		return count;
	};
	BYTE GetAllPaiCount()
	{
		BYTE num = 0;
		for (int i = 0;i < HAND_CARD_NUM;i++)
		{
			if (data[i][0] != 255)
				num += data[i][1];
		}
		return num;
	};
	void SetPaiCount(BYTE pai, BYTE num)
	{
		for (int i = 0;i < HAND_CARD_NUM;i++)
		{
			if (data[i][0] == pai && pai != 255)
			{
				data[i][1] = num;
			}
		}
	}
};

///ƽ���������ݽṹ
struct PingHuStruct
{
	BYTE  byType[6];    //��������
	BYTE  data[6][4];   //����
	bool  isjing[6][4];//�Ƿ�������
	BYTE  count;        //�ڵ���
	PingHuStruct()
	{
		Init();
	};
	void Init()
	{
		memset(byType, 255, sizeof(byType));
		memset(data, 255, sizeof(data));
		memset(isjing, 0, sizeof(isjing));
		count = 0;
	};
	void AddData(BYTE type, BYTE pai[], bool jing[])
	{
		for (int i = 0;i < 6;i++)
		{
			if (byType[i] == 255)
			{
				byType[i] = type;
				data[i][0] = pai[0];data[i][1] = pai[1];data[i][2] = pai[2];data[i][3] = pai[3];
				isjing[i][0] = jing[0];isjing[i][1] = jing[1];isjing[i][2] = jing[2];isjing[i][3] = jing[3];
				count++;
				break;
			}
		}
	};
	void DeleteData(BYTE type, BYTE pai[], bool jing[])//ɾ��Ԫ��
	{
		for (int i = 0;i < 6;i++)
		{
			if (byType[i] == type && data[i][0] == pai[0] && data[i][1] == pai[1] && isjing[i][0] == jing[0] && isjing[i][1] == jing[1])
			{
				byType[i] = 255;
				memset(data[i], 255, sizeof(data[i]));
				memset(isjing[i], 0, sizeof(isjing[i]));
				count--;
				break;
			}
		}
	};
	void SortData()
	{
		for (int i = 0;i < 5;i++)
		{
			if (byType[i] != 255)
				continue;
			for (int j = i;j < 5;j++)
			{
				byType[j] = byType[j + 1];
				memcpy(data[j], data[j + 1], sizeof(data[j]));//����
				memcpy(isjing[j], isjing[j + 1], sizeof(isjing[j]));//�Ƿ�������
			}
		}
		count = 0;
		for (int i = 0;i < 5;i++)
		{
			if (byType[i] != 255)
				count++;
		}
	};
};

///�������ݽṹ
struct JingStruct
{
	BYTE  byPai[MAX_JING_PAI_NUM]; //ÿ�ž��Ƶ���ֵ
	BYTE  byJingNum;               //������������
	BYTE  byJingGen;
	JingStruct()
	{
		Init();
	}
	void Init()
	{
		memset(byPai, 255, sizeof(byPai));
		byJingNum = 0;
		byJingGen = 255;
	};
	bool CheckIsJing(BYTE pai)
	{
		if (pai == 255)
		{
			return false;
		}
		for (int i = 0;i < MAX_JING_PAI_NUM;i++)
		{
			if (byPai[i] == pai)
				return true;
		}
		return false;
	};
	bool AddJing(BYTE pai)
	{
		if (!CMjRef::IsValidateMj(pai))
		{
			return false;
		}
		for (int i = 0;i < MAX_JING_PAI_NUM;i++)
		{
			if (byPai[i] != 255 && byPai[i] == pai) //�Ѿ������־�����
			{
				return false;
			}
			if (byPai[i] == 255)
			{
				byPai[i] = pai;
				byJingNum++;
				return true;
			}
		}
		return false;
	};
	bool DeleteJing(BYTE pai)
	{
		for (int i = 0;i < MAX_JING_PAI_NUM;i++)
		{
			if (pai == byPai[i] && pai != 255)
			{
				byPai[i] = 255;
				byJingNum--;
				return true;
			}
		}
		return false;
	}
	void Sort()//��С��������
	{
		CLogicBase::SelectSort(byPai, MAX_JING_PAI_NUM, false);
	}
};

//�齫�ƣ��������ƣ������㣬��ǰ���Ʒ���
struct MjMenPaiData
{
	BYTE    byMenPai[MAX_MJ_PAI_COUNT];				//�齫����������Ҹ�����������������ÿ������2���� 
	BYTE    byNt;						//ׯ��λ��
	BYTE	byStation;					//��ǰץ����ҵ�λ��
	BYTE	byDir;						//��ǰץ�Ʒ���(ץ�Ǹ������ǰ����)
	BYTE	byGetPaiDir;				//�������Ʒ���
	BYTE	byRemainDun;				//ʣ�����
	BYTE	byGetPai;				    //���ƶ���
	BYTE    byMenPaiDun[PLAY_COUNT];	//ÿ�������ǰ�Ķ���
	BYTE    byStartIndex;				//������ʼ����		
	BYTE    byEndIndex;				    //���ƽ�������		
	BYTE    byCaiShenIndex;				//����λ������		
	BYTE    byAllPaiNum;				//�����Ƶ�����
	BYTE    byPaiNum;                   //��������
	MjMenPaiData()
	{
		Init();
	};
	void Init()
	{
		memset(byMenPai, 255, sizeof(byMenPai));
		memset(byMenPaiDun, 0, sizeof(byMenPaiDun));//�����ǰ�Ķ���
		byNt = 255;						//ׯ��λ��
		byStation = 255;					//��ǰץ����ҵ�λ��
		byDir = 255;						//��ǰץ�Ʒ���(ץ�Ǹ������ǰ����)
		byGetPaiDir = 255;				//�������Ʒ���
		byRemainDun = 255;				//ʣ�����
		byGetPai = 255;				    //���ƶ���
		byStartIndex = 255;				//������ʼ����
		byEndIndex = 255;				    //���ƽ�������
		byAllPaiNum = 0;				    //�����Ƶ�����
		byCaiShenIndex = 255;				//����λ������
		byPaiNum = 0;
	};
	BYTE GetMenPaiNum()
	{
		BYTE num = 0;
		for (int i = 0;i < MAX_MJ_PAI_COUNT;i++)
		{
			if (byMenPai[i] != 255)
			{
				num++;
			}
		}
		return num;
	};
	BYTE GetOnePaiNum(BYTE pai)
	{
		if (pai == 255)
		{
			return 0;
		}
		BYTE num = 0;
		for (int i = 0;i < MAX_MJ_PAI_COUNT;i++)
		{
			if (byMenPai[i] == pai)
			{
				num++;
			}
		}
		return num;
	};
	bool DeleteAnMenPai(BYTE pai)
	{
		if (pai == 255)
			return false;
		for (int i = 0;i < MAX_MJ_PAI_COUNT;++i)
		{
			if (byMenPai[i] != 255)
			{
				byMenPai[i] = 255;
				return true;
			}
		}
		return false;
	};
	bool ChangAnMenPai(BYTE oldpai, BYTE newpai)// ������Ҫ�������� oldpai,���������� newpai
	{
		if (oldpai == 255 || newpai == 255)
			return false;
		for (int i = 0;i < MAX_MJ_PAI_COUNT;++i)
		{
			if (byMenPai[i] == oldpai)
			{
				byMenPai[i] = newpai;//���Ƴɹ�
				return true;
			}
		}
		return false;
	};
};

/*----------------------------------------------------------------*/
///ץ�����¼�
struct	tagZhuaPai
{
	BYTE	byUser;		//ץ�����λ��
	BYTE	byPs;		//ץ�Ƶ�ֵ
	BYTE	byGetPaiIndex;//ץ�Ƶ�����
	BYTE	m_byArHandPai[PLAY_COUNT][HAND_CARD_NUM];//��������
	BYTE	m_byArHandPaiCount[PLAY_COUNT];	//��������
	BYTE	m_byArOutPai[PLAY_COUNT][OUT_CARD_NUM];//���Ʋ�����40�ţ�0Ϊ������־
	BYTE	byMenPai[4][MEN_CARD_NUM];//����
	BYTE	m_byArHuaPai[PLAY_COUNT][HUA_CARD_NUM];//��һ���
	tagZhuaPai()
	{
		Init();
	}
	void Init()
	{
		byUser = 255;		//ץ�����λ��
		byPs = 255;		//ץ�Ƶ�ֵ
		byGetPaiIndex = 255;//ץ�Ƶ�����
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArHandPaiCount, 0, sizeof(m_byArHandPaiCount));	//��������
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//��������
		memset(byMenPai, 255, sizeof(byMenPai));	//����
		memset(m_byArHuaPai, 255, sizeof(m_byArHuaPai));	//����
	}
};
/*----------------------------------------------------------------*/
///������Ҳ����¼�
struct tagOneBuHua
{
	bool	bFinish;//�Ƿ񲹻�����
	BYTE	byUser;		//�������λ��
	BYTE	byHua;		//ץ���û���
	BYTE	byPs;		//��������
	BYTE	m_byArHandPai[PLAY_COUNT][HAND_CARD_NUM];//��������
	BYTE	m_byArHandPaiCount[PLAY_COUNT];	//��������
	BYTE	byMenPai[4][MEN_CARD_NUM];//����
	BYTE	m_byArHuaPai[PLAY_COUNT][HUA_CARD_NUM];
	tagOneBuHua()
	{
		Init();
	}
	void Init()
	{
		byUser = 255;//�������λ��
		byHua = 255;	  //ץ���û���
		byPs = 255;	  //��������
		memset(byMenPai, 255, sizeof(byMenPai));	//����
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������
		memset(m_byArHandPaiCount, 0, sizeof(m_byArHandPaiCount));	//��������
		memset(m_byArHuaPai, 255, sizeof(m_byArHuaPai));//��������
		bFinish = false;
	}
};
/*----------------------------------------------------------------*/
///��2��ɫ��ȷ������λ�ã������ͣ������Ƶ㣨��С�㣩�¼�	
struct	tagTwoSeziDirAndGetPai
{
	BYTE	byUser;		//��ɫ�����
	BYTE	bySezi0;	//ɫ��0����
	BYTE	bySezi1;	//ɫ��1����
	tagTwoSeziDirAndGetPai()
	{
		Init();
	}
	void Init()
	{
		byUser = 255;	//��ɫ�����
		bySezi0 = 255;	//ɫ��0����
		bySezi1 = 255;	//ɫ��1����
	}
};

/*----------------------------------------------------------------*/
///�����¼�
struct	tagSendPai
{
	BYTE	m_byArHandPai[PLAY_COUNT][HAND_CARD_NUM];//��������
	BYTE	m_byArHandPaiCount[PLAY_COUNT];		//�����������
	BYTE	byMenPai[4][MEN_CARD_NUM];//����
	tagSendPai()
	{
		Init();
	}
	void Init()
	{
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArHandPaiCount, 0, sizeof(m_byArHandPaiCount));//��������
		memset(byMenPai, 255, sizeof(byMenPai));	//����
	}
};
/*----------------------------------------------------------------*/
///������Ҳ����¼�
struct	tagAllBuHua
{
	BYTE byBeiBuCard[PLAY_COUNT][MAX_MJ_HUA_NUM];//�����Ļ���
	BYTE byBuCard[PLAY_COUNT][MAX_MJ_HUA_NUM];   //���ķǻ���
	int iBuCount[PLAY_COUNT];//��������
	tagAllBuHua()
	{
		Init();
	}
	void Init()
	{
		memset(byBeiBuCard, 255, sizeof(byBeiBuCard));
		memset(byBuCard, 255, sizeof(byBuCard));
		memset(iBuCount, 0, sizeof(iBuCount));
	}
};

/*----------------------------------------------------------------*/
///�����ܺ�֪ͨ�¼��¼�
struct	tagCPGNotifyMes
{
	bool	bZhuaPai;			//�����ץ�ƺ��ж���(�ͻ���������ʾ����ʱʱ���)
	bool	bChi;				//�Ƿ��ܳ�
	bool	bPeng;				//�Ƿ�����
	bool	bGang;				//�Ƿ��ܸ�
	bool	bKan;				//�Ƿ��ܿ���
	bool	bSaoHu;				//��ǰ�ܷ�ɨ��
	bool	bTing;				//��ǰ�ܷ�����
	bool	bHu;				//�Ƿ��ܺ�
	bool    bHaveReturn;		//�ͻ����Ƿ񷵻ص��û�ѡ��
	bool    bCanAction;			//����Ƿ��ж�������
	bool    bCanChi[3];			//ͷ��β�ܹ���
	BYTE    byPeng;				//��������
	BYTE	byChi[3][3];		//�ܳԵ���
	BYTE	iGangData[4][2];	//�ܸܵ���	
	BYTE	byTingCanOut[HAND_CARD_NUM];//�����ܴ����
	BYTE    byMaxThing;					//��������������
	BYTE	byHuType[MAX_HUPAI_TYPE];	//��������
	BYTE    iHuPs;				//��Һ�����
	BYTE	byHu;			    //�������λ��
	BYTE    byFangPao;	  	    //�����λ��
	BYTE	byBlockTime;		//����ʱ��	
	BYTE	byUser;				//����λ��
	BYTE	byPs;				//���Ƶ�ֵ
	tagCPGNotifyMes()
	{
		Init();
	}
	void Init()
	{
		byUser = 255;			//�������λ��
		byPs = 255;			//���Ƶ�ֵ
		byBlockTime = 255;		//����˼��ʱ��
		bZhuaPai = false;			//�Ƿ��Լ�ץ��
		bChi = 0;				//�Ƿ��ܳ�
		bPeng = 0;				//�Ƿ�����
		bGang = 0;				//�Ƿ��ܸ�
		bKan = 0;				//�Ƿ��ܿ���
		bSaoHu = 0;				//��ǰ�ܷ�ɨ��
		bTing = 0;				//��ǰ�ܷ�����
		bHu = 0;				//�Ƿ��ܺ�
		bCanAction = false;		//����Ƿ��ж�������
		byMaxThing = ACTION_NO;//��������������
		bHaveReturn = false;		//�ͻ����Ƿ񷵻ص��û�ѡ��
		memset(byChi, 255, sizeof(byChi));//�ܳԵ���
		memset(iGangData, 255, sizeof(iGangData));//�ܸܵ���	
		memset(byTingCanOut, 255, sizeof(byTingCanOut));///�����ܴ����
		memset(byHuType, 255, sizeof(byHuType));///��������
		iHuPs = 255;///��Һ�����
		byHu = 255;
		byFangPao = 255;
		byPeng = 255;//��������
		memset(bCanChi, false, sizeof(bCanChi));
	}
};

///�����ܺ�֪ͨ�¼��¼�
struct	tagCPGNotify
{
	bool	bZhuaPai;			//�����ץ�ƺ��ж���(�ͻ���������ʾ����ʱʱ���)
	bool	bChi;				//�Ƿ��ܳ�
	bool	bPeng;				//�Ƿ�����
	bool	bGang;				//�Ƿ��ܸ�
	bool	bKan;				//�Ƿ��ܿ���
	bool	bSaoHu;				//��ǰ�ܷ�ɨ��
	bool	bTing;				//��ǰ�ܷ�����
	bool	bHu;				//�Ƿ��ܺ�
	bool    bHaveReturn;		//�ͻ����Ƿ񷵻ص��û�ѡ��
	bool    bCanAction;			//����Ƿ��ж�������
	bool    bCanChi[3];			//ͷ��β�ܹ���
	BYTE    byPeng;				//��������
	BYTE	byChi[3][3];		//�ܳԵ���
	BYTE	iGangData[4][2];	//�ܸܵ���	
	BYTE	byTingCanOut[HAND_CARD_NUM];//�����ܴ����
	BYTE    byMaxThing;					//��������������
	BYTE	byHuType[MAX_HUPAI_TYPE];	//��������
	BYTE    iHuPs;				//��Һ�����
	BYTE	byHu;				//�������λ��
	BYTE    byFangPao;			//�������λ��
	BYTE	byBlockTime;		//����ʱ��	
	BYTE	byUser;				//����λ��
	BYTE	byPs;				//���Ƶ�ֵ

	//���Ƶ�ʱ���õ�
	BYTE	byHuCard[HAND_CARD_NUM][MAX_CANHU_CARD_NUM];  //���ƣ��������ƺ�������
	BYTE	byCardRemainNum[MAX_MJ_PAI]; //ÿ���Ƶ�ʣ������
	tagCPGNotify()
	{
		Init();
	}
	void Init()
	{
		byUser = 255;			//�������λ��
		byPs = 255;			//���Ƶ�ֵ
		byBlockTime = 255;		//����˼��ʱ��
		bZhuaPai = false;			//�Ƿ��Լ�ץ��
		bChi = 0;				//�Ƿ��ܳ�
		bPeng = 0;				//�Ƿ�����
		bGang = 0;				//�Ƿ��ܸ�
		bKan = 0;				//�Ƿ��ܿ���
		bSaoHu = 0;				//��ǰ�ܷ�ɨ��
		bTing = 0;				//��ǰ�ܷ�����
		bHu = 0;				//�Ƿ��ܺ�
		bCanAction = false;		//����Ƿ��ж�������
		byMaxThing = ACTION_NO;//��������������
		bHaveReturn = false;		//�ͻ����Ƿ񷵻ص��û�ѡ��
		memset(byChi, 255, sizeof(byChi));//�ܳԵ���
		memset(iGangData, 255, sizeof(iGangData));//�ܸܵ���	
		memset(byTingCanOut, 255, sizeof(byTingCanOut));///�����ܴ����
		memset(byHuType, 255, sizeof(byHuType));///��������
		iHuPs = 255;///��Һ�����
		byHu = 255;
		byFangPao = 255;
		byPeng = 255;//��������
		memset(bCanChi, false, sizeof(bCanChi));

		memset(byHuCard, 255, sizeof(byHuCard));
		memset(byCardRemainNum, 0, sizeof(byCardRemainNum));
	}
};
/*----------------------------------------------------------------*/
///�����¼�
struct	tagOutPai
{
	bool	bTing;
	BYTE	byUser;//�������λ��
	BYTE	byPs;//���Ƶ�ֵ
	tagOutPai()
	{
		Init();
	}
	void Init()
	{
		bTing = false;
		byUser = 255;				//�������λ��
		byPs = 255;				//���Ƶ�ֵ
	}
};

/*----------------------------------------------------------------*/
///�������¼�
struct tagChiPai
{
	BYTE	byDo;		//0δ������10�ѽ������м�״̬Ϊ���ڷ���
	BYTE	byUser;		//�������λ��
	BYTE	byBeChi;	//���������λ��
	BYTE	byType;		//���Ƶ�����
	BYTE	byPs;		//�Ե���ֵ
	BYTE	byChiPs[3];	//���Ƶ�����
	BYTE	m_byArHandPai[PLAY_COUNT][HAND_CARD_NUM];//��������
	BYTE	m_byArHandPaiCount[PLAY_COUNT];	//��������
	BYTE	m_byArOutPai[PLAY_COUNT][OUT_CARD_NUM];	//���Ʋ�����40�ţ�0Ϊ������־
	BYTE	m_byArOutPaiCount[PLAY_COUNT];		//���Ƶ�����
	TCPGStruct m_UserGCPData[PLAY_COUNT][5];     //�����ܸ���
	tagChiPai()
	{
		Init();
	}
	void Init()
	{
		byDo = 255;//0δ������10�ѽ������м�״̬Ϊ���ڷ���
		byUser = 255;//�������λ��
		byBeChi = 255;//���������λ��
		byType = ACTION_NO;//���Ƶ�����
		byPs = 255;	//�Ե���ֵ
		memset(byChiPs, 255, sizeof(byChiPs));//���Ƶ�����
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArHandPaiCount, 0, sizeof(m_byArHandPaiCount));//��������
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//��������
		memset(m_byArOutPaiCount, 0, sizeof(m_byArOutPaiCount));	//��������
		memset(m_UserGCPData, 255, sizeof(m_UserGCPData));//�����ܸ���
	}
};

/*----------------------------------------------------------------*/
///�������¼�
struct tagPengPai
{
	BYTE	byUser;		//�������λ��
	BYTE	byBePeng;	//���������λ��
	BYTE	byPs;		//������ֵ
	BYTE	m_byArHandPai[PLAY_COUNT][HAND_CARD_NUM];//��������
	BYTE	m_byArHandPaiCount[PLAY_COUNT];	//��������
	BYTE	m_byArOutPai[PLAY_COUNT][OUT_CARD_NUM];//���Ʋ�����40�ţ�0Ϊ������־
	BYTE	m_byArOutPaiCount[PLAY_COUNT];		//���Ƶ�����
	TCPGStruct m_UserGCPData[PLAY_COUNT][5];     //�����ܸ���
	tagPengPai()
	{
		Init();
	}
	void Init()
	{
		byUser = 255;//�������λ��
		byBePeng = 255;//���������λ��
		byPs = 255;//������ֵ
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArHandPaiCount, 0, sizeof(m_byArHandPaiCount));//��������
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//��������
		memset(m_byArOutPaiCount, 0, sizeof(m_byArOutPaiCount));	//��������
		memset(m_UserGCPData, 255, sizeof(m_UserGCPData));//�����ܸ���
	}
};

/*----------------------------------------------------------------*/
///�������¼�
struct tagGangPai
{
	BYTE	byUser;		//�������λ��
	BYTE	byBeGang;	//���������λ��
	BYTE	byType;		//���Ƶ�����
	BYTE	byPs;		//�ܵ���ֵ
	BYTE	byGangPai[4];//Ҫ�ܵ�������
	BYTE	m_byArHandPai[PLAY_COUNT][HAND_CARD_NUM];//��������
	BYTE	m_byArHandPaiCount[PLAY_COUNT];			//��������
	BYTE	m_byArOutPai[PLAY_COUNT][OUT_CARD_NUM];	//���Ʋ�����40�ţ�0Ϊ������־
	BYTE	m_byArOutPaiCount[PLAY_COUNT];			//���Ƶ�����
	int     iGangFen[PLAY_COUNT];					//�ܷ�
	TCPGStruct m_UserGCPData[PLAY_COUNT][5];		//�����ܸ���
	tagGangPai()
	{
		Init();
	}
	void Init()
	{
		byUser = 255;//�������λ��
		byBeGang = 255;//���������λ��
		byType = ACTION_NO;//���Ƶ�����
		byPs = 255;	//�ܵ���ֵ
		memset(byGangPai, 255, sizeof(byGangPai));//Ҫ�ܵ�������
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArHandPaiCount, 0, sizeof(m_byArHandPaiCount));//��������
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//��������
		memset(m_byArOutPaiCount, 0, sizeof(m_byArOutPaiCount));	//��������
		memset(m_UserGCPData, 255, sizeof(m_UserGCPData));//�����ܸ���
		memset(iGangFen, 0, sizeof(iGangFen));
	}
};
/*----------------------------------------------------------------*/
///�������¼�
struct tagTingPai
{
	bool	bTing;		//�Ƿ�����
	bool	bHitTing[4];//��ҵ��������
	BYTE	byUser;		//�������λ��
	BYTE	byTing;		//�������λ��
	BYTE	byPs;		//���Ƴ�����ֵ
	BYTE	byTingCanOut[HAND_CARD_NUM];			//�����ܴ����
	tagTingPai()
	{
		Init();
	}
	void Init()
	{
		byUser = 255;//�������λ��
		byTing = 255;//�������λ��
		byPs = 255;//���Ƴ�����ֵ	
		bTing = false;
		memset(bHitTing, 0, sizeof(bHitTing));//��ҵ��������
		memset(byTingCanOut, 255, sizeof(byTingCanOut));
	}
};
/*----------------------------------------------------------------*/
///�������¼�
struct tagHuPai
{
	bool	bHaveHu[PLAY_COUNT];//��Ӧ������Ƿ������
	bool    bZimo;				//�Ƿ�����
	bool	bIsLiuJu;			//�Ƿ�����
	BYTE	byHu;				//�������λ��
	BYTE    byFangPao;			//�������λ��
	BYTE	byPs;				//������
	int	    iUserNums;			//������ҵĸ���
	BYTE	m_byArHandPai[PLAY_COUNT][HAND_CARD_NUM];//��������
	BYTE	m_byArHandPaiCount[PLAY_COUNT];			//��������
	BYTE	m_byArOutPai[PLAY_COUNT][OUT_CARD_NUM];	//���Ʋ�����40�ţ�0Ϊ������־
	BYTE    byHuType[PLAY_COUNT][MAX_HUPAI_TYPE];	//������ҵĺ�������
	tagHuPai()
	{
		Init();
	}
	void Init()
	{
		iUserNums = 0;
		byHu = 255;//�������λ��
		byFangPao = 255;
		byPs = 255;	//������ֵ
		bIsLiuJu = true;	//�Ƿ�����
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArHandPaiCount, 0, sizeof(m_byArHandPaiCount));//��������
		memset(m_byArOutPai, 255, sizeof(m_byArOutPai));//��������
		memset(byHuType, 255, sizeof(byHuType));//������ҵĺ�������
		memset(bHaveHu, 0, sizeof(bHaveHu));
		bZimo = false;		//�Ƿ�����
	}
};
struct tagHuPaiInfo
{
	int     iHuFenType[PLAY_COUNT][MAX_HU_FEN_KIND];//������ҵĺ��Ʒ���
	tagHuPaiInfo()
	{
		Init();
	}
	void Init()
	{
		memset(iHuFenType, 0, sizeof(iHuFenType));
	}
};
/*----------------------------------------------------------------*/
///����¼�
struct tagCountFen
{
	bool	bZiMo;								    //�Ƿ�����
	bool	bIsLiuJu;								//�Ƿ�����
	bool    bIsHu[PLAY_COUNT];						//��Щ��Һ�����
	BYTE	byFangPao;								//�������λ��
	BYTE	byNt;									//ׯ��λ��
	bool    m_bQiangGangHu;							 //�Ƿ����ܺ����������ܺ��Ĺ����²Ŵ���
	BYTE	m_byArHandPai[PLAY_COUNT][HAND_CARD_NUM];//��������
	BYTE	m_byArHandPaiCount[PLAY_COUNT];			//��������
	BYTE    byHuType[PLAY_COUNT][MAX_HUPAI_TYPE];	//������ҵĺ�������
	BYTE    byTypeFans[MAX_HUPAI_TYPE];				//���ͷ���
	int	    byUserNums;								//������ҵĸ���
	int		iBase;						//�׷�
	int		iFanCount[PLAY_COUNT];		//����ҷ���
	int		iZongFen[PLAY_COUNT];		//������ܷ�
	int		iMingGang[PLAY_COUNT];		//���ܵĸ���
	int		iAnGang[PLAY_COUNT];		//���ܵĸ���
	int		iTax;						//����ѣ���˰��
	int     iHuFenType[PLAY_COUNT][MAX_HU_FEN_KIND];//������ҵĺ��Ʒ���
	__int64 		i64ZongFen[PLAY_COUNT];			//������ҵ��ܷ�
	__int64 		i64Money[PLAY_COUNT];			//������ҵ��ܷ�
	TCPGStruct	UserGCPData[PLAY_COUNT][5];			//�����ܸ���
	tagHuPaiInfo HupaiInfo[PLAY_COUNT];
	tagCountFen()
	{
		Init();
	}
	void Init()
	{
		byFangPao = 255;		//�������λ��
		byNt = 255;		//ׯ��λ��
		bIsLiuJu = true;		//�Ƿ�����
		bZiMo = false;
		memset(i64ZongFen, 0, sizeof(i64ZongFen));
		memset(i64Money, 0, sizeof(i64Money));
		memset(m_byArHandPai, 255, sizeof(m_byArHandPai));//��������	
		memset(m_byArHandPaiCount, 0, sizeof(m_byArHandPaiCount));//��������	
		memset(UserGCPData, 255, sizeof(UserGCPData));
		memset(iFanCount, 0, sizeof(iFanCount));
		memset(iZongFen, 0, sizeof(iZongFen));
		memset(iMingGang, 0, sizeof(iMingGang));
		memset(iAnGang, 0, sizeof(iAnGang));
		memset(byHuType, 255, sizeof(byHuType));
		memset(bIsHu, 0, sizeof(bIsHu));	//��Щ��Һ�����
		memset(byTypeFans, 0, sizeof(byTypeFans));
		memset(iHuFenType, 0, sizeof(iHuFenType));
		byUserNums = 0;
		iBase = 0;
		iTax = 0;//����ѣ���˰��
		m_bQiangGangHu = false;
		for (int i = 0;i < PLAY_COUNT;i++)
		{
			HupaiInfo[i].Init();
		}
	}
};

//��������
struct tagMatchMJ
{
	//���Ʋ�����14�� 
	BYTE byArHandPai[PLAY_COUNT][HAND_CARD_NUM];
	//��������	  
	BYTE byArHandPaiCount[PLAY_COUNT];
	tagMatchMJ()
	{
		Init();
	}
	void Init()
	{
		memset(byArHandPai, 255, sizeof(byArHandPai));
		memset(byArHandPaiCount, 0, sizeof(byArHandPaiCount));
	}
};

//������Ѱ�¼�
struct tagActionTurn
{
	BYTE	byChi;		//��
	BYTE	byPeng;		//��
	BYTE	byGang;		//��
	BYTE    byHu;		//��
	tagActionTurn()
	{
		Init();
	}
	void Init()
	{
		byChi = 255;
		byPeng = 255;
		byGang = 255;
		byHu = 255;
	}
};
#pragma pack() //1�ֽڶ���
#endif
