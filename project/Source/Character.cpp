#include "Character.h"
#include "InputManager.h"
#include "LaneManager.h"


Character::Character() : Object3D("Chara_Slime", Transform(Vector3(0.0f, 20.0f, -200.0f), Vector3(90.0f, 180.0f, 0.0f) * DegToRad, Vector3::One()))
{
	SetStaticMeshFlag(false);
	radius = transform_.position.y;
}

Character::~Character()
{
}

void Character::Update()
{
	MoveUpdate();
}

void Character::MoveUpdate()
{
	// ジョイパッドの左スティックの傾きで移動
	float stick_rate = InputManager::Instance()->GetCurrentState().GetMaxStickRateX();

#if (_DEBUG)
	static bool is_mouse_mode = false;
	if (InputManager::Instance()->GetKeyDown(KEY_INPUT_M))
	{
		is_mouse_mode = !is_mouse_mode;
	}

	static const int SCREEN_OFFSET = 0;
	if (is_mouse_mode)
	{
		Vector2I mouse;
		GetMousePoint(&mouse.x, &mouse.y);
		stick_rate = (float)(std::clamp(mouse.x, SCREEN_OFFSET, Screen::WIDTH - SCREEN_OFFSET)) / (float)(Screen::WIDTH);
		stick_rate -= 0.5f;
		stick_rate *= 2.0f;
	}
#endif

	transform_.position.x = LaneLeapHorizontalRate(stick_rate);
}
