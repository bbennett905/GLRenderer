#include "BaseUIElement.h"

#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <SDL_ttf.h>

#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "Window.h"
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

CBaseUIElement::CBaseUIElement(glm::vec2 pos, glm::vec2 scale) :
	CBaseDrawable(std::vector<VertexData>(std::begin(quad), std::end(quad))),
	CBaseObject(glm::vec3(pos, 0.0f))//_texture(new CTexture())
{
	_materials.push_back(new CMaterial(new CTexture()));
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
	_scale = glm::vec3(float(width) / float(Window::GetWidth()),
		float(height) / float(Window::GetHeight()), 1.0f);
	_position = glm::vec3(-1.0f + (float(x) / float(Window::GetWidth())) + _scale.x,
		1.0f - (float(y) / float(Window::GetHeight())) - _scale.y, 0.0f);
}
