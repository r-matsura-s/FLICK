#pragma once
#include "../Library/myDxLib.h"
#include <string>
#include <cmath>

/// <summary>
/// 3Dの位置、回転、拡大率などを管理するクラス
/// </summary>
class Transform
{
public:
	/* ========== 変数の宣言 ========== */

	Vector3 position;	// ローカル座標
	Vector3 rotation;	// ローカル回転（ラジアン）
	Vector3 scale;		// ローカル拡大率

	std::string tag;	// 管理用タグ

	Transform* parent;	// 親のポインタ

	bool is_active;		// 有効フラグ


	/* ========== コンストラクタ定義 ========== */

	Transform() 
		: Transform(Vector3(), Vector3(), Vector3::One(), nullptr) {
	}
	Transform(const Vector3& pos, Transform* _parent = nullptr) 
		: Transform(pos, Vector3(), Vector3::One(), _parent) {
	}
	Transform(const Vector3& pos, const Vector3& rot, Transform* _parent = nullptr)
		: Transform(pos, rot, Vector3::One(), _parent) {
	}
	Transform(const Vector3& pos, const Vector3& rot, const Vector3& scl, Transform* _parent = nullptr) {
		position = pos;
		rotation = rot;
		scale = scl;
		parent = _parent;
		is_active = true;
	}

	~Transform() {}

	/* ========== セッター定義 ========== */

	void SetParent(Transform* _parent) {
		parent = _parent;
	}
	void SetPosition(const Vector3& pos) {
		position = pos;
	}
	void SetRotation(const Vector3& rot) {
		rotation = rot;
	}
	void SetScale(const Vector3& scl) {
		scale = scl;
	}
	void SetActive(bool active) {
		is_active = active;
	}

	/* ========== 向きベクトルの定義 ========== */

	Vector3 Up() const {
		return Vector3(0, 1, 0) * MGetRotationZXY(rotation);
	}
	Vector3 Down() const {
		return Vector3(0, -1, 0) * MGetRotationZXY(rotation);
	}
	Vector3 Right() const {
		return Vector3(1, 0, 0) * MGetRotationZXY(rotation);
	}
	Vector3 Left() const {
		return Vector3(-1, 0, 0) * MGetRotationZXY(rotation);
	}
	Vector3 Forward() const {
		return Vector3(0, 0, 1) * MGetRotationZXY(rotation);
	}
	Vector3 Back() const {
		return Vector3(0, 0, -1) * MGetRotationZXY(rotation);
	}

	/// <summary>
	/// ターゲット座標を向くように回転を設定します。
	/// </summary>
	/// <param name="target">注視する対象の座標</param>
	void LookAt(const Vector3& target) {
		// 注視行列を作成
		MATRIX look_mat = MGetIdent();
		Vector3 up = Vector3::UnitY();
		CreateLookAtMatrix(&look_mat, &position, &target, &up);
		
		// 行列からオイラー角（ラジアン）を抽出
		rotation.x = std::asin(-look_mat.m[2][1]); // pitch
		rotation.y = std::atan2(look_mat.m[2][0], look_mat.m[2][2]); // yaw
		rotation.z = std::atan2(look_mat.m[0][1], look_mat.m[1][1]); // roll（不要なら省略可）
	}


	MATRIX GetMatrix() const {
		return MGetScale(scale) * MGetRotationZXY(rotation) * MGetTranslate(position);
	}
};