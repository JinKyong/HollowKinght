#pragma once
#include "State.h"

class Focus : public State
{
public:
	Focus() {};
	~Focus() {};

	virtual HRESULT init(Player* player, bool reverse = false);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);
	virtual void updateDirect(int direct);
	virtual void updateRect();

	virtual int getStateNum() { return FOCUS; }

	void controlKey();
	void controlFrame();
};

