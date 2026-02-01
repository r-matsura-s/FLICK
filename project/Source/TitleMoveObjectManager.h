#pragma once
#include "MoveObject.h"
#include "ActionEvent.h"

/// <summary>
/// GameObjectåpè≥
/// </summary>
class TitleMoveObjectManager : public GameObject
{
public:
	TitleMoveObjectManager();
	~TitleMoveObjectManager();

	void Update() override;

private:
	ActionEvent<> on_create_action_;
	std::list<GameObject*> object_list_;
	float wait_timer_ = 0.0f;
	float erase_timer_ = 100.0f;

	void CreateObject();
};