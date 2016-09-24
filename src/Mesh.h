#pragma once
#include "BaseDrawable.h"

class Mesh :
	public BaseDrawable, public BaseObject
{
public:
	Mesh(std::vector<VertexData> & vert, std::vector<GLuint> & ind, std::vector<Material> texts, Shader * shad);
	glm::mat4 GetModelMatrix();
};