#include "stdafx.h"
#include "Mush.h"
#include "Player.h"

HRESULT Mush::init(POINT position)
{
	//몸체
	_x = position.x;
	_y = position.y;

	//상태 이미지
	_vStateImage.push_back(new image);
	_vStateImage.push_back(new image);
	_vStateImage.push_back(new image);
	_vStateImage.push_back(new image);

	_vStateImage[0]->init("image/enemy/mush/walk.bmp", 500, 224, 4, 2, true, RGB(255, 0, 255));
	_vStateImage[1]->init("image/enemy/mush/turn.bmp", 246, 214, 2, 2, true, RGB(255, 0, 255));
	_vStateImage[2]->init("image/enemy/mush/deathair.bmp", 270, 266, 2, 2, true, RGB(255, 0, 255));
	_vStateImage[3]->init("image/enemy/mush/death.bmp", 453, 224, 3, 2, true, RGB(255, 0, 255));

	//상태
	_state = MUSHWALK;
	_direct = 0;
	_angle = 0;
	_isDeath = false;

	updateRect();

	//스탯
	_hp = 30;
	_speed = 300;
	_type = MINION;

	//프레임
	_count = 0;
	_img = _vStateImage[_state];
	_img->setFrameY(_direct);
	if (_direct)
		_img->setFrameX(_img->getMaxFrameX());
	else
		_img->setFrameX(0);

	return S_OK;
}

void Mush::release()
{
	for (_viStateImage = _vStateImage.begin(); _viStateImage != _vStateImage.end();) {
		if ((*_viStateImage) != NULL) {
			(*_viStateImage)->release();
			SAFE_DELETE((*_viStateImage));
			_viStateImage = _vStateImage.erase(_viStateImage);
		}
		else
			++_viStateImage;
	}

	_vStateImage.clear();
}

void Mush::update()
{
	if (_collision) {
		setDirect(!_direct);
		changeState(MUSHTURN);
		_collision = false;
	}
	if(_state == MUSHWALK)
		walk();

	controlFrame();
}

void Mush::render()
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
	sprintf_s(str, "direct : %d", _direct);
	TextOut(getMemDC(), _body.right + 10, _body.top + 40, str, strlen(str));
	sprintf_s(str, "HP : %f", _hp);
	TextOut(getMemDC(), _body.right + 10, _body.top + 60, str, strlen(str));

	//PRINTMANAGER->print(getMemDC(), _body.right + 10, _body.top, "state : %d", _state);
	//PRINTMANAGER->print(getMemDC(), _body.right + 10, _body.top + 20, "index : %d", _img->getFrameX());
#endif // _DEBUG

	_img->frameRender(getMemDC(),
		_x - _img->getFrameWidth() / 2, _y - _img->getFrameHeight() / 2);
}

void Mush::changeState(int state)
{
	if (state >= ENDMUSH) return;

	_state = state;
	_img = _vStateImage[_state];

	_img->setFrameY(_direct);
	if (_direct)
		_img->setFrameX(_img->getMaxFrameX());
	else
		_img->setFrameX(0);

	_count = 0;
}

void Mush::updateRect()
{
	_body = RectMakeCenter(_x, _y, MUSHWIDTH, MUSHHEIGHT);
	_prove = RectMakeCenter(_x, _y, 0, 0);
}

void Mush::updateDirect(int direct)
{
	if (_direct != direct) {
		_direct = direct;

		_img->setFrameY(_direct);
		_img->setFrameX(_img->getMaxFrameX() - _img->getFrameX());
	}
}

void Mush::minusHP(float damage)
{
	_hp -= damage;

	if (_hp <= 0) {
		changeState(MUSHDEATH);
		ITEMMANAGER->dropGeo(_x, _y, RND->getFromIntTo(3, 6));
	}
}

void Mush::controlFrame()
{
	switch (_state) {
	case MUSHWALK:
		controlFrameWalk();
		break;
	case MUSHTURN:
		controlFrameTurn();
		break;
	case MUSHDEATH:
		controlFrameDeath();
		break;
	case MUSHDEATHAIR:
		controlFrameDeathAir();
		break;
	}
}

void Mush::attack(Player * player)
{
}

void Mush::controlFrameWalk()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT * 2) {
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

void Mush::controlFrameTurn()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX())
				changeState(MUSHWALK);
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0) 
				changeState(MUSHWALK);
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}

void Mush::controlFrameDeathAir()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT*2) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX())
				changeState(MUSHDEATH);
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0)
				changeState(MUSHDEATH);
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}

void Mush::controlFrameDeath()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT*2) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX())
				_isDeath = true;
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0)
				_isDeath = true;
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}

void Mush::walk()
{
	if (_direct)	_x -= _speed * TIMEMANAGER->getElapsedTime();
	else			_x += _speed * TIMEMANAGER->getElapsedTime();
}
