#include "CBaseUIElement.h"

#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <SDL_ttf.h>

#include "CShader.h"
#include "CMaterial.h"
#include "CTexture.h"
#include "CWindow.h"
#include "Logging.h"

bool CBaseUIElement::_is_ttf_init = false;

VertexData quad[] = {
	// Positions				// Normals       // Texture Coords
	{ {-1.0f, -1.0f, 0.0f},	{0.0f, 0.0f, -1.0f}, {0.0f,  0.0f} },
	{ { 1.0f, -1.0f, 0.0f},	{0.0f, 0.0f, -1.0f}, {1.0f,  0.0f} },
	{ { 1.0f,  1.0f, 0.0f},	{0.0f, 0.0f, -1.0f}, {1.0f, -1.0f} },
	{ { 1.0f,  1.0f, 0.0f},	{0.0f, 0.0f, -1.0f}, {1.0f, -1.0f} },
	{ {-1.0f,  1.0f, 0.0f},	{0.0f, 0.0f, -1.0f}, {0.0f, -1.0f} },
	{ {-1.0f, -1.0f, 0.0f},	{0.0f, 0.0f, -1.0f}, {0.0f,  0.0f} } };

CBaseUIElement::CBaseUIElement(CWindow * window, glm::vec2 pos, glm::vec2 scale) :
	CBaseDrawable(std::vector<VertexData>(std::begin(quad), std::end(quad))),
	CBaseObject(glm::vec3(pos, 0.0f)), _texture(new CTexture()), _window(window)
{
	_flags |= Drawable_Translucent | Drawable_Unlit | Drawable_UI;
	_scale = glm::vec3(scale, 1.0f);
	if (!_is_ttf_init)
	{
		if (TTF_Init() < 0)
		{
			Logging::LogMessage(LogLevel_Error, "SDL_TTF intialization failed: %s", SDL_GetError());
			return;
		}
		_is_ttf_init = true;
	}
}

CBaseUIElement::~CBaseUIElement()
{
	delete _texture;
}

void CBaseUIElement::Draw()
{
	if (!_texture) return;
	_shader->Use();

	glUniform1i(_shader->GetUniformLocation("hasMaterials"), 1);

	//The reason the shader works even when one of these uniforms isn't set is because
	//Sampler2Ds in GLSL are guaranteed to return black if there's no texture unit bound.
	glUniform1i(_shader->GetUniformLocation("materials[0].HasDiffMap"), 1);
	glActiveTexture(GL_TEXTURE0 + _shader->TextureCount);
	_texture->Bind();
	glUniform1i(_shader->GetUniformLocation("materials[0].DiffMap"),
		_shader->TextureCount);
	_shader->TextureCount++;

	glUniform1i(_shader->GetUniformLocation("materials[0].HasSpecMap"), 0);

	glUniform1i(_shader->GetUniformLocation("numMaterials"), 1);

	//model matrix transforms model space to world space - rotation and translation
	glUniformMatrix4fv(_shader->GetUniformLocation("model"), 1, GL_FALSE,
		glm::value_ptr(GetModelMatrix()));

	glBindVertexArray(VAO());
	//Draw! - type of primitive, starting index of vertex array, number of vertices
	glDrawArrays(GL_TRIANGLES, 0, GetVertices().size());
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

glm::mat4 CBaseUIElement::GetModelMatrix()
{
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(GetPosition().x, GetPosition().y, 0.01f));
	transform = glm::scale(transform, glm::vec3(GetScale().x, GetScale().y, 1.0f));
	return transform;
}

void CBaseUIElement::autoPosition(int width, int height, int x, int y)
{
	_scale = glm::vec3(float(width) / float(_window->GetWidth()),
		float(height) / float(_window->GetHeight()), 1.0f);
	_position = glm::vec3(-1.0f + (float(x) / float(_window->GetWidth())) + _scale.x, 
		1.0f - (float(y) / float(_window->GetHeight())) - _scale.y, 0.0f);
}
