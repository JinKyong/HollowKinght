#include "stdafx.h"
#include "Fire.h"
#include "Player.h"

HRESULT Fire::init(Player * player, bool reverse)
{
	State::init(player, reverse);

	_img = IMAGEMANAGER->addFrameImage("fire", "image/knight/fire.bmp",
		609, 254, 7, 2, true, RGB(255, 0, 255), true);
	_count = 0;
	_direct = _player->getDirect();

	_reverse = reverse;

	_img->setFrameY(_direct);
	if (_direct) {
		_img->setFrameX(_img->getMaxFrameX());

		//effect
		_player->getEM()->addEffect(PointMake(_player->getX(), _player->getHead().top),
			PI, 1500, _player->getDamage() * 3, EF_FIRE);
	}
	else {
		_img->setFrameX(0);

		//effect
		_player->getEM()->addEffect(PointMake(_player->getX(), _player->getHead().top),
			0, 1500, _player->getDamage() * 3, EF_FIRE);
	}

	_player->setGravity(0);
	_player->changeSoul(-30);

	return S_OK;
}

void Fire::release()
{
}

void Fire::update()
{
	controlFrame();

	updateRect();
}

void Fire::render(HDC hdc)
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
	sprintf_s(str, "state : FIRE");
	TextOut(hdc, rc.right + 10, rc.top, str, strlen(str));
	sprintf_s(str, "index : %d", _img->getFrameX());
	TextOut(hdc, rc.right + 10, rc.top + 20, str, strlen(str));
	sprintf_s(str, "soul : %f", _player->getSoul());
	TextOut(hdc, rc.right + 10, rc.top + 40, str, strlen(str));

	/*PRINTMANAGER->print(hdc, rc.right + 10, rc.top, "state : FIRE");
	PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 20, "index : %d", _img->getFrameX());
	PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 40, "soul : %f", _player->getSoul());*/
#endif // _DEBUG
}

void Fire::updateDirect(int direct)
{
}

void Fire::updateRect()
{
	float x = _player->getX();
	float y = _player->getY();
	float correct;

	//머리
	correct = 5;
	if(_direct)
		_player->setHead(x + correct, y, HWIDTH + 10, HHEIGHT);
	else
		_player->setHead(x - correct, y, HWIDTH + 10, HHEIGHT);

	//몸
	correct = 5;
	y += HHEIGHT / 2 + BHEIGHT / 2;
	if (_direct)
		_player->setBody(x - correct, y, BWIDTH + 10, BHEIGHT);
	else
		_player->setBody(x + correct, y, BWIDTH + 10, BHEIGHT);
}

void Fire::controlKey()
{
	/**** 조작 ****/

	/**** 상태 변화 ****/
}

void Fire::controlFrame()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX())
				STATEMANAGER->setState(STATEMANAGER->getPrevState());
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0)
				STATEMANAGER->setState(STATEMANAGER->getPrevState());
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}
