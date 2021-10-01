#include "stdafx.h"
#include "dartMount.h"
#include "Player.h"

HRESULT dartMount::init(Player* player)
{
	Scene::init(player);

	IMAGEMANAGER->addImage("dartmount_back", "image/map/dartmount_sample.bmp", MAPWIDTH, MAPHEIGHT, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("dartmount_front", "image/map/dartmount_front.bmp", MAPWIDTH, MAPHEIGHT, true, RGB(255, 0, 255));
	CAMERAMANAGER->setBackScreenSize(MAPWIDTH, MAPHEIGHT);

	//ÁöÇü
	_ground[0] = RectMake(0, 0, 207, 500);
	_ground[1] = RectMake(207, 0, 140, 422);
	_ground[2] = RectMake(0, 2142, 2910, 358);
	_ground[3] = RectMake(2310, 2178, 748, 322);
	_ground[4] = RectMake(2458, 2215, 750, 285);
	_ground[5] = RectMake(3991, 1172, 157, 1328);

	//_floatingGround[0] = RectMake(0, 960, 412, 56);
	_floatingGround[0] = RectMake(0, 960, 415, 1182);
	_floatingGround[1] = RectMake(3208, 2249, 258, 251);
	_floatingGround[2] = RectMake(3603, 2246, 545, 254);

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
		//_em->setMinion(0);
		_resetEnemy = false;
	}

	COLLISIONMANAGER->init(this);

	return S_OK;
}

void dartMount::release()
{
	_grounds.clear();
	_floatingGrounds.clear();
	_portals.clear();

	if (_em != nullptr)
		_em->release();
}

void dartMount::update()
{
	changeScene();
}

void dartMount::render()
{
	IMAGEMANAGER->findImage("dartmount_back")->render(getMemDC(), 0, 0);
}

void dartMount::renderFront()
{
	IMAGEMANAGER->findImage("dartmount_front")->render(getMemDC(), 0, 0);

#ifdef _DEBUG
	if (PRINTMANAGER->isDebug()) {
		for (int i = 0; i < GROUNDNUM; ++i)
			Rectangle(getMemDC(), _grounds[i]);
		for (int i = 0; i < FGROUNDNUM; ++i)
			Rectangle(getMemDC(), _floatingGrounds[i]);
	}
#endif
}

void dartMount::changeScene()
{
	float x = _player->getX();
	float y = _player->getY();

	if (x <= 0) {
		CAMERAMANAGER->setFade(FADEOUT);
		_player->move(LEFT);

		if (CAMERAMANAGER->getAlpha() == 255) {
			SCENEMANAGER->changeScene("stage0");

			_player->setX(CAMERAMANAGER->getBackScreenWidth() - 100);
			_player->setY(2390);
		}
	}
	else if (y >= CAMERAMANAGER->getBackScreenHeight()) {
		CAMERAMANAGER->setFade(FADEOUT);
		_player->move(DOWN);

		if (CAMERAMANAGER->getAlpha() == 255) {
			SCENEMANAGER->changeScene("stage1");

			_player->setX(2460);
			_player->setY(100);
		}
	}
}
