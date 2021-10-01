#include "stdafx.h"
#include "EF_Focus.h"

HRESULT EF_Focus::init(POINT position, float angle, float speed, float damage)
{
	_x = position.x;
	_y = position.y;

	_img = new image;
	_img->init("image/effect/focus.bmp", 296, 234, 4, 2, true, RGB(255, 0, 255));

	_angle = angle;
	_speed = speed;
	_damage = damage;

	if (PI / 2 < _angle && _angle < 3 * PI / 2)
		_direct = LEFT;
	else
		_direct = RIGHT;

	//ÀÌ¹ÌÁö
	_img->setFrameY(_direct);
	if (_direct) {
		_img->setFrameX(_img->getMaxFrameX());
	}
	else {
		_img->setFrameX(0);
	}

	updateRect();

	_state = 0;
	_count = 0;
	_type = EF_FOCUS;
	_aCount = 0;

	return S_OK;
}

void EF_Focus::release()
{
	_img->release();
	SAFE_DELETE(_img);
}

void EF_Focus::update()
{
	//move

	controlFrame();
}

void EF_Focus::render()
{
#ifdef _DEBUG
	if (PRINTMANAGER->isDebug()) {
		Rectangle(getMemDC(), _body);
	}

	char str[128];
	sprintf_s(str, "state : %d", _state);
	TextOut(getMemDC(), _body.right + 10, _body.top, str, strlen(str));
	sprintf_s(str, "index : %d", _img->getFrameX());
	TextOut(getMemDC(), _body.right + 10, _body.top + 20, str, strlen(str));

	/*PRINTMANAGER->print(getMemDC(), _body.right + 10, _body.top + 20, "state : %d", _state);
	PRINTMANAGER->print(getMemDC(), _body.right + 10, _body.top + 40, "index : %d", _img->getFrameX());*/
#endif // _DEBUG

	if (_direct)
		_img->frameRender(getMemDC(), _x, _y);
	else
		_img->frameRender(getMemDC(), _x - _img->getFrameWidth(), _y);
}

void EF_Focus::updateRect()
{
	if (_direct)
		_body = RectMake(_x, _y, _img->getFrameWidth(), _img->getFrameHeight());
	else
		_body = RectMake(_x - _img->getFrameWidth(), _y, _img->getFrameWidth(), _img->getFrameHeight());
}

void EF_Focus::controlFrame()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX())
				_img->setFrameX(0);
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0)
				_img->setFrameX(_img->getMaxFrameX());
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}
