#pragma once
#include "BaseObject.h"

#include <vector>
#include <glew.h>

#include "Material.h"

//Eventually, these should be passed to the renderer in an array or vector,
//but not yet
class BaseDrawable :
	public BaseObject
{
public:
	BaseDrawable();
	BaseDrawable(glm::vec3 pos, glm::vec3 ang,
		const GLfloat vertices[], int verticesSize,
		Material & mat);
	BaseDrawable(glm::vec3 pos, glm::vec3 ang, 
		const GLfloat vertices[], const GLuint indices[],
		Material & mat);

	virtual glm::mat4 GetModelMatrix();

	std::vector<GLfloat> Vertices;
	std::vector<GLuint> Indices;

	Material MatObj;

	GLuint VertexArrayObj;
	GLuint VertexBufferObj;
	GLuint ElementBufferObj;

	bool bUsesIndices;
};

