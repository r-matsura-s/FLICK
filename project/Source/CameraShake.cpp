#include "CameraShake.h"

namespace
{
	// ひねり回転の最大角度(deg)
	static constexpr float TWIST_MAX = 3.0f;
	// ひねりの補間時間(秒)
	static constexpr float TWIST_TIME = 1.0f;
	// ひねりの減衰(*=)
	static constexpr float TWIST_FACTOR = 0.86f;
}

CameraShake::CameraShake()
{
}

CameraShake::~CameraShake()
{
}

void CameraShake::Update()
{
	// ひねり減衰のアップデート
	if (twist_rate_ > 0.0f)
	{
		twist_angle_ = (twist_angle_ - move_.rotation.z) * (twist_rate_ * twist_rate_ * twist_rate_) + move_.rotation.z;
		move_.rotation.z *= TWIST_FACTOR;
		twist_rate_ -= Time::DeltaTime() * TWIST_TIME;
		if (twist_rate_ < 0.0f)
		{
			twist_rate_ = 0.0f;
		}
	}
}

void CameraShake::Draw()
{
	Vector3 pos = base_transform_.position + move_.position;
	Vector3 rot = base_transform_.rotation + move_.rotation;
	SetCameraPositionAndAngle(pos, rot.x, rot.y, rot.z);
}

void CameraShake::StartRotate(float power)
{
	twist_rate_ = 1.0f;
	move_.rotation = Vector3(0, 0, power * DegToRad);
}
