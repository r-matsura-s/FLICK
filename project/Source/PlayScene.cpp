#include "PlayScene.h"
#include "GameManager.h"
#include "SoundManager.h"
#include "StaticMesh.h"

PlayScene::PlayScene()
{
	StaticMeshManager::Instance();
	SoundManager::Instance()->LoadSound("guid", ".wav", SoundType::SE, 20);
	SoundManager::Instance()->LoadSound("note_tap", ".wav", SoundType::SE, 10);

	new GameManager();
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
}

void PlayScene::Draw()
{
	DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
	DrawFormatString(100, 100, GetColor(255, 255, 255), "fps_%f", 1.0f / Time::DeltaTimePlane());
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}
