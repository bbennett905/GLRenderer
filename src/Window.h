#pragma once

#include "Input.h"

/*
 * Wrapper class for SDL2 window management
 */

struct SDL_Window;
typedef void * SDL_GLContext;

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

	bool ShouldExit;

	friend void Input::PollEvents(double delta_time);

private:
	bool _has_focus;
	SDL_Window * _window;
	SDL_GLContext _context;
	int _width;
	int _height;
};

