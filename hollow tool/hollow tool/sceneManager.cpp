#include "stdafx.h"
#include "sceneManager.h"
#include "Scene.h"
#include "stage0.h"
#include "dartMount.h"
#include "stage1.h"
#include "stage2.h"
#include "stage3.h"
#include "bossRoom.h"

Player* sceneManager::_player = NULL;
Scene* sceneManager::_currentScene = NULL;

HRESULT sceneManager::init(Player* player)
{
	_player = player;
	_currentScene = NULL;

	addScene("stage0", new stage0);
	addScene("dartmount", new dartMount);
	addScene("stage1", new stage1);
	addScene("stage2", new stage2);
	addScene("stage3", new stage3);
	addScene("bossroom", new bossRoom);

	setEnemy(true);

	return S_OK;
}

void sceneManager::release()
{
	mapSceneIter miSceneList = _mSceneList.begin();

	for (; miSceneList != _mSceneList.end();)
	{
		if (miSceneList->second != NULL)
		{
			if (miSceneList->second == _currentScene) miSceneList->second->release();
			SAFE_DELETE(miSceneList->second);
			miSceneList = _mSceneList.erase(miSceneList);
		}
		else ++miSceneList;
	}

	ITEMMANAGER->release();

	_mSceneList.clear();
}

void sceneManager::update()
{
	if (_currentScene) _currentScene->update();

	ITEMMANAGER->update();
}

void sceneManager::render()
{
	if (_currentScene) _currentScene->render();
}

void sceneManager::renderFront()
{
	if (_currentScene) _currentScene->renderFront();
}

void sceneManager::setEnemy(bool enemy)
{
	mapSceneIter mIter = _mSceneList.begin();

	for (; mIter != _mSceneList.end(); ++mIter)
		mIter->second->setEnemy(enemy);
}

Scene* sceneManager::addScene(string sceneName, Scene* scene)
{
	if (!scene) return nullptr;

	_mSceneList.insert(make_pair(sceneName, scene));

	return scene;
}

HRESULT sceneManager::changeScene(string sceneName)
{
	mapSceneIter find = _mSceneList.find(sceneName);

	if (find == _mSceneList.end()) return E_FAIL;

	if (find->second == _currentScene) return S_OK;

	ITEMMANAGER->release();
	if (SUCCEEDED(find->second->init(_player)))
	{
		//현재(있던) 씬의 릴리즈 함수를 실행해주고
		if (_currentScene) _currentScene->release();

		//바꾸려는 씬을 현재씬으로 체인지
		_currentScene = find->second;

		CAMERAMANAGER->setFade(FADEIN);

		return S_OK;
	}

	return E_FAIL;
}