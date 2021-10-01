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

	virtual HRESULT init();			//�ʱ�ȭ �Լ�
	virtual void release();			//�޸� ���� �Լ�
	virtual void update();			//���� �Լ�
	virtual void render();			//�׸��� �Լ�

	Effect* registerEffect(int effectNum, Effect* effect);

	HRESULT addEffect(POINT position, float angle, float speed, float damage, int effect);
	void removeEffect(int type);

	playEffectList getvEffect() { return _vEffect; }
	playEffectListIter getviEffect() { return _viEffect; }
};

