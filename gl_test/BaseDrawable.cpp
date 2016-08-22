#include "BaseDrawable.h"

#include <gtc\matrix_transform.hpp>

BaseDrawable::BaseDrawable() { }

BaseDrawable::BaseDrawable(glm::vec3 pos, glm::vec3 ang, 
	const GLfloat vertices[], int verticesSize,
	Material & mat) :
	BaseObject(pos, ang), MatObj(mat)
{
	Vertices = std::vector<GLfloat>(vertices, vertices + verticesSize);
	bUsesIndices = false;
}

BaseDrawable::BaseDrawable(glm::vec3 pos, glm::vec3 ang, 
	const GLfloat vertices[], const GLuint indices[],
	Material & mat) :
	BaseObject(pos, ang), MatObj(mat)
{
	Vertices = std::vector<GLfloat>(vertices, vertices + sizeof(vertices) / sizeof(vertices[0]));
	Indices = std::vector<GLuint>(indices, indices + sizeof(indices) / sizeof(indices[0]));
	bUsesIndices = true;
}

glm::mat4 BaseDrawable::GetModelMatrix()
{
	//TODO cache this, add bIsStatic to determine if this is always updated or cached?
	glm::mat4 model;
	model = glm::translate(model, Position);

	glm::vec3 ang = GetAngles();
	model = glm::rotate(model, glm::radians(ang.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(ang.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(ang.z), glm::vec3(0.0f, 0.0f, 1.0f));
	return model;
}
