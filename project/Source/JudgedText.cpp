#include "JudgedText.h"
#include "NotesManager.h"
#include "../Library/Renderer2D.h"


namespace
{
	// 結果表示のオフセット位置
	static const Vector2 OFFSET_POS = Vector2(0.0f, -80.0f);	
	// ノーツの座標から変換するための倍率
	static const float TO_SCREEN_HORIZONTAL = 250.0f;
	// 画像の拡大率
	static const float IMAGE_SCALE = 0.4f;
	// 判定画像の数
	static const int JUDGE_MAX = 5;
}

JudgedText::JudgedText()
{
	image_handle_ = LoadGraph("data/texture/judge.png");
	image_size_ = GetGraphSize(image_handle_);
	image_size_.y /= JUDGE_MAX;
	judge_result_ = 0;

	active_frame_ = 20;
}

JudgedText::~JudgedText()
{
	DeleteGraph(image_handle_);
}

void JudgedText::Update()
{
	if(--active_frame_ <= 0) 
	{
		DestroyMe();
	}
}

void JudgedText::Draw()
{
	Renderer::RectRotaGraph(Vector2(transform_.position) + OFFSET_POS, Vector2I(0, judge_result_ * image_size_.y), image_size_, IMAGE_SCALE, transform_.rotation.z, image_handle_);

	/* //表示位置確認用
	Vector2 pos = Vector2(TO_SCREEN_HORIZONTAL, 0.0f);
	Renderer::Circle(pos * -1.0f + OFFSET_POS, 20.0f, Color::Magenta());
	Renderer::Circle(pos * -0.5f + OFFSET_POS, 20.0f, Color::Magenta());
	Renderer::Circle(pos *  0.0f + OFFSET_POS, 20.0f, Color::Magenta());
	Renderer::Circle(pos *  0.5f + OFFSET_POS, 20.0f, Color::Magenta());
	Renderer::Circle(pos *  1.0f + OFFSET_POS, 20.0f, Color::Magenta());
	// */
}

void JudgedText::SetJudgeResult(JudgeResult result, float horizontal_rate)
{
	judge_result_ = static_cast<int>(result);
	transform_.position.x = horizontal_rate * TO_SCREEN_HORIZONTAL;
}
