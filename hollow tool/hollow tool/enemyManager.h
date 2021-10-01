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

	virtual HRESULT init();			//초기화 함수
	virtual void release();			//메모리 해제 함수
	virtual void update();			//연산 함수
	virtual void render();			//그리기 함수

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

