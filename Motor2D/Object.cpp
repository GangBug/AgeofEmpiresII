#include "Object.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1EntityManager.h"


Object::Object(OBJECT_TYPE oType, fPoint pos) : Entity(OBJECT, pos)
{
	type = oType;
}

Object::~Object()
{
}

void Object::Update()
{
	App->render->Blit(App->tex->objectsTex,GetX(), GetY(), &App->entity_manager->getObjectRect(type));
}