#include "GameManager.h"
#include "LaneManager.h"
#include "NotesManager.h"
#include "Character.h"
#include "Object3D.h"


namespace
{
	static const Vector3 CAMERA_DEFAULT_POSITION = Vector3(0.0f, 550.0f, -550.0f);
	static const float CAMERA_DEFAULT_ANGLE = 35.0f;
}

GameManager::GameManager()
{
	SetBackgroundColor(127, 157, 200);
	SetCameraPositionAndAngle(CAMERA_DEFAULT_POSITION, CAMERA_DEFAULT_ANGLE * DegToRad, 0.0f, 0.0f);

	new Object3D("Sky_Daylight01", Transform(Vector3(), Vector3(), Vector3::One() * 3));

	new LaneManager();
	new NotesManager();
	new Character();
}

GameManager::~GameManager()
{
	SetBackgroundColor(0, 0, 0);
}

void GameManager::Update()
{
}

void GameManager::Draw()
{
}
