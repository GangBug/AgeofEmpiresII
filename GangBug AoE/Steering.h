#pragma once

#include "Entity.h"

enum class SteerType
{
	ARRIVE,
	SEPARATION
};

class Steering
{
public:	
	virtual void Update(float dt);


	Entity* gameObject;
	SteerType type;
};
