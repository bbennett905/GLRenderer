#pragma once

#include <string>

namespace Profiling
{
	/*
	 * A timed sample - timer starts on creation, ends and submits on destruction
	 */
	class ProfileSample
	{
	public:
		ProfileSample(std::string name);
		~ProfileSample();

	private:
		std::string _name;
		uint64_t _start_time;
	};

	//Initializes profiling
	void ProfInit();

	//Adds an entry of data to the profiler
	void AddProfileData(double time, std::string name);

	//Processes and prints all profile data as LogLevel_Debug messages
	void LogProfileData();
}
