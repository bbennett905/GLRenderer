#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>

enum LogLevel
{
	LogLevel_Debug,
	LogLevel_Info,
	LogLevel_Warn,
	LogLevel_Error,
	LogLevel_Critical
};

namespace Logging
{
	//Log to the file specified in LogFilePath
	bool bLogToFile;
	//TODO
	bool bLogToScreen;
	//Logs to the console
	bool bLogToConsole;

	//Minimum log level to print log messages for
	LogLevel MinLogLevel;

	namespace
	{
		//File stream object
		std::ofstream _file;

		//Path of the file to store the logs in
		std::string _log_file_path;
	}

	//Initialize logging - must be called before using LogMessage
	void LogInit(std::string path)
	{
		_log_file_path = path;
		_file.open(_log_file_path.c_str(), std::fstream::out | std::fstream::ate | std::fstream::app);

		bLogToFile = true;
		bLogToScreen = false;
		bLogToConsole = true;
		MinLogLevel = LogLevel_Warn;
	}

	//Logs a message
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
	}

	//Called at the end of the application to terminate logging
	void LogTerm()
	{
		_file.close();
	}
}