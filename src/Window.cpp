#include "Window.h"
#include <iostream>

#include <SDL.h>
#include <SDL_opengl.h>

Window::Window(int width, int height, const char * title) :
	ShouldExit(false)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL Failed to initialize! Error:%s\n", SDL_GetError());
		return;
	}
	_window = SDL_CreateWindow("OpenGL Testing",
		                       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		                       width, height,
		                       SDL_WINDOW_OPENGL);
	if (_window == nullptr)
	{
		printf("Failed to create SDL window! Error:%s\n", SDL_GetError());
		return;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	_context = SDL_GL_CreateContext(_window);
	if (_context == NULL)
	{
		printf("Failed to create GL Context! Error:%s\n", SDL_GetError());
		return;
	}
	
	SDL_CaptureMouse(SDL_TRUE);
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
	}

	//TODO we could combine callbacks into InputCallback
	const Uint8 * keys = SDL_GetKeyboardState(NULL);
	_key_callback(keys, delta_time);

	//TODO get buttons, and handle somewhere
	int x, y;
	SDL_GetRelativeMouseState(&x, &y);
	_cursor_callback(x, y);
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
