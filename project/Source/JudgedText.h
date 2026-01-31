#pragma once
#include "../Library/GameObject.h"
#include "../Library/transform.h"
#include "ActionEvent.h"

enum class JudgeResult;

/// <summary>
/// 判定結果を表示するテキストオブジェクトを表します。
/// </summary>
class JudgedText : public GameObject
{
public:
	JudgedText();
	~JudgedText();

	void Update() override;
	void Draw() override;

	bool CanCreate() const;
	void SetJudgeResult(JudgeResult result, float horizontal_rate);
	void DrawImage() const;

private:
	void OnCreate();

	Transform transform_;
	ActionEvent<const JudgedText*> draw_action_;
	Vector2I image_size_;
	int judge_result_;

	int active_frame_;
};

namespace JudgedTextContext
{
	void Init();
	void Release();

	void Create(JudgeResult result, float horizontal_rate);
	void DrawImage(const JudgedText* self);
}