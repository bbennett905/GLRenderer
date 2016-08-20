#include "LightSimple.h"


LightSimple::LightSimple(glm::vec3 pos, 
	glm::vec3 lightColor, float lightIntens, 
	glm::vec3 ambColor, float ambIntens) :
	BaseObject(pos, glm::vec3(0.0f, 0.0f, 0.0f)), 
	LightColor(lightColor), LightIntensity(lightIntens),
	AmbientColor(ambColor), AmbientIntensity(ambIntens)
{
}
