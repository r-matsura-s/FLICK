#include "StaticMesh.h"
#include "../Library/resourceLoader.h"

StaticMeshManager* StaticMeshManager::instance = nullptr;

#pragma region __StaticMesh__

StaticMesh::StaticMesh(const std::string& file_path, const Transform& _transform, bool push_manager)
{
	h_mesh = ResourceLoader::MV1LoadModel(file_path);
	transform = _transform;
	MV1SetMatrix(h_mesh, transform.GetMatrix());

	if (push_manager)
	{
		StaticMeshManager::Instance()->Push(this);
	}
}

StaticMesh::~StaticMesh()
{
	MV1DeleteModel(h_mesh);
}

void StaticMesh::Draw() const
{
	MV1DrawModel(h_mesh);
}

#pragma endregion


#pragma region __StaticMeshManager__

StaticMeshManager* StaticMeshManager::Instance()
{
	if (instance == nullptr) 
	{
		instance = new StaticMeshManager();
	}
	return instance;
}

void StaticMeshManager::Release()
{
	SAFE_DELETE(instance);
}

StaticMeshManager::~StaticMeshManager()
{
	for (StaticMesh* obj : object_list) 
	{
		delete obj;
	}
	object_list.clear();
}

void StaticMeshManager::Draw()
{
	for (const StaticMesh* obj : object_list) 
	{
		obj->Draw();
	}
}

void StaticMeshManager::Push(StaticMesh* obj)
{
	object_list.push_back(obj);
}

void StaticMeshManager::Pop(StaticMesh* obj, bool need_delete)
{
	auto itr = std::find(object_list.begin(), object_list.end(), obj);
	if (itr != object_list.end()) 
	{
		if (need_delete) 
		{
			StaticMesh* p = *itr;
			SAFE_DELETE(p);
		}
		object_list.erase(itr);
	}
}

#pragma endregion