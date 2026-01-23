#pragma once
#include "../Library/GameObject.h"
#include <vector>
#include "NotesMetaData.h"

class Notes;
class SoundManager;

// 判定タイプ
enum class JudgeResult
{
	PERFECT = 0,
	GREAT,
	GOOD,
	BAD,
	MISS,
	NONE,
	MAX
};

enum class NotesTypeCreator
{
	NOMAL = 0,
	HOLD,
	NONE
};

class NotesManager : public GameObject
{
public:
	NotesManager();
	~NotesManager();

	void Update() override;
	void Draw() override;

	enum class InGameState
	{
		LOAD,
		READY,
		PLAY,
		PAUSE,
		FINISH,
	};

	/// <summary>
	/// 曲のBPM、小節番号、拍子、拍の位置からノーツの再生時間（秒）を計算します。
	/// </summary>
	/// <param name="bpm">曲のBPM</param>
	/// <param name="measure_num">小節番号(1から始まる)</param>
	/// <param name="beats_num">1小節あたりの拍数(例: 4拍子の場合は4)</param>
	/// <param name="beat_pos">小節内の拍の位置(1から始まる。例: 2拍目なら2, 2拍目の裏[8分]なら2.5)</param>
	/// <returns>曲の開始からの経過時間（秒）</returns>
	static float CalcNotesTime(float bpm, int measure_num, int beats_num, float beat_pos);
	static float TimeToTick(float time_sec);

private:
	void UpdateStateLoad();
	void UpdateStateReady();
	void UpdateStatePlay();
	void UpdateStatePause();
	void UpdateStateFinish();

	void OnJudged(const Notes* note, JudgeResult result);
	void CreateNotes(const std::string& file_name, float hs_rate);
	void JudgeNotes();

	inline void ChangeState(InGameState next_state) { ingame_state_ = next_state; }
	inline float GetBeatTime() const { return (60.0f / current_bpm_); }

	// ノーツのリスト
	std::vector<Notes*> notes_list_;
	
	SoundManager* sound_manager_;

	float ready_timer_ = 0.0f;
	int ready_count_ = 0;

	NotesMetaData meta_data_;
	float current_bpm_ = 120.0f;
	int combo_count_ = 0;
	InGameState ingame_state_ = InGameState::LOAD;

	bool is_auto_play_ = false;
};