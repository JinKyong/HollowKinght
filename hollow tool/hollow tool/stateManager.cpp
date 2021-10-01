#include "stdafx.h"
#include "stateManager.h"
#include "Player.h"
#include "State.h"
#include "Idle.h"
#include "Run.h"
#include "Jump.h"
#include "DoubleJump.h"
#include "Fall.h"
#include "Dash.h"
#include "Dash2Idle.h"
#include "Slash.h"
#include "UpSlash.h"
#include "DownSlash.h"
#include "Fire.h"
#include "Focus.h"
#include "Focus2Idle.h"
#include "LookUp.h"
#include "LookUp2Idle.h"
#include "LookDown.h"
#include "LookDown2Idle.h"
#include "Wall.h"
#include "WallJump.h"
#include "Stun.h"
#include "Death.h"
#include "Inventory.h"

Player* stateManager::_player = NULL;
State* stateManager::_currentState = NULL;
State* stateManager::_prevState = NULL;

HRESULT stateManager::init(Player * player)
{
	_player = player;
	_prevState = _currentState = NULL;

	/**** 상태 ****/
	addState(IDLE, new Idle);
	addState(RUN, new Run);
	addState(JUMP, new Jump);
	addState(DOUBLEJUMP, new DoubleJump);
	addState(FALL, new Fall);
	addState(DASH, new Dash);
	addState(DASH2IDLE, new Dash2Idle);
	addState(SLASH, new Slash);
	addState(UPSLASH, new UpSlash);
	addState(DOWNSLASH, new DownSlash);
	addState(FIRE, new Fire);
	addState(FOCUS, new Focus);
	addState(FOCUS2IDLE, new Focus2Idle);
	addState(LOOKUP, new LookUp);
	addState(LOOKUP2IDLE, new LookUp2Idle);
	addState(LOOKDOWN, new LookDown);
	addState(LOOKDOWN2IDLE, new LookDown2Idle);
	addState(WALL, new Wall);
	addState(WALLJUMP, new WallJump);
	addState(STUN, new Stun);
	addState(DEATH, new Death);
	addState(INVENTORY, new Inventory);

	_player->jump();
	changeState(FALL);

	return S_OK;
}

void stateManager::release()
{
	stateIter istateList = _stateList.begin();

	for (; istateList != _stateList.end();)
	{
		if (istateList->second != NULL)
		{
			if (istateList->second == _currentState) istateList->second->release();
			SAFE_DELETE(istateList->second);
			istateList = _stateList.erase(istateList);
		}
		else ++istateList;
	}

	_stateList.clear();
}

void stateManager::update()
{
	if (_currentState) _currentState->update();
}

void stateManager::render(HDC hdc)
{
	if (_currentState) _currentState->render(hdc);
}

State * stateManager::addState(int stateNum, State * state)
{
	if (!state) return nullptr;

	_stateList.insert(make_pair(stateNum, state));

	return state;
}

HRESULT stateManager::changeState(int state, BOOL reverse)
{
	stateIter find = _stateList.find(state);

	if (find == _stateList.end()) return E_FAIL;

	if (find->second == _currentState) return S_OK;

	if (SUCCEEDED(find->second->init(_player, reverse))) {
		//if (_currentState) _currentState->release();

		_prevState = _currentState;
		_currentState = find->second;

		//대쉬 상태가 아닐 때 대쉬 이펙트 삭제
		if (_currentState->getStateNum() != DASH)
			_player->getEM()->removeEffect(EF_DASH);

		//더블 점프 상태가 아닐 때 더블 점프 이펙트 삭제
		if (_currentState->getStateNum() != DOUBLEJUMP)
			_player->getEM()->removeEffect(EF_DOUBLEJUMP);

		//포커스 상태가 아닐 때 포커스 이펙트 삭제
		if(_currentState->getStateNum() != FOCUS)
			_player->getEM()->removeEffect(EF_FOCUS);

		TIMEMANAGER->endTimer();

		return S_OK;
	}

	return E_FAIL;
}

HRESULT stateManager::changeStateNonPrev(int state, BOOL reverse)
{
	stateIter find = _stateList.find(state);

	if (find == _stateList.end()) return E_FAIL;

	if (find->second == _currentState) return S_OK;

	if (SUCCEEDED(find->second->init(_player, reverse))) {
		//if (_currentState) _currentState->release();

		//_prevState = _currentState;
		_currentState = find->second;

		//포커스 상태가 아닐 때 포커스 이펙트 삭제
		if (_currentState->getStateNum() != FOCUS)
			_player->getEM()->removeEffect(EF_FOCUS);

		//대쉬 상태가 아닐 때 대쉬 이펙트 삭제
		if (_currentState->getStateNum() != DASH)
			_player->getEM()->removeEffect(EF_DASH);

		TIMEMANAGER->endTimer();

		return S_OK;
	}

	return E_FAIL;
}
