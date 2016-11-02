#include "Timing.h"

#include <SDL_timer.h>

namespace Timing
{
	uint64_t GetTime()
	{
		return SDL_GetPerformanceCounter();
	}
	double GetSeconds()
	{
		return double(SDL_GetPerformanceCounter()) / SDL_GetPerformanceFrequency();
	}
	uint64_t GetTimeSince(uint64_t start_time)
	{
		return SDL_GetPerformanceCounter() - start_time;
	}
	double GetSecondsSince(uint64_t start_time)
	{
		return double(SDL_GetPerformanceCounter() - start_time) / SDL_GetPerformanceFrequency();
	}
}