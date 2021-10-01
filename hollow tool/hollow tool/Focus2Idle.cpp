#include "stdafx.h"
#include "Focus2Idle.h"
#include "Player.h"

HRESULT Focus2Idle::init(Player * player, bool reverse)
{
	State::init(player, reverse);

	_img = IMAGEMANAGER->addFrameImage("focus2idle", "image/knight/focus2idle.bmp",
		128, 254, 2, 2, true, RGB(255, 0, 255), true);
	_count = 0;
	_direct = _player->getDirect();

	_reverse = reverse;

	_img->setFrameY(_direct);
	if (_direct) {
		if(_reverse)
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

void Focus2Idle::release()
{
}

void Focus2Idle::update()
{
	controlKey();
	controlFrame();

	updateRect();
}

void Focus2Idle::render(HDC hdc)
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
	sprintf_s(str, "state : FOCUS2IDLE");
	TextOut(hdc, rc.right + 10, rc.top, str, strlen(str));
	sprintf_s(str, "index : %d", _img->getFrameX());
	TextOut(hdc, rc.right + 10, rc.top + 20, str, strlen(str));

	/*PRINTMANAGER->print(hdc, rc.right + 10, rc.top, "state : FOCUS2IDLE");
	PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 20, "index : %d", _img->getFrameX());*/
#endif // _DEBUG
	
}

void Focus2Idle::updateDirect(int direct)
{
}

void Focus2Idle::updateRect()
{
	float x = _player->getX();
	float y = _player->getY();
	float correct;

	//�Ӹ�
	_player->setHead(x, y, HWIDTH, HHEIGHT);

	//��
	correct = 5;
	y += HHEIGHT / 2 + BHEIGHT / 2;
	if (_direct)
		_player->setBody(x + correct, y, BWIDTH, BHEIGHT);
	else
		_player->setBody(x - correct, y, BWIDTH, BHEIGHT);
}

void Focus2Idle::controlKey()
{
	/**** ���� ****/

	/**** ���� ��ȭ ****/
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

void Focus2Idle::controlFrame()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT) {
		if (_reverse) {
			switch (_direct) {
			case 0:
				if (_img->getFrameX() <= 0)
					STATEMANAGER->changeState(FOCUS);
				else
					_img->setFrameX(_img->getFrameX() - 1);
				break;
			case 1:
				if (_img->getFrameX() >= _img->getMaxFrameX())
					STATEMANAGER->changeState(FOCUS);
				else
					_img->setFrameX(_img->getFrameX() + 1);
				break;
			}
		}
		else {
			switch (_direct) {
			case 0:
				if (_img->getFrameX() >= _img->getMaxFrameX())
					STATEMANAGER->changeState(IDLE);
				else
					_img->setFrameX(_img->getFrameX() + 1);
				break;
			case 1:
				if (_img->getFrameX() <= 0)
					STATEMANAGER->changeState(IDLE);
				else
					_img->setFrameX(_img->getFrameX() - 1);
				break;
			}
		}

		_count = 0;
	}
}
