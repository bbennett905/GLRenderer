#include "BaseDrawable.h"

#include <gtc\matrix_transform.hpp>

BaseDrawable::BaseDrawable() { }

BaseDrawable::BaseDrawable(const GLfloat vertices[], int verticesSize,
	Shader * shad, Material & mat) :
	ShaderObj(shad)
{
	Materials.push_back(mat);
	for (int i = 0; i < verticesSize; i+=8)// ? ok
	{
		VertexData data = { glm::vec3(vertices[i], vertices[i+1], vertices[i+2]),	//Pos
							glm::vec3(vertices[i+3], vertices[i+4], vertices[i+5]),	//Norm
							glm::vec2(vertices[i+6], vertices[i+7]) };				//TexCoord
		Vertices.push_back(data);
	}
}

BaseDrawable::BaseDrawable(const GLfloat vertices[], int verticesSize, 
	Shader * shad, std::vector<Material> & mat) :
	ShaderObj(shad), Materials(mat)
{
	for (int i = 0; i < verticesSize; i += 8)// ? ok
	{
		VertexData data = { glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]),	//Pos
			glm::vec3(vertices[i + 3], vertices[i + 4], vertices[i + 5]),	//Norm
			glm::vec2(vertices[i + 6], vertices[i + 7]) };				//TexCoord
		Vertices.push_back(data);
	}
}

BaseDrawable::BaseDrawable(std::vector<VertexData> vert, Shader * shad) :
	Vertices(vert), ShaderObj(shad)
{ }

BaseDrawable::BaseDrawable(std::vector<VertexData> & vert, std::vector<GLuint> & ind, Shader * shad,
	std::vector<Material> & mat) :
	Vertices(vert), Indices(ind), ShaderObj(shad), Materials(mat)
{ }

glm::mat4 BaseDrawable::GetModelMatrix()
{
	return glm::mat4(); //Return an identity matrix, no transform
}
