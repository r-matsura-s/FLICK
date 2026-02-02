#pragma once
#include "Object3D.h"

class MoveObject : public Object3D
{
public:
	MoveObject(const std::string& mesh_name);
	~MoveObject();

	void Update() override;

	Transform move_speed_;
};