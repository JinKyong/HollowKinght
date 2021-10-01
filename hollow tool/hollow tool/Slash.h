#pragma once
#include "State.h"

class Slash : public State
{
private:
	bool _air;
	float _gravity;
	int _slashCount;

public:
	Slash() {};
	~Slash() {};

	virtual HRESULT init(Player* player, bool reverse = false);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);
	virtual void updateDirect(int direct);
	virtual void updateRect();

	virtual int getStateNum() { return SLASH; }

	void controlKey();
	void controlFrame();
};

