#include "stdafx.h"
#include "Death.h"
#include "Player.h"

HRESULT Death::init(Player * player, bool reverse)
{
	State::init(player, reverse);

	_img = IMAGEMANAGER->addFrameImage("death", "image/knight/death.bmp",
		1284, 248, 12, 2, true, RGB(255, 0, 255), true);
	_count = 0;
	_direct = _player->getDirect();

	_reverse = reverse;

	_img->setFrameY(_direct);
	if (_direct)
		_img->setFrameX(_img->getMaxFrameX());
	else
		_img->setFrameX(0);

	_player->setGravity(0);

	return S_OK;
}

void Death::release()
{
}

void Death::update()
{
	if (TIMEMANAGER->clock() > 2) {
		_player->setX(WINSIZEX);
		_player->setY(0);
		_player->changeSoul(-100);
		_player->changeHP(5);

		STATEMANAGER->changeState(IDLE);
		SCENEMANAGER->changeScene("dartmount");
	}

	controlKey();
	controlFrame();

	updateRect();
}

void Death::render(HDC hdc)
{
	RECT rc = _player->getHead();

	if (_direct)
		_img->frameRender(hdc, rc.left, rc.top);
	else
		_img->frameRender(hdc, rc.right - _img->getFrameWidth(), rc.top);

#ifdef _DEBUG
	char str[128];
	sprintf_s(str, "state : DEATH");
	TextOut(hdc, rc.right + 10, rc.top, str, strlen(str));
	sprintf_s(str, "index : %d", _img->getFrameX());
	TextOut(hdc, rc.right + 10, rc.top + 20, str, strlen(str));

	/*PRINTMANAGER->print(hdc, rc.right + 10, rc.top, "state : DOUBLEJUMP");
	PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 20, "index : %d", _img->getFrameX());
	PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 40, "jumpPower : %f", _jumpPower);
	PRINTMANAGER->print(hdc, rc.right + 10, rc.top + 60, "jumpCount : %d", _player->getJumpCount());*/
#endif // _DEBUG
}

void Death::updateDirect(int direct)
{
}

void Death::updateRect()
{
	float x = _player->getX();
	float y = _player->getY();

	_player->setHead(x, y, HWIDTH + 30, HHEIGHT - 5);
	_player->setBody(x, y, 0, 0);
}

void Death::controlKey()
{
	/**** 조작 ****/

	/**** 상태 변화 ****/
}

void Death::controlFrame()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT2) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX()) {
				TIMEMANAGER->setTimer();
			}
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0) {
				TIMEMANAGER->setTimer();
			}
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}
