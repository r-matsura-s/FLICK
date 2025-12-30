#pragma once
#include "../Library/GameObject.h"

class Object3D;

class GameManager : public GameObject
{
public:
	GameManager();
	~GameManager();

	void Update() override;
	void Draw() override;

private:
	Object3D* back_ground_;
};