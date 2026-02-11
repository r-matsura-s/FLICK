using UnityEngine;
using UnityEngine.Playables;

public class NoteMixerBehaviour : PlayableBehaviour
{
    public AudioClip hitSound;
    public float volume = 1.0f;
    public AudioSource trackBinding;

    public override void ProcessFrame(Playable playable, FrameData info, object playerData)
    {
        trackBinding = playerData as AudioSource;
        if (trackBinding == null || hitSound == null) return;

        int inputCount = playable.GetInputCount();

        for (int i = 0; i < inputCount; i++)
        {
            float inputWeight = playable.GetInputWeight(i);
            ScriptPlayable<NoteBehaviour> inputPlayable = (ScriptPlayable<NoteBehaviour>)playable.GetInput(i);
            NoteBehaviour inputBehaviour = inputPlayable.GetBehaviour();

            // クリップ上を再生中 (weight > 0)
            if (inputWeight > 0.0f)
            {
                double time = inputPlayable.GetTime();     // クリップ内での現在の時間
                double duration = inputPlayable.GetDuration(); // クリップの長さ

                // --- 1. 始点の音 ---
                if (!inputBehaviour.hasPlayedStart)
                {
                    // タイムライン再生中は time は常に進むので、入った瞬間に鳴らす
                    trackBinding.PlayOneShot(hitSound, volume);
                    inputBehaviour.hasPlayedStart = true;
                }

                // --- 2. 終点の音 ---
                // ホールド用: 現在時間が「長さ - わずかな時間」を超えたら鳴らす
                // (0.05秒くらい手前で判定すると安定します)
                if (inputBehaviour.isHold && !inputBehaviour.hasPlayedEnd && time >= (duration - 0.05))
                {
                    trackBinding.PlayOneShot(hitSound, volume);
                    inputBehaviour.hasPlayedEnd = true;
                }
            }
            else
            {
                // 再生範囲外に出たらフラグをリセット（シークバーで戻った時などのため）
                inputBehaviour.hasPlayedStart = false;
                inputBehaviour.hasPlayedEnd = false;
            }
        }
    }
}