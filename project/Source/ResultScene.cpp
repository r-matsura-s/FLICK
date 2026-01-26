#include "ResultScene.h"
#include "ResultBoardManager.h"

ResultScene::ResultScene()
{
	new ResultBoardManager();
}

ResultScene::~ResultScene()
{
}

void ResultScene::Update()
{
	if (CheckHitKey(KEY_INPUT_T))
	{
		SceneManager::ChangeScene("TITLE");
	}
}

void ResultScene::Draw()
{
	DrawString(0, 0, "RESULT SCENE", GetColor(255, 255, 255));
}
