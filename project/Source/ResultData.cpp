#include "ResultData.h"
#include "../Library/Renderer2D.h"

#undef max
#undef min

namespace
{
	const int SCORE_VALUES[5] = 
	{
		1000,	// PERFECT
		800,	// GREAT
		500,	// GOOD
		200,	// BAD
		0		// MISS
	};
}

ResultData::ResultData()
{
	played_track_ = nullptr;
	score_ = 0;
	max_combo_ = 0;
	for (int i = 0; i < 5; i++)
	{
		judge_counts_[i] = 0;
	}
}

ResultData::~ResultData()
{
	SAFE_DELETE(played_track_);
}

void ResultData::DrawScore(const Vector2& pos) const
{
	Renderer::Text(pos, Color::White(), "SCORE: " + std::to_string(score_));
}

void ResultData::SetNotesMeta(const NotesMetaData& data)
{
	if (played_track_ == nullptr)
	{
		played_track_ = new MusicTrack(data);
	}
}

void ResultData::OnJudge(int judge_enum, int current_combo)
{
	AddScore(judge_enum);
	AddJudgeCount(judge_enum);
	ComperSetMaxCombo(current_combo);
}

void ResultData::AddScore(int judge_enum)
{
	if (judge_enum < 5)
	{
		score_ += SCORE_VALUES[judge_enum];
	}
}

void ResultData::AddJudgeCount(int judge_enum)
{
	if (judge_enum < 5)
	{
		judge_counts_[judge_enum]++;
	}
}

void ResultData::ComperSetMaxCombo(int current_combo)
{
	max_combo_ = std::max(max_combo_, current_combo);
}
