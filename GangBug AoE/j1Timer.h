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
	void Pause();
	void UnPause();

	uint Read() const;

	float ReadSec() const;

	uint GetStartedAt() const;

	void SetStartedAt(uint);

	void SetSec(int sec);

private:
	uint	startedAt;
	uint	pausedAt;

	bool stopped;
	bool paused;
};

#endif //__TIMER_H__