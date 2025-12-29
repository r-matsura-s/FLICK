#include "Character.h"
#include "InputManager.h"
#include "LaneManager.h"

Character::Character()
{
	position = Vector3(0.0f, 0.0f, -200.0f);
	radius = 20.0f;
}

Character::~Character()
{
}

void Character::Update()
{
	MoveUpdate();
}

void Character::Draw()
{
	DrawSphere3D(position + Vector3(0, radius, 0), radius, 2, Color::White16(), Color::White16(), TRUE);
}

void Character::MoveUpdate()
{
	// ジョイパッドの左スティックの傾きで移動
	float stick_rate = InputManager::Instance()->GetCurrentState().GetMaxStickRateX();

	position.x = LaneLeapHorizontalRate(stick_rate);
}
