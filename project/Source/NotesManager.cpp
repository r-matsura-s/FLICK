#include "NotesManager.h"
#include "Notes.h"
#include "JudgedText.h"
#include "../Library/Renderer2D.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "NotesLine.h"
#include "../Library/json.hpp"
#include <fstream>
#include "ResultDataConnector.h"
#include "ActionEvent.h"
#include "EffectManager.h"
#include "ActionGameObject.h"

#undef max
#undef min
using namespace nlohmann;

namespace
{
	// 判定ウィンドウの定義（秒）
	static const float PERFECT_WINDOW	= 0.033f;
	static const float GREAT_WINDOW		= 0.066f;
	static const float GOOD_WINDOW		= 0.100f;
	static const float BAD_WINDOW		= 0.150f;

	// ヒーリングノーツの距離判定半径
	static const float HEALING_NOTE_RADIUS = 0.3f;

	static const int READY_COUNT = 4;

	static const float EFFECT_SCALE = 13.0f;
}

// キー配置はここ
#pragma region __region__

/// <summary>
/// ノーツごとのキーバインド取得
/// </summary>
/// <param name="type">判定するノーツタイプ</param>
/// <returns>キーコードのリスト</returns>
static std::vector<int> GetKeySetForNotesType(NotesType type)
{
	std::vector<int> keys;
	InputManager* input = InputManager::Instance();
	switch (type)
	{
	case NotesType::TAP_1:
		keys.push_back(XINPUT_BUTTON_A);
		keys.push_back(XINPUT_BUTTON_X);
		keys.push_back(XINPUT_BUTTON_DPAD_RIGHT);
		keys.push_back(XINPUT_BUTTON_DPAD_DOWN);
		break;

	case NotesType::TAP_2:
		keys.push_back(XINPUT_BUTTON_B);
		keys.push_back(XINPUT_BUTTON_Y);
		keys.push_back(XINPUT_BUTTON_DPAD_UP);
		keys.push_back(XINPUT_BUTTON_DPAD_LEFT);
		break;

	case NotesType::FLICK_L:
		keys.push_back(XINPUT_BUTTON_LEFT_SHOULDER);
		break;
	case NotesType::FLICK_R:
		keys.push_back(XINPUT_BUTTON_RIGHT_SHOULDER);
		break;

	case NotesType::HOLD_1:
		keys.push_back(XINPUT_BUTTON_A);
		keys.push_back(XINPUT_BUTTON_X);
		keys.push_back(XINPUT_BUTTON_DPAD_RIGHT);
		keys.push_back(XINPUT_BUTTON_DPAD_DOWN);
		break;

	case NotesType::HOLD_2:
		keys.push_back(XINPUT_BUTTON_B);
		keys.push_back(XINPUT_BUTTON_Y);
		keys.push_back(XINPUT_BUTTON_DPAD_UP);
		keys.push_back(XINPUT_BUTTON_DPAD_LEFT);
		break;

	default:
		keys.push_back(XINPUT_BUTTON_RIGHT_SHOULDER);
		keys.push_back(XINPUT_BUTTON_LEFT_SHOULDER);
		break;
	}

	return keys;
}

float NotesManager::CalcNotesTime(float bpm, int measure_num, int beats_num, float beat_pos)
{
#if (_DEBUG)
	// BPMが0以下の場合、計算不能のため0を返す
	if (bpm <= 0)
	{
		DebugLog("警告：BPMの値が不正です。" + std::to_string(bpm));
		return 0.0f;
	}
#endif
	// 1拍あたりの秒数を計算
	float secondsPerBeat = 60.0f / bpm;

	// 曲の開始から指定されたノーツまでの総拍数を計算
	float totalBeats = static_cast<float>((measure_num - 1) * beats_num) + (beat_pos - 1.0f);

	// 総拍数に1拍あたりの秒数を掛けて、最終的な秒数を算出
	return totalBeats * secondsPerBeat;
}

float NotesManager::TimeToTick(float time_sec)
{
	return time_sec;//現状使っていないので未実装です
}

#pragma endregion	// static関数定義

NotesManager::NotesManager()
{
	sound_manager_ = SoundManager::Instance();

	EffectManager::LoadResource("note_tap", EFFECT_SCALE);

	combo_count_ = 0;
	//is_auto_play_ = true;

	result_data_ = new ResultData();
}

NotesManager::~NotesManager()
{
	// 楽曲の停止
	if (DxLib::CheckSoundMem(sound_manager_->GetHandleBGM()) != 0)
	{
		DxLib::StopSoundMem(sound_manager_->GetHandleBGM());
	}

	for (Notes* note : notes_list_)
	{
		SAFE_DELETE(note);
	}
	notes_list_.clear();

	SAFE_DELETE(result_data_);
}

void NotesManager::Update()
{
	// ステートごとに更新
	switch (ingame_state_)
	{
	case NotesManager::InGameState::LOAD:	UpdateStateLoad(); break;
	case NotesManager::InGameState::READY:	UpdateStateReady(); break;
	case NotesManager::InGameState::PLAY:	UpdateStatePlay(); break;
	case NotesManager::InGameState::PAUSE:	UpdateStatePause();	break;
	case NotesManager::InGameState::FINISH:	UpdateStateFinish(); break;

	default:
		DebugLogError("NotesManager::Update: 不明なInGameStateです。");
		break;
	}
}

void NotesManager::Draw()
{
	for (Notes* note : notes_list_) 
	{
		note->Draw();
	}

	// コンボ数が1以上のときのみ表示
	if (combo_count_ != 0) 
	{
		Renderer::Text(Vector2(-600, -40), Color::White(), "COMBO:" + std::to_string(combo_count_));
	}

	result_data_->DrawScore(Vector2(-600, -300));
	
	Renderer::Text(Vector2(400, -350), Color::White(), meta_data_.title_);

#if _DEBUG
	if (is_auto_play_)
	{
		Renderer::Text(Vector2(-400, -300), Color::White(), "AUTO PLAY");
	}
#endif
}


void NotesManager::UpdateStateLoad()
{
	//Json配置
	std::string music_name = "MarbleBlue_EXPERT.json";
	float hs_rate = 0.75f;
	if (NotesMetaDataContext* selected = FindGameObject<NotesMetaDataContext>())
	{
		music_name = selected->meta_data_.GetJsonFileName();// 選択された譜面データ名を取得
		hs_rate = selected->high_spin_rate_;// 選択されたハイスピを取得
		CreateNotes("data/_MusicSheets/" + music_name, hs_rate);//ここでリーク???
	}

	ready_timer_ = GetBeatTime();
	ready_count_ = 0;

	ChangeState(InGameState::READY);
}

void NotesManager::UpdateStateReady()
{
	ready_timer_ -= Time::DeltaTime();
	if (ready_timer_ <= 0)
	{
		ready_count_++;
		if (ready_count_ <= READY_COUNT)
		{
			sound_manager_->PlaySE("guid");
			ready_timer_ = GetBeatTime();
			if (ready_count_ == READY_COUNT)
			{
				ready_timer_ = GetBeatTime() * 4.0f;
			}
		}

		if (ready_count_ == READY_COUNT + 1)
		{
			// 曲の再生開始
			sound_manager_->PlayBGM(SoundPlayType::ONCE);
			ChangeState(InGameState::PLAY);
		}
	}
}

void NotesManager::UpdateStatePlay()
{
	// ノーツの更新
	for (Notes* note : notes_list_)
	{
		note->Update();
	}

	// ノーツを逃した場合のコンボリセット
	for (Notes* note : notes_list_)
	{
		if (note->IsJudged()) continue;

		// ホールドノーツの判定処理
		if (note->IsHoldNote())
		{
			// 開始判定を逃した場合
			if (!note->HoldStarted())
			{
				// 開始判定ウィンドウを過ぎていたらミス
				if (note->ArriveTime() - note->CurrentTime() < -BAD_WINDOW)
				{
					note->SetJudged(true);
					combo_count_ = 0;

					JudgedTextContext::Create(JudgeResult::MISS, note->Horizontal());
				}
			}
			//*
			// 終了判定を逃した場合
			else if (note->HoldStarted() && !note->HoldEnded())
			{
				// 終了判定ウィンドウを過ぎていたらミス
				if (note->HoldEndTime() - note->CurrentTime() < -BAD_WINDOW)
				{
					note->SetJudged(true);
					note->SetHolding(false);
					combo_count_ = 0;

					JudgedTextContext::Create(JudgeResult::MISS, note->Horizontal());
				}
			}
			// */
			continue;
		}

		// 通常のノーツの判定ウィンドウを過ぎた場合
		if (!note->IsHoldNote())
		{
			if (note->ArriveTime() - note->CurrentTime() < -BAD_WINDOW)
			{
				note->SetJudged(true); // スルー扱いで判定済みに

				if (note->IsMissSkip()) continue;

				combo_count_ = 0;

				JudgedTextContext::Create(JudgeResult::MISS, note->Horizontal());
			}
		}
	}

	// 判定処理
	JudgeNotes();

	// 楽曲の再生が終わった
	if (!sound_manager_->IsPlayingBGM())
	{
		ChangeState(InGameState::FINISH);
	}
	if (CheckHitKey(KEY_INPUT_1))
	{
		ChangeState(InGameState::FINISH);
	}
}

void NotesManager::UpdateStatePause()
{
}

void NotesManager::UpdateStateFinish()
{
	// 仮
	if (true)
	{
		// プレイデータを送る
		result_data_->SetNotesMeta(meta_data_);
		FindGameObject<ResultDataConnector>()->SetResultData(*result_data_);
		
		SceneManager::ChangeScene("RESULT");
	}
}

void NotesManager::JudgeNotes()
{
    // キーごとに判定済みフラグを用意
    std::vector<bool> key_used;
    // 必要なキーを全てfalseで初期化
	key_used.resize(16, false);// (16)XINPUTのボタン数に合わせる

	// ノーツの判定処理
	for (Notes* note : notes_list_)
	{
		// 判定済み、または画面外のノーツはスキップ
		if (note->IsJudged()) continue;
		if (note->ArriveTime() - note->CurrentTime() < -BAD_WINDOW && !note->IsHoldNote())
		{
			// (ホールドノーツ以外は、BADウィンドウを過ぎたら自動的にミス)
			note->SetJudged(true);
			OnJudged(note, JudgeResult::MISS);
			continue;
		}
		if (note->ArriveTime() - note->CurrentTime() > BAD_WINDOW)
		{
			// (まだ判定するには早いノーツ)
			continue;
		}

		// --- 自動プレイ処理 ---
		if (is_auto_play_)
		{
			if (note->Type() == NotesType::DAMAGE || note->Type() == NotesType::HEALING)
			{
				//passする
			}
			else
			{

				// ホールドノーツの自動処理
				if (note->IsHoldNote())
				{
					// 開始前なら開始を自動で行う（PERFECT ウィンドウ内で）
					if (!note->HoldStarted())
					{
						float diff = note->ArriveTime() - note->CurrentTime();
						if (std::abs(diff) <= PERFECT_WINDOW)
						{
							// ホールド開始を自動で行う（終了時に判定）
							note->SetHoldStarted(true);
							note->SetHolding(true);

							// 開始の判定表示（PERFECT）
							OnJudged(note, JudgeResult::PERFECT);
							// 開始時点では SetJudged(true) しない（終了判定があるため）
						}
					}
					// 開始済みで終了前なら、終了を自動で処理
					else if (note->HoldStarted() && !note->HoldEnded())
					{
						float hold_end_diff = note->HoldEndDiff();
						// 終了タイミングが PERFECT ウィンドウ内なら自動でリリースして PERFECT
						if (std::abs(hold_end_diff) <= PERFECT_WINDOW)
						{
							note->SetJudged(true);
							note->SetHoldEnded(true);
							note->SetHolding(false);
							OnJudged(note, JudgeResult::PERFECT);
							continue;
						}
						// それ以外は自動で押し続ける
						note->SetHolding(true);
					}

					// ホールドの自動処理はここまで（他の判定へ進めない）
					continue;
				}

				// ヒーリング・通常・フリックなどの非ホールド系は、PERFECT ウィンドウ内で自動判定
				{
					float diff = std::abs(note->ArriveTime() - note->CurrentTime());
					if (diff <= PERFECT_WINDOW)
					{
						// ノーツを判定済みにする
						note->SetJudged(true);
						// 常に PERFECT
						OnJudged(note, JudgeResult::PERFECT);
					}
					// 自動プレイのときは、判定を行ったら次のノーツへ
					continue;
				}
			}
		}
		// --- 自動プレイ処理ここまで ---

		// ------------------------------------------------
		// 1. ホールドノーツの処理
		// ------------------------------------------------
		if (note->IsHoldNote())
		{
			// 1a. ホールド開始判定 (まだ開始していない場合)
			if (!note->HoldStarted())
			{
				float diff = note->ArriveTime() - note->CurrentTime();

				// 開始判定ウィンドウ内か？
				if (std::abs(diff) <= GOOD_WINDOW)
				{
					// キーが押された瞬間か？
					std::vector<int> keyset = GetKeySetForNotesType(note->Type());
					bool key_pressed = false;
					int used_key = -1;
					for (int key : keyset)
					{
						if (!key_used[key] && InputManager::Instance()->GetButtonDown(key))
						{
							key_pressed = true;
							used_key = key;
							break;
						}
					}
#if (_DEBUG)
					// キーボード操作
					if (InputManager::Instance()->GetKeyDown(KEY_INPUT_D) || InputManager::Instance()->GetKeyDown(KEY_INPUT_F) || InputManager::Instance()->GetKeyDown(KEY_INPUT_J) || InputManager::Instance()->GetKeyDown(KEY_INPUT_K))
					{
						key_pressed = true;
						used_key = XINPUT_BUTTON_DPAD_DOWN;
					}
#endif

					if (key_pressed)
					{
						// ホールド開始
						note->SetHoldStarted(true);
						note->SetHolding(true);
						key_used[used_key] = true;

						// 開始判定
						JudgeResult result = JudgeResult::NONE;
						if (std::abs(diff) <= PERFECT_WINDOW) result = JudgeResult::PERFECT;
						else if (std::abs(diff) <= GREAT_WINDOW) result = JudgeResult::GREAT;
						else result = JudgeResult::GOOD;

						OnJudged(note, result);
						// ★注意: ここでは SetJudged(true) しない（終了判定があるため）
					}
				}
				// 開始ミス (ウィンドウを過ぎた)
				else if (diff < -GOOD_WINDOW)
				{
					note->SetJudged(true); // ミス
					note->SetHoldEnded(true);
					OnJudged(note, JudgeResult::MISS);
				}
			}
			// 1b. 継続判定 と 終了判定 (すでに開始している場合)
			else if (note->HoldStarted() && !note->HoldEnded())
			{
				// (前回の修正内容)
				std::vector<int> keyset = GetKeySetForNotesType(note->Type());
				bool key_held = false; // 今、押されているか？
				for (int key : keyset)
				{
					if (InputManager::Instance()->GetButton(key)) // GetButton (継続)
					{
						key_held = true;
						break;
					}
				}
#if (_DEBUG)
				// キーボード操作
				if (InputManager::Instance()->GetKey(KEY_INPUT_D) || InputManager::Instance()->GetKey(KEY_INPUT_F) || InputManager::Instance()->GetKey(KEY_INPUT_J) || InputManager::Instance()->GetKey(KEY_INPUT_K))
				{
					key_held = true;
				}
#endif

				float hold_end_diff = note->HoldEndDiff(); // (終了時刻 - 現在時刻)

				// 判定1: 終了ウィンドウ内
				if (std::abs(hold_end_diff) <= GOOD_WINDOW)
				{
					// 1a. 成功 (タイミングよく離した)
					if (note->IsHolding() && !key_held)
					{
						note->SetJudged(true); // 終了
						note->SetHoldEnded(true);
						note->SetHolding(false);
						OnJudged(note, JudgeResult::PERFECT);
						continue; // 処理終了
					}
					// 1b. 継続 (押し続けている)
					else if (key_held)
					{
						note->SetHolding(true);
						continue; // 次のフレームへ
					}
					// 1c. ミス (すでに離していた)
					else if (!note->IsHolding() && !key_held)
					{
						note->SetJudged(true);
						note->SetHoldEnded(true);
						note->SetHolding(false);
						OnJudged(note, JudgeResult::MISS);
						continue; // 処理終了
					}
				}
				// 判定2: 押しっぱなしミス (ウィンドウ過ぎ)
				else if (hold_end_diff < -GOOD_WINDOW)
				{
					if (key_held || note->IsHolding())
					{
						note->SetJudged(true);
						note->SetHoldEnded(true);
						note->SetHolding(false);
						OnJudged(note, JudgeResult::MISS);
						continue; // 処理終了
					}
				}

				// 判定3: 早離しミス (ウィンドウ到達前)
				if (!key_held)
				{
					note->SetJudged(true);
					note->SetHoldEnded(true);
					note->SetHolding(false);
					OnJudged(note, JudgeResult::MISS);
					continue; // 処理終了
				}

				// 判定4: 継続 (ウィンドウ到達前で押している)
				note->SetHolding(true);
			}

			// ホールドノーツの処理はここまで。
			// (他のノーツ判定に進まないよう) ループの次へ
			continue;
		}

		// ヒーリングノーツの判定処理
		if (note->Type() == NotesType::HEALING)
		{
			if (note->ArriveTime() - note->CurrentTime() < GREAT_WINDOW)
			{
				float stick_rate_x = InputManager::Instance()->GetCurrentState().GetMaxStickRateX();
				float distance = std::abs(note->Horizontal() - stick_rate_x);

				// キャラとの距離が近いか
				if (distance <= HEALING_NOTE_RADIUS)
				{
					note->SetJudged(true);
					note->PlayTapSE();
					result_data_->AddScore(1);
				}
			}

			// 処理が完了したら (または継続なら) continue で次のノーツへ
			continue;
		}

		// ------------------------------------------------
		// 2. フリックノーツの処理 (スニペットより)
		// ------------------------------------------------
		JudgeResult result = JudgeResult::NONE; // 判定結果をリセット

		if (note->Type() == NotesType::FLICK_L || note->Type() == NotesType::FLICK_R)
		{
			bool flick_success = false;
			float diff = std::abs(note->ArriveTime() - note->CurrentTime());

			// (スニペットのフリックロジックをそのままコピー)
			if (diff <= GOOD_WINDOW)
			{
				float stick_moved_x = InputManager::Instance()->GetMaxStickMovedX();
				if (note->Type() == NotesType::FLICK_L && stick_moved_x < 0)
				{
					flick_success = true;
					result = JudgeResult::PERFECT;
				}
				else if (note->Type() == NotesType::FLICK_R && stick_moved_x > 0)
				{
					flick_success = true;
					result = JudgeResult::PERFECT;
				}
			}

			if (!flick_success)
			{
				// フリック失敗、またはまだ時間内
				continue;
			}
			// フリック成功なら下に落ちて SetJudged(true) される
		}
		// ------------------------------------------------
		// 3. 通常のノーツ (TAPなど) の処理 (スニペットより)
		// ------------------------------------------------
		else
		{
			float diff = std::abs(note->ArriveTime() - note->CurrentTime());

			// (スニペットの通常ノーツロジックをコピー)
			std::vector<int> keyset = GetKeySetForNotesType(note->Type());
			bool key_pressed = false;
			int used_key = -1;
			for (int key : keyset)
			{
				// ★ GetButtonDown (押した瞬間)
				if (!key_used[key] && InputManager::Instance()->GetButtonDown(key))
				{
					key_pressed = true;
					used_key = key;
					break;
				}
			}
#if (_DEBUG)
			// キーボード操作
			if (InputManager::Instance()->GetKeyDown(KEY_INPUT_D) || InputManager::Instance()->GetKeyDown(KEY_INPUT_F) || InputManager::Instance()->GetKeyDown(KEY_INPUT_J) || InputManager::Instance()->GetKeyDown(KEY_INPUT_K))
			{
				key_pressed = true;
				used_key = XINPUT_BUTTON_DPAD_DOWN;
			}
#endif

			if (!key_pressed)
			{
				// 押されていないならループの次へ
				continue;
			}

			key_used[used_key] = true;

			// 判定
			if (diff <= PERFECT_WINDOW) result = JudgeResult::PERFECT;
			else if (diff <= GREAT_WINDOW) result = JudgeResult::GREAT;
			else if (diff <= GOOD_WINDOW) result = JudgeResult::GOOD;
			else result = JudgeResult::BAD;
		}

		// ------------------------------------------------
		// (フリック成功 または 通常ノーツ成功 の場合のみ、ここに来る)
		// ------------------------------------------------

		// ノーツを判定済みにする
		note->SetJudged(true);

		// 判定結果の処理
		OnJudged(note, result);
	}
}

void NotesManager::OnJudged(Notes* note, JudgeResult result)
{
	// 判定結果の表示
	JudgedTextContext::Create(result, note->Horizontal());

	// コンボ数の更新
	if (result <= JudgeResult::GREAT)
	{
		combo_count_++;
	}
	else
	{
		combo_count_ = 0;
	}

	result_data_->OnJudge((int)result, combo_count_);

	// ノーツ音の再生
	note->PlayTapSE();
	EffectManager::Play("note_tap", note->GetTransform());

	note->finish_action_.Invoke((int)result);
}

#define PARSE_CATCH catch (nlohmann::detail::exception ex) { MessageBox(NULL, _TEXT("JSONのパースに失敗しました。\n"), std::to_string(ex.id).c_str() , MB_OK);exit(0); }

#ifdef PARSE_CATCH
void NotesManager::CreateNotes(const std::string& file_name, float hs_rate)
{
	// コンパイルエラー >>> v145(?)がないとエラーになるかも

	// ファイルを開く
	std::ifstream ifs(file_name);
	if (!ifs.is_open()) 
	{
		DebugLogError("JSONファイルが開けません: " + file_name);
		return;
	}

	try
	{
		// ファイル内容を文字列に読み込む
		json json_obj = json::parse(ifs);
		meta_data_.SetToJsonObj(json_obj);
		current_bpm_ = meta_data_.bpm_;

		sound_manager_->LoadSound(meta_data_.music_, ".wav", SoundType::MUSIC, 2);
		sound_manager_->SetBGMVolume(meta_data_.volume_);

		// ノーツデータの読み込み
		for (auto itr = json_obj["notes"].begin(); itr != json_obj["notes"].end(); itr++)
		{
			notes_list_.push_back(
				new Notes((*itr)["lane"], (*itr)["time"], Notes::ToNoteType((*itr)["type"]))
			);
		}

		// ホールド系ノーツデータの読み込み
		for (auto itr = json_obj["holds"].begin(); itr != json_obj["holds"].end(); itr++)
		{
			double be_time = (*itr)["startTime"];
			double en_time = (*itr)["endTime"];
			double be_lane = (*itr)["startLane"];
			double en_lane = (*itr)["endLane"];
			notes_list_.push_back(
				new Notes(Notes::ToNoteType((*itr)["type"]), be_lane, be_time, en_lane, en_time)
			);
		}

		// 座標の初期化
		for (Notes* note : notes_list_)
		{
			note->SetHighSpin(hs_rate);
			note->UpdateVerticalPos();
		}

	}PARSE_CATCH
	
	DebugLog("-- ノーツデータの読み込み完了 ---");
}
#endif //ifdef PARSE_CATCH
