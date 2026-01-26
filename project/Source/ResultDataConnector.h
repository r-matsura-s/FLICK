#pragma once
#include "../Library/GameObject.h"
#include "ResultData.h"

/// <summary>
/// リザルトに使うデータをシーンをまたいで保持するクラス。 GameObject継承
/// </summary>
class ResultDataConnector : public GameObject
{
public:
	ResultDataConnector();
	~ResultDataConnector();

	inline void SetResultData(const ResultData& data) { result_data_ = data; }
	inline const ResultData& GetResultData() { return result_data_; }

private:
	ResultData result_data_;
};