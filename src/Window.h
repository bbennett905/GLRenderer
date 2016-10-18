#pragma once

#include <SDL.h>
#include <SDL_opengl.h>

/*
 * Wrapper class for SDL2 window management
 */

class Window
{
public:
	//Creates a window obj with the specified width, height, and title
	Window(int width, int height, const char * title);
	~Window();

	//Calls the previously specified callbacks
	void PollEvents(double delta_time);

	//Swaps the draw buffers and draws to the screen
	void SwapBuffers();
	
	//Sets the function to call on keypress
	void SetKeyCallback(void (* key_callback)(const Uint8 *, double));
	
	//Sets the function to call on mouse movement
	void SetCursorCallback(void(* cursor_callback)(int, int));

	//Returns window width
	int GetWidth();
	//Returns window height
	int GetHeight();

	bool ShouldExit;
	
private:
	bool _has_focus;
	SDL_Window * _window;
	SDL_GLContext _context;
	int _width;
	int _height;
	void (* _key_callback)(const Uint8 *, double);
	void (* _cursor_callback)(int, int);
};

