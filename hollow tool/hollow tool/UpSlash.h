#pragma once
#include "State.h"

class UpSlash : public State
{
private:
	bool _air;
	float _gravity;

public:
	UpSlash() {};
	~UpSlash() {};

	virtual HRESULT init(Player* player, bool reverse = false);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);
	virtual void updateDirect(int direct);
	virtual void updateRect();

	virtual int getStateNum() { return UPSLASH; }

	void controlKey();
	void controlFrame();
};

