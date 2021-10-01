#include "stdafx.h"
#include "stage1.h"
#include "Player.h"

HRESULT stage1::init(Player* player)
{
	Scene::init(player);

	IMAGEMANAGER->addImage("stage1_back", "image/map/stage1_sample.bmp", MAPWIDTH, MAPHEIGHT, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("stage1_front", "image/map/stage1_front.bmp", MAPWIDTH, MAPHEIGHT, true, RGB(255, 0, 255));
	CAMERAMANAGER->setBackScreenSize(MAPWIDTH, MAPHEIGHT);

	//ÁöÇü
	_ground[0] = RectMake(0, 0, 1650, 1000);
	_ground[1] = RectMake(0, 1917, 4519, 683);
	_ground[2] = RectMake(2837, 0, 1682, 841);


	_floatingGround[0] = RectMake(0, 1000, 153, 500);
	_floatingGround[1] = RectMake(0, 1648, 204, 269);

	_floatingGround[2] = RectMake(204, 1828, 1552, 89);
	_floatingGround[3] = RectMake(750, 1736, 1006, 92);
	_floatingGround[4] = RectMake(824, 1556, 850, 180);

	_floatingGround[5] = RectMake(1650, 0, 367, 388);
	_floatingGround[6] = RectMake(2017, 0, 332, 485);
	_floatingGround[7] = RectMake(1650, 617, 699, 495);
	_floatingGround[8] = RectMake(2059, 1112, 290, 137);
	_floatingGround[9] = RectMake(2568, 0, 269, 1248);

	_floatingGround[10] = RectMake(2278, 1780, 2241, 137);
	_floatingGround[11] = RectMake(2278, 1689, 1154, 91);
	_floatingGround[12] = RectMake(1874, 1512, 404, 160);
	_floatingGround[13] = RectMake(2278, 1512, 653, 177);

	_floatingGround[14] = RectMake(4236, 1416, 283, 364);

	_floatingGround[15] = RectMake(339, 1510, 275, 96);
	_floatingGround[16] = RectMake(3123, 1509, 121, 36);
	_floatingGround[17] = RectMake(3486, 1470, 200, 50);
	_floatingGround[18] = RectMake(3898, 1452, 118, 38);

	// W : 4519, H : 2600

	for (int i = 0; i < GROUNDNUM; ++i)
		_grounds.push_back(_ground[i]);
	for (int i = 0; i < FGROUNDNUM; ++i)
		_floatingGrounds.push_back(_floatingGround[i]);

	//enemyManager
	if (_em == nullptr) {
		_em = new enemyManager;
		_em->init();
	}

	if (_resetEnemy) {
		_em->release();
		_em->setMinion(1);
		_resetEnemy = true;
	}

	COLLISIONMANAGER->init(this);

	return S_OK;
}

void stage1::release()
{
	_grounds.clear();
	_floatingGrounds.clear();
	_portals.clear();

	if(_em != nullptr)
		_em->release();
}

void stage1::update()
{
	_em->update();

	changeScene();
}

void stage1::render()
{
	IMAGEMANAGER->findImage("stage1_back")->render(getMemDC(), 0, 0);

	_em->render();
}

void stage1::renderFront()
{
	IMAGEMANAGER->findImage("stage1_front")->render(getMemDC(), 0, 0);

#ifdef _DEBUG
	if (PRINTMANAGER->isDebug()) {
		for (int i = 0; i < GROUNDNUM; ++i)
			Rectangle(getMemDC(), _grounds[i]);
		for (int i = 0; i < FGROUNDNUM; ++i)
			Rectangle(getMemDC(), _floatingGrounds[i]);
	}
#endif
}

void stage1::changeScene()
{
	float x = _player->getX();
	float y = _player->getY();

	if (y <= 0) {
		CAMERAMANAGER->setFade(FADEOUT);
		_player->move(UP);

		if (CAMERAMANAGER->getAlpha() == 255) {
			SCENEMANAGER->changeScene("dartmount");

			_player->setX(3500);
			_player->setY(2000);
		}
	}
	else if (x <= 0) {
		CAMERAMANAGER->setFade(FADEOUT);
		_player->move(LEFT);

		if (CAMERAMANAGER->getAlpha() == 255) {
			SCENEMANAGER->changeScene("stage3");

			_player->setX(CAMERAMANAGER->getBackScreenWidth() - 100);
			_player->setY(1167);
		}
	}
	else if (x >= CAMERAMANAGER->getBackScreenWidth()) {
		CAMERAMANAGER->setFade(FADEOUT);
		_player->move(RIGHT);

		if (CAMERAMANAGER->getAlpha() == 255) {
			SCENEMANAGER->changeScene("stage2");

			_player->setX(100);
			_player->setY(1167);
		}
	}
}
