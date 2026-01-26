#include "NotesMetaData.h"

using namespace nlohmann;
namespace
{
	//const int LEVEL_EXPERT = 8;	// 難易度エキスパートの最低レベル
	//const int LEVEL_HARD = 5;	// 難易度ハードの最低レベル
}

// -------------------------------------------------------------
// NotesMetaData
// -------------------------------------------------------------

void NotesMetaData::SetToJsonObj(const nlohmann::json& json_obj)
{
	title_		= json_obj["title"].get<std::string>();
	sub_title_	= json_obj["subTitle"].get<std::string>();
	class_name_ = json_obj["className"].get<std::string>();
	music_		= json_obj["music"].get<std::string>();
	bpm_		= json_obj["bpm"].get<float>();
	volume_		= json_obj["volume"].get<float>();
	offset_		= json_obj["offset"].get<float>();
	beat_		= json_obj["beat"].get<int>();
	level_		= json_obj["level"].get<int>();
}

int NotesMetaData::GetLevelToIndex() const
{
	if (class_name_ == "EXPERT")
	{
		return 2;	// エキスパート
	}
	else if (class_name_ == "HARD")
	{
		return 1;	// ハード
	}
	return 0;		// イージー
}

std::string NotesMetaData::GetJsonFileName() const
{
	// "MarbleBlue_EXPERT.json"の形式
	return music_ + "_" + class_name_ + ".json";
}

// -------------------------------------------------------------
// NotesMetaDataContext
// -------------------------------------------------------------

NotesMetaDataContext::NotesMetaDataContext() : GameObject()
{
	SetTag("NotesMeta");
	DontDestroyOnSceneChange();
}

NotesMetaDataContext::~NotesMetaDataContext()
{
}
