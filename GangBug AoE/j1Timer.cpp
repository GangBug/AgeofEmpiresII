// ----------------------------------------------------
// j1Timer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "j1Timer.h"
#include "SDL\include\SDL_timer.h"

// ---------------------------------------------
Timer::Timer()
{
	Start();
}

// ---------------------------------------------
void Timer::Start()
{
	stopped = false;
	startedAt = SDL_GetTicks();
}

// ---------------------------------------------
uint Timer::Read() const
{
	return stopped ? 0 : SDL_GetTicks() - startedAt;
}

// ---------------------------------------------
float Timer::ReadSec() const
{
	return stopped ? 0 : float(SDL_GetTicks() - startedAt) / 1000.0f;
}

void Timer::SetSec(int sec)
{
	startedAt = -((sec * 1000.0f) - SDL_GetTicks());
}

uint Timer::GetStartedAt() const
{
	return startedAt;
}

void Timer::SetStartedAt(uint at)
{
	startedAt = at + SDL_GetTicks();
}


void Timer::Stop()
{
	stopped = true;
}