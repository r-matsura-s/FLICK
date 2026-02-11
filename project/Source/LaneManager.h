#pragma once
#include "../Library/GameObject.h"

/// <summary>
/// レーン内の座標を取得します。
/// </summary>
/// <param name="horizontal_rate">(-1~1)</param>
/// <param name="vertical_rate">(0~1)</param>
/// <returns>ワールド座標</returns>
Vector3 LaneLeapPosition(float horizontal_rate, float vertical_rate);
/// <summary>
/// レーン内のX座標を取得します。
/// </summary>
/// <param name="x">(-1~1)</param>
/// <returns>ワールド座標.x</returns>
float LaneLeapHorizontalRate(float x);
/// <summary>
/// レーン内のZ座標を取得します。
/// </summary>
/// <param name="z">(-1~1)</param>
/// <returns>ワールド座標.z</returns>
float LaneLeapVerticalRate(float z);


/// <summary>
/// 黒い地面の領域等を管理するクラス
/// </summary>
class LaneManager : public GameObject
{
public:
	LaneManager();
	~LaneManager();

	void Update() override;
	void Draw() override;

	void DrawJudgeLine();
	void DrawSideWall();

private:
	float side_wall_scroll_ = 0.0f;
	int side_wall_image_ = -1;
};