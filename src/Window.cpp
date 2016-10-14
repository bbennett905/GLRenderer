#include "Window.h"
#include <iostream>

#include <SDL.h>
#include <SDL_opengl.h>

Window::Window(int width, int height, const char * title)
{
	glfwInit();
	//TODO take this as an arg? or windowcreateinfo struct, with all these things
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	_window = glfwCreateWindow(width, height, title, nullptr, nullptr);

	if (_window == nullptr)
	{
		std::cout << "Window creation failed!" << std::endl;
	}
	glfwMakeContextCurrent(_window);
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwGetFramebufferSize(_window, &_width, &_height);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL Failed to initialize! Error:%s\n", SDL_GetError());
		return;
	}
	_window = SDL_CreateWindow("OpenGL Testing",
		                       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		                       width, height);
	if (_window == nullptr)
	{
		printf("Failed to create SDL window! Error:%s\n", SDL_GetError());
		return;
	}

}

Window::~Window()
{
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void Window::SetShouldClose(bool val)
{
	glfwSetWindowShouldClose(_window, int(val));
}

bool Window::ShouldClose()
{
	return 1 == glfwWindowShouldClose(_window);
}

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(_window);
}

void Window::SetKeyCallback(void (* keyCallback))
{
	glfwSetKeyCallback(_window, GLFWkeyfun(keyCallback));
}

void Window::SetCursorCallback(void (* cursorCallback))
{
	glfwSetCursorPosCallback(_window, GLFWcursorposfun(cursorCallback));
}

int Window::GetWidth()
{
	return _width;
}

int Window::GetHeight()
{
	return _height;
}
