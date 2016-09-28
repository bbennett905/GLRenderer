#pragma once
#include "BaseObject.h"

#include <vector>
#include <glew.h>

#include "Material.h"
#include "Shader.h"

/*
 * Class that any 3D drawable object inherits from
 */

struct VertexData
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

//TODO add scale member, see Cube class for implementation

class BaseDrawable
{
public:
	BaseDrawable();
	BaseDrawable(const GLfloat vertices[], int verticesSize, Shader * shad,
		Material & mat);
	BaseDrawable(const GLfloat vertices[], int verticesSize, Shader * shad,
		std::vector<Material> & mat);
	BaseDrawable(std::vector<VertexData> vert, Shader * shad);
	BaseDrawable(std::vector<VertexData> & vert, std::vector<GLuint> & ind, Shader * shad, std::vector<Material> & mat);

	//Returns the model transformation matrix this object should use
	virtual glm::mat4 GetModelMatrix();

	//List of Vertices of the object
	std::vector<VertexData> Vertices;
	//List of Indices, if used by the object
	std::vector<GLuint> Indices;
	//List of Materials this obj uses
	std::vector<Material> Materials;

	//The shader that this object uses
	Shader * ShaderObj;

	//OpenGL Vertex Array Object
	GLuint VertexArrayObj;
	//OpenGL Vertex Buffer Object
	GLuint VertexBufferObj;
	//OpenGL Element Buffer Object
	GLuint ElementBufferObj;
};
