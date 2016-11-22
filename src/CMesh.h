#pragma once
#include "CBaseDrawable.h"
#include "CBaseObject.h"

/*
 * Mesh object, used by Model class. Essentially combination of BaseDrawable and BaseObject
 */

class CMesh :
	public CBaseDrawable, public CBaseObject
{
public:
	CMesh();
	CMesh(std::vector<VertexData> & vert, std::vector<GLuint> & ind, 
		std::vector<CMaterial *> & mats);
	glm::mat4 GetModelMatrix();
};