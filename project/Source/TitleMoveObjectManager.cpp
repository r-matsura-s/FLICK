#include "TitleMoveObjectManager.h"

namespace
{
	// ìØéûÇ…âΩå¬Ç‹Ç≈ê∂ê¨Ç∑ÇÈÇ©
	const int CREATE_MULT_COUNT = 1;
	const float CREATE_TIME = 0.5f;
	const float ERASE_TIME = 5.0f;

	// transformóp
	const Vector3 CREATE_OFFSET = Vector3(0.0f, 100.0f, 1000.0f);
	const float OBJECT_SCALE = 1.0f;

	// move_speedóp
	const Vector3 VELOCITY = Vector3(0.0f, 120.0f, -640.0f);
}

float get_randf();
float get_randf() 
{ 
	int r = GetRand(RAND_MAX) - GetRand(RAND_MAX);
	return sinf(static_cast<float>(r));
}

TitleMoveObjectManager::TitleMoveObjectManager()
{
	erase_timer_ = ERASE_TIME;

	on_create_action_ += [this] { this->wait_timer_ = CREATE_TIME; };
	on_create_action_ += [this] { this->CreateObject(); };
}

TitleMoveObjectManager::~TitleMoveObjectManager()
{
	object_list_.clear();
}

void TitleMoveObjectManager::Update()
{
	// push
	wait_timer_ -= Time::DeltaTime();
	if (wait_timer_ <= 0.0f)
	{
		for (int i = 0; i <= GetRand(CREATE_MULT_COUNT - 1); i++)
		{
			on_create_action_.Invoke();
		}
	}

	// pop
	erase_timer_ -= Time::DeltaTime();
	if (erase_timer_ <= 0.0f && object_list_.size() > 0)
	{
		erase_timer_ = ERASE_TIME;
		(*object_list_.begin())->DestroyMe();
		object_list_.pop_front();
	}
}

void TitleMoveObjectManager::CreateObject()
{
	MoveObject* obj = new MoveObject("Cube");

	float rot = get_randf() * DX_TWO_PI_F;
	MATRIX rot_matrix = MGetRotZ(rot);

	obj->transform_ = Transform(
		Vector3(0.0f, CREATE_OFFSET.y, 0.0f) * rot_matrix + Vector3(0, 0, CREATE_OFFSET.z),
		Vector3(get_randf(), get_randf(), get_randf()) * DX_TWO_PI_F,
		Vector3::One() * OBJECT_SCALE
	);
	obj->move_speed_ = Transform(
		VELOCITY * rot_matrix, 
		Vector3(get_randf(), get_randf(), get_randf()) * DX_TWO_PI_F,
		Vector3::Zero()
	);

	object_list_.push_back(obj);
}
