#include "BaseUIElement.h"

#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Shader.h"
#include "Material.h"
#include "Texture.h"

//TODO fix these
VertexData quad[] = {
	// Positions				// Normals       // Texture Coords
	{ {0.5f, 0.5f, 0.0f},	{0.0f, 0.0f, -1.0f}, {1.0f, 1.0f} },
	{ {0.5f, -0.5f, 0.0f},	{0.0f, 0.0f, -1.0f}, {1.0f, 0.0f} },
	{ {-0.5f, -0.5f, 0.0f},	{0.0f, 0.0f, -1.0f}, {0.0f, 0.0f} },
	{ {-0.5f, -0.5f, 0.0f},	{0.0f, 0.0f, -1.0f}, {0.0f, 0.0f} },
	{ {-0.5f, 0.5f, 0.0f},	{0.0f, 0.0f, -1.0f}, {0.0f, 1.0f} },
	{ {0.5f, 0.5f, 0.0f},	{0.0f, 0.0f, -1.0f}, {1.0f, 1.0f} } };

BaseUIElement::BaseUIElement(glm::vec2 pos, glm::vec2 scale) :
	BaseDrawable(std::vector<VertexData>(std::begin(quad), std::end(quad))),
	Position(pos), Scale(scale)
{

}

BaseUIElement::~BaseUIElement()
{

}

//Doesn't really need the args, just have them because basedraw does
void BaseUIElement::Draw(Camera * camera, std::vector<LightPoint *> & point_light_list,
	std::vector<LightSpot *> & spot_light_list, LightDirectional * directional_light)
{
	ShaderObj->Use();

	if (Materials.size())
		glUniform1i(glGetUniformLocation(ShaderObj->Program, "hasMaterials"), 1);
	else
		glUniform1i(glGetUniformLocation(ShaderObj->Program, "hasMaterials"), 0);
	for (uint32_t i = 0; i < Materials.size(); i++)
	{
		//The reason the shader works even when one of these uniforms isn't set is because
		//Sampler2Ds in GLSL are guaranteed to return black if there's no texture unit bound.
		if (Materials[i]->DiffuseMap != nullptr)
		{
			glUniform1i(glGetUniformLocation(ShaderObj->Program,
				("materials[" + std::to_string(i) + "].HasDiffMap").c_str()), 1);
			glActiveTexture(GL_TEXTURE0 + ShaderObj->TextureCount);
			Materials[i]->DiffuseMap->Bind();
			glUniform1i(glGetUniformLocation(ShaderObj->Program,
				("materials[" + std::to_string(i) + "].DiffMap").c_str()),
				ShaderObj->TextureCount);
			ShaderObj->TextureCount++;
		}
		else
		{
			glUniform1i(glGetUniformLocation(ShaderObj->Program,
				("materials[" + std::to_string(i) + "].HasDiffMap").c_str()), 0);
		}

		if (Materials[i]->SpecularMap != nullptr)
		{
			glUniform1i(glGetUniformLocation(ShaderObj->Program,
				("materials[" + std::to_string(i) + "].HasSpecMap").c_str()), 1);
			glActiveTexture(GL_TEXTURE0 + ShaderObj->TextureCount);
			Materials[i]->SpecularMap->Bind();
			glUniform1i(glGetUniformLocation(ShaderObj->Program,
				("materials[" + std::to_string(i) + "].SpecMap").c_str()),
				ShaderObj->TextureCount);
			ShaderObj->TextureCount++;
		}
		else
		{
			glUniform1i(glGetUniformLocation(ShaderObj->Program,
				("materials[" + std::to_string(i) + "].HasSpecMap").c_str()), 0);
		}

		glUniform1f(glGetUniformLocation(ShaderObj->Program,
			("materials[" + std::to_string(i) + "].AmbientStrength").c_str()),
			Materials[i]->AmbientStrength);
		glUniform1f(glGetUniformLocation(ShaderObj->Program,
			("materials[" + std::to_string(i) + "].DiffuseStrength").c_str()),
			Materials[i]->DiffuseStrength);
		glUniform1f(glGetUniformLocation(ShaderObj->Program,
			("materials[" + std::to_string(i) + "].SpecularStrength").c_str()),
			Materials[i]->SpecularStrength);
		glUniform1f(glGetUniformLocation(ShaderObj->Program,
			("materials[" + std::to_string(i) + "].Shininess").c_str()),
			Materials[i]->Shininess);
	}

	glUniform1i(glGetUniformLocation(ShaderObj->Program, "numMaterials"),
		ShaderObj->TextureCount);

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
	transform = glm::translate(transform, glm::vec3(Position, 0.0f));
	transform = glm::scale(transform, glm::vec3(Scale, 1.0f));
	return transform;
}
