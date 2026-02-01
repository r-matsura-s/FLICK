#include "TitleScene.h"
#include "Object3D.h"
#include "TitleMoveObjectManager.h"

namespace
{
	const float FOG_TWEEN_TIME = 5.0f;
	const int FOG_COLOR_MAX = 4;
	const Color FOG_COLORS[FOG_COLOR_MAX] =
	{
		Color(0.8f, 0.4f, 0.0f),
		Color(0.1f, 0.7f, 0.2f),
		Color(0.8f, 0.8f, 0.0f),
		Color(0.8f, 0.4f, 0.8f),
	};
}

TitleScene::TitleScene()
{
	SetCameraPositionAndTarget_UpVecY(Vector3::Zero(), Vector3::UnitZ());

	SetFogColor(200, 100, 0);
	SetFogStartEnd(10.0f, 2700.0f);
	target_fog_ = Color(0.1f, 0.8f, 0.8f);
	colors_index_ = GetRand(RAND_MAX);
	ChangeFogColor();

	new Object3D("Sky_Daylight01", Transform(Vector3(), Vector3(), Vector3::One()));
	new TitleMoveObjectManager();

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
	if (fog_tween_rate_ >= 1.0f)
	{
		ChangeFogColor();
		return;
	}

	// tween‚·‚é
	fog_tween_rate_ += Time::DeltaTime() / FOG_TWEEN_TIME;
	if (fog_tween_rate_ <= 1.0f)
	{
		current_fog_ = begin_fog_ + (target_fog_ - begin_fog_) * (fog_tween_rate_);
		SetFogColor((int)(current_fog_.r * 255.0f), (int)(current_fog_.g * 255.0f), (int)(current_fog_.b * 255.0f));
	}

}

void TitleScene::ChangeFogColor()
{
	fog_tween_rate_ = 0.0f;
	begin_fog_ = target_fog_;
	current_fog_ = begin_fog_;
	target_fog_ = FOG_COLORS[colors_index_ % FOG_COLOR_MAX];
	colors_index_++;
}
