#pragma once

#include <vector>
#include <glm.hpp>

/*
 * Interface for drawable objects
 */

typedef unsigned int GLuint;

class CMaterial;
class CShader;

struct VertexData
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 BiTangent;
};

enum DrawableFlags
{
	Drawable_Translucent = 1 << 0,
	Drawable_Unlit = 1 << 1,
	Drawable_UI = 1 << 2,
	Drawable_Skybox = 1 << 3
};

class IDrawable
{
public:
	virtual ~IDrawable() {}

	//The model transform matrix
	virtual glm::mat4 GetModelMatrix() = 0;

	//Vertex data (pos, normal, texture coords)
	virtual std::vector<VertexData>& GetVertices() = 0;
	//Vertex indices
	virtual std::vector<GLuint>& GetIndices() = 0;
	//Materials used to draw this
	virtual std::vector<CMaterial *>& GetMaterials() = 0;

	//Returns a pointer to the shader object to be used
	virtual CShader*& GetShader() = 0;

	//Vertex array object
	virtual GLuint& VAO() = 0;
	//Vertex buffer object
	virtual GLuint& VBO() = 0;
	//Element buffer object
	virtual GLuint& EBO() = 0;

	//Bit flags indicating special draw considerations
	virtual uint32_t& DrawFlags() = 0;
};