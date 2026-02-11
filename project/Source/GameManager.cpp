#include "GameManager.h"
#include "LaneManager.h"
#include "NotesManager.h"
#include "Character.h"
#include "ActionGameObject.h"
#include "CameraShake.h"


namespace
{
	static const Vector3 CAMERA_DEFAULT_POSITION = Vector3(0.0f, 550.0f, -550.0f);
	static const float CAMERA_DEFAULT_ANGLE = 35.0f;

	static const float BACKGROUND_ROTATE_SPEED = 0.00015f * DegToRad;
}

GameManager::GameManager()
{
	SetBackgroundColor(127, 157, 200);
	SetCameraPositionAndAngle(CAMERA_DEFAULT_POSITION, CAMERA_DEFAULT_ANGLE * DegToRad, 0.0f, 0.0f);

	back_ground_ = new Object3D("Sky_Daylight01", Transform(Vector3(), Vector3(), Vector3::One() * 3));
	back_ground_->SetStaticMeshFlag(false);

	LaneManager* lane_manager = new LaneManager();
	new NotesManager();
	new Character();

	CameraShake* camera = new CameraShake();
	camera->base_transform_.SetPosition(CAMERA_DEFAULT_POSITION);
	camera->base_transform_.SetRotation(Vector3(CAMERA_DEFAULT_ANGLE * DegToRad, 0.0f, 0.0f));
	camera->Draw();

	ActionGameObject* side_wall = new ActionGameObject();
	side_wall->SetDrawOrder(-2000);
	side_wall->OnDraw += [lane_manager] { lane_manager->DrawSideWall(); };
}

GameManager::~GameManager()
{
	SetBackgroundColor(0, 0, 0);
}

void GameManager::Update()
{
	back_ground_->transform_.rotation += Vector3(0, BACKGROUND_ROTATE_SPEED, 0) / Time::DeltaTime();
}

void GameManager::Draw()
{
}
