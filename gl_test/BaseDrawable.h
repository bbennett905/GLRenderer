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
	BaseDrawable(const GLfloat vertices[], int verticesSize,
		Material & mat);
	BaseDrawable(std::vector<VertexData> vert);
	BaseDrawable(std::vector<VertexData> vert, std::vector<GLuint> ind);

	//TODO this may need to be in a different place or done differently somehow
	virtual glm::mat4 GetModelMatrix();

	std::vector<VertexData> Vertices;
	std::vector<GLuint> Indices;

	Material MatObj;

	GLuint VertexArrayObj;
	GLuint VertexBufferObj;
	GLuint ElementBufferObj;

	bool bUsesIndices;
};
