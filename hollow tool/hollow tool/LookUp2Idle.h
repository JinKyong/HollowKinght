#pragma once
#include "State.h"

class LookUp2Idle :	public State
{
public:
	LookUp2Idle() {};
	~LookUp2Idle() {};

	virtual HRESULT init(Player* player, bool reverse = false);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);
	virtual void updateDirect(int direct);
	virtual void updateRect();

	virtual int getStateNum() { return LOOKUP2IDLE; }

	void controlKey();
	void controlFrame();
};

