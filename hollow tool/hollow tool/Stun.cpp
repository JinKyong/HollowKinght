#include "stdafx.h"
#include "Stun.h"
#include "Player.h"

HRESULT Stun::init(Player * player, bool reverse)
{
	State::init(player, reverse);
	
	_img = IMAGEMANAGER->addFrameImage("stun", "image/knight/stun.bmp",
		94, 248, 1, 2, true, RGB(255, 0, 255), true);
	_count = 0;
	_direct = _player->getDirect();

	_reverse = reverse;

	_img->setFrameY(_direct);
	if (_direct)
		_img->setFrameX(_img->getMaxFrameX());
	else
		_img->setFrameX(0);

	_player->stun();
	_alpha = 255;

	return S_OK;
}

void Stun::release()
{
}

void Stun::update()
{
	TIMEMANAGER->setTimer();
	if(TIMEMANAGER->clock() > DURATION){		
		//상태 변화
		if (_player->getJumpCount() == _player->getJumpMax())
			STATEMANAGER->changeState(IDLE);
		else
			STATEMANAGER->changeState(FALL);
	}

	_alpha = _alpha == 0 ? 255 : 0;
	if (_direct)
		_player->moveS(RIGHT, 100.0);
	else
		_player->moveS(LEFT, 100.0);

	updateRect();
}

void Stun::render(HDC hdc)
{
	RECT rc = _player->getHead();
	if (_direct)
		_img->frameAlphaRender(hdc, rc.right - _img->getFrameWidth(), rc.top, _alpha);
	else
		_img->frameAlphaRender(hdc, rc.left, rc.top, _alpha); 

#ifdef _DEBUG
	char str[128];
	sprintf_s(str, "state : STUN");
	TextOut(hdc, rc.right + 10, rc.top, str, strlen(str));
	sprintf_s(str, "index : %d", _img->getFrameX());
	TextOut(hdc, rc.right + 10, rc.top + 20, str, strlen(str));
	sprintf_s(str, "duration : %f", DURATION);
	TextOut(hdc, rc.right + 10, rc.top + 40, str, strlen(str));

	//PRINTMANAGER->print(hdc, rc.right + 10, rc.top, "state : STUN");
	//PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 20, "index : %d", _img->getFrameX());
	//PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 40, "duration : %f", DURATION);
#endif // _DEBUG
}

void Stun::updateDirect(int direct)
{
}

void Stun::updateRect()
{
	float x = _player->getX();
	float y = _player->getY();
	float correct;

	//머리
	correct = 20;
	if(_direct)
		_player->setHead(x + correct, y, HWIDTH + 20, HHEIGHT);
	else
		_player->setHead(x - correct, y, HWIDTH + 20, HHEIGHT);

	//몸
	correct = 5;
	y += HHEIGHT / 2 + BHEIGHT / 2;
	if (_direct)
		_player->setBody(x - correct, y, BWIDTH + 20, BHEIGHT);
	else
		_player->setBody(x + correct, y, BWIDTH + 20, BHEIGHT);
}

void Stun::controlKey()
{/**** 조작 ****/
	//좌우
	if (KEYMANAGER->isStayKeyDown(KEY_RIGHT))
		_player->move(RIGHT);
	if (KEYMANAGER->isStayKeyDown(KEY_LEFT))
		_player->move(LEFT);

	/**** 상태 변화 ****/
}
