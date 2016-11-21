#include "SceneRenderer.h"

#include <map>
#include <iostream>
#include <glew.h>
#include <gtc\type_ptr.hpp>

#include "Window.h"
#include "IDrawable.h"
#include "IObject.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "Logging.h"
#include "Camera.h"
#include "Lights.h"

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
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

void SceneRenderer::AddPointLight(CLightPoint * light)
{
	_point_light_list.push_back(light);
}

void SceneRenderer::AddSpotLight(CLightSpot * light)
{
	_spot_light_list.push_back(light);
}

void SceneRenderer::SetDirectionalLight(CLightDirectional * light)
{
	_directional_light = light;
}

void SceneRenderer::AddDrawable(IDrawable * drawable)
{
	_draw_list.push_back(drawable);

	for (auto new_material : drawable->GetMaterials())
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
				if (texture == new_material->MetalAndRoughMap) spec_in_vector = true;
			}
			if (!diff_in_vector) 
				_texture_list.push_back(new_material->DiffuseMap);
			if (!spec_in_vector) 
				_texture_list.push_back(new_material->MetalAndRoughMap);
		}
	}

	glGenVertexArrays(1, &(drawable->VAO()));
	glGenBuffers(1, &(drawable->VBO()));
	glGenBuffers(1, &(drawable->EBO()));

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) 
	//and attribute pointer(s).
	glBindVertexArray(drawable->VAO());

	//bind the VBO to array_buffer - it is now actually what we want it to be
	glBindBuffer(GL_ARRAY_BUFFER, drawable->VBO());
	//copies vertex data into buffer's memory
	//last arg means data is not likely to change, or only rarely
	glBufferData(GL_ARRAY_BUFFER, drawable->GetVertices().size() * sizeof(VertexData),
		&(drawable->GetVertices())[0], GL_STATIC_DRAW);

	if (drawable->GetIndices().size() > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawable->EBO());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, drawable->GetIndices().size() * sizeof(GLuint),
			&(drawable->GetIndices())[0], GL_STATIC_DRAW);
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
		shader_create_info.Flags = 0;
		//If we have new ShaderCreateInfo params or flags, add them here
		if (drawable->DrawFlags() & Drawable_Translucent)
			shader_create_info.Flags |= Shader_Translucent;
		if (drawable->DrawFlags() & Drawable_Unlit)
			shader_create_info.Flags |= Shader_Unlit;
		if (drawable->DrawFlags() & Drawable_Skybox)
			shader_create_info.Flags |= Shader_Skybox;
		if (drawable->DrawFlags() & Drawable_UI)
			shader_create_info.Flags |= Shader_UI;

		Shader * shader = Shader::ShaderExists(shader_create_info);

		//shader was null, so lets make a new one
		if (!shader) shader = new Shader(shader_create_info);

		drawable->GetShader() = shader;

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
	glClear(GL_DEPTH_BUFFER_BIT); 

	//For each shader, set the light/camera uniforms - don't change per object so set once/frame
	for (auto shader : _shader_list)
	{
		shader->Use();
		setMatrixUniforms(shader);

		if (shader->CreateInfo.Flags & Shader_Skybox || shader->CreateInfo.Flags & Shader_Unlit ||
			shader->CreateInfo.Flags & Shader_UI)
			continue;
		
		setLightUniforms(shader);
	}

	//Draw normal objects first
	for (auto drawable : _draw_list)
	{
		if (!(drawable->DrawFlags() & Drawable_Translucent) && !(drawable->DrawFlags() & Drawable_UI) &&
			!(drawable->DrawFlags() & Drawable_Skybox))
			drawable->Draw();
	}

	//Draw the skybox
	for (auto drawable : _draw_list)
	{
		if (drawable->DrawFlags() & Drawable_Skybox)
			drawable->Draw();
	}

	//Then draw translucent objects, ordered by distance
	//This being called every frame is probably a bit silly - 
	//maybe instead find a way to call it only when the camera moves over a certain distance?
	std::map<float, IDrawable *> sorted;
	for (auto drawable : _draw_list)
	{
		//If the object matches what we're trying to draw..
		if (drawable->DrawFlags() & Drawable_Translucent && !(drawable->DrawFlags() & Drawable_UI))
		{
			//..and is a CBaseObject (has a 3d position)..
			CBaseObject * object = dynamic_cast<CBaseObject *>(drawable);
			if (object)
			{
				//..add it to the sorted list..
				sorted[glm::length(_camera->GetPosition() - object->GetPosition())] = drawable;
			}
		}
	}
	//..and finally, draw everything in that list.
	for (std::map<float, IDrawable *>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		it->second->Draw();

	//Finally, draw UI on top of everything else - disable depth testing 
	glDisable(GL_DEPTH_TEST);
	for (auto drawable : _draw_list)
	{
		if (drawable->DrawFlags() & Drawable_UI)
			drawable->Draw();
	}
	glEnable(GL_DEPTH_TEST);

	Shader::UseNull();
}

void SceneRenderer::setLightUniforms(Shader * shader)
{
	for (uint32_t i = 0; i < _point_light_list.size(); i++)
	{
		glUniform3f(shader->GetUniformLocation(
			("pointLights[" + std::to_string(i) + "].Position").c_str()),
			_point_light_list[i]->GetPosition().x, _point_light_list[i]->GetPosition().y,
			_point_light_list[i]->GetPosition().z);
		glUniform3f(shader->GetUniformLocation(
			("pointLights[" + std::to_string(i) + "].Color").c_str()),
			_point_light_list[i]->Color.x, _point_light_list[i]->Color.y,
			_point_light_list[i]->Color.z);
		glUniform1f(shader->GetUniformLocation(
			("pointLights[" + std::to_string(i) + "].Intensity").c_str()),
			_point_light_list[i]->Intensity);
		glUniform1f(shader->GetUniformLocation(
			("pointLights[" + std::to_string(i) + "].Constant").c_str()),
			_point_light_list[i]->Constant);
		glUniform1f(shader->GetUniformLocation(
			("pointLights[" + std::to_string(i) + "].Linear").c_str()),
			_point_light_list[i]->Linear);
		glUniform1f(shader->GetUniformLocation(
			("pointLights[" + std::to_string(i) + "].Quadratic").c_str()),
			_point_light_list[i]->Quadratic);
	}

	for (uint32_t i = 0; i < _spot_light_list.size(); i++)
	{
		glUniform3f(shader->GetUniformLocation(
			("spotLights[" + std::to_string(i) + "].Position").c_str()),
			_spot_light_list[i]->GetPosition().x, _spot_light_list[i]->GetPosition().y,
			_spot_light_list[i]->GetPosition().z);
		glUniform3f(shader->GetUniformLocation(
			("spotLights[" + std::to_string(i) + "].Direction").c_str()),
			_spot_light_list[i]->GetForward().x, _spot_light_list[i]->GetForward().y,
			_spot_light_list[i]->GetForward().z);
		glUniform3f(shader->GetUniformLocation(
			("spotLights[" + std::to_string(i) + "].Color").c_str()),
			_spot_light_list[i]->Color.x, _spot_light_list[i]->Color.y,
			_spot_light_list[i]->Color.z);
		glUniform1f(shader->GetUniformLocation(
			("spotLights[" + std::to_string(i) + "].InnerCutOff").c_str()),
			glm::cos(glm::radians(_spot_light_list[i]->InnerCutOff)));
		glUniform1f(shader->GetUniformLocation(
			("spotLights[" + std::to_string(i) + "].OuterCutOff").c_str()),
			glm::cos(glm::radians(_spot_light_list[i]->OuterCutOff)));
		glUniform1f(shader->GetUniformLocation(
			("spotLights[" + std::to_string(i) + "].Intensity").c_str()),
			_spot_light_list[i]->Intensity);
		glUniform1f(shader->GetUniformLocation(
			("spotLights[" + std::to_string(i) + "].Constant").c_str()),
			_spot_light_list[i]->Constant);
		glUniform1f(shader->GetUniformLocation(
			("spotLights[" + std::to_string(i) + "].Linear").c_str()),
			_spot_light_list[i]->Linear);
		glUniform1f(shader->GetUniformLocation(
			("spotLights[" + std::to_string(i) + "].Quadratic").c_str()),
			_spot_light_list[i]->Quadratic);
	}
	glUniform3f(shader->GetUniformLocation("directionalLight.Direction"),
		_directional_light->GetForward().x, _directional_light->GetForward().y,
		_directional_light->GetForward().z);
	glUniform3f(shader->GetUniformLocation("directionalLight.Color"),
		_directional_light->Color.x, _directional_light->Color.y,
		_directional_light->Color.z);
	glUniform1f(shader->GetUniformLocation("directionalLight.Intensity"),
		_directional_light->Intensity);
	glUniform3f(shader->GetUniformLocation("directionalLight.AmbientColor"),
		_directional_light->AmbientColor.x, _directional_light->AmbientColor.y,
		_directional_light->AmbientColor.z);
	glUniform1f(shader->GetUniformLocation("directionalLight.AmbientIntensity"),
		_directional_light->AmbientIntensity);
}

void SceneRenderer::setMatrixUniforms(Shader * shader)
{
	if (shader->CreateInfo.Flags & Shader_Skybox)
	{
		glUniformMatrix4fv(shader->GetUniformLocation("view"), 1, GL_FALSE,
			glm::value_ptr(glm::mat4(glm::mat3(_camera->GetViewMatrix()))));
		glUniformMatrix4fv(shader->GetUniformLocation("projection"), 1, GL_FALSE,
			glm::value_ptr(_camera->GetProjMatrix()));
		return;
	}
	if (shader->CreateInfo.Flags & Shader_UI)
	{
		glUniformMatrix4fv(shader->GetUniformLocation("view"), 1, GL_FALSE,
			glm::value_ptr(glm::mat4()));
		glUniformMatrix4fv(shader->GetUniformLocation("projection"), 1, GL_FALSE,
			glm::value_ptr(glm::mat4()));
		return;
	}

	glUniform3f(shader->GetUniformLocation("viewPos"),
		_camera->GetPosition().x, _camera->GetPosition().y, _camera->GetPosition().z);
	glUniformMatrix4fv(shader->GetUniformLocation("view"), 1, GL_FALSE,
		glm::value_ptr(_camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->GetUniformLocation("projection"), 1, GL_FALSE,
		glm::value_ptr(_camera->GetProjMatrix()));
}
