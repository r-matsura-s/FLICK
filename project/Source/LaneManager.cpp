#include "LaneManager.h"
#include "../Library/Renderer2D.h"
#include "../Library/resourceLoader.h"

namespace
{
	static const Color LANE_COLOR = Color(0.5f, 0.5f, 0.5f);
	static const Vector3 LANE_SIZE = Vector3(480.0f, 1.0f, 3200.0f);
	static const int LANE_NUM = 4;

	//static const float LINE_WIDTH = 5.5f;
	//static const bool LINE_DRAW = false;

	static const float LINE_LENGTH = 485.0f;
	static const int JUDGE_LINE_COLOR = 0xFF7F27;

	static const float SIDE_WALL_HEIGHT = 240.0f;
	static const float SIDE_WALL_SCROLL_SPEED = 0.5f;
}

#pragma region static関連

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

#pragma endregion

LaneManager::LaneManager()
{
	//judge_line_image_ = LoadGraph("data/texture/judge_line.png");
	side_wall_image_ = LoadGraph("data/texture/side_wall.png");
}

LaneManager::~LaneManager()
{
	DeleteGraph(side_wall_image_);
}

void LaneManager::Update()
{
	side_wall_scroll_ += Time::DeltaTime() * SIDE_WALL_SCROLL_SPEED;
}

void LaneManager::Draw()
{
	// レーンを引く
	Vector3 lane_min = LANE_SIZE * -1.0f;
	DrawCube3D(lane_min - Vector3(0, 1, 0), LANE_SIZE - Vector3(0, 1, 0), LANE_COLOR.RGB16(), Color::White16(), TRUE);

	// 判定ライン
	DrawJudgeLine();

#if (false)
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
#endif
}

void LaneManager::DrawJudgeLine()
{
	// 判定ライン
	Vector3 line_pos_1 = ConvWorldPosToScreenPos(Vector3(-LINE_LENGTH, 0.0f, 0.0f));
	Vector3 line_pos_2 = ConvWorldPosToScreenPos(Vector3(LINE_LENGTH, 0.0f, 0.0f));//カメラがz回転するので2つ獲得
	Renderer::SetBlendMode_Alpha(90);
	DrawLineAA(line_pos_1.x, line_pos_1.y, line_pos_2.x, line_pos_2.y, JUDGE_LINE_COLOR, 11.0f);
	DrawLineAA(line_pos_1.x, line_pos_1.y, line_pos_2.x, line_pos_2.y, JUDGE_LINE_COLOR, 7.0f);
	DrawLineAA(line_pos_1.x, line_pos_1.y, line_pos_2.x, line_pos_2.y, JUDGE_LINE_COLOR, 3.0f);
	Renderer::ResetBlendMode();
	DrawLineAA(line_pos_1.x, line_pos_1.y, line_pos_2.x, line_pos_2.y, JUDGE_LINE_COLOR, 2.0f);
}

void LaneManager::DrawSideWall()
{
	// 横の壁
	float horizontal = -1.0f;
	Vector3 norm = Vector3(1.0f, 0.0f, -0.1f).Normalize();
	Vector3 height = Vector3(0.0f, SIDE_WALL_HEIGHT, 0.0f);
	Color tex_color = Color(0.0f, 0.6f, 0.8f);
	norm = Vector3(-1.0f, 0.0f, -0.1f).Normalize();// なぜかこれで表示できる
	SetTextureAddressMode(DX_TEXADDRESS_WRAP);
	// 頂点情報
	VERTEX3D side_wall[6] =
	{
		GetVertex3D(LaneLeapPosition(horizontal, -1.0f) + height,	norm, tex_color, Color::White(), Vector2(0.0f, 0.0f + side_wall_scroll_), Vector2::Zero()),
		GetVertex3D(LaneLeapPosition(horizontal, -1.0f),			norm, tex_color, Color::White(), Vector2(0.0f, 1.0f + side_wall_scroll_), Vector2::Zero()),
		GetVertex3D(LaneLeapPosition(horizontal,  8.0f) + height,	norm, tex_color, Color::White(), Vector2(1.0f, 0.0f + side_wall_scroll_), Vector2::Zero()),
		GetVertex3D(LaneLeapPosition(horizontal, -1.0f),			norm, tex_color, Color::White(), Vector2(0.0f, 1.0f + side_wall_scroll_), Vector2::Zero()),
		GetVertex3D(LaneLeapPosition(horizontal,  8.0f),			norm, tex_color, Color::White(), Vector2(1.0f, 1.0f + side_wall_scroll_), Vector2::Zero()),
		GetVertex3D(LaneLeapPosition(horizontal,  8.0f) + height,	norm, tex_color, Color::White(), Vector2(1.0f, 0.0f + side_wall_scroll_), Vector2::Zero()),
	};
	Renderer::SetBlendMode_Add(180);
	// 左
	DrawPolygon3D(side_wall, 2, side_wall_image_, FALSE);

	//norm = Vector3(-1.0f, 0.0f, -0.1f).Normalize();
	horizontal = 1.0f;
	side_wall[0].pos = LaneLeapPosition(horizontal, -1.0f) + height;	side_wall[0].norm = norm;
	side_wall[1].pos = LaneLeapPosition(horizontal, -1.0f);				side_wall[1].norm = norm;
	side_wall[2].pos = LaneLeapPosition(horizontal,  8.0f) + height;	side_wall[2].norm = norm;
	side_wall[3].pos = LaneLeapPosition(horizontal, -1.0f);				side_wall[3].norm = norm;
	side_wall[4].pos = LaneLeapPosition(horizontal,  8.0f);				side_wall[4].norm = norm;
	side_wall[5].pos = LaneLeapPosition(horizontal,  8.0f) + height;	side_wall[5].norm = norm;

	// 右
	DrawPolygon3D(side_wall, 2, side_wall_image_, TRUE);
	Renderer::ResetBlendMode();
}
