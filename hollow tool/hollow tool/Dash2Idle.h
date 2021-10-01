#pragma once
#include "State.h"

class Dash2Idle : public State
{
public:
	Dash2Idle() {};
	~Dash2Idle() {};

	virtual HRESULT init(Player* player, bool reverse = false);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);
	virtual void updateDirect(int direct);
	virtual void updateRect();

	virtual int getStateNum() { return DASH2IDLE; }

	void controlKey();
	void controlFrame();

};

