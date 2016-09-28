#pragma once

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include <vector>

#include "Shader.h"
#include "BaseDrawable.h"
#include "Camera.h"
#include "Window.h"
#include "Mesh.h"
#include "Model.h"
#include "Lights.h"

/*
 * Class managing most OpenGL rendering
 */

class Renderer
{
public:
	Renderer(Window & window, Camera * cam);
	~Renderer();

	//Draws everything on the screen
	void Draw();

	//Adds an object inheriting from BaseDrawable to the draw list
	void AddToDrawList(BaseDrawable * obj);
	//Adds a model to the draw list
	void AddToDrawList(Model * model);

	//Adds a point light to the list
	void AddPointLight(LightPoint * light);
	//Adds a spot light to the list
	void AddSpotLight(LightSpot * light);
	//Sets the directional light for this scene
	void SetDirLight(LightDirectional * light);

	//Returns number of point lights in the list
	int GetNumPointLights();
	//Returns number of spot lights in the list
	int GetNumSpotLights();

	//Color to clear with before every frame
	glm::vec4 ClearColor;

private:
	std::vector<BaseDrawable *> _drawList;

	std::vector<LightPoint *> _lightPointList;
	std::vector<LightSpot *> _lightSpotList;
	LightDirectional * _lightDir;
	
	Window _window;
	Camera * _camera;
};

