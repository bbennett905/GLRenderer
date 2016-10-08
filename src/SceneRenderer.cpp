#include "SceneRenderer.h"

#include <iostream>
#include "Window.h"
#include "BaseDrawable.h"

SceneRenderer::SceneRenderer(Window * window)
{
	glewExperimental = GL_TRUE;
	GLenum f = glewInit();
	if (f != GLEW_OK)
	{
		std::cout << "GLEW initialization failed!" << std::endl;
	}

	glViewport(0, 0, window->GetWidth(), window->GetHeight());
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//TODO remove this once cubemaps/skybox 
	_clear_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}


SceneRenderer::~SceneRenderer()
{
	//TODO we probably need to destroy things here (lights)?
}

void SceneRenderer::AddPointLight(LightPoint * light)
{
	_point_light_list.push_back(light);
}

void SceneRenderer::AddSpotLight(LightSpot * light)
{
	_spot_light_list.push_back(light);
}

void SceneRenderer::SetDirectionalLight(LightDirectional * light)
{
	_directional_light = light;
}

void SceneRenderer::AddDrawable(BaseDrawable * drawable)
{
	_draw_list.push_back(drawable);
}

void SceneRenderer::Draw()
{
	glClearColor(_clear_color.x, _clear_color.y, _clear_color.z, _clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto obj : _draw_list)
	{
		//TODO give this light info
		obj->Draw();
	}
}
