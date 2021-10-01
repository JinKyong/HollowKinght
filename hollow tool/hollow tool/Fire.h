#pragma once
#include "State.h"

class Fire : public State
{
public:
	Fire() {};
	~Fire() {};

	virtual HRESULT init(Player* player, bool reverse = false);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);
	virtual void updateDirect(int direct);
	virtual void updateRect();

	virtual int getStateNum() { return FIRE; }

	void controlKey();
	void controlFrame();
};

