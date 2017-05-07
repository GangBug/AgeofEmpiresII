#include "IA_Steering.h"
#include "App.h"
#include "M_Render.h"
#include "Steer_Arrive.h"

IA_Steering::IA_Steering(Entity* parent): Entity(ENTITY_STEERING, parent)
{
}

IA_Steering::~IA_Steering()
{
}

void IA_Steering::OnStart()
{
	Steer_Arrive* arrive = new Steer_Arrive();
	arrive->gameObject = parent;
	steeringList.push_back(arrive);
}

void IA_Steering::OnFinish()
{
}

void IA_Steering::OnEnable()
{
}

void IA_Steering::OnDisable()
{
}

void IA_Steering::OnTransformUpdated()
{
}

void IA_Steering::OnUpdate(float dt)
{
	for (auto steering : steeringList)
	{
		steering->Update(dt);
	}
}

bool IA_Steering::OnSave(pugi::xml_node& node) const
{
	bool ret = true;


	return ret;
}

bool IA_Steering::OnLoad(pugi::xml_node* node)
{
	bool ret = true;


	return ret;
}

void IA_Steering::DrawDebug()
{
	app->render->DrawQuad({ 0, 0, 10, 10 }, 255, 0, 0, 255);
}