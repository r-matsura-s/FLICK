#pragma once
#include "../Library/GameObject.h"
#include "../Library/Transform.h"

/// <summary>
/// 3Dオブジェクトを表し、メッシュの描画や更新を行います。
/// </summary>
class Object3D : public GameObject
{
public:
	Object3D();
	/// <summary>
	/// staticメッシュ用コンストラクタ
	/// </summary>
	/// <param name="mesh_name">メッシュの名前</param>
	/// <param name="self">objectのTrasnform</param>
	Object3D(const std::string& mesh_name, const Transform& self);
	~Object3D();

	/// <summary>
	/// Updateの最初に一度だけ呼ばれます
	/// </summary>
	virtual void Start();
	/// <summary>
	/// 毎フレーム呼ばれる更新
	/// </summary>
	virtual void Update() override;
	/// <summary>
	/// 毎フレーム呼ばれる描画
	/// </summary>
	virtual void Draw() override;

protected:
	Transform transform_;// 位置、回転、拡大率など

private:
	int mesh_handle_ = -1;		// メッシュハンドル
	bool static_mesh_ = false;	// staticメッシュならtrue
	bool need_start_ = true;	// Start()が呼ばれていないならtrue
};