#pragma once
#include "gameNode.h"
#include "enemyManager.h"
#include <vector>

class Player;

enum CAMERAFADE {
	FADEOUT= -1,
	NORMAL,
	FADEIN,
	ENDFADE
};

class Scene : public gameNode
{
protected:
	vector<RECT> _grounds;			//º®Å¸±â ¾ÈµÊ
	vector<RECT> _floatingGrounds;	//º®Å¸±â µÊ
	vector<RECT> _portals;			//Æ÷Å»

	enemyManager* _em;
	bool _resetEnemy;

	static Player* _player;

public:
	Scene() {};
	~Scene() {};

	virtual HRESULT init(Player* player);
	virtual void release() = 0;
	virtual void update() = 0;
	virtual void render() = 0;
	virtual void renderFront() = 0;

	virtual void changeScene() = 0;

	virtual vector<RECT> getGround() = 0;
	virtual vector<RECT> getFGround() = 0;
	virtual vector<RECT> getPortal() = 0;

	virtual enemyManager* getEM() = 0;

	virtual void setEnemy(bool enemy) = 0;
	virtual bool resetEnemy() = 0;

	static Player* getPlayer() { return _player; }
};