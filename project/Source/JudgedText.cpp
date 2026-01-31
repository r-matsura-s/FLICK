#include "JudgedText.h"
#include "NotesManager.h"
#include "../Library/Renderer2D.h"
#include "../Library/resourceLoader.h"


namespace
{
	// 結果表示のオフセット位置
	static const Vector2 OFFSET_POS = Vector2(0.0f, 0.0f);	
	// ノーツの座標から変換するための倍率
	static const float TO_SCREEN_HORIZONTAL = 250.0f;
	// 画像の拡大率
	static const float IMAGE_SCALE = 0.3f;
	// 判定画像の数
	static const int JUDGE_MAX = 5;

	// -------------------------------------------------------
	// Context用変数
	// -------------------------------------------------------

	std::vector<JudgedText*>* context_pool_list_;
	int context_judge_image_;
}

JudgedText::JudgedText()
{
	OnCreate();
}

JudgedText::~JudgedText()
{
}

void JudgedText::OnCreate()
{
	image_size_ = GetGraphSize(context_judge_image_);
	image_size_.y /= JUDGE_MAX;
	judge_result_ = 0;

	active_frame_ = 20;
}

void JudgedText::Update()
{
	if(--active_frame_ <= 0) 
	{
		draw_action_.Clear();
	}
}

void JudgedText::Draw()
{
	draw_action_.Invoke(this);
	/* //表示位置確認用
	Vector2 pos = Vector2(TO_SCREEN_HORIZONTAL, 0.0f);
	Renderer::Circle(pos * -1.0f + OFFSET_POS, 20.0f, Color::Magenta());
	Renderer::Circle(pos * -0.5f + OFFSET_POS, 20.0f, Color::Magenta());
	Renderer::Circle(pos *  0.0f + OFFSET_POS, 20.0f, Color::Magenta());
	Renderer::Circle(pos *  0.5f + OFFSET_POS, 20.0f, Color::Magenta());
	Renderer::Circle(pos *  1.0f + OFFSET_POS, 20.0f, Color::Magenta());
	// */
}

bool JudgedText::CanCreate() const
{
	return (active_frame_ <= 0);
}

void JudgedText::SetJudgeResult(JudgeResult result, float horizontal_rate)
{
	OnCreate();

	judge_result_ = static_cast<int>(result);
	transform_.position.x = horizontal_rate * TO_SCREEN_HORIZONTAL;
	
	draw_action_ += JudgedTextContext::DrawImage;
}

void JudgedText::DrawImage() const
{
	Vector2 pos = Vector2(transform_.position) + OFFSET_POS;
	Vector2I src = Vector2I(0, judge_result_ * image_size_.y);
	Renderer::RectRotaGraph(pos, src, image_size_, IMAGE_SCALE, transform_.rotation.z, context_judge_image_);
}
void JudgedTextContext::DrawImage(const JudgedText* self)
{
	self->DrawImage();
}

void JudgedTextContext::Init()
{
	context_pool_list_ = new std::vector<JudgedText*>();
	context_judge_image_ = LoadGraph("data/texture/judge.png");

	const int BUF_SIZE = 30;
	for (int i = 0; i < BUF_SIZE; i++)
	{
		JudgedText* p = new JudgedText();
		context_pool_list_->push_back(p);

		p->DontDestroyOnSceneChange();//一応
	}
}

void JudgedTextContext::Release()
{
	DeleteGraph(context_judge_image_);
	context_judge_image_ = -1;

	for (auto itr = context_pool_list_->begin(); itr != context_pool_list_->end(); itr++)
	{
		JudgedText* p = *itr;
		p->DestroyMe();
	}
	context_pool_list_->clear();
	SAFE_DELETE(context_pool_list_);
}

void JudgedTextContext::Create(JudgeResult result, float horizontal_rate)
{
	for (auto itr = context_pool_list_->begin(); itr != context_pool_list_->end(); itr++)
	{
		JudgedText* p = *itr;
		if (p->CanCreate())
		{
			p->SetDrawOrder(-1000);
			p->SetJudgeResult(result, horizontal_rate);
			return;
		}
	}

	DebugLog("JudgedTextContext::did not create");
}
