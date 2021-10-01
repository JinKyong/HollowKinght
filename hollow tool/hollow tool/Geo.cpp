#include "stdafx.h"
#include "Geo.h"

HRESULT Geo::init(float x, float y, float angle)
{
	_x = x;
	_y = y;
	_body = RectMakeCenter(_x, _y, GEOWIDTH, GEOHEIGHT);

	_angle = angle;
	_speed = ITEMSPEED;

	_state = ITEMMOVE;
	_price = 15;
	_type = GEO;

	_img = new image;
	_img->init("image/item/geomove.bmp",
		168, 41, 6, 1, true, RGB(255, 0, 255));

	_icon = IMAGEMANAGER->addImage("UI_geo", "image/UI/UI_geo.bmp",
		52, 54, true, RGB(253, 9, 9));

	_info = IMAGEMANAGER->addImage("geo_info", "image/item/geo_info.bmp",
		500, 400, false, RGB(0, 0, 0));

	_gravity = 0;

	return S_OK;
}

void Geo::release()
{
	_img->release();
	SAFE_DELETE(_img);

	//_inven->release();
	//SAFE_DELETE(_inven);
}

void Geo::update()
{
	//ÀÌµ¿
	if (_state == ITEMMOVE)
		move();

	controlFrame();
}

void Geo::render(HDC hdc)
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

	_img->frameRender(hdc,
		_x - _img->getFrameWidth() / 2, _y - _img->getFrameHeight() / 2);

}

void Geo::renderIcon(HDC hdc, float x, float y)
{
	_icon->render(hdc, x, y);
}

void Geo::renderInfo(HDC hdc, float x, float y)
{
	_info->render(hdc, x, y);
}

void Geo::updateRect()
{
	_body = RectMakeCenter(_x, _y, GEOWIDTH, GEOHEIGHT);
}

void Geo::changeState(int state)
{
	if (state > ENDITEMSTATE) return;

	_state = state;
}

void Geo::move()
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

void Geo::controlFrame()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT) {
		if (_img->getFrameX() >= _img->getMaxFrameX())
			_img->setFrameX(0);
		else
			_img->setFrameX(_img->getFrameX() + 1);

		_count = 0;
	}
}
