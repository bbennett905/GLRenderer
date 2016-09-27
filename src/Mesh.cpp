#include "Mesh.h"

#include <gtc\matrix_transform.hpp>

Mesh::Mesh(std::vector<VertexData> & vert, std::vector<GLuint> & ind, std::vector<Material> texts, Shader * shad) :
	BaseDrawable(vert, ind, shad, texts)
{ }

glm::mat4 Mesh::GetModelMatrix()
{
	glm::mat4 model;
	model = glm::translate(model, Position);

	glm::vec3 ang = GetAngles();
	model = glm::rotate(model, glm::radians(ang.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(ang.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(ang.z), glm::vec3(0.0f, 0.0f, 1.0f));

	return model;
}