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

bool SceneRenderer::BuildShaders()
{
	ShaderCreateInfo shader_create_info;
	shader_create_info.Version =		ShaderVersion330Core;
	shader_create_info.NumPointLights = _point_light_list.size();
	shader_create_info.NumSpotLights =	_spot_light_list.size();

	for (auto drawable : _draw_list)
	{
		//If we have new ShaderCreateInfo params or flags, go under here
		if (drawable->Flags & Drawable_Translucent)
			shader_create_info.Flags |= Shader_Translucent;

		//Do we already have a shader matching the requirements? if so, use it!
		for (auto shader : _shader_list)
		{
			//TODO does this work?
			if (shader->CreateInfo.Version == shader_create_info.Version &&
				shader->CreateInfo.NumPointLights == shader_create_info.NumPointLights &&
				shader->CreateInfo.NumSpotLights == shader_create_info.NumSpotLights &&
				shader->CreateInfo.Flags == shader_create_info.Flags)
					drawable->ShaderObj = shader;
		}

		if (drawable->ShaderObj == nullptr)
		{
			drawable->ShaderObj = new Shader(shader_create_info);
			_shader_list.push_back(drawable->ShaderObj);
		}
	}
	return true;
}

void SceneRenderer::Draw()
{
	glClearColor(_clear_color.x, _clear_color.y, _clear_color.z, _clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//TODO sort this by transparent textures
	for (auto drawable : _draw_list)
	{
		//TODO give this light info
		drawable->Draw(_camera, _point_light_list, _spot_light_list, _directional_light);
	}
}
