#pragma once

#include "Input.h"

/*
 * Wrapper class for SDL2 window management
 */

struct SDL_Window;
typedef void * SDL_GLContext;

namespace Window
{
	//Creates a window and GL context with the specified width, height, and title
	void WindowInit(int width, int height, const char* title);
	//Close the window
	void WindowTerm();
	//Swaps the draw buffers and draws to the screen
	void SwapBuffers();

	//Returns window width
	int GetWidth();
	//Returns window height
	int GetHeight();

	//Tell the window if it has focus - should only be used by Input
	void HasFocus(bool val);

	//true if the window should close next frame
	extern bool ShouldExit;
}
