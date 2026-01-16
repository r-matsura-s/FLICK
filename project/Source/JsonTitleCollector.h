#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include "../Library/json.hpp"
#include "NotesMetaData.h"

// 短く書くためのエイリアス
namespace fs = std::filesystem;
using json = nlohmann::json;

class JsonTitleCollector 
{
public:
    // タイトルを保存するメンバ変数
    std::vector<NotesMetaData> titles;

    /**
     * 指定したディレクトリ内のJSONファイルを読み込み、
     * meta["title"] の値を titles に格納する
     */
    void LoadTitlesFromDirectory(const std::string& directoryPath) {
        // ディレクトリが存在するか確認
        if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath)) {
            std::cout << "エラー: ディレクトリが見つかりません: " << directoryPath << std::endl;
            return;
        }

        // メンバ変数をクリア（再利用する場合のため）
        titles.clear();

        // ディレクトリ内を走査
        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            // ファイルであり、かつ拡張子が .json の場合のみ処理
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                ProcessJsonFile(entry.path().string());
            }
        }
    }

private:
    void ProcessJsonFile(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cout << "警告: ファイルが開けません: " << filePath << std::endl;
            return;
        }

        try {
            json j = json::parse(file);
            
            // 文字列として値を取得し、メンバ変数に追加
            NotesMetaData meta_data;
            meta_data.SetToJsonObj(j);
            titles.push_back(meta_data);

            std::cout << "[読込成功] " << filePath << std::endl;

        }
        catch (const json::parse_error& e) {
            std::cout << "[パースエラー] " << filePath << ": " << e.what() << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "[エラー] " << filePath << ": " << e.what() << std::endl;
        }
    }
};

// サンプル
/*
int main() {
    // 読み込み対象のディレクトリパス (適宜変更してください)
    // "." は現在のディレクトリ、"./data" など指定可能
    std::string targetDir = "./json_files";

    // テスト用にディレクトリとダミーファイルを作成（存在しない場合）
    // ※ 実際に動かす際は不要なブロックです
    if (!fs::exists(targetDir)) {
        fs::create_directory(targetDir);
        std::ofstream(targetDir + "/test1.json") << R"({ "meta": { "title": "First Document" }, "id": 1 })";
        std::ofstream(targetDir + "/test2.json") << R"({ "meta": { "title": "Second Document" }, "id": 2 })";
        std::ofstream(targetDir + "/ignore.txt") << "This is not a json file.";
        std::cout << "テスト用ファイルを作成しました: " << targetDir << std::endl;
    }

    // --- メイン処理 ---
    JsonTitleCollector collector;
    collector.loadTitlesFromDirectory(targetDir);

    // 結果確認
    collector.printTitles();

    return 0;
}
// */