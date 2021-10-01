#include "stdafx.h"
#include "enemyManager.h"
#include "Mush.h"
#include "Beetle.h"
#include "Bee.h"
#include "FalseKnight.h"
#include "Mace.h"

HRESULT enemyManager::init()
{
	return S_OK;
}

void enemyManager::release()
{
	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end();) {
		if ((*_viEnemy) != NULL) {
			(*_viEnemy)->release();
			SAFE_DELETE((*_viEnemy));
			_viEnemy = _vEnemy.erase(_viEnemy);
		}
		else ++_viEnemy;
	}

	_vEnemy.clear();
}

void enemyManager::update()
{
	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end();) {
		(*_viEnemy)->update();
		(*_viEnemy)->updateRect();

		COLLISIONMANAGER->collisionEnemy((*_viEnemy));
		(*_viEnemy)->updateRect();

		if ((*_viEnemy)->isDeath()) {
			(*_viEnemy)->release();
			SAFE_DELETE(*_viEnemy);
			_viEnemy = _vEnemy.erase(_viEnemy);
		}
		else
			++_viEnemy;
	}
}

void enemyManager::render()
{
	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); ++_viEnemy)
		(*_viEnemy)->render();
}

void enemyManager::setMinion(int stage)
{
	if (stage < 0 || stage > 4) return;

	switch (stage) {
	case 0:
		setStage0();
		break;
	case 1:
		setStage1();
		break;
	case 2:
		setStage2();
		break;
	case 3:
		setStage3();
		break;
	case 4:
		setBoss();
		break;
	}
}

void enemyManager::setStage0()
{

}

void enemyManager::setStage1()
{
	_vEnemy.push_back(new Mush);
	_vEnemy.push_back(new Mush);
	_vEnemy.push_back(new Mush);

	_vEnemy[0]->init(PointMake(496, 1828 - MUSHHEIGHT / 2));
	_vEnemy[1]->init(PointMake(1887, 1917 - MUSHHEIGHT / 2));
	_vEnemy[2]->init(PointMake(3638, 1780 - MUSHHEIGHT / 2));
}

void enemyManager::setStage2()
{
	_vEnemy.push_back(new Bee);
	_vEnemy.push_back(new Bee);
	_vEnemy.push_back(new Bee);
	_vEnemy.push_back(new Bee);
	_vEnemy.push_back(new Bee);

	_vEnemy[0]->init(PointMake(603, 988));
	_vEnemy[1]->init(PointMake(1314, 864));
	_vEnemy[2]->init(PointMake(1878, 760));
	_vEnemy[3]->init(PointMake(2853, 940));
	_vEnemy[4]->init(PointMake(3369, 908));
}

void enemyManager::setStage3()
{
	_vEnemy.push_back(new Beetle);
	_vEnemy.push_back(new Beetle);
	_vEnemy.push_back(new Beetle);

	_vEnemy[0]->init(PointMake(1613, 1416 - BEETLEHEIGHT / 2));
	_vEnemy[1]->init(PointMake(2679, 1416 - BEETLEHEIGHT / 2));
	_vEnemy[2]->init(PointMake(412, 1467 - BEETLEHEIGHT / 2));
}

void enemyManager::setBoss()
{
	//_vEnemy.push_back(new FalseKnight);
	Mace* mc = new Mace;
	FalseKnight* fc = new FalseKnight;

	_vEnemy.push_back(mc);
	_vEnemy.push_back(fc);

	_vEnemy[0]->init(PointMake(2000, 1375 - BOSSHEIGHT / 2));
	_vEnemy[1]->init(PointMake(2000, 1375 - BOSSHEIGHT / 2));

	fc->equipMace(mc);

	//푸쉬백으로 에너미 생성해서 넣어줌
}

void enemyManager::addEnemy(Enemy * enemy)
{
	_vEnemy.push_back(enemy);
}

void enemyManager::removeEnemy(int arrNum)
{
	_vEnemy.erase(_vEnemy.begin() + arrNum);
}
