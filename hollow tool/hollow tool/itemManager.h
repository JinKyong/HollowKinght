#pragma once
#include "singletonBase.h"
#include "Item.h"
#include "string.h"

class itemManager : public singletonBase<itemManager>
{
private:
	typedef vector<Item*>			itemList;
	typedef vector<Item*>::iterator	itemListIter;

private:
	itemList		_vItem;
	itemListIter	_viItem;

	itemManager() {};
	friend singletonBase<itemManager>;

public:
	HRESULT init();		
	void release();		
	void update();		
	void render(HDC hdc);

	void dropGeo(float x, float y, int num);
	void dropSeal(float x, float y, int num);
	void dropCloak(float x, float y);
	void dropWing(float x, float y);
	void dropHook(float x, float y);

	void removeItem(int arrNum);

	itemList getvItem() { return _vItem; }
	itemListIter getviItem() { return _viItem; }
};

