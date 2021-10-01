#include "stdafx.h"
#include "playGround.h"

playGround::playGround()
{
}


playGround::~playGround()
{
}

//초기화는 여기다 하세요 제발
HRESULT playGround::init()
{
	gameNode::init(true);

	INVENTORYMANAGER->init();

	_player = new Player;
	_player->init();

	SCENEMANAGER->init(_player);
	SCENEMANAGER->changeScene("stage0");

	UIMANAGER->init(_player);

	_isStart = true;
	_debug = false;

	return S_OK;
}

HRESULT playGround::init(const char * str)
{
	gameNode::init(true);

	INVENTORYMANAGER->init();

	_player = new Player;
	_player->init();

	SCENEMANAGER->init(_player);
	SCENEMANAGER->changeScene(str);

	UIMANAGER->init(_player);

	_debug = false;

	return S_OK;
}

//메모리 해제는 여기다 하세요 제발
void playGround::release()
{
	gameNode::release();

	INVENTORYMANAGER->release();
	INVENTORYMANAGER->releaseSingleton();

	_player->release();
	SAFE_DELETE(_player);

	SCENEMANAGER->release();
	SCENEMANAGER->releaseSingleton();

	UIMANAGER->release();
	UIMANAGER->releaseSingleton();
}


void playGround::update()
{
	gameNode::update();

	if (KEYMANAGER->isOnceKeyDown(VK_TAB)) {
		_debug = !_debug;
		PRINTMANAGER->setDebug(_debug);
	}
	if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
		_isStart = !_isStart;

	if (_isStart) {
		if (CAMERAMANAGER->getFade() >= 0)
			_player->update();

		/*
		if (_player->isDeath()) {
			_player->release();
			this->init("dartmount");
		}*/

		SCENEMANAGER->update();
		//if(_player->getStun())
			//CAMERAMANAGER->vibrateScreenOut(_player->getX(), _player->getY(), 10);
		//else
		CAMERAMANAGER->updateScreen(_player->getX(), _player->getY());
	}
}


void playGround::render()
{
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
	//================제발 이 사이에 좀 그립시다==========================

	//글자 배경색 모드
	SetBkMode(getMemDC(), TRANSPARENT);
	SetTextColor(getMemDC(), RGB(255, 255, 255));

	HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(getMemDC(), myBrush);

	HPEN myPen = (HPEN)CreatePen(1, 2, RGB(255, 0, 0));
	SelectObject(getMemDC(), myPen);

	SCENEMANAGER->render();
	_player->render();
	ITEMMANAGER->render(getMemDC());
	SCENEMANAGER->renderFront();

	UIMANAGER->render(getMemDC());

	if(INVENTORYMANAGER->isOpen())
		INVENTORYMANAGER->render(getMemDC());

	SelectObject(getMemDC(), oldBrush);
	DeleteObject(myPen);
	DeleteObject(myBrush);

	//==================================================
	//this->getBackBuffer()->render(getHDC(), 0, 0);
	CAMERAMANAGER->render(getHDC(), 0, 0, getMemDC());
}