#include "stdafx.h"
#include "printManager.h"

HRESULT printManager::init()
{
	_debug = false;

	return S_OK;
}

void printManager::release()
{
}

void printManager::print(HDC hdc, float x, float y, const char * str)
{
	sprintf_s(_text, str);
	TextOut(hdc, x, y, _text, strlen(_text));
}


