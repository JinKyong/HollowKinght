#include "stdafx.h"
#include "stage0.h"
#include "Player.h"

HRESULT stage0::init(Player* player)
{
	Scene::init(player);

	IMAGEMANAGER->addImage("stage0_back", "image/map/stage0_sample.bmp", MAPWIDTH, MAPHEIGHT, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("stage0_front", "image/map/stage0_front.bmp", MAPWIDTH, MAPHEIGHT, true, RGB(255, 0, 255));
	CAMERAMANAGER->setBackScreenSize(MAPWIDTH, MAPHEIGHT);

	//ÁöÇü
	_ground[0] = RectMake(0, 2478, 1958, 522);
	_ground[1] = RectMake(0, 0, 273, 2478);
	_ground[2] = RectMake(1816, 0, 142, 2300);

	for (int i = 0; i < GROUNDNUM; ++i)
		_grounds.push_back(_ground[i]);

	//enemyManager
	if (_em == nullptr) {
		_em = new enemyManager;
		_em->init();
	}

	if (_resetEnemy) {
		_em->release();
		//_em->setMinion(0);
		_resetEnemy = true;
	}


	COLLISIONMANAGER->init(this);

	return S_OK;
}

void stage0::release()
{
	_grounds.clear();
	_floatingGrounds.clear();
	_portals.clear();

	if (_em != nullptr)
		_em->release();
}

void stage0::update()
{
	_em->update();

	changeScene();
}

void stage0::render()
{
	IMAGEMANAGER->findImage("stage0_back")->render(getMemDC(), 0, 0);

	_em->render();
}

void stage0::renderFront()
{
	IMAGEMANAGER->findImage("stage0_front")->render(getMemDC(), 0, 0);

#ifdef _DEBUG
	if (PRINTMANAGER->isDebug()) {
		for (int i = 0; i < GROUNDNUM; ++i)
			Rectangle(getMemDC(), _grounds[i]);
	}
#endif

}

void stage0::changeScene()
{
	float x = _player->getX();
	float y = _player->getY();

	if (x >= CAMERAMANAGER->getBackScreenWidth()) {
		CAMERAMANAGER->setFade(FADEOUT);
		_player->move(RIGHT);

		if (CAMERAMANAGER->getAlpha() == 255) {
			SCENEMANAGER->changeScene("dartmount");

			_player->setX(100);
			_player->setY(873);
		}
	}
}
