#include "Object.h"
#include "M_EntityManager.h"
#include "App.h"
#include "M_Input.h"
#include "M_Textures.h"

Object::Object(object_type objectType, Entity * parent) : Entity(ENTITY_OBJECT, parent), objectType(objectType)
{
	entityTexture = app->tex->objectTexture;
	app->entityManager->getObjectRect(objectType);
}

Object::~Object()
{

}

void Object::OnUpdate(float dt)
{
}
