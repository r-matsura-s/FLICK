#include "SceneFactory.h"
#include <windows.h>
#include <assert.h>
#include "../Library/myDxLib.h"

#include "BootScene.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "ResultScene.h"

SceneBase* SceneFactory::CreateFirst()
{
#if (_DEBUG)
	OutputDebugStringA("---------- First scene is started ----------\n");
#endif
	return new BootScene();
}

SceneBase * SceneFactory::Create(const std::string & name)
{
#if (_DEBUG)
	DebugLog(name + "シーン生成");
#endif

	if (name == "TITLE")
	{
		return new TitleScene();
	}
	if (name == "PLAY")
	{
		return new PlayScene();
	}
	if (name == "RESULT")
	{
		return new ResultScene();
	}

	// エラーメッセージを表示して強制終了
	MessageBox(NULL, ("次のシーンはありません\n" + name).c_str(), "SceneFactory", MB_ICONERROR | MB_OK);
	assert(false);
	return nullptr;
}
