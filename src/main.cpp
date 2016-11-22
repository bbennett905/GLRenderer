//#include <vld.h> //For debugging

#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "CTexture.h"
#include "CCamera.h"
#include "Lights.h"
#include "CMaterial.h"
#include "Window.h"
#include "CCube.h"
#include "CModel.h"
#include "Scene.h"
#include "Logging.h"
#include "CFPSCounter.h"
#include "CSkybox.h"
#include "Keycodes.h"
#include "Timing.h"
#include "Profiling.h"

//only needed if sdl is included in this file
//#undef main //some sdl weird cross-platform shit, figure it out later

#define SCREEN_WIDTH 1760
#define SCREEN_HEIGHT 990

#define MOVE_SPEED 0.1f

CCamera camera = CCamera(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 90.0f, 0.0f), 
	80.0f, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT));
Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Testing");

void InputCallback(const uint8_t * keys, double delta_time, int delta_x, int delta_y, int mouse_buttons)
{
	float cameraSpeed = 3.0f * float(delta_time);
	if (keys[KEY_W]) 
		camera.SetPosition(camera.GetPosition() + (cameraSpeed * camera.GetForward()));
	if (keys[KEY_A]) 
		camera.SetPosition(camera.GetPosition() - (cameraSpeed * camera.GetRight()));
	if (keys[KEY_S]) 
		camera.SetPosition(camera.GetPosition() - (cameraSpeed * camera.GetForward()));
	if (keys[KEY_D]) 
		camera.SetPosition(camera.GetPosition() + (cameraSpeed * camera.GetRight()));
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

	CLightDirectional * dirLight = new CLightDirectional(glm::vec3(-45.0f, 45.0f, 0.0f),
		glm::vec3(1.0f, 0.95f, 0.75f), 0.0f,
		glm::vec3(0.8f, 0.7f, 0.9f), 0.05f);
	CLightPoint * pointLight = new CLightPoint(glm::vec3(-2.0f, 1.0f, -3.0f), 
		glm::vec3(0.0f, 1.0f, 0.3f), 1.0f);
	CLightPoint * pointLight2 = new CLightPoint(glm::vec3(-1.0f, 0.5f, 0.8f),
		glm::vec3(0.6f, 0.0f, 1.0f), 1.0f);
	CLightSpot * spotLight = new CLightSpot(glm::vec3(0.0f, 0.0f, -2.0f), 
		glm::vec3(0.0f, 90.0f, 0.0f),
		glm::vec3(0.5f, 0.5f, 1.0f), 3.0f, 12.5f, 20.0f);
	scene->AddObjectToScene(dirLight);
	scene->AddObjectToScene(pointLight);
	scene->AddObjectToScene(pointLight2);
	scene->AddObjectToScene(spotLight);

	CSkybox * sky = new CSkybox("../images/skybox/a");
	scene->AddObjectToScene(sky);

	CModel * suit = new CModel("../nanosuit/nanosuit.obj");
	scene->AddObjectToScene(suit);
	suit->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
	suit->SetAngles(glm::vec3(0.0f, 180.0f, 0.0f));
	suit->SetPosition(glm::vec3(0.0f, -1.0f, 1.0f));

	CMaterial * crate = new CMaterial("../images/crate.txt");
	CMaterial * marble = new CMaterial("../images/marble.txt");
	CMaterial * wood = new CMaterial("../images/wood.txt");
	CMaterial * metal = new CMaterial("../images/metal.txt");
	CMaterial * glass_material = new CMaterial("../images/window.txt");

	CCube * cube = new CCube(crate);
	cube->SetScale(glm::vec3(1.50f, 1.2f, 1.2f));
	cube->SetPosition(glm::vec3(0.0f, 2.0f, 3.0f));
	scene->AddObjectToScene(cube);
	CCube * cube2 = new CCube(marble);
	cube2->SetPosition(glm::vec3(0.0f, 3.0f, 5.0f));
	scene->AddObjectToScene(cube2);
	CCube * cube3 = new CCube(wood);
	cube3->SetPosition(glm::vec3(3.0f, 2.0f, 1.0f));
	scene->AddObjectToScene(cube3);
	CCube * cube4 = new CCube(metal);
	cube4->SetPosition(glm::vec3(-1.5f, 1.5f, 0.0f));
	scene->AddObjectToScene(cube4);

	CCube * glass = new CCube(glass_material);
	glass->DrawFlags() |= Drawable_Translucent;
	glass->SetScale(glm::vec3(2.0f, 2.0f, 0.1f));
	glass->SetPosition(glm::vec3(1.2f, 0.3f, 0.0f));
	scene->AddObjectToScene(glass);
	CCube * glass2 = new CCube(glass_material);
	glass2->DrawFlags() |= Drawable_Translucent;
	glass2->SetScale(glm::vec3(2.0f, 2.0f, 0.1f));
	glass2->SetPosition(glm::vec3(1.7f, 0.3f, 1.6f));
	scene->AddObjectToScene(glass2);

	CCube * floor = new CCube(wood);
	floor->SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));
	floor->SetScale(glm::vec3(10.0f, 0.01f, 10.0f));
	scene->AddObjectToScene(floor);

	CModel * cone = new CModel("../cone/cone.obj");
	cone->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	cone->SetPosition(glm::vec3(-0.01f, 2.02f, 1.0f));
	scene->AddObjectToScene(cone);

	CFPSCounter * fps_element = new CFPSCounter(&window);
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
		cube->SetAngles(cube->GetAngles() + delta_rotate);
		cube2->SetAngles(cube2->GetAngles() + delta_rotate);
		cube3->SetAngles(cube3->GetAngles() + delta_rotate);
		cube4->SetAngles(cube4->GetAngles() + delta_rotate);

		//Go to the event callbacks specified before
		Input::PollEvents(delta_time);

		spotLight->SetPosition(camera.GetPosition());
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
