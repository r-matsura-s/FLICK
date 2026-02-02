#pragma once
#include "../Library/SceneBase.h"
#include "MusicSelectManager.h"
#include "ActionEvent.h"

enum class TitleState
{
	TITLE_LOGO = 0,
	SELECT_MUSIC,
	TITLE_END
};

/// <summary>
/// タイトルシーン
/// 
/// タイトルを表示して、キーを押したらプレイシーンに移行する。
/// </summary>
class TitleScene : public SceneBase
{
public:
	TitleScene();
	~TitleScene();
	
	void Update() override;
	void Draw() override;

	void UpdateFog();

private:
	void ChangeFogColor();
	void DrawTitleLogo();

	// ----------------------------------
	// タイトルロゴ関連
	// ----------------------------------

	TitleState state_ = TitleState::TITLE_LOGO;
	Vector2I logo_size_;
	std::vector<int> logo_image_;
	float logo_scale_rate_ = 0.0f;
	
	// ----------------------------------
	// 楽曲選択関連
	// ----------------------------------

	MusicSelectManager* music_select_manager_;
	
	Color current_fog_;
	Color begin_fog_;
	Color target_fog_;
	int colors_index_ = 0;
	float fog_tween_rate_ = -1.0f;
};
