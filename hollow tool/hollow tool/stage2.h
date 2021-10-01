#pragma once
#include "Scene.h"

#define MAPWIDTH	3793
#define MAPHEIGHT	2200

#define GROUNDNUM	4

class stage2 : public Scene
{
private:
	RECT _ground[GROUNDNUM];
	RECT _portal;

public:
	stage2() {};
	~stage2() {};

	virtual HRESULT init(Player* player);
	virtual void release();
	virtual void update();
	virtual void render();
	virtual void renderFront();

	virtual void changeScene();

	virtual vector<RECT> getGround() { return _grounds; }
	virtual vector<RECT> getFGround() { return _floatingGrounds; }
	virtual vector<RECT> getPortal() { return _portals; }

	virtual enemyManager* getEM() { return _em; }

	virtual void setEnemy(bool enemy) { _resetEnemy = enemy; }
	virtual bool resetEnemy() { return _resetEnemy; }
};

