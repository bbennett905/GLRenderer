#include "Mesh.h"

#include <gtc\matrix_transform.hpp>

CMesh::CMesh() { }

CMesh::CMesh(std::vector<VertexData> & vert, std::vector<GLuint> & ind,
	std::vector<CMaterial *> & mats) :
	CBaseDrawable(vert, ind, mats)
{ }

glm::mat4 CMesh::GetModelMatrix()
{
	glm::mat4 model;
	model = glm::translate(model, _position);

	glm::vec3 ang = GetAngles();
	model = glm::rotate(model, glm::radians(ang.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(ang.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(ang.z), glm::vec3(0.0f, 0.0f, 1.0f));

	model = glm::scale(model, _scale);
	return model;
}
