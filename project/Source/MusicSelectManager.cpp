#include "MusicSelectManager.h"
#include "JsonTitleCollector.h"
#include "MusicTrack.h"

MusicSelectManager::MusicSelectManager()
{
	JsonTitleCollector collector;
	collector.LoadTitlesFromDirectory("data/_MusicSheets/");
	for (int i = 0; i <  collector.titles.size(); i++)
	{
		MusicTrack* track = new MusicTrack(collector.titles[i]);
		track->transform_.SetPosition(Vector3(i * 400.0f - 100.0f, 0.0f, 0.0f));
		music_tracks_.push_back(track);
	}
}

MusicSelectManager::~MusicSelectManager()
{
}

void MusicSelectManager::Update()
{
}

void MusicSelectManager::Draw()
{
}
