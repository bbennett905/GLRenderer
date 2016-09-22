#pragma once
#include "BaseObject.h"

#include <vector>
#include <glew.h>

#include "Material.h"

struct VertexData
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class BaseDrawable
{
public:
	BaseDrawable();
	//TODO more constructors
	BaseDrawable(const GLfloat vertices[], int verticesSize, Shader * shad,
		Material & mat);
	BaseDrawable(const GLfloat vertices[], int verticesSize, Shader * shad,
		std::vector<Material> & mat);
	BaseDrawable(std::vector<VertexData> vert, Shader * shad);
	BaseDrawable(std::vector<VertexData> & vert, std::vector<GLuint> & ind, Shader * shad, std::vector<Material> & mat);

	virtual glm::mat4 GetModelMatrix();

	std::vector<VertexData> Vertices;
	std::vector<GLuint> Indices;

	Shader * ShaderObj;
	std::vector<Material> Materials;

	GLuint VertexArrayObj;
	GLuint VertexBufferObj;
	GLuint ElementBufferObj;

	bool bUsesIndices;
};
