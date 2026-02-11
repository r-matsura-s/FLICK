

/// <summary>
/// このプロジェクトを使用する際の注意点やヒントを記載するクラス
/// ブロックごとに ifdefで囲み、コメントを無視させる
/// </summary>
public class HowToMemo
{
    // Timelineアセットの作成
#if (false)
    １．Timelineの準備
    ・Hierarchyウィンドウで、Create > Empty GameObject を作成します（名前は ChartEditor など）。

    ・ChartEditor オブジェクト
    　Audio Source コンポーネントを追加します。
    　選択した状態で、Window > Sequencing > Timeline を開きます。
    
    ・Timelineウィンドウに表示される Create ボタンを押し、Timelineアセット（譜面データの実体）を保存します（名前は test_chart.playable など）。
    
    ・Timelineに Audio Track を追加し、BGMにしたいWAVやMP3をドラッグ＆ドロップします。
    　Assets フォルダ（Projectウィンドウ）にあるBGMの AudioClip (WAVやMP3のアイコン) を、Timelineウィンドウの Audio Track の右側の広いトラックエリアに、直接ドラッグ＆ドロップしてください。
    
    2. NoteTrack と NoteClip の配置
    ・Timelineのトラックリスト（左側）で右クリックし、表示されたメニューから NoteTrack を選択して追加します。
    
    ・追加された NoteTrack のトラックエリア（右側）で右クリックし、Add NoteClip を選択します。
    　トラック上に、NoteClip のクリップが配置されます。
    
    3. インスペクタでのデータ編集 (最重要)
    ・Timeline上で、先ほど追加した NoteClip のクリップを1回クリックして選択します。
    　UnityエディタのInspectorウィンドウ（通常、右側にあるウィンドウ）を見てください。
    　Inspectorに、NoteClip.cs で定義した項目が表示されているはずです。
    
     ・この Type や Lane の値を、インスペクタ上で自由に変更できることを確認してください。

#endif

    // TimeLineをjsonに書き出す
#if (false)
    １．Unityエディタの上部メニューに Window > Chart Editor > Open Editor を開く。

    ２．各項目を入力、TimeLine（test_chart.playable など）をアタッチ。

    ３．入力完了できたら「JSONに書き出し」ボタンを押す。
#endif
}
