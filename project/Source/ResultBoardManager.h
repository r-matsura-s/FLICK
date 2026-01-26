#pragma once
#include "../Library/GameObject.h"
#include "ResultData.h"

/// <summary>
/// プレイの結果を表示するクラス。　GameObject継承
/// </summary>
class ResultBoardManager : public GameObject
{
public:
	ResultBoardManager();
	~ResultBoardManager();

	void Update() override;
	void Draw() override;

	void UpdateViewResultData();

private:
	ResultData play_result_data_;
	ResultData view_result_data_;
};