#include "stdafx.h"
#include "Idle.h"
#include "Player.h"
#include "Wing.h"

HRESULT Idle::init(Player * player, bool reverse)
{
	State::init(player, reverse);

	_img = IMAGEMANAGER->addFrameImage("idle", "image/knight/idle.bmp",
		360, 254, 6, 2, true, RGB(255, 0, 255), true);
	_count = 0;
	_direct = _player->getDirect();

	_reverse = reverse;

	_img->setFrameY(_direct);
	if (_direct)
		_img->setFrameX(_img->getMaxFrameX());
	else
		_img->setFrameX(0);

	_player->setGravity(0);
	_player->resetJump();
	_player->resetDash();

	return S_OK;
}

void Idle::release()
{
}

void Idle::update()
{
	controlKey();
	controlFrame();

	updateRect();
}

void Idle::render(HDC hdc)
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
	sprintf_s(str, "state : IDLE");
	TextOut(hdc, rc.right + 10, rc.top, str, strlen(str));
	sprintf_s(str, "index : %d", _img->getFrameX());
	TextOut(hdc, rc.right + 10, rc.top + 20, str, strlen(str));
	sprintf_s(str, "jumpCount : %d", _player->getJumpCount());
	TextOut(hdc, rc.right + 10, rc.top + 40, str, strlen(str));
	sprintf_s(str, "dashCount : %d", _player->getDashCount());
	TextOut(hdc, rc.right + 10, rc.top + 60, str, strlen(str));

	/*PRINTMANAGER->print(hdc, rc.right + 10, rc.top, "state : IDLE");
	PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 20, "index : %d", _img->getFrameX());
	PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 60, "jumpCount : %d", _player->getJumpCount());*/
#endif // _DEBUG
	
}

void Idle::updateDirect(int direct)
{
}

void Idle::updateRect()
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

void Idle::controlKey()
{
	/**** 조작 ****/
	if (KEYMANAGER->isOnceKeyDown(KEY_UP) || KEYMANAGER->isOnceKeyDown(KEY_DOWN)) {
		if (_player->getPortal()) { _player->getIn(); }
		TIMEMANAGER->setTimer();
	}

	if (KEYMANAGER->isOnceKeyDown(KEY_FOCUS))
		TIMEMANAGER->setTimer();

#ifdef _DEBUG
	/**** 운영자 모드 ****/
	if (KEYMANAGER->isOnceKeyDown(KEY_GOD)) {
		if (_player->getDamage() == 15)
			_player->setDamage(150);
		else
			_player->setDamage(15);
	}

	if (KEYMANAGER->isOnceKeyDown(KEY_SOUL))
		_player->changeSoul(100);

	if (KEYMANAGER->isOnceKeyDown(KEY_HURT))
		_player->changeHP(-1);

	if (KEYMANAGER->isOnceKeyDown(KEY_WING)) {
		Item* item = new Wing;
		item->init(0, 0, 0);
		INVENTORYMANAGER->addItem(item);
	}
#endif // _DEBUG


	/**** 상태 변화 ****/
	//RUN
	if (KEYMANAGER->isStayKeyDown(KEY_RIGHT)) {
		_player->setDirect(RIGHT);
		STATEMANAGER->changeState(RUN);
	}
	if (KEYMANAGER->isStayKeyDown(KEY_LEFT)) {
		_player->setDirect(LEFT);
		STATEMANAGER->changeState(RUN);
	}

	//JUMP
	if (KEYMANAGER->isOnceKeyDown(KEY_JUMP) && _player->jump())
		STATEMANAGER->changeState(JUMP);

	//DASH
	if (KEYMANAGER->isOnceKeyDown(KEY_DASH) && _player->dash())
		STATEMANAGER->changeState(DASH);

	//SLASH
	if (KEYMANAGER->isStayKeyDown(KEY_UP)) {
		if (KEYMANAGER->isOnceKeyDown(KEY_SLASH))
			STATEMANAGER->changeState(UPSLASH);
	}
	if (KEYMANAGER->isOnceKeyDown(KEY_SLASH))
		STATEMANAGER->changeState(SLASH);

	//FIRE
	if (KEYMANAGER->isOnceKeyDown(KEY_FIRE) && _player->getSoul() >= 30)
		STATEMANAGER->changeState(FIRE);

	//FOCUS2IDLE
	if (KEYMANAGER->isStayKeyDown(KEY_FOCUS) && _player->getSoul() > 0) {
		if(TIMEMANAGER->clock() >= 0.5)
			STATEMANAGER->changeState(FOCUS2IDLE, true);
	}
	
	//LOOK
	if (KEYMANAGER->isStayKeyDown(KEY_UP)) {
		if (TIMEMANAGER->clock() >= 0.5)
			STATEMANAGER->changeState(LOOKUP2IDLE);
	}
	if (KEYMANAGER->isStayKeyDown(KEY_DOWN)) {
		if (TIMEMANAGER->clock() >= 0.5)
			STATEMANAGER->changeState(LOOKDOWN2IDLE);
	}

	if (KEYMANAGER->isOnceKeyDown(KEY_INVEN)) {
		INVENTORYMANAGER->open();
		STATEMANAGER->changeState(INVENTORY);
	}
		
}

void Idle::controlFrame()
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
