#pragma once

#include "Entity.h"

class IA_Steering : Entity
{
public:
	IA_Steering(entity_type type, Entity* parent);
	~IA_Steering();
	
	void OnStart() override;
	void OnFinish() override;
	void OnEnable() override;
	void OnDisable() override;
	void OnTransformUpdated() override; //Called whenever transform has changed and recalculed.

	void OnUpdate(float dt) override;

	bool OnSave(pugi::xml_node& node)const override;
	bool OnLoad(pugi::xml_node* node) override;

	void DrawDebug() override;


};
