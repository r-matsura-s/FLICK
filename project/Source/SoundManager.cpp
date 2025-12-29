#include "SoundManager.h"

namespace
{
	static const char* TO_PATH = "data/sound/";
	
	// SoundPlayTypeからDxLibに変換用マップ
	static const int PLAY_TYPE_MAP[] = { DX_PLAYTYPE_BACK, DX_PLAYTYPE_LOOP };
	
	// BGMのバッファサイズ
	static const int BGM_BUFFER_SIZE = 2; 
}

#pragma region __region__
SoundManager* SoundManager::Instance()
{
	static SoundManager* instance = nullptr;
	if (instance == nullptr) {
		instance = new SoundManager();
	}

	return instance;
}
#pragma endregion //シングルトン関数定義

SoundManager::SoundManager()
{
	DontDestroyOnSceneChange();
}

SoundManager::~SoundManager()
{
	// サウンドハンドルの解放
	DeleteSoundAll();
}

void SoundManager::Update()
{
	ChangeVolumeAll();
}

void SoundManager::LoadSound(const std::string& name, const std::string& ext, SoundType type, int mem_size)
{
	std::string path = std::string(TO_PATH);
	int handle = -1;

	// 種類ごとにパスを分けてロード
	switch (type) 
	{
	case SoundType::SE:
		path = path + "se/" + name + ext;
		if (se_handles_.count(name) == 0) 
		{
			handle = DxLib::LoadSoundMem(path.c_str(), mem_size);
			se_handles_[name] = handle;
		}
		else
		{
			handle = se_handles_[name];
		}
		break;

	case SoundType::VOICE:
		path = path + "voice/" + name + ext;
		if (voice_handles_.count(name) == 0) 
		{
			handle = DxLib::LoadSoundMem(path.c_str(), mem_size);
			voice_handles_[name] = handle;
		}
		else
		{
			handle = voice_handles_[name];
		}
		break;

	case SoundType::BGM:
		path = path + "bgm/" + name + ext;
		if (bgm_handle_ != -1) 
		{
			DxLib::DeleteSoundMem(bgm_handle_);
			bgm_handle_ = -1;
		}
		handle = DxLib::LoadSoundMem(path.c_str(), BGM_BUFFER_SIZE);
		bgm_handle_ = handle;
		break;

	case SoundType::MUSIC:
		path = "data/_MusicSheets/";
		path = path + name + ext;
		if (bgm_handle_ != -1)
		{
			DxLib::DeleteSoundMem(bgm_handle_);
			bgm_handle_ = -1;
		}
		handle = DxLib::LoadSoundMem(path.c_str(), BGM_BUFFER_SIZE);//BGMに上書きする
		bgm_handle_ = handle; 
		break;
	}

	// エラーハンドル
	if (handle < 0) 
	{
		DebugLogError("SoundManager::LoadSound 次のサウンドパスが見つかりません\n [ " + path + " ]");
	}
}

void SoundManager::DeleteSoundAll()
{
	// SE
	for (auto& pair : se_handles_) 
	{
		if (pair.second != -1) 
		{
			DxLib::DeleteSoundMem(pair.second);
			pair.second = -1;
		}
	}
	se_handles_.clear();

	// ボイス
	for (auto& pair : voice_handles_) 
	{
		if (pair.second != -1) 
		{
			DxLib::DeleteSoundMem(pair.second);
			pair.second = -1;
		}
	}
	voice_handles_.clear();

	// BGM
	if (bgm_handle_ != -1) 
	{
		DxLib::DeleteSoundMem(bgm_handle_);
		bgm_handle_ = -1;
	}
}

void SoundManager::ChangeVolumeAll()
{
	// 音量変更が必要ない場合は処理しない
	if (!need_update_volume_) return;

	// SE
	for (auto& pair : se_handles_)
	{
		if (pair.second != -1)
		{
			DxLib::ChangeVolumeSoundMem(GetSEVolume(), pair.second);
		}
	}

	// ボイス
	for (auto& pair : voice_handles_)
	{
		if (pair.second != -1)
		{
			DxLib::ChangeVolumeSoundMem(GetVoiceVolume(), pair.second);
		}
	}

	// BGM
	if (bgm_handle_ != -1)
	{
		DxLib::ChangeVolumeSoundMem(GetBGMVolume(), bgm_handle_);
	}
}

void SoundManager::SetSoundCurrentTime(int handle, float sec)
{
	DxLib::SetSoundCurrentTime(static_cast<LONGLONG>(sec / 1000.0f), handle);
}

void SoundManager::PlayBGM(const std::string& name, std::string ext)
{
	// 新しいBGMをロードして再生
	LoadSound(name, ext, SoundType::BGM);
	PlayBGM();
}

void SoundManager::PlayBGM(SoundPlayType play_type)
{
	// 既にロード済みのBGMを再生
	if (bgm_handle_ != -1)
	{
		Play(bgm_handle_, play_type);
	}
}

void SoundManager::PlaySE(const std::string& name)
{
	// ロード済みのSEを再生
	auto it = se_handles_.find(name);
	if (it != se_handles_.end() && it->second != -1) 
	{
		Play(it->second, SoundPlayType::ONCE);
	}
}

void SoundManager::PlayVoice(const std::string& name)
{
	// ロード済みのボイスを再生
	auto it = voice_handles_.find(name);
	if (it != voice_handles_.end() && it->second != -1) 
	{
		Play(it->second, SoundPlayType::ONCE);
	}
}

void SoundManager::Play(int handle, SoundPlayType play_type)
{
	int type = static_cast<int>(play_type);

	DxLib::PlaySoundMem(handle, PLAY_TYPE_MAP[type]);
}

int SoundManager::GetHandleSE(const std::string& name) const
{
	auto it = se_handles_.find(name);
	if (it != se_handles_.end())
	{
		return it->second;
	}
	return -1;
}

int SoundManager::GetHandleVoice(const std::string& name) const
{
	auto it = voice_handles_.find(name);
	if (it != voice_handles_.end())
	{
		return it->second;
	}
	return -1;
}
