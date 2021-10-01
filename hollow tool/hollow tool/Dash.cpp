#include "stdafx.h"
#include "Dash.h"
#include "Player.h"

HRESULT Dash::init(Player * player, bool reverse)
{
	State::init(player, reverse);

	_img = IMAGEMANAGER->addFrameImage("dash", "image/knight/dash.bmp",
		1204, 228, 7, 2, true, RGB(7, 7, 7), true);
	_count = 0;
	_direct = _player->getDirect();

	_reverse = reverse;

	_dashPower = _player->getDashPower();

	_img->setFrameY(_direct);
	if (_direct) {
		_img->setFrameX(_img->getMaxFrameX());

		//effect
		_player->getEM()->addEffect(PointMake(_player->getX(), _player->getHead().top),
			PI, _dashPower, 0, EF_DASH);
	}
	else {
		_img->setFrameX(0);

		//effect
		_player->getEM()->addEffect(PointMake(_player->getX(), _player->getHead().top),
			0, _dashPower, 0, EF_DASH);
	}

	_player->setGravity(0);

	return S_OK;
}

void Dash::release()
{
}

void Dash::update()
{
	_dashPower -= 5000 * TIMEMANAGER->getElapsedTime();
	if (_dashPower > 0)
		_player->moveS(_direct, _dashPower);

	controlKey();
	controlFrame();

	updateRect();
}

void Dash::render(HDC hdc)
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
	sprintf_s(str, "state : DASH");
	TextOut(hdc, rc.right + 10, rc.top, str, strlen(str));
	sprintf_s(str, "index : %d", _img->getFrameX());
	TextOut(hdc, rc.right + 10, rc.top + 20, str, strlen(str));
	sprintf_s(str, "dashPower : %f", _dashPower);
	TextOut(hdc, rc.right + 10, rc.top + 40, str, strlen(str));
	
	/*PRINTMANAGER->print(hdc, rc.right + 10, rc.top, "state : DASH");
	PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 20, "index : %d", _img->getFrameX());
	PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 40, "dashPower : %f", _dashPower);*/
#endif // _DEBUG	
}

void Dash::updateDirect(int direct)
{
}

void Dash::updateRect()
{
	float x = _player->getX();
	float y = _player->getY();
	float correct;

	//머리
	_player->setHead(x, y, HWIDTH, HHEIGHT);

	//몸
	correct = BHEIGHT / 2;
	y += HHEIGHT / 2 + BWIDTH / 2;
	if (_direct)
		_player->setBody(x + correct, y, BHEIGHT, BWIDTH);
	else
		_player->setBody(x - correct, y, BHEIGHT, BWIDTH);
}

void Dash::controlKey()
{
	/**** 상태 변화 ****/
	//JUMP
	if (KEYMANAGER->isOnceKeyDown(KEY_JUMP) && _player->jump()) {
  		if (_player->getJumpCount() == _player->getJumpMax() - 1)
			STATEMANAGER->changeState(JUMP);
		else
			STATEMANAGER->changeState(DOUBLEJUMP);
	}
}

void Dash::controlFrame()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX()) {
				if (_player->getJumpCount() == _player->getJumpMax())
					STATEMANAGER->changeState(DASH2IDLE);
				else
					STATEMANAGER->changeState(FALL);
			}
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0) {
				if (_player->getJumpCount() == _player->getJumpMax())
					STATEMANAGER->changeState(DASH2IDLE);
				else
					STATEMANAGER->changeState(FALL);
			}
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}
