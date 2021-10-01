#include "stdafx.h"
#include "collisionManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Scene.h"
#include "State.h"
#include "Bullet.h"
#include "Item.h"

HRESULT collisionManager::init()
{
	return S_OK;
}

HRESULT collisionManager::init(Scene* scene)
{
	_player = scene->getPlayer();
	_state = 0;

	_ground = scene->getGround();
	_fground = scene->getFGround();
	_portal = scene->getPortal();

	return S_OK;
}

void collisionManager::release()
{
}

void collisionManager::collisionPlayer()
{
	//플레이어 상태
	_state = STATEMANAGER->getCurrentState()->getStateNum();

	playerWithGround();
	playerWithFGround();
	playerWithPortal();
	onAir();
}

void collisionManager::playerWithGround()
{
	RECT tmp;
	RECT head = _player->getHead();
	RECT body = _player->getBody();

	for (int i = 0; i < _ground.size(); ++i) {
		//HEAD
		if (IntersectRect(&tmp, &head, &_ground[i])) {
			float width = tmp.right - tmp.left;
			float height = tmp.bottom - tmp.top;
			float x = (_ground[i].right + _ground[i].left) / 2;
			float y = (_ground[i].bottom + _ground[i].top) / 2;

			//좌우 충돌
			if (height > width) {
				//좌
				if (_player->getX() < x)
					_player->setX(_player->getX() - width);
				//우
				else
					_player->setX(_player->getX() + width);
			}
			//상하 충돌
			else if (width > height) {

				//아래
				if (_player->getY() > y) {
					_player->setY(_player->getY() + height);

					//점프 또는 더블점프때는 상태변화 x
					if (_state == JUMP || _state == DOUBLEJUMP) continue;
					STATEMANAGER->changeState(FALL);
				}
				//위
				else {
					if (_state == FALL || _state == WALL ||
						_state == SLASH || _state == DOWNSLASH || _state == UPSLASH) {
						_player->setY(_player->getY() - height);
						STATEMANAGER->changeState(IDLE);
					}
				}
			}
		}
		//BODY
		else if (IntersectRect(&tmp, &body, &_ground[i])) {
			float width = tmp.right - tmp.left;
			float height = tmp.bottom - tmp.top;
			float x = (_ground[i].right + _ground[i].left) / 2;
			float y = (_ground[i].bottom + _ground[i].top) / 2;

			//좌우 충돌
			if (height > width) {
				//좌
				if (_player->getX() < x)
					_player->setX(_player->getX() - width);
				//우
				else
					_player->setX(_player->getX() + width);
			}
			//상하 충돌
			else if (width > height) {

				//아래
				if (_player->getY() > y) {
					_player->setY(_player->getY() + height);

					//점프 또는 더블점프때는 상태변화 x
					if (_state == JUMP || _state == DOUBLEJUMP) continue;
					STATEMANAGER->changeState(FALL);
				}
				//위
				else {
					if (_state == FALL || _state == WALL ||
						_state == SLASH || _state == DOWNSLASH || _state == UPSLASH) {
						_player->setY(_player->getY() - height);
						STATEMANAGER->changeState(IDLE);
					}
				}
			}
		}
	}
}

void collisionManager::playerWithFGround()
{
	RECT tmp;
	RECT head = _player->getHead();
	RECT body = _player->getBody();

	for (int i = 0; i < _fground.size(); ++i) {
		//HEAD
		if (IntersectRect(&tmp, &head, &_fground[i])) {
			float width = tmp.right - tmp.left;
			float height = tmp.bottom - tmp.top;
			float x = (_fground[i].right + _fground[i].left) / 2;
			float y = (_fground[i].bottom + _fground[i].top) / 2;

			//좌우 충돌
			if (height > width) {
				//좌
				if (_player->getX() < x) {
					_player->setX(_player->getX() - width);
					if(STATEMANAGER->getCurrentState()->getStateNum() == FALL)
						STATEMANAGER->changeState(WALL);
				}
				//우
				else {
					_player->setX(_player->getX() + width);
					if (STATEMANAGER->getCurrentState()->getStateNum() == FALL)
						STATEMANAGER->changeState(WALL);
				}
			}
			//상하 충돌
			else if (width > height) {

				//아래
				if (_player->getY() > y) {
					_player->setY(_player->getY() - height);

					//점프 또는 더블점프때는 상태변화 x
					if (_state == JUMP || _state == DOUBLEJUMP) continue;
					STATEMANAGER->changeState(FALL);
				}
				//위
				else {
					if (_state == FALL || _state == WALL ||
						_state == SLASH || _state == DOWNSLASH || _state == UPSLASH) {
						_player->setY(_player->getY() - height);
						STATEMANAGER->changeState(IDLE);
					}
				}
			}
		}
		//BODY
		else if (IntersectRect(&tmp, &body, &_fground[i])) {
			float width = tmp.right - tmp.left;
			float height = tmp.bottom - tmp.top;
			float x = (_fground[i].right + _fground[i].left) / 2;
			float y = (_fground[i].bottom + _fground[i].top) / 2;

			//좌우 충돌
			if (height > width) {
				//좌
				if (_player->getX() < x) {
					_player->setX(_player->getX() - width);
					if (STATEMANAGER->getCurrentState()->getStateNum() == FALL)
						STATEMANAGER->changeState(WALL);
				}
				//우
				else {
					_player->setX(_player->getX() + width);
					if (STATEMANAGER->getCurrentState()->getStateNum() == FALL)
						STATEMANAGER->changeState(WALL);
				}
			}
			//상하 충돌
			else if (width > height) {
				//아래
				if (_player->getY() > y) {
					_player->setY(_player->getY() - height);

					//점프 또는 더블점프때는 상태변화 x
					if (_state == JUMP || _state == DOUBLEJUMP) continue;
					STATEMANAGER->changeState(FALL);
				}
				//위
				else {
					if (_state == FALL || _state == WALL ||
						_state == SLASH || _state == DOWNSLASH || _state == UPSLASH) {
						_player->setY(_player->getY() - height);
						STATEMANAGER->changeState(IDLE);
					}
				}
			}
		}
	}
}

void collisionManager::playerWithPortal()
{
	RECT tmp;
	RECT head = _player->getHead();
	RECT body = _player->getBody();

	for (int i = 0; i < _portal.size(); ++i) {
		if (IntersectRect(&tmp, &head, &_portal[i]) ||
			IntersectRect(&tmp, &body, &_portal[i])) {
			_player->setPortal(true);
		}
		else
			_player->setPortal(false);
	}
}

void collisionManager::onAir()
{
	//RECT head = _player->getHead();
	RECT body = _player->getBody();

	bool air = true;
	for (int i = 0; i < _ground.size(); ++i) {
		if ((_ground[i].left <= body.right && body.left <= _ground[i].right) &&
			(_ground[i].top <= body.bottom && body.top <= _ground[i].bottom)) {
			air = false;
			break;
		}
	}
	for (int i = 0; i < _fground.size(); ++i) {
		if ((_fground[i].left <= body.right && body.left <= _fground[i].right) &&
			(_fground[i].top <= body.bottom && body.top <= _fground[i].bottom)) {
			air = false;
			break;
		}
	}

	if (air) {
		if (_state == IDLE || _state == RUN || _state == WALL) {
			_player->jump();
			STATEMANAGER->changeState(FALL);
		}
	}
}

bool collisionManager::collisionEnemy(Enemy* enemy)
{
	//지형
	enemyWithGround(enemy);
	enemyWithFGround(enemy);

	//이펙트
	if (enemyWithEffect(enemy)) {
		return true;
	}

	//플레이어
	if (!(_player->isStun() || _state == DEATH))
		enemyWithPlayer(enemy);

	//enemyOnAir(enemy);

	return false;
}

void collisionManager::enemyWithGround(Enemy* enemy)
{
	RECT tmp;
	RECT body = enemy->getRect();

	for (int i = 0; i < _ground.size(); ++i) {
		if (IntersectRect(&tmp, &body, &_ground[i])) {
 			float width = tmp.right - tmp.left;
			float height = tmp.bottom - tmp.top;
			float x = (_ground[i].right + _ground[i].left) / 2;
			float y = (_ground[i].bottom + _ground[i].top) / 2;

			if (height > width) {
				//좌
				if (enemy->getX() < x) {
					enemy->setX(enemy->getX() - width);
					if (enemy->getType() == MINION)
						enemy->collision();
				}
				//우
				else {
					enemy->setX(enemy->getX() + width);
					if(enemy->getType() == MINION)
						enemy->collision();
				}
			}
			//상하 충돌
			else if (width > height) {

				//아래
				if (enemy->getY() > y) {
					enemy->setY(enemy->getY() + height);
						enemy->collision();
				}
				//위
				else {
					enemy->setY(enemy->getY() - height);
						enemy->collision();
				}
			}
			break;
		}
	}
}

void collisionManager::enemyWithFGround(Enemy* enemy)
{
	RECT tmp;
	RECT body = enemy->getRect();

	for (int i = 0; i < _fground.size(); ++i) {
		if (IntersectRect(&tmp, &body, &_fground[i])) {
			float width = tmp.right - tmp.left;
			float height = tmp.bottom - tmp.top;
			float x = (_fground[i].right + _fground[i].left) / 2;
			float y = (_fground[i].bottom + _fground[i].top) / 2;

			if (height > width) {
				//좌
				if (enemy->getX() < x) {
					enemy->setX(enemy->getX() - width);

					if (enemy->getType() == MINION)
						enemy->collision();
				}
				//우
				else {
					enemy->setX(enemy->getX() + width);

					if (enemy->getType() == MINION)
						enemy->collision();
				}
			}
			//상하 충돌
			else if (width > height) {

				//아래
				if (enemy->getY() > y) {
					enemy->setY(enemy->getY() + height);
						enemy->collision();
				}
				//위
				else {
					enemy->setY(enemy->getY() - height);
						enemy->collision();
				}
			}
			break;
		}
	}
}

void collisionManager::enemyWithPlayer(Enemy * enemy)
{
	if (enemy->getHP() <= 0) return;

	RECT tmp;
	RECT head = _player->getHead();
	RECT body = _player->getBody();

	if (IntersectRect(&tmp, &head, &enemy->getRect()) ||
		IntersectRect(&tmp, &body, &enemy->getRect())) {
		_player->changeHP(-1);
		if (_player->getHP() <= 0)
			STATEMANAGER->changeState(DEATH);
		else
			STATEMANAGER->changeState(STUN);
	}
	if (IntersectRect(&tmp, &head, &enemy->getProve()) ||
		IntersectRect(&tmp, &body, &enemy->getProve())) {
		enemy->attack(_player);
	}
}

bool collisionManager::enemyWithEffect(Enemy * enemy)
{
	vector<Effect*> effect = _player->getEM()->getvEffect();
	RECT tmp;

	for (int i = 0; i < effect.size(); ++i) {
		if (effect[i]->getDamage() <= 0 ||
			effect[i]->getACount() <= 0 ||
			enemy->getHP() <= 0)			continue;

		if (IntersectRect(&tmp, &effect[i]->getRect(), &enemy->getRect())){
			effect[i]->collision();

			if (effect[i]->getX() - enemy->getX() < 0) {
				if(enemy->getType() == MINION)
					enemy->setX(enemy->getX() + 20);
			}
			else {
				if (enemy->getType() == MINION)
					enemy->setX(enemy->getX() - 20);
			}
			enemy->minusHP(effect[i]->getDamage());

			if(effect[i]->getType() != EF_FIRE)
				_player->changeSoul(5);
		}
	}

	return false;
}

void collisionManager::enemyOnAir(Enemy* enemy)
{
	RECT body = enemy->getRect();

	bool air = true;
	for (int i = 0; i < _ground.size(); ++i) {
		if ((_ground[i].left < body.left && body.right < _ground[i].right) &&
			(_ground[i].top <= body.bottom && body.top <= _ground[i].bottom)) {
			air = false;
			break;
		}
	}
	for (int i = 0; i < _fground.size(); ++i) {
		if ((_fground[i].left < body.left && body.right < _fground[i].right) &&
			(_fground[i].top <= body.bottom && body.top <= _fground[i].bottom)) {
			air = false;
			break;
		}
	}

	if (air) {
		if(enemy->getType() == MINION)
			enemy->collision();
	}
}

void collisionManager::collisionBullet(Bullet * bullet)
{
	//지형
	bulletWithGround(bullet);
	bulletWithFGround(bullet);

	//플레이어
	if (!(_player->isStun() || _state == DEATH))
		bulletWithPlayer(bullet);

	bullet->updateRect();
}

void collisionManager::bulletWithGround(Bullet * bullet)
{
	RECT tmp;
	RECT body = bullet->getRect();

	for (int i = 0; i < _ground.size(); ++i) {
		if (IntersectRect(&tmp, &body, &_ground[i])) {
			float width = tmp.right - tmp.left;
			float height = tmp.bottom - tmp.top;
			float x = (_ground[i].right + _ground[i].left) / 2;
			float y = (_ground[i].bottom + _ground[i].top) / 2;

			if (height > width) {
				//좌
				if (bullet->getX() < x) {
					bullet->setDirect(LEFT);
					bullet->changeState(BULLETENDSIDE);
				}
				//우
				else {
					bullet->setDirect(RIGHT);
					bullet->changeState(BULLETENDSIDE);
				}
			}
			//상하 충돌
			else if (width > height) {
				bullet->setDirect(0);
				bullet->changeState(BULLETEND);
			}
			break;
		}
	}
}

void collisionManager::bulletWithFGround(Bullet * bullet)
{
	RECT tmp;
	RECT body = bullet->getRect();

	for (int i = 0; i < _fground.size(); ++i) {
		if (IntersectRect(&tmp, &body, &_fground[i])) {
			float width = tmp.right - tmp.left;
			float height = tmp.bottom - tmp.top;
			float x = (_fground[i].right + _fground[i].left) / 2;
			float y = (_fground[i].bottom + _fground[i].top) / 2;

			if (height > width) {
				//좌
				if (bullet->getX() < x) {
					bullet->setDirect(LEFT);
					bullet->changeState(BULLETENDSIDE);
				}
				//우
				else {
					bullet->setDirect(RIGHT);
					bullet->changeState(BULLETENDSIDE);
				}
			}
			//상하 충돌
			else if (width > height) {
				bullet->setDirect(0);
				bullet->changeState(BULLETEND);
			}
			break;
		}
	}
}

void collisionManager::bulletWithPlayer(Bullet * bullet)
{
	RECT tmp;
	RECT head = _player->getHead();
	RECT body = _player->getBody();

	if (IntersectRect(&tmp, &head, &bullet->getRect())) {
		float width = tmp.right - tmp.left;
		float height = tmp.bottom - tmp.top;
		float x = (head.right + head.left) / 2;
		float y = (head.bottom + head.top) / 2;

		if (height > width) {
			//좌
			if (bullet->getX() < x) {
				bullet->setDirect(LEFT);
				bullet->changeState(BULLETENDSIDE);
			}
			//우
			else {
				bullet->setDirect(RIGHT);
				bullet->changeState(BULLETENDSIDE);
			}
		}
		//상하 충돌
		else if (width > height) {
			bullet->setDirect(0);
			bullet->changeState(BULLETEND);
		}

		float angle = getAngle(_player->getX(), _player->getY(), bullet->getX(), bullet->getY());
		if (PI / 2 < angle && angle <= 3 * PI / 2)
			_player->setDirect(1);
		else
			_player->setDirect(0);

		_player->changeHP(-1);
		if (_player->getHP() <= 0)
			STATEMANAGER->changeState(DEATH);
		else
			STATEMANAGER->changeState(STUN);

	}
	//BODY
	else if (IntersectRect(&tmp, &body, &bullet->getRect())) {
		float width = tmp.right - tmp.left;
		float height = tmp.bottom - tmp.top;
		float x = (body.right + body.left) / 2;
		float y = (body.bottom + body.top) / 2;

		if (height > width) {
			//좌
			if (bullet->getX() < x) {
				bullet->setDirect(LEFT);
				bullet->changeState(BULLETENDSIDE);
			}
			//우
			else {
				bullet->setDirect(RIGHT);
				bullet->changeState(BULLETENDSIDE);
			}
		}
		//상하 충돌
		else if (width > height) {
			bullet->setDirect(0);
			bullet->changeState(BULLETEND);
		}

		float angle = getAngle(_player->getX(), _player->getY(), bullet->getX(), bullet->getY());
		if (PI / 2 < angle && angle <= 3 * PI / 2)
			_player->setDirect(1);
		else
			_player->setDirect(0);

		_player->changeHP(-1);
		if (_player->getHP() <= 0)
			STATEMANAGER->changeState(DEATH);
		else
			STATEMANAGER->changeState(STUN);
	}
}

bool collisionManager::collisionItem(Item * item)
{
	//지형
	ItemWithGround(item);
	ItemWithFGround(item);

	//플레이어
	if (ItemWithPlayer(item))
		return true;

	item->updateRect();
	return false;
}

void collisionManager::ItemWithGround(Item * item)
{
	RECT tmp;
	RECT iRect = item->getBody();

	for (int i = 0; i < _ground.size(); ++i) {
		if (IntersectRect(&tmp, &iRect, &_ground[i])) {
			float width = tmp.right - tmp.left;
			float height = tmp.bottom - tmp.top;
			float x = (_ground[i].right + _ground[i].left) / 2;
			float y = (_ground[i].bottom + _ground[i].top) / 2;

			if (height > width) {
				//좌
				if (item->getX() < x)
					item->setX(item->getX() - width);
				//우
				else
					item->setX(item->getX() + width);
			}
			//상하 충돌
			else if (width > height) {
				//아래
				if (item->getY() > y)
					item->setY(item->getY() + height);
				//위
				else {
					item->setY(item->getY() - height);
					item->changeState(item->getState() + 1);
				}
			}
			break;
		}
	}
}

void collisionManager::ItemWithFGround(Item * item)
{
	RECT tmp;
	RECT iRect = item->getBody();

	for (int i = 0; i < _fground.size(); ++i) {
		if (IntersectRect(&tmp, &iRect, &_fground[i])) {
			float width = tmp.right - tmp.left;
			float height = tmp.bottom - tmp.top;
			float x = (_fground[i].right + _fground[i].left) / 2;
			float y = (_fground[i].bottom + _fground[i].top) / 2;

			if (height > width) {
				//좌
				if (item->getX() < x)
					item->setX(item->getX() - width);
				//우
				else
					item->setX(item->getX() + width);
			}
			//상하 충돌
			else if (width > height) {
				//아래
				if (item->getY() > y)
					item->setY(item->getY() + height);
				//위
				else {
					item->setY(item->getY() - height);
					item->changeState(item->getState() + 1);
				}
			}
			break;
		}
	}
}

bool collisionManager::ItemWithPlayer(Item * item)
{
	RECT tmp;
	RECT head = _player->getHead();
	RECT body = _player->getBody();
	RECT iRect = item->getBody();

	if (IntersectRect(&tmp, &iRect, &head) ||
		IntersectRect(&tmp, &iRect, &body))
		return true;

	return false;
}