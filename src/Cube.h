#pragma once
#include "BaseDrawable.h"

/*
 * A 3D cube that can be scaled 
 */

class Cube :
	public BaseDrawable, public BaseObject //TODO inherit from Primitive class?
{
public:
	Cube(glm::vec3 pos, glm::vec3 ang, Shader * shad, Material & mat);
	glm::mat4 GetModelMatrix();
	glm::vec3 Scale;
};

