#include "Object.h"
#include "j1App.h"
#include "j1Render.h"


Object::Object(OBJECT_TYPE oType, fPoint pos) : Entity(OBJECT, pos)
{
	type = oType;
}

Object::~Object()
{
}

void Object::Update()
{
	App->render->SpriteOrdering(this);
}