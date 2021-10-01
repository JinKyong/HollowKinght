#pragma once
#include "State.h"

class Fall : public State
{
private:
	float _gravity;

public:
	Fall() {};
	~Fall() {};

	virtual HRESULT init(Player* player, bool reverse = false);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);
	virtual void updateDirect(int direct);
	virtual void updateRect();

	virtual int getStateNum() { return FALL; }

	void controlKey();
	void controlFrame();
};

