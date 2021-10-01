#pragma once
#include "gameNode.h"
#include "effectManager.h"

#define HWIDTH	55
#define HHEIGHT	82
#define BWIDTH	30
#define BHEIGHT	45

#define GRAVITY	20
#define GRAVMAX	1200

#define HPMAX		5
#define SOULMAX	100.0f

#define STUNINTERVAL	2.0f

class Player : public gameNode
{
private:
	//몸체
	float _x, _y;
	RECT _head;
	RECT _body;
	int _direct;

	//스탯
	int _hp;
	float _speed;
	float _jumpPower;
	float _dashPower;
	float _damage;
	float _soul;

	//상태
	float _gravity;
	int _jumpCount;
	int _dashCount;

	bool _stun;
	float _stunCount;
	int _alpha;

	bool _onPortal;
	bool _getInPortal;

	bool _isDeath;

	//이펙트
	effectManager* _effect;

public:
	Player() {};
	~Player() {};

	virtual HRESULT init();			//초기화 함수
	virtual void release();			//메모리 해제 함수
	virtual void update();			//연산 함수
	virtual void render();			//그리기 함수

	//스탯 증감
	void changeHP(int num);
	void changeSoul(float num);

	//동작
	void move(int direct, bool correction = true);
	void moveS(int direct, float speed, bool correction = true);

	void updateRect();
	
	void resetJump();
	bool jump();

	void resetDash();
	bool dash();

	bool wall();
	
	//================= 접근자 =================//

	void setX(float x) { _x = x; }
	float getX() { return _x; }
	void setY(float y) { _y = y; }
	float getY() { return _y; }

	void setHead(float x, float y, int width, int height) {
		_head = RectMakeCenter(x, y, width, height);
	}
	RECT getHead() { return _head; }
	void setBody(float x, float y, int width, int height) {
		_body = RectMakeCenter(x, y, width, height);
	}
	RECT getBody() { return _body; }

	void setDirect(int direct) { _direct = direct; }
	int getDirect() { return _direct; }

	int getHP() { return _hp; }
	int getHPMax() { return HPMAX; }

	void setSpeed(float speed) { _speed = speed; }
	float getSpeed() { return _speed; }

	float getJumpPower() { return _jumpPower; }
	float getDashPower() { return _dashPower; }

	void setDamage(float damage) { _damage = damage; }
	float getDamage() { return _damage; }

	float getSoul() { return _soul; }
	float getSoulMax() { return SOULMAX; }

	void setGravity(float gravity) { _gravity = gravity; }
	float getGravity() { return _gravity; }

	int getJumpMax() { return 1 + INVENTORYMANAGER->wing(); }

	int getJumpCount() { return _jumpCount; }
	int getDashCount() { return _dashCount; }

	void stun() { _stun = true; }
	bool isStun() { return _stun; }

	int getAlpha() { return _alpha; }

	void setPortal(bool portal) { _onPortal = portal; }
	bool getPortal() { return _onPortal; }

	void getIn() { _getInPortal = true; }
	void getOut() { _getInPortal = false; }
	bool isGetIn() { return _getInPortal; }

	void death() { _isDeath = true; }
	bool isDeath() { return _isDeath; }

	effectManager* getEM() { return _effect; }
};