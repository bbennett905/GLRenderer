#include "Logging.h"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>

namespace Logging
{
	bool bLogToFile;
	bool bLogToScreen;
	bool bLogToConsole;
	LogLevel MinLogLevel;

	namespace
	{
		//File stream object
		std::ofstream _file;
	}

	void LogMessage(LogLevel level, const char * str, ...)
	{
		if (level < MinLogLevel) return;

		char buffer[1024];
		va_list args;
		va_start(args, str);
		vsnprintf(buffer, 255, str, args);

		if (bLogToFile)
		{
			_file << str << std::endl;
		}

		if (bLogToScreen)
		{
			//TODO do this, after some kind of 2d thing added
		}

		if (bLogToConsole)
		{
			std::cout << buffer << std::endl;
		}

		if (level == LogLevel_Critical)
		{
			//TODO add log to error box for critical errors - SDL error box thing
		}

	}

	void LogInit(const char * path)
	{
		_file.open(path, std::fstream::out | std::fstream::ate | std::fstream::app);

		bLogToFile = true;
		bLogToScreen = false;
		bLogToConsole = true;
		MinLogLevel = LogLevel_Warn;
		LogMessage(LogLevel_Info, "Logging Initialized");
	}

	void LogTerm()
	{
		_file.close();
		LogMessage(LogLevel_Info, "Logging Terminated");
	}
}
