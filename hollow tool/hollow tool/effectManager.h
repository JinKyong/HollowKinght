#pragma once
#include "Effect.h"
#include <map>

class effectManager : public gameNode
{
private:
	typedef map<int, Effect*>				effectList;
	typedef map<int, Effect*>::iterator		effectIter;

	typedef vector<Effect*>				playEffectList;
	typedef vector<Effect*>::iterator	playEffectListIter;

private:
	effectList			_vEffectList;
	effectIter			_viEffectList;

	playEffectList		_vEffect;
	playEffectListIter	_viEffect;

public:
	effectManager() {};
	~effectManager() {};

	virtual HRESULT init();			//초기화 함수
	virtual void release();			//메모리 해제 함수
	virtual void update();			//연산 함수
	virtual void render();			//그리기 함수

	Effect* registerEffect(int effectNum, Effect* effect);

	HRESULT addEffect(POINT position, float angle, float speed, float damage, int effect);
	void removeEffect(int type);

	playEffectList getvEffect() { return _vEffect; }
	playEffectListIter getviEffect() { return _viEffect; }
};

