#pragma once
#include "singletonBase.h"
#include <vector>

class Player;
class Enemy;
class Bullet;
class Item;

class collisionManager : public singletonBase<collisionManager>
{
private:
	//플레이어
	Player* _player;
	int _state;

	//지형
	vector<RECT> _ground;
	vector<RECT> _fground;
	vector<RECT> _portal;

	collisionManager() {};
	friend singletonBase<collisionManager>;

public:
	HRESULT init();
	HRESULT init(Scene* scene);
	void release();

	//플레이어 충돌
	void collisionPlayer();
	void playerWithGround();
	void playerWithFGround();
	void playerWithPortal();
	void onAir();

	//에너미 충돌
	bool collisionEnemy(Enemy* enemy);
	void enemyWithGround(Enemy* enemy);
	void enemyWithFGround(Enemy* enemy);
	void enemyWithPlayer(Enemy* enemy);
	bool enemyWithEffect(Enemy* enemy);
	void enemyOnAir(Enemy* enemy);

	//에너미 불릿
	void collisionBullet(Bullet* bullet);
	void bulletWithGround(Bullet* bullet);
	void bulletWithFGround(Bullet* bullet);
	void bulletWithPlayer(Bullet* bullet);

	//아이템 충돌
	bool collisionItem(Item* item);
	void ItemWithGround(Item* item);
	void ItemWithFGround(Item* item);
	bool ItemWithPlayer(Item* item);
};
