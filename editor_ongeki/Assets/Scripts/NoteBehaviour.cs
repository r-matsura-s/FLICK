using UnityEngine;
using UnityEngine.Playables;

// ノーツ個別の状態管理クラス
public class NoteBehaviour : PlayableBehaviour
{
    //public bool hasPlayed = false; // 音を鳴らしたかどうかのフラグ

    public bool hasPlayedStart = false; // 始点を鳴らしたか
    public bool hasPlayedEnd = false;   // 終点を鳴らしたか

    public bool isHold = false;
}