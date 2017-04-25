#include "Object.h"
#include "M_EntityManager.h"
#include "App.h"
#include "M_Input.h"
#include "M_Textures.h"
#include "M_Render.h"

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
//  app->render->Blit(app->tex->objectTexture, GetGlobalPosition().x, GetGlobalPosition().y, &app->entityManager->getObjectRect(this->objectType));
}
