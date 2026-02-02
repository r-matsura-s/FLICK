#include "MoveObject.h"

MoveObject::MoveObject(const std::string& mesh_name) : Object3D(mesh_name, Transform())
{
	SetStaticMeshFlag(false);
}

MoveObject::~MoveObject()
{
}

void MoveObject::Update()
{
	Object3D::Update();

	transform_.position += move_speed_.position * Time::DeltaTime();
	transform_.rotation += move_speed_.rotation * Time::DeltaTime();
	transform_.scale += move_speed_.scale * Time::DeltaTime();
}
