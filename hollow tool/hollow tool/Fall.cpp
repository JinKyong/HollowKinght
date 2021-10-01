#include "stdafx.h"
#include "Fall.h"
#include "Player.h"

HRESULT Fall::init(Player * player, bool reverse)
{
	State::init(player, reverse);

	_img = IMAGEMANAGER->addFrameImage("fall", "image/knight/fall.bmp",
		522, 258, 6, 2, true, RGB(7, 7, 7), true);
	_count = 0;
	_direct = _player->getDirect();

	_reverse = reverse;

	_img->setFrameY(_direct);
	if (_direct)
		_img->setFrameX(_img->getMaxFrameX());
	else
		_img->setFrameX(0);

	if (_player->getGravity() <= 0)
		_gravity = GRAVITY * 5;
	else
		_gravity = _player->getGravity();

	return S_OK;
}

void Fall::release()
{
}

void Fall::update()
{
	//중력
	_gravity += GRAVITY;
	if (_gravity >= GRAVMAX)
		_gravity = GRAVMAX;
	_player->setGravity(_gravity);
	_player->moveS(DOWN, _gravity);

	controlKey();
	controlFrame();

	updateRect();
}

void Fall::render(HDC hdc)
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
	sprintf_s(str, "state : FALL");
	TextOut(hdc, rc.right + 10, rc.top, str, strlen(str));
	sprintf_s(str, "index : %d", _img->getFrameX());
	TextOut(hdc, rc.right + 10, rc.top + 20, str, strlen(str));
	sprintf_s(str, "gravity : %f", _gravity);
	TextOut(hdc, rc.right + 10, rc.top + 40, str, strlen(str));
	sprintf_s(str, "jumpCount : %d", _player->getJumpCount());
	TextOut(hdc, rc.right + 10, rc.top + 60, str, strlen(str));

	/*PRINTMANAGER->print(hdc, rc.right + 10, rc.top, "state : FALL");
	PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 20, "index : %d", _img->getFrameX());
	PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 40, "gravity : %f", _gravity);
	PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 60, "jumpCount : %d", _player->getJumpCount());*/
#endif // _DEBUG
}

void Fall::updateDirect(int direct)
{
	if (_direct != direct) {
		_direct = direct;

		_img->setFrameX(_img->getMaxFrameX() - _img->getFrameX());
		_img->setFrameY(_direct);
	}
}

void Fall::updateRect()
{
	float x = _player->getX();
	float y = _player->getY();
	float correct;

	//머리
	_player->setHead(x, y, HWIDTH, HHEIGHT);

	//몸
	correct = 15;
	y += HHEIGHT / 2 + BHEIGHT / 2;
	if (_direct)
		_player->setBody(x + correct, y, BWIDTH + 10, BHEIGHT);
	else
		_player->setBody(x - correct, y, BWIDTH + 10, BHEIGHT);
}

void Fall::controlKey()
{
	/**** 조작 ****/
	//좌우
	if (KEYMANAGER->isStayKeyDown(KEY_RIGHT)) {
		_player->setDirect(0);
		_player->move(RIGHT);
	}
	if (KEYMANAGER->isStayKeyDown(KEY_LEFT)) {
		_player->setDirect(1);
		_player->move(LEFT);
	}

	updateDirect(_player->getDirect());

	/**** 상태 변화 ****/
	//DOUBLEJUMP
	if (KEYMANAGER->isOnceKeyDown(KEY_JUMP) && _player->jump())
		STATEMANAGER->changeState(DOUBLEJUMP);

	//DASH
	if (KEYMANAGER->isOnceKeyDown(KEY_DASH) && _player->dash())
		STATEMANAGER->changeState(DASH);

	//SLASH
	if (KEYMANAGER->isStayKeyDown(KEY_DOWN)) {
		if (KEYMANAGER->isOnceKeyDown(KEY_SLASH))
			STATEMANAGER->changeState(DOWNSLASH);
	}
	if (KEYMANAGER->isStayKeyDown(KEY_UP)) {
		if (KEYMANAGER->isOnceKeyDown(KEY_SLASH))
			STATEMANAGER->changeState(UPSLASH);
	}
	if(KEYMANAGER->isOnceKeyDown(KEY_SLASH))
		STATEMANAGER->changeState(SLASH);

	//FIRE
	if (KEYMANAGER->isOnceKeyDown(KEY_FIRE) && _player->getSoul() >= 30)
		STATEMANAGER->changeState(FIRE);
}

void Fall::controlFrame()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX())
				_img->setFrameX(3);
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0)
				_img->setFrameX(_img->getMaxFrameX() - 3);
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}
