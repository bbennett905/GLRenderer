#pragma once

#include <vector>
#include "Lights.h"
#include "Camera.h"

//see Scene.h for most info,
//but this is basically what is currently the Renderer class with a little moved to BaseDrawable

class Window;

class SceneRenderer
{
public:
	SceneRenderer(Window * window);
	~SceneRenderer();

	//TODO
	//Add light method
	//Set Light uniforms method

private:
	std::vector<LightPoint *>	_point_light_list;
	std::vector<LightSpot *>	_spot_light_list;
	LightDirectional *			_directional_light;

	glm::vec4 _clear_color;

	Camera * _camera;
};

