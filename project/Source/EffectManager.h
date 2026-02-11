#pragma once
#include "../Library/Transform.h"
#include <string>

/// <summary>
/// エフェクシアをまとめた、エフェクト管理クラス
/// </summary>
namespace EffectManager 
{
    void Init();    // 初期化
    void Release(); // 終了処理
    void Update();  // 更新
    void Draw();    // 描画

    /// <summary>
    /// リソースのロード
    /// </summary>
    /// <param name="name">.efkefcのファイル名</param>
    /// <param name="scale">拡大率</param>
    void LoadResource(const std::string& name, float scale = 1.0f);

    /// <summary>
    /// エフェクトの再生
    /// </summary>
    /// <param name="name">ロード時の名前</param>
    /// <param name="_transform">再生する位置</param>
    /// <returns>一応、Playingハンドルを返す(!= -1)</returns>
    int Play(const std::string& name, const Transform& _transform);
};