#pragma once
#include "State.h"

class Focus2Idle : public State
{
public:
	Focus2Idle() {};
	~Focus2Idle() {};

	virtual HRESULT init(Player* player, bool reverse = false);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);
	virtual void updateDirect(int direct);
	virtual void updateRect();

	virtual int getStateNum() { return FOCUS2IDLE; }

	void controlKey();
	void controlFrame();
};

