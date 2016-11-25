#include "BaseObject.h"

#include <gtc/matrix_transform.hpp>

CBaseObject::CBaseObject(glm::vec3 pos, glm::vec3 ang) :
	_position(pos), _angles(ang), _scale(glm::vec3(1.0f, 1.0f, 1.0f))
{
	updateVectors();
}

CBaseObject::~CBaseObject()
{ }

glm::vec3 CBaseObject::GetAngles() 
{
	return _angles;
}

void CBaseObject::SetAngles(glm::vec3 newAngles)
{
	// Setter/getter is used so we update Up, Front, Right vectors when this is changed
	_angles = newAngles;
	updateVectors();
}

glm::vec3 CBaseObject::GetPosition() 
{ 
	return _position;
}

void CBaseObject::SetPosition(glm::vec3 new_pos)
{
	_position = new_pos;
}

glm::vec3 CBaseObject::GetScale()
{
	return _scale;
}

void CBaseObject::SetScale(glm::vec3 new_scale)
{
	_scale = new_scale;
}

glm::vec3 CBaseObject::GetUp() { return _up; }
glm::vec3 CBaseObject::GetRight() { return _right; }
glm::vec3 CBaseObject::GetForward() { return _forward; }

void CBaseObject::updateVectors()
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
