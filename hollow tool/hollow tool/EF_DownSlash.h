#pragma once
#include "Effect.h"

class EF_DownSlash : public Effect
{
public:
	EF_DownSlash() {};
	~EF_DownSlash() {};
	
	virtual HRESULT init(POINT position, float angle, float speed, float damage);
	virtual void release();
	virtual void update();
	virtual void render();

	virtual void updateRect();



	virtual RECT getRect() { return _body; }
	virtual float getX() { return _x; }
	virtual float getY() { return _y; }

	virtual float getDamage() { return _damage; }

	virtual void setState(int state) { _state = state; }
	virtual int getState() { return _state; }

	virtual int getType() { return _type; }

	virtual void collision() { _aCount--; }
	virtual int getACount() { return _aCount; }

	void controlFrame();


};

