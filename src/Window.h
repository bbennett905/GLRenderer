#pragma once

#include <SDL.h>
#include <SDL_opengl.h>

#include "Input.h"

/*
 * Wrapper class for SDL2 window management
 */

class Window
{
public:
	//Creates a window obj with the specified width, height, and title
	Window(int width, int height, const char * title);
	~Window();

	//Swaps the draw buffers and draws to the screen
	void SwapBuffers();

	//Returns window width
	int GetWidth();
	//Returns window height
	int GetHeight();

	static bool ShouldExit;

	friend void Input::PollEvents(double delta_time);

private:
	static bool _has_focus;
	SDL_Window * _window;
	SDL_GLContext _context;
	int _width;
	int _height;
};

