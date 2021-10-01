#include "stdafx.h"
#include "UpSlash.h"
#include "Player.h"

HRESULT UpSlash::init(Player * player, bool reverse)
{
	State::init(player, reverse);
	
	_img = IMAGEMANAGER->addFrameImage("upslash", "image/knight/upslash.bmp",
		475, 242, 5, 2, true, RGB(255, 0, 255), true);

	_count = 0;
	_direct = _player->getDirect();

	_reverse = reverse;

	_img->setFrameY(_direct);
	if (_direct) {
		_img->setFrameX(_img->getMaxFrameX());

		//effect
		_player->getEM()->addEffect(PointMake(_player->getHead().left, _player->getBody().top),
			PI, 0, _player->getDamage(), EF_UPSLASH);
	}
	else {
		_img->setFrameX(0);

		//effect
		_player->getEM()->addEffect(PointMake(_player->getHead().right, _player->getBody().top),
			0, 0, _player->getDamage(), EF_UPSLASH);
	}

	if (_player->getGravity() > 0) {
		_air = true;
		_gravity = _player->getGravity();
	}
	else {
		_air = false;
		_gravity = 0;
	}

	return S_OK;
}

void UpSlash::release()
{
}

void UpSlash::update()
{
	//중력
	if (_air) {
		_gravity += GRAVITY;
		if (_gravity >= GRAVMAX)
			_gravity = GRAVMAX;
		_player->setGravity(_gravity);
		_player->moveS(DOWN, _gravity);
	}

	controlKey();
	controlFrame();

	updateRect();
}

void UpSlash::render(HDC hdc)
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
	sprintf_s(str, "state : UPSLASH");
	TextOut(hdc, rc.right + 10, rc.top, str, strlen(str));
	sprintf_s(str, "index : %d", _img->getFrameX());
	TextOut(hdc, rc.right + 10, rc.top + 20, str, strlen(str));
	sprintf_s(str, "gravity : %f", _gravity);
	TextOut(hdc, rc.right + 10, rc.top + 40, str, strlen(str));

	//PRINTMANAGER->print(hdc, rc.right + 10, rc.top, "state : SLASH");
	//PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 20, "index : %d", _img->getFrameX());
	//PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 40, "sCount : %d", _slashCount);
#endif // _DEBUG
}

void UpSlash::updateDirect(int direct)
{
}

void UpSlash::updateRect()
{
	float x = _player->getX();
	float y = _player->getY();
	float correct;

	//머리
	_player->setHead(x, y, HWIDTH + 20, HHEIGHT);

	//몸
	correct = 10;
	y += HHEIGHT / 2 + BHEIGHT / 2;
	if (_direct)
		_player->setBody(x - correct, y, BWIDTH + 10, BHEIGHT);
	else
		_player->setBody(x + correct, y, BWIDTH + 10, BHEIGHT);
}

void UpSlash::controlKey()
{
	/**** 조작 ****/
	//좌우
	if (KEYMANAGER->isStayKeyDown(KEY_RIGHT)) {
		_player->setDirect(RIGHT);
		_player->move(RIGHT);
	}
	if (KEYMANAGER->isStayKeyDown(KEY_LEFT)) {
		_player->setDirect(LEFT);
		_player->move(LEFT);
	}

	/**** 상태 변화 ****/
	//JUMP
	if (KEYMANAGER->isOnceKeyDown(KEY_JUMP) && _player->jump())
		STATEMANAGER->changeState(JUMP);

	//DASH
	if (KEYMANAGER->isOnceKeyDown(KEY_DASH) && _player->dash())
		STATEMANAGER->changeState(DASH);

	//FIRE
	if (KEYMANAGER->isOnceKeyDown(KEY_FIRE) && _player->getSoul() >= 30)
		STATEMANAGER->changeStateNonPrev(FIRE);
}

void UpSlash::controlFrame()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT2) {
		switch (_direct) {
		case 0:	//RIGHT
			if (_img->getFrameX() >= _img->getMaxFrameX())
				STATEMANAGER->setState(STATEMANAGER->getPrevState());
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:	//LEFT
			if (_img->getFrameX() <= 0)
				STATEMANAGER->setState(STATEMANAGER->getPrevState());
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}
