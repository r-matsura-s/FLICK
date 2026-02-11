using System;
using UnityEngine;
using UnityEditor;
using UnityEngine.Timeline;
using System.IO; // File.WriteAllText 用
using System.Collections.Generic; // List<> 用

public class ChartEditorWindow : EditorWindow
{
    // --- 編集用のフィールド群 ---
    private TimelineAsset currentTimeline;
    private ChartData metaData = new ChartData();


    [MenuItem("Window/Chart Editor/Open Editor")]
    public static void ShowWindow()
    {
        GetWindow<ChartEditorWindow>("Chart Editor");
    }

    private void OnGUI()
    {
        // ------------------------------------------
        // 1. メタデータ入力
        // ------------------------------------------
        GUILayout.Label("メタデータ入力", EditorStyles.boldLabel);
        metaData.title =       EditorGUILayout.TextField("曲名", metaData.title);
        metaData.subTitle =    EditorGUILayout.TextField("サブタイトル", metaData.subTitle);
        metaData.className =   EditorGUILayout.TextField("難易度タイプ", metaData.className);
        metaData.music =       EditorGUILayout.TextField("楽曲パス/名前", metaData.music);
        metaData.bpm =         EditorGUILayout.DoubleField("基準BPM", metaData.bpm);
        metaData.volume =      EditorGUILayout.DoubleField("音量(0~1)", metaData.volume);
        metaData.offset =      EditorGUILayout.DoubleField("オフセット (秒)", metaData.offset);
        metaData.beat =        EditorGUILayout.IntField("拍子 (例: 4)", metaData.beat);
        metaData.level =       EditorGUILayout.IntField("レベル", metaData.level);

        EditorGUILayout.Space(6);

        // JSONからメタ読み込みボタン
        if (GUILayout.Button("JSONからメタ読み込み", GUILayout.Height(24)))
        {
            string path = EditorUtility.OpenFilePanel(
                "メタデータJSONを選択",
                "Assets",
                "json"
            );

            if (string.IsNullOrEmpty(path) == false)
            {
                try
                {
                    string json = File.ReadAllText(path);
                    ChartData loaded = JsonUtility.FromJson<ChartData>(json);
                    if (loaded != null)
                    {
                        // エディタ上の metaData に反映
                        metaData.SetMetaData(loaded);
                        Debug.Log($"メタデータを読み込みました: {path}");
                        EditorUtility.DisplayDialog("読み込み完了", "メタデータを読み込みました。", "OK");
                    }
                    else
                    {
                        EditorUtility.DisplayDialog("読み込み失敗", "JSONの解析に失敗しました。", "OK");
                    }
                }
                catch (Exception e)
                {
                    Debug.LogError(e);
                    EditorUtility.DisplayDialog("読み込みエラー", $"ファイル読み込み中にエラーが発生しました:\n{e.Message}", "OK");
                }
            }
        }

        EditorGUILayout.Space(10);

        // ------------------------------------------
        // 2. タイムライン選択
        // ------------------------------------------
        GUILayout.Label("割り当てる Timeline", EditorStyles.boldLabel);
        currentTimeline = (TimelineAsset)EditorGUILayout.ObjectField(
            "Timeline Asset",
            currentTimeline,
            typeof(TimelineAsset),
            false
        );

        EditorGUILayout.Space(20);

        // ------------------------------------------
        // 3. JSON出力ボタン (エクスポート)
        // ------------------------------------------
        GUI.enabled = (currentTimeline != null);

        if (GUILayout.Button("JSONを書き出す", GUILayout.Height(40)))
        {
            // 呼び出し先 ExportToJson に metaData.level が反映される
            ExportToJson(currentTimeline, metaData);
        }

        GUI.enabled = true;
    }

    // ------------------------------------------------------------
    // JSON生成 (Unity 5系の JsonUtility を利用)
    // ------------------------------------------------------------
    private void ExportToJson(TimelineAsset timeline, ChartData meta)
    {
        // 1. ChartData インスタンス生成
        ChartData chart = new ChartData();
        chart.SetMetaData(meta);
        chart.notes = new List<NoteData>();
        chart.holds = new List<HoldData>();

        Debug.Log("Timeline読み取り中...");

        // 2. Timeline のトラックを調べてノーツ情報を収集
        foreach (TrackAsset track in timeline.GetRootTracks())
        {
            // --- NoteTrack (TAP/FLICK) の処理 ---
            if (track is NoteTrack noteTrack)
            {
                if(!noteTrack.needEncode) continue;

                foreach (TimelineClip clip in noteTrack.GetClips())
                {
                    NoteClip noteAsset = clip.asset as NoteClip;
                    if (noteAsset != null)
                    {
                        NoteData note = new NoteData();
                        note.time = clip.start;
                        note.lane = noteAsset.Lane;
                        note.type = noteAsset.Type;
                        chart.notes.Add(note);
                    }
                }
            }
            // --- HoldTrack (HOLD) の処理 ---
            else if (track is HoldTrack holdTrack)
            {
                foreach (TimelineClip clip in holdTrack.GetClips())
                {
                    HoldClip holdAsset = clip.asset as HoldClip;
                    if (holdAsset != null)
                    {
                        HoldData hold = new HoldData();

                        // 追加情報取得
                        hold.type = holdAsset.Type;
                        hold.startTime = clip.start;
                        hold.endTime = clip.start + clip.duration; // 開始時間 + 期間
                        hold.startLane = holdAsset.StartLane;
                        hold.endLane = holdAsset.EndLane;

                        chart.holds.Add(hold);
                    }
                }
            }
        }

        Debug.Log($"抽出完了 Notes: {chart.notes.Count} 個, Holds: {chart.holds.Count} 個");

        // 6. ファイル名決定と保存ダイアログ
        string fileName = $"{meta.title}_.json";
        string path = EditorUtility.SaveFilePanel(
            "書き出すJSONを選択",
            "Assets",
            fileName,
            "json"
        );

        // 7. JSONを書き出す
        if (string.IsNullOrEmpty(path) == false)
        {
            string jsonString = JsonUtility.ToJson(chart, true);
            File.WriteAllText(path, jsonString);

            Debug.Log($"JSON書き出し完了: {path}");
            EditorUtility.DisplayDialog("書き出し完了",
                $"JSONを書き出しました。\nNotes: {chart.notes.Count}, Holds: {chart.holds.Count}",
                "OK");
        }
    }
}