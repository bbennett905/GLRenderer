#pragma once

#include <glm.hpp>
#include <glew.h>

#include "BaseObject.h"

class Camera :
	public BaseObject
{
public:
	//as far as i can tell, WorldUp has no reason to be set here, it should be always the same .. ?
	//FOV in degrees
	Camera(glm::vec3 pos, glm::vec3 ang, float FOV, float ratio);
	//Alternate constructor so that glm isnt necessary
	Camera(GLfloat xpos = 0.0f, GLfloat ypos = 0.0f, GLfloat zpos = 0.0f, GLfloat pitch = 0.0f, GLfloat yaw = -90.0f, GLfloat roll = 0.0f);

	//Returns the view transformation matrix for this camera
	glm::mat4 GetViewMatrix();
	//Returns the projection transformation matrix for this camera
	glm::mat4 GetProjMatrix();

	glm::vec3 GetPos();
	void SetPos(glm::vec3 pos);
	void SetAngles(glm::vec3 newAngles);

private:
	void updateMatrices();
	//Store these so we don't have to recalculate them every frame
	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;

	//TODO test this, make sure it is private!
	glm::vec3 Position;
	//radians here
	float fov;
	float aspectRatio;
};

