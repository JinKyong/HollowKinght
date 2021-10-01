#pragma once
#include "Enemy.h"

#define BEETLEWIDTH		120
#define BEETLEHEIGHT	175

enum BEETLESTATE {
	BEETLEIDLE,
	BEETLEWALK,
	BEETLETURN,
	BEETLEATTACK_S,
	BEETLEATTACK,
	BEETLEATTACK_E,
	BEETLEDEATH,
	ENDBEETLE
};

class Beetle : public Enemy
{
private:
	int _angleCount, _moveCount;
	bool _moving;

	float _attackY;
	float _tmpBottom;

public:
	Beetle() {};
	~Beetle() {};

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

	void controlFrameIdle();
	void controlFrameWalk();
	void controlFrameTurn();
	void controlFrameAttackS();
	void controlFrameAttack();
	void controlFrameAttackE();
	void controlFrameDeath();

	void changeDirect();
	void move();
	void walk();
	void attack();
};

