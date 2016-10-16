#include "BaseObject.h"
#include <gtc/matrix_transform.hpp>

BaseObject::BaseObject(glm::vec3 pos, glm::vec3 ang)
{
	Position = pos;
	_angles = ang;
	Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	updateVectors();
}

BaseObject::~BaseObject()
{ }

glm::vec3 BaseObject::GetAngles() { return _angles; }
void BaseObject::SetAngles(glm::vec3 newAngles)
{
	// Setter/getter is used so we update Up, Front, Right vectors when this is changed
	_angles = newAngles;
	updateVectors();
}

//These getters are used so that these are read-only
glm::vec3 BaseObject::GetUp() { return _up; }
glm::vec3 BaseObject::GetRight() { return _right; }
glm::vec3 BaseObject::GetForward() { return _forward; }

void BaseObject::updateVectors()
{
	//TODO this does NOT account for roll!
	glm::vec3 front;
	front.x = cos(glm::radians(_angles.y)) * cos(glm::radians(_angles.x));
	front.y = sin(glm::radians(_angles.x));
	front.z = sin(glm::radians(_angles.y)) * cos(glm::radians(_angles.x));
	_forward = glm::normalize(front);
	//0,1,0 is the world up vector - I dont think this would ever need to be different
	_right = glm::normalize(glm::cross(_forward, glm::vec3(0.0f, 1.0f, 0.0f)));
	_up = glm::normalize(glm::cross(_right, _forward));
}
