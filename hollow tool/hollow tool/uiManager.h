#pragma once
#include "singletonBase.h"

class uiManager : public singletonBase<uiManager>
{
private:
	image* _uibarImg;
	image* _hpImg;
	image* _nonhpImg;
	image* _soulImg;
	image* _geoImg;

	int _hp;
	int _hpMax;
	float _soul;
	float _soulMax;
	float _alpha;

	uiManager() {};
	friend singletonBase<uiManager>;

public:
	HRESULT init(Player* player);
	void release();
	void render(HDC hdc);

	void setHP(int num);
	void setSoul(float num);
};

