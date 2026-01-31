#include "App.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "Time.h"
#include "resourceLoader.h"
#include <DxLib.h>
#include "../Source/ActionEvent.h"
#include "../Source/JudgedText.h"

namespace
{
	struct AppAction
	{
		ActionEvent<> init, update, draw, release;
	};

	AppAction* actions;
}
void AddActions();

void AddActions()
{
	// Init
	actions->init += ObjectManager::Init;
	actions->init += SceneManager::Init;
	actions->init += Time::Init;
	actions->init += ResourceLoader::Init;
	actions->init += JudgedTextContext::Init;

	// Release
	actions->release += JudgedTextContext::Release;
	actions->release += Time::Release;
	actions->release += SceneManager::Release;
	actions->release += ObjectManager::Release;
	actions->release += ResourceLoader::Release;

	// Update
	actions->update += SceneManager::Update;
	actions->update += ObjectManager::Update;

	// Draw
	actions->draw += Time::Refresh;
	actions->draw += ObjectManager::Draw;
	actions->draw += SceneManager::Draw;
}

void AppInit()
{
	actions = new AppAction();
	AddActions();
	actions->init.Invoke();
}

void AppUpdate()
{
	actions->update.Invoke();
}

void AppDraw()
{
	actions->draw.Invoke();
}

void AppRelease()
{
	actions->release.Invoke();
	delete actions;

	OutputDebugStringA("---------- This app program is finished ----------\n");
}

bool AppIsExit()
{
	return SceneManager::IsExit();
}