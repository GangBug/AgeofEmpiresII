#include "IA_FormationManager.h"
#include "App.h"
#include "M_Render.h"

IA_FormationManager::IA_FormationManager(entity_type type, Entity* parent) : Entity(type, parent)
{

}

IA_FormationManager::~IA_FormationManager()
{
}

void IA_FormationManager::OnStart()
{
}

void IA_FormationManager::OnFinish()
{
}

void IA_FormationManager::OnEnable()
{
}

void IA_FormationManager::OnDisable()
{
}

void IA_FormationManager::OnTransformUpdated()
{
}

void IA_FormationManager::OnUpdate(float dt)
{
}

bool IA_FormationManager::OnSave(pugi::xml_node& node) const
{
	bool ret = true;


	return ret;
}

bool IA_FormationManager::OnLoad(pugi::xml_node* node)
{
	bool ret = true;


	return ret;
}

void IA_FormationManager::DrawDebug()
{
}