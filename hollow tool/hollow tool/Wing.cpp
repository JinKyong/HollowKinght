#include "stdafx.h"
#include "Wing.h"

HRESULT Wing::init(float x, float y, float angle)
{
	_x = x;
	_y = y;
	_body = RectMakeCenter(_x, _y, 50, 50);

	_angle = angle;
	_speed = ITEMSPEED;

	_state = ENDITEMSTATE;
	_price = 1;
	_type = WING;

	_img = IMAGEMANAGER->addImage("specialitem", "image/item/special.bmp",
		39, 39, true, RGB(0, 0, 0));

	_icon = IMAGEMANAGER->addImage("wing", "image/item/wing.bmp",
		120, 110, true, RGB(32, 34, 64));

	_info = IMAGEMANAGER->addImage("wing_info", "image/item/wing_info.bmp",
		500, 400, false, RGB(0, 0, 0));

	return S_OK;
}

void Wing::release()
{
}

void Wing::update()
{
	//ÀÌµ¿

	//controlFrame();
}

void Wing::render(HDC hdc)
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

void Wing::renderIcon(HDC hdc, float x, float y)
{
	_icon->render(hdc, x, y);
}

void Wing::renderInfo(HDC hdc, float x, float y)
{
	_info->render(hdc, x, y);
}

void Wing::updateRect()
{
	_body = RectMakeCenter(_x, _y, 39, 39);
}

void Wing::changeState(int state)
{
	if (state > ENDITEMSTATE) return;

	_state = state;
}
