#pragma once

#include <vector>
#include <glm.hpp>

/*
 * Used by Scene class to render everything in the scene
 */

class IDrawable;
class CShader;
class CMaterial;
class CTexture;
class CCamera;
class CLightPoint;
class CLightSpot;
class CLightDirectional;
class CFramebuffer;
class CScreenFramebuffer;

class CSceneRenderer
{
public:
	CSceneRenderer(CCamera* camera);
	~CSceneRenderer();

	void AddPointLight(CLightPoint* light);
	void AddSpotLight(CLightSpot* light);
	void SetDirectionalLight(CLightDirectional* light);

	void AddDrawable(IDrawable* drawable);

	bool BuildShaders();

	void Draw();

protected:
	void setLightUniforms(CShader* shader);
	void setMatrixUniforms(CShader* shader);
	void draw(IDrawable* drawable);
	void drawSkybox(IDrawable* drawable);
	void drawUI(IDrawable* drawable);

	std::vector<CLightPoint*>	_point_light_list;
	std::vector<CLightSpot*>	_spot_light_list;
	CLightDirectional*			_directional_light;

	std::vector<IDrawable*>		_draw_list;

	std::vector<CMaterial*>		_material_list;
	std::vector<CTexture*>		_texture_list;
	std::vector<CShader*>		_shader_list;

	CCamera*					_camera;
	CScreenFramebuffer*			_screen_framebuffer;
};
