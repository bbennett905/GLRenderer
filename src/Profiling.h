#pragma once

#include <string>
#include <vector>

namespace Profiling
{
	/*
	 * Holds data collected from a ProfileSample
	 */
	struct ProfileData
	{
		//Time, in seconds
		double time;
		//Name of this profile
		std::string name;
		//Pointer to the parent of this sample, or nullptr if there is none
		ProfileData * parent;
		//Number of parents of this sample
		uint32_t num_parents;
	};

	/*
	 * A timed sample - timer starts on creation, ends and submits on destruction
	 */
	class ProfileSample
	{
	public:
		ProfileSample(std::string name);
		~ProfileSample();
		ProfileData * Self;

	private:
		static std::vector<ProfileData *> _open_samples;
		uint64_t _start_time;
	};

	//Initializes profiling
	void ProfInit();

	//Adds an entry of data to the profiler
	void AddProfileData(ProfileData * data);

	//Processes and prints all profile data as LogLevel_Debug messages
	void LogProfileData();
}
