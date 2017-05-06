#include "IA_SquareFormation.h"
#include "App.h"
#include "M_Render.h"

IA_SquareFormation::IA_SquareFormation(entity_type type, Entity* parent): Entity(type, parent)
{
}

IA_SquareFormation::~IA_SquareFormation()
{
}

void IA_SquareFormation::OnStart()
{
}

void IA_SquareFormation::OnFinish()
{
}

void IA_SquareFormation::OnEnable()
{
}

void IA_SquareFormation::OnDisable()
{
}

void IA_SquareFormation::OnTransformUpdated()
{
}

void IA_SquareFormation::OnUpdate(float dt)
{
}

bool IA_SquareFormation::OnSave(pugi::xml_node& node) const
{
	bool ret = true;


	return ret;
}

bool IA_SquareFormation::OnLoad(pugi::xml_node* node)
{
	bool ret = true;


	return ret;
}

void IA_SquareFormation::DrawDebug()
{
	app->render->DrawQuad({ 0, 0, 10, 10 }, 255, 0, 0, 255);
}
