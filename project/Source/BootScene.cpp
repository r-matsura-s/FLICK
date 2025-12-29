#include "BootScene.h"
#include "InputManager.h"
#include "SoundManager.h"

BootScene::BootScene()
{
	InputManager::Instance(); // InputManager‚Ì¶¬
	SoundManager::Instance(); // SoundManager‚Ì¶¬
}

BootScene::~BootScene()
{
}

void BootScene::Update()
{
	SceneManager::ChangeScene("TITLE"); // ‹N“®‚ªI‚í‚Á‚½‚çTitle‚ğ•\¦
}

void BootScene::Draw()
{
}
