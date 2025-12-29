#pragma once
#include <DxLib.h>
#include <algorithm>
#include <math.h>
#include <string>
#include "assert.h"
#include "../Source/Screen.h"

// --------------------------------------------------------- //
//															 //
//	myDxLib.h												 //
//															 //
//		DxLib.hのヘルパー定義								 //
//		関数を追加する際はnamespace内にinlineで追加。		 //
//															 //
// --------------------------------------------------------- //


/* ==================== ポインタ削除関連 ==================== */

// nullでなければdeleteする
#define SAFE_DELETE(p) do { \
    if (p != nullptr) {     \
        delete p;           \
        p = nullptr;        \
    }                       \
} while(false)              \

// nullでなければdelete[]する
#define SAFE_DELETE_ARRAY(p) do {	\
    if (p != nullptr) {				\
        delete[] p;					\
        p = nullptr;				\
    }								\
} while(false)						\

/* ==================== 関数定義ここから ==================== */

namespace DxLib 
{
	/* ==================== デバッグ関連 ==================== */

	inline void DebugLog(const std::string& msg)
	{
		OutputDebugStringA(("DxLib_DebugLog::" + msg + "\n").c_str());
	}

	inline void DebugLogError(const std::string& msg)
	{
		MessageBox(nullptr, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
		assert(false);
	}

	/* ==================== ラジアン系の定義 ==================== */

	// 弧度法とラジアンの変換用定数（これを掛ける）
	const float DegToRad = DX_PI_F / 180.0f;
	const float RadToDeg = 180.0f / DX_PI_F;
	const float HalfPI = DX_PI_F / 2.0f;

	/* ==================== ベクトル系の定義 ==================== */

	// VECTORとVECTOR_Dとの型変換
	inline const VECTOR VGet(VECTOR_D v) { return VGet((float)v.x, (float)v.y, (float)v.z);	}
	inline const VECTOR_D VGetD(VECTOR v) { return VGetD((double)v.x, (double)v.y, (double)v.z); }

	// ベクトルの加算
	inline const VECTOR operator +(const VECTOR& v1, const VECTOR& v2) { return VAdd(v1, v2); }
	inline const VECTOR_D operator +(const VECTOR_D& v1, const VECTOR_D& v2) { return VAddD(v1, v2); }

	inline VECTOR& operator +=(VECTOR& v1, const VECTOR& v2) { return v1 = VAdd(v1, v2); }
	inline VECTOR_D& operator +=(VECTOR_D& v1, const VECTOR_D& v2) { return v1 = VAddD(v1, v2); }

	// ベクトルの減算
	inline const VECTOR operator -(const VECTOR& v1, const VECTOR& v2) { return VSub(v1, v2); }
	inline const VECTOR_D operator -(const VECTOR_D& v1, const VECTOR_D& v2) { return VSubD(v1, v2); }

	inline VECTOR& operator -=(VECTOR& v1, const VECTOR& v2) { return v1 = VSub(v1, v2); }
	inline VECTOR_D& operator -=(VECTOR_D& v1, const VECTOR_D& v2) { return v1 = VSubD(v1, v2); }

	// ベクトルを拡大する
	inline const VECTOR operator *(const VECTOR& v1, float scale) { return VScale(v1, scale); }
	inline const VECTOR_D operator *(const VECTOR_D& v1, const double& scale) { return VScaleD(v1, scale); }

	inline VECTOR& operator *=(VECTOR& v1, float scale) { return v1 = VScale(v1, scale); }
	inline VECTOR_D& operator *=(VECTOR_D& v1, const double& scale) { return v1 = VScaleD(v1, scale); }

	// ベクトルを縮小する
	inline const VECTOR operator /(const VECTOR& v1, float scale) { return VScale(v1, 1.0f/scale); }
	inline const VECTOR_D operator /(const VECTOR_D& v1, const double& scale) { return VScaleD(v1, 1.0/scale); }

	inline VECTOR& operator /=(VECTOR& v1, float scale) { return v1 = VScale(v1, 1.0f / scale); }
	inline VECTOR_D& operator /=(VECTOR_D& v1, const double& scale) { return v1 = VScaleD(v1, 1.0 / scale); }

	// 行列の足し算
	inline const MATRIX operator +(const MATRIX& m1, const MATRIX& m2) { return MAdd(m1, m2); }
	inline const MATRIX_D operator +(const MATRIX_D& m1, const MATRIX_D& m2) { return MAddD(m1, m2); }

	inline MATRIX& operator +=(MATRIX& m1, const MATRIX& m2) { return m1 = MAdd(m1, m2); }
	inline MATRIX_D& operator +=(MATRIX_D& m1, const MATRIX_D& m2) { return m1 = MAddD(m1, m2); }

	// 行列の掛け算
	inline const MATRIX operator *(const MATRIX& m1, const MATRIX& m2) { return MMult(m1, m2); }
	inline const MATRIX_D operator *(const MATRIX_D& m1, const MATRIX_D& m2) { return MMultD(m1, m2); }

	inline MATRIX& operator *=(MATRIX& m1, const MATRIX& m2) { return m1 = MMult(m1, m2); }
	inline MATRIX_D& operator *=(MATRIX_D& m1, const MATRIX_D& m2) { return m1 = MMultD(m1, m2); }

	// ベクトルに行列を掛ける
	inline const VECTOR operator *(const VECTOR& v, const MATRIX& m1) { return VTransform(v, m1); }
	inline const VECTOR_D operator *(const VECTOR_D& v, const MATRIX_D& m1) { return VTransformD(v, m1); }

	inline VECTOR& operator *=(VECTOR& v, const MATRIX& m1) { return v = VTransform(v, m1); }
	inline VECTOR_D& operator *=(VECTOR_D& v, const MATRIX_D& m1) { return v = VTransformD(v, m1); }

	/// <summary>
	/// ZXY順の回転行列を取得
	/// </summary>
	/// <param name="rot">回転量(ラジアン)</param>
	inline MATRIX MGetRotationZXY(const VECTOR& rot) {
		return MGetRotZ(rot.z) * MGetRotX(rot.x) * MGetRotY(rot.y);
	}

	// 定数
	static const VECTOR VZero = { 0,0,0 };

	// VECTORとしても使える３次元ベクトルクラス
	class Vector3 : public VECTOR {
	public:
		Vector3() : Vector3(0, 0, 0) {}
		Vector3(float _x, float _y, float _z) {
			x = _x;
			y = _y;
			z = _z;
		}

		Vector3(const Vector3& v) {
			x = v.x;
			y = v.y;
			z = v.z;
		}

		Vector3(const VECTOR& v) {
			x = v.x;
			y = v.y;
			z = v.z;
		}

		Vector3 operator =(VECTOR v) {
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}

		/// <summary>ゼロベクトルを返します。</summary>
		static const Vector3 Zero() noexcept { return Vector3(0.f, 0.f, 0.f); }
		/// <summary>全要素が 1 のベクトルを返します。</summary>
		static const Vector3 One() noexcept { return Vector3(1.f, 1.f, 1.f); }
		/// <summary>単位 X ベクトルを返します。</summary>
		static const Vector3 UnitX() noexcept { return Vector3(1.f, 0.f, 0.f); }
		/// <summary>単位 Y ベクトルを返します。</summary>
		static const Vector3 UnitY() noexcept { return Vector3(0.f, 1.f, 0.f); }
		/// <summary>単位 Z ベクトルを返します。</summary>
		static const Vector3 UnitZ() noexcept { return Vector3(0.f, 0.f, 1.f); }

		// 長さを返す（VSize(VECTOR）と同じ）
		float Size() const {
			return sqrtf(SquareSize());
		}

		// 長さの二乗を返す（VSizeSquare(VECTOR)と同じ）
		float SquareSize() const {
			return x * x + y * y + z * z;
		}

		// 長さを１にする（VNorm(VECTOR)と同じ）
		Vector3 Normalize() const {
			float size = Size();
			if (size == 0.0f) {
				return VZero;
			}
			return *this / size;
		}

		// 内積を求める（VDot(VECTOR, VECTOR)と同じ）
		float Dot(const Vector3& other) const {
			return x*other.x + y*other.y + z*other.z;
		}

		// 外積を求める（VCross(VECTOR, VECTOR)と同じ）
		Vector3 Cross(const Vector3& other) const {
			return Vector3(
				y * other.z + z * other.y,
				z * other.x + x * other.z,
				x * other.y + y * other.x);
		}

		/// <summary>線形補間（a -> b を t で補間）を行います。</summary>
		static Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
			return a + (b - a) * t;
		}
	};

	// ２次元ベクトルクラス
	struct Vector2 {
		float x, y;
		Vector2() : Vector2(0, 0) {}
		Vector2(float _x, float _y) {
			x = _x;
			y = _y;
		}
		Vector2(Vector3 v) : Vector2(v.x, v.y) {}

		/// <summary>ゼロベクトルを返します。</summary>
		static const Vector2 Zero() noexcept { return Vector2(0.f, 0.f); }

		/// <summary>全要素が 1 のベクトルを返します。</summary>
		static const Vector2 One() noexcept { return Vector2(1.f, 1.f); }

		/// <summary>単位 X ベクトルを返します。</summary>
		static const Vector2 UnitX() noexcept { return Vector2(1.f, 0.f); }

		/// <summary>単位 Y ベクトルを返します。</summary>
		static const Vector2 UnitY() noexcept { return Vector2(0.f, 1.f); }

		// 長さを求める
		float Size() const {
			return sqrtf(SquareSize());
		}

		// 長さの二乗を求める
		float SquareSize() const {
			return x * x + y * y;
		}

		// ベクトルの加算
		const Vector2 operator +(const Vector2& v) { return Vector2(x + v.x, y + v.y); }
		Vector2& operator +=(Vector2& v) { x += v.x; y += v.y; return *this; }

		// ベクトルの減算
		const Vector2 operator -(const Vector2& v) { return Vector2(x - v.x, y - v.y); }
		Vector2& operator -=(Vector2& v) { x -= v.y; y -= v.y; return *this; }

		// ベクトルの拡大
		const Vector2 operator *(float scale) { return Vector2(x * scale, y * scale); }
		Vector2& operator *=(float scale) { x *= scale; y *= scale; return *this; }

		// ベクトルの縮小
		const Vector2 operator /(float scale) { return Vector2(x / scale, y / scale); }
		Vector2& operator /=(float scale) { x /= scale; y /= scale; return *this; }

		// 向きベクトルを獲得
		Vector2 Normalize() {
			float size = Size();
			if (size == 0.0f) {
				return Vector2(0, 0);
			}
			return Vector2(x / size, y / size);
		}

		// 内積を求める
		float Dot(const Vector2& other) const {
			return x * other.x + y * other.y;
		}
	};

	// ベクトルの加算
	inline const Vector2 operator +(const Vector2& v1, const Vector2& v2) { return Vector2(v1.x + v2.x, v1.y + v2.y); }
	inline Vector2& operator +=(Vector2& v1, const Vector2& v2) { v1.x += v2.x; v1.y += v2.y; return v1; }

	// ベクトルの減算
	inline const Vector2 operator -(const Vector2& v1, const Vector2& v2) { return Vector2(v1.x - v2.x, v1.y - v2.y); }
	inline Vector2& operator -=(Vector2& v1, const Vector2& v2) { v1.x -= v2.x; v1.y -= v2.y; return v1; }

	// ベクトルの拡大
	inline const Vector2 operator *(const Vector2& v, float scale) { return Vector2(v.x * scale, v.y * scale); }
	inline Vector2& operator *=(Vector2& v, float scale) { v.x *= scale; v.y *= scale; return v; }

	// ベクトルの縮小
	inline const Vector2 operator /(const Vector2 v, float scale) { return Vector2(v.x / scale, v.y / scale); }
	inline Vector2& operator /=(Vector2 v, float scale) { v.x /= scale; v.y /= scale; return v; }


	// VECTORと同じ用に使う関数
	inline float VSize(const Vector2& v) {
		return sqrtf(v.x * v.x + v.y * v.y);
	}

	inline float VSquareSize(const Vector2& v) {
		return v.x * v.x + v.y * v.y;
	}

	inline Vector2 VNorm(const Vector2& v) {
		float size = v.Size();
		if (size == 0.0f) {
			return Vector2(0, 0);
		}
		return Vector2(v.x / size, v.y / size);
	}

	inline float VDot(const Vector2& v1, const Vector2& v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}

	// ２次元整数ベクトルクラス
	struct Vector2I {
		int x, y;
		Vector2I() : x(0), y(0) {}
		Vector2I(int _x, int _y) : x(_x), y(_y) {}
		Vector2I(const Vector2I& v) : x(v.x), y(v.y) {}
		// Vector2 からの変換
		Vector2I(const Vector2& v) : x(static_cast<int>(v.x)), y(static_cast<int>(v.y)) {}

		// Vector2I から Vector2 への変換
		operator Vector2() const { return Vector2(static_cast<float>(x), static_cast<float>(y)); }

		// 加算
		Vector2I operator +(const Vector2I& v) const { return Vector2I(x + v.x, y + v.y); }
		Vector2I& operator +=(const Vector2I& v) { x += v.x; y += v.y; return *this; }

		// 減算
		Vector2I operator -(const Vector2I& v) const { return Vector2I(x - v.x, y - v.y); }
		Vector2I& operator -=(const Vector2I& v) { x -= v.x; y -= v.y; return *this; }

		// スカラー倍
		Vector2I operator *(int scale) const { return Vector2I(x * scale, y * scale); }
		Vector2I& operator *=(int scale) { x *= scale; y *= scale; return *this; }

		// スカラー除算
		Vector2I operator /(int scale) const { return Vector2I(x / scale, y / scale); }
		Vector2I& operator /=(int scale) { x /= scale; y /= scale; return *this; }

		// 等価比較
		bool operator ==(const Vector2I& v) const { return x == v.x && y == v.y; }
		bool operator !=(const Vector2I& v) const { return !(*this == v); }

		// ゼロベクトル
		static const Vector2I Zero() noexcept { return Vector2I(0, 0); }
		// 単位X
		static const Vector2I UnitX() noexcept { return Vector2I(1, 0); }
		// 単位Y
		static const Vector2I UnitY() noexcept { return Vector2I(0, 1); }
	};


	/// <summary>
	/// ワールド座標をスクリーン座標に変換
	/// </summary>
	/// <param name="world_pos">変換されるワールド座標(const)</param>
	/// <returns>{スクリーン座標x,y、カメラからの距離(0.0～1.0)}</returns>
	inline Vector3 WorldPosToScreenPos(const Vector3& world_pos)
	{
		Vector3 view_pos = world_pos * GetCameraViewMatrix();
		MATRIX proj_mat = GetCameraProjectionMatrix();
		Vector3 clip_pos = view_pos * proj_mat;
		float clip_pos_w = clip_pos.x * proj_mat.m[0][3] + clip_pos.y * proj_mat.m[1][3] + clip_pos.z * proj_mat.m[2][3] + proj_mat.m[3][3]; // w成分を追加

		//エラー処理
		if (clip_pos_w == 0.0f)
		{
			DebugLog("WorldPosToScreenPos::ゼロ徐算");
			return Vector3();
		}

		Vector3 ndc_pos = clip_pos / clip_pos_w;
		Vector3 result = Vector3(
			(ndc_pos.x + 1.0f) * 0.5f * Screen::WIDTH,
			(1.0f - ndc_pos.y) * 0.5f * Screen::HEIGHT,
			clip_pos.z / clip_pos_w);
		return result;
	}

	/* ==================== カラー系の定義 ==================== */

	// COLOR_Fとしても使えるカラークラス(0~1)
	class Color : public COLOR_F {
	public:
		Color() : Color(1, 1, 1, 1) {}
		Color(float r, float g, float b, float a = 1.0f) {
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}
		Color(const Color& c) {
			r = c.r;
			g = c.g;
			b = c.b;
			a = c.a;
		}
		Color(const COLOR_F& c) {
			r = c.r;
			g = c.g;
			b = c.b;
			a = c.a;
		}
		Color operator =(const COLOR_F& c) {
			r = c.r;
			g = c.g;
			b = c.b;
			a = c.a;
			return *this;
		}

		// 定数

		static const Color Black()	noexcept { return Color(0.f, 0.f, 0.f, 1.f); }
		static const Color White()	noexcept { return Color(1.f, 1.f, 1.f, 1.f); }
		static const Color Red()	noexcept { return Color(1.f, 0.f, 0.f, 1.f); }
		static const Color Green()	noexcept { return Color(0.f, 1.f, 0.f, 1.f); }
		static const Color Blue()	noexcept { return Color(0.f, 0.f, 1.f, 1.f); }
		static const Color Yellow() noexcept { return Color(1.f, 1.f, 0.f, 1.f); }
		static const Color Cyan()	noexcept { return Color(0.f, 1.f, 1.f, 1.f); }
		static const Color Magenta()noexcept { return Color(1.f, 0.f, 1.f, 1.f); }

		static const int Black16()	noexcept { return Color::Black().RGB16(); }
		static const int White16()	noexcept { return Color::White().RGB16(); }
		static const int Red16()	noexcept { return Color::Red().RGB16(); }
		static const int Green16()	noexcept { return Color::Green().RGB16(); }
		static const int Blue16()	noexcept { return Color::Blue().RGB16(); }
		static const int Yellow16()	noexcept { return Color::Yellow().RGB16(); }
		static const int Cyan16()	noexcept { return Color::Cyan().RGB16(); }
		static const int Magenta16()noexcept { return Color::Magenta().RGB16(); }

		// 16進数のRGB値を返す
		int RGB16() const {
			return GetColor(
				(int)(r * 255.0f),
				(int)(g * 255.0f),
				(int)(b * 255.0f));
		}

		// 0~255のアルファ値を返す
		int Alpha() const {
			return (int)(a * 255.0f);
		}
	};

	// カラーの加算
	inline const Color operator +(const Color& c1, const Color& c2) { return Color(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a + c2.a); }
	inline Color& operator +=(Color& c1, const Color& c2) { c1 = c1 + c2; return c1; }
	// カラーの減算
	inline const Color operator -(const Color& c1, const Color& c2) { return Color(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b, c1.a - c2.a);	}
	inline Color& operator -=(Color& c1, const Color& c2) { c1 = c1 - c2; return c1; }
	// カラーの拡大
	inline const Color operator *(const Color& c, float scale) { return Color(c.r * scale, c.g * scale, c.b * scale, c.a * scale); }
	inline Color& operator *=(Color& c, float scale) { c = c * scale; return c; }
	// カラーの縮小
	inline const Color operator /(const Color c, float scale) { return Color(c.r / scale, c.g / scale, c.b / scale, c.a / scale); }
	inline Color& operator /=(Color c, float scale) { c = c / scale; return c; }

	
	/* ==================== 画像関連 ==================== */

	inline Vector2I GetGraphSize(int image_handle_)
	{
		Vector2I size;
		GetGraphSize(image_handle_, &size.x, &size.y);
		return size;
	};
	
	
};
