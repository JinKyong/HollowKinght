#pragma once
#include "State.h"

class DoubleJump : public State
{
private:
	float _jumpPower;

public:
	DoubleJump() {};
	~DoubleJump() {};

	virtual HRESULT init(Player* player, bool reverse = false);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);
	virtual void updateDirect(int direct);
	virtual void updateRect();

	virtual int getStateNum() { return DOUBLEJUMP; }

	void controlKey();
	void controlFrame();
};

