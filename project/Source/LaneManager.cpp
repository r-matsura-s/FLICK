#include "LaneManager.h"


namespace
{
	static const Color LANE_COLOR = Color(0.5f, 0.5f, 0.5f);
	static const Vector3 LANE_SIZE = Vector3(400.0f, 1.0f, 3200.0f);
	static const int LANE_NUM = 4;

	static const float LINE_WIDTH = 5.5f;
	static const bool LINE_DRAW = false;
}

float LaneLeapHorizontalRate(float x)
{
	static const Vector3 lane_min = LANE_SIZE * -1.0f;
	static const Vector3 lane_max = Vector3(LANE_SIZE.x, -LANE_SIZE.y, -LANE_SIZE.z);

	// 0~1に変換
	x = (x + 1.0f) / 2.0f;
	return lane_min.x + (lane_max.x - lane_min.x) * x;
}

float LaneLeapVerticalRate(float z)
{
	return LANE_SIZE.z * z;
}

Vector3 LaneLeapPosition(float horizontal_rate, float vertical_rate)
{
	return Vector3(
		LaneLeapHorizontalRate(horizontal_rate),
		LANE_SIZE.y, 
		LaneLeapHorizontalRate(vertical_rate)
	);
}

LaneManager::LaneManager()
{
}

LaneManager::~LaneManager()
{
}

void LaneManager::Update()
{
}

void LaneManager::Draw()
{
	// レーンを引く
	Vector3 lane_min = LANE_SIZE * -1.0f;
	DrawCube3D(lane_min, LANE_SIZE, LANE_COLOR.RGB16(), Color::White16(), TRUE);

	// 判定ライン
	DrawLine3D(Vector3(-5000, 2, 0), Vector3(5000, 2, 0), Color::Yellow16());

	// レーンの区切り線を引く
	if (LINE_DRAW)
	{
		Vector3 lane_max = Vector3(LANE_SIZE.x, -LANE_SIZE.y, -LANE_SIZE.z);
		for (int i = 0; i <= LANE_NUM; i++)
		{
			// 線を引く座標を求める
			float rate = (float)i / (float)LANE_NUM;
			Vector3 pos = LANE_SIZE.Lerp(lane_min, lane_max, rate);

			// 線を引く
			DrawCapsule3D(pos, pos + Vector3(0, 0, LANE_SIZE.z * 2.0f), LINE_WIDTH, 8, Color::White16(), Color::White16(), TRUE);
		}
	}
}
