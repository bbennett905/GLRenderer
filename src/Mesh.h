#pragma once
#include "BaseDrawable.h"

/*
 * Mesh object, used by Model class. Essentially combination of BaseDrawable and BaseObject
 */

class Mesh :
	public BaseDrawable, public BaseObject
{
public:
	Mesh(std::vector<VertexData> & vert, std::vector<GLuint> & ind, 
		Shader * shad, std::vector<Material> texts);
	glm::mat4 GetModelMatrix();
};