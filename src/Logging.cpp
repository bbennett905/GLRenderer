#include "Logging.h"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <deque>
#include <tuple>
#include <SDL_video.h>
#include <SDL_ttf.h>

#include "Window.h"
#include "Texture.h"
#include "BaseUIElement.h"
#include "Scene.h"
#include "Material.h"

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

		class CLogUI :
			public CBaseUIElement
		{
		public:
			CLogUI() :
				CBaseUIElement()
			{
				_font = TTF_OpenFont("C:/Windows/Fonts/Arial.ttf", 14);
				SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(_font, "N/A", { 200, 200, 200 }, 500);

				if (_materials[0]->DiffuseMap)
					delete _materials[0]->DiffuseMap;
				_materials[0]->DiffuseMap = new CTexture(surface, Texture_Translucent);
				autoPosition(surface->w, surface->h, 0, 40);
				SDL_FreeSurface(surface);
			}

			~CLogUI()
			{
				TTF_CloseFont(_font);
			}

			void AddLogMessage(std::string message, double time = 5.0)
			{
				_log_queue.push_back(std::tuple<double, std::string>(time, message));
				_has_changed = true;
			}

			void Update(double delta_time)
			{
				//Don't do anything if theres nothing to display
				if (_log_queue.size() == 0) return;

				while (_log_queue.size() > 5) //Show only 5 entries at a time
				{
					_log_queue.pop_front();
					_has_changed = true;
				}

				std::string buffer;
				for (uint32_t i = 0; i < _log_queue.size(); i++)
				{
					//Update the time and remove old elements
					std::get<0>(_log_queue[i]) -= delta_time;
					if (std::get<0>(_log_queue[i]) <= 0.0)
					{
						_log_queue.erase(_log_queue.begin() + i);
						_has_changed = true;
						continue;
					}

					buffer += std::get<1>(_log_queue[i]) + "\n";
				}

				if (!_has_changed) return;

				SDL_Surface* surface;
				if (buffer.length() > 0)
				{
					surface = TTF_RenderText_Blended_Wrapped(_font, buffer.c_str(),
					{ 200, 200, 200 }, 500);

					autoPosition(surface->w, surface->h, 0, 40);
					_materials[0]->DiffuseMap->Update(surface);
				}
				else
				{
					surface = SDL_CreateRGBSurface(0, 1, 1, 8, 0, 0, 0, 0);
					_materials[0]->DiffuseMap->Update(surface);
				}
				SDL_FreeSurface(surface);
				_has_changed = false;
			}

		private:
			bool _has_changed;
			std::deque<std::tuple<double, std::string>> _log_queue;
			TTF_Font * _font;
		};

		CLogUI * _log_ui_element;
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
			_log_ui_element->AddLogMessage(buffer);
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

	void LogInit(CScene * scene, const char * path)
	{
		_log_ui_element = new CLogUI();

		scene->AddUIElementToScene(_log_ui_element);

		_file.open(path, std::fstream::out | std::fstream::ate | std::fstream::app);

		bLogToFile = true;
		bLogToScreen = true;
		bLogToConsole = true;
		MinLogLevel = LogLevel_Warn;
		LogMessage(LogLevel_Info, "Logging Initialized");
	}

	void LogUpdate(double delta_time)
	{
		if (bLogToScreen)
			_log_ui_element->Update(delta_time);
	}

	void LogTerm()
	{
		LogMessage(LogLevel_Info, "Logging Terminated");
		_file.close();
		delete _log_ui_element;
	}
}
