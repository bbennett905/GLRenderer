#include "SceneRenderer.h"

#include <iostream>
#include <glew.h>

#include "Window.h"
#include "BaseDrawable.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "Logging.h"

SceneRenderer::SceneRenderer(Window * window, Camera * camera) :
	_camera(camera)
{
	glewExperimental = GL_TRUE;
	GLenum f = glewInit();
	if (f != GLEW_OK)
	{
		Logging::LogMessage(LogLevel_Critical, "GLEW initialization failed!");
		return;
	}

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE); //TODO reenable this once UIElements winding is fixed!
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//TODO remove this once cubemaps/skybox 
	_clear_color = glm::vec4(0.0f, 0.0f, 0.5f, 1.0f);
}

SceneRenderer::~SceneRenderer()
{
	//TODO replace this form of memory management with mm base class everything inherits from

	//We only really need to destroy the shaders, materials, textures, 
	//as all objects themselves are destroyed in Scene destructor
	for (auto shader : _shader_list)
		delete shader;

	for (auto texture : _texture_list)
		delete texture;

	for (auto material : _material_list)
		delete material;
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

	for (auto new_material : drawable->Materials)
	{
		bool in_vector = false;
		for (auto existing_material : _material_list)
		{
			if (new_material == existing_material)
			{
				in_vector = true;
				break;
			}
		}
		if (!in_vector)
		{
			_material_list.push_back(new_material);
			bool diff_in_vector = false, spec_in_vector = false;
			for (auto texture : _texture_list)
			{
				if (texture == new_material->DiffuseMap) diff_in_vector = true;
				if (texture == new_material->SpecularMap) spec_in_vector = true;
			}
			if (!diff_in_vector) 
				_texture_list.push_back(new_material->DiffuseMap);
			if (!spec_in_vector) 
				_texture_list.push_back(new_material->SpecularMap);
		}
	}

	glGenVertexArrays(1, &(drawable->VertexArrayObj));
	glGenBuffers(1, &(drawable->VertexBufferObj));
	glGenBuffers(1, &(drawable->ElementBufferObj));

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) 
	//and attribute pointer(s).
	glBindVertexArray(drawable->VertexArrayObj);

	//bind the VBO to array_buffer - it is now actually what we want it to be
	glBindBuffer(GL_ARRAY_BUFFER, drawable->VertexBufferObj);
	//copies vertex data into buffer's memory
	//last arg means data is not likely to change, or only rarely
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
	Logging::LogMessage(LogLevel_Debug, "Building shaders...");
	ShaderCreateInfo shader_create_info;
	shader_create_info.Version =		ShaderVersion330Core;
	shader_create_info.NumPointLights = _point_light_list.size();
	shader_create_info.NumSpotLights =	_spot_light_list.size();

	for (auto drawable : _draw_list)
	{
		//If we have new ShaderCreateInfo params or flags, add them here
		if (drawable->Flags & Drawable_Translucent)
			shader_create_info.Flags |= Shader_Translucent;
		if (drawable->Flags & Drawable_Unlit)
			shader_create_info.Flags |= Shader_Unlit;

		Shader * shader = Shader::ShaderExists(shader_create_info);

		//shader was null, so lets make a new one
		if (!shader) shader = new Shader(shader_create_info);

		drawable->ShaderObj = shader;

		bool in_shader_list = false;
		for (auto shad : _shader_list)
		{
			if (shad == shader)
			{
				in_shader_list = true;
				break;
			}
		}
		if (!in_shader_list)
			_shader_list.push_back(shader);
	}
	Logging::LogMessage(LogLevel_Debug, "Built %d shaders", _shader_list.size());
	return true;
}

void SceneRenderer::Draw()
{
	glClearColor(_clear_color.x, _clear_color.y, _clear_color.z, _clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//TODO sort this by transparent textures
	for (auto drawable : _draw_list)
	{
		if (!(drawable->Flags & Drawable_Translucent))
			drawable->Draw(_camera, _point_light_list, _spot_light_list, _directional_light);
	}
	for (auto drawable : _draw_list)
	{
		if (drawable->Flags & Drawable_Translucent)
			drawable->Draw(_camera, _point_light_list, _spot_light_list, _directional_light);
	}
}
