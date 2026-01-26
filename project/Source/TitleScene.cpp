#include "TitleScene.h"

namespace
{
	const float FOG_TWEEN_TIME = 5.0f;
}

TitleScene::TitleScene()
{
	SetFogColor(200, 100, 0);

	music_select_manager_ = new MusicSelectManager();
}

TitleScene::~TitleScene()
{
	delete music_select_manager_;
	SetFogEnable(FALSE);
}

void TitleScene::Update()
{
	switch (state_)
	{
	case TitleState::TITLE_LOGO:
		if (true)
		{
			state_ = TitleState::SELECT_MUSIC;
			SetFogEnable(TRUE);
		}
		break;
	case TitleState::SELECT_MUSIC:
		UpdateFog();
		music_select_manager_->Update();
		if (CheckHitKey(KEY_INPUT_P))
		{
			SceneManager::ChangeScene("PLAY");
		}
		break;
	default:
		break;
	}

	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}


void TitleScene::Draw()
{
	switch (state_)
	{
	case TitleState::TITLE_LOGO:
		break;
	case TitleState::SELECT_MUSIC:
		music_select_manager_->Draw();
		break;
	default:
		break;
	}

	extern const char* Version();
	DrawString(0, 20, Version(), GetColor(255,255,255));
	DrawString(0, 0, "TITLE SCENE", GetColor(255,255,255));
	DrawFormatString(100, 100, GetColor(255,255,255), "fps_%f", 1.0f / Time::DeltaTimePlane());
	DrawString(100, 400, "Push [P]Key To Play", GetColor(255, 255, 255));
}

void TitleScene::UpdateFog()
{
	if (fog_tween_rate_ < 0.0f) return;

	// tween‚·‚é
	fog_tween_rate_ += Time::DeltaTime();
	if (fog_tween_rate_ <= 1.0f)
	{
		current_fog_ = begin_fog_ + (target_fog_ - begin_fog_) * (fog_tween_rate_ * FOG_TWEEN_TIME);
		SetFogColor((int)(current_fog_.r * 255.0f), (int)(current_fog_.g * 255.0f), (int)(current_fog_.b * 255.0f));
	}

}
