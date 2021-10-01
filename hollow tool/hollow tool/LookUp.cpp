#include "stdafx.h"
#include "LookUp.h"
#include "Player.h"

HRESULT LookUp::init(Player * player, bool reverse)
{
	State::init(player, reverse);

	_img = IMAGEMANAGER->addFrameImage("lookup", "image/knight/lookup.bmp",
		58, 254, 1, 2, true, RGB(255, 0, 255), true);
	_count = 0;
	_direct = _player->getDirect();

	_reverse = reverse;

	_img->setFrameY(_direct);
	if (_direct)
		_img->setFrameX(_img->getMaxFrameX());
	else
		_img->setFrameX(0);

	return S_OK;
}

void LookUp::release()
{
}

void LookUp::update()
{
	controlKey();

	updateRect();
}

void LookUp::render(HDC hdc)
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
	sprintf_s(str, "state : LOOKUP");
	TextOut(hdc, rc.right + 10, rc.top, str, strlen(str));
	sprintf_s(str, "index : %d", _img->getFrameX());
	TextOut(hdc, rc.right + 10, rc.top + 20, str, strlen(str));
	sprintf_s(str, "camY : %f", CAMERAMANAGER->getCamY());
	TextOut(hdc, rc.right + 10, rc.top + 40, str, strlen(str));

	/*PRINTMANAGER->print(hdc, rc.right + 10, rc.top, "state : LOOKUP");
	PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 20, "index : %d", _img->getFrameX());
	PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 40, "camY : %f", CAMERAMANAGER->getCamY());*/
#endif // _DEBUG
	
}

void LookUp::updateDirect(int direct)
{
}

void LookUp::updateRect()
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

void LookUp::controlKey()
{
	/**** 조작 ****/
	if (KEYMANAGER->isStayKeyDown(KEY_UP))
		CAMERAMANAGER->moveCam(UP);
	else {
		/**** 상태 변화 ****/
		//LOOKUP2IDLE
		CAMERAMANAGER->setMove(false);
		STATEMANAGER->changeState(LOOKUP2IDLE, true);
	}
}