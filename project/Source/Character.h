#pragma once
#include "../Library/GameObject.h"

class Character : public GameObject
{
public:
	Character();
	virtual ~Character();

	void Update() override;
	void Draw() override;

protected:
	virtual void MoveUpdate();

	Vector3 position;

private:
	float radius;
};