#pragma once

/*
 * Wrapper class for GLFW window management
 */

class SDL_Window;

class Window
{
public:
	//Creates a window obj with the specified width, height, and title
	Window(int width, int height, const char * title);
	~Window();

	//Sets if the window should close next frame
	void SetShouldClose(bool val);

	//Gets if the window should close next frame
	bool ShouldClose();

	//Calls the previously specified callbacks
	void PollEvents();

	//Swaps the draw buffers and draws to the screen
	void SwapBuffers();
	
	//TODO replace with own callback
	//Sets the function to call on keypress
	void SetKeyCallback(void (* keyCallback));
	
	//Sets the function to call on mouse movement
	void SetCursorCallback(void(* cursorCallback));

	//Returns window width
	int GetWidth();
	//Returns window height
	int GetHeight();
	
private:
	//GLFWwindow * _window;
	SDL_Window * _window;
	int _width;
	int _height;
};

