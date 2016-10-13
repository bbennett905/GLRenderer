#pragma once

#include <vector>
#include "Lights.h"
#include "Camera.h"

//see Scene.h for most info,
//but this is basically what is currently the Renderer class with a little moved to BaseDrawable

class Window;
class BaseDrawable;

class SceneRenderer
{
public:
	SceneRenderer(Window * window);
	~SceneRenderer();

	//TODO
	//Set Light uniforms method
	void AddPointLight(LightPoint * light);
	void AddSpotLight(LightSpot * light);
	void SetDirectionalLight(LightDirectional * light);

	void AddDrawable(BaseDrawable * drawable);

	bool BuildShaders();

	void Draw();

private:
	std::vector<Shader *>		_shader_list;
	std::vector<LightPoint *>	_point_light_list;
	std::vector<LightSpot *>	_spot_light_list;
	LightDirectional *			_directional_light;

	std::vector<BaseDrawable *> _draw_list;

	glm::vec4 _clear_color;

	Camera * _camera;
};

