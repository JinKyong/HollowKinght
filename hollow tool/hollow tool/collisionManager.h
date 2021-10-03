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
	//�÷��̾�
	Player* _player;
	int _state;

	//����
	vector<RECT> _ground;
	vector<RECT> _fground;
	vector<RECT> _portal;

	collisionManager() {};
	friend singletonBase<collisionManager>;

public:
	HRESULT init();
	HRESULT init(Scene* scene);
	void release();

	//�÷��̾� �浹
	void collisionPlayer();
	void playerWithGround();
	void playerWithFGround();
	void playerWithPortal();
	void onAir();

	//���ʹ� �浹
	bool collisionEnemy(Enemy* enemy);
	void enemyWithGround(Enemy* enemy);
	void enemyWithFGround(Enemy* enemy);
	void enemyWithPlayer(Enemy* enemy);
	bool enemyWithEffect(Enemy* enemy);
	void enemyOnAir(Enemy* enemy);

	//���ʹ� �Ҹ�
	void collisionBullet(Bullet* bullet);
	void bulletWithGround(Bullet* bullet);
	void bulletWithFGround(Bullet* bullet);
	void bulletWithPlayer(Bullet* bullet);

	//������ �浹
	bool collisionItem(Item* item);
	void ItemWithGround(Item* item);
	void ItemWithFGround(Item* item);
	bool ItemWithPlayer(Item* item);
};
