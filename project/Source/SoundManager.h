#pragma once
#include "../Library/GameObject.h"
#include <unordered_map>


/// <summary>
/// サウンドの種類
/// </summary>
enum class SoundType
{
	BGM = 0,
	SE,
	VOICE,
	MUSIC,
};

/// <summary>
/// サウンド再生タイプ
/// </summary>
enum class SoundPlayType
{
	ONCE = 0,	// 一回だけ再生
	LOOP,		// ループ再生

};

/// <summary>
/// サウンドの再生、リソースの管理を行う
/// </summary>
class SoundManager : public GameObject
{
public:
	static SoundManager* Instance();
	~SoundManager();

	void Update() override;

	/// <summary>
	/// 指定された名前、拡張子、種類のサウンドを読み込みます。
	/// </summary>
	/// <param name="name">読み込むサウンドの名前。</param>
	/// <param name="ext">サウンドファイルの拡張子。".wav"等</param>
	/// <param name="type">サウンドの種類を示す SoundType 列挙型。</param>
	/// <param name="mem_size">同じハンドラで同時に再生できる数</param>
	void LoadSound(const std::string& name, const std::string& ext, SoundType type, int mem_size = 3);

	/// <summary>
	/// すべてのサウンドを削除します。
	/// </summary>
	void DeleteSoundAll();

	/// <summary>
	/// 音量を適応
	/// </summary>
	void ChangeVolumeAll();

	inline void SetMasterVolume(float rate) { master_volume_ = rate; OnChangeVolume(); }
	inline void SetBGMVolume(float rate)	{ bgm_volume_ = rate; OnChangeVolume(); }
	inline void SetSEVolume(float rate)		{ se_volume_ = rate; OnChangeVolume(); }
	inline void SetVoiceVolume(float rate)	{ voice_volume_ = rate; OnChangeVolume(); }

	/// <summary>
	/// 再生位置を秒単位で設定する
	/// </summary>
	/// <param name="handle">設定するサウンドのハンドル。</param>
	/// <param name="sec">開始する秒数</param>
	void SetSoundCurrentTime(int handle, float sec);

	/// <summary>
	/// 指定された名前と拡張子のBGMを再生します。
	/// </summary>
	/// <param name="name">再生するBGMのファイル名</param>
	/// <param name="ext">ファイルの拡張子".wav"など。</param>
	void PlayBGM(const std::string& name, std::string ext = ".wav");
	/// <summary>
	/// 読み込み済みのBGMを再生
	/// </summary>
	void PlayBGM(SoundPlayType play_type = SoundPlayType::LOOP);
	/// <summary>
	/// 指定された名前の効果音を再生します。
	/// </summary>
	/// <param name="name">再生する効果音の名前。</param>
	void PlaySE(const std::string& name);
	/// <summary>
	/// 指定された名前の音声を再生します。
	/// </summary>
	/// <param name="name">再生する音声の名前。</param>
	void PlayVoice(const std::string& name);

	/// <summary>
	/// 指定したハンドルのサウンドを再生します。
	/// </summary>
	/// <param name="handle">再生するサウンドのハンドル。</param>
	/// <param name="play_type">サウンドの再生方法を指定する SoundPlayType 列挙型。</param>
	void Play(int handle, SoundPlayType play_type);

	/// <summary>
	/// BGMのハンドルを取得します。
	/// </summary>
	/// <returns>BGMのハンドルを返します。</returns>
	inline int GetHandleBGM() const { return bgm_handle_; }
	/// <summary>
	/// 指定された名前の効果音ハンドルを取得します。
	/// </summary>
	/// <param name="name">取得する効果音の名前。</param>
	/// <returns>効果音のハンドル。</returns>
	int GetHandleSE(const std::string& name) const;
	/// <summary>
	/// 指定された名前の音声ハンドルを取得します。
	/// </summary>
	/// <param name="name">音声ハンドルを取得するための名前。</param>
	/// <returns>対応する音声ハンドル。</returns>
	int GetHandleVoice(const std::string& name) const;

	inline float GetMasterVolumeRate() const	{ return master_volume_; }
	inline float GetBGMVolumeRate() const		{ return bgm_volume_ * master_volume_; }
	inline float GetSEVolumeRate() const		{ return se_volume_ * master_volume_; }
	inline float GetVoiceVolumeRate() const		{ return voice_volume_ * master_volume_; }

	inline int GetBGMVolume() const		{ return static_cast<int>(GetBGMVolumeRate() * 255.0f); }
	inline int GetSEVolume() const		{ return static_cast<int>(GetSEVolumeRate() * 255.0f); }
	inline int GetVoiceVolume() const	{ return static_cast<int>(GetVoiceVolumeRate() * 255.0f); }


private:
	SoundManager();
	inline void OnChangeVolume() { need_update_volume_ = true; }

	int bgm_handle_ = -1;	// 現在再生中のBGMハンドル
	std::unordered_map<std::string, int> se_handles_;		// SEハンドルリスト
	std::unordered_map<std::string, int> voice_handles_;	// ボイスハンドルリスト

	float master_volume_ = 1.0f;		// マスターボリューム(0.0~1.0)
	float bgm_volume_ = 1.0f;			// BGMの音量(0.0~1.0)
	float se_volume_ = 1.0f;			// SEの音量(0.0~1.0)
	float voice_volume_ = 1.0f;			// ボイスの音量(0.0~1.0)
	bool need_update_volume_ = false;	// 音量更新フラグ
};