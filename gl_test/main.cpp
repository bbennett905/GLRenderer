#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "LightSimple.h"
#include "Material.h"
#include "Window.h"
#include "Cube.h"
#include "Renderer.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

Camera camera = Camera(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 90.0f, 0.0f), 80.0f, float(SCREEN_WIDTH / SCREEN_HEIGHT));

void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main()
{
	LightSimple light(glm::vec3(2.0f, 1.0f, -3.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 2.0f,
		glm::vec3(0.3f, 0.6f, 0.7f), 0.5f);


	Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Testing");

	Renderer render(window, &camera, light);
	render.ClearColor = glm::vec4(0.0f, 1.0f, 0.8f, 1.0f);

	Texture texture1("../images/container.jpg");
	Texture texture2("../images/awesomeface.png");

	Shader newShader("../shaders/test.vert", "../shaders/test.frag");

	Material mat1(&newShader, &texture1, &texture2, 0.4f,
		0.5f, 1.0f, 1.0f, 32.0f);

	Cube * cube = new Cube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), mat1);
	render.AddToDrawList(cube);

	//Set where we handle input
	window.SetKeyCallback(KeyCallback);

	double lastTime = glfwGetTime();
	int numFrames = 0;
	while (!window.ShouldClose())
	{
		double currentTime = glfwGetTime();
		numFrames++;
		if (currentTime - lastTime >= 0.2) 
		{
			double timePerFrame = 200.0 / double(numFrames);
			printf("FPS: %f (%f ms)\n", numFrames / timePerFrame, timePerFrame);
			numFrames = 0;
			lastTime = glfwGetTime();
		}

		glm::vec3 rotate(GLfloat(glfwGetTime() * -40.0f), GLfloat(glfwGetTime() * 25.0f), 0.0f);
		cube->SetAngles(rotate);

		//Go to the event callbacks specified before
		window.PollEvents();

		render.Draw();

		window.SwapBuffers();
	}

	return 0;
}