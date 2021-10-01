#pragma once
#include "gameNode.h"

#define BULLETSIZE	60

enum BULLETSTATE {
	BULLETREADY,
	BULLETFIRE,
	BULLETEND,
	BULLETENDSIDE,
	BULLETFIN
};

class Bullet : public gameNode
{
private:
	image* _fire;
	image* _end;
	image* _endSide;
	image* _img;

	float _x, _y;
	RECT _body;

	float _angle;
	float _speed;

	int _direct;
	int _state;

	float _count;
	bool _isFire;

public:
	Bullet() { _state = BULLETREADY; };
	~Bullet() {};

	HRESULT init();
	HRESULT init(float x, float y, float angle);
	virtual void release();
	virtual void update();
	virtual void render();

	void updateRect();
	void changeState(int state);
	void bulletMove();
	void controlFrame();

	float getX() { return _x; }
	float getY() { return _y; }
	RECT getRect() { return _body; }
	
	void setDirect(int direct) { _direct = direct; }
	int getState() { return _state; }
};

