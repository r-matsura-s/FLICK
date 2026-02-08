#include "ActionGameObject.h"

ActionGameObject::ActionGameObject() : Object3D()
{
}

ActionGameObject::~ActionGameObject()
{
	OnRelease.Invoke();
}

void ActionGameObject::Start()
{
	OnStart.Invoke();
}
void ActionGameObject::Update()
{
	Object3D::Update();
	OnUpdate.Invoke();
}
void ActionGameObject::Draw()
{
	OnDraw.Invoke();
}
