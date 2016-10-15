#include "SceneRenderer.h"

#include <iostream>
#include "Window.h"
#include "BaseDrawable.h"

SceneRenderer::SceneRenderer(Window * window, Camera * camera) :
	_camera(camera)
{
	glewExperimental = GL_TRUE;
	GLenum f = glewInit();
	if (f != GLEW_OK)
	{
		std::cout << "GLEW initialization failed!" << std::endl;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//TODO remove this once cubemaps/skybox 
	_clear_color = glm::vec4(0.7f, 0.0f, 0.0f, 1.0f);
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

	glGenVertexArrays(1, &(drawable->VertexArrayObj));
	glGenBuffers(1, &(drawable->VertexBufferObj));
	glGenBuffers(1, &(drawable->ElementBufferObj));

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(drawable->VertexArrayObj);

	//bind the VBO to array_buffer - it is now actually what we want it to be
	glBindBuffer(GL_ARRAY_BUFFER, drawable->VertexBufferObj);
	//copies vertex data into buffer's memory - last arg means data is not likely to change, or only rarely
	glBufferData(GL_ARRAY_BUFFER, drawable->Vertices.size() * sizeof(VertexData),
		&(drawable->Vertices)[0], GL_STATIC_DRAW);

	if (drawable->Indices.size() > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawable->ElementBufferObj);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, drawable->Indices.size() * sizeof(GLuint),
			&(drawable->Indices)[0], GL_STATIC_DRAW);
	}
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 
		(GLvoid *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 
		(GLvoid *)(offsetof(VertexData, Normal)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), 
		(GLvoid*)(offsetof(VertexData, TexCoords)));
	glEnableVertexAttribArray(2);

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
			if (shader->CreateInfo.Version == shader_create_info.Version &&
				shader->CreateInfo.NumPointLights == shader_create_info.NumPointLights &&
				shader->CreateInfo.NumSpotLights == shader_create_info.NumSpotLights &&
				shader->CreateInfo.Flags == shader_create_info.Flags)
					drawable->ShaderObj = shader;
		}

		if (drawable->ShaderObj == nullptr)
		{
			Shader * shader = new Shader(shader_create_info);
			drawable->ShaderObj = shader;
			_shader_list.push_back(shader);
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
		drawable->Draw(_camera, _point_light_list, _spot_light_list, _directional_light);
	}
}
