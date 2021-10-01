#include "stdafx.h"
#include "itemManager.h"
#include "Geo.h"
#include "Hook.h"
#include "Wing.h"
#include "Cloak.h"

HRESULT itemManager::init()
{
	return S_OK;
}

void itemManager::release()
{
	for (_viItem = _vItem.begin(); _viItem != _vItem.end();) {
		if ((*_viItem) != NULL) {
			(*_viItem)->release();
			SAFE_DELETE((*_viItem));
			_viItem = _vItem.erase(_viItem);
		}
		else ++_viItem;
	}

	_vItem.clear();
}

void itemManager::update()
{
	for (_viItem = _vItem.begin(); _viItem != _vItem.end();) {
		(*_viItem)->update();
		(*_viItem)->updateRect();

		if (COLLISIONMANAGER->collisionItem((*_viItem))) {
			(*_viItem)->changeState(ENDITEMSTATE);
			if ((*_viItem)->getType() > GEO) {
				//아이템(GEO제외) 인벤에 추가
				INVENTORYMANAGER->addItem((*_viItem));
				_viItem = _vItem.erase(_viItem);
			}
			else {
				INVENTORYMANAGER->addGeo(15);
				//GEO는 충돌하면 삭제
				if ((*_viItem)->getState() == ENDITEMSTATE) {
					(*_viItem)->release();
					SAFE_DELETE((*_viItem));
					_viItem = _vItem.erase(_viItem);
				}
				else
					++_viItem;
			}
		}
		else {
			(*_viItem)->updateRect();
			++_viItem;
		}
	}
}

void itemManager::render(HDC hdc)
{
	for (_viItem = _vItem.begin(); _viItem != _vItem.end(); ++_viItem)
		(*_viItem)->render(hdc);
}

void itemManager::dropGeo(float x, float y, int num)
{
	for (int i = 0; i < num; i++) {
		Item* it = new Geo;
		it->init(x, y, RND->getFromFloatTo(TORADIAN(10), PI - TORADIAN(10)));
		_vItem.push_back(it);
	}
}

void itemManager::dropSeal(float x, float y, int num)
{
}

void itemManager::dropCloak(float x, float y)
{
	Item* it = new Cloak;
	it->init(x, y, RND->getFromFloatTo(TORADIAN(10), PI - TORADIAN(10)));
	_vItem.push_back(it);
}

void itemManager::dropWing(float x, float y)
{
	Item* it = new Wing;
	it->init(x, y, RND->getFromFloatTo(TORADIAN(10), PI - TORADIAN(10)));
	_vItem.push_back(it);
}

void itemManager::dropHook(float x, float y)
{
	Item* it = new Hook;
	it->init(x, y, RND->getFromFloatTo(TORADIAN(10), PI - TORADIAN(10)));
	_vItem.push_back(it);
}

void itemManager::removeItem(int arrNum)
{
	_vItem.erase(_vItem.begin() + arrNum);
}