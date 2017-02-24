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
	startedAt = SDL_GetTicks();
}

// ---------------------------------------------
uint32 Timer::Read() const
{
	return SDL_GetTicks() - startedAt;
}

// ---------------------------------------------
float Timer::ReadSec() const
{
	return float(SDL_GetTicks() - startedAt) / 1000.0f;
}