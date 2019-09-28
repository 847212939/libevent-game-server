#ifndef CHECKCPGACTION_H
#define CHECKCPGACTION_H
#include "GameDataEx.h"
#include "mj.h"
///�����ܼ����
class CheckCPGAction
{
public:
	CheckCPGAction(void);
	~CheckCPGAction(void);
public:
	///��ʼ������
	virtual void InitData();

	///�ܷ����
	virtual bool CanChi(GameDataEx *pGameData, BYTE station, BYTE pai, BYTE Result[][3], bool bChi[3]);

	///�ܷ�����
	virtual bool CanPeng(GameDataEx *pGameData, BYTE station, BYTE pai);

	//�ܷ���
	virtual bool CanKan(GameDataEx *pGameData, BYTE station, BYTE kan[]);

	///�ܷ����
	virtual bool CanGang(GameDataEx *pGameData, BYTE station, BYTE pai, BYTE Result[][2], bool bMe = false);
};
#endif