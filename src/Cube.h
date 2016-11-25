#pragma once
#include "BaseDrawable.h"
#include "BaseObject.h"

/*
 * A 3D cube primitive
 */

class CCube :
	public CBaseObject, public CBaseDrawable //TODO inherit from Primitive class?
{
public:
	CCube(CMaterial * mat);
	~CCube();

	glm::mat4 GetModelMatrix();
};

