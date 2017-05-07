#pragma once
#include "Steering.h"
#include "p2Point.h"
#include <vector>

class Steer_Arrive : public Steering
{
public:
	Steer_Arrive();
	~Steer_Arrive();
	void Update(float dt) override;

	std::vector<iPoint> path;
	fPoint origin;
	fPoint destiny;

	float speed = 100;
	fPoint currentVelocity;

	bool arrived = true;


private:
	
};
