#ifndef MJ_H
#define MJ_H
#define MAKE_A_MJ(hs,pd)  ( (hs) * 10 +(pd))
#define MAX_MJ_KIND_NUM		34		//����齫��������
#define MAX_MJ_HUA_NUM		8		//������������
#define MAX_MJ_PAI_COUNT    144     //һ���齫�����������Ŀǰ���ܳ���255
#define MAX_JING_PAI_NUM    8       //���������������ָ�����ƣ���ӣ�
#define MAX_CPG_NUM         5		//������������
//ץ��˳ʱ�룬������ʱ��
class CMjEnum
{
public:
	enum MJ_TYPE_HUA_SE//������
	{
		MJ_TYPE_PAI_NONE = -10,//�޻�ɫ
		MJ_TYPE_PAI_WAN = 0,//��
		MJ_TYPE_PAI_TIAO = 1,//��
		MJ_TYPE_PAI_BING = 2,//ͬ
		MJ_TYPE_PAI_FENG = 3,//��
		MJ_TYPE_PAI_HUA = 4//��
	};
	enum MJ_TYPE_PAI_DIAN//�Ƶ�
	{
		MJ_TYPE_PAI_DIAN_NONE = -1,
		MJ_TYPE_PAI_DIAN_1 = 1,
		MJ_TYPE_PAI_DIAN_2 = 2,
		MJ_TYPE_PAI_DIAN_3 = 3,
		MJ_TYPE_PAI_DIAN_4 = 4,
		MJ_TYPE_PAI_DIAN_5 = 5,
		MJ_TYPE_PAI_DIAN_6 = 6,
		MJ_TYPE_PAI_DIAN_7 = 7,
		MJ_TYPE_PAI_DIAN_8 = 8,
		MJ_TYPE_PAI_DIAN_9 = 9
	};
	enum MJ_TYPE
	{
		MJ_TYPE_NONE = 0,
		MJ_TYPE_FCHUN = MAKE_A_MJ(MJ_TYPE_PAI_FENG, 11),
		MJ_TYPE_FXIA = MAKE_A_MJ(MJ_TYPE_PAI_FENG, 12),
		MJ_TYPE_FQIU = MAKE_A_MJ(MJ_TYPE_PAI_FENG, 13),
		MJ_TYPE_FDONG = MAKE_A_MJ(MJ_TYPE_PAI_FENG, 14),
		MJ_TYPE_FMEI = MAKE_A_MJ(MJ_TYPE_PAI_FENG, 15),
		MJ_TYPE_FLAN = MAKE_A_MJ(MJ_TYPE_PAI_FENG, 16),
		MJ_TYPE_FZHU = MAKE_A_MJ(MJ_TYPE_PAI_FENG, 17),
		MJ_TYPE_FJU = MAKE_A_MJ(MJ_TYPE_PAI_FENG, 18),

		MJ_TYPE_CAISHEN = MAKE_A_MJ(MJ_TYPE_PAI_FENG, 19),
		MJ_TYPE_YUANBAO = MAKE_A_MJ(MJ_TYPE_PAI_FENG, 20),
		MJ_TYPE_MAO = MAKE_A_MJ(MJ_TYPE_PAI_FENG, 21),
		MJ_TYPE_LAOXU = MAKE_A_MJ(MJ_TYPE_PAI_FENG, 22),

		MJ_TYPE_FD = MAKE_A_MJ(MJ_TYPE_PAI_FENG, MJ_TYPE_PAI_DIAN_1),///��
		MJ_TYPE_FN = MAKE_A_MJ(MJ_TYPE_PAI_FENG, MJ_TYPE_PAI_DIAN_2),///��
		MJ_TYPE_FX = MAKE_A_MJ(MJ_TYPE_PAI_FENG, MJ_TYPE_PAI_DIAN_3),///��
		MJ_TYPE_FB = MAKE_A_MJ(MJ_TYPE_PAI_FENG, MJ_TYPE_PAI_DIAN_4),///��
		MJ_TYPE_ZHONG = MAKE_A_MJ(MJ_TYPE_PAI_FENG, MJ_TYPE_PAI_DIAN_5),///����
		MJ_TYPE_FA = MAKE_A_MJ(MJ_TYPE_PAI_FENG, MJ_TYPE_PAI_DIAN_6),///����
		MJ_TYPE_BAI = MAKE_A_MJ(MJ_TYPE_PAI_FENG, MJ_TYPE_PAI_DIAN_7),///�װ�

		MJ_TYPE_W1 = MAKE_A_MJ(MJ_TYPE_PAI_WAN, MJ_TYPE_PAI_DIAN_1),///��
		MJ_TYPE_W2 = MAKE_A_MJ(MJ_TYPE_PAI_WAN, MJ_TYPE_PAI_DIAN_2),
		MJ_TYPE_W3 = MAKE_A_MJ(MJ_TYPE_PAI_WAN, MJ_TYPE_PAI_DIAN_3),
		MJ_TYPE_W4 = MAKE_A_MJ(MJ_TYPE_PAI_WAN, MJ_TYPE_PAI_DIAN_4),
		MJ_TYPE_W5 = MAKE_A_MJ(MJ_TYPE_PAI_WAN, MJ_TYPE_PAI_DIAN_5),
		MJ_TYPE_W6 = MAKE_A_MJ(MJ_TYPE_PAI_WAN, MJ_TYPE_PAI_DIAN_6),
		MJ_TYPE_W7 = MAKE_A_MJ(MJ_TYPE_PAI_WAN, MJ_TYPE_PAI_DIAN_7),
		MJ_TYPE_W8 = MAKE_A_MJ(MJ_TYPE_PAI_WAN, MJ_TYPE_PAI_DIAN_8),
		MJ_TYPE_W9 = MAKE_A_MJ(MJ_TYPE_PAI_WAN, MJ_TYPE_PAI_DIAN_9),

		MJ_TYPE_T1 = MAKE_A_MJ(MJ_TYPE_PAI_TIAO, MJ_TYPE_PAI_DIAN_1),///��
		MJ_TYPE_T2 = MAKE_A_MJ(MJ_TYPE_PAI_TIAO, MJ_TYPE_PAI_DIAN_2),
		MJ_TYPE_T3 = MAKE_A_MJ(MJ_TYPE_PAI_TIAO, MJ_TYPE_PAI_DIAN_3),
		MJ_TYPE_T4 = MAKE_A_MJ(MJ_TYPE_PAI_TIAO, MJ_TYPE_PAI_DIAN_4),
		MJ_TYPE_T5 = MAKE_A_MJ(MJ_TYPE_PAI_TIAO, MJ_TYPE_PAI_DIAN_5),
		MJ_TYPE_T6 = MAKE_A_MJ(MJ_TYPE_PAI_TIAO, MJ_TYPE_PAI_DIAN_6),
		MJ_TYPE_T7 = MAKE_A_MJ(MJ_TYPE_PAI_TIAO, MJ_TYPE_PAI_DIAN_7),
		MJ_TYPE_T8 = MAKE_A_MJ(MJ_TYPE_PAI_TIAO, MJ_TYPE_PAI_DIAN_8),
		MJ_TYPE_T9 = MAKE_A_MJ(MJ_TYPE_PAI_TIAO, MJ_TYPE_PAI_DIAN_9),

		MJ_TYPE_B1 = MAKE_A_MJ(MJ_TYPE_PAI_BING, MJ_TYPE_PAI_DIAN_1),///ͬ
		MJ_TYPE_B2 = MAKE_A_MJ(MJ_TYPE_PAI_BING, MJ_TYPE_PAI_DIAN_2),
		MJ_TYPE_B3 = MAKE_A_MJ(MJ_TYPE_PAI_BING, MJ_TYPE_PAI_DIAN_3),
		MJ_TYPE_B4 = MAKE_A_MJ(MJ_TYPE_PAI_BING, MJ_TYPE_PAI_DIAN_4),
		MJ_TYPE_B5 = MAKE_A_MJ(MJ_TYPE_PAI_BING, MJ_TYPE_PAI_DIAN_5),
		MJ_TYPE_B6 = MAKE_A_MJ(MJ_TYPE_PAI_BING, MJ_TYPE_PAI_DIAN_6),
		MJ_TYPE_B7 = MAKE_A_MJ(MJ_TYPE_PAI_BING, MJ_TYPE_PAI_DIAN_7),
		MJ_TYPE_B8 = MAKE_A_MJ(MJ_TYPE_PAI_BING, MJ_TYPE_PAI_DIAN_8),
		MJ_TYPE_B9 = MAKE_A_MJ(MJ_TYPE_PAI_BING, MJ_TYPE_PAI_DIAN_9)
	};
};
static int stcArr_A_Mj[] =
{
	CMjEnum::MJ_TYPE_W1,
	CMjEnum::MJ_TYPE_W2,
	CMjEnum::MJ_TYPE_W3,
	CMjEnum::MJ_TYPE_W4,
	CMjEnum::MJ_TYPE_W5,
	CMjEnum::MJ_TYPE_W6,
	CMjEnum::MJ_TYPE_W7,
	CMjEnum::MJ_TYPE_W8,
	CMjEnum::MJ_TYPE_W9,

	CMjEnum::MJ_TYPE_B1 ,
	CMjEnum::MJ_TYPE_B2 ,
	CMjEnum::MJ_TYPE_B3 ,
	CMjEnum::MJ_TYPE_B4,
	CMjEnum::MJ_TYPE_B5,
	CMjEnum::MJ_TYPE_B6,
	CMjEnum::MJ_TYPE_B7,
	CMjEnum::MJ_TYPE_B8,
	CMjEnum::MJ_TYPE_B9,


	CMjEnum::MJ_TYPE_T1,
	CMjEnum::MJ_TYPE_T2,
	CMjEnum::MJ_TYPE_T3,
	CMjEnum::MJ_TYPE_T4,
	CMjEnum::MJ_TYPE_T5,
	CMjEnum::MJ_TYPE_T6,
	CMjEnum::MJ_TYPE_T7,
	CMjEnum::MJ_TYPE_T8,
	CMjEnum::MJ_TYPE_T9,

	CMjEnum::MJ_TYPE_FD,
	CMjEnum::MJ_TYPE_FN,
	CMjEnum::MJ_TYPE_FX,
	CMjEnum::MJ_TYPE_FB,
	CMjEnum::MJ_TYPE_ZHONG,
	CMjEnum::MJ_TYPE_FA,
	CMjEnum::MJ_TYPE_BAI

};
//�齫����
static int stcArr_A_Mj_W[] =
{
	CMjEnum::MJ_TYPE_W1,
	CMjEnum::MJ_TYPE_W2,
	CMjEnum::MJ_TYPE_W3,
	CMjEnum::MJ_TYPE_W4,
	CMjEnum::MJ_TYPE_W5,
	CMjEnum::MJ_TYPE_W6,
	CMjEnum::MJ_TYPE_W7,
	CMjEnum::MJ_TYPE_W8,
	CMjEnum::MJ_TYPE_W9
};
//�齫����
static int stcArr_A_Mj_T[] =
{
	CMjEnum::MJ_TYPE_T1,
	CMjEnum::MJ_TYPE_T2,
	CMjEnum::MJ_TYPE_T3,
	CMjEnum::MJ_TYPE_T4,
	CMjEnum::MJ_TYPE_T5,
	CMjEnum::MJ_TYPE_T6,
	CMjEnum::MJ_TYPE_T7,
	CMjEnum::MJ_TYPE_T8,
	CMjEnum::MJ_TYPE_T9
};
//�齫��Ͳ
static int stcArr_A_Mj_B[] =
{
	CMjEnum::MJ_TYPE_B1 ,
	CMjEnum::MJ_TYPE_B2 ,
	CMjEnum::MJ_TYPE_B3 ,
	CMjEnum::MJ_TYPE_B4,
	CMjEnum::MJ_TYPE_B5,
	CMjEnum::MJ_TYPE_B6,
	CMjEnum::MJ_TYPE_B7,
	CMjEnum::MJ_TYPE_B8,
	CMjEnum::MJ_TYPE_B9
};
//�齫�ķ���
static	int	stcArr_A_Mj_FENG[] =
{
	CMjEnum::MJ_TYPE_FD,//����
	CMjEnum::MJ_TYPE_FN,//�Ϸ�
	CMjEnum::MJ_TYPE_FX,//����
	CMjEnum::MJ_TYPE_FB //����
};
//�齫������
static	int	stcArr_A_Mj_ZI[] =
{
	CMjEnum::MJ_TYPE_ZHONG,	//����
	CMjEnum::MJ_TYPE_FA,	//����
	CMjEnum::MJ_TYPE_BAI	//�װ�

};
//�齫�Ļ���
static int stcArr_A_Mj_Other[] =
{
	CMjEnum::MJ_TYPE_FCHUN,//��
	CMjEnum::MJ_TYPE_FXIA, //��
	CMjEnum::MJ_TYPE_FQIU, //��
	CMjEnum::MJ_TYPE_FDONG,//��

	CMjEnum::MJ_TYPE_FMEI, //÷
	CMjEnum::MJ_TYPE_FLAN, //��
	CMjEnum::MJ_TYPE_FZHU, //��
	CMjEnum::MJ_TYPE_FJU   //��
};
//��������
enum HUPAI_TYPE//��������ö�ٽṹ
{
	HUPAI_TYPE_None = 0,		////û��	
	HUPAI_TYPE_HuPai = 1,		////����
	HUPAI_TYPE_ZiMo = 2,		////����	
	HUPAI_TYPE_PingHu = 3,		////ƽ��	
	HUPAI_TYPE_TianHu = 4,		////���	
	HUPAI_TYPE_DiHu = 5,		////�غ�	
	HUPAI_TYPE_GangKai = 6,		//���ϻ�	
	HUPAI_TYPE_GangHaoPao = 7,		//�ܺ���
	HUPAI_TYPE_YiBanGao = 8,		//һ���	
	HUPAI_TYPE_XiXiangFeng = 9,		//ϲ���	
	HUPAI_TYPE_LianLiu = 10,	//����		
	HUPAI_TYPE_LaoShaoFu = 11,	//���ٸ�	
	HUPAI_TYPE_YaoJiuKe = 12,	//�۾ſ�	
	HUPAI_TYPE_MingGang = 13,	//����
	HUPAI_TYPE_QueYiMen = 14,	//ȱһ��	
	HUPAI_TYPE_WuZi = 15,	//����
	HUPAI_TYPE_BianZhang = 16,	//����		
	HUPAI_TYPE_KanZhang = 17,	//����		
	HUPAI_TYPE_DanDiaoJiang = 18,	//������
	HUPAI_TYPE_JianKe = 19,	//����
	HUPAI_TYPE_QuanFengKe = 20,	//Ȧ���
	HUPAI_TYPE_MenFengKe = 21,	//�ŷ��
	HUPAI_TYPE_MenQianQing = 22,	//��ǰ��	
	HUPAI_TYPE_SiGuiYi = 23,	//�Ĺ�һ	
	HUPAI_TYPE_ShuangTongKe = 24,	//˫ͬ��	
	HUPAI_TYPE_ShuangAnKe = 25,	//˫����	
	HUPAI_TYPE_AnGang = 26,	//����
	HUPAI_TYPE_DuanYao = 27,	//����		
	HUPAI_TYPE_QuanDaiYao = 28,	//ȫ����	
	HUPAI_TYPE_BuQiuRen = 29,	//������	
	HUPAI_TYPE_ShuangMingGang = 30,	//˫����	
	HUPAI_TYPE_HuJueZhang = 31,	//�;���	
	HUPAI_TYPE_PengPengHu = 32,	//������	
	HUPAI_TYPE_HunYiSe = 33,	//��һɫ
	HUPAI_TYPE_SanSeSanBuGao = 34,	//��ɫ������
	HUPAI_TYPE_WuMenQi = 35,	//������
	HUPAI_TYPE_QuanQiuRen = 36,	//ȫ����	
	HUPAI_TYPE_ShuangAnGang = 37,	//˫����	
	HUPAI_TYPE_ShuangJianKe = 38,	//˫����
	HUPAI_TYPE_HuaLong = 39,	//����		
	HUPAI_TYPE_TuiBuDao = 40,	//�Ʋ���
	HUPAI_TYPE_SanSeSanTongShun = 41,	//��ɫ��ͬ˳
	HUPAI_TYPE_SanSeSanJieGao = 42,	//��ɫ���ڸ�
	HUPAI_TYPE_WuFanHu = 43,	//�޷���	
	HUPAI_TYPE_MiaoShouHuiChun = 44,	//���ֻش�	
	HUPAI_TYPE_HaiDiLaoYue = 45,	//��������	
	HUPAI_TYPE_QiangGang = 46,	//���ܺ�	
	HUPAI_TYPE_QuanBuKao = 47,	//ȫ����(ʮ����)
	HUPAI_TYPE_ZuHeLong = 48,	//�����	
	HUPAI_TYPE_DaYuWu = 49,	//������	
	HUPAI_TYPE_XiaoYuWu = 50,	//С����	
	HUPAI_TYPE_SanFengKe = 51,	//�����
	HUPAI_TYPE_QingLong = 52,	//����		
	HUPAI_TYPE_SanSeShuangLongHui = 53,	//��ɫ˫����
	HUPAI_TYPE_YiSeSanBuGao = 54,	//һɫ������
	HUPAI_TYPE_QuanDaiWu = 55,	//ȫ����	
	HUPAI_TYPE_SanTongKe = 56,	//��ͬ��	
	HUPAI_TYPE_SanAnKe = 57,	//������	
	HUPAI_TYPE_QiDui = 58,	//�߶�		
	HUPAI_TYPE_QiXingBuKao = 59,	//���ǲ���
	HUPAI_TYPE_QuanShuangKe = 60,	//ȫ˫��	
	HUPAI_TYPE_QingYiSe = 61,	//��һɫ	
	HUPAI_TYPE_YiSeSanTongShun = 62,	//һɫ��ͬ˳
	HUPAI_TYPE_YiSeSanJieGao = 63,	//һɫ���ڸ�
	HUPAI_TYPE_QuanDa = 64,	//ȫ��		
	HUPAI_TYPE_QuanZhong = 65,	//ȫ��		
	HUPAI_TYPE_QuanXiao = 66,	//ȫС		
	HUPAI_TYPE_YiSeSiBuGao = 67,	//һɫ�Ĳ���
	HUPAI_TYPE_SanGang = 68,	//����		
	HUPAI_TYPE_HunYaoJiu = 69,	//���۾�
	HUPAI_TYPE_YiSeSiTongShun = 70,	//һɫ��ͬ˳
	HUPAI_TYPE_YiSeSiJieGao = 71,	//һɫ�Ľڸ�
	HUPAI_TYPE_QingYaoJiu = 72,	//���۾�	
	HUPAI_TYPE_XiaoSiXi = 73,	//С��ϲ
	HUPAI_TYPE_XiaoSanYuan = 74,	//С��Ԫ
	HUPAI_TYPE_ZiYiSe = 75,	//��һɫ
	HUPAI_TYPE_SiAnKe = 76,	//�İ���	
	HUPAI_TYPE_YiSeShuangLongHui = 77,	//һɫ˫����
	HUPAI_TYPE_DaSiXi = 78,	//����ϲ
	HUPAI_TYPE_DaSanYuan = 79,	//����Ԫ
	HUPAI_TYPE_LvYiSe = 80,	//��һɫ
	HUPAI_TYPE_JiuLianBaoDeng = 81,	//��������
	HUPAI_TYPE_SiGang = 82,	//�ĸܣ�ʮ���޺���	
	HUPAI_TYPE_LianQiDui = 83,	//���߶�			
	HUPAI_TYPE_ShiSanYao = 84,	//ʮ����
	HUPAI_TYPE_HUA = 85,    //����
	HUPAI_TYPE_QingYiSeQiDui = 86,	//��һɫ�߶�
	HUPAI_TYPE_QingYiSeYiTiaoLong = 87,	//��һɫһ����
	HUPAI_TYPE_LongQiDui = 88,	//���߶�
	HUPAI_TYPE_DaPiao = 89,	//��Ʈ
	HUPAI_TYPE_JingHu = 90,	//4����Ӻ�
};

//��������
enum MJ_GROUP_TYPE
{
	TYPE_JIANG_PAI = 15,				//��������
	TYPE_JINGDIAO_JIANG = 16,				//������������
	TYPE_SHUN_ZI = 17,				//˳������
	TYPE_AN_KE = 18,				//��������
	TYPE_JING_KE = 19,				//�����������
};
class CMjRef
{
public:
	CMjRef() {}
	virtual ~CMjRef() {}
	static bool IsValidateMj(BYTE iCard)//�Ϸ���ֵ
	{
		return iCard >= CMjEnum::MJ_TYPE_FCHUN && iCard <= CMjEnum::MJ_TYPE_LAOXU ||
			iCard >= CMjEnum::MJ_TYPE_FD && iCard <= CMjEnum::MJ_TYPE_BAI ||
			iCard >= CMjEnum::MJ_TYPE_B1 && iCard <= CMjEnum::MJ_TYPE_B9 ||
			iCard >= CMjEnum::MJ_TYPE_W1 && iCard <= CMjEnum::MJ_TYPE_W9 ||
			iCard >= CMjEnum::MJ_TYPE_T1 && iCard <= CMjEnum::MJ_TYPE_T9;
	}
	//��ȡ�ƵĻ�ɫ
	static BYTE GetHuaSe(BYTE byCard)
	{
		return (byCard) / 10;
	}
	//��ȡ�Ƶ��Ƶ�
	static BYTE GetCardPaiDian(BYTE byCard)
	{
		return (byCard) % 10;
	}
	//��ȡ�Ƶ��Ƶ�
	static BYTE GetCardPaiDianEx(BYTE byCard)
	{
		if (byCard >= CMjEnum::MJ_TYPE_FD)
		{
			return 0;
		}
		return (byCard) % 10;
	}
	//�ж����ǲ���1,9
	static bool IsYiJiuPai(BYTE byCard)
	{
		if (byCard >= CMjEnum::MJ_TYPE_FD)
		{
			return false;
		}
		if (byCard % 10 != 9 && byCard % 10 != 1)
		{
			return false;
		}
		return true;
	}
	//���2�����Ƿ�ͬһ��ɫ
	static bool CheckIsTongSe(BYTE Card1, BYTE Card2)
	{
		return (Card1 / 10 == Card2 / 10);
	}
	//���2�����Ƿ�һ����
	static  bool CheckIsYiXiangLian(BYTE Card1, BYTE Card2, bool big = false)
	{
		if (!CheckIsTongSe(Card1, Card2))
			return false;
		if (!big)
			return (Card1 == Card2 + 1);
		return (Card1 == Card2 - 1);
	}
	//���2�����Ƿ������
	static  bool CheckIsErXiangLian(BYTE Card1, BYTE Card2, bool big = false)
	{
		if (!CheckIsTongSe(Card1, Card2))
			return false;
		if (!big)
			return (Card1 == Card2 + 1 || Card1 == Card2 + 2);
		return (Card1 == Card2 - 1 || Card1 == Card2 - 2);
	}

	static BYTE ExDToB(BYTE byMJ)//�߼�����ת���ɿؼ�����
	{
		if (byMJ == 0)
		{
			return 34;
		}
		else if (byMJ <= 9)	//����ת��
		{
			return (byMJ - 1);
		}
		else if (byMJ <= 19)	//��ͬ��ת��
		{
			return (byMJ + 7);
		}
		else if (byMJ <= 29)	//������ת��
		{
			return (byMJ - 12);
		}
		else if (byMJ <= 37)	//���ơ����з��ס�ת��
		{
			if (byMJ == 31)//��
			{
				return 27;
			}
			else if (byMJ == 32)//��
			{
				return 29;
			}
			else if (byMJ == 33)//��
			{
				return 28;
			}
			else if (byMJ == 34)//��
			{
				return 30;
			}
			else
			{
				return byMJ - 4;
			}
		}
		else if (byMJ <= 48)	//�������ﶬ÷����ա�ת��
		{
			return (byMJ - 5);
		}
		return 255;
	}

	static BYTE ExBToD(BYTE byMJ)//�ؼ�����ת�����߼�����
	{
		if (byMJ < 0)
		{
			return byMJ;
		}
		else if (byMJ <= 8)	//����ת��
		{
			return (byMJ + 1);
		}
		else if (byMJ <= 17)	//������ת��
		{
			return (byMJ + 12);
		}
		else if (byMJ <= 26)	//��ͬ��ת��
		{
			return (byMJ - 7);
		}
		else if (byMJ <= 33)	//���ơ����з��ס�ת��
		{
			if (byMJ == 27)//��
			{
				return 31;
			}
			else if (byMJ == 29)//��
			{
				return 32;
			}
			else if (byMJ == 28)//��
			{
				return 33;
			}
			else if (byMJ == 30)//��
			{
				return 34;
			}
			else
			{
				return (byMJ + 4);
			}
		}
		else if (byMJ == 34) //�����뱳��
		{
			return 0;
		}
		else if (byMJ <= 43)	//�������ﶬ÷����ա�ת��
		{
			return (byMJ + 5);
		}
		return 255;
	}

	static bool ExDToB(BYTE* byT, const BYTE* byS, int nMJSize)//�߼��齫ת���ɿؼ��齫���ݣ�����ת����
	{
		if (!byT || !byS)
			return false;
		for (int i = 0; i < nMJSize; i++)
		{
			byT[i] = CMjRef::ExDToB(byS[i]);
		}
		return true;
	}
};
#endif