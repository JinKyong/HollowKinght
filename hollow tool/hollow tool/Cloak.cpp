#include "stdafx.h"
#include "Cloak.h"

HRESULT Cloak::init(float x, float y, float angle)
{
	_x = x;
	_y = y;
	_body = RectMakeCenter(_x, _y, 50, 50);

	_angle = angle;
	_speed = ITEMSPEED;

	_state = ITEMMOVE;
	_price = 1;
	_type = CLOAK;

	_img = IMAGEMANAGER->addImage("specialitem", "image/item/special.bmp",
		39, 39, true, RGB(0, 0, 0));

	_icon = IMAGEMANAGER->addImage("cloak", "image/item/cloak.bmp",
		120, 110, true, RGB(0, 0, 0));

	_info = IMAGEMANAGER->addImage("cloak_info", "image/item/cloak_info.bmp",
		500, 400, false, RGB(0, 0, 0));

	_gravity = 0;

	return S_OK;
}

void Cloak::release()
{

}

void Cloak::update()
{
	//ÀÌµ¿
	if (_state == ITEMMOVE)
		move();

	//controlFrame();
}

void Cloak::render(HDC hdc)
{
#ifdef _DEBUG
	if (PRINTMANAGER->isDebug()) {
		Rectangle(hdc, _body);
	}

	char str[128];
	sprintf_s(str, "state : %d", _state);
	TextOut(hdc, _body.right + 10, _body.top, str, strlen(str));
	sprintf_s(str, "index : %d", _img->getFrameX());
	TextOut(hdc, _body.right + 10, _body.top + 20, str, strlen(str));

	//PRINTMANAGER->print(getMemDC(), _body.right + 10, _body.top, "index : %d", _img->getFrameX());
#endif // _DEBUG

	_img->render(hdc, _x - _img->getWidth() / 2, _y - _img->getHeight() / 2);
}

void Cloak::renderIcon(HDC hdc, float x, float y)
{
	_icon->render(hdc, x, y);
}

void Cloak::renderInfo(HDC hdc, float x, float y)
{
	_info->render(hdc, x, y);
}

void Cloak::updateRect()
{
	_body = RectMakeCenter(_x, _y, 39, 39);
}

void Cloak::changeState(int state)
{
	if (state > ENDITEMSTATE) return;

	_state = state;
}

void Cloak::move()
{
	_gravity = _gravity + ITEMGRAVITY < ITEMGRAVMAX ?
		_gravity + ITEMGRAVITY : ITEMGRAVMAX;

	if (_angle < PI / 2)
		_angle += TORADIAN(ITEMFRICT);
	else if (_angle > PI / 2)
		_angle -= TORADIAN(ITEMFRICT);

	_x += cosf(_angle) * _speed * TIMEMANAGER->getElapsedTime();
	_y += (-sinf(_angle) * _speed + _gravity) * TIMEMANAGER->getElapsedTime();
}
