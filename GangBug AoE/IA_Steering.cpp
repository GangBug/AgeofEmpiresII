#include "IA_Steering.h"

IA_Steering::IA_Steering(entity_type type, Entity* parent): Entity(type, parent)
{
}

IA_Steering::~IA_Steering()
{
}

void IA_Unit::OnStart()
{
}

void IA_Unit::OnFinish()
{
}

void IA_Unit::OnEnable()
{
}

void IA_Unit::OnDisable()
{
}

void IA_Unit::OnTransformUpdated()
{
}

void IA_Unit::OnUpdate(float dt)
{
}

bool IA_Unit::OnSave(pugi::xml_node& node) const
{
	bool ret = true;


	return ret;
}

bool IA_Unit::OnLoad(pugi::xml_node* node)
{
	bool ret = true;


	return ret;
}

void IA_Unit::DrawDebug()
{
	app->render->DrawQuad({ 0, 0, 10, 10 }, 255, 0, 0, 255);
}