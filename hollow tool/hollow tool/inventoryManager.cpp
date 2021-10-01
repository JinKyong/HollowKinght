#include "stdafx.h"
#include "inventoryManager.h"

HRESULT inventoryManager::init()
{
	_back = new image;
	_back->init("image/fade_B.bmp", WINSIZEX - 200, WINSIZEY - 100, 
		true, RGB(255, 0, 255), true);

	IMAGEMANAGER->addImage("geo_info", "image/item/geo_info.bmp",
		500, 400, false, RGB(0, 0, 0));

	_index = 0;
	_geo = 0;

	_hook = false;
	_wing = false;
	_open = false;

	return S_OK;
}

void inventoryManager::release()
{
	_back->release();
	SAFE_DELETE(_back);

	itemListIter miItem = _mItem.begin();

	for (; miItem != _mItem.end();) {
		if (miItem->second != NULL) {
			miItem->second->release();
			SAFE_DELETE(miItem->second);
			miItem = _mItem.erase(miItem);
		}
		else
			++miItem;
	}

	_mItem.clear();
}

void inventoryManager::render(HDC hdc)
{
	RECT rc = CAMERAMANAGER->getScreen();

	_back->alphaRender(hdc, rc.left + 100, rc.top + 50, 250);
	//아이템 목록 렌더
	itemListIter miItem = _mItem.begin();

	//geo
	IMAGEMANAGER->findImage("UI_geo")->render(hdc, rc.left + 200, rc.bottom - 200);

	char str[128];
	sprintf_s(str, "%d", INVENTORYMANAGER->getGeo());
	TextOut(hdc, rc.left + 270, rc.bottom - 185, str, strlen(str));

	IMAGEMANAGER->findImage("geo_info")->render(hdc, rc.right - 600, rc.top + 200);

	for (int i = 1; miItem != _mItem.end(); ++i) {
		//아이템 출력
		miItem->second->renderIcon(hdc, rc.left + i * 200, rc.top + 200);
		//정보 출력
		if(_index == i)
			miItem->second->renderInfo(hdc, rc.right - 600, rc.top + 200);

		++miItem;
	}
}

void inventoryManager::addItem(Item * item)
{
	if (item->getType() == HOOK)
		_hook = true;
	else if (item->getType() == WING)
		_wing = true;

	_mItem.insert(make_pair(item->getType(), item));
}

void inventoryManager::plusIndex()
{
	_index = _index + 1 < _mItem.size() ? _index + 1 : _mItem.size();
}

void inventoryManager::minusIndex()
{
	_index = _index - 1 > 0 ? _index - 1 : 0;
}