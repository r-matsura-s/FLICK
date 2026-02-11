using System.Numerics;
using UnityEngine;
using UnityEngine.Playables;
using UnityEngine.Timeline;

// IDurationインターフェースを実装すると、Timelineで長さを変えられます
public class HoldClip : PlayableAsset, ITimelineClipAsset
{
    public enum HoldType
    {
        HOLD_1,
        HOLD_2,
        // ホールド以外は別のクラスで定義する
    }

    // インスペクタで設定する項目
    [SerializeField]
    private HoldType type = HoldType.HOLD_1;
    [SerializeField, Range(-1.0f, 1.0f)]
    private double startLane = 0.0f;
    [SerializeField, Range(-1.0f, 1.0f)]
    private double endLane = 0.0f;

    public double StartLane => startLane;
    public double EndLane => endLane;
    public string Type => type.ToString();


    // クリップの機能（今回は「長さが変えられる」ことだけが重要）
    public ClipCaps clipCaps
    {
        get { return ClipCaps.Blending; } // (BlendingでOK)
    }

    // (PlayableBehaviourは空でOK)
    public override Playable CreatePlayable(PlayableGraph graph, GameObject owner)
    {
        ScriptPlayable<NoteBehaviour> playable = ScriptPlayable<NoteBehaviour>.Create(graph);
        playable.GetBehaviour().isHold = true;

        return playable;
    }
}