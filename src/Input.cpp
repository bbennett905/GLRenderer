#include "Input.h"

#include <SDL.h>
#include <SDL_keycode.h>

#include "Window.h"

namespace Input
{
	namespace
	{
		void (* _input_callback)(const unsigned char *, double, int, int);
	}

	void SetInputHandler(void (* input_callback)(const unsigned char *, double, int, int))
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
					Window::_has_focus = true;
					break;
				case SDL_WINDOWEVENT_FOCUS_LOST:
					Window::_has_focus = false;
					break;
				}
			}
		}

		const Uint8 * keys = SDL_GetKeyboardState(NULL);

		//TODO get buttons, and handle somewhere
		int x, y;
		SDL_GetRelativeMouseState(&x, &y);
		_input_callback(keys, delta_time, x, y);
	}
}