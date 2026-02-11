#include "EffectManager.h"
#include <EffekseerForDXLib.h>
#include <unordered_map>
#include <list>

namespace
{
    // Iteratorのリークを防ぐため、構造体にまとめる
    struct Handlers
    {
        // リソース管理：エフェクト名 -> リソースハンドル
        std::unordered_map<std::string, int> resources;

        // 再生中ハンドルの管理（自動削除用）
        std::list<int> playingHandles;
    };

    Handlers* self_ = nullptr;
}

void EffectManager::Init() 
{
    // Effekseerの初期化（最大表示数は適宜調整してください）
    if (Effekseer_Init(2000) == -1) 
    {
        printfDx("Effekseerの初期化に失敗しました。\n");
    }

    // DxLibの描画設定と同期
    Effekseer_InitDistortion();
    //SetUseZBuffer3D(TRUE);
    //SetWriteZBuffer3D(TRUE);

    // managerインスタンス生成
    self_ = new Handlers();
}

void EffectManager::Release() 
{
    // 全リソースの削除
    for (auto& res : self_->resources)
    {
        DeleteEffekseerEffect(res.second);
    }
    self_->resources.clear();
    self_->playingHandles.clear();
    delete self_;

    // Effekseerの終了処理
    Effkseer_End();
}

void EffectManager::Update() 
{
    // Effekseerの更新
    //UpdateEffekseer2D(); // 2Dで使用する場合
    UpdateEffekseer3D();

    // 再生が終わったハンドルをリストから除外
    self_->playingHandles.erase(
        std::remove_if(self_->playingHandles.begin(), self_->playingHandles.end(),
            [](int handle) { return IsEffekseer3DEffectPlaying(handle) != 0; }),
        self_->playingHandles.end()
    );
}

void EffectManager::Draw() 
{
    DrawEffekseer3D();
}

void EffectManager::LoadResource(const std::string& name, float scale) 
{
    // 既に読み込まれている場合はスキップ
    if (self_->resources.find(name) != self_->resources.end()) return;

    // ファイルパスを生成（例：Resources/Effect/name.efkefc）
    std::string path = "data/effect/" + name + ".efkefc";
    int resourceHandle = LoadEffekseerEffect(path.c_str(), scale);
    if (resourceHandle != -1) 
    {
        self_->resources[name] = resourceHandle;
    }
}

int EffectManager::Play(const std::string& name, const Transform& _transform) 
{
    if (self_->resources.find(name) == self_->resources.end()) return -1;

    int playHandle = PlayEffekseer3DEffect(self_->resources[name]);
    if (playHandle != -1) 
    {
        // Transformの適用
        SetPosPlayingEffekseer3DEffect(playHandle, _transform.position.x, _transform.position.y, _transform.position.z);
        SetRotationPlayingEffekseer3DEffect(playHandle, _transform.rotation.x, _transform.rotation.y, _transform.rotation.z);
        SetScalePlayingEffekseer3DEffect(playHandle, _transform.scale.x, _transform.scale.y, _transform.scale.z);

        self_->playingHandles.push_back(playHandle);
    }
    return playHandle;
}

