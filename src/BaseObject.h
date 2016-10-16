#pragma once

//Use glm until separate vector classes made
#include <glm.hpp>

/*
 * Class to inherit from for any object located in 3D space
 */

class BaseObject
{
public:
	BaseObject(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), 
		       glm::vec3 ang = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~BaseObject();

	//Returns the Euler angles of the object - pitch, yaw, roll
	virtual glm::vec3 GetAngles();
	//Set the object's Euler angles - pitch, yaw, roll
	virtual void SetAngles(glm::vec3 newAngles);

	//Position of this object in 3D space
	glm::vec3 Position;

	//Size scaling of the object in 3 dimensions
	glm::vec3 Scale;

	//Returns the upwards vector from this object
	glm::vec3 GetUp();
	//Returns the rightwards vector from this object
	glm::vec3 GetRight();
	//Returns the forwards vector from this object
	glm::vec3 GetForward();

private:
	//The Euler angles of the object - pitch, yaw, roll
	glm::vec3 _angles;

	//Update direction vectors, called when angles are changed
	void updateVectors();

	//Up vector
	glm::vec3 _up;
	//Right vector
	glm::vec3 _right;
	//Forward vector
	glm::vec3 _forward;
};

