#include "stdafx.h"
#include "uiManager.h"
#include "Player.h"

HRESULT uiManager::init(Player* player)
{
	//ÀÌ¹ÌÁö
	_uibarImg = IMAGEMANAGER->addImage("UI_bar", "image/UI/UI_bar.bmp",
		450, 180, true, RGB(7, 7, 7));
	_hpImg = IMAGEMANAGER->addImage("UI_hp", "image/UI/UI_hp.bmp",
		36, 48, true, RGB(255, 0, 255));
	_nonhpImg = IMAGEMANAGER->addImage("UI_nonhp", "image/UI/UI_nonhp.bmp",
		36, 48, true, RGB(255, 0, 255));
	_soulImg = IMAGEMANAGER->addImage("UI_soul", "image/UI/UI_soul.bmp",
		100, 100, true, RGB(247, 231, 241), true);
	_geoImg = IMAGEMANAGER->addImage("UI_geo", "image/UI/UI_geo.bmp",
		52, 54, true, RGB(253, 9, 9));

	//½ºÅÈ
	//hp
	_hp = player->getHP();
	_hpMax = player->getHPMax();

	//soul
	_soul = player->getSoul();	
	_soulMax = player->getSoulMax();
	_alpha = 255;

	return S_OK;
}

void uiManager::release()
{
}

void uiManager::render(HDC hdc)
{
	RECT rc = CAMERAMANAGER->getScreen();

	_uibarImg->render(hdc, rc.left + 50, rc.top + 50);

	//hp
	for (int i = 1; i <= _hpMax; i++) {
		if (i <= _hp)
			_hpImg->render(hdc, rc.left + 170 + i * 50, rc.top + 125);
		else
			_nonhpImg->render(hdc, rc.left + 170 + i * 50, rc.top + 125);
	}

	//soul
	_soulImg->alphaRender(hdc, rc.left + 94, rc.top + 198 - _soul,
		0, 100 - _soul, 100, _soul, _alpha);

	//geo
	_geoImg->render(hdc, rc.left + 230, rc.top + 185);
	char str[128];
	sprintf_s(str, "%d", INVENTORYMANAGER->getGeo());
	TextOut(hdc, rc.left + 300, rc.top + 200, str, strlen(str));
}

void uiManager::setHP(int num)
{
	_hp = num;
}

void uiManager::setSoul(float num)
{
	_soul = num;

	if (_soul < 30)	_alpha = 125;
	else			_alpha = 255;
}