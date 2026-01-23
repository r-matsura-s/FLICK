#include "MusicSelectManager.h"
#include "JsonTitleCollector.h"
#include "MusicTrack.h"
#include "InputManager.h"
#include "SoundManager.h"

namespace
{
	const char* MUSIC_SHEETS = "data/_MusicSheets/"; //"data/_MusicSheets/"
	const float TRACK_SPACING = 500.0f;
}

MusicSelectManager::MusicSelectManager()
{
	JsonTitleCollector collector;
	collector.LoadTitlesFromDirectory(MUSIC_SHEETS);
	for (int i = 0; i <  collector.titles.size(); i++)
	{
		MusicTrack* track = new MusicTrack(collector.titles[i]);
		music_tracks_.push_back(track);

		track->LoadJacketImage(MUSIC_SHEETS);
		track->transform_.SetParent(&transform_);
		track->transform_.SetPosition(Vector3(i * TRACK_SPACING, 0.0f, 0.0f));
		// 描画処理登録
		track->draw_action_ += [track] { track->DrawJacket(); };
		track->draw_action_ += [track] { track->DrawTrackData(); };
	}

	OnTrackChange();
}

MusicSelectManager::~MusicSelectManager()
{
	for (MusicTrack* track : music_tracks_)
	{
		delete track;
	}
	music_tracks_.clear();
}

void MusicSelectManager::Update()
{
	for (MusicTrack* track : music_tracks_)
	{
		track->Update();
	}

	InputManager& input_manager = *InputManager::Instance();
	if (input_manager.GetButtonDown(XINPUT_BUTTON_RIGHT_SHOULDER) || input_manager.GetKeyDown(KEY_INPUT_W))
	{
		high_spin_rate_ += 0.05f; 
		if (high_spin_rate_ > 1.5f)
		{
			high_spin_rate_ = 1.5f;
		}
	}
	else if (input_manager.GetButtonDown(XINPUT_BUTTON_LEFT_SHOULDER) || input_manager.GetKeyDown(KEY_INPUT_S))
	{
		high_spin_rate_ -= 0.05f;
		if (high_spin_rate_ < 0.05f)
		{
			high_spin_rate_ = 0.05f;
		}
	}
	// 楽曲の選択
	if (input_manager.GetButtonDown(XINPUT_BUTTON_DPAD_RIGHT) || input_manager.GetKeyDown(KEY_INPUT_D))
	{
		select_index_ = (select_index_ + 1) % music_tracks_.size();
		OnTrackChange();
	}
	else if (input_manager.GetButtonDown(XINPUT_BUTTON_DPAD_LEFT) || input_manager.GetKeyDown(KEY_INPUT_A))
	{
		select_index_ = (select_index_ - 1 + music_tracks_.size()) % music_tracks_.size();
		OnTrackChange();
	}
	// 楽曲の決定
	if (input_manager.GetButtonDown(XINPUT_BUTTON_A) || input_manager.GetKeyDown(KEY_INPUT_SPACE))
	{
		//OnTrackChange();
	}

}

void MusicSelectManager::Draw()
{
	Vector2 box_padding = Vector2(420, 40);
	Vector2 box_offset = Vector2(0, -480);
	Vector2 box_pos1 = Vector2(0, 450) + box_padding + box_offset;
	Vector2 box_pos2 = Vector2(Screen::WIDTH, Screen::HEIGHT) - box_padding + box_offset;
	DrawBoxAA(box_pos1.x, box_pos1.y, box_pos2.x, box_pos2.y, Color(0.04f, 0.08f, 0.16f).RGB16(), TRUE);
	DrawBoxAA(box_pos1.x, box_pos1.y, box_pos2.x, box_pos2.y, Color::White16(), FALSE, 5.0f);

	Vector2I text_pos = Vector2I(540, 160);
	float notes_speed = 1.0f + (13.0f - 1.0f) * high_spin_rate_;
	DrawFormatString(text_pos.x, text_pos.y, Color::White16(), "ノーツ速度:%.1f", notes_speed);

	for (MusicTrack* track : music_tracks_)
	{
		track->Draw();
	}
}

void MusicSelectManager::OnTrackChange()
{
	// 選択された楽曲のメタデータを更新
	NotesMetaDataContext* selected = FindGameObject<NotesMetaDataContext>();
	selected->meta_data_ = *music_tracks_[select_index_]->GetMetaData();
	selected->high_spin_rate_ = high_spin_rate_;

	// BGMを変更して再生
	SoundManager& sound_manager = *SoundManager::Instance();
	sound_manager.LoadSound(selected->meta_data_.music_, ".wav", SoundType::MUSIC);
	sound_manager.SetBGMVolume(selected->meta_data_.volume_);
	sound_manager.PlayBGM();

	// トラックの位置を更新
	Vector3 target_pos = Vector3(-select_index_ * TRACK_SPACING, 0.0f, 0.0f);
	transform_.SetPosition(target_pos);
}
