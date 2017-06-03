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
	paused = false;
	startedAt = SDL_GetTicks();
}

// ---------------------------------------------
uint Timer::Read() const
{
	if (stopped)
	{
		return 0;
	}
	else if (paused)
	{
		return pausedAt - startedAt;
	}
	else
	{
		return SDL_GetTicks() - startedAt;
	}
}

// ---------------------------------------------
float Timer::ReadSec() const
{
	if (stopped)
	{
		return 0;
	}
	else if (paused)
	{
		return float(pausedAt - startedAt) / 1000.0f;
	}
	else
	{
		return float(SDL_GetTicks() - startedAt) / 1000.0f;
	}
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

void Timer::Pause()
{
	pausedAt = SDL_GetTicks();
	paused = true;
}

void Timer::UnPause()
{
	startedAt = SDL_GetTicks() - (pausedAt - startedAt);
	paused = false;
}