#include "BaseDrawable.h"

#include <gtc\matrix_transform.hpp>

BaseDrawable::BaseDrawable() { }

BaseDrawable::BaseDrawable(const GLfloat vertices[], int verticesSize,
	Material & mat) :
	MatObj(mat)
{
	Vertices = std::vector<GLfloat>(vertices, vertices + verticesSize);
	bUsesIndices = false;
}

BaseDrawable::BaseDrawable(const GLfloat vertices[], const GLuint indices[],
	Material & mat) :
	MatObj(mat)
{
	Vertices = std::vector<GLfloat>(vertices, vertices + sizeof(vertices) / sizeof(vertices[0]));
	Indices = std::vector<GLuint>(indices, indices + sizeof(indices) / sizeof(indices[0]));
	bUsesIndices = true;
}
