#include "stdafx.h"
#include "LookUp2Idle.h"
#include "Player.h"

HRESULT LookUp2Idle::init(Player * player, bool reverse)
{
	State::init(player, reverse);

	_img = IMAGEMANAGER->addFrameImage("lookup2idle", "image/knight/lookup2idle.bmp",
		116, 254, 2, 2, true, RGB(255, 0, 255), true);
	_count = 0;
	_direct = _player->getDirect();

	_reverse = reverse;

	_img->setFrameY(_direct);
	if (_direct) {
		if (_reverse)
			_img->setFrameX(0);
		else
			_img->setFrameX(_img->getMaxFrameX());

	}
	else {
		if (_reverse)
			_img->setFrameX(_img->getMaxFrameX());
		else
			_img->setFrameX(0);
	}

	return S_OK;
}

void LookUp2Idle::release()
{
}

void LookUp2Idle::update()
{
	controlKey();
	controlFrame();

	updateRect();
}

void LookUp2Idle::render(HDC hdc)
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
	sprintf_s(str, "state : LOOKUP2IDLE");
	TextOut(hdc, rc.right + 10, rc.top, str, strlen(str));
	sprintf_s(str, "index : %d", _img->getFrameX());
	TextOut(hdc, rc.right + 10, rc.top + 20, str, strlen(str));
	sprintf_s(str, "camY : %f", CAMERAMANAGER->getCamY());
	TextOut(hdc, rc.right + 10, rc.top + 40, str, strlen(str));

	//PRINTMANAGER->print(hdc, rc.right + 10, rc.top, "state : LOOKUP2IDLE");
	//PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 20, "index : %d", _img->getFrameX());
	//PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 40, "camY : %f", CAMERAMANAGER->getCamY());
#endif // _DEBUG
	
}

void LookUp2Idle::updateDirect(int direct)
{
}

void LookUp2Idle::updateRect()
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

void LookUp2Idle::controlKey()
{
	/**** 조작 ****/
	if (KEYMANAGER->isStayKeyDown(KEY_UP))
		CAMERAMANAGER->moveCam(UP);

	/**** 상태 변화 ****/
	//IDLE
	if (KEYMANAGER->isOnceKeyUp(KEY_UP)) {
		CAMERAMANAGER->setMove(false);
		STATEMANAGER->changeState(IDLE);
	}

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
	if (KEYMANAGER->isOnceKeyDown(KEY_JUMP) && _player->jump()) {
		STATEMANAGER->changeState(JUMP);
	}

	//DASH
	if (KEYMANAGER->isOnceKeyDown(KEY_DASH) && _player->dash()) {
		STATEMANAGER->changeState(DASH);
	}

	//SLASH
	if (KEYMANAGER->isOnceKeyDown(KEY_SLASH)) {
		STATEMANAGER->changeState(SLASH);
	}

	//FIRE
	if (KEYMANAGER->isOnceKeyDown(KEY_FIRE) && _player->getSoul() >= 30) {
		STATEMANAGER->changeState(FIRE);
	}

}

void LookUp2Idle::controlFrame()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT * 2) {
		if (_reverse) {
			switch (_direct) {
			case 0:
				if (_img->getFrameX() <= 0)
					STATEMANAGER->changeState(IDLE);
				else
					_img->setFrameX(_img->getFrameX() - 1);
				break;
			case 1:
				if (_img->getFrameX() >= _img->getMaxFrameX())
					STATEMANAGER->changeState(IDLE);
				else
					_img->setFrameX(_img->getFrameX() + 1);
				break;
			}
		}
		else {
			switch (_direct) {
			case 0:
				if (_img->getFrameX() >= _img->getMaxFrameX()) {
					STATEMANAGER->changeState(LOOKUP);
				}
				else
					_img->setFrameX(_img->getFrameX() + 1);
				break;
			case 1:
				if (_img->getFrameX() <= 0) {
					STATEMANAGER->changeState(LOOKUP);
				}
				else
					_img->setFrameX(_img->getFrameX() - 1);
				break;
			}
		}

		_count = 0;
	}
}
