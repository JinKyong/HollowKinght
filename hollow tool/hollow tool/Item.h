#pragma once

#define RENDERCOUNT	0.05

#define ITEMSPEED	800
#define ITEMGRAVITY	20
#define ITEMGRAVMAX	2400
#define ITEMFRICT	0.1

enum ITEMTYPE {
	GEO,
	SEAL,
	WING,
	HOOK,
	CLOAK,
	ENDITEM
};

enum ITEMSTATE {
	ITEMMOVE,
	ITEMSTOP,
	ENDITEMSTATE
};

class Item
{
protected:
	image* _img;
	image* _icon;
	image* _info;

	RECT _body;
	float _x, _y;

	float _angle;
	float _speed;

	int _state;
	int _price;
	int _type;

	float _count;

public:
	Item() {};
	~Item() {};

	virtual HRESULT init(float x, float y, float angle) = 0;
	virtual void release() = 0;
	virtual void update() = 0;
	virtual void render(HDC hdc) = 0;
	virtual void renderIcon(HDC hdc, float x, float y) = 0;
	virtual void renderInfo(HDC hdc, float x, float y) = 0;

	virtual void updateRect() = 0;
	virtual void changeState(int state) = 0;

	virtual RECT getBody() = 0;
	virtual float getX() = 0;
	virtual void setX(float x) = 0;
	virtual float getY() = 0;
	virtual void setY(float y) = 0;

	virtual int getState() = 0;
	virtual int getType() = 0;
};

