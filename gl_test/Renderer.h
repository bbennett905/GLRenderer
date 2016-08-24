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

#include "Lights.h"

#define MAX_DRAWABLES 256

//TODO this will need to take a BaseDrawable or whatever in the draw methods 
//so save that for later

class Renderer
{
public:
	Renderer(Window & window, Camera * cam);
	~Renderer();

	void Draw();
	void AddToDrawList(BaseDrawable * obj);

	void AddPointLight(LightPoint * light);
	void AddSpotLight(LightSpot * light);
	void SetDirLight(LightDirectional * light);

	glm::vec4 ClearColor;
private:
	std::vector<BaseDrawable *> _drawList;

	//TODO this will be broken if exceeds NR_POINT_LIGHTS in shader!
	std::vector<LightPoint *> _lightPointList;
	std::vector<LightSpot *> _lightSpotList;
	LightDirectional * _lightDir;
	
	Window _window;
	Camera * _camera;
	//LightSimple _light; //TEMP
};

