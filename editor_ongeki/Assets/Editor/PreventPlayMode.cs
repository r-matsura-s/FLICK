using UnityEditor;
using UnityEngine;

// エディタ起動時に自動的に読み込まれる属性
[InitializeOnLoad]
public class PreventPlayMode
{
    // 静的コンストラクタ（起動時に一度だけ呼ばれる）
    static PreventPlayMode()
    {
        // プレイモードの状態が変化しようとした時のイベントを登録
        EditorApplication.playModeStateChanged += OnPlayModeStateChanged;
    }

    private static void OnPlayModeStateChanged(PlayModeStateChange state)
    {
        // 「エディタモードから抜けようとしている（＝再生ボタンが押された）」瞬間を検知
        if (state == PlayModeStateChange.ExitingEditMode)
        {
            // 警告ダイアログを表示
            bool cancelPlay = EditorUtility.DisplayDialog(
                "確認",
                "このプロジェクトは譜面制作ツールです。\nPlayボタンを押す必要はありません。\n\n再生を中止しますか？",
                "はい (再生中止)", // こちらがデフォルト(Enterキー)
                "いいえ (無理やり再生)"
            );

            if (cancelPlay)
            {
                // 再生をキャンセルする（Playボタンをオフに戻す）
                EditorApplication.isPlaying = false;
                Debug.Log("Playモードへの移行をキャンセルしました。");
            }
        }
    }
}