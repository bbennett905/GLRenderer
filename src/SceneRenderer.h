#pragma once

#include <vector>
#include <glm.hpp>

/*
 * Used by Scene class to render everything in the scene
 */

class Window;
class BaseDrawable;
class Shader;
struct Material;
class Texture;
class Camera;
class LightPoint;
class LightSpot;
class LightDirectional;

class SceneRenderer
{
public:
	SceneRenderer(Window * window, Camera * camera);
	~SceneRenderer();

	//TODO Set Light uniforms method? Doing this means not passing light info to Drawable::Draw
	void AddPointLight(LightPoint * light);
	void AddSpotLight(LightSpot * light);
	void SetDirectionalLight(LightDirectional * light);

	void AddDrawable(BaseDrawable * drawable);

	bool BuildShaders();

	void Draw();

private:
	std::vector<LightPoint *>	_point_light_list;
	std::vector<LightSpot *>	_spot_light_list;
	LightDirectional *			_directional_light;

	std::vector<BaseDrawable *> _draw_list;

	std::vector<Material *>		_material_list;
	std::vector<Texture *>		_texture_list;
	std::vector<Shader *>		_shader_list;

	glm::vec4 _clear_color;

	Camera * _camera;
};

