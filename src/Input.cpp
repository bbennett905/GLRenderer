#include "Input.h"

#include <SDL.h>
#include <SDL_keycode.h>

#include "Window.h"
#include "Logging.h"

namespace Input
{
	namespace
	{
		void (* _input_callback)(const unsigned char *, double, int, int, int);
		Window * _window;
	}

	void InputInit(Window * window)
	{
		_window = window;
	}

	void SetInputHandler(void (* input_callback)(const unsigned char *, double, int, int, int))
	{
		_input_callback = input_callback;
	}

	void PollEvents(double delta_time)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				_window->ShouldExit = true;
				return;
			}
			if (e.type == SDL_WINDOWEVENT)
			{
				switch (e.window.event)
				{
				case SDL_WINDOWEVENT_FOCUS_GAINED:
					_window->_has_focus = true;
					Logging::LogMessage(LogLevel_Debug, "Window gained focus");
					break;
				case SDL_WINDOWEVENT_FOCUS_LOST:
					_window->_has_focus = false;
					Logging::LogMessage(LogLevel_Debug, "Window lost focus");
					break;
				}
			}
		}

		const Uint8 * keys = SDL_GetKeyboardState(NULL);

		int x, y, buttons;
		buttons = SDL_GetRelativeMouseState(&x, &y);
		_input_callback(keys, delta_time, x, y, buttons);
	}
}