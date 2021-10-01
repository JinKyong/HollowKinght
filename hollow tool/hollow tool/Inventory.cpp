#include "stdafx.h"
#include "Inventory.h"
#include "Player.h"

HRESULT Inventory::init(Player * player, bool reverse)
{
	State::init(player, reverse);

	_img = IMAGEMANAGER->addFrameImage("idle", "image/knight/idle.bmp",
		360, 254, 6, 2, true, RGB(255, 0, 255), true);
	_count = 0;
	_direct = _player->getDirect();

	_reverse = reverse;

	_img->setFrameY(_direct);

	return S_OK;
}

void Inventory::release()
{
}

void Inventory::update()
{
	controlKey();
	controlFrame();

	updateRect();
}

void Inventory::render(HDC hdc)
{
	RECT rc = _player->getHead();

	if (_player->isStun()) {
		if (_direct)
			_img->frameAlphaRender(hdc, rc.left, rc.top, _player->getAlpha());
		else
			_img->frameAlphaRender(hdc, rc.right - _img->getFrameWidth(), rc.top, _player->getAlpha());
	}
	else {
		if (_direct)
			_img->frameRender(hdc, rc.left, rc.top);
		else
			_img->frameRender(hdc, rc.right - _img->getFrameWidth(), rc.top);
	}

#ifdef _DEBUG
	char str[128];
	sprintf_s(str, "state : INVENTORY");
	TextOut(hdc, rc.right + 10, rc.top, str, strlen(str));
	sprintf_s(str, "index : %d", INVENTORYMANAGER->getIndex());
	TextOut(hdc, rc.right + 10, rc.top + 20, str, strlen(str));

	/*PRINTMANAGER->print(hdc, rc.right + 10, rc.top, "state : IDLE");
	PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 20, "index : %d", _img->getFrameX());
	PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 60, "jumpCount : %d", _player->getJumpCount());*/
#endif // _DEBUG
}

void Inventory::updateDirect(int direct)
{
}

void Inventory::updateRect()
{
	float x = _player->getX();
	float y = _player->getY();
	float correct;

	//머리
	_player->setHead(x, y, HWIDTH, HHEIGHT);

	//몸
	correct = 5;
	y += HHEIGHT / 2 + BHEIGHT / 2;
	if (_direct)
		_player->setBody(x + correct, y, BWIDTH, BHEIGHT);
	else
		_player->setBody(x - correct, y, BWIDTH, BHEIGHT);
}

void Inventory::controlKey()
{
	/**** 조작 ****/
	//인벤 조작
	if (KEYMANAGER->isOnceKeyDown(KEY_RIGHT))
		INVENTORYMANAGER->plusIndex();
	if (KEYMANAGER->isOnceKeyDown(KEY_LEFT))
		INVENTORYMANAGER->minusIndex();
	if (KEYMANAGER->isOnceKeyDown(KEY_UP))
		INVENTORYMANAGER->setIndex(INVENTORYMANAGER->getMaxIndex());
	if (KEYMANAGER->isOnceKeyDown(KEY_DOWN))
		INVENTORYMANAGER->setIndex(0);

	/**** 상태 변화 ****/
	//IDLE
	if (KEYMANAGER->isOnceKeyDown(KEY_INVEN)) {
		INVENTORYMANAGER->close();
		STATEMANAGER->changeState(IDLE);
	}
}

void Inventory::controlFrame()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT * 2) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX())
				_img->setFrameX(0);
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0)
				_img->setFrameX(_img->getMaxFrameX());
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}
