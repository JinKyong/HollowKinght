#pragma once
#include "Enemy.h"
#include "Mace.h"

#define BOSSWIDTH	320
#define BOSSHEIGHT	380

#define BOSSGRAV	20
#define BOSSGRAVMAX	2000

enum BOSSSTATE {
	BOSSIDLE,
	BOSSTURN,
	BOSSATTACK_S,
	BOSSATTACK,
	BOSSATTACK_E,
	BOSSJUMP_S,
	BOSSJUMP,
	BOSSJUMP_E,
	BOSSJATTACK_S,
	BOSSJATTACK1,
	BOSSJATTACK2,
	BOSSJATTACK3,
	BOSSSTUNROLL,
	BOSSSTUNROLL_E,
	BOSSSTUNOPEN,
	BOSSDEATH,
	BOSSDEATH2,
	ENDBOSS
};

class FalseKnight : public Enemy
{
private:
	float _armorHP;
	float _attackCount;
	float _stunCount;
	float _gravity;
	float _openCount;
	float _dropCount;

	int _attackDelay;
	int _jumpDelay;

	Mace* _mace;

public:
	virtual HRESULT init(POINT position);
	virtual void release();
	virtual void update();
	virtual void render();

	virtual void changeState(int state);
	virtual void updateRect();
	virtual void updateDirect(int direct);
	virtual void minusHP(float damage);
	virtual void controlFrame();

	virtual stateImage getVstateImage() { return _vStateImage; };
	virtual stateImageIter getVIstateImage() { return _viStateImage; };

	virtual RECT getRect() { return _body; }
	virtual RECT getProve() { return _prove; }

	virtual float getX() { return _x; }
	virtual void setX(float x) { _x = x; }
	virtual float getY() { return _y; }
	virtual void setY(float y) { _y = y; }

	virtual void setDirect(int direct) { _direct = direct; }
	virtual int getDirect() { return _direct; }

	virtual void attack(Player* player);
	virtual void attackEnd() { _onAttack = false; }

	virtual void collision() { _collision = true; }

	virtual void death() { _isDeath = true; }
	virtual bool isDeath() { return _isDeath; }

	virtual float getHP() { return _hp; }

	virtual int getType() { return _type; }

	void frameRender();

	// 프레임 //
	void controlFrameIdle();
	void controlFrameTurn();
	void controlFrameAttackS();
	void controlFrameAttack();
	void controlFrameAttackE();
	void controlFrameJumpS();
	void controlFrameJump();
	void controlFrameJumpE();
	void controlFrameJAttackS();
	void controlFrameJAttack();
	void controlFrameStun();
	void controlFrameStunOpen();
	void controlFrameDeath();

	//공격패턴
	void changeDirect();
	void move();
	void jump();
	void jumpAttack();
	void attack();

	void updateMace();
	void equipMace(Mace* mace) { _mace = mace; }

	void bossCollision();
	void bossMove();
	void bossAttack();
};

