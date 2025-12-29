#pragma once
#include <array>
#include <vector>
#include "../Library/GameObject.h"


/// <summary>
/// キーボードとゲームパッドの入力状態を保持する構造体
/// </summary>
class InputFrameState
{
public:
    std::array<char, 256> keys_;
    XINPUT_STATE pad_state_;

    InputFrameState()
    {
        keys_.fill(0);
        pad_state_ = XINPUT_STATE();
    }
    InputFrameState(const std::array<char, 256>& k, const XINPUT_STATE& p)
		: keys_(k), pad_state_(p) 
    {
	}

    // スティック
    inline Vector2 GetLeftStick()      const { return Vector2(pad_state_.ThumbLX, pad_state_.ThumbLY); }
    inline Vector2 GetLeftStickRate()  const { return Vector2(pad_state_.ThumbLX / 32767.0f, pad_state_.ThumbLY / 32767.0f); }
    inline Vector2 GetRightStick()     const { return Vector2(pad_state_.ThumbRX, pad_state_.ThumbRY); }
    inline Vector2 GetRightStickRate() const { return Vector2(pad_state_.ThumbRX / 32767.0f, pad_state_.ThumbRY / 32767.0f); }

    float GetMaxStickRateX() const
    {
        float left = GetLeftStickRate().x;
        float right = GetRightStickRate().x;
        return (std::abs(left) >= std::abs(right)) ? left : right;
    }
};

/// <summary>
/// 入力データを管理、保存しているクラス。
/// </summary>
class InputManager : public GameObject
{
public:
    static InputManager* Instance();

    static constexpr int INPUT_HISTORY_SIZE = 60; // 保存するフレーム数（例:60）

    void Update() override;

    // キーボード
    bool GetKey(int keyCode) const;
    bool GetKeyDown(int keyCode) const;
    bool GetKeyUp(int keyCode) const;

    // XINPUTコントローラー
    bool GetButton(int button) const;
    bool GetButtonDown(int button) const;
    bool GetButtonUp(int button) const;
    
	//スティックの値が大きいほうを返す
	float GetMaxStickMovedX() const;

	// 前フレームからのスティックの移動量を取得
    Vector2 GetLeftStickMoved() const;
    Vector2 GetRightStickMoved() const;

    /// <summary>
    /// 指定したフレーム番号の直前の入力フレーム状態を取得します。
    /// </summary>
    /// <param name="frame">何フレーム前か(0~MAX)</param>
    /// <returns>nフレーム前の入力データ</returns>
    const InputFrameState& GetPrevState(int frame) const;

	inline const InputFrameState& GetCurrentState() const { return current_state_; }

private:
    InputManager();

    std::vector<InputFrameState> prev_states_;  // 履歴リングバッファ
	InputFrameState current_state_;             // 現在のフレーム
    int history_index_;
};