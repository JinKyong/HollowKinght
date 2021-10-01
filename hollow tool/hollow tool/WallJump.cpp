#include "stdafx.h"
#include "WallJump.h"
#include "Player.h"

HRESULT WallJump::init(Player * player, bool reverse)
{
	State::init(player, reverse);

	_img = IMAGEMANAGER->addFrameImage("walljump", "image/knight/walljump.bmp",
		582, 246, 6, 2, true, RGB(255, 0, 255), true);
	_count = 0;
	_direct = _player->getDirect();

	_reverse = reverse;

	_img->setFrameY(_direct);
	if (_direct)
		_img->setFrameX(_img->getMaxFrameX());
	else
		_img->setFrameX(0);

	_angle = PI / 4;
	_jumpPower = _player->getJumpPower();

	return S_OK;
}

void WallJump::release()
{
}

void WallJump::update()
{
	controlKey();
	controlFrame();

	updateRect();
}

void WallJump::render(HDC hdc)
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
	sprintf_s(str, "state : WALLJUMP");
	TextOut(hdc, rc.right + 10, rc.top, str, strlen(str));
	sprintf_s(str, "index : %d", _img->getFrameX());
	TextOut(hdc, rc.right + 10, rc.top + 20, str, strlen(str));
	sprintf_s(str, "jumpPower : %f", _jumpPower);
	TextOut(hdc, rc.right + 10, rc.top + 40, str, strlen(str));
	sprintf_s(str, "jumpCount : %d", _player->getJumpCount());
	TextOut(hdc, rc.right + 10, rc.top + 60, str, strlen(str));
	sprintf_s(str, "angle : %f", _angle);
	TextOut(hdc, rc.right + 10, rc.top + 80, str, strlen(str));

	/*PRINTMANAGER->print(hdc, rc.right + 10, rc.top, "state : JUMP");
	PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 20, "index : %d", _img->getFrameX());
	PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 40, "jumpPower : %f", _jumpPower);
	PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 60, "jumpCount : %d", _player->getJumpCount());*/
#endif // _DEBUG
}

void WallJump::updateDirect(int direct)
{
}

void WallJump::updateRect()
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
		_player->setBody(x - correct, y, BWIDTH + 10, BHEIGHT);
	else
		_player->setBody(x + correct, y, BWIDTH + 10, BHEIGHT);
}

void WallJump::move()
{
	_player->moveS(!_direct, cosf(_angle) * _jumpPower);
	_player->moveS(UP, sinf(_angle) * _jumpPower * 2);
}

void WallJump::controlKey()
{
	/**** 조작 ****/
	//점프
	if (KEYMANAGER->isStayKeyDown(KEY_JUMP))
		move();
	if (KEYMANAGER->isStayKeyDown(KEY_LEFT)) {
		if (_direct) {
			if (_img->getFrameX() <= 2)
				_player->moveS(LEFT, _player->getSpeed());
		}
		else {
			if (_img->getFrameX() >= 3)
				_player->moveS(LEFT, _player->getSpeed());
		}
	}
	if (KEYMANAGER->isStayKeyDown(KEY_RIGHT)) {
		if (_direct) {
			if (_img->getFrameX() <= 2)
				_player->moveS(RIGHT, _player->getSpeed());
		}
		else {
			if (_img->getFrameX() >= 3)
				_player->moveS(RIGHT, _player->getSpeed());
		}
	}

	/**** 상태 변화 ****/
	//FALL
	if (KEYMANAGER->isOnceKeyUp(KEY_JUMP))
		STATEMANAGER->changeState(FALL);

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
	if (KEYMANAGER->isOnceKeyDown(KEY_SLASH))
		STATEMANAGER->changeState(SLASH);

	//FIRE
	if (KEYMANAGER->isOnceKeyDown(KEY_FIRE) && _player->getSoul() >= 30)
		STATEMANAGER->changeState(FIRE);
}

void WallJump::controlFrame()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX()) {
				_player->setDirect(1);
				STATEMANAGER->changeState(FALL);
			}
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0) {
				_player->setDirect(0);
				STATEMANAGER->changeState(FALL);
			}
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}
