#pragma once
#include "Entity.h"
#include "IA_Steering.h"

class IA_Unit :
	public Entity
{
public:
	IA_Unit(entity_type type, Entity* parent);
	~IA_Unit();

	void OnStart() override;
	void OnFinish() override;
	void OnEnable() override;
	void OnDisable() override;
	void OnTransformUpdated() override; //Called whenever transform has changed and recalculed.

	void OnUpdate(float dt) override;

	bool OnSave(pugi::xml_node& node)const override;
	bool OnLoad(pugi::xml_node* node) override;

	void DrawDebug() override;

private:
	IA_Steering* steering;
};

