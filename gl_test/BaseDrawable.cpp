#include "BaseDrawable.h"

#include <gtc\matrix_transform.hpp>

BaseDrawable::BaseDrawable() { }

BaseDrawable::BaseDrawable(const GLfloat vertices[], int verticesSize,
	Material & mat) :
	MatObj(mat)
{
	for (int i = 0; i < verticesSize;)// ? ok
	{
		VertexData data = { glm::vec3(vertices[i++], vertices[i++], vertices[i++]),	//Pos
							glm::vec3(vertices[i++], vertices[i++], vertices[i++]),	//Norm
							glm::vec2(vertices[i++], vertices[i++]) };				//TexCoord
	}
	//Vertices = std::vector<GLfloat>(vertices, vertices + verticesSize);
	bUsesIndices = false; //TODO this is probably irrelevant
}

BaseDrawable::BaseDrawable(std::vector<VertexData> vert) :
	Vertices(vert)
{ }

BaseDrawable::BaseDrawable(std::vector<VertexData> vert, std::vector<GLuint> ind) :
	Vertices(vert), Indices(ind)
{ }

glm::mat4 BaseDrawable::GetModelMatrix()
{
	return glm::mat4(); //Return an identity matrix, no transform
}
