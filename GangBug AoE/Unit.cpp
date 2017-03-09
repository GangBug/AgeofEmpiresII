#include "Unit.h"
#include "App.h"
#include "M_Animation.h"
#include "M_Render.h"

Unit::Unit(unit_type type, Entity* parent) : type(type), Entity(parent)
{
	name.assign("unit");
}

void Unit::OnUpdate(float dt)
{
	entityTexture = app->animation->GetTexture(CAVALRY_ARCHER);
	action = WALK;
	unitDirection = SOUTH;
	
}

unit_type Unit::GetType() const
{
	return type;
}

action_type Unit::GetAction() const
{
	return action;
}

direction Unit::GetDirection() const
{
	return unitDirection;
}