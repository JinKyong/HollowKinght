#pragma once
#include "State.h"

class Dash : public State
{
private:
	float _dashPower;

public:
	Dash() {};
	~Dash() {};

	virtual HRESULT init(Player* player, bool reverse = false);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);
	virtual void updateDirect(int direct);
	virtual void updateRect();

	virtual int getStateNum() { return DASH; }

	void controlKey();
	void controlFrame();
};

