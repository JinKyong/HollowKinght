#include "stdafx.h"
#include "FalseKnight.h"
#include "Player.h"

HRESULT FalseKnight::init(POINT position)
{
	//몸체	
	_x = position.x;
	_y = position.y;
	_prove = RectMake(0, 0, CAMERAMANAGER->getBackScreenWidth(), CAMERAMANAGER->getBackScreenHeight());
	//weapon

	//상태 이미지
	for(int i=0; i<ENDBOSS; i++)
		_vStateImage.push_back(new image);

	_vStateImage[BOSSIDLE]->init("image/enemy/falseknight/idle.bmp", 3103, 780, 5, 2, true, RGB(7, 7, 7));
	_vStateImage[BOSSTURN]->init("image/enemy/falseknight/turn.bmp", 1096, 782, 2, 2, true, RGB(7, 7, 7));
	_vStateImage[BOSSATTACK_S]->init("image/enemy/falseknight/attackinit.bmp", 3492, 804, 6, 2, true, RGB(7, 7, 7));
	_vStateImage[BOSSATTACK]->init("image/enemy/falseknight/attack.bmp", 2077, 1188, 3, 2, true, RGB(7, 7, 7));
	_vStateImage[BOSSATTACK_E]->init("image/enemy/falseknight/attackend2.bmp", 3285, 1156, 5, 2, true, RGB(33, 44, 73));
	_vStateImage[BOSSJUMP_S]->init("image/enemy/falseknight/jumpinit.bmp", 1828, 766, 3, 2, true, RGB(7, 7, 7));
	_vStateImage[BOSSJUMP]->init("image/enemy/falseknight/jump.bmp", 2414, 678, 4, 2, true, RGB(7, 7, 7));
	_vStateImage[BOSSJUMP_E]->init("image/enemy/falseknight/jumpend.bmp", 1829, 766, 3, 2, true, RGB(7, 7, 7));
	_vStateImage[BOSSJATTACK_S]->init("image/enemy/falseknight/jumpattackinit.bmp", 3565, 848, 5, 2, true, RGB(7, 7, 7));
	_vStateImage[BOSSJATTACK1]->init("image/enemy/falseknight/jumpattack1.bmp", 1683, 1248, 2, 2, true, RGB(7, 7, 7));
	_vStateImage[BOSSJATTACK2]->init("image/enemy/falseknight/jumpattack2.bmp", 977, 1184, 2, 2, true, RGB(7, 7, 7));
	_vStateImage[BOSSJATTACK3]->init("image/enemy/falseknight/jumpattack3.bmp", 1236, 796, 2, 2, true, RGB(7, 7, 7));
	_vStateImage[BOSSSTUNROLL]->init("image/enemy/falseknight/stunroll.bmp", 3824, 1036, 5, 2, true, RGB(7, 7, 7));
	_vStateImage[BOSSSTUNROLL_E]->init("image/enemy/falseknight/stunrollend.bmp", 1675, 936, 4, 2, true, RGB(7, 7, 7));
	_vStateImage[BOSSSTUNOPEN]->init("image/enemy/falseknight/stunopen.bmp", 1980, 914, 5, 2, true, RGB(7, 7, 7));
	_vStateImage[BOSSDEATH]->init("image/enemy/falseknight/death.bmp", 2700, 468, 10, 2, true, RGB(33, 44, 73));
	_vStateImage[BOSSDEATH2]->init("image/enemy/falseknight/death2.bmp", 1092, 392, 4, 2, true, RGB(33, 44, 73));

	//상태
	_state = BOSSIDLE + 1;
	_direct = 1;
	_angle = 0;
	_isDeath = false;

	updateRect();

	//스탯
	_hp = 800;
	_armorHP = 1000;
	_speed = 1250;
	_type = BOSS;

	//프레임
	_count = 0;
	_img = _vStateImage[_state];
	_img->setFrameY(_direct);
	if (_direct)
		_img->setFrameX(_img->getMaxFrameX());
	else
		_img->setFrameX(0);

	_attackCount = _stunCount  = 0;
	_openCount = _dropCount = 0;

	_attackDelay = 1;
	_jumpDelay = 2;
	_gravity = 0;

	return S_OK;
}

void FalseKnight::release()
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
/*
	_bullet->release();
	SAFE_DELETE(_bullet);*/

	_vStateImage.clear();
}

void FalseKnight::update()
{
//	bossCollision();	//충돌
//	bossMove();			//이동
//	bossAttack();		//공격

	controlFrame();

	updateRect();
	updateMace();
}

void FalseKnight::render()
{
	frameRender();

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
	sprintf_s(str, "armor : %f", _armorHP);
	TextOut(getMemDC(), _body.right + 10, _body.top + 80, str, strlen(str));
	sprintf_s(str, "stunCount : %f", _stunCount);
	TextOut(getMemDC(), _body.right + 10, _body.top + 100, str, strlen(str));

	//PRINTMANAGER->print(getMemDC(), _body.right + 10, _body.top, "state : %d", _state);
	//PRINTMANAGER->print(getMemDC(), _body.right + 10, _body.top + 20, "index : %d", _img->getFrameX());
	//PRINTMANAGER->print(getMemDC(), _body.right + 10, _body.top + 40, "direct : %d", _direct);
#endif // _DEBUG

	/*
	if (_bullet->getState() < BULLETFIN)
		_bullet->render();*/
}

void FalseKnight::changeState(int state)
{
	if (state >= ENDBOSS) return;

	_state = state;
	_img = _vStateImage[_state];

	if (_direct) {
		if (_state >= BOSSDEATH) {
			_img->setFrameY(!_direct);
			_img->setFrameX(0);
		}
		else {
			_img->setFrameY(_direct);
			_img->setFrameX(_img->getMaxFrameX());
		}
	}
	else {
		if (_state >= BOSSDEATH) {
			_img->setFrameY(!_direct);
			_img->setFrameX(_img->getMaxFrameX());
		}
		else {
			_img->setFrameY(_direct);
			_img->setFrameX(0);
		}
	}

	_count = 0;
}

void FalseKnight::updateRect()
{
	float correctX = 0;
	float correctY = 0;
	float correctW = 0;
	float correctH = 0;

	switch (_state) {
	case BOSSATTACK:
		correctX = 50;
		break;
	case BOSSATTACK_E:
		correctX = -30;
		break;
	case BOSSJUMP:
	case BOSSJATTACK_S:
		correctY = 10;
		correctH = -30;
		break;
	case BOSSSTUNROLL_E:
		correctW = -BOSSWIDTH;
		correctH = -BOSSHEIGHT;
		break;
	case BOSSSTUNOPEN:
		correctX = 140;
		correctY = 130;
		correctW = -205;
		correctH = -265;
		break;
	case BOSSDEATH:
		correctX = 70;
		correctY = 120;
		correctW = -100;
		correctH = -240;
		break;
	case BOSSDEATH2:
		correctW = -BOSSWIDTH;
		correctH = -BOSSHEIGHT;
		break;
	}

	if(_direct)
		_body = RectMakeCenter(_x - correctX, _y + correctY,
			BOSSWIDTH + correctW, BOSSHEIGHT + correctH);
	else
		_body = RectMakeCenter(_x + correctX, _y + correctY,
			BOSSWIDTH + correctW, BOSSHEIGHT + correctH);
}

void FalseKnight::updateDirect(int direct)
{
	if (_direct != direct) {
		_direct = direct;

		_img->setFrameY(_direct);
		_img->setFrameX(_img->getMaxFrameX() - _img->getFrameX());
	}
}

void FalseKnight::minusHP(float damage)
{
	if (_state == BOSSSTUNOPEN)
		_hp -= damage;
	else
		_armorHP -= damage;

	if (_state < BOSSSTUNROLL && _armorHP <= 0) {
		if (_direct)
			_angle = PI / 4;
		else
			_angle = 3 * PI / 4;
		changeState(BOSSSTUNROLL);
	}

	if (_hp <= 0) {
		if (_direct)
			_angle = PI / 8;
		else
			_angle = 7 * PI / 8;
		changeState(BOSSDEATH);
		ITEMMANAGER->dropGeo(_x, _y, RND->getFromIntTo(30, 40));
	}
}

void FalseKnight::controlFrame()
{
	switch(_state){
	case BOSSIDLE:
		controlFrameIdle();
		break;
	case BOSSTURN:
		controlFrameTurn();
		break;
	case BOSSATTACK_S:
		controlFrameAttackS();
		break;
	case BOSSATTACK:
		controlFrameAttack();
		break;
	case BOSSATTACK_E:
		controlFrameAttackE();
		break;
	case BOSSJUMP_S:
		controlFrameJumpS();
		break;
	case BOSSJUMP:
		controlFrameJump();
		break;
	case BOSSJUMP_E:
		controlFrameJumpE();
		break;
	case BOSSJATTACK_S:
		controlFrameJAttackS();
		break;
	case BOSSJATTACK1:
	case BOSSJATTACK2:
	case BOSSJATTACK3:
		controlFrameJAttack();
		break;
	case BOSSSTUNROLL:
	case BOSSSTUNROLL_E:
		controlFrameStun();
		break;
	case BOSSSTUNOPEN:
		controlFrameStunOpen();
		break;
	case BOSSDEATH:
	case BOSSDEATH2:
		controlFrameDeath();
		break;
	}
}

void FalseKnight::attack(Player * player)
{
	_player = player;
	_onAttack = true;
}

void FalseKnight::frameRender()
{
	float correctX = 0;
	float correctY = 0;

	switch (_state) {
	case BOSSIDLE:
		correctX = 50;
	case BOSSTURN:
		correctX = 50;
		break;
	case BOSSATTACK_S:
		correctX = 100;
		correctY = 5;
		break;
	case BOSSATTACK:
		correctX = -140;
		correctY = -80;
		break;
	case BOSSATTACK_E:
		correctX = 5;
		correctY = -100;
		break;
	case BOSSJUMP_S:
		correctX = 35;
		correctY = 5;
		break;
	case BOSSJUMP:
		correctX = 100;
		correctY = 5;
		break;
	case BOSSJUMP_E:
		correctX = 40;
		correctY = 5;
		break;
	case BOSSJATTACK_S:
		correctX = 150;
		correctY = -30;
		break;
	case BOSSJATTACK1:
		correctX = 70;
		correctY = -80;
		break;
	case BOSSJATTACK2:
		correctX = 60;
		correctY = -100;
		break;
	case BOSSJATTACK3:
		correctX = 20;
		correctY = -5;
		break;
	case BOSSSTUNROLL:
		correctX = -20;
		correctY = -40;
		break;
	case BOSSSTUNROLL_E:
		correctX = 30;
		correctY = -35;
		break;
	case BOSSSTUNOPEN:
		correctX = -10;
		correctY = -39;
		break;
	case BOSSDEATH:
	case BOSSDEATH2:
		correctX = -80;
		correctY = 100;
		correctX = -80;
		correctY = 100;
		break;
	}

	if (_direct)
		_img->frameRender(getMemDC(),
			_x - _img->getFrameWidth() / 2 + correctX, _y - _img->getFrameHeight() / 2 + correctY);
	else
		_img->frameRender(getMemDC(),
			_x - _img->getFrameWidth() / 2 - correctX, _y - _img->getFrameHeight() / 2 + correctY);
}

void FalseKnight::controlFrameIdle()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT2) {
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

void FalseKnight::controlFrameTurn()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT2) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX())
				changeState(BOSSIDLE);
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0)
				changeState(BOSSIDLE);
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}

void FalseKnight::controlFrameAttackS()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT2) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX()) {
				_attackDelay--;
				_img->setFrameX(0);
				if (!_attackDelay) {
					changeState(BOSSATTACK);
					_attackDelay = 1;
				}
			}
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0) {
				_attackDelay--;
				_img->setFrameX(_img->getMaxFrameX());
				if (!_attackDelay) {
					changeState(BOSSATTACK);
					_attackDelay = 1;
				}
			}
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}

void FalseKnight::controlFrameAttack()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT2) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX())
				changeState(BOSSATTACK_E);
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0)
				changeState(BOSSATTACK_E);
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}

void FalseKnight::controlFrameAttackE()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT2) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX()) {
				changeState(BOSSIDLE);
				_attackCount = 0;
			}
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0) {
				changeState(BOSSIDLE);
				_attackCount = 0;
			}
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}

void FalseKnight::controlFrameJumpS()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT2) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX())
				changeState(BOSSJUMP);
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0)
				changeState(BOSSJUMP);
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}

void FalseKnight::controlFrameJump()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT2) {
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

void FalseKnight::controlFrameJumpE()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT2) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX()) {
				changeState(BOSSIDLE);
				_attackCount = 0;
			}
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0) {
				changeState(BOSSIDLE);
				_attackCount = 0;
			}
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}

void FalseKnight::controlFrameJAttackS()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT2) {
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

void FalseKnight::controlFrameJAttack()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT2) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX())
				if (_state == BOSSJATTACK3) {
					changeState(BOSSIDLE);
					_attackCount = 0;
				}
				else
					changeState(_state + 1);
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0)
				if (_state == BOSSJATTACK3) {
					changeState(BOSSIDLE);
					_attackCount = 0;
				}
				else
					changeState(_state + 1);
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}

void FalseKnight::controlFrameStun()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT2) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX()) {
				if(_state == BOSSSTUNROLL)
					_img->setFrameX(0);
				else {
					_openCount += TIMEMANAGER->getElapsedTime();
					if (_openCount > 0.25) {
						changeState(BOSSSTUNOPEN);
						_openCount = 0;
					}
				}
			}
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0) {
				if(_state== BOSSSTUNROLL)
					_img->setFrameX(_img->getMaxFrameX());
				else {
					_openCount += TIMEMANAGER->getElapsedTime();
					if (_openCount > 0.25) {
						changeState(BOSSSTUNOPEN);
						_openCount = 0;
					}
				}
			}
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}

void FalseKnight::controlFrameStunOpen()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT2) {
		switch (_direct) {
		case 0:
			if (_img->getFrameX() >= _img->getMaxFrameX()) {
				_stunCount += TIMEMANAGER->getElapsedTime();
				if (_stunCount > 1.0) {
					changeState(BOSSIDLE);
					_armorHP = 1000;
					_stunCount = 0;
				}
			}
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 1:
			if (_img->getFrameX() <= 0) {
				_stunCount += TIMEMANAGER->getElapsedTime();
				if (_stunCount > 1.0) {
					changeState(BOSSIDLE);
					_armorHP = 1000;
					_stunCount = 0;
				}
			}
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}

void FalseKnight::controlFrameDeath()
{
	_count += TIMEMANAGER->getElapsedTime();
	if (_count >= RENDERCOUNT2) {
		switch (_direct) {
		case 1:
			if (_img->getFrameX() >= _img->getMaxFrameX())
				if (_state == BOSSDEATH)
					changeState(_state + 1);
				else {
					_dropCount += TIMEMANAGER->getElapsedTime();
					if (_dropCount < 100 && _dropCount > 0.7) {
						if(!INVENTORYMANAGER->hook())
							ITEMMANAGER->dropHook(_x, _y);
						_dropCount = 100;
					}
				}
			else
				_img->setFrameX(_img->getFrameX() + 1);
			break;
		case 0:
			if (_img->getFrameX() <= 0)
				if (_state == BOSSDEATH)
					changeState(_state + 1);
				else {
					_dropCount += TIMEMANAGER->getElapsedTime();
					if (_dropCount < 100 && _dropCount > 0.7) {
						if (!INVENTORYMANAGER->hook())
							ITEMMANAGER->dropHook(_x, _y);
						_dropCount = 100;
					}
				}
			else
				_img->setFrameX(_img->getFrameX() - 1);
			break;
		}

		_count = 0;
	}
}

void FalseKnight::changeDirect()
{
	int tmp;

	if (PI / 2 < _angle && _angle < 3 * PI / 2)
		tmp = 1;
	else if (0 <= _angle < PI / 2 && 3 * PI / 2 < _angle < PI2)
		tmp = 0;

	if (_direct != tmp) {
		_direct = tmp;
		changeState(BOSSTURN);
	}
}

void FalseKnight::move()
{
	_gravity += BOSSGRAV;
	if (_gravity >= BOSSGRAVMAX)
		_gravity = BOSSGRAVMAX;

	_x += cosf(_angle) * _speed * TIMEMANAGER->getElapsedTime() / 5;
	_y += -sinf(_angle) * _speed * TIMEMANAGER->getElapsedTime();

	_y += _gravity * TIMEMANAGER->getElapsedTime();
}

void FalseKnight::jump()
{
	if (_direct)
		_angle = RND->getFromFloatTo(TORADIAN(110), TORADIAN(135));
	else
		_angle = RND->getFromFloatTo(TORADIAN(45), TORADIAN(70));

	changeState(BOSSJUMP_S);
}

void FalseKnight::jumpAttack()
{
	if (_direct)
		_angle = RND->getFromFloatTo(TORADIAN(100), TORADIAN(120));
	else
		_angle = RND->getFromFloatTo(TORADIAN(60), TORADIAN(80));

	changeState(BOSSJATTACK_S);
}

void FalseKnight::attack()
{
	changeState(BOSSATTACK_S);
}

void FalseKnight::updateMace()
{
	float correctX = 0;
	float correctY = 0;
	float width = MACESIZE;
	float height = MACESIZE;

	switch (_state) {
	case BOSSIDLE:
		correctX = 285;
		break;
	case BOSSTURN:
		correctX = 240;
		width += 40;
		break;
	case BOSSATTACK_S:
		break;
	case BOSSATTACK:
		break;
	case BOSSATTACK_E:
		break;
	case BOSSJUMP_S:
		break;
	case BOSSJUMP:
		break;
	case BOSSJUMP_E:
		break;
	case BOSSJATTACK_S:
		break;
	case BOSSJATTACK1:
		break;
	case BOSSJATTACK2:
		break;
	case BOSSJATTACK3:
		break;
	case BOSSSTUNROLL:
		break;
	case BOSSSTUNROLL_E:
		break;
	case BOSSSTUNOPEN:
		break;
	case BOSSDEATH:
		break;
	case BOSSDEATH2:
		break;
	}

	if(_direct)
		_mace->updateMace(_x + correctX, _y + correctY, width, height);
	else
		_mace->updateMace(_x - correctX, _y + correctY, width, height);
}

void FalseKnight::bossCollision()
{
	if (_collision) {
		if (_state == BOSSJUMP) {
			changeState(BOSSJUMP_E);

			_gravity = 0;
		}
		else if (_state == BOSSJATTACK_S) {
			changeState(BOSSJATTACK1);

			_gravity = 0;
		}
		else if (_state == BOSSSTUNROLL) {
			changeState(BOSSSTUNROLL_E);

			_gravity = 0;
		}

		_collision = false;
	}
}

void FalseKnight::bossMove()
{
	if (_state == BOSSJUMP || _state == BOSSJATTACK_S ||
		_state == BOSSSTUNROLL || _state == BOSSDEATH) {
		move();
	}
}

void FalseKnight::bossAttack()
{
	if (_state == BOSSIDLE) {
		if (_onAttack) {
			_angle = getAngle(_x, _y, _player->getX(), _player->getY());
			changeDirect();
		}

		_attackCount += TIMEMANAGER->getElapsedTime();
		if (_attackCount > 1.5) {
			//공격
			int num = RND->getInt(3);
			switch (num) {
			case 0:
				jump();
				break;
			case 1:
				attack();
				break;
			case 2:
				jumpAttack();
				break;
			}

		}
	}
}
