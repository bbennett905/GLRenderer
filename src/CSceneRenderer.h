#pragma once

#include <vector>
#include <glm.hpp>

/*
 * Used by Scene class to render everything in the scene
 */

class CWindow;
class IDrawable;
class CShader;
class CMaterial;
class CTexture;
class CCamera;
class CLightPoint;
class CLightSpot;
class CLightDirectional;

class CSceneRenderer
{
public:
	CSceneRenderer(CWindow * window, CCamera * camera);
	~CSceneRenderer();

	void AddPointLight(CLightPoint * light);
	void AddSpotLight(CLightSpot * light);
	void SetDirectionalLight(CLightDirectional * light);

	void AddDrawable(IDrawable * drawable);

	bool BuildShaders();

	void Draw();

private:
	void setLightUniforms(CShader * shader);
	void setMatrixUniforms(CShader * shader);

	std::vector<CLightPoint *>	_point_light_list;
	std::vector<CLightSpot *>	_spot_light_list;
	CLightDirectional *			_directional_light;

	std::vector<IDrawable *>	_draw_list;

	std::vector<CMaterial *>	_material_list;
	std::vector<CTexture *>		_texture_list;
	std::vector<CShader *>		_shader_list;

	CCamera * _camera;
};
