#include "Steer_Arrive.h"


Steer_Arrive::Steer_Arrive() : Steering()
{
	type = SteerType::ARRIVE;
}

Steer_Arrive::~Steer_Arrive()
{
}

void Steer_Arrive::Update(float dt)
{
	if (!arrived)
	{
		currentVelocity = destiny - origin;
		currentVelocity.Normalize();
		currentVelocity *= speed;
		gameObject->SetGlobalPosition(gameObject->GetGlobalPosition() + currentVelocity * dt );
	}
	
}
