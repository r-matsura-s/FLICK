#include "MusicTrack.h"
#include "NotesMetaData.h"
#include "../Library/Renderer2D.h"


MusicTrack::MusicTrack(const NotesMetaData& meta_data) : GameObject()
{
	meta_data_ = new NotesMetaData(meta_data);
}

MusicTrack::~MusicTrack()
{
	delete meta_data_;
}

void MusicTrack::Update()
{
	draw_action_.Clear();
	draw_action_ += [this] { this->DrawJacket(); };
	if (true)
	{
		draw_action_ += [this] { this->DrawTitle(); };
	}
}

void MusicTrack::Draw()
{
	draw_action_.Invoke();
}

void MusicTrack::DrawJacket() const
{
	Renderer::RectRotaGraph(transform_.position, Vector2I(), Vector2I(256, 256), 1.0f, 0.0f, jacket_image_);
}

void MusicTrack::DrawTitle() const
{
	Renderer::Text(Vector2(0, 30 * 0), Color(), "" + meta_data_->title_);
	Renderer::Text(Vector2(0, 30 * 1), Color(), "" + meta_data_->sub_title_);
	Renderer::Text(Vector2(0, 30 * 2), Color(), "BPM:" + std::to_string(meta_data_->bpm_));
	Renderer::Text(Vector2(0, 30 * 3), Color(), "ƒŒƒxƒ‹:" + std::to_string(meta_data_->level_));
}
