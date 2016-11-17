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
{
}

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
		glUniform3f(ShaderObj->GetUniformLocation("materials[" + std::to_string(i) + "].Color"),
			Materials[i]->BaseColor.x, Materials[i]->BaseColor.y, Materials[i]->BaseColor.z);
		glUniform1f(ShaderObj->GetUniformLocation("materials[" + std::to_string(i) + "].Roughness"),
			Materials[i]->Roughness);
		glUniform1f(ShaderObj->GetUniformLocation("materials[" + std::to_string(i) + "].Metallicity"),
			Materials[i]->Metallicity);

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

		if (Materials[i]->MetalAndRoughMap != nullptr)
		{
			glUniform1i(ShaderObj->GetUniformLocation(
				("materials[" + std::to_string(i) + "].HasMetalAndRoughMap").c_str()), 1);
			glActiveTexture(GL_TEXTURE0 + ShaderObj->TextureCount);
			Materials[i]->MetalAndRoughMap->Bind();
			glUniform1i(ShaderObj->GetUniformLocation(
				("materials[" + std::to_string(i) + "].MetalAndRoughMap").c_str()),
				ShaderObj->TextureCount);
			ShaderObj->TextureCount++;
		}
		else
		{
			glUniform1i(ShaderObj->GetUniformLocation(
				("materials[" + std::to_string(i) + "].HasMetalAndRoughMap").c_str()), 0);
		}
	}
	//TODO wtf? thats not right at all
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
