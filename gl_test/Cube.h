#pragma once
#include "BaseDrawable.h"

class Cube :
	public BaseDrawable
{
public:
	Cube(glm::vec3 pos, glm::vec3 ang, Material & mat);

};

