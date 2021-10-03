#pragma once
#include "singletonBase.h"

class printManager : public singletonBase<printManager>
{
private:
	char _text[128];
	bool _debug;

	printManager() {};
	friend singletonBase<printManager>;

public:
	HRESULT init();
	void release();

	void print(HDC hdc, float x, float y, const char * str);
	template <typename T>
	void print(HDC hdc, float x, float y, const char * str, T t);

	void setDebug(bool debug) { _debug = debug; }
	bool isDebug() { return _debug; }
};

template<typename T>
inline void printManager::print(HDC hdc, float x, float y, const char * str, T t)
{
	sprintf_s(_text, str, t);
	TextOut(hdc, x, y, _text, strlen(_text));
}
