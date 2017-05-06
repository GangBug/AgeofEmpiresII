#include "Entity.h"
#pragma once

class IA_FormationManager : public Entity
{
public:
	IA_FormationManager(entity_type type, Entity* parent);
	~IA_FormationManager();

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
