using UnityEngine;
using UnityEngine.Playables;

public class NoteClip : PlayableAsset
{
    public enum NoteType
    {
        TAP_1,
        TAP_2,
        FLICK_L,
        FLICK_R,
        HEALING,
        DAMAGE,
        //ホールド属性はHoldClip.csで扱うのでここには不要
    }

    // インスペクタで設定する項目
    [SerializeField, Range(-1, 1)]
    private double lane = 0.0;
    [SerializeField]
    private NoteType type = NoteType.TAP_1;

    public double Lane => lane;
    public string Type => type.ToString();

    // (PlayableBehaviour を返す処理をここに書く)
    public override Playable CreatePlayable(PlayableGraph graph, GameObject owner)
    {
        ScriptPlayable <NoteBehaviour> playable = ScriptPlayable<NoteBehaviour>.Create(graph);
        playable.GetBehaviour().isHold = false;

        return playable;
    }
}