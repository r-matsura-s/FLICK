using UnityEngine;
using UnityEngine.Playables;
using UnityEngine.Timeline;


[TrackClipType(typeof(HoldClip))] // ここは HoldClip
[TrackBindingType(typeof(AudioSource))] // ★追加: スピーカー対応
public class HoldTrack : TrackAsset
{
    // ★追加: SE設定用
    public AudioClip hitSound;
    [Range(0, 1)] public float volume = 1.0f;

    // ★追加: Mixer作成
    public override Playable CreateTrackMixer(PlayableGraph graph, GameObject go, int inputCount)
    {
        var scriptPlayable = ScriptPlayable<NoteMixerBehaviour>.Create(graph, inputCount);

        NoteMixerBehaviour mixer = scriptPlayable.GetBehaviour();
        mixer.hitSound = hitSound;
        mixer.volume = volume;

        return scriptPlayable;
    }
}