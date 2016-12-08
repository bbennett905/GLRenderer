#include "Input.h"

#include <SDL.h>
#include <SDL_keycode.h>

#include "Window.h"
#include "Logging.h"

namespace Input
{
	namespace
	{
		void (*_input_callback)(const unsigned char*, double, int, int, int);
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
				Window::ShouldExit = true;
				return;
			}
			if (e.type == SDL_WINDOWEVENT)
			{
				switch (e.window.event)
				{
				case SDL_WINDOWEVENT_FOCUS_GAINED:
					Window::HasFocus(true);
					Logging::LogMessage(LogLevel_Debug, "Window gained focus");
					break;
				case SDL_WINDOWEVENT_FOCUS_LOST:
					Window::HasFocus(false);
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