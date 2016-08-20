#pragma once

#include <glfw3.h>

class Window
{
public:
	Window(int width, int height, const char * title);
	~Window();

	void SetShouldClose(bool val);
	bool ShouldClose();
	void PollEvents();
	void SwapBuffers();
	
	//TODO replace with own callback
	void SetKeyCallback(void (* keyCallback));

	int GetWidth();
	int GetHeight();
	
private:
	GLFWwindow * _window;
	int _width;
	int _height;
};

