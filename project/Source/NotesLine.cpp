#include "NotesLine.h"
#include "LaneManager.h"

NotesLine::NotesLine(Color color)
{
	color_ = color.RGB16();
	base_position_ = Vector3();
}

NotesLine::~NotesLine()
{
	position_list_.clear();
	SAFE_DELETE(arrive_time_);
}

void NotesLine::Update()
{
	if (arrive_time_ == nullptr) return;

	current_time_ += Time::DeltaTime();
	base_position_.z = LaneLeapVerticalRate(*arrive_time_ - current_time_);
}

void NotesLine::Draw()
{
	for (const Node& node : position_list_) 
	{
		DrawLine3D(base_position_ + node.begin, base_position_ + node.end, color_);
	}
}

void NotesLine::Push(const Node& node)
{
	position_list_.push_back(node);
}
