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

private:
	TitleState state_ = TitleState::TITLE_LOGO;
	MusicSelectManager* music_select_manager_;
};
