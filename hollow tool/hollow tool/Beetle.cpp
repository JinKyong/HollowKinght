#include "stdafx.h"
#include "Beetle.h"
#include "Player.h"

HRESULT Beetle::init(POINT position)
{
	//몸체
	_x = position.x;
	_y = position.y;

	//상태 이미지
	_vStateImage.push_back(new image);
	_vStateImage.push_back(new image);
	_vStateImage.push_back(new image);
	_vStateImage.push_back(new image);
	_vStateImage.push_back(new image);
	_vStateImage.push_back(new image);
	_vStateImage.push_back(new image);

	_vStateImage[0]->init("image/enemy/beetle/idle.bmp", 708, 384, 6, 2, true, RGB(255, 0, 255));
	_vStateImage[1]->init("image/enemy/beetle/walk.bmp", 973, 364, 7, 2, true, RGB(255, 0, 255));
	_vStateImage[2]->init("image/enemy/beetle/turn.bmp", 208, 370, 2, 2, true, RGB(255, 0, 255));
	_vStateImage[3]->init("image/enemy/beetle/attackinit.bmp", 855, 380, 5, 2, true, RGB(255, 0, 255));
	_vStateImage[4]->init("image/enemy/beetle/attack.bmp", 794, 240, 4, 2, true, RGB(255, 0, 255));
	_vStateImage[5]->init("image/enemy/beetle/attackend.bmp", 132, 326, 1, 2, true, RGB(255, 0, 255));
	_vStateImage[6]->init("image/enemy/beetle/death.bmp", 1710, 262, 9, 2, true, RGB(255, 0, 255));

	//상태
	_state = BEETLEIDLE;
	_direct = 0;
	_angle = 0;
	_isDeath = false;

	updateRect();

	//스탯
	_hp = 100;
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

	_angleCount = _moveCount = 0;
	_moving = false;
	_attackY = _tmpBottom = 0;

	return S_OK;
}

void Beetle::release()
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

void Beetle::update()
{
	if (_collision) {
		_collision = false;
		if (_state == BEETLEATTACK) {
			changeState(BEETLEATTACK_E);
			_onAttack = false;
		}
	}
	move();

	controlFrame();
}

void Beetle::render()
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

	//PRINTMANAGER->print(getMemDC(), _body.right + 10, _body.top, "state : %d", _state);
	//PRINTMANAGER->print(getMemDC(), _body.right + 10, _body.top + 20, "index : %d", _img->getFrameX());
#endif // _DEBUG
	if (_state == BEETLEATTACK || _state == BEETLEDEATH) {
		_img->frameRender(getMemDC(),
			_x - _img->getFrameWidth() / 2, _attackY - _img->getFrameHeight() / 2);
	}
	else {
		_img->frameRender(getMemDC(),
			_x - _img->getFrameWidth() / 2, _y - _img->getFrameHeight() / 2);
	}

	
}

void Beetle::changeState(int state)
{
	if (state >= ENDBEETLE) return;

	_state = state;
	_img = _vStateImage[_state];
	_tmpBottom = _body.bottom;

	_img->setFrameY(_direct);
	if (_direct)
		_img->setFrameX(_img->getMaxFrameX());
	else
		_img->setFrameX(0);

	_count = 0;
}

void Beetle::updateRect()
{
	if (_state == BEETLEATTACK || _state == BEETLEDEATH) {
		_attackY = _tmpBottom - _img->getFrameHeight() / 2;

		_body = RectMakeCenter(_x, _attackY, BEETLEHEIGHT, BEETLEWIDTH);
		_prove = RectMakeCenter(_x , _y , 0, 0);
	}
	else {
		_body = RectMakeCenter(_x, _y, BEETLEWIDTH, BEETLEHEIGHT);
		if(_direct)
			_prove = RectMakeCenter(_x - 1.5 * BEETLEWIDTH, _y, BEETLEWIDTH * 3, BEETLEHEIGHT);
		else
			_prove = RectMakeCenter(_x + 1.5 * BEETLEWIDTH, _y, BEETLEWIDTH * 3, BEETLEHEIGHT);
	}
}

void Beetle::updateDirect(int direct)
{
	if (_direct != direct) {
		_direct = direct;

		_img->setFrameY(_direct);
		_img->setFrameX(_img->getMaxFrameX() - _img->getFrameX());
	}
}

void Beetle::minusHP(float damage)
{
	_hp -= damage;

	if (_hp <= 0) {
		changeState(BEETLEDEATH);
		ITEMMANAGER->dropGeo(_x, _y, RND->getFromIntTo(7, 13));
	}
}

void Beetle::controlFrame()
{
	switch (_state) {
	case BEETLEIDLE:
		controlFrameIdle();
		break;
	case BEETLEWALK:
		controlFrameWalk();
		break;
	case BEETLETURN:
		controlFrameTurn();
		break;
	case BEETLEATTACK_S:
		controlFrameAttackS();
		break;
	case BEETLEATTACK:
		controlFrameAttack();
		break;
	case BEETLEATTACK_E:
		controlFrameAttackE();
		break;
	case BEETLEDEATH:
		controlFrameDeath();
		break;
	}
}

void Beetle::attack(Player * player)
{
	_player = player;
	_onAttack = true;
}

void Beetle::controlFrameIdle()
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

void Beetle::controlFrameWalk()
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

void Beetle::controlFrameTurn()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT * 2) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX())
				changeState(BEETLEIDLE);
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0)
				changeState(BEETLEIDLE);
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}

void Beetle::controlFrameAttackS()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT * 2) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX())
				changeState(BEETLEATTACK);
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0)
				changeState(BEETLEATTACK);
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}

void Beetle::controlFrameAttack()
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

void Beetle::controlFrameAttackE()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT * 2) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX())
				changeState(BEETLEIDLE);
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0)
				changeState(BEETLEIDLE);
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}

void Beetle::controlFrameDeath()
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

void Beetle::changeDirect()
{
	int tmp;

	if (PI / 2 < _angle && _angle < 3 * PI / 2)
		tmp = 1;
	else if (0 <= _angle < PI / 2 && 3 * PI / 2 < _angle < PI2)
		tmp = 0;

	if (_direct != tmp) {
		_direct = tmp;
		changeState(BEETLETURN);
	}
}

void Beetle::move()
{
	if (_onAttack) {
		_moving = false;
		if (_state < BEETLEATTACK_S)
			changeState(BEETLEATTACK_S);
		if(_state == BEETLEATTACK)
			attack();
	}
	else {
		_angleCount++;
		if (_angleCount % RND->getFromIntTo(1, 1200) == 0) {
			_moving = false;
			_angle = RND->getFloat(PI2);
			changeDirect();
			_angleCount = 0;
		}

		_moveCount++;
		if (_moveCount % RND->getFromIntTo(1, 600) == 0) {
			if (_moving) {
				if (!RND->getInt(4))
					_moving = !_moving;
			}
			_moving = !_moving;
			if (_moving)
				changeState(BEETLEWALK);
			else
				changeState(BEETLEIDLE);
			_moveCount = 0;
		}
	}

	if (_moving) walk();
}

void Beetle::walk()
{
	if (_direct)
		_x -= _speed * TIMEMANAGER->getElapsedTime();
	else 
		_x += _speed * TIMEMANAGER->getElapsedTime();
}

void Beetle::attack()
{
	if (_direct)
		_x -= 3 * _speed * TIMEMANAGER->getElapsedTime();
	else
		_x += 3 * _speed * TIMEMANAGER->getElapsedTime();
}
