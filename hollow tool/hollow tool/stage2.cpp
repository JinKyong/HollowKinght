#include "stdafx.h"
#include "stage2.h"
#include "Player.h"

HRESULT stage2::init(Player* player)
{
	Scene::init(player);

	IMAGEMANAGER->addImage("stage2_back", "image/map/stage2_sample.bmp", MAPWIDTH, MAPHEIGHT, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("stage2_front", "image/map/stage2_front.bmp", MAPWIDTH, MAPHEIGHT, true, RGB(255, 0, 255));
	CAMERAMANAGER->setBackScreenSize(MAPWIDTH, MAPHEIGHT);

	//ÁöÇü
	_ground[0] = RectMake(0, 1408, 3793, 792);
	_ground[1] = RectMake(0, 0, 3793, 250);
	_ground[2] = RectMake(0, 250, 102, 858);
	_ground[3] = RectMake(3647, 250, 146, 992);

	_portal = RectMake(1899, 1248, 100, 160);

	// W : 3793, H : 2200

	for (int i = 0; i < GROUNDNUM; ++i)
		_grounds.push_back(_ground[i]);
	_portals.push_back(_portal);

	//enemyManager
	if (_em == nullptr) {
		_em = new enemyManager;
		_em->init();
	}

	if (_resetEnemy) {
		_em->release();
		_em->setMinion(2);
		_resetEnemy = true;
	}

	COLLISIONMANAGER->init(this);

	return S_OK;
}

void stage2::release()
{
	_grounds.clear();
	_floatingGrounds.clear();
	_portals.clear();

	if (_em != nullptr)
		_em->release();
}

void stage2::update()
{
	_em->update();

	changeScene();
}

void stage2::render()
{
	IMAGEMANAGER->findImage("stage2_back")->render(getMemDC(), 0, 0);

	_em->render();
}

void stage2::renderFront()
{
	IMAGEMANAGER->findImage("stage2_front")->render(getMemDC(), 0, 0);

#ifdef _DEBUG
	if (PRINTMANAGER->isDebug()) {
		for (int i = 0; i < GROUNDNUM; ++i)
			Rectangle(getMemDC(), _grounds[i]);
		Rectangle(getMemDC(), _portal);
	}
#endif

}

void stage2::changeScene()
{
	float x = _player->getX();
	float y = _player->getY();

	if (x <= 0) {
		CAMERAMANAGER->setFade(FADEOUT);
		_player->move(LEFT);

		if (CAMERAMANAGER->getAlpha() == 255) {
			SCENEMANAGER->changeScene("stage1");

			_player->setX(CAMERAMANAGER->getBackScreenWidth() - 100);
			_player->setY(1335);
		}
	}
	else if (_player->isGetIn()) {
		CAMERAMANAGER->setFade(FADEOUT);

		if (CAMERAMANAGER->getAlpha() == 255) {
			SCENEMANAGER->changeScene("bossroom");
			_player->getOut();

			_player->setX(1000);
			_player->setY(100);
		}
	}
}
