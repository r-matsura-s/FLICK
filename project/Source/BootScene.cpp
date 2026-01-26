#include "BootScene.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "NotesMetaData.h"
#include "ResultDataConnector.h"

BootScene::BootScene()
{
	InputManager::Instance(); // InputManagerの生成
	SoundManager::Instance(); // SoundManagerの生成
	new NotesMetaDataContext(); // 楽曲メタデータコンテキストの生成
	new ResultDataConnector(); // 結果データコネクタの生成
}

BootScene::~BootScene()
{
}

void BootScene::Update()
{
	SceneManager::ChangeScene("TITLE"); // 起動が終わったらTitleを表示
}

void BootScene::Draw()
{
}
