#include "stdafx.h"
#include "Scene.h"
#include "Player.h"

Player* Scene::_player = NULL;

HRESULT Scene::init(Player * player)
{
	_player = player;

	return S_OK;
}