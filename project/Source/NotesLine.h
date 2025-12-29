#pragma once
#include "../Library/GameObject.h"
#include <vector>

/// <summary>
/// 次のノーツまで線を引くクラス
/// 1本ごとにインスタンスを作る
/// </summary>
class NotesLine : public GameObject
{
public:
	NotesLine(Color color);
	~NotesLine();

	struct Node
	{
		Vector3 begin, end;
		Node() {}
		Node(const Vector3& _begin, const Vector3& _end)
			: begin(_begin), end(_end) {}
	};

	void Update()	override;
	void Draw()		override;

	void Push(const Node& node);
	inline void SetStartTime(float t) { if(arrive_time_ == nullptr) arrive_time_ = new float(t); }

private:

	std::vector<Node> position_list_;
	Vector3 base_position_;
	float current_time_ = 0.0f;
	float* arrive_time_;
	int color_ = 0x00ffff;
};