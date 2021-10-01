#pragma once
#include "State.h"

#define DURATION 1.0f

class Stun : public State
{
private:
	float _alpha;

public:
	Stun() {};
	~Stun() {};

	virtual HRESULT init(Player* player, bool reverse = false);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);
	virtual void updateDirect(int direct);
	virtual void updateRect();

	virtual int getStateNum() { return STUN; }

	void controlKey();
};

