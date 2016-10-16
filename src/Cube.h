#pragma once
#include "BaseDrawable.h"

/*
 * A 3D cube primitive
 */

class Cube :
	public BaseObject, public BaseDrawable //TODO inherit from Primitive class?
{
public:
	Cube(Material * mat);
	~Cube();

	glm::mat4 GetModelMatrix();
};

