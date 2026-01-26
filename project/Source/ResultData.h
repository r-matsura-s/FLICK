#pragma once
#include "MusicTrack.h"
#include "../Library/GameObject.h"

class ResultData
{
public:
	ResultData();
	~ResultData();

	void DrawScore(const Vector2& pos) const;

	// ここでnewする
	void SetNotesMeta(const NotesMetaData& data, bool dont_delete = true);
	void DeleteNotesMeta() { SAFE_DELETE(played_track_); }

	void OnJudge(int judge_enum, int current_combo);

	MusicTrack* played_track_; // プレイした楽曲の情報
	int score_;				// スコア
	int max_combo_;			// 最大コンボ数
	int judge_counts_[5];	// PERFECT, GREAT, GOOD, BAD, MISS

	void AddScore(int judge_enum);
private:
	void AddJudgeCount(int judge_enum);
	void ComperSetMaxCombo(int current_combo);

	bool delete_played_track_ = false;
};