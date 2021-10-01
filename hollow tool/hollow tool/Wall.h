#pragma once
#include "State.h"

#define WGRAVITY	400

class Wall : public State
{
private:
	float _gravity;

public:
	Wall() {};
	~Wall() {};

	virtual HRESULT init(Player* player, bool reverse = false);
	virtual void release();
	virtual void update();
	virtual void render(HDC hdc);
	virtual void updateDirect(int direct);
	virtual void updateRect();

	virtual int getStateNum() { return WALL; };

	void controlKey();
	void controlFrame();
};

