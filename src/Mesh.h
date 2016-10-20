#pragma once
#include "BaseDrawable.h"
#include "BaseObject.h"

/*
 * Mesh object, used by Model class. Essentially combination of BaseDrawable and BaseObject
 */

class Mesh :
	public BaseDrawable, public BaseObject
{
public:
	Mesh();
	Mesh(std::vector<VertexData> & vert, std::vector<GLuint> & ind, 
		std::vector<Material *> & mats);
	glm::mat4 GetModelMatrix();
};