#pragma once
#include "gameNode.h"
#include <vector>

class Player;

//#define RENDERCOUNT	0.05
enum ENEMYTYPE {
	MINION,
	BOSS,
	WEAPON,
	ENDENEMYTYPE
};

class Enemy : public gameNode
{
protected:
	typedef vector<image*>				stateImage;
	typedef vector<image*>::iterator	stateImageIter;

protected:
	Player* _player;

	//몸체
	RECT _body;
	RECT _prove;
	float _x, _y;

	//상태 이미지
	stateImage		_vStateImage;
	stateImageIter	_viStateImage;
	//상태
	int _state;
	int _direct;
	float _angle;
	bool _onAttack;
	bool _collision;
	bool _isDeath;

	//스탯
	float _hp;
	float _speed;
	int _type;

	//프레임
	float _count;
	image* _img;

public:
	Enemy() {};
	~Enemy() {};

	virtual HRESULT init(POINT position) = 0;
	virtual void release() = 0;
	virtual void update() = 0;
	virtual void render() = 0;

	virtual void changeState(int state) = 0;
	virtual void updateRect() = 0;
	virtual void updateDirect(int direct) = 0;
	virtual void minusHP(float damage) = 0;
	virtual void controlFrame() = 0;

	virtual stateImage getVstateImage() = 0;
	virtual stateImageIter getVIstateImage() = 0;

	virtual RECT getRect() = 0;
	virtual RECT getProve() = 0;

	virtual void setX(float x) = 0;
	virtual float getX() = 0;
	virtual void setY(float y) = 0;
	virtual float getY() = 0;

	virtual void setDirect(int direct) = 0;
	virtual int getDirect() = 0;

	virtual void attack(Player* player) = 0;
	virtual void attackEnd() = 0;

	virtual void collision() = 0;

	virtual void death() = 0;
	virtual bool isDeath() = 0;

	virtual float getHP() = 0;

	virtual int getType() = 0;
};