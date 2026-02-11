using System;
using System.Collections.Generic;
using Unity.VisualScripting;

/// <summary>
/// このクラス（のリスト）を最終的にJSONに変換する
/// </summary>
[Serializable]
public class NoteData
{
    // C++側で欲しい情報（Tick, レーン, ノーツ種別）
    public string type;     // "TAP", "FLICK" など
    public double time;     // ノーツの開始時間 (秒)
    public double lane;      // レーン (-1.0 ~ 1.0)

    // C++側でBPMとtime(秒)から「Tick」に変換します。
}

/// <summary>
/// ご提示のホールドノーツのデータ例に対応するクラス
/// </summary>
[Serializable]
public class HoldData
{
    // C++側で欲しい情報
    public string type;         // "HOLD_1" など
    public double startTime;    // ホールドの開始時間 (秒)
    public double endTime;      // ホールドの終了時間 (秒)
    public double startLane;     // 開始レーン(-1.0 ~ 1.0)
    public double endLane;       // 終了レーン(-1.0 ~ 1.0)
}


// JSON出力用のルートクラス
[Serializable]
public class ChartData
{
    // メタデータ
    public string title = "ここに曲名";
    public string subTitle = "---";
    public string className = "EXPERT";
    public string music = "music";
    public double bpm = 150.0;
    public double volume = 1.0;
    public double offset = 0.0;//未定
    public int beat = 4;
    public int level = 7;//1~15くらいで

    // ノーツデータ
    public List<NoteData> notes = new List<NoteData>();
    public List<HoldData> holds = new List<HoldData>();


    public void SetMetaData(ChartData other)
    {
        this.title      = other.title;
        this.subTitle   = other.subTitle;
        this.className  = other.className;
        this.music      = other.music;
        this.bpm        = other.bpm;
        this.volume     = Math.Clamp(other.volume, 0.0, 2.0);//本当は0~1だが、2倍まで許容
        this.offset     = other.offset;
        this.beat       = other.beat;
        this.level      = other.level;
    }
}