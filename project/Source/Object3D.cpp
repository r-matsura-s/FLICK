#include "Object3D.h"

Object3D::Object3D() : GameObject()
{
}

Object3D::Object3D(const std::string& mesh_name, const Transform& self) : GameObject()
{
	transform_ = self;
	static_mesh_ = true;

	mesh_handle_ = MV1LoadModel(("data/model/" + mesh_name + ".mv1").c_str());
#if (_DEBUG)
	if(mesh_handle_ < 0)
	{
		DebugLogError("Error: Failed to load model: " + mesh_name);
	}
#endif
	MV1SetMatrix(mesh_handle_, transform_.GetMatrix());
}

Object3D::~Object3D()
{
	if(mesh_handle_ > 0) 
	{
		MV1DeleteModel(mesh_handle_);
		mesh_handle_ = -1;
	}
}

void Object3D::Start()
{
}

void Object3D::Update()
{
	if(need_start_)
	{
		Start();
		need_start_ = false;
	}
}

void Object3D::Draw()
{
	// staticメッシュなら計算不要
	if (!static_mesh_)
	{
		MV1SetMatrix(mesh_handle_, transform_.GetMatrix());
	}

	// 描画
	MV1DrawModel(mesh_handle_);
}
