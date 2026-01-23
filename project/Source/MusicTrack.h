#pragma once
#include "../Library/GameObject.h"
#include "../Library/Transform.h"
#include "../Library/json.hpp"
#include "ActionEvent.h"

class NotesMetaData;

/// <summary>
/// 選択できる楽曲クラス。
/// </summary>
class MusicTrack
{
public:
	MusicTrack(const NotesMetaData& meta_data);
	~MusicTrack();

	void Update();
	void Draw();
	void DrawJacket() const;
	void DrawTrackData() const;

	void LoadJacketImage(const std::string& parent_path);
	NotesMetaData* GetMetaData() const { return meta_data_; }

	Transform transform_;
	ActionEvent<> draw_action_;


private:
	NotesMetaData* meta_data_ = nullptr;
	int jacket_image_ = -1;
	Color level_color_;
};