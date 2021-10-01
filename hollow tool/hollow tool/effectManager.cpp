#include "stdafx.h"
#include "effectManager.h"
#include "EF_Dash.h"
#include "EF_DoubleJump.h"
#include "EF_Slash.h"
#include "EF_Slash2.h"
#include "EF_DownSlash.h"
#include "EF_UpSlash.h"
#include "EF_Focus.h"
#include "EF_Fire.h"

HRESULT effectManager::init()
{
	registerEffect(EF_DASH, new EF_Dash);
	registerEffect(EF_DOUBLEJUMP, new EF_DoubleJump);
	registerEffect(EF_SLASH, new EF_Slash);
	registerEffect(EF_SLASH2, new EF_Slash2);
	registerEffect(EF_DOWNSLASH, new EF_DownSlash);
	registerEffect(EF_UPSLASH, new EF_UpSlash);
	registerEffect(EF_FOCUS, new EF_Focus);
	registerEffect(EF_FIRE, new EF_Fire);

	return S_OK;
}

void effectManager::release()
{
	for (_viEffect = _vEffect.begin(); _viEffect != _vEffect.end();) {
		if ((*_viEffect) != NULL) {
			(*_viEffect)->release();
			SAFE_DELETE((*_viEffect));
			_viEffect = _vEffect.erase(_viEffect);
		}
		else ++_viEffect;
	}

	_vEffect.clear();
}

void effectManager::update()
{
	for (_viEffect = _vEffect.begin(); _viEffect != _vEffect.end();) {
		(*_viEffect)->update();
		(*_viEffect)->updateRect();

		//삭제
		if ((*_viEffect)->getState()) {
			(*_viEffect)->release();
			//SAFE_DELETE((*_viEffect));
			_viEffect = _vEffect.erase(_viEffect);
		}
		else ++_viEffect;
	}
}

void effectManager::render()
{
	for (_viEffect = _vEffect.begin(); _viEffect != _vEffect.end(); ++_viEffect)
		(*_viEffect)->render();
}

Effect * effectManager::registerEffect(int effectNum, Effect * effect)
{
	if (!effect) return nullptr;

	_vEffectList.insert(make_pair(effectNum, effect));

	return effect;
}

HRESULT effectManager::addEffect(POINT position, float angle, float speed, float damage, int effect)
{
	effectIter find = _vEffectList.find(effect);

	if (find == _vEffectList.end()) return E_FAIL;

	//이펙트가 있으면 init해서 벡터에 삽입
	if (SUCCEEDED(find->second->init(position, angle, speed, damage))) {
		_vEffect.push_back(find->second);

		return S_OK;
	}

	return E_FAIL;
}

void effectManager::removeEffect(int type)
{
	for (_viEffect = _vEffect.begin(); _viEffect != _vEffect.end(); ++_viEffect) {
		//삭제
		if ((*_viEffect)->getType() == type) {
			(*_viEffect)->setState((*_viEffect)->getState() + 1);

			break;
		}
	}
}
