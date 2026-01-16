#include "Notes.h"
#include "LaneManager.h"
#include "SoundManager.h"
#include "../Library/resourceLoader.h"
#include "../Library/Renderer2D.h"

#undef max
#undef min


Notes::Notes(double horizontal, double arrive_time, NotesType type)
{
	position_ = LaneLeapPosition((float)horizontal, (float)arrive_time);
	horizontal_rate_ = (float)horizontal;
	arrive_time_ = (float)arrive_time;
	current_time_ = 0.0f;
	type_ = type;
	judged_ = false;
	sound_played_ = false; 
	
	// ホールドノーツの初期化
	end_position_ = Vector3::Zero();
	hold_end_time_ = 0.0f;
	is_holding_ = false;
	hold_started_ = false;
	hold_ended_ = false;
	end_sound_played_ = true;
	
	// 忘れずに各セッターを呼ぶ
	SetTapSound();
	SetUpdateAction();
	SetDrawAction();
	SetNoteImage();
}

// ホールドノーツ専用コンストラクタ
Notes::Notes(NotesType type, double begin_horizontal, double begin_arrive_time, double end_horizontal, double end_arrive_time)
{
	// 基本情報の設定
	position_ = LaneLeapPosition((float)begin_horizontal, (float)begin_arrive_time);
	end_position_ = LaneLeapPosition((float)end_horizontal, (float)end_arrive_time);
	start_position_ = position_;
	horizontal_rate_ = (float)begin_horizontal;
	arrive_time_ = (float)begin_arrive_time;
	current_time_ = 0.0f;
	type_ = type;
	judged_ = false;
	sound_played_ = false;
	
	// ホールドノーツ専用の設定
	hold_end_time_ = (float)end_arrive_time;
	is_holding_ = false;
	hold_started_ = false;
	hold_ended_ = false;
	end_sound_played_ = false;

	// 忘れずに各セッターを呼ぶ
	SetTapSound();
	SetUpdateAction();
	SetDrawAction();
	SetNoteImage();
}

Notes::~Notes()
{
}

// ここからUpdate関連

void Notes::Update()
{
	update_action_.Invoke();
}

void Notes::UpdateMove()
{
	// ノーツの位置更新
	current_time_ += Time::DeltaTime();
	UpdateVerticalPos();
}

void Notes::UpdateOnHold()
{
	// ホールド中の処理
	if (is_holding_)
	{
		position_.x = Vector3::Lerp(end_position_, start_position_, std::clamp((hold_end_time_ - current_time_), -0.0f, 1.0000001f)).x;
		position_.z = std::max(position_.z, 0.0f);
		end_position_.z = std::max(end_position_.z, 0.0f);
	}
}

void Notes::UpdateGuidSound()
{
	// 開始地点のガイド音
	if (!sound_played_)
	{
		// 次のフレームで到達している
		if (current_time_ + Time::DeltaTime() >= arrive_time_)
		{
			SoundManager::Instance()->PlaySE("guid");
			sound_played_ = true;
		}
	}

	// ホールドノーツの終了地点のガイド音
	if (!end_sound_played_ && !IsJudged())
	{
		// 次のフレームで終了地点に到達している
		if (current_time_ + Time::DeltaTime() >= hold_end_time_)
		{
			SoundManager::Instance()->PlaySE("guid");
			end_sound_played_ = true;
		}
	}
}

void Notes::SetUpdateAction()
{
	update_action_ += [this] { UpdateMove(); };

	if (IsHoldNote())
	{
		update_action_ += [this] { UpdateOnHold(); };
	}

	update_action_ += [this] { UpdateGuidSound(); };
}

void Notes::SetJudged(bool judged)
{
	judged_ = judged;
	if (judged_)
	{
		// 判定されたら描画処理をクリア
		draw_action_.Clear();
	}
}

// ここからサウンド関連

void Notes::PlayTapSE() const
{
	SoundManager::Instance()->PlaySE(tap_sound_);
}

void Notes::SetTapSound()
{
	switch (type_)
	{
	case NotesType::TAP_1:
	case NotesType::TAP_2:
		tap_sound_ = "note_tap";
		break;

	case NotesType::HOLD_1:
	case NotesType::HOLD_2:
		tap_sound_ = "note_tap";
		DebugLog("Notes::SetTapSound()_仮サウンド [ホールド]");
		break;

	case NotesType::SIDE_1:
	case NotesType::SIDE_2:
		tap_sound_ = "note_tap";
		DebugLog("Notes::SetTapSound()_仮サウンド [サイド]");
		break;

	case NotesType::FLICK_L:
	case NotesType::FLICK_R:
		tap_sound_ = "note_tap";
		DebugLog("Notes::SetTapSound()_仮サウンド [フリック]");
		break;

	case NotesType::DAMAGE:
		tap_sound_ = "note_tap";
		DebugLog("Notes::SetTapSound()_仮サウンド [ダメージ]");
		break;

	case NotesType::HEALING:
		tap_sound_ = "note_tap";
		DebugLog("Notes::SetTapSound()_仮サウンド [回復]");
		break;

	default:
		DebugLog("Notes::SetTapSound() - 未対応のノーツタイプ");
		break;
	}
}

// ここから描画関連

void Notes::Draw()
{
	//if (judged_) return;

	// 描画処理
	draw_action_.Invoke();
}

void Notes::SetDrawAction()
{
	switch (type_)
	{
	case NotesType::TAP_1:
		draw_action_ += [this] { this->DrawTap(position_, Color::Red()); };
		break;

	case NotesType::TAP_2:
		draw_action_ += [this] { this->DrawTap(position_, Color::Green()); };
		break;

	case NotesType::HOLD_1:
		draw_action_ += [this] { this->DrawHold(position_, end_position_, Color::Red()); };
		break;
	case NotesType::HOLD_2:
		draw_action_ += [this] { this->DrawHold(position_, end_position_, Color::Green()); };
		break;

		//case NotesType::SIDE_1:
		//	break;
		//case NotesType::SIDE_2:
		//	break;

		case NotesType::FLICK_L:
			draw_action_ += [this] { this->DrawFlick(); };
			break;
		case NotesType::FLICK_R:
			draw_action_ += [this] { this->DrawFlick(); };
			break;

	//case NotesType::DAMAGE:
	//	draw_action_ += [this] { this->DrawDamage(); };
	//	break;

	case NotesType::HEALING:
		draw_action_ += [this] { this->DrawHealing(); };
		break;

	default:
#if (_DEBUG)
		DebugLog("Notes::SetDrawAction() - 未対応のノーツタイプ");
#endif
		break;
	}
}

void Notes::SetNoteImage()
{
	// 画像の読み込み
	switch (type_)
	{
	case NotesType::TAP_1:
	case NotesType::TAP_2:
		note_handle_ = ResourceLoader::LoadGraph("data/texture/note.png");
		note_overlay_handle_ = ResourceLoader::LoadGraph("data/texture/note_overley.png");
		break;
	case NotesType::HOLD_1:
	case NotesType::HOLD_2:
		note_handle_ = ResourceLoader::LoadGraph("data/texture/note.png");
		note_overlay_handle_ = ResourceLoader::LoadGraph("data/texture/note_overley.png");
		//hold_handle_ = ResourceLoader::LoadGraph("data/texture/hold.png");
		break;
	case NotesType::FLICK_L:
		flick_handle_ = ResourceLoader::LoadGraph("data/texture/f_L.png");
		break;
	case NotesType::FLICK_R:
		flick_handle_ = ResourceLoader::LoadGraph("data/texture/f_R.png");
		break;
	case NotesType::HEALING:
		//healing_handle_ = ResourceLoader::LoadGraph("data/texture/healing.png");
		break;
	}
}

void Notes::DrawTap(const Vector3& pos, const Color& color) const
{
	//DrawSphere3D(position_, 40.0f, 8, color.RGB16(), color.RGB16(), TRUE);

	// 頂点情報
	std::vector<VERTEX3D> vertexs = GetVertexArray(pos, Vector2(2.0f, 1.0f) * 30.0f, color, Color::White());
	// 普通に描画
	DrawPolygon3D(&vertexs[0], 2, note_handle_, TRUE);
	// 暗いので加算する
	Renderer::SetBlendMode_Add(200);
	DrawPolygon3D(&vertexs[0], 2, note_handle_, TRUE);
	// 少し上に移動
	for (int i = 0; i < 6; i++)
	{
		vertexs[i].pos.y += 0.001f;
		vertexs[i].dif = { 0xff, 0xff ,0xff, 0xff };
	}
	// オーバーレイ画像の表示
	//Renderer::SetBlendMode_Add(128);
	DrawPolygon3D(&vertexs[0], 2, note_overlay_handle_, TRUE);
	Renderer::ResetBlendMode();
}

void Notes::DrawHold(const Vector3& begin, const Vector3& end, Color color) const
{	
	// 頂点情報
	VERTEX3D hold[6] =
	{
		GetVertex3D(begin + Vector3(-1.8f * 30.0f, 0.0f, 0.0f), Vector3::UnitY(), color, Color::White(), Vector2(0.0f, 0.0f), Vector2::Zero()),
		GetVertex3D(begin + Vector3( 1.8f * 30.0f, 0.0f, 0.0f), Vector3::UnitY(), color, Color::White(), Vector2(0.0f, 1.0f), Vector2::Zero()),
		GetVertex3D(end   + Vector3(-1.8f * 30.0f, 0.0f, 0.0f), Vector3::UnitY(), color, Color::White(), Vector2(1.0f, 0.0f), Vector2::Zero()),

		GetVertex3D(begin + Vector3( 1.8f * 30.0f, 0.0f, 0.0f), Vector3::UnitY(), color, Color::White(), Vector2(0.0f, 1.0f), Vector2::Zero()),
		GetVertex3D(end   + Vector3( 1.8f * 30.0f, 0.0f, 0.0f), Vector3::UnitY(), color, Color::White(), Vector2(1.0f, 1.0f), Vector2::Zero()),
		GetVertex3D(end   + Vector3(-1.8f * 30.0f, 0.0f, 0.0f), Vector3::UnitY(), color, Color::White(), Vector2(1.0f, 0.0f), Vector2::Zero()),
	};

	//DrawCapsule3D(begin, end, 20.0f, 8, color.RGB16(), color.RGB16(), TRUE);
	DrawPolygon3D(hold, 2, note_handle_, TRUE);
	if (is_holding_)
	{
		Renderer::SetBlendMode_Add(170);
		//DrawCapsule3D(begin, end, 20.5f, 8, Color::White16(), Color::White16(), TRUE);
		DrawPolygon3D(hold, 2, note_handle_, TRUE);
		//Renderer::ResetBlendMode();
	}
	
	//DrawSphere3D(end, 40.0f, 8, color.RGB16(), color.RGB16(), TRUE);
	DrawTap(end, color);
	//DrawSphere3D(begin, 40.0f, 8, color.RGB16(), color.RGB16(), TRUE);
	DrawTap(begin, color);

#if (FALSE)
	Vector2 pos = WorldPosToScreenPos(position_);
	DrawFormatStringF(pos.x, pos.y, 0x00ff00, "%.2f", (hold_end_time_ - current_time_));
#endif
}

void Notes::DrawFlick() const
{
	static const float radius = 40.0f;
	static const double scale = 40.0 * (1.0f / 512.0) * 5.0;
	DrawExtendGraph3D(position_.x - radius * 2.0f, position_.y + radius * 2.0f, position_.z, scale, scale, flick_handle_, TRUE);
	DrawExtendGraph3D(position_.x + radius * 2.0f, position_.y + radius * 2.0f, position_.z, scale, scale, flick_handle_, TRUE);
}

void Notes::DrawDamage() const
{
	return;
}

void Notes::DrawHealing() const
{
	DrawSphere3D(position_, 60.0f, 4, Color::Yellow16(), Color::Yellow16(), FALSE);
}


// 文字列からNotesTypeへ変換

NotesType Notes::ToNoteType(const std::string& note_type)
{
	// if文ラッシュを避ける
	static const std::unordered_map<std::string, NotesType> STR_TO_ENUM = 
	{
		{ "TAP_1", NotesType::TAP_1 },
		{ "TAP_2", NotesType::TAP_2 },
		{ "HOLD_1", NotesType::HOLD_1 },
		{ "HOLD_2", NotesType::HOLD_2 },
		{ "SIDE_1", NotesType::SIDE_1 },
		{ "SIDE_2", NotesType::SIDE_2 },
		{ "FLICK_L", NotesType::FLICK_L },
		{ "FLICK_R", NotesType::FLICK_R },
		{ "DAMAGE", NotesType::DAMAGE },
		{ "HEALING", NotesType::HEALING },
	};

#if (TRUE)
	// 存在するENUMか確認する
	auto itr = STR_TO_ENUM.find(note_type);
	if (itr != STR_TO_ENUM.end()) {
		return itr->second;
	}
	
	DebugLog("警告!：[" + note_type + "] は定義されていません");
	return NotesType::HEALING;
#else
	auto itr = STR_TO_ENUM.find(note_type);
	return itr->second;
#endif
}

void Notes::UpdateVerticalPos()
{
	position_.z = LaneLeapVerticalRate(arrive_time_ - current_time_);
	end_position_.z = LaneLeapVerticalRate(hold_end_time_ - current_time_);
}
