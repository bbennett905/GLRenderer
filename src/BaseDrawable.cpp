#include "BaseDrawable.h"

#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Material.h"
#include "Shader.h"
#include "Lights.h"
#include "Texture.h"

CBaseDrawable::CBaseDrawable() 
{ 
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);
}

CBaseDrawable::CBaseDrawable(const GLfloat vertices[], int verticesSize, 
	std::vector<CMaterial *> & mat) :
	_materials(mat)
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

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

CBaseDrawable::CBaseDrawable(std::vector<VertexData> &vert) :
	_vertices(vert)
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
}

CBaseDrawable::CBaseDrawable(std::vector<VertexData> &vert, std::vector<GLuint> &ind,
	std::vector<CMaterial *> &mat) :
	_vertices(vert), _indices(ind), _materials(mat)
{ 
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	if (ind.size())
	{
		glGenBuffers(1, &_ebo);
	}
	else
	{
		_ebo = 0;
	}
	assert(mat.size() < MAX_MATERIALS && "An object exceeded MAX_MATERIALS!");
}

CBaseDrawable::~CBaseDrawable()
{
	//We DON'T destroy the shader, because they may be shared with other objects
	//Same with materials/textures
	if (_vao) glDeleteVertexArrays(1, &_vao);
	if (_vbo) glDeleteBuffers(1, &_vbo);
	if (_ebo) glDeleteBuffers(1, &_ebo);
}

glm::mat4 CBaseDrawable::GetModelMatrix()
{
	return glm::mat4(); //Return an identity matrix, no transform
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
