#include "stdafx.h"
#include "Bee.h"
#include "Player.h"

HRESULT Bee::init(POINT position)
{
	//몸체	
	_x = position.x;
	_y = position.y;

	//상태 이미지
	_vStateImage.push_back(new image);
	_vStateImage.push_back(new image);
	_vStateImage.push_back(new image);
	_vStateImage.push_back(new image);
/*
	_vStateImage[0]->init("image/enemy/falseknight/death.bmp", 2700, 468, 10, 2, true, RGB(33, 44, 73));
	_vStateImage[1]->init("image/enemy/falseknight/death2.bmp", 1092, 392, 4, 2, true, RGB(33, 44, 73));
	_vStateImage[2]->init("image/enemy/falseknight/stunopen.bmp", 1980, 914, 5, 2, true, RGB(7, 7, 7));
	_vStateImage[3]->init("image/enemy/falseknight/stunopen.bmp", 1980, 914, 5, 2, true, RGB(7, 7, 7));*/
	_vStateImage[0]->init("image/enemy/bee/idle.bmp", 852, 244, 6, 2, true, RGB(255, 0, 255));
	_vStateImage[1]->init("image/enemy/bee/turn.bmp",	320, 244, 2, 2, true, RGB(255, 0, 255));
	_vStateImage[2]->init("image/enemy/bee/attack.bmp",	1848, 234, 12, 2, true, RGB(255, 0, 255));
	_vStateImage[3]->init("image/enemy/bee/death.bmp", 906, 266, 6, 2, true, RGB(255, 0, 255));

	//상태
	_state = BEEIDLE;
	_direct = 0;
	_angle = 0;
	_isDeath = false;

	//스탯
	_hp = 50;
	_speed = 150;
	_type = MINION;

	//프레임
	_count = 0;
	_img = _vStateImage[_state];
	_img->setFrameY(_direct);
	if (_direct)
		_img->setFrameX(_img->getMaxFrameX());
	else
		_img->setFrameX(0);

	_angleCount = _flyCount = 0;
	_fireCount = 5;
	_flying = _onAttack = false;

	updateRect();

	_bullet = new Bullet;
	_bullet->init();

	return S_OK;
}

void Bee::release()
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

	_bullet->release();
	SAFE_DELETE(_bullet);

	_vStateImage.clear();
}

void Bee::update()
{
	fly();

	if (_bullet->getState() > BULLETREADY) {
		_bullet->update();
		_bullet->updateRect();
	}

	controlFrame();
}

void Bee::render()
{
#ifdef _DEBUG
	if (PRINTMANAGER->isDebug()) {
		Rectangle(getMemDC(), _prove);
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
	sprintf_s(str, "Attack : %d", _onAttack);
	TextOut(getMemDC(), _body.right + 10, _body.top + 80, str, strlen(str));

	//PRINTMANAGER->print(getMemDC(), _body.right + 10, _body.top, "state : %d", _state);
	//PRINTMANAGER->print(getMemDC(), _body.right + 10, _body.top + 20, "index : %d", _img->getFrameX());
	//PRINTMANAGER->print(getMemDC(), _body.right + 10, _body.top + 40, "direct : %d", _direct);
#endif // _DEBUG

	_img->frameRender(getMemDC(),
		_x - _img->getFrameWidth() / 2, _y - _img->getFrameHeight() / 2);

	if (_bullet->getState() > BULLETREADY)
		_bullet->render();
}

void Bee::changeState(int state)
{
	if (state >= ENDBEE) return;

	_state = state;
	_img = _vStateImage[_state];

	_img->setFrameY(_direct);
	if (_direct)
		_img->setFrameX(_img->getMaxFrameX());
	else
		_img->setFrameX(0);

	_count = 0;
}

void Bee::updateRect()
{
	if (_direct) {
		_body = RectMakeCenter(_x - 5, _y + 10, BEEWIDTH, BEEHEIGHT);
		_prove = RectMakeCenter(_x - 2 * BEEWIDTH, _y + 10, BEEWIDTH * 5, BEEHEIGHT * 4);
	}
	else {
		_body = RectMakeCenter(_x + 5, _y + 10, BEEWIDTH, BEEHEIGHT);
		_prove = RectMakeCenter(_x + 2 * BEEWIDTH, _y + 10, BEEWIDTH * 5, BEEHEIGHT * 4);
	}
}

void Bee::updateDirect(int direct)
{
	if (_direct != direct) {
		_direct = direct;

		_img->setFrameY(_direct);
		_img->setFrameX(_img->getMaxFrameX() - _img->getFrameX());
	}
}

void Bee::minusHP(float damage)
{
	_hp -= damage;

	if (_hp <= 0) {
		changeState(BEEDEATH);
		ITEMMANAGER->dropGeo(_x, _y, RND->getFromIntTo(5, 9));
	}
}

void Bee::controlFrame()
{
	switch (_state) {
	case BEEIDLE:
		controlFrameIdle();
		break;
	case BEETURN:
		controlFrameTurn();
		break;
	case BEEATTACK:
		controlFrameAttack();
		break;
	case BEEDEATH:
		controlFrameDeath();
		break;
	}
}

void Bee::attack(Player * player)
{
	_player = player;
	_onAttack = true;
}

void Bee::controlFrameIdle()
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

void Bee::controlFrameTurn()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT * 2) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX())
				changeState(BEEIDLE);
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0)
				changeState(BEEIDLE);
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}

void Bee::controlFrameAttack()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT * 2) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX())
				changeState(BEEIDLE);
			else {
				if (_img->getFrameX() == 9)
					fire();
				_img->setFrameX(_img->getFrameX() + 1);
			}
			break;
		case 1:
			if (_img->getFrameX() <= 0)
				changeState(BEEIDLE);
			else {
				if (_img->getFrameX() == 2)
					fire();
				_img->setFrameX(_img->getFrameX() - 1);
			}
			break;
		}

		_count = 0;
	}
}

void Bee::controlFrameDeath()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT * 2) {
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

void Bee::changeDirect()
{
	int tmp;

	if (PI / 2 < _angle && _angle < 3 * PI / 2)
		tmp = 1;
	else if (0 <= _angle < PI / 2 && 3 * PI / 2 < _angle < PI2)
		tmp = 0;

	if (_direct != tmp) {
		_direct = tmp;
		changeState(BEETURN);
	}
}

void Bee::fly()
{
	if (_onAttack) {
		_angle = getAngle(_x, _y, _player->getX(), _player->getY());
		changeDirect();

		_flying = true;
		if (getDistance(_x, _y, _player->getX(), _player->getY()) < 250)
			_angle = -_angle;

		_fireCount += TIMEMANAGER->getElapsedTime();
		if (_fireCount > 5) {
			if (_state == BEEIDLE && _bullet->getState() == BULLETREADY) {
				changeState(BEEATTACK);
			}
		}
	}
	else {
		_angleCount++;
		if (_angleCount % RND->getFromIntTo(1, 1200) == 0) {
			_angle = RND->getFloat(PI2);
			changeDirect();
			_angleCount = 0;
		}

		_flyCount++;
		if (_flyCount % RND->getFromIntTo(1, 800) == 0) {
			_flying = !_flying;
			_flyCount = 0;
		}
	}

	if (_flying) {
		_x += cosf(_angle) * _speed * TIMEMANAGER->getElapsedTime();
		_y += -sinf(_angle) * _speed * TIMEMANAGER->getElapsedTime();
	}
}

void Bee::fire()
{
	if (_bullet->getState() == BULLETREADY) {
		_angle = getAngle(_x, _y, _player->getX(), _player->getY());

		switch (_direct) {
		case 0:
			_bullet->init(_body.right, _y, _angle);
			break;
		case 1:
			_bullet->init(_body.left, _y, _angle);
			break;
		}

		_fireCount = 0;
	}
}
