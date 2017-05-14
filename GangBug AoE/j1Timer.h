#ifndef __TIMER_H__
#define __TIMER_H__

#include "Defs.h"

class Timer
{
public:
	// Constructor
	Timer();

	void Start();
	void Stop();

	uint Read() const;

	float ReadSec() const;

	uint GetStartedAt() const;

	void SetStartedAt(uint);

	void SetSec(int sec);

private:
	uint	startedAt;

	bool stopped;
};

#endif //__TIMER_H__