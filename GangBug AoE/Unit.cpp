#include "Unit.h"
#include "App.h"
#include "M_Animation.h"
#include "M_Render.h"

//TMP
#include "M_Input.h"
#include "App.h"

Unit::Unit(unit_type type, Entity* parent) : unitType(type), Entity(ENTITY_UNIT, parent)
{
	name.assign("unit_");
	action = WALK;
	unitDirection = SOUTH;
	entityTexture = app->animation->GetTexture(CAVALRY_ARCHER);
}

void Unit::OnUpdate(float dt)
{
	iPoint p;
	GB_Rectangle<int> tmp;
	app->animation->GetFrame(tmp, p, this);
	SetEnclosingBoxSize(tmp.w, tmp.h);
	drawQuad = tmp;

	int x, y;
	app->input->GetMousePosition(x, y);
	//mPos += {app->render->camera.x, app->render->camera.y};

	fPoint mPos(x, y);
	fPoint gPos = GetGlobalPosition();
	velocity = mPos - gPos;
	velocity.Normalize();
	velocity *= (speed * dt);

	fPoint p2 = { (float)p.x, (float)p.y };
	SetGlobalPosition(gPos + velocity);
}

unit_type Unit::GetType() const
{
	return unitType;
}

action_type Unit::GetAction() const
{
	return action;
}

direction Unit::GetDirection() const
{
	return unitDirection;
}