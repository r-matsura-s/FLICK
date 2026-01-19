#pragma once
#include <string>
#include "../Library/json.hpp"

/// <summary>
/// 楽曲のメタデータ構造体
/// </summary>
class NotesMetaData
{
public:
	NotesMetaData()  {};
	~NotesMetaData() {};

	void SetToJsonObj(const nlohmann::json& json_obj);

	std::string title_;		// 曲のタイトル
	std::string sub_title_;	// サブタイトル等
	std::string music_;		// 音声ファイル名
	float bpm_ = 120.0f;	// 基本BPM
	float volume_ = 1.0f;	// 再生ボリューム
	float offset_ = 0.0f;	// 再生オフセット秒
	int beat_ = 4;			// 曲の拍子
	int level_ = 7;			// 難易度レベル
};