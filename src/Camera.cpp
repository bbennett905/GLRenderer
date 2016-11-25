#include "Camera.h"

#include <gtc\matrix_transform.hpp>

CCamera::CCamera(glm::vec3 pos, glm::vec3 ang, float FOV, float ratio) :
	CBaseObject(pos, ang), _fov(glm::radians(FOV)), _aspectRatio(ratio)
{
	updateMatrices();
}

CCamera::CCamera(GLfloat xpos, GLfloat ypos, GLfloat zpos, GLfloat pitch, GLfloat yaw, GLfloat roll) :
	CBaseObject(glm::vec3(xpos, ypos, zpos), glm::vec3(pitch, yaw, roll))
{ }

glm::mat4 CCamera::GetViewMatrix()
{
	return _viewMatrix;
}

glm::mat4 CCamera::GetProjMatrix()
{
	return _projMatrix;
}

void CCamera::SetPosition(glm::vec3 pos)
{
	_position = pos;
	updateMatrices();
}

void CCamera::SetAngles(glm::vec3 newAngles)
{
	CBaseObject::SetAngles(newAngles);
	updateMatrices();
}

void CCamera::updateMatrices()
{
	_viewMatrix = glm::lookAt(_position, _position + GetForward(), GetUp());
	_projMatrix = glm::perspective(_fov, _aspectRatio, 0.01f, 1000.0f);
}
