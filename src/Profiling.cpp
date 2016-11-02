#include "Profiling.h"

#include <vector>
#include <thread>

#include "Timing.h"
#include "Logging.h"

namespace Profiling
{
	std::vector<ProfileData *> ProfileSample::_open_samples;

	ProfileSample::ProfileSample(std::string name) :
		_start_time(Timing::GetTime())
	{
		//oh god
		Self = new ProfileData();
		Self->name = name;
		Self->num_parents = _open_samples.size();

		//Check if this sample has a parent
		if (_open_samples.size() >= 1)
		{
			//Set the parent to the last sample in the vector
			Self->parent = _open_samples[_open_samples.size() - 1];
		}
		else
		{
			Self->parent = nullptr;
		}

		_open_samples.push_back(Self);
	}

	ProfileSample::~ProfileSample()
	{
		Self->time = Timing::GetSecondsSince(_start_time);

		//so uh threading this made it way slower oops
		AddProfileData(Self);

		//Find the current sample in the vector and remove it
		for (uint32_t i = 0; i < _open_samples.size(); i++)
		{
			if (_open_samples[i] == Self)
			{
				_open_samples.erase(_open_samples.begin() + i);
				break;
			}
		}
	}


	namespace
	{
		std::vector<ProfileData *> _entries;
		uint64_t _prof_start_time;
		double _prof_full_seconds;
	}

	void ProfInit()
	{
		_prof_start_time = Timing::GetTime();
	}

	void AddProfileData(ProfileData * data)
	{
		//Realistically we'd want this to be fast, and this might not be so fast - 
		//but, its going to be about the same speed for each sample, so it'll be fine
		bool already_exists = false;
		for (uint32_t i = 0; i < _entries.size(); i++)
		{
			if (_entries[i]->name == data->name)
			{
				already_exists = true;
				_entries[i]->time += data->time;
				break;
			}
		}
		if (!already_exists)
		{
			_entries.push_back(data);
		}
	}

	//Recursive function for printing the entries
	void printEntries(int level = 0, ProfileData * parent = nullptr)
	{
		for (auto entry : _entries)
		{
			if (entry->num_parents == level && (parent == nullptr || parent == entry->parent))
			{
				std::string buff;
				for (uint32_t i = 0; i < entry->num_parents; i++) buff += '-';
				buff += entry->name.c_str();
				if (parent != nullptr)
				{
					Logging::LogMessage(LogLevel_Debug, " %06.5f | %06.3f | %s",
						entry->time, 100.0 * (entry->time / parent->time),
						buff.c_str());
				}
				else
				{
					Logging::LogMessage(LogLevel_Debug, " %06.5f | %06.3f | %s",
						entry->time, 100.0 * (entry->time / _prof_full_seconds),
						buff.c_str());
				}
				printEntries(level + 1, entry);
			}
		}
	}

	void LogProfileData()
	{
		_prof_full_seconds = Timing::GetSecondsSince(_prof_start_time);
		Logging::LogMessage(LogLevel_Debug, "-------------------------------");
		Logging::LogMessage(LogLevel_Debug, " Profiling Output:");
		Logging::LogMessage(LogLevel_Debug, " Total Execution Time: %f", _prof_full_seconds);
		Logging::LogMessage(LogLevel_Debug, " Entries:");

		Logging::LogMessage(LogLevel_Debug, "   sec   |   %%    | name");

		printEntries();

		Logging::LogMessage(LogLevel_Debug, "-------------------------------");
	}
}