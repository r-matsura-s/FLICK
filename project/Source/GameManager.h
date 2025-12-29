#pragma once
#include "../Library/GameObject.h"

class GameManager : public GameObject
{
public:
	GameManager();
	~GameManager();

	void Update() override;
	void Draw() override;
};