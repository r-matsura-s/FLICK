#include "TitleScene.h"

TitleScene::TitleScene()
{
	music_select_manager_ = new MusicSelectManager();
}

TitleScene::~TitleScene()
{
	delete music_select_manager_;
}

void TitleScene::Update()
{
	music_select_manager_->Update();

	if (CheckHitKey(KEY_INPUT_P)) {
		SceneManager::ChangeScene("PLAY");
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}


void TitleScene::Draw()
{
	music_select_manager_->Draw();

	extern const char* Version();
	DrawString(0, 20, Version(), GetColor(255,255,255));
	DrawString(0, 0, "TITLE SCENE", GetColor(255,255,255));
	DrawFormatString(100, 100, GetColor(255,255,255), "fps_%f", 1.0f / Time::DeltaTimePlane());
	DrawString(100, 400, "Push [P]Key To Play", GetColor(255, 255, 255));
}
