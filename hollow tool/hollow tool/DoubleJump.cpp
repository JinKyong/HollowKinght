#include "stdafx.h"
#include "DoubleJump.h"
#include "Player.h"

HRESULT DoubleJump::init(Player * player, bool reverse)
{
	State::init(player, reverse);

	_img = IMAGEMANAGER->addFrameImage("doublejump", "image/knight/doublejump.bmp",
		928, 258, 8, 2, true, RGB(255, 0, 255), true);
	_count = 0;
	_direct = _player->getDirect();

	_reverse = reverse;

	_img->setFrameY(_direct);
	if (_direct) {
		_img->setFrameX(_img->getMaxFrameX());

		//effect
		_player->getEM()->addEffect(PointMake(_player->getX(), _player->getBody().bottom),
			PI, _jumpPower, 0, EF_DOUBLEJUMP);
	}
	else {
		_img->setFrameX(0);

		//effect
		_player->getEM()->addEffect(PointMake(_player->getX(), _player->getBody().bottom),
			0, _jumpPower, 0, EF_DOUBLEJUMP);
	}

	_player->setGravity(0);
	_jumpPower = _player->getJumpPower();

	return S_OK;
}

void DoubleJump::release()
{
}

void DoubleJump::update()
{
	controlKey();
	controlFrame();

	updateRect();
}

void DoubleJump::render(HDC hdc)
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
	sprintf_s(str, "state : DOUBLEJUMP");
	TextOut(hdc, rc.right + 10, rc.top, str, strlen(str));
	sprintf_s(str, "index : %d", _img->getFrameX());
	TextOut(hdc, rc.right + 10, rc.top + 20, str, strlen(str));
	sprintf_s(str, "jumpPower : %f", _jumpPower);
	TextOut(hdc, rc.right + 10, rc.top + 40, str, strlen(str));
	sprintf_s(str, "jumpCount : %d", _player->getJumpCount());
	TextOut(hdc, rc.right + 10, rc.top + 60, str, strlen(str));

	/*PRINTMANAGER->print(hdc, rc.right + 10, rc.top, "state : DOUBLEJUMP");
	PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 20, "index : %d", _img->getFrameX());
	PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 40, "jumpPower : %f", _jumpPower);
	PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 60, "jumpCount : %d", _player->getJumpCount());*/
#endif // _DEBUG
}

void DoubleJump::updateDirect(int direct)
{
	if (_direct != direct) {
		_direct = direct;

		_img->setFrameX(_img->getMaxFrameX() - _img->getFrameX());
		_img->setFrameY(_direct);
	}
}

void DoubleJump::updateRect()
{
	float x = _player->getX();
	float y = _player->getY();
	float correct;

	//머리
	_player->setHead(x, y, HWIDTH + 10, HHEIGHT);

	//몸
	correct = 15;
	y += HHEIGHT / 2 + BHEIGHT / 2;
	if (_direct)
		_player->setBody(x + correct, y, BWIDTH + 10, BHEIGHT);
	else
		_player->setBody(x - correct, y, BWIDTH + 10, BHEIGHT);
}

void DoubleJump::controlKey()
{
	/**** 조작 ****/
	//좌우
	if (KEYMANAGER->isOnceKeyDown(KEY_RIGHT))
		_player->setDirect(0);
	if (KEYMANAGER->isOnceKeyDown(KEY_LEFT))
		_player->setDirect(1);

	if (KEYMANAGER->isStayKeyDown(KEY_RIGHT))
		_player->move(RIGHT);
	if (KEYMANAGER->isStayKeyDown(KEY_LEFT))
		_player->move(LEFT);

	//JUMP
	if (KEYMANAGER->isStayKeyDown(KEY_JUMP))
		_player->moveS(UP, _jumpPower);

	updateDirect(_player->getDirect());

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

void DoubleJump::controlFrame()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX())
				STATEMANAGER->changeState(FALL);
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0)
				STATEMANAGER->changeState(FALL);
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}
