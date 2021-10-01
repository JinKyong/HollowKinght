#pragma once
#include "State.h"

class Jump : public State
{
private:
	float _jumpPower;

public:
	Jump() {};
	~Jump() {};

	virtual HRESULT init(Player* player, bool reverse = false);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);
	virtual void updateDirect(int direct);
	virtual void updateRect();

	virtual int getStateNum() { return JUMP; }

	void controlKey();
	void controlFrame();
};

