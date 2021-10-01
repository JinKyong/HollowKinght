#include "stdafx.h"
#include "stage3.h"
#include "Player.h"

HRESULT stage3::init(Player* player)
{
	Scene::init(player);

	IMAGEMANAGER->addImage("stage3_back", "image/map/stage3_sample.bmp", MAPWIDTH, MAPHEIGHT, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("stage3_front", "image/map/stage3_front.bmp", MAPWIDTH, MAPHEIGHT, true, RGB(255, 0, 255));
	CAMERAMANAGER->setBackScreenSize(MAPWIDTH, MAPHEIGHT);

	//ÁöÇü
	_ground[0] = RectMake(0, 0, 648, 778);
	_ground[1] = RectMake(0, 0, 3725, 345);
	_ground[2] = RectMake(0, 1467, 1250, 233);
	_ground[3] = RectMake(1250, 1416, 2475, 284);
	_ground[4] = RectMake(3029, 0, 695, 765);


	_floatingGround[0] = RectMake(0, 1312, 216, 388);
	_floatingGround[1] = RectMake(809, 1256, 426, 445);
	_floatingGround[2] = RectMake(917, 772, 487, 113);
	_floatingGround[3] = RectMake(1507, 930, 114, 111);
	_floatingGround[4] = RectMake(1685, 1154, 211, 56);
	_floatingGround[5] = RectMake(2021, 989, 107, 103);
	_floatingGround[6] = RectMake(2625, 774, 477, 161);
	_floatingGround[7] = RectMake(3269, 1252, 456, 448);
	// W : 3725, H : 1700

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
		_em->setMinion(3);
		_resetEnemy = true;
	}

	if(!INVENTORYMANAGER->wing())
		ITEMMANAGER->dropWing(2782, 774);
	COLLISIONMANAGER->init(this);

	return S_OK;
}

void stage3::release()
{
	_grounds.clear();
	_floatingGrounds.clear();
	_portals.clear();

	if (_em != nullptr)
		_em->release();
}

void stage3::update()
{
	_em->update();

	changeScene();
}

void stage3::render()
{
	IMAGEMANAGER->findImage("stage3_back")->render(getMemDC(), 0, 0);

	_em->render();
}

void stage3::renderFront()
{
	IMAGEMANAGER->findImage("stage3_front")->render(getMemDC(), 0, 0);

#ifdef _DEBUG
	if (PRINTMANAGER->isDebug()) {
		for (int i = 0; i < GROUNDNUM; ++i)
			Rectangle(getMemDC(), _grounds[i]);
		for (int i = 0; i < FGROUNDNUM; ++i)
			Rectangle(getMemDC(), _floatingGrounds[i]);
	}
#endif
}

void stage3::changeScene()
{
	float x = _player->getX();
	float y = _player->getY();

	if (x >= CAMERAMANAGER->getBackScreenWidth()) {
		CAMERAMANAGER->setFade(FADEOUT);
		_player->move(RIGHT);

		if (CAMERAMANAGER->getAlpha() == 255) {
			SCENEMANAGER->changeScene("stage1");

			_player->setX(100);
			_player->setY(1563);
		}
	}
}
