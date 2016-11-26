#include "BaseDrawable.h"

#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Material.h"
#include "Shader.h"
#include "Lights.h"
#include "Texture.h"

CBaseDrawable::CBaseDrawable() { }

CBaseDrawable::CBaseDrawable(const GLfloat vertices[], int verticesSize, CMaterial* mat)
{
	_materials.push_back(mat);
	for (int i = 0; i < verticesSize; i += 8)// ? ok
	{
		VertexData data = { glm::vec3(vertices[i], vertices[i+1], vertices[i+2]),	//Pos
							glm::vec3(vertices[i+3], vertices[i+4], vertices[i+5]),	//Norm
							glm::vec2(vertices[i+6], vertices[i+7]) };				//TexCoord
		_vertices.push_back(data);
	}

	//Pass to calc tangents
	for (uint32_t i = 0; i < _vertices.size() - 2; i += 3) //we do this for each triangle (3 verts
	{
		glm::vec3 edge1 = _vertices[i + 1].Position - _vertices[i].Position;
		glm::vec3 edge2 = _vertices[i + 2].Position - _vertices[i].Position;
		glm::vec2 deltaUV1 = _vertices[i + 1].TexCoords - _vertices[i].TexCoords;
		glm::vec2 deltaUV2 = _vertices[i + 2].TexCoords - _vertices[i].TexCoords;
		GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		glm::vec3 tangent;
		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent = glm::normalize(tangent);
		_vertices[i].Tangent = tangent;
		_vertices[i + 1].Tangent = tangent;
		_vertices[i + 2].Tangent = tangent;

		glm::vec3 bitangent;
		bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent = glm::normalize(bitangent);
		_vertices[i].BiTangent = bitangent;
		_vertices[i + 1].BiTangent = bitangent;
		_vertices[i + 2].BiTangent = bitangent;
	}
}

CBaseDrawable::CBaseDrawable(const GLfloat vertices[], int verticesSize, 
	std::vector<CMaterial *> & mat) :
	_materials(mat)
{
	for (int i = 0; i < verticesSize; i += 8)// ? ok
	{
		
		VertexData data = { glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]),	//Pos
			glm::vec3(vertices[i + 3], vertices[i + 4], vertices[i + 5]),	//Norm
			glm::vec2(vertices[i + 6], vertices[i + 7]) };				//TexCoord
		_vertices.push_back(data);
	}

	//Pass to calc tangents
	for (uint32_t i = 0; i < _vertices.size() - 2; i += 3) //we do this for each triangle (3 verts
	{
		glm::vec3 edge1 = _vertices[i + 1].Position - _vertices[i].Position;
		glm::vec3 edge2 = _vertices[i + 2].Position - _vertices[i].Position;
		glm::vec2 deltaUV1 = _vertices[i + 1].TexCoords - _vertices[i].TexCoords;
		glm::vec2 deltaUV2 = _vertices[i + 2].TexCoords - _vertices[i].TexCoords;
		GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		glm::vec3 tangent;
		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent = glm::normalize(tangent);
		_vertices[i].Tangent = tangent;
		_vertices[i + 1].Tangent = tangent;
		_vertices[i + 2].Tangent = tangent;

		glm::vec3 bitangent;
		bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent = glm::normalize(bitangent);
		_vertices[i].BiTangent = bitangent;
		_vertices[i + 1].BiTangent = bitangent;
		_vertices[i + 2].BiTangent = bitangent;
	}
}

CBaseDrawable::CBaseDrawable(std::vector<VertexData> vert) :
	_vertices(vert)
{
}

CBaseDrawable::CBaseDrawable(std::vector<VertexData> & vert, std::vector<GLuint> & ind,
	std::vector<CMaterial *> & mat) :
	_vertices(vert), _indices(ind), _materials(mat)
{ 
	assert(mat.size() < MAX_MATERIALS && "An object exceeded MAX_MATERIALS!");
}

CBaseDrawable::~CBaseDrawable()
{
	//We DON'T destroy the shader, because they may be shared with other objects
	//Same with materials/textures
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
}

glm::mat4 CBaseDrawable::GetModelMatrix()
{
	return glm::mat4(); //Return an identity matrix, no transform
}

void CBaseDrawable::Draw()
{
	_shader->Use();

	if (_materials.size())
	{
		glUniform1i(_shader->GetUniformLocation("hasMaterials"), 1);
	}
	else
	{
		glUniform1i(_shader->GetUniformLocation("hasMaterials"), 0);
	}
	for (uint32_t i = 0; i < _materials.size(); i++)
	{
		glUniform3f(_shader->GetUniformLocation("materials[" + std::to_string(i) + "].Color"),
			_materials[i]->BaseColor.x, _materials[i]->BaseColor.y, _materials[i]->BaseColor.z);
		glUniform1f(_shader->GetUniformLocation("materials[" + std::to_string(i) + "].Roughness"),
			_materials[i]->Roughness);
		glUniform1f(_shader->GetUniformLocation("materials[" + std::to_string(i) + "].Metallicity"),
			_materials[i]->Metallicity);

		if (_materials[i]->DiffuseMap != nullptr)
		{
			glUniform1i(_shader->GetUniformLocation(
				("materials[" + std::to_string(i) + "].HasDiffMap").c_str()), 1);
			glActiveTexture(GL_TEXTURE0 + _shader->TextureCount);
			_materials[i]->DiffuseMap->Bind();
			glUniform1i(_shader->GetUniformLocation(
				("materials[" + std::to_string(i) + "].DiffMap").c_str()),
				_shader->TextureCount);
			_shader->TextureCount++;
		}
		else
		{
			glUniform1i(_shader->GetUniformLocation(
				("materials[" + std::to_string(i) + "].HasDiffMap").c_str()), 0);
		}

		if (_materials[i]->MetalAndRoughMap != nullptr)
		{
			glUniform1i(_shader->GetUniformLocation(
				("materials[" + std::to_string(i) + "].HasMetalAndRoughMap").c_str()), 1);
			glActiveTexture(GL_TEXTURE0 + _shader->TextureCount);
			_materials[i]->MetalAndRoughMap->Bind();
			glUniform1i(_shader->GetUniformLocation(
				("materials[" + std::to_string(i) + "].MetalAndRoughMap").c_str()),
				_shader->TextureCount);
			_shader->TextureCount++;
		}
		else
		{
			glUniform1i(_shader->GetUniformLocation(
				("materials[" + std::to_string(i) + "].HasMetalAndRoughMap").c_str()), 0);
		}

		if (_materials[i]->NormalMap != nullptr)
		{
			glUniform1i(_shader->GetUniformLocation(
				("materials[" + std::to_string(i) + "].HasNormalMap").c_str()), 1);
			glActiveTexture(GL_TEXTURE0 + _shader->TextureCount);
			_materials[i]->NormalMap->Bind();
			glUniform1i(_shader->GetUniformLocation(
				("materials[" + std::to_string(i) + "].NormalMap").c_str()),
				_shader->TextureCount);
			_shader->TextureCount++;
		}
		else
		{
			glUniform1i(_shader->GetUniformLocation(
				("materials[" + std::to_string(i) + "].HasNormalMap").c_str()), 0);
		}
	}
	glUniform1i(_shader->GetUniformLocation("numMaterials"),
		_materials.size());

	//model matrix transforms model space to world space - rotation and translation
	glUniformMatrix4fv(_shader->GetUniformLocation("model"), 1, GL_FALSE,
		glm::value_ptr(GetModelMatrix()));

	//Bind our VAO so we have the correct vertex attribute configuration
	glBindVertexArray(_vao);
	//Draw! - type of primitive, starting index of vertex array, number of vertices
	if (_indices.size() > 0) glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	else glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

std::vector<VertexData>& CBaseDrawable::GetVertices()
{
	return _vertices;
}

std::vector<GLuint>& CBaseDrawable::GetIndices()
{
	return _indices;
}

std::vector<CMaterial*>& CBaseDrawable::GetMaterials()
{
	return _materials;
}

CShader*& CBaseDrawable::GetShader()
{
	return _shader;
}

GLuint& CBaseDrawable::VAO()
{
	return _vao;
}

GLuint& CBaseDrawable::VBO()
{
	return _vbo;
}

GLuint& CBaseDrawable::EBO()
{
	return _ebo;
}

uint32_t& CBaseDrawable::DrawFlags()
{
	return _flags;
}
