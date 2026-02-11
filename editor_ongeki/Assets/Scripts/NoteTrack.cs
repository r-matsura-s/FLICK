using UnityEngine;
using UnityEngine.Timeline;
using UnityEngine.Playables;

[TrackClipType(typeof(NoteClip))]
[TrackBindingType(typeof(AudioSource))] // ▼ 1. AudioSourceを紐づけ可能にする
public class NoteTrack : TrackAsset
{
    // ▼ 2. トラックのインスペクタでSEを設定できるようにする
    public AudioClip hitSound;
    [Range(0, 1)] public float volume = 1.0f;
    public bool needEncode = true;

    // ▼ 3. Mixerを作成して、SE情報を渡す処理
    public override Playable CreateTrackMixer(PlayableGraph graph, GameObject go, int inputCount)
    {
        // Mixerを作る
        var scriptPlayable = ScriptPlayable<NoteMixerBehaviour>.Create(graph, inputCount);

        // MixerにSEと音量を渡す
        NoteMixerBehaviour mixer = scriptPlayable.GetBehaviour();
        mixer.hitSound = hitSound;
        mixer.volume = volume;

        return scriptPlayable;
    }
}