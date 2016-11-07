#include "BaseDrawable.h"

#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Camera.h"
#include "Material.h"
#include "Shader.h"
#include "Lights.h"
#include "Texture.h"

BaseDrawable::BaseDrawable() { }

BaseDrawable::BaseDrawable(const GLfloat vertices[], int verticesSize, Material * mat)
{
	Materials.push_back(mat);
	for (int i = 0; i < verticesSize; i += 8)// ? ok
	{
		VertexData data = { glm::vec3(vertices[i], vertices[i+1], vertices[i+2]),	//Pos
							glm::vec3(vertices[i+3], vertices[i+4], vertices[i+5]),	//Norm
							glm::vec2(vertices[i+6], vertices[i+7]) };				//TexCoord
		Vertices.push_back(data);
	}
}

BaseDrawable::BaseDrawable(const GLfloat vertices[], int verticesSize, 
	std::vector<Material *> & mat) :
	Materials(mat)
{
	for (int i = 0; i < verticesSize; i += 8)// ? ok
	{
		VertexData data = { glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]),	//Pos
			glm::vec3(vertices[i + 3], vertices[i + 4], vertices[i + 5]),	//Norm
			glm::vec2(vertices[i + 6], vertices[i + 7]) };				//TexCoord
		Vertices.push_back(data);
	}
}

BaseDrawable::BaseDrawable(std::vector<VertexData> vert) :
	Vertices(vert)
{ }

BaseDrawable::BaseDrawable(std::vector<VertexData> & vert, std::vector<GLuint> & ind,
	std::vector<Material *> & mat) :
	Vertices(vert), Indices(ind), Materials(mat)
{ 
	assert(mat.size() < MAX_MATERIALS && "An object exceeded MAX_MATERIALS!");
}

BaseDrawable::~BaseDrawable()
{
	//We DON'T destroy the shader, because they may be shared with other objects
	//Same with materials/textures
	glDeleteVertexArrays(1, &VertexArrayObj);
	glDeleteBuffers(1, &VertexBufferObj);
	glDeleteBuffers(1, &ElementBufferObj);
}

glm::mat4 BaseDrawable::GetModelMatrix()
{
	return glm::mat4(); //Return an identity matrix, no transform
}

void BaseDrawable::Draw(Camera * camera)
{
	ShaderObj->Use();

	if (Materials.size())
	{
		glUniform1i(ShaderObj->GetUniformLocation("hasMaterials"), 1);
	}
	else
	{
		glUniform1i(ShaderObj->GetUniformLocation("hasMaterials"), 0);
	}
	for (uint32_t i = 0; i < Materials.size(); i++)
	{
		//The reason the shader works even when one of these uniforms isn't set is because
		//Sampler2Ds in GLSL are guaranteed to return black if there's no texture unit bound.
		if (Materials[i]->DiffuseMap != nullptr)
		{
			glUniform1i(ShaderObj->GetUniformLocation(
				("materials[" + std::to_string(i) + "].HasDiffMap").c_str()), 1);
			glActiveTexture(GL_TEXTURE0 + ShaderObj->TextureCount);
			Materials[i]->DiffuseMap->Bind();
			glUniform1i(ShaderObj->GetUniformLocation(
				("materials[" + std::to_string(i) + "].DiffMap").c_str()),
				ShaderObj->TextureCount);
			ShaderObj->TextureCount++;
		}
		else
		{
			glUniform1i(ShaderObj->GetUniformLocation(
				("materials[" + std::to_string(i) + "].HasDiffMap").c_str()), 0);
		}

		if (Materials[i]->SpecularMap != nullptr)
		{
			glUniform1i(ShaderObj->GetUniformLocation(
				("materials[" + std::to_string(i) + "].HasSpecMap").c_str()), 1);
			glActiveTexture(GL_TEXTURE0 + ShaderObj->TextureCount);
			Materials[i]->SpecularMap->Bind();
			glUniform1i(ShaderObj->GetUniformLocation(
				("materials[" + std::to_string(i) + "].SpecMap").c_str()),
				ShaderObj->TextureCount);
			ShaderObj->TextureCount++;
		}
		else
		{
			glUniform1i(ShaderObj->GetUniformLocation(
				("materials[" + std::to_string(i) + "].HasSpecMap").c_str()), 0);
		}

		glUniform1f(ShaderObj->GetUniformLocation(
			("materials[" + std::to_string(i) + "].AmbientStrength").c_str()),
			Materials[i]->AmbientStrength);
		glUniform1f(ShaderObj->GetUniformLocation(
			("materials[" + std::to_string(i) + "].DiffuseStrength").c_str()),
			Materials[i]->DiffuseStrength);
		glUniform1f(ShaderObj->GetUniformLocation(
			("materials[" + std::to_string(i) + "].SpecularStrength").c_str()),
			Materials[i]->SpecularStrength);
		glUniform1f(ShaderObj->GetUniformLocation(
			("materials[" + std::to_string(i) + "].Shininess").c_str()),
			Materials[i]->Shininess);
	}

	glUniform1i(ShaderObj->GetUniformLocation("numMaterials"),
		ShaderObj->TextureCount);

	//model matrix transforms model space to world space - rotation and translation
	glUniformMatrix4fv(ShaderObj->GetUniformLocation("model"), 1, GL_FALSE,
		glm::value_ptr(GetModelMatrix()));

	//Bind our VAO so we have the correct vertex attribute configuration
	glBindVertexArray(VertexArrayObj);
	//Draw! - type of primitive, starting index of vertex array, number of vertices
	if (Indices.size() > 0) glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
	else glDrawArrays(GL_TRIANGLES, 0, Vertices.size());
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}
