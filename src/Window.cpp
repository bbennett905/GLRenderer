#include "Window.h"

#include <iostream>

#include "Logging.h"

Window::Window(int width, int height, const char * title) :
	ShouldExit(false), _width(width), _height(height)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		Logging::LogMessage(LogLevel_Critical, "SDL Failed to initialize! Error:%s\n", SDL_GetError());
		return;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	_window = SDL_CreateWindow("OpenGL Testing",
		                       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		                       width, height,
		                       SDL_WINDOW_OPENGL | SDL_WINDOW_MOUSE_CAPTURE | 
		                       SDL_WINDOW_BORDERLESS | SDL_WINDOW_INPUT_GRABBED);
	if (_window == nullptr)
	{
		Logging::LogMessage(LogLevel_Critical, "Failed to create SDL window! Error:%s\n", SDL_GetError());
		return;
	}

	_context = SDL_GL_CreateContext(_window);
	if (_context == NULL)
	{
		Logging::LogMessage(LogLevel_Critical, "Failed to create GL Context! Error:%s\n", SDL_GetError());
		return;
	}
	
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

Window::~Window()
{
	SDL_GL_DeleteContext(_context);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void Window::PollEvents(double delta_time)
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			ShouldExit = true;
			return;
		}
		if (e.type == SDL_WINDOWEVENT)
		{
			switch (e.window.event)
			{
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				_has_focus = true;
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				_has_focus = false;
				break;
			}
		}
	}

	//TODO we could combine callbacks into InputCallback
	const Uint8 * keys = SDL_GetKeyboardState(NULL);
	_key_callback(keys, delta_time);

	//TODO get buttons, and handle somewhere
	int x, y;
	SDL_GetRelativeMouseState(&x, &y);
	_cursor_callback(x, y);

	if (_has_focus)
		SDL_WarpMouseInWindow(_window, _width / 2, _height / 2);
}

void Window::SwapBuffers()
{
	SDL_GL_SwapWindow(_window);
}

void Window::SetKeyCallback(void (* key_callback)(const Uint8 *, double))
{
	_key_callback = key_callback;
}

void Window::SetCursorCallback(void (* cursor_callback)(int, int))
{
	_cursor_callback = cursor_callback;
}

int Window::GetWidth()
{
	return _width;
}

int Window::GetHeight()
{
	return _height;
}
