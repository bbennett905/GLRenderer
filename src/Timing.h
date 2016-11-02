#pragma once

#include <stdint.h>

namespace Timing
{
	uint64_t GetTime();
	double GetSeconds();
	uint64_t GetTimeSince(uint64_t start_time);
	double GetSecondsSince(uint64_t start_time);
}