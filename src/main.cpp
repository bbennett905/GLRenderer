//#include <vld.h> //For debugging

#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Texture.h"
#include "Camera.h"
#include "Lights.h"
#include "Material.h"
#include "Window.h"
#include "Cube.h"
#include "Model.h"
#include "Scene.h"
#include "Logging.h"
#include "FPS_UIElement.h"
#include "Skybox.h"
#include "Keycodes.h"
#include "Timing.h"
#include "Profiling.h"

//only needed if sdl is included in this file
//#undef main //some sdl weird cross-platform shit, figure it out later

#define SCREEN_WIDTH 1760
#define SCREEN_HEIGHT 990

#define MOVE_SPEED 0.1f

Camera camera = Camera(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 90.0f, 0.0f), 
	80.0f, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT));
Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Testing");

void InputCallback(const uint8_t * keys, double delta_time, int delta_x, int delta_y, int mouse_buttons)
{
	float cameraSpeed = 3.0f * float(delta_time);
	if (keys[KEY_W]) 
		camera.SetPos(camera.GetPos() + (cameraSpeed * camera.GetForward()));
	if (keys[KEY_A]) 
		camera.SetPos(camera.GetPos() - (cameraSpeed * camera.GetRight()));
	if (keys[KEY_S]) 
		camera.SetPos(camera.GetPos() - (cameraSpeed * camera.GetForward()));
	if (keys[KEY_D]) 
		camera.SetPos(camera.GetPos() + (cameraSpeed * camera.GetRight()));
	if (keys[KEY_ESCAPE]) window.ShouldExit = true;

	//Example
	if (mouse_buttons & MOUSE_BUTTON(MOUSE_BUTTON_LEFT)) { }

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
	//TODO render system namespace for initialization and such
	Scene * scene = new Scene(&window, &camera);

	Input::InputInit(&window);

	Logging::LogInit(&window, scene, "log.txt");
	Logging::MinLogLevel = LogLevel_Debug;

	Logging::LogMessage(LogLevel_Info, "-------------------------------------------------------");
	Logging::LogMessage(LogLevel_Info, "Application starting");
	
	Profiling::ProfInit();

	LightDirectional * dirLight = new LightDirectional(glm::vec3(-45.0f, 45.0f, 0.0f),
		glm::vec3(1.0f, 0.95f, 0.75f), 1.0f,
		glm::vec3(0.8f, 0.7f, 0.9f), 0.05f);
	LightPoint * pointLight = new LightPoint(glm::vec3(-2.0f, 1.0f, -3.0f), 
		glm::vec3(0.0f, 1.0f, 0.3f), 1.0f);
	LightPoint * pointLight2 = new LightPoint(glm::vec3(-1.0f, 0.5f, 0.8f),
		glm::vec3(0.6f, 0.0f, 1.0f), 1.0f);
	LightSpot * spotLight = new LightSpot(glm::vec3(0.0f, 0.0f, -2.0f), 
		glm::vec3(0.0f, 90.0f, 0.0f),
		glm::vec3(0.5f, 0.5f, 1.0f), 3.0f, 12.5f, 20.0f);
	scene->AddObjectToScene(dirLight);
	scene->AddObjectToScene(pointLight);
	scene->AddObjectToScene(pointLight2);
	scene->AddObjectToScene(spotLight);

	Skybox * sky = new Skybox("../images/skybox/a");
	scene->AddObjectToScene(sky);

	Model * suit = new Model("../nanosuit/nanosuit.obj");
	scene->AddObjectToScene(suit);
	suit->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
	suit->SetAngles(glm::vec3(0.0f, 180.0f, 0.0f));
	suit->SetPosition(glm::vec3(0.0f, -1.0f, 1.0f));

	Material * crate = new Material("../images/crate.txt");
	Material * marble = new Material("../images/marble.txt");
	Material * wood = new Material("../images/wood.txt");
	Material * metal = new Material("../images/metal.txt");
	Material * glass_material = new Material("../images/window.txt");

	Cube * cube = new Cube(crate);
	cube->Scale = glm::vec3(1.50f, 1.2f, 1.2f);
	cube->Position = glm::vec3(0.0f, 2.0f, 3.0f);
	scene->AddObjectToScene(cube);
	Cube * cube2 = new Cube(marble);
	cube2->Position = glm::vec3(0.0f, 3.0f, 5.0f);
	scene->AddObjectToScene(cube2);
	Cube * cube3 = new Cube(wood);
	cube3->Position = glm::vec3(3.0f, 2.0f, 1.0f);
	scene->AddObjectToScene(cube3);
	Cube * cube4 = new Cube(metal);
	cube4->Position = glm::vec3(-1.5f, 1.5f, 0.0f);
	scene->AddObjectToScene(cube4);

	Cube * glass = new Cube(glass_material);
	glass->Flags |= Drawable_Translucent;
	glass->Scale = glm::vec3(2.0f, 2.0f, 0.1f);
	glass->Position = glm::vec3(1.2f, 0.3f, 0.0f);
	scene->AddObjectToScene(glass);
	Cube * glass2 = new Cube(glass_material);
	glass2->Flags |= Drawable_Translucent;
	glass2->Scale = glm::vec3(2.0f, 2.0f, 0.1f);
	glass2->Position = glm::vec3(1.7f, 0.3f, 1.6f);
	scene->AddObjectToScene(glass2);

	Cube * floor = new Cube(wood);
	floor->Position = glm::vec3(0.0f, -1.0f, 0.0f);
	floor->Scale = glm::vec3(10.0f, 0.01f, 10.0f);
	scene->AddObjectToScene(floor);

	Model * cone = new Model("../cone/cone.obj");
	cone->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	cone->SetPosition(glm::vec3(-0.01f, 2.02f, 1.0f));
	scene->AddObjectToScene(cone);

	FPS_UIElement * fps_element = new FPS_UIElement(&window);
	scene->AddUIElementToScene(fps_element);

	scene->PrepareScene();

	//Set where we handle input
	Input::SetInputHandler(InputCallback);

	uint64_t start_time = Timing::GetTime();
	uint64_t last_print_time = Timing::GetTime();
	int num_frames = 0;

	while (!window.ShouldExit)
	{
		//Cap fps to 300
		while (Timing::GetSecondsSince(start_time) < double(1.0f / 300.0f));

		double delta_time = Timing::GetSecondsSince(start_time);
		start_time = Timing::GetTime();

		num_frames++;
		double delta_print_time = Timing::GetSecondsSince(last_print_time);
		if (delta_print_time >= 0.5) 
		{
			fps_element->Update(delta_print_time / (double)num_frames);
			last_print_time = Timing::GetTime();
			num_frames = 0;
		}

		glm::vec3 delta_rotate(40.0 * delta_time, -25.0f * delta_time, 0.0f);
		//cube->SetAngles(cube->GetAngles() + delta_rotate);
		//cube2->SetAngles(cube2->GetAngles() + delta_rotate);
		//cube3->SetAngles(cube3->GetAngles() + delta_rotate);
		//cube4->SetAngles(cube4->GetAngles() + delta_rotate);

		//Go to the event callbacks specified before
		Input::PollEvents(delta_time);

		spotLight->Position = camera.GetPos();
		spotLight->SetAngles(camera.GetAngles());

		scene->Update(delta_time);
		Logging::LogUpdate(delta_time);

		window.SwapBuffers();
	}

	delete scene;

	Profiling::LogProfileData();

	Logging::LogMessage(LogLevel_Info, "Application exiting");
	Logging::LogTerm();

	return 0;
}
