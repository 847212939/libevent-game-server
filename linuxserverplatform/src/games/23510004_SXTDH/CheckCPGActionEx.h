#ifndef CHECKCPGACTIONEX_H
#define CHECKCPGACTIONEX_H
#include "CheckCPGAction.h"
#include "CheckHuPaiEx.h"
///�����ܼ����(����)
class CheckCPGActionEx :public CheckCPGAction
{
public:
	CheckCPGActionEx(void);
	~CheckCPGActionEx(void);
public:
	///��ʼ������
	virtual void InitData();

	///�ܷ�����
	virtual bool CanPeng(GameDataEx *pGameData, BYTE station, BYTE pai);

	///�ܷ����
	bool CanGang(GameDataEx *pGameData, BYTE station, BYTE pai, BYTE Result[][2], bool bMe = false, CheckHuPaiEx *pCheckHuData = NULL);

	//�������
	virtual void CountGangFen(GameDataEx *pGameData, int iStation, int iBeStation, BYTE byType);

	//����ܷ�
	void CountGangFenEx(GameDataEx *pGameData);

	//��ȥ���ܺ����øܷ�,�����ܷ�
	void MinusGangFen(GameDataEx *pGameData, int iStation, int iBeStation);
};
#endif