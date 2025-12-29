#pragma once
#include "../Library/GameObject.h"
#include "../Library/Transform.h"
//#include <vector>

/// <summary>
/// Updateを行わない静的メッシュオブジェクト
/// newしたらStaticMeshManagerに登録される
/// </summary>
class StaticMesh
{
public:
	StaticMesh(const std::string& file_path, const Transform& _transform, bool push_manager = true);
	~StaticMesh();

	void Draw() const;

	inline const Transform& GetTransform() const { return transform; }

private:
	Transform transform;
	int h_mesh;
};

/// <summary>
/// StaticMeshの描画を行う管理クラス
/// GameObjectを継承。各シーンの初めに生成すればいい。
/// </summary>
class StaticMeshManager : public GameObject
{
public:
	static StaticMeshManager* Instance();
	static void Release();

	void Push(StaticMesh* obj);
	void Pop(StaticMesh* obj, bool need_delete = true);

	void Draw() override;

private:
	StaticMeshManager() {};	
	~StaticMeshManager();

	static StaticMeshManager* instance;
	std::list<StaticMesh*> object_list;
};