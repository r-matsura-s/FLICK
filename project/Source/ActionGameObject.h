#pragma once
#include "Object3D.h"
#include "ActionEvent.h"

class ActionGameObject : public Object3D
{
public:
	ActionGameObject();
	~ActionGameObject();

	void Start()	override;
	void Update()	override;
	void Draw()		override;

	ActionEvent<> OnStart;
	ActionEvent<> OnUpdate;
	ActionEvent<> OnDraw;
	ActionEvent<> OnRelease;
};