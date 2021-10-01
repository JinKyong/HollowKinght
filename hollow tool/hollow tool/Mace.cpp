#include "stdafx.h"
#include "Mace.h"

HRESULT Mace::init(POINT position)
{
	_x = position.x;
	_y = position.y;

	//상태
	_state = MACEIDLE;
	_direct = 0;
	_angle = 0;
	_onAttack = false;
	_collision = false;
	_isDeath = false;

	//스탯
	_hp = 100;
	_speed = 0;
	_type = WEAPON;

	//프레임
	_count = 0;

	_width = MACESIZE;
	_height = MACESIZE;

	return S_OK;
}

void Mace::release()
{
}

void Mace::update()
{
	_body = RectMakeCenter(_x, _y, _width, _height);
}

void Mace::render()
{
#ifdef _DEBUG
	if (PRINTMANAGER->isDebug()) {
		Rectangle(getMemDC(), _body);
	}

	char str[128];
	sprintf_s(str, "x : %f", _x);
	TextOut(getMemDC(), _body.right + 10, _body.top, str, strlen(str));
	sprintf_s(str, "y : %f", _y);
	TextOut(getMemDC(), _body.right + 10, _body.top + 20, str, strlen(str));
	sprintf_s(str, "HP : %f", _hp);
	TextOut(getMemDC(), _body.right + 10, _body.top + 40, str, strlen(str));

	//PRINTMANAGER->print(getMemDC(), _body.right + 10, _body.top, "state : %d", _state);
	//PRINTMANAGER->print(getMemDC(), _body.right + 10, _body.top + 20, "index : %d", _img->getFrameX());
	//PRINTMANAGER->print(getMemDC(), _body.right + 10, _body.top + 40, "direct : %d", _direct);
#endif // _DEBUG
}

void Mace::changeState(int state)
{
	if (_state >= ENDMACE) return;

	_state = state;
}

void Mace::updateRect()
{
}

void Mace::updateDirect(int direct)
{
}

void Mace::minusHP(float damage)
{
}

void Mace::controlFrame()
{
}

void Mace::attack(Player * player)
{
}

void Mace::updateMace(float x, float y, float width, float height)
{
	_x = x;
	_y = y;
	_width = width;
	_height = height;
}
