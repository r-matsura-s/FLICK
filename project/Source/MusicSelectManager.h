#pragma once
#include "../Library/GameObject.h"

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

private:
	std::vector<MusicTrack*> music_tracks_;
	int select_index_ = 0;
};