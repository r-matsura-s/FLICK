#include "MusicTrack.h"
#include "NotesMetaData.h"
#include "../Library/Renderer2D.h"
#include "../Library/resourceLoader.h"

namespace
{
	const Vector2I JAKET_SIZE = Vector2(200, 200);
	const float LEVEL_BOX_SCALE = 1.08f;
	const float TITLE_OFFSET_Y = -340.0f;

	const Color LEVEL_COLORS[3] = 
	{
		Color(0.5f, 0.9f, 0.0f),	// イージー
		Color(0.9f, 0.0f, 0.5f),	// ハード
		Color(0.5f, 0.0f, 0.9f)		// エキスパート
	};
}

MusicTrack::MusicTrack(const NotesMetaData& meta_data) 
{
	meta_data_ = new NotesMetaData(meta_data);
}

MusicTrack::~MusicTrack()
{
	delete meta_data_;
}

void MusicTrack::Update()
{
	/* //外部で登録することに
	draw_action_.Clear();
	draw_action_ += [this] { this->DrawJacket(); };
	if (true)
	{
		draw_action_ += [this] { this->DrawTitle(); };
	}
	// */
}

void MusicTrack::Draw()
{
	draw_action_.Invoke();
}

void MusicTrack::LoadJacketImage(const std::string& parent_path)
{
	// ジャケット画像の読み込み
	jacket_image_ = ResourceLoader::LoadGraph(parent_path + meta_data_->music_ + ".png");

	// レベルカラー設定
	level_color_ = LEVEL_COLORS[meta_data_->GetLevelToIndex()];
}

void MusicTrack::DrawJacket() const
{
	Vector2 position = transform_.GetPositionParent();
	Renderer::Box(position, Vector2(JAKET_SIZE) * LEVEL_BOX_SCALE, level_color_);
	Renderer::RectRotaGraph(position, Vector2I(), JAKET_SIZE, 1.0f, 0.0f, jacket_image_);
	Renderer::Box(position, JAKET_SIZE, Color::Black(), FALSE);
}

void MusicTrack::DrawTrackData() const
{
	Vector2 text_pos = transform_.GetPositionParent() + Vector2(-JAKET_SIZE.x / 2.0f, TITLE_OFFSET_Y);
	Renderer::Text(text_pos + Vector2(0, 30 * 0), Color(), "" + meta_data_->title_);
	Renderer::Text(text_pos + Vector2(0, 30 * 1), Color(), "レベル:" + std::to_string(meta_data_->level_));
	Renderer::Text(text_pos + Vector2(0, 30 * 2), Color(), "BPM:" + std::to_string(meta_data_->bpm_));
}
