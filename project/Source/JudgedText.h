#pragma once
#include "../Library/GameObject.h"
#include "../Library/transform.h"

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

	void SetJudgeResult(JudgeResult result, float horizontal_rate);

private:
	Transform transform_;
	Vector2I image_size_;
	int image_handle_;
	int judge_result_;

	int active_frame_;
};