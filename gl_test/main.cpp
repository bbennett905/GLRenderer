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

#define MOVE_SPEED 0.1f

Camera camera = Camera(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 90.0f, 0.0f), 80.0f, float(SCREEN_WIDTH / SCREEN_HEIGHT));

void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		glm::vec3 vec;
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_W:
			vec = camera.GetForward() * MOVE_SPEED;
			camera.SetPos(camera.GetPos() + vec);
			break;
		case GLFW_KEY_A:
			vec = camera.GetRight() * MOVE_SPEED;
			camera.SetPos(camera.GetPos() - vec);
			break;
		case GLFW_KEY_S:
			vec = camera.GetForward() * MOVE_SPEED;
			camera.SetPos(camera.GetPos() - vec);
			break;
		case GLFW_KEY_D:
			vec = camera.GetRight() * MOVE_SPEED;
			camera.SetPos(camera.GetPos() + vec);
			break;
		}
	}
}

int main()
{
	LightSimple light(glm::vec3(-2.0f, 1.0f, -3.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 2.0f,
		glm::vec3(0.3f, 0.6f, 0.7f), 0.5f);

	Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Testing");

	Renderer render(window, &camera, light);
	render.ClearColor = glm::vec4(0.0f, 0.05f, 0.1f, 1.0f);

	Texture texture1("../images/container2.png");
	//Texture texture2("../images/awesomeface.png");
	Texture specMap("../images/container2_specular.png");

	Shader newShader("../shaders/test.vert", "../shaders/test.frag");

	Material mat1(&newShader, &texture1, nullptr, 0.4f, &specMap,
		0.5f, 1.0f, 1.0f, 32.0f);

	Cube * cube = new Cube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), mat1);
	render.AddToDrawList(cube);
	Cube * cube2 = new Cube(glm::vec3(0.0f, 3.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), mat1);
	render.AddToDrawList(cube2);
	Cube * cube3 = new Cube(glm::vec3(3.0f, 2.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), mat1);
	render.AddToDrawList(cube3);
	Cube * cube4 = new Cube(glm::vec3(-1.5f, -1.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), mat1);
	render.AddToDrawList(cube4);

	//Set where we handle input
	window.SetKeyCallback(KeyCallback);

	double lastTime = glfwGetTime();
	int numFrames = 0;
	while (!window.ShouldClose())
	{
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;
		numFrames++;
		if (deltaTime >= 0.2) 
		{
			double timePerFrame = deltaTime / double(numFrames);
			printf("FPS: %f (%f ms)\n", 1.0 / timePerFrame, timePerFrame * 1000.0);
			numFrames = 0;
			lastTime = glfwGetTime();
		}

		glm::vec3 rotate(GLfloat(glfwGetTime() * -40.0f), GLfloat(glfwGetTime() * 25.0f), 0.0f);
		cube->SetAngles(rotate);
		cube2->SetAngles(rotate);
		cube3->SetAngles(rotate);
		cube4->SetAngles(rotate);

		//Go to the event callbacks specified before
		window.PollEvents();

		render.Draw();

		window.SwapBuffers();
	}

	return 0;
}