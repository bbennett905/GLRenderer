#pragma once

#include <glm.hpp>
#include <glew.h>

#include "BaseObject.h"

class Camera :
	public BaseObject
{
public:
	//as far as i can tell, WorldUp has no reason to be set here, it should be always the same .. ?
	Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3 ang = glm::vec3(0.0f, 90.0f, 0.0f));
	//Alternate constructor so that glm isnt necessary
	Camera(GLfloat xpos = 0.0f, GLfloat ypos = 0.0f, GLfloat zpos = 0.0f, GLfloat pitch = 0.0f, GLfloat yaw = -90.0f, GLfloat roll = 0.0f);

	//Returns the view transformation matrix for this camera
	glm::mat4 GetViewMatrix();
};

