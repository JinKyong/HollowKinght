#include "stdafx.h"
#include "Run.h"
#include "Player.h"

HRESULT Run::init(Player * player, bool reverse)
{
	State::init(player, reverse);

	_img = IMAGEMANAGER->addFrameImage("run", "image/knight/run.bmp",
		984, 254, 12, 2, true, RGB(255, 0, 255), true);
	_count = 0;
	_direct = _player->getDirect();

	_reverse = reverse;

	_img->setFrameY(_direct);
	if (_direct)
		_img->setFrameX(_img->getMaxFrameX());
	else
		_img->setFrameX(0);

	_player->setGravity(0);
	_player->resetDash();

	return S_OK;
}

void Run::release()
{
}

void Run::update()
{
	controlKey();
	controlFrame();

	updateRect();
}

void Run::render(HDC hdc)
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
	sprintf_s(str, "state : RUN");
	TextOut(hdc, rc.right + 10, rc.top, str, strlen(str));
	sprintf_s(str, "index : %d", _img->getFrameX());
	TextOut(hdc, rc.right + 10, rc.top + 20, str, strlen(str));

	//PRINTMANAGER->print(hdc, rc.right + 10, rc.top, "state : RUN");
	//PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 20, "index : %d", _img->getFrameX());
#endif // _DEBUG

}

void Run::updateDirect(int direct)
{
	if (_direct != direct)
		this->init(_player);
}

void Run::updateRect()
{
	float x = _player->getX();
	float y = _player->getY();
	float correct;

	//머리
	_player->setHead(x, y, HWIDTH, HHEIGHT);

	//몸
	correct = 10;
	y += HHEIGHT / 2 + BHEIGHT / 2;

	if (_direct)
		_player->setBody(x + correct, y, BWIDTH, BHEIGHT);
	else
		_player->setBody(x - correct, y, BWIDTH, BHEIGHT);
}

void Run::controlKey()
{
	/**** 조작 ****/
	//좌우
	if (KEYMANAGER->isStayKeyDown(KEY_RIGHT))
		_player->move(RIGHT);
	if (KEYMANAGER->isStayKeyDown(KEY_LEFT))
		_player->move(LEFT);

	updateDirect(_player->getDirect());

	/**** 상태 변화 ****/
	//IDLE
	if (KEYMANAGER->isOnceKeyUp(KEY_RIGHT))
		STATEMANAGER->changeState(IDLE);
	if (KEYMANAGER->isOnceKeyUp(KEY_LEFT))
		STATEMANAGER->changeState(IDLE);

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

}

void Run::controlFrame()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT * 2) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX())
				_img->setFrameX(4);
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0)
				_img->setFrameX(_img->getMaxFrameX() - 4);
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}
