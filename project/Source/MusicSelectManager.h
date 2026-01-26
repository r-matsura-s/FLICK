#pragma once
#include "../Library/GameObject.h"
#include "../Library/Transform.h"

class MusicTrack;

/// <summary>
/// 楽曲選択関連、GameObjectではない
/// </summary>
class MusicSelectManager
{
public:
	MusicSelectManager();
	~MusicSelectManager();

	void Update();
	void Draw();

	static const char* GetMusicSheetsPath();

private:
	void OnTrackChange();

	std::vector<MusicTrack*> music_tracks_;
	Transform transform_;
	int select_index_ = 0;
	float high_spin_rate_ = 0.5f;
};