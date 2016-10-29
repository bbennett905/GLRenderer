#include "Window.h"

#include <iostream>

#include "Logging.h"

Window::Window(int width, int height, const char * title) :
	_width(width), _height(height)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		Logging::LogMessage(LogLevel_Critical, 
			"SDL Failed to initialize! Error:%s\n", SDL_GetError());
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
		Logging::LogMessage(LogLevel_Critical, 
			"Failed to create SDL window! Error:%s\n", SDL_GetError());
		return;
	}

	_context = SDL_GL_CreateContext(_window);
	if (_context == NULL)
	{
		Logging::LogMessage(LogLevel_Critical, 
			"Failed to create GL Context! Error:%s\n", SDL_GetError());
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

void Window::SwapBuffers()
{
	//Do this here because its called every frame
	if (_has_focus)
		SDL_WarpMouseInWindow(_window, _width / 2, _height / 2);

	SDL_GL_SwapWindow(_window);
}

int Window::GetWidth()
{
	return _width;
}

int Window::GetHeight()
{
	return _height;
}
