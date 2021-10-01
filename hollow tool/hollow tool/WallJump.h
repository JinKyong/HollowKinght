#pragma once
#include "State.h"

class WallJump : public State
{
private:
	float _jumpPower;
	float _angle;

public:
	WallJump() {};
	~WallJump() {};

	virtual HRESULT init(Player* player, bool reverse = false);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);
	virtual void updateDirect(int direct);
	virtual void updateRect();

	virtual int getStateNum() { return WALLJUMP; }

	void move();
	void controlKey();
	void controlFrame();
};

