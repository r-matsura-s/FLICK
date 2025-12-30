#include "NotesMetaData.h"

using namespace nlohmann;

void NotesMetaData::SetToJsonObj(const nlohmann::json& json_obj)
{
	title_ = json_obj["title"].get<std::string>();
	sub_title_ = json_obj["subTitle"].get<std::string>();
	music_ = json_obj["music"].get<std::string>();
	bpm_ = json_obj["bpm"].get<float>();
	volume_ = json_obj["volume"].get<float>();
	offset_ = json_obj["offset"].get<float>();
	beat_ = json_obj["beat"].get<int>();
}
