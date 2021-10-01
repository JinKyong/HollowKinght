#include "stdafx.h"
#include "Wall.h"
#include "Player.h"

HRESULT Wall::init(Player * player, bool reverse)
{
	State::init(player, reverse);

	_img = IMAGEMANAGER->addFrameImage("wall", "image/knight/wall.bmp",
		300, 256, 4, 2, true, RGB(7, 7, 7), true);
	_count = 0;
	_direct = _player->getDirect();

	_reverse = reverse;

	_img->setFrameY(_direct);
	if (_direct)
		_img->setFrameX(_img->getMaxFrameX());
	else
		_img->setFrameX(0);

	_gravity = WGRAVITY;
	_player->setGravity(WGRAVITY);
	_player->resetJump();

	return S_OK;
}

void Wall::release()
{
}

void Wall::update()
{
	//중력
	_player->moveS(DOWN, _gravity);

	controlKey();
	controlFrame();

	updateRect();
}

void Wall::render(HDC hdc)
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
	sprintf_s(str, "state : WALL");
	TextOut(hdc, rc.right + 10, rc.top, str, strlen(str));
	sprintf_s(str, "index : %d", _img->getFrameX());
	TextOut(hdc, rc.right + 10, rc.top + 20, str, strlen(str));
	sprintf_s(str, "gravity : %f", _gravity);
	TextOut(hdc, rc.right + 10, rc.top + 40, str, strlen(str));

	//PRINTMANAGER->print(hdc, rc.right + 10, rc.top, "state : WALL");
	//PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 20, "index : %d", _img->getFrameX());
	//PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 40, "gravity : %f", _gravity);
#endif // _DEBUG
}

void Wall::updateDirect(int direct)
{
	if (_direct != direct)
		STATEMANAGER->changeState(FALL);
}

void Wall::updateRect()
{
	float x = _player->getX();
	float y = _player->getY();
	float correct;

	//머리
	correct = 15;
	if(_direct)
		_player->setHead(x - correct, y, HWIDTH + 10, HHEIGHT);
	else
		_player->setHead(x + correct, y, HWIDTH + 10, HHEIGHT);

	//몸
	//correct = 30;
	y += HHEIGHT / 2 + BHEIGHT / 2;
	if (_direct)
		_player->setBody(_player->getHead().left + BWIDTH / 2, y, BWIDTH, BHEIGHT);
	else
		_player->setBody(_player->getHead().right - BWIDTH / 2, y, BWIDTH, BHEIGHT);
}

void Wall::controlKey()
{
	/**** 조작 ****/
	if (KEYMANAGER->isStayKeyDown(KEY_RIGHT)) {
		_player->setDirect(RIGHT);
		//STATEMANAGER->changeState(RUN);
		//_player->move(RIGHT);
	}
	if (KEYMANAGER->isStayKeyDown(KEY_LEFT)) {
		_player->setDirect(LEFT);
		//STATEMANAGER->changeState(RUN);
		//_player->move(LEFT);
	}

	updateDirect(_player->getDirect());

	/**** 상태 변화 ****/
	//WALLJUMP
	if (KEYMANAGER->isOnceKeyDown(KEY_JUMP) && _player->wall()) {
		_player->jump();
		STATEMANAGER->changeState(WALLJUMP);
	}

	//DASH
	if (KEYMANAGER->isOnceKeyDown(KEY_DASH) && _player->dash()) {
		if (_direct)	_player->setDirect(0);
		else			_player->setDirect(1);
		STATEMANAGER->changeState(DASH);
	}
}

void Wall::controlFrame()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT2) {
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
