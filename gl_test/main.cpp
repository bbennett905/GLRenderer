#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Lights.h"
#include "Material.h"
#include "Window.h"
#include "Cube.h"
#include "Renderer.h"
#include "Mesh.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define MOVE_SPEED 0.1f

Camera camera = Camera(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 90.0f, 0.0f), 80.0f, float(SCREEN_WIDTH / SCREEN_HEIGHT));
Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Testing");

bool keys[1024];
void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		keys[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		keys[key] = false;
	}
}

double lastX, lastY;
bool firstMouse = true;
void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left
	lastX = xpos;
	lastY = ypos;

	double sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	glm::vec3 ang = camera.GetAngles();
	ang.y += float(xoffset); //Yaw
	ang.x += float(yoffset); //Pitch

	if (ang.x > 89.0f)
		ang.x = 89.0f;
	if (ang.x < -89.0f)
		ang.x = -89.0f;
	
	camera.SetAngles(ang);
}

void HandleMovement(double deltaTime)
{
	float cameraSpeed = 0.1f * float(deltaTime);
	if (keys[GLFW_KEY_W])
		camera.SetPos(camera.GetPos() + (cameraSpeed * camera.GetForward()));
	if (keys[GLFW_KEY_S])
		camera.SetPos(camera.GetPos() - (cameraSpeed * camera.GetForward()));
	if (keys[GLFW_KEY_A])
		camera.SetPos(camera.GetPos() - (cameraSpeed * camera.GetRight()));
	if (keys[GLFW_KEY_D])
		camera.SetPos(camera.GetPos() + (cameraSpeed * camera.GetRight()));
	if (keys[GLFW_KEY_ESCAPE])
		window.SetShouldClose(true);
}

int main()
{
	LightDirectional dirLight(glm::vec3(-45.0f, 45.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 3.0f,
		glm::vec3(0.1f, 0.1f, 0.5f), 0.5f);
	LightPoint pointLight(glm::vec3(-2.0f, 1.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 5.0f);
	LightSpot spotLight(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 90.0f, 0.0f),
		glm::vec3(0.5f, 0.5f, 1.0f), 4.0f, 12.5f, 20.0f);

	Renderer render(window, &camera);
	render.ClearColor = glm::vec4(0.0f, 0.05f, 0.1f, 1.0f);
	render.SetDirLight(&dirLight);
	render.AddPointLight(&pointLight);
	render.AddSpotLight(&spotLight);

	Texture texture1("../images/container2.png");
	//Texture texture2("../images/awesomeface.png");
	Texture specMap("../images/container2_specular.png");

	Shader newShader("../shaders/default.vert", "../shaders/default.frag");

	Material mat1(&newShader, &texture1, nullptr, 0.4f, &specMap,
		0.5f, 1.0f, 1.0f, 32.0f);

	Cube * cube = new Cube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), mat1);
	cube->Scale = glm::vec3(2.0f, 1.0f, 1.0f);
	render.AddToDrawList(cube);
	Cube * cube2 = new Cube(glm::vec3(0.0f, 3.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), mat1);
	render.AddToDrawList(cube2);
	Cube * cube3 = new Cube(glm::vec3(3.0f, 2.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), mat1);
	render.AddToDrawList(cube3);
	Cube * cube4 = new Cube(glm::vec3(-1.5f, -1.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), mat1);
	render.AddToDrawList(cube4);

	Model * suit = new Model("../nanosuit/nanosuit.obj");
	render.AddToDrawList(suit);

	//Set where we handle input
	window.SetKeyCallback(KeyCallback);
	window.SetCursorCallback(MouseCallback);

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

		spotLight.Position = camera.GetPos();
		spotLight.SetAngles(camera.GetAngles());

		//Go to the event callbacks specified before
		window.PollEvents();
		HandleMovement(deltaTime);

		render.Draw();

		window.SwapBuffers();
	}

	return 0;
}