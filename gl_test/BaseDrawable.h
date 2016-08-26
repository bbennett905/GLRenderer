#pragma once
#include "BaseObject.h"

#include <vector>
#include <glew.h>

#include "Material.h"

class BaseDrawable
{
public:
	BaseDrawable();
	//TODO more constructors
	BaseDrawable(const GLfloat vertices[], int verticesSize,
		Material & mat);
	BaseDrawable(const GLfloat vertices[], const GLuint indices[],
		Material & mat);

	virtual glm::mat4 GetModelMatrix()=0;

	std::vector<GLfloat> Vertices;
	std::vector<GLuint> Indices;

	Material MatObj;

	GLuint VertexArrayObj;
	GLuint VertexBufferObj;
	GLuint ElementBufferObj;

	bool bUsesIndices;
};

