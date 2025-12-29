#include "InputManager.h"

#pragma region //シングルトン関数定義
InputManager* InputManager::Instance()
{
    static InputManager* instance = nullptr;
    if (instance == nullptr) {
        instance = new InputManager();
    }
    return instance;
}
#pragma endregion 

InputManager::InputManager()
{
    DontDestroyOnSceneChange();
    
    // pad入力の獲得
    GetJoypadXInputState(DX_INPUT_PAD1, &current_state_.pad_state_);

	// prevの初期化
    prev_states_.resize(INPUT_HISTORY_SIZE);
    for (InputFrameState& state : prev_states_) {
        state = InputFrameState();
    }
    history_index_ = 0;
}

void InputManager::Update()
{
    // 履歴保存
	prev_states_[history_index_] = current_state_;
    history_index_ = (history_index_ + 1) % INPUT_HISTORY_SIZE;

    // キーボード
    GetHitKeyStateAll(current_state_.keys_.data());

    // XINPUT
    GetJoypadXInputState(DX_INPUT_PAD1, &current_state_.pad_state_);
}

#pragma region //Get関数定義

const InputFrameState& InputManager::GetPrevState(int frame) const
{
    // 負の値なら最新データを返す
    if (frame < 0)
    {
        int curr = (history_index_ - 1 + INPUT_HISTORY_SIZE) % INPUT_HISTORY_SIZE;
        return prev_states_[curr];
    }

    // 保存数より多い場合は最古データを返す
    if (frame >= INPUT_HISTORY_SIZE)
    {
        return prev_states_[history_index_];//次に書き込む位置
    }

    // 通常
    int idx = (history_index_ - frame - 1 + INPUT_HISTORY_SIZE) % INPUT_HISTORY_SIZE;
    return prev_states_[idx];
}

// キーボード判定
bool InputManager::GetKey(int key_input_code) const
{
    return current_state_.keys_[key_input_code] != 0;
}

bool InputManager::GetKeyDown(int key_input_code) const
{
    return current_state_.keys_[key_input_code] != 0 &&
        GetPrevState(0).keys_[key_input_code] == 0;
}

bool InputManager::GetKeyUp(int key_input_code) const
{
    return current_state_.keys_[key_input_code] == 0 &&
        GetPrevState(0).keys_[key_input_code] != 0;
}

// XINPUTボタン判定
bool InputManager::GetButton(int button) const
{
    return (current_state_.pad_state_.Buttons[button] != 0);
}

bool InputManager::GetButtonDown(int button) const
{
    return (current_state_.pad_state_.Buttons[button] != 0) &&
        (GetPrevState(0).pad_state_.Buttons[button] == 0);
}

bool InputManager::GetButtonUp(int button) const
{
    return (current_state_.pad_state_.Buttons[button] == 0) &&
        (GetPrevState(0).pad_state_.Buttons[button] != 0);
}

Vector2 InputManager::GetLeftStickMoved() const
{
    InputFrameState prev = GetPrevState(0);
    return current_state_.GetLeftStick() - Vector2(prev.pad_state_.ThumbLX, prev.pad_state_.ThumbLY);
}

Vector2 InputManager::GetRightStickMoved() const
{
    InputFrameState prev = GetPrevState(0);
    return current_state_.GetRightStick() - Vector2(prev.pad_state_.ThumbRX, prev.pad_state_.ThumbRY);
}

float InputManager::GetMaxStickMovedX() const
{
    float left = GetLeftStickMoved().x;
    float right = GetRightStickMoved().x;
    return (std::abs(left) >= std::abs(right)) ? left : right;
}
#pragma endregion
