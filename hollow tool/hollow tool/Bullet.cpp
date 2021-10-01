#include "stdafx.h"
#include "Bullet.h"

HRESULT Bullet::init()
{
	_fire = new image;
	_fire->init("image/enemy/bullet/bullet.bmp",
		64, 64, 1, 1, true, RGB(255, 0, 255));
	_end = new image;
	_end->init("image/enemy/bullet/bulletend.bmp",
		528, 103, 6, 1, true, RGB(255, 0, 255));
	_endSide = new image;
	_endSide->init("image/enemy/bullet/bulletendside.bmp",
		606, 174, 6, 2, true, RGB(255, 0, 255));

	return S_OK;
}

HRESULT Bullet::init(float x, float y, float angle)
{
	_x = x;
	_y = y;
	_body = RectMakeCenter(_x, _y, BULLETSIZE, BULLETSIZE);

	_angle = angle;
	_speed = 600;
	_direct = 0;

	changeState(BULLETFIRE);

	return S_OK;
}

void Bullet::release()
{
	_fire->release();
	SAFE_DELETE(_fire);

	_end->release();
	SAFE_DELETE(_end);

	_endSide->release();
	SAFE_DELETE(_endSide);
}

void Bullet::update()
{
	if (_isFire) {
		//Ãæµ¹
		COLLISIONMANAGER->collisionBullet(this);

		bulletMove();
	}
	controlFrame();
}

void Bullet::render()
{
#ifdef _DEBUG
	if (PRINTMANAGER->isDebug()) {
		Rectangle(getMemDC() , _body);
	}

	char str[128];
	sprintf_s(str, "state : %d", _state);
	TextOut(getMemDC(), _body.right + 10, _body.top, str, strlen(str));
	sprintf_s(str, "index : %d", _img->getFrameX());
	TextOut(getMemDC(), _body.right + 10, _body.top + 20, str, strlen(str));

	/*PRINTMANAGER->print(getMemDC(), _body.right + 10, _body.top + 20, "state : %d", _state);
	PRINTMANAGER->print(getMemDC(), _body.right + 10, _body.top + 40, "index : %d", _img->getFrameX());*/
#endif // _DEBUG

	if(_state == BULLETEND)
		_img->frameRender(getMemDC(),
			_x - _img->getFrameWidth() / 2, _y - _img->getFrameHeight() / 2);
	else
		_img->frameRender(getMemDC(),
			_x - _img->getFrameWidth() / 2, _y - _img->getFrameHeight() / 2);
}

void Bullet::updateRect()
{
	_body = RectMakeCenter(_x, _y, BULLETSIZE, BULLETSIZE);
}

void Bullet::changeState(int state)
{
	_state = state;

	switch (state) {
	case BULLETFIRE:
		_img = _fire;
		_isFire = true;
		break;
	case BULLETEND:
		_img = _end;
		_isFire = false;
		break;
	case BULLETENDSIDE:
		_img = _endSide;
		_isFire = false;
		break;
	default:
		break;
	}

	if (_state >= BULLETFIRE) {
		_img->setFrameY(_direct);
		if (_direct)
			_img->setFrameX(_img->getMaxFrameX());
		else
			_img->setFrameX(0);
	}
}

void Bullet::bulletMove()
{
	_x += cosf(_angle)*_speed *TIMEMANAGER->getElapsedTime();
	_y += -sinf(_angle)*_speed *TIMEMANAGER->getElapsedTime();
}

void Bullet::controlFrame()
{
	if (_state == BULLETFIRE) return;

	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX())
				changeState(BULLETREADY);
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0)
				changeState(BULLETREADY);
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}

}
