#pragma once
#include "Scene.h"

#define MAPWIDTH	1958
#define MAPHEIGHT	3000

#define GROUNDNUM	3

class stage0 : public Scene
{
private:
	RECT _ground[GROUNDNUM];

public:
	stage0() {};
	~stage0() {};

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