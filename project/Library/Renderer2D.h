#pragma once
#include "myDxLib.h"
#include "../Source/Screen.h"

namespace Renderer
{
	/* ========== ブレンドモード系 ========== */

	inline int ResetBlendMode()
	{
		return SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	inline int SetBlendMode_Alpha(int alpha)
	{
		return SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	}

	inline int SetBlendMode_Add(int add)
	{
		return SetDrawBlendMode(DX_BLENDMODE_ADD, add);
	}

	/* ========== 描画系 ========== */

	static const Vector2 SCREEN_CENTER = Vector2(Screen::WIDTH, Screen::HEIGHT) * 0.5f;

	/* ========== 図形系 ========== */

	static const int POS_NUM = 24;

	/// <summary>
	/// 四角形を描画します。
	/// </summary>
	/// <param name="pos">矩形の中心座標を表す Vector2 型の値。</param>
	/// <param name="size">矩形の幅と高さを表す Vector2 型の値。</param>
	/// <param name="color">矩形の色を表す Color 型の値。</param>
	/// <param name="fill_flag">矩形を塗りつぶすかどうかを指定するフラグ（TRUE で塗りつぶし、FALSE で枠のみ）。デフォルトは TRUE。</param>
	/// <param name="thickness">枠線の太さ（塗りつぶしの場合は無視される）。デフォルトは 1.0f。</param>
	/// <returns>エラー:-1</returns>
	inline int Box(const Vector2& pos, const Vector2& size, const Color& color, int fill_flag = TRUE, float thickness = 1.0f)
	{
		return DrawBoxAA(
			pos.x - (size.x / 2.0f) + SCREEN_CENTER.x,
			pos.y - (size.y / 2.0f) + SCREEN_CENTER.y,
			pos.x + (size.x / 2.0f) + SCREEN_CENTER.x,
			pos.y + (size.y / 2.0f) + SCREEN_CENTER.y,
			color.RGB16(), fill_flag, thickness
		);
	}

	/// <summary>
	/// 円を描画します。
	/// </summary>
	/// <param name="pos">円の中心座標（Vector2型）。</param>
	/// <param name="radius">円の半径（float型）。</param>
	/// <param name="color">円の色（Color型）。</param>
	/// <param name="fill_flag">塗りつぶしフラグ。TRUEの場合は塗りつぶし、FALSEの場合は枠のみ（デフォルトはTRUE）。</param>
	/// <param name="thickness">円の枠線の太さ（float型、デフォルトは1.0f）。</param>
	/// <returns>エラー:-1</returns>
	inline int Circle(const Vector2& pos, float radius, const Color& color, int fill_flag = TRUE, float thickness = 1.0f)
	{
		return DrawCircleAA(
			pos.x + SCREEN_CENTER.x,
			pos.y + SCREEN_CENTER.y,
			radius,
			POS_NUM,
			color.RGB16(), 
			fill_flag, 
			thickness
		);
	}

	/// <summary>
	/// 楕円を描画します。
	/// </summary>
	/// <param name="pos">楕円の中心座標（2次元ベクトル）。</param>
	/// <param name="radius">楕円の半径（2次元ベクトル、x方向とy方向）。</param>
	/// <param name="color">楕円の色。</param>
	/// <param name="fill_flag">塗りつぶしフラグ。TRUEの場合は塗りつぶし、FALSEの場合は枠線のみ描画します（デフォルトはTRUE）。</param>
	/// <param name="thickness">枠線の太さ。塗りつぶしの場合は無視されます（デフォルトは1.0f）。</param>
	/// <returns>エラー:-1</returns>
	inline int Oval(const Vector2& pos, const Vector2& radius, const Color& color, int fill_flag = TRUE, float thickness = 1.0f)
	{
		return DrawOvalAA(
			pos.x + SCREEN_CENTER.x,
			pos.y + SCREEN_CENTER.y,
			radius.x,
			radius.y,
			POS_NUM,
			color.RGB16(),
			fill_flag,
			thickness
		);
	}

	/// <summary>
	/// 三角形を描画します。
	/// </summary>
	/// <param name="pos1">三角形の1つ目の頂点の座標。</param>
	/// <param name="pos2">三角形の2つ目の頂点の座標。</param>
	/// <param name="pos3">三角形の3つ目の頂点の座標。</param>
	/// <param name="color">三角形の描画に使用する色。</param>
	/// <param name="fill_flag">三角形を塗りつぶすかどうかを指定するフラグ（デフォルトはTRUE）。</param>
	/// <param name="thickness">線の太さ（塗りつぶしでない場合、デフォルトは1.0f）。</param>
	/// <returns>エラー:-1</returns>
	inline int Triangle(const Vector2& pos1, const Vector2& pos2, const Vector2& pos3, const Color& color, int fill_flag = TRUE, float thickness = 1.0f)
	{
		return DrawTriangleAA(
			pos1.x + SCREEN_CENTER.x, 
			pos1.y + SCREEN_CENTER.y,
			pos2.x + SCREEN_CENTER.x,
			pos2.y + SCREEN_CENTER.y,
			pos3.x + SCREEN_CENTER.x,
			pos3.y + SCREEN_CENTER.y,
			color.RGB16(),
			fill_flag,
			thickness
		);
	}

	/// <summary>
	/// 線分の描画。
	/// </summary>
	/// <param name="start">開始座標</param>
	/// <param name="end">終了座標</param>
	/// <param name="color">塗りつぶす色</param>
	/// <param name="thickness">太さ</param>
	/// <returns>エラー:-1</returns>
	inline int Line(const Vector2& start, const Vector2& end, const Color& color, float thickness = 1.0f)
	{
		return DrawLineAA(
			start.x + SCREEN_CENTER.x,
			start.y + SCREEN_CENTER.y,
			end.x + SCREEN_CENTER.x,
			end.y + SCREEN_CENTER.y,
			color.RGB16(),
			thickness
		);
	}

	/* ========== 画像系 ========== */

	/// <summary>
	/// 画像の描画
	/// </summary>
	/// <param name="pos"中心座標></param>
	/// <param name="image">画像ハンドラ</param>
	/// <param name="use_alpha">透明部分を適応するか</param>
	/// <returns>エラー:-1</returns>
	inline int Graph(const Vector2& pos, int image, int use_alpha = TRUE)
	{
		Vector2 size;
		GetGraphSizeF(image, &size.x, &size.y);
		return DrawGraphF(
			pos.x - size.x + SCREEN_CENTER.x,
			pos.y - size.y + SCREEN_CENTER.y,
			image,
			use_alpha
		);
	}

	/// <summary>
	/// 画像の切り取り描画
	/// </summary>
	/// <param name="pos">中心座標</param>
	/// <param name="src">どこから切り取るか(ピクセル)</param>
	/// <param name="size">切り取る大きさ(ピクセル)</param>
	/// <param name="image">画像ハンドラ</param>
	/// <param name="use_alpha">透明部分を適応するか</param>
	/// <returns>エラー:-1</returns>
	inline int RectGraph(const Vector2& pos, const Vector2I& src, const Vector2I& size, int image, int use_alpha = TRUE)
	{
		return DrawRectGraphF(
			pos.x - (size.x / 2.0f) + SCREEN_CENTER.x,
			pos.y - (size.y / 2.0f) + SCREEN_CENTER.y,
			src.x,
			src.y,
			size.x,
			size.y,
			image,
			use_alpha
		);
	}

	/// <summary>
	/// 画像の切り取り描画
	/// </summary>
	/// <param name="pos">中心座標</param>
	/// <param name="src">どこから切り取るか(ピクセル)</param>
	/// <param name="size">切り取る大きさ(ピクセル)</param>
	/// <param name="scale">拡大率</param>
	/// <param name="angle">Z軸回転(ラジアン)</param>
	/// <param name="image">画像ハンドラ</param>
	/// <param name="use_alpha">透明部分を適応するか</param>
	/// <returns>エラー:-1</returns>
	inline int RectRotaGraph(const Vector2& pos, const Vector2I& src, const Vector2I& size, float scale, float angle, int image,int use_alpha = TRUE)
	{
		return DrawRectRotaGraphF(
			pos.x + SCREEN_CENTER.x,
			pos.y + SCREEN_CENTER.y,
			src.x,
			src.y,
			size.x,
			size.y,
			static_cast<double>(scale),
			static_cast<double>(angle),
			image,
			use_alpha
		);
	}

	/* ========== 文字系 ========== */

	/// <summary>
	/// 文字の表示(デフォルトフォント)
	/// </summary>
	/// <param name="pos">左上座標</param>
	/// <param name="color">文字の色</param>
	/// <param name="str">表示する文字列</param>
	/// <returns>エラー:-1</returns>
	inline int Text(const Vector2& pos, const Color& color, const std::string& str)
	{
		return DrawFormatStringF(
			pos.x + SCREEN_CENTER.x,
			pos.y + SCREEN_CENTER.y,
			color.RGB16(),
			"%s", str.c_str()
		);
	}
}