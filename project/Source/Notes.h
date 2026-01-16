#pragma once
#include "../Library/GameObject.h"
#include "ActionEvent.h"

class SoundManager;

enum class NotesType
{
	TAP_1 = 0,
	TAP_2,
	HOLD_1,
	HOLD_2,
	SIDE_1,
	SIDE_2,
	FLICK_L,
	FLICK_R,
	DAMAGE,
	HEALING,
	MAX
};

/// <summary>
/// ノーツクラス
/// GameObjectは継承してないので注意
/// </summary>
class Notes
{
public:
	Notes() : Notes(0.0, 1.0f, NotesType::TAP_1) {}
	// 通常 (ホールド以外はこれで)
	Notes(double horizontal, double arrive_time, NotesType type);
	// ホールド専用
	Notes(NotesType type, double begin_horizontal, double begin_arrive_time, double end_horizontal, double end_arrive_time);
	~Notes();

	void Update();
	void Draw();

	void PlayTapSE() const;

	void SetJudged(bool judged);

	// ホールド用
	inline void SetHoldStarted(bool started) { hold_started_ = started; }
	inline void SetHolding(bool holding) { is_holding_ = holding; }
	inline void SetHoldEnded(bool ended) { hold_ended_ = ended; }

	inline NotesType Type()		const { return type_; }
	inline float Horizontal()	const { return horizontal_rate_; }
	inline float ArriveTime()	const { return arrive_time_; }
	inline float CurrentTime()	const { return current_time_; }
	inline float DiffTime()		const { return std::abs(current_time_ - arrive_time_); }
	inline bool IsJudged()		const { return judged_; }
	inline bool IsMissSkip()	const { return ((type_ == NotesType::DAMAGE) || (type_ == NotesType::HEALING)); }
	inline const Vector3& Position() const { return position_; }

	// ホールド用
	inline bool IsHoldNote()	const { return (type_ == NotesType::HOLD_1 || type_ == NotesType::HOLD_2); }
	inline float HoldEndTime()	const { return hold_end_time_; }
	inline float HoldEndDiff()	const { return hold_end_time_ - current_time_; }
	inline bool IsHolding()		const { return is_holding_; }
	inline bool HoldStarted()	const { return hold_started_; }
	inline bool HoldEnded()		const { return hold_ended_; }

	// StringToEnum
	static NotesType ToNoteType(const std::string& note_type);

	void UpdateVerticalPos();

private:
	void UpdateMove();
	void UpdateOnHold();
	void UpdateGuidSound();

	void SetTapSound();
	void SetUpdateAction();
	void SetDrawAction();
	void SetNoteImage();

	void DrawTap(const Vector3& pos, const Color& color) const;
	void DrawHold(const Vector3& begin, const Vector3& end, Color color) const;
	void DrawFlick() const;
	void DrawDamage() const;
	void DrawHealing() const;
	
	ActionEvent<> update_action_;	// 更新処理を登録する
	ActionEvent<> draw_action_;		// 描画処理を登録する
	std::string tap_sound_;			// タップ音サウンド名
	
	Vector3 position_;				// ノーツ座標
	float horizontal_rate_ = 0.0f;	// ノーツの水平位置(-1.0 ~ 1.0)
	NotesType type_;				// なんのノーツか

	float arrive_time_;				// ノーツ到達時間(秒)
	float current_time_ = 0.0f;		// 経過時間(秒)

	int note_handle_ = -1;			// 通常ノーツ画像ハンドル
	int note_overlay_handle_ = -1;	// 通常ノーツ[2]画像ハンドル
	int flick_handle_ = -1;			// フリックノーツ画像ハンドル

	bool judged_ = false;			// 判定済みか

	bool sound_played_ = false;		// ガイド音再生済みか

	// ホールド専用
	Vector3 start_position_;		// ホールドの開始位置を記録
	Vector3 end_position_;			// ホールドノーツ終了位置
	float hold_end_time_;			// ホールドノーツ終了時間
	bool is_holding_ = false;		// ホールド中か
	bool hold_started_ = false;		// ホールド開始済みか
	bool hold_ended_ = false;		// ホールド終了済みか
	bool end_sound_played_ = false;	// ホールド終了音再生済みか
};