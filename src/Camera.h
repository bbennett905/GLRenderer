#pragma once

#include <glm.hpp>

#include "BaseObject.h"

class CCamera :
	public CBaseObject
{
public:
	//FOV in degrees, ratio is aspect ratio
	CCamera(glm::vec3 pos, glm::vec3 ang, float FOV, float ratio);
	//Alternate constructor so that glm isnt necessary
	CCamera(float xpos = 0.0f, float ypos = 0.0f, float zpos = 0.0f, 
		float pitch = 0.0f, float yaw = -90.0f, float roll = 0.0f);

	//Returns the view transformation matrix for this camera
	glm::mat4 GetViewMatrix();
	//Returns the projection transformation matrix for this camera
	glm::mat4 GetProjMatrix();

	//Sets position
	void SetPosition(glm::vec3 pos);
	//Sets angles
	void SetAngles(glm::vec3 newAngles);

private:
	void updateMatrices();
	//Store these so we don't have to recalculate them every frame
	glm::mat4 _viewMatrix;
	glm::mat4 _projMatrix;

	//radians here
	float _fov;
	float _aspectRatio;
};

