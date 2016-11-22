#pragma once

class CWindow;
class CScene;

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
	extern bool bLogToFile;
	//Prints log messages to the screen
	extern bool bLogToScreen;
	//Logs to the console
	extern bool bLogToConsole;

	//Minimum log level to print log messages for
	extern LogLevel MinLogLevel;

	//Logs a message
	void LogMessage(LogLevel level, const char * str, ...);

	//Initialize logging - must be called before using LogMessage
	void LogInit(CWindow * window, CScene * scene, const char * path);

	//Updates the on-screen log
	void LogUpdate(double delta_time);

	//Called at the end of the application to terminate logging
	void LogTerm();
}
