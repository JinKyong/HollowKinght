#pragma once
#include "Item.h"

class Cloak : public Item
{
private:
	float _gravity;

public:
	Cloak() {};
	~Cloak() {};

	virtual HRESULT init(float x, float y, float angle);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);
	virtual void renderIcon(HDC hdc, float x, float y);
	virtual void renderInfo(HDC hdc, float x, float y);

	virtual void updateRect();
	virtual void changeState(int state);

	virtual RECT getBody() { return _body; }
	virtual float getX() { return _x; }
	virtual void setX(float x) { _x = x; }
	virtual float getY() { return _y; }
	virtual void setY(float y) { _y = y; }

	virtual int getState() { return _state; }
	virtual int getType() { return _type; }

	void move();
};

