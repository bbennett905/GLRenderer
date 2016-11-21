#pragma once

#include <glm.hpp>

#include "IObject.h"

/*
 * Class to inherit from for any object located in 3D space
 */

class CBaseObject :
	public IObject
{
public:
	CBaseObject(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), 
		       glm::vec3 ang = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~CBaseObject();

	//Returns the Euler angles of the	object - pitch, yaw, roll
	virtual glm::vec3 GetAngles();
	//Set the object's Euler angles - pitch, yaw, roll
	virtual void SetAngles(glm::vec3 new_ang);

	//The world position of this object
	virtual glm::vec3 GetPosition();
	//Sets the world position of this object
	virtual void SetPosition(glm::vec3 new_pos);

	//The scale in 3 dimensions of this object
	virtual glm::vec3 GetScale();
	//Sets the 3D scale of the object
	virtual void SetScale(glm::vec3 new_scale);

	//Returns the upwards vector from this object
	glm::vec3 GetUp();
	//Returns the rightwards vector from this object
	glm::vec3 GetRight();
	//Returns the forwards vector from this object
	glm::vec3 GetForward();

protected:
	//The Euler angles of the object - pitch, yaw, roll
	glm::vec3 _angles;
	//Position of this object in 3D space
	glm::vec3 _position;
	//Size scaling of the object in 3 dimensions
	glm::vec3 _scale;

	//Update direction vectors, called when angles are changed
	void updateVectors();

	//Up vector
	glm::vec3 _up;
	//Right vector
	glm::vec3 _right;
	//Forward vector
	glm::vec3 _forward;
};

