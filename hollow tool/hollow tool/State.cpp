#include "stdafx.h"
#include "State.h"
#include "Player.h"

Player* State::_player = NULL;

HRESULT State::init(Player * player, bool reverse)
{
	_player = player;

	return S_OK;
}
