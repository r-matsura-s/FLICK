#pragma once
#include "../Library/SceneBase.h"

/// <summary>
/// リザルトのシーンを制御する
/// </summary>
class ResultScene : public SceneBase
{
public:
	ResultScene();
	~ResultScene();

	void Update() override;
	void Draw() override;

	void DrawBackImage() const;
	int back_image_ = -1;

	float scene_time_ = 0.0f;
};
