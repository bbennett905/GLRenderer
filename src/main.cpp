//#include <vld.h> //For debugging

#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <SDL_keycode.h>

#include "Texture.h"
#include "Camera.h"
#include "Lights.h"
#include "Material.h"
#include "Window.h"
#include "Cube.h"
#include "Model.h"
#include "Scene.h"
#include "Logging.h"

#undef main //Thanks, SDL!

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 900

#define MOVE_SPEED 0.1f

Camera camera = Camera(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 90.0f, 0.0f), 
	80.0f, float(SCREEN_WIDTH / SCREEN_HEIGHT));
Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Testing");

void KeyCallback(const Uint8 * keys, double delta_time)
{
	float cameraSpeed = 3.0f * float(delta_time);
	if (keys[SDL_SCANCODE_W]) 
		camera.SetPos(camera.GetPos() + (cameraSpeed * camera.GetForward()));
	if (keys[SDL_SCANCODE_A]) 
		camera.SetPos(camera.GetPos() - (cameraSpeed * camera.GetRight()));
	if (keys[SDL_SCANCODE_S]) 
		camera.SetPos(camera.GetPos() - (cameraSpeed * camera.GetForward()));
	if (keys[SDL_SCANCODE_D]) 
		camera.SetPos(camera.GetPos() + (cameraSpeed * camera.GetRight()));
	if (keys[SDL_SCANCODE_ESCAPE]) window.ShouldExit = true;
}

void CursorCallback(int delta_x, int delta_y)
{
	float sensitivity = 0.075f;

	glm::vec3 ang = camera.GetAngles();
	ang.y += float(delta_x) * sensitivity; //Yaw
	ang.x -= float(delta_y) * sensitivity; //Pitch

	if (ang.x > 89.0f)
		ang.x = 89.0f;
	if (ang.x < -89.0f)
		ang.x = -89.0f;

	camera.SetAngles(ang);
}

int main()
{
	Logging::LogInit("log.txt");
	Logging::MinLogLevel = LogLevel_Debug;

	Logging::LogMessage(LogLevel_Info, "Application starting");

	Scene * scene = new Scene(&window, &camera);

	LightDirectional * dirLight = new LightDirectional(glm::vec3(-45.0f, 45.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 3.0f,
		glm::vec3(0.1f, 0.1f, 0.5f), 0.5f);
	LightPoint * pointLight = new LightPoint(glm::vec3(-2.0f, 1.0f, -3.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f), 5.0f);
	LightSpot * spotLight = new LightSpot(glm::vec3(0.0f, 0.0f, -2.0f), 
		glm::vec3(0.0f, 90.0f, 0.0f),
		glm::vec3(0.5f, 0.5f, 1.0f), 4.0f, 12.5f, 20.0f);
	scene->AddObjectToScene(dirLight);
	scene->AddObjectToScene(pointLight);
	scene->AddObjectToScene(spotLight);

	Texture * texture1 = new Texture("../images/container2.png");
	Texture * specMap = new Texture("../images/container2_specular.png");
	Material * mat1 = new Material(texture1, specMap,
		0.5f, 1.0f, 1.0f, 32.0f);

	Model * suit = new Model("../nanosuit/nanosuit.obj");
	scene->AddObjectToScene(suit);
	suit->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
	suit->SetAngles(glm::vec3(0.0f, 180.0f, 0.0f));
	suit->SetPosition(glm::vec3(0.0f, -1.0f, 1.0f));

	Cube * cube = new Cube(mat1);
	cube->Scale = glm::vec3(2.0f, 1.0f, 1.0f);
	cube->Position = glm::vec3(0.0f, 2.0f, 3.0f);
	scene->AddObjectToScene(cube);
	Cube * cube2 = new Cube(mat1);
	cube2->Position = glm::vec3(0.0f, 3.0f, 5.0f);
	scene->AddObjectToScene(cube2);
	Cube * cube3 = new Cube(mat1);
	cube3->Position = glm::vec3(3.0f, 2.0f, 1.0f);
	scene->AddObjectToScene(cube3);
	Cube * cube4 = new Cube(mat1);
	cube4->Position = glm::vec3(-1.5f, 1.5f, 0.0f);
	scene->AddObjectToScene(cube4);

	Texture * glass_diffuse = new Texture("../images/window.png", Texture_Translucent);
	Material * glass_material = new Material(glass_diffuse);
	Cube * glass = new Cube(glass_material);
	glass->Flags |= Drawable_Translucent;
	glass->Scale = glm::vec3(2.0f, 2.0f, 0.1f);
	glass->Position = glm::vec3(0.5f, 0.3f, 0.0f);
	scene->AddObjectToScene(glass);

	Cube * floor = new Cube(mat1);
	floor->Position = glm::vec3(0.0f, -1.0f, 0.0f);
	floor->Scale = glm::vec3(10.0f, 0.01f, 10.0f);
	scene->AddObjectToScene(floor);

	Model * cone = new Model("../cone/cone.obj");
	cone->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	cone->SetPosition(glm::vec3(-0.01f, 2.02f, 1.0f));
	scene->AddObjectToScene(cone);

	scene->PrepareScene();

	//Set where we handle input
	window.SetKeyCallback(KeyCallback);
	window.SetCursorCallback(CursorCallback);

	Uint64 end_time, start_time = SDL_GetPerformanceCounter();
	Uint64 last_print_time = SDL_GetPerformanceCounter();
	int num_frames = 0;

	while (!window.ShouldExit)
	{
		while (SDL_GetPerformanceCounter() <
			start_time + (double(1.0f / 300.0f) * SDL_GetPerformanceFrequency()));

		end_time = SDL_GetPerformanceCounter();
		double delta_time = double(end_time - start_time) / SDL_GetPerformanceFrequency();
		start_time = SDL_GetPerformanceCounter();

		num_frames++;
		double delta_print_time = double(start_time - last_print_time) /
			SDL_GetPerformanceFrequency();
		if (delta_print_time >= 0.5) 
		{
			printf("FPS: %f (%f ms)\n", num_frames / delta_print_time, 
				delta_print_time / num_frames);
			last_print_time = SDL_GetPerformanceCounter();
			num_frames = 0;
		}

		//TODO this rotation is really jittery
		glm::vec3 rotate(GLfloat(40.0f * float(SDL_GetPerformanceCounter()) 
										/ SDL_GetPerformanceFrequency()), 
						 GLfloat(-25.0f * float(SDL_GetPerformanceCounter()) 
										/ SDL_GetPerformanceFrequency()), 0.0f);
		cube->SetAngles(rotate);
		cube2->SetAngles(rotate);
		cube3->SetAngles(rotate);
		cube4->SetAngles(rotate);

		spotLight->Position = camera.GetPos();
		spotLight->SetAngles(camera.GetAngles());

		//Go to the event callbacks specified before
		window.PollEvents(delta_time);

		scene->Update(delta_time);

		window.SwapBuffers();
	}

	delete scene;

	Logging::LogMessage(LogLevel_Info, "Application exiting");
	Logging::LogTerm();

	return 0;
}