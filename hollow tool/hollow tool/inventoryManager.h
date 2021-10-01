#pragma once
#include "singletonBase.h"
#include <map>

class inventoryManager : public singletonBase<inventoryManager>
{
private:
	typedef map<int, Item*>				itemList;
	typedef map<int, Item*>::iterator	itemListIter;

private:
	itemList _mItem;

	image* _back;
	int _index;
	int _geo;

	bool _hook;
	bool _wing;
	bool _open;

public:
	inventoryManager() {};
	~inventoryManager() {};

	HRESULT init();
	void release();
	void render(HDC hdc);

	void addItem(Item* item);

	void plusIndex();
	void minusIndex();

	void setIndex(int num) { _index = num; }
	int getIndex() { return _index; }
	int getMaxIndex() { return _mItem.size(); }

	void addGeo(int num) { _geo += num; }
	int getGeo() { return _geo; }

	bool hook() { return _hook; }
	bool wing() { return _wing; }

	void open() { _open = true; }
	void close() { _open = false; }
	bool isOpen() { return _open; }
};

