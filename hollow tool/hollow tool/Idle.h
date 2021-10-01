#pragma once
#include "State.h"

class Idle : public State
{
public:
	Idle() {};
	~Idle() {};

	virtual HRESULT init(Player* player, bool reverse = false);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);
	virtual void updateDirect(int direct);
	virtual void updateRect();

	virtual int getStateNum() { return IDLE; }

	void controlKey();
	void controlFrame();
};

