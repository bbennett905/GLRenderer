#include "Profiling.h"

#include <vector>
#include <thread>

#include "Timing.h"
#include "Logging.h"

namespace Profiling
{
	ProfileSample::ProfileSample(std::string name) :
		_name(name)
	{
		_start_time = Timing::GetTime();
	}

	ProfileSample::~ProfileSample()
	{
		//so uh threading this made it way slower oops
		AddProfileData(Timing::GetSecondsSince(_start_time), _name);
	}

	namespace
	{
		std::vector<std::tuple<double, std::string>> _entries;
		uint64_t _prof_start_time;
	}

	void ProfInit()
	{
		_prof_start_time = Timing::GetTime();
	}

	void AddProfileData(double seconds, std::string name)
	{
		//Realistically we'd want this to be fast, and this might not be so fast - 
		//but, its going to be about the same speed for each sample, so it'll be fine
		bool already_exists = false;
		for (uint32_t i = 0; i < _entries.size(); i++)
		{
			if (std::get<1>(_entries[i]) == name)
			{
				already_exists = true;
				std::get<0>(_entries[i]) += seconds;
				break;
			}
		}
		if (!already_exists)
		{
			_entries.push_back(std::tuple<double, std::string>(seconds, name));
		}
	}

	void LogProfileData()
	{
		double prof_end_seconds = Timing::GetSecondsSince(_prof_start_time);
		Logging::LogMessage(LogLevel_Debug, "-------------------------------");
		Logging::LogMessage(LogLevel_Debug, " Profiling Output:");
		Logging::LogMessage(LogLevel_Debug, " Total Execution Time: %f", prof_end_seconds);
		Logging::LogMessage(LogLevel_Debug, " Entries:");
		double sample_total = 0.0;
		for (auto entry : _entries)
		{
			sample_total += std::get<0>(entry);
		}
		Logging::LogMessage(LogLevel_Debug, "   sec   |   %%    | name");
		for (auto entry : _entries)
		{
			Logging::LogMessage(LogLevel_Debug, " %06.5f | %06.3f | %s", 
				std::get<0>(entry), 100.0 * (std::get<0>(entry) / sample_total), 
				std::get<1>(entry).c_str());
		}
		Logging::LogMessage(LogLevel_Debug, "-------------------------------");
	}
}