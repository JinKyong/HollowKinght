#pragma once
#include "singletonBase.h"
#include <map>

class Player;
class State;

class stateManager : public singletonBase<stateManager>
{
private:
	typedef map<int, State*>				stateList;
	typedef map<int, State*>::iterator		stateIter;

private:
	static Player* _player;

	static State* _currentState;
	static State* _prevState;

	stateList _stateList;

	stateManager() {};
	friend singletonBase<stateManager>;

public:
	HRESULT init(Player* player);
	void release();
	void update();
	void render(HDC hdc);

	State* addState(int stateNum, State* state);
	HRESULT changeState(int state, BOOL reverse = false);
	HRESULT changeStateNonPrev(int state, BOOL reverse = false);

	void setState(State* state) { _currentState = state; };
	State* getPrevState() { return _prevState; }
	State* getCurrentState() { return _currentState; }
};

