#pragma once
//#include "gameNode.h"
#include "Enemy.h"
//#include <vector>

class itemManager;

class enemyManager : public gameNode
{
private:
	typedef vector<Enemy*>				enemyList;
	typedef vector<Enemy*>::iterator	enemyListIter;

private:
	enemyList		_vEnemy;
	enemyListIter	_viEnemy;

public:
	enemyManager() {};
	~enemyManager() {};

	virtual HRESULT init();			//�ʱ�ȭ �Լ�
	virtual void release();			//�޸� ���� �Լ�
	virtual void update();			//���� �Լ�
	virtual void render();			//�׸��� �Լ�

	//virtual void setEnemy();
	void setMinion(int stage);
	void setStage0();
	void setStage1();
	void setStage2();
	void setStage3();
	void setBoss();

	void addEnemy(Enemy* enemy);
	void removeEnemy(int arrNum);

	enemyList getvEnemy() { return _vEnemy; }
	enemyListIter getviEnemy() { return _viEnemy; }
};

