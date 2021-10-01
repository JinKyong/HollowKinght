#pragma once
#include "Scene.h"

#define MAPWIDTH	3725
#define MAPHEIGHT	1700

#define GROUNDNUM	5
#define FGROUNDNUM	8

class stage3 : public Scene
{
private:
	RECT _ground[GROUNDNUM];
	RECT _floatingGround[GROUNDNUM];

public:
	stage3() {};
	~stage3() {};

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

