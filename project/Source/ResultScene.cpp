#include "ResultScene.h"
#include "ResultBoardManager.h"
#include "ActionGameObject.h"
#include "SoundManager.h"
#include "InputManager.h"

namespace
{
	const float WAIT_TIME = 2.0f;
}

ResultScene::ResultScene()
{
	SoundManager* Sound = SoundManager::Instance();
	Sound->LoadSound("result_start", ".wav", SoundType::BGM);
	Sound->PlayBGM(SoundPlayType::ONCE);
	
	back_image_ = LoadGraph("data/texture/result_bg.png");

	ActionGameObject* back_ground = new ActionGameObject();
	back_ground->OnDraw += [this] { this->DrawBackImage(); };

	new ResultBoardManager();
}

ResultScene::~ResultScene()
{
	DeleteGraph(back_image_);
}

void ResultScene::Update()
{
	scene_time_ += Time::DeltaTime();

	if (scene_time_ > WAIT_TIME)
	{
		InputManager& Input = *InputManager::Instance();
		if (Input.GetButtonDown(XINPUT_BUTTON_A) || Input.GetButtonDown(XINPUT_BUTTON_B)|| CheckHitKey(KEY_INPUT_SPACE))
		{
			SceneManager::ChangeScene("TITLE");
		}
	}
}

void ResultScene::Draw()
{
	DrawString(0, 0, "RESULT SCENE", GetColor(255, 255, 255));
}

void ResultScene::DrawBackImage() const
{
	DrawGraph(0, 0, back_image_, TRUE);
}
