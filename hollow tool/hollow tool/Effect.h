#pragma once
#include "gameNode.h"
#include <vector>

enum EFFECTTYPE {
	EF_DASH,
	EF_DOUBLEJUMP,
	EF_SLASH,
	EF_SLASH2,
	EF_DOWNSLASH,
	EF_UPSLASH,
	EF_FOCUS,
	EF_FIRE,
	ENDEFFECT
};

class Effect : public gameNode
{
protected:
	image* _img;

	//��ü
	RECT _body;
	float _x, _y;

	float _angle;
	float _speed;
	float _damage;

	int _direct;
	int _state;		//���� -> 0�Ǹ� �����
	int _type;		//Ÿ��
	int _aCount;	//Ÿ��Ƚ��

	float _count;
	
public:
	Effect() {};
	~Effect() {};

	virtual HRESULT init(POINT position, float angle, float speed, float damage) = 0;
	virtual void release() = 0;
	virtual void update() = 0;
	virtual void render() = 0;

	virtual void updateRect() = 0;



	virtual RECT getRect() = 0;
	virtual float getX() = 0;
	virtual float getY() = 0;

	virtual float getDamage() = 0;

	virtual void setState(int state) = 0;
	virtual int getState() = 0;

	virtual int getType() = 0;

	virtual void collision() = 0;
	virtual int getACount() = 0;
};

