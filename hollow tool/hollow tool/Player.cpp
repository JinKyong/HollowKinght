#include "stdafx.h"
#include "Player.h"
#include "State.h"

HRESULT Player::init()
{
	//¸öÃ¼
	_x = 1000;
	_y = 0;
	_head = RectMakeCenter(_x, _y, HWIDTH, HHEIGHT);
	_body = RectMakeCenter(_x, _y + HHEIGHT / 2 + BHEIGHT / 2, BWIDTH, BHEIGHT);
	_direct = 0;

	//½ºÅÈ
	_hp = 5;
	_speed = 500;
	_jumpPower = 600;
	_dashPower = 2000;
	_damage = 15;
	_soul = 0;

	//»óÅÂ
	_gravity = 0;
	_jumpCount = 1;
	_dashCount = 1;

	_stun = false;
	_stunCount = 0;
	_alpha = 255;

	_onPortal = false;
	_getInPortal = false;

	_isDeath = false;

	//ÀÌÆåÆ®
	_effect = new effectManager;
	_effect->init();

	STATEMANAGER->init(this);

	return S_OK;
}

void Player::release()
{
	_effect->release();
	SAFE_DELETE(_effect);

	STATEMANAGER->release();
	STATEMANAGER->releaseSingleton();
}

void Player::update()
{
	if (_stun) {
		_stunCount += TIMEMANAGER->getElapsedTime();
		_alpha = _alpha == 0 ? 255 : 0;

		if (_stunCount > STUNINTERVAL) {
			_stunCount = 0;
			_stun = false;
		}
	}

	STATEMANAGER->update();

	_effect->update();

	COLLISIONMANAGER->collisionPlayer();
	STATEMANAGER->getCurrentState()->updateRect();
}

void Player::render()
{
	if (PRINTMANAGER->isDebug()) {
		Rectangle(getMemDC(), _head);
		Rectangle(getMemDC(), _body);

		char str[128];
		sprintf_s(str, "x : %f", _x);
		TextOut(getMemDC(), _x - 100, _y - 90, str, strlen(str));
		sprintf_s(str, "y : %f", _y);
		TextOut(getMemDC(), _x - 100, _y - 70, str, strlen(str));
		sprintf_s(str, "direct : %d", _direct);
		TextOut(getMemDC(), _x - 100, _y - 50, str, strlen(str));

		//PRINTMANAGER->print(getMemDC(), _x - 100, _y - 90, "x : %f", _x);
		//PRINTMANAGER->print(getMemDC(), _x - 100, _y - 70, "y : %f", _y);
		//PRINTMANAGER->print(getMemDC(), _x - 100, _y - 50, "direct : %d", _direct);

		TIMEMANAGER->render(getMemDC(), _x - 100, _y - 200);
	}

	STATEMANAGER->render(getMemDC());
	_effect->render();
}

void Player::changeHP(int num)
{
	_hp += num;

	if (_hp > HPMAX)		_hp = HPMAX;
	else if (_hp < 0)		_hp = 0;

	UIMANAGER->setHP(_hp);
}

void Player::changeSoul(float num)
{
	_soul += num;

	if (_soul > SOULMAX)	_soul = SOULMAX;
	else if (_soul < 0)		_soul = 0;

	UIMANAGER->setSoul(_soul);
}

void Player::move(int direct, bool correction)
{
	float distance = _speed;
	if (correction)
		distance = _speed * TIMEMANAGER->getElapsedTime();

	switch (direct) {
	case RIGHT:
		_x += distance;
		break;
	case LEFT:
		_x -= distance;
		break;
	case UP:
		_y -= distance;
		break;
	case DOWN:
		_y += distance;
		break;
	default:
		break;
	}
}

void Player::moveS(int direct, float speed, bool correction)
{
	float distance = speed;
	if (correction)
		distance = speed * TIMEMANAGER->getElapsedTime();

	switch (direct) {
	case RIGHT:
		_x += distance;
		break;
	case LEFT:
		_x -= distance;
		break;
	case UP:
		_y -= distance;
		break;
	case DOWN:
		_y += distance;
		break;
	default:
		break;
	}
}

void Player::updateRect()
{
	_head = RectMakeCenter(_x, _y, HWIDTH, HHEIGHT);
}

void Player::resetJump()
{
	_jumpCount = 1 + INVENTORYMANAGER->wing();
}

bool Player::jump()
{
	if (_jumpCount) {
		_jumpCount--;
		return true;
	}

	return false;
}

void Player::resetDash()
{
	_dashCount = 1;
}

bool Player::dash()
{
	if (_dashCount) {
		_dashCount--;
		return true;
	}

	return false;
}

bool Player::wall()
{
	if (INVENTORYMANAGER->hook())
		return true;

	return false;
}
