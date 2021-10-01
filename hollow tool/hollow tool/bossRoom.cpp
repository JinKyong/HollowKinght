#include "stdafx.h"
#include "bossRoom.h"
#include "Player.h"

HRESULT bossRoom::init(Player* player)
{
	Scene::init(player);

	IMAGEMANAGER->addImage("boosRoom_back", "image/map/bossroom_sample.bmp", MAPWIDTH, MAPHEIGHT, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("boosRoom_front", "image/map/bossroom_front.bmp", MAPWIDTH, MAPHEIGHT, true, RGB(255, 0, 255));
	CAMERAMANAGER->setBackScreenSize(MAPWIDTH, MAPHEIGHT);

	//ÁöÇü
	_ground[0] = RectMake(0, 1375, 3000, 314);

	_floatingGround[0] = RectMake(0, 0, 250, 1375);
	_floatingGround[1] = RectMake(2687, 0, 313, 1375);
	
	// W : 3000,  H : 1689

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
		_em->setMinion(4);
		_resetEnemy = false;
	}

	COLLISIONMANAGER->init(this);

	return S_OK;
}

void bossRoom::release()
{
	_grounds.clear();
	_floatingGrounds.clear();
	_portals.clear();

	if (_em != nullptr)
		_em->release();
}

void bossRoom::update()
{
	_em->update();
	changeScene();
}

void bossRoom::render()
{
	IMAGEMANAGER->findImage("boosRoom_back")->render(getMemDC(), 0, 0);

	_em->render();
}

void bossRoom::renderFront()
{
	IMAGEMANAGER->findImage("boosRoom_front")->render(getMemDC(), 0, 0);

#ifdef _DEBUG
	if (PRINTMANAGER->isDebug()) {
		for (int i = 0; i < GROUNDNUM; ++i)
			Rectangle(getMemDC(), _grounds[i]);
		for (int i = 0; i < FGROUNDNUM; ++i)
			Rectangle(getMemDC(), _floatingGrounds[i]);
	}
#endif
}

void bossRoom::changeScene()
{
	float x = _player->getX();
	float y = _player->getY();

	if(y <= 0){
		CAMERAMANAGER->setFade(FADEOUT);
		_player->move(UP);

		if (CAMERAMANAGER->getAlpha() == 255) {
			SCENEMANAGER->changeScene("stage2");

			_player->setX(1937);
			_player->setY(1300);
		}
	}
}
