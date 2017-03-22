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
	entityTexture = app->animation->GetTexture(unitType);
}

void Unit::OnUpdate(float dt)
{
	int x, y;
	app->input->GetMousePosition(x, y);
	fPoint mPos(x, y);

	fPoint gPos = GetGlobalPosition();
	velocity = mPos - gPos;
	velocity.Normalize();
	velocity *= (speed * dt);

	direction dir = GetDirectionFromVelocity(velocity);
	if (dir != unitDirection)
		unitDirection = dir;

	iPoint p;
	GB_Rectangle<int> tmp;
	app->animation->GetFrame(tmp, p, this);
	SetEnclosingBoxSize(tmp.w, tmp.h);
	drawQuad = tmp;

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

direction Unit::GetDirectionFromVelocity(fPoint vel)
{
	direction ret = NORTH;

	float angle = vel.GetAngleDeg();

	if (angle < 22.5 && angle > -22.5)
		ret = SOUTH;//
	else if (angle >= 22.5 && angle <= 67.5)
		ret = SOUTH_EAST;
	else if (angle > 67.5 && angle < 112.5)
		ret = EAST;
	else if (angle >= 112.5 && angle <= 157.5)
		ret = NORTH_EAST;
	else if (angle > 157.5 || angle < -157.5)
		ret = NORTH;//
	else if (angle >= -157.5 && angle <= -112.5)
		ret = NORTH_WEST;
	else if (angle > -112.5 && angle < -67.5)
		ret = WEST;//
	else if (angle >= -67.5 && angle <= -22.5)
		ret = SOUTH_WEST;//

	//NOTE: SOUTH_EAST, EAST, NORTH_EAST & NORTH_WEST are missing

	return ret;
}