#include "TitleScene.h"
#include "Object3D.h"
#include "TitleMoveObjectManager.h"
#include "../Library/Renderer2D.h"

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
float OutQuint(float begin, float end, float x);
float OutQuint(float begin, float end, float x) {
	if (x > 1.0f) {
		x = 1.0f - (x - 1.0f);
		//return x * x * x * x;
	}
	return begin + (end - begin) * (1.0f - std::powf(1.0f - x, 5)); 
}

TitleScene::TitleScene()
{
	SetCameraPositionAndTarget_UpVecY(Vector3::Zero(), Vector3::UnitZ());

	SetFogColor(200, 100, 0);
	SetFogStartEnd(8.0f, 2300.0f);
	target_fog_ = Color(0.1f, 0.8f, 0.8f);
	colors_index_ = GetRand(RAND_MAX);
	ChangeFogColor();

	new Object3D("Sky_Daylight01", Transform(Vector3(), Vector3(), Vector3::One()));
	new TitleMoveObjectManager();

	music_select_manager_ = new MusicSelectManager();

	logo_image_.resize(2, -1);
	for (int i = 0; i < logo_image_.size(); i++)
	{
		logo_image_[i] = LoadGraph(("data/texture/logo_title_" + std::to_string(i) + ".png").c_str());
	}
	logo_size_ = GetGraphSize(logo_image_[0]);
}

TitleScene::~TitleScene()
{
	delete music_select_manager_;
	SetFogEnable(FALSE);

	for (int i = 0; i < logo_image_.size(); i++)
	{
		DeleteGraph(logo_image_[i]);
	}
	logo_image_.clear();
}

void TitleScene::Update()
{
	switch (state_)
	{
	case TitleState::TITLE_LOGO:
		logo_scale_rate_ += Time::DeltaTime();
		if (logo_scale_rate_ > 2.0f)
		{
			logo_scale_rate_ = 0.0f;
		}

		if (CheckHitKeyAll())
		{
			state_ = TitleState::SELECT_MUSIC;
			SetFogEnable(TRUE);
			music_select_manager_->OnTrackChange();
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
		DrawTitleLogo();
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

	// tweenする
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

void TitleScene::DrawTitleLogo()
{
	// BG
	Renderer::RectRotaGraph(Vector2::Zero(), Vector2I::Zero(), logo_size_, 1.0f, 0.0f, logo_image_[0]);
	
	// アニメーション用
	DxLib::SetDrawBright(1, 0, 1);
	Renderer::RectRotaGraph(Vector2::Zero(), Vector2I::Zero(), logo_size_, OutQuint(1.0f, 1.15f, logo_scale_rate_), 0.0f, logo_image_[1]);
	DxLib::SetDrawBright(255, 0, 255);
	Renderer::RectRotaGraph(Vector2::Zero(), Vector2I::Zero(), logo_size_, OutQuint(1.0f, 1.1f, logo_scale_rate_), 0.0f, logo_image_[1]);
	Renderer::SetBlendMode_Add(64);
	Renderer::RectRotaGraph(Vector2::Zero(), Vector2I::Zero(), logo_size_, OutQuint(1.0f, 1.1f, logo_scale_rate_), 0.0f, logo_image_[1]);

	// 上から重ねる
	DxLib::SetDrawBright(255, 255, 255);
	Renderer::ResetBlendMode();
	Renderer::RectRotaGraph(Vector2::Zero(), Vector2I::Zero(), logo_size_, 1.0f, 0.0f, logo_image_[1]);
}
