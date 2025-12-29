#include "resourceLoader.h"
#include "myDxLib.h"
#include <unordered_map>
#include <windows.h>
#include <assert.h>

namespace 
{
	// ハンドラーのタイプ
	enum TYPE 
	{
		T_IMAGE = 0,	// 画像
		T_MODEL,		// モデル
		T_SOUND,		// 音声
		T_NULL			// なし
	};

	// ハンドラー管理する構造体
	struct FILE_HANDLE 
	{
		int handle;
		TYPE type;

		FILE_HANDLE() : handle(-1), type(TYPE::T_NULL) {}
	};

	
	class Loader
	{
	public:
		// ハンドラー保存リスト
		std::unordered_map<std::string, FILE_HANDLE> files;

		Loader() {}
		~Loader() {}
	};

	// メモリリーク対策にクラス化 (イテレーターのデストラクタが呼ばれたらOK)
	Loader* loader = nullptr;
};


void ResourceLoader::Init()
{
	if (loader == nullptr)
	{
		loader = new Loader();
	}

	loader->files.clear();
}

void ResourceLoader::Release()
{
	ResourceLoader::ReleaseAllFile();
	SAFE_DELETE(loader);
}

int ResourceLoader::LoadGraph(std::string filename)
{
	FILE_HANDLE f = loader->files[filename];
	if (f.handle == -1) 
	{
		f.handle = DxLib::LoadGraph(filename.c_str());
		f.type = TYPE::T_IMAGE;
		loader->files[filename] = f;
	}
	return f.handle;// 画像はデュプリケートできない
}

int ResourceLoader::MV1LoadModel(std::string filename)
{
	FILE_HANDLE f = loader->files[filename];
	if (f.handle == -1) 
	{
		f.handle = DxLib::MV1LoadModel(filename.c_str());
		f.type = TYPE::T_MODEL;
		loader->files[filename] = f;
	}
	return DxLib::MV1DuplicateModel(f.handle);
}

int ResourceLoader::LoadSoundMem(std::string filename)
{
	FILE_HANDLE f = loader->files[filename];
	if (f.handle == -1) 
	{
		f.handle = DxLib::LoadSoundMem(filename.c_str());
		f.type = TYPE::T_SOUND;
		loader->files[filename] = f;
	}
	return DuplicateSoundMem(f.handle);
}

void ResourceLoader::LoadFolderFile(std::string folder, bool loadSubFolder)
{
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;
	hFind = FindFirstFile(folder.c_str(), &win32fd);
	
	if (hFind == INVALID_HANDLE_VALUE) assert(false);

	do 
	{
		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{
			// ディレクトリが見つかったら、再帰的にファイルを読みたい
		} 
		else 
		{
			std::string fname = win32fd.cFileName;
			// ファイルが見つかったので、unordered_mapで登録する
			// フォルダ名とか、ファイル名の相対表示に気を付けるよ
		}
	} while (FindNextFile(hFind, &win32fd) != 0);
}

void ResourceLoader::SetAsync(bool async)
{
	SetUseASyncLoadFlag(async);
}

bool ResourceLoader::IsLoading()
{
	return (GetASyncLoadNum() > 0);
}

void ResourceLoader::ReleaseAllFile()
{
	for(auto f : loader->files)
	{
		switch (f.second.type) 
		{
		case TYPE::T_IMAGE:
			DxLib::DeleteGraph(f.second.handle);
			break;
		case TYPE::T_MODEL:
			DxLib::MV1DeleteModel(f.second.handle);
			break;
		case TYPE::T_SOUND:
			DxLib::DeleteSoundMem(f.second.handle);
			break;
		case TYPE::T_NULL:
			assert("ResourceLoader.HandleType" == "TYPE::T_NULL");
			break;
		}
	}
	loader->files.clear();
}
