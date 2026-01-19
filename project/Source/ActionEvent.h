#pragma once
#include <iostream>
#include <functional>
#include <vector>

/// <summary>
/// ・UinityのC#のActionイベントのようなもの
/// 
/// ・基本的にラムダ式を使用してコールバック関数を登録する
/// </summary>
/// <typeparam name="...Args">　<int>や<int, int>など　</typeparam>
template<typename... Args>
class ActionEvent 
{
public:
    ActionEvent& operator+=(std::function<void(Args...)> func) 
    {
        callback_list_.push_back(func);
        length_++;
        return *this;
    }

    /// <summary>
    /// リストに追加した関数を呼ぶ
    /// </summary>
    /// <param name="...args">ActionEvent宣言時の型<></param>
    inline void Invoke(Args... args)  
    {
		is_invoking_ = true;
        for (const auto& func : callback_list_) 
        {
            func(args...);
        }
		is_invoking_ = false;
    }

    /// <summary>
    /// 指定インデックスのコールバックのみを呼ぶ
    /// インデックスが範囲外の場合は何もしない（早期リターン）
    /// コールバックに引数がある場合でも渡せるように Args... を受け取って転送する
    /// </summary>
    inline void InvokeAt(int index, Args... args)
    {
        // 安全な範囲チェック：負の値および大きすぎる値は無視する
        if (index < 0 || index >= Length())
        {
            return;
        }

		is_invoking_ = true;
        if (const auto& func = callback_list_[index])
        {
            func(args...);
        }
        is_invoking_ = false;
    }

    /// <summary>
    /// 登録されている要素数を獲得
    /// </summary>
    inline int Length() const 
    { 
        return length_; 
    }

    /// <summary>
	/// Invoke中か取得
    /// </summary>
    inline bool IsInvoking() const
    {
        return is_invoking_;
	}

    /// <summary>
    /// 登録されているコールバックを全て削除する
    /// </summary>
    inline void Clear()
    {
#if (_DEBUG)
        if (is_invoking_)
        {
            OutputDebugStringA("ActionEvent::Clear() - Invoke中にClearは推奨されていません。予期せぬ処理になる場合があります。\n");
        }
#endif

        callback_list_.clear();
        length_ = 0;
    }

private:
    std::vector<std::function<void(Args...)>> callback_list_;
    int length_ = 0;
	bool is_invoking_ = false;
};

// サンプル
/*
int main() 
{
    ActionEvent<int> onScore;
    Player player;

    // 引数付きイベントに、引数なしメソッドを登録（ラムダでラップ）
    onScore += [&player](int score) { player.ShowStatus(); };

	// 引数付きイベントに、引数付き処理を登録
    onScore += [](int score) { std::cout <<  score << std::endl; };

	onScore.Invoke(100); // player.ShowStatus()、"Scored: 100"、が呼ばれる

	// インデックス0 のコールバックのみ呼ぶ
	onScore.InvokeAt(0, 200);
}

Player::Init() 
{
    // クラス内でメソッドを登録する場合

    ActionEvent<> actionEvent;
    actionEvent += [this] { this->ShowStatus(); };

	actionEvent.Invoke();
}

*/
