#pragma once
#include "singletonBase.h"
#include "timer.h"

class timeManager : public singletonBase<timeManager>
{
private:
	timer* _timer;
	
	float _start, _end;
	bool _onTimer;

public:
	timeManager();
	~timeManager();

	HRESULT init();
	void release();
	void update(float lock = 0.0f);
	void render(HDC hdc, float x, float y);

	void setTimer();
	float clock();
	void endTimer();

	inline float getElapsedTime() const { return _timer->getElapsedTime(); }
	inline float getWorldTime() const { return _timer->getWorldTime(); }
};