#include "Window.h"
#include <iostream>

Window::Window(int width, int height, const char * title)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	//this sets the opengl version to use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	_window = glfwCreateWindow(width, height, title, nullptr, nullptr);

	if (_window == nullptr)
	{
		std::cout << "Window creation failed!" << std::endl;
	}
	glfwMakeContextCurrent(_window);
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

void Window::GetFramebufferSize(int * width, int * height)
{
	glfwGetFramebufferSize(_window, width, height);//width, height);
}

void Window::SetKeyCallback(void (* keyCallback))
{
	glfwSetKeyCallback(_window, GLFWkeyfun(keyCallback));
}
