#include "Entity.h"

Entity::Entity(ENTITY_TYPE entity_type, fPoint pos): to_delete (false), entity_type(entity_type), position(pos)
{}

Entity::~Entity()
{
}

void Entity::Die()
{
	to_delete = true;
}

bool Entity::ToDelete() const
{
	return to_delete;
}

ENTITY_TYPE Entity::GetEntityType() const
{
	return entity_type;
}

ENTITY_STATUS Entity::GetEntityStatus()
{

	return entity_status;
}

void Entity::SetEntityStatus(ENTITY_STATUS status)
{
	entity_status = status;
}

void Entity::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

int Entity::GetHP() const
{
	return hp;
}
const float Entity::GetX() const
{
	return position.x;
}

const float Entity::GetY() const
{
	return position.y;
}

void Entity::SetArmor(int new_armor)
{
	armor = new_armor;
}

void Entity::SetHp(int new_hp)
{
	hp = new_hp;
}
