#pragma once
#include "BaseObject.h"
class LightSimple :
	public BaseObject
{
public:
	LightSimple(glm::vec3 pos, 
		glm::vec3 lightColor, float lightIntens, 
		glm::vec3 ambColor, float ambIntens);

	glm::vec3 LightColor;
	float LightIntensity;

	glm::vec3 AmbientColor;
	float AmbientIntensity;
	
};

