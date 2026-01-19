#pragma once
#include "../Library/GameObject.h"
#include "../Library/Transform.h"
#include "../Library/json.hpp"
#include "ActionEvent.h"

class NotesMetaData;

/// <summary>
/// 選択できる楽曲クラス。GameObject継承
/// </summary>
class MusicTrack : public GameObject
{
public:
	MusicTrack(const NotesMetaData& meta_data);
	~MusicTrack();

	void Update();
	void Draw();

	Transform transform_;
	ActionEvent<> draw_action_;


private:
	void DrawJacket() const;
	void DrawTitle() const;

	NotesMetaData* meta_data_ = nullptr;
	int jacket_image_ = -1;
};