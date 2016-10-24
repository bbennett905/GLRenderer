#include "BaseUIElement.h"

#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "Window.h"

VertexData quad[] = {
	// Positions				// Normals       // Texture Coords
	{ {-1.0f, -1.0f, 0.0f},	{0.0f, 0.0f, -1.0f}, {0.0f,  0.0f} },
	{ { 1.0f, -1.0f, 0.0f},	{0.0f, 0.0f, -1.0f}, {1.0f,  0.0f} },
	{ { 1.0f,  1.0f, 0.0f},	{0.0f, 0.0f, -1.0f}, {1.0f, -1.0f} },
	{ { 1.0f,  1.0f, 0.0f},	{0.0f, 0.0f, -1.0f}, {1.0f, -1.0f} },
	{ {-1.0f,  1.0f, 0.0f},	{0.0f, 0.0f, -1.0f}, {0.0f, -1.0f} },
	{ {-1.0f, -1.0f, 0.0f},	{0.0f, 0.0f, -1.0f}, {0.0f,  0.0f} } };

BaseUIElement::BaseUIElement(Window * window, glm::vec2 pos, glm::vec2 scale) :
	BaseDrawable(std::vector<VertexData>(std::begin(quad), std::end(quad))),
	Position(pos), Scale(scale), _texture(new Texture()), _window(window)
{
	Flags = Drawable_Translucent | Drawable_Unlit | Drawable_UI;
}

BaseUIElement::~BaseUIElement()
{

}

//Doesn't really need the args, just have them because basedraw does
void BaseUIElement::Draw(Camera * camera, std::vector<LightPoint *> & point_light_list,
	std::vector<LightSpot *> & spot_light_list, LightDirectional * directional_light)
{
	ShaderObj->Use();

	glUniform1i(glGetUniformLocation(ShaderObj->Program, "hasMaterials"), 1);

	//The reason the shader works even when one of these uniforms isn't set is because
	//Sampler2Ds in GLSL are guaranteed to return black if there's no texture unit bound.
	glUniform1i(glGetUniformLocation(ShaderObj->Program, "materials[0].HasDiffMap"), 1);
	glActiveTexture(GL_TEXTURE0 + ShaderObj->TextureCount);
	_texture->Bind();
	glUniform1i(glGetUniformLocation(ShaderObj->Program, "materials[0].DiffMap"),
		ShaderObj->TextureCount);
	ShaderObj->TextureCount++;

	glUniform1i(glGetUniformLocation(ShaderObj->Program, "materials[0].HasSpecMap"), 0);

	glUniform1i(glGetUniformLocation(ShaderObj->Program, "numMaterials"), 1);

	//model matrix transforms model space to world space - rotation and translation
	glUniformMatrix4fv(glGetUniformLocation(ShaderObj->Program, "model"), 1, GL_FALSE,
		glm::value_ptr(GetModelMatrix()));

	//both are given identity matrix
	glUniformMatrix4fv(glGetUniformLocation(ShaderObj->Program, "view"), 1, GL_FALSE,
		glm::value_ptr(glm::mat4()));
	glUniformMatrix4fv(glGetUniformLocation(ShaderObj->Program, "projection"), 1, GL_FALSE,
		glm::value_ptr(glm::mat4()));

	glBindVertexArray(VertexArrayObj);
	//Draw! - type of primitive, starting index of vertex array, number of vertices
	glDrawArrays(GL_TRIANGLES, 0, Vertices.size());
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

glm::mat4 BaseUIElement::GetModelMatrix()
{
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(Position, 0.01f));
	transform = glm::scale(transform, glm::vec3(Scale, 1.0f));
	return transform;
}

void BaseUIElement::autoPosition(int width, int height, int x, int y)
{
	Scale = glm::vec2(float(width) / float(_window->GetWidth()),
		float(height) / float(_window->GetHeight()));
	Position = glm::vec2(-1.0f + (float(x) / float(_window->GetWidth())) + Scale.x, 
		1.0f - (float(y) / float(_window->GetHeight())) - Scale.y);
}
