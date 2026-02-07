#pragma once
#include "../Library/GameObject.h"
#include "../Library/Transform.h"

class CameraShake : public GameObject
{
public:
	CameraShake();
	~CameraShake();

	void Update() override;
	void Draw() override;

	void StartRotate(float power);

	Transform base_transform_;

private:
	Transform move_;
	float twist_angle_ = 1.0f;
	float twist_rate_ = 0.0f;
};