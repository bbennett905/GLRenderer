#pragma once

#include <vector>
#include <glm.hpp>

/*
 * Used by Scene class to render everything in the scene
 */

class Window;
class IDrawable;
class Shader;
class Material;
class Texture;
class Camera;
class CLightPoint;
class CLightSpot;
class CLightDirectional;

class SceneRenderer
{
public:
	SceneRenderer(Window * window, Camera * camera);
	~SceneRenderer();

	void AddPointLight(CLightPoint * light);
	void AddSpotLight(CLightSpot * light);
	void SetDirectionalLight(CLightDirectional * light);

	void AddDrawable(IDrawable * drawable);

	bool BuildShaders();

	void Draw();

private:
	void setLightUniforms(Shader * shader);
	void setMatrixUniforms(Shader * shader);

	std::vector<CLightPoint *>	_point_light_list;
	std::vector<CLightSpot *>	_spot_light_list;
	CLightDirectional *			_directional_light;

	std::vector<IDrawable *> _draw_list;

	std::vector<Material *>		_material_list;
	std::vector<Texture *>		_texture_list;
	std::vector<Shader *>		_shader_list;

	Camera * _camera;
};
