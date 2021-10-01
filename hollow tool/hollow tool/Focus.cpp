#include "stdafx.h"
#include "Focus.h"
#include "Player.h"

HRESULT Focus::init(Player * player, bool reverse)
{
	State::init(player, reverse);

	_img = IMAGEMANAGER->addFrameImage("focus", "image/knight/focus.bmp",
		940, 230, 10, 2, true, RGB(255, 0, 255), true);
	_count = 0;
	_direct = _player->getDirect();

	_reverse = reverse;

	_img->setFrameY(_direct);
	if (_direct) {
		_img->setFrameX(_img->getMaxFrameX());

		//effect
		_player->getEM()->addEffect(PointMake(_player->getHead().left, _player->getHead().top + 10),
			PI, 0, 0, EF_FOCUS);
	}
	else {
		_img->setFrameX(0);

		//effect
		_player->getEM()->addEffect(PointMake(_player->getHead().right, _player->getHead().top + 10),
			0, 0, 0, EF_FOCUS);
	}

	return S_OK;
}

void Focus::release()
{
}

void Focus::update()
{
	//영혼 고갈되면 상태 변화
	if (_player->getSoul() <= 0)
		STATEMANAGER->changeState(FOCUS2IDLE);

	//일정 시간마다 HP회복
	TIMEMANAGER->setTimer();
	if (TIMEMANAGER->clock() > 1.5) {
		TIMEMANAGER->endTimer();
		_player->changeHP(1);
	}

	controlKey();
	controlFrame();
	
	updateRect();
}

void Focus::render(HDC hdc)
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
	sprintf_s(str, "state : FOCUS");
	TextOut(hdc, rc.right + 10, rc.top, str, strlen(str));
	sprintf_s(str, "index : %d", _img->getFrameX());
	TextOut(hdc, rc.right + 10, rc.top + 20, str, strlen(str));
	sprintf_s(str, "soul : %f", _player->getSoul());
	TextOut(hdc, rc.right + 10, rc.top + 40, str, strlen(str));

	//PRINTMANAGER->print(hdc, rc.right + 10, rc.top, "state : FOCUS");
	//PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 20, "index : %d", _img->getFrameX());
	//PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 40, "soul : %f", _player->getSoul());
#endif // _DEBUG

}

void Focus::updateDirect(int direct)
{
}

void Focus::updateRect()
{
	float x = _player->getX();
	float y = _player->getY();
	float correct;

	//머리
	correct = 10;
	_player->setHead(x, y + correct, HWIDTH, HHEIGHT);

	//몸
	correct = 15;
	y += HHEIGHT / 2 + BHEIGHT / 2;
	if (_direct)
		_player->setBody(x + correct, y, BWIDTH + 5, BHEIGHT);
	else
		_player->setBody(x - correct, y, BWIDTH + 5, BHEIGHT);
}

void Focus::controlKey()
{
	/**** 조작 ****/
	if (KEYMANAGER->isStayKeyDown(KEY_FOCUS)) {}
		_player->changeSoul(-18 * TIMEMANAGER->getElapsedTime());

	/**** 상태 변화 ****/
	//FOCUS2IDLE
	if (KEYMANAGER->isOnceKeyUp(KEY_FOCUS))
		STATEMANAGER->changeState(FOCUS2IDLE);

	//RUN
	if (KEYMANAGER->isOnceKeyDown(KEY_RIGHT)) {
		_player->setDirect(RIGHT);
		STATEMANAGER->changeState(RUN);
	}
	if (KEYMANAGER->isOnceKeyDown(KEY_LEFT)) {
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
	if (KEYMANAGER->isOnceKeyDown(KEY_SLASH))
		STATEMANAGER->changeState(SLASH);

	//FIRE
	if (KEYMANAGER->isOnceKeyDown(KEY_FIRE) && _player->getSoul() >= 30)
		STATEMANAGER->changeState(FIRE);
}

void Focus::controlFrame()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT * 2) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX())
				_img->setFrameX(5);
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0)
				_img->setFrameX(_img->getMaxFrameX() - 5);
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}
