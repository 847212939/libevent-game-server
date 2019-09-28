#ifndef CHECKHUPAI_H
#define CHECKHUPAI_H
#include "GameDataEx.h"
#include "CLogicBase.h"
#include "mj.h"

///���Ƽ����
class CheckHuPai
{
public:
	///�������ṹ����
	PingHuStruct	m_tHuPaiStruct;
	///�������ṹ����
	PingHuStruct	m_tTempHupaistruct;
	///���Ƽ����ʱ����
	CheckHuStruct	m_tHuTempData;
	///���Ƽ����ʱ����(�޲���)
	CheckHuStruct	m_tNoJingHuTempData;
	///��ǰ�����ҵ�λ��
	BYTE            m_byStation;
	///����Ƿ���������
	bool            m_bZimo;
	///��������
	BYTE			m_byJingNum;
	///��������
	BYTE			m_byHuType[MAX_HUPAI_TYPE];
	//��ǰ�����ֵ
	int				m_iMaxFan;
	//�����������ڵ���
	BYTE			m_byLastPai;
	//�������λ��
	BYTE			m_byBeStation;
	//���Ʋ�����20�ţ�0Ϊ������־
	BYTE			m_byArHandPai[HAND_CARD_NUM];
	///��������
	BYTE		   m_byShunNum;			//˳�ӵĸ���
	BYTE           m_byShunData[MAX_CPG_NUM];//˳�ӵ�����
	BYTE		   m_byKeziNum;			//���ӵĸ���
	BYTE           m_byKeziData[MAX_CPG_NUM];//���ӵ�����
	BYTE		   m_byGangNum;			//���Ƶĸ���
	BYTE           m_byGangData[MAX_CPG_NUM][2];//���Ƶ�����
	BYTE		   m_byJiang;			//��������
	BYTE		   m_byChiNum;			//���Ƶĸ���
	BYTE		   m_byPengNum;			//���Ƶĸ���
	BYTE           m_byTypeFans[MAX_HUPAI_TYPE];//���ͷ���	

	BYTE		   m_byHandShunNum;			//����˳�ӵĸ�������������
	BYTE           m_byHandShunData[MAX_CPG_NUM];//˳�ӵ����ݣ���������
public:
	CheckHuPai(void);
	~CheckHuPai(void);
	///��ʼ������
	virtual void InitData();

	//////////////////////////////////////////////////////////////////////////ƽ��
	//��ȡ��������
	///ƽ������ȡ�����������
	virtual bool GetHuPaiTypes(GameDataEx *pGameData, bool zimo);

	///����ܷ�����
	virtual bool CanTing(GameDataEx *pGameData, BYTE station, BYTE CanOut[]);

	///����ܷ�����ƣ��Ժ�����
	virtual bool ChiTing(GameDataEx *pGameData, BYTE station, BYTE CanOut[], BYTE pai[][3]);

	///���ܺ��ܷ񱣴�����״̬
	virtual bool GangTing(GameDataEx *pGameData, BYTE station, tagCPGNotify &cpg, BYTE ps = 255, bool CanTing[] = NULL);

	//������
	virtual bool CanHu(GameDataEx *pGameData, BYTE station, BYTE lastpai, bool zimo);

	///�����������ݼ��뵽���ƽṹ��
	virtual void AddGCPData(TCPGStruct gcpData[], PingHuStruct * tempstruct);

	///ת���������ݵĴ洢��ʽ��Ϊ�˷�����Ƽ��
	virtual void ChangeHandPaiData(GameDataEx *pGameData, BYTE handpai[]);

	///ת���������ݵĴ洢��ʽ��Ϊ�˷�����Ƽ��
	virtual void ChangeHandPaiDataEx(GameDataEx *pGameData, BYTE handpai[]);

	///���һ���Ƶ���ʱ������
	virtual void AddAPaiToArr(BYTE pai);

	///ƽ�����
	virtual bool CheckPingHu(GameDataEx *pGameData);

	///���þ���
	virtual void SetJingPai();

	///�ָ�����
	virtual void ReSetJingPai();

	///ƽ������
	virtual bool MakePingHu(GameDataEx *pGameData, CheckHuStruct &PaiData, PingHuStruct &m_tHuPaiStruct, int iJingnum = 0);

	///�ָ�����,�Ѽ������ƻ�ԭ
	virtual void ReSetPaiNum(BYTE num[], CheckHuStruct &TempPai);

	///��������Ƿ�ͬһ��ɫ
	virtual bool CheckTongSe(BYTE pai1, BYTE pai2);

	///��������Ƿ�����
	virtual bool CheckLianPai(GameDataEx *pGameData, BYTE pai1, BYTE pai2, BYTE num = 1);

	///�����������������
	virtual void ClearPingHuData();

	///���ƽ��������ϣ����ƣ��ԣ����ܣ�˳�ӣ����̵�
	virtual void CheckPingHuData(GameDataEx *pGameData);

	///���һ�ֺ�������
	virtual void SetAHupaiType(BYTE type, BYTE hupai[]);

	///����һ�ֺ�������
	virtual void ReSetAHupaiType(BYTE type, BYTE hupai[]);

	///������������к�������
	virtual void ClearHupaiType(BYTE hupai[]);

	///����Ƿ����ĳ�ֺ�������
	virtual bool CheckHupaiType(BYTE type, BYTE hupai[]);

	/////////////////��ϸ�������ͼ��///////////////////////////////////////
	virtual bool CheckTianHu(GameDataEx *pGameData);//���
	virtual bool CheckDiHu(GameDataEx *pGameData);//�غ�	

	//88�����ͣ�5�֣�
	virtual bool CheckDaSiXi();//����ϲ
	virtual bool CheckDaSanYuan();//����Ԫ
	virtual bool CheckSiGang();//�ĸܣ�ʮ���޺���
	virtual bool CheckJiuLianBaoDeng(GameDataEx *pGameData);//��������	
	virtual bool CheckLianQiDui(GameDataEx *pGameData);//���߶ԣ��߶Ե����⣩

	//64�����ͣ�5�֣�
	virtual bool CheckXiaoSiXi();//С��ϲ
	virtual bool CheckXiaoSanYuan();//С��Ԫ	
	virtual bool CheckZiYiSe(GameDataEx *pGameData);//��һɫ	
	virtual bool CheckSiAnKe(GameDataEx *pGameData);//�İ���
	virtual bool CheckYiSeShuangLongHui();//һɫ˫����	

	//48�����ͣ�2�֣�
	virtual bool CheckYiSeSiTongShun();//һɫ��ͬ˳
	virtual bool CheckYiSeSiJieGao();//һɫ�Ľڸ�	

	//32�����ͣ�3�֣�
	virtual bool CheckYiSeSiBuGao();//һɫ�Ĳ���	
	virtual bool CheckHunYaoJiu(GameDataEx *pGameData);//���۾�	
	virtual bool CheckSanGang();//����

	//24�����ͣ�4�֣�
	virtual bool CheckQiDui(GameDataEx *pGameData);//�߶�
	virtual bool CheckLongQiDui(GameDataEx *pGameData);//���߶�
	virtual bool CheckQingYiSeQiDui(GameDataEx *pGameData);//��һɫ�߶�
	virtual bool CheckQingYiSe(GameDataEx *pGameData);//��һɫ	 
	virtual bool CheckQingYiSeYiTiaoLong(GameDataEx *pGameData);//��һɫһ����	 
	virtual bool CheckYiSeSanTongShun();//һɫ��ͬ˳	
	virtual bool CheckYiSeSanJieGao();//һɫ���ڸ�	

	//16�����ͣ�3�֣�
	virtual bool CheckQingLong();//����	
	virtual bool CheckYiSeSanBuGao();//һɫ������	
	virtual bool CheckSanAnKe();//������

	//12�����ͣ�3�֣�
	virtual bool CheckDaYuWu(GameDataEx *pGameData);//������	
	virtual bool CheckXiaoYuWu(GameDataEx *pGameData);//С����	
	virtual bool CheckSanFengKe();//�����	

	//8�����ͣ�4�֣�
	virtual bool CheckMiaoShouHuiChun(GameDataEx *pGameData);//���ֻش�	
	virtual bool CheckHaiDiLaoYue(GameDataEx *pGameData);//��������	

	virtual bool CheckGangKai(GameDataEx *pGameData);//���ϻ�
	virtual bool CheckQiangGangHe(GameDataEx *pGameData);//���ܺ�	

	//6�����ͣ�5�֣�
	virtual bool CheckPengPengHu(GameDataEx *pGameData);//������	
	virtual bool CheckHunYiSe(GameDataEx *pGameData);//��һɫ	
	virtual bool CheckQuanQiuRen(GameDataEx *pGameData);//ȫ����	
	virtual bool CheckShuangAnKe();//˫����	
	virtual bool CheckShuangJianKe();//˫����	

	//4�����ͣ�4�֣�
	virtual bool CheckQuanDaiYao(GameDataEx *pGameData);//ȫ����	
	virtual bool CheckBuQiuRen(GameDataEx *pGameData);//������	
	virtual bool CheckShuangMingGang();//˫����	
	virtual bool CheckHuJueZhang();//�;���		

	//2�����ͣ�7�֣�
	virtual bool CheckJianKe();//����
	virtual bool CheckMenQianQing(GameDataEx *pGameData);//��ǰ��	
	virtual bool CheckSiGuiYi(GameDataEx *pGameData);//�Ĺ�һ	
	virtual bool CheckShuangAnGang();//˫����	
	virtual bool CheckAnGang();//����	
	virtual bool CheckDuanYao(GameDataEx *pGameData);//����	

	//1�����ͣ�10�֣�
	virtual bool CheckYiBanGao();//һ���	
	virtual bool CheckLianLiu();//����	
	virtual bool CheckLaoShaoFu();//���ٸ�	
	virtual bool CheckYaoJiuKe();//�۾ſ�	
	virtual bool CheckMingGang();//����	
	virtual bool CheckBianZhang(GameDataEx *pGameData);//����	
	virtual bool CheckKanZhang(GameDataEx *pGameData);//����	
	virtual bool CheckDanDiaoJiang(GameDataEx *pGameData);//������	

	//////////////////����������ȸ���в��㷬////////////////////////////////
	virtual bool CheckGangHouPao(GameDataEx *pGameData);//�ܺ���
	virtual bool CheckXiXiangFeng();//ϲ���
	virtual bool CheckQueYiMen();//ȱһ��	
	virtual bool CheckWuZi();//����
	virtual bool CheckQuanFengKe(GameDataEx *pGameData);//Ȧ���	
	virtual bool CheckMenFengKe();//�ŷ��	
	virtual bool CheckShuangTongKe();//˫ͬ��	
	virtual int  CheckYiSe(GameDataEx *pGameData);//һɫ�Ƽ�⣨������ƺͳ������ƣ����ﶼ���ã���0����һɫ�ƣ�1��һɫ��2��һɫ��	
	virtual bool CheckSanSeSanBuGao();//��ɫ������	
	virtual bool CheckWuMenQi(GameDataEx *pGameData);//������	
	virtual bool CheckHuaLong();//����	
	virtual bool CheckTuiBuDao(GameDataEx *pGameData);//�Ʋ���	
	virtual bool CheckSanSeSanTongShun();//��ɫ��ͬ˳	
	virtual bool CheckSanSeSanJieGao();//��ɫ���ڸ�	
	virtual bool CheckWuFanHe();//�޷���	
	virtual bool CheckSanSeShuangLongHui();//��ɫ˫����	
	virtual bool CheckQuanDaiWu(GameDataEx *pGameData);	//ȫ����
	virtual bool CheckSanTongKe();//��ͬ��		
	virtual bool CheckQuanShuangKe();//ȫ˫��	
	virtual bool CheckQuanDa(GameDataEx *pGameData);//ȫ��	
	virtual bool CheckQuanZhong(GameDataEx *pGameData);//ȫ��	
	virtual bool CheckQuanXiao(GameDataEx *pGameData);//ȫС		
	virtual bool CheckQingYaoJiu(GameDataEx *pGameData);//���۾�	
	virtual bool CheckLvYiSe(GameDataEx *pGameData);//��һɫ	

	/////////////////////////��������////////////////////////////////////////////////////////////
	virtual bool CheckQuanBuKao(GameDataEx *pGameData);	//ȫ��������ʮ���ã�

	virtual bool CheckZuHeLong(GameDataEx *pGameData);//�����	

	virtual bool CheckQiXingBuKao(GameDataEx *pGameData);//���ǲ���������ʮ���ã�	

	virtual bool CheckShiSanYao(GameDataEx *pGameData);//ʮ����	

	virtual bool CheckDaPiao(GameDataEx *pGameData);//��Ʈ(Դ�Խ����齫)	

	virtual bool CheckJiangDui(GameDataEx *pGameData);//��⽫�ԣ�258��������

	virtual bool CheckYiJiuHu(GameDataEx *pGameData);//���۾�

	virtual bool CheckQingYiJiuHu(GameDataEx *pGameData);//���۾�

	bool QiangQiangHu258(GameDataEx *pGameData);//��һɫ��������ȫ����258ʱ��������

	virtual bool CheckYiTiaoLong();//һ������������ʱ�����в�ͬ��ɫ��123456789˳�ӳ�Ϊһ����
};
#endif