#include "Logging.h"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <SDL.h>

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

		switch (level)
		{
			case LogLevel_Critical:
			{
				if (bLogToFile) _file << "CRITICAL ERROR: ";
				if (bLogToConsole) std::cout << "CRITICAL ERROR: ";
				break;
			}
			case LogLevel_Error:
			{
				if (bLogToFile) _file << "ERROR: ";
				if (bLogToConsole) std::cout << "ERROR: ";
				break;
			}
			case LogLevel_Warn:
			{
				if (bLogToFile) _file << "Warning: ";
				if (bLogToConsole) std::cout << "Warning: ";
				break;
			}
			case LogLevel_Info:
			{
				if (bLogToFile) _file << "Info: ";
				if (bLogToConsole) std::cout << "Info: ";
				break;
			}
			case LogLevel_Debug:
			{
				if (bLogToFile) _file << "Debug: ";
				if (bLogToConsole) std::cout << "Debug: ";
				break;
			}
		}

		if (bLogToFile)
		{
			_file << buffer << std::endl;
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
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Critical Error", buffer, NULL);
			exit(0);
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
