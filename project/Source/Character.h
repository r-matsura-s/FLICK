#pragma once
#include "Object3D.h"

class Character : public Object3D
{
public:
	Character();
	virtual ~Character();

	void Update() override;

protected:
	virtual void MoveUpdate();

private:
	float radius;
};