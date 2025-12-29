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
    inline void Invoke(Args... args) const 
    {
        for (const auto& func : callback_list_) 
        {
            func(args...);
        }
    }

    /// <summary>
    /// 登録されている要素数を獲得
    /// </summary>
    inline int Length() const 
    { 
        return length_; 
    }

private:
    std::vector<std::function<void(Args...)>> callback_list_;
    int length_ = 0;
};

// サンプル
/*
int main() 
{
    ActionEvent<int> onScore;
    Player player;

    // 引数付きイベントに、引数なしメソッドを登録（ラムダでラップ）
    onScore += [&player](int score) {
        player.ShowStatus(); // 引数は無視して呼び出す
    };

	// 引数付きイベントに、引数付き処理を登録
    onScore += [](int score) {
        std::cout << "Scored: " << score << std::endl;
    };

	onScore.Invoke(100); // player.ShowStatus()、"Scored: 100"、が呼ばれる
}

*/
