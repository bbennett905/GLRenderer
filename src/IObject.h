#pragma once

#include <glm.hpp>

class IObject
{
public:
	virtual ~IObject() {}

	//The world position of this object
	virtual glm::vec3 GetPosition() = 0;
	//The euler angles of this object
	virtual glm::vec3 GetAngles() = 0;
	//The scale in 3 dimensions of this object
	virtual glm::vec3 GetScale() = 0;

	//The upwards vector from this object
	virtual glm::vec3 GetUp() = 0;
	//The rightwards vector from this object
	virtual glm::vec3 GetRight() = 0;
	//The forwards vector from this object
	virtual glm::vec3 GetForward() = 0;	
};