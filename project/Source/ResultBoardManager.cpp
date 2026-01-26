#include "ResultBoardManager.h"
#include "ResultDataConnector.h"
#include "../Library/Renderer2D.h"
#include "NotesMetaData.h"
#include "MusicSelectManager.h"

namespace
{
	const int JUDGE_COUNTER_SPEED = 17;
	const int SCORE_COUNTER_SPEED = 10000;
	const int COMBO_COUNTER_SPEED = 20;
}

ResultBoardManager::ResultBoardManager()
{
	play_result_data_ = FindGameObject<ResultDataConnector>()->GetResultData();
	play_result_data_.played_track_->transform_.parent = nullptr;// なんか残っていたので削除
	play_result_data_.played_track_->LoadJacketImage(MusicSelectManager::GetMusicSheetsPath());
}

ResultBoardManager::~ResultBoardManager()
{
	play_result_data_.DeleteNotesMeta();
}

void ResultBoardManager::Update()
{
	UpdateViewResultData();
}

void ResultBoardManager::Draw()
{
	play_result_data_.played_track_->transform_.
		SetPosition(Vector3(200.0f, 0.0f, 0.0f));
	// ジャケットの描画
	play_result_data_.played_track_->DrawJacket();
	// 曲名の描画
	Renderer::Text(Vector2(200.0f, 230.0f), Color::White(), play_result_data_.played_track_->GetMetaData()->title_);

	// スコアの描画
	view_result_data_.DrawScore(Vector2(0.0f, 0.0f));
	// 最大コンボ数の描画
	DrawFormatString(640, 390, GetColor(255, 255, 255), "MAX COMBO: %d", view_result_data_.max_combo_);
	// 判定カウントの描画
	for (int i = 0; i < 5; i++)
	{
		std::string str[5] = { "Perfect", "Great  ", "Good   ", "Bad    ", "Miss   " };
		DrawFormatString(300, 420 + (i * 30), GetColor(255, 255, 255), 
			"%s : %d", str[i].c_str(), view_result_data_.judge_counts_[i]);
	}
}

void ResultBoardManager::UpdateViewResultData()
{
	// 判定カウントの更新
	for (int i = 0; i < 5; i++)
	{
		const int play_count = play_result_data_.judge_counts_[i];
		int& view_count = view_result_data_.judge_counts_[i];

		view_count += JUDGE_COUNTER_SPEED;
		if (view_count > play_count)
		{
			view_count = play_count;
		}
	}

	// スコアの更新
	view_result_data_.score_ += SCORE_COUNTER_SPEED;
	if (view_result_data_.score_ > play_result_data_.score_)
	{
		view_result_data_.score_ = play_result_data_.score_;
	}
	
	// 最大コンボ数の更新
	view_result_data_.max_combo_ += COMBO_COUNTER_SPEED;
	if (view_result_data_.max_combo_ > play_result_data_.max_combo_)
	{
		view_result_data_.max_combo_ = play_result_data_.max_combo_;
	}
}
