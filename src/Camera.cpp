#include "Camera.h"

#include <gtc\matrix_transform.hpp>

Camera::Camera(glm::vec3 pos, glm::vec3 ang, float FOV, float ratio) :
	CBaseObject(pos, ang), _fov(glm::radians(FOV)), _aspectRatio(ratio)
{
	updateMatrices();
}

Camera::Camera(GLfloat xpos, GLfloat ypos, GLfloat zpos, GLfloat pitch, GLfloat yaw, GLfloat roll) :
	CBaseObject(glm::vec3(xpos, ypos, zpos), glm::vec3(pitch, yaw, roll))
{ }

glm::mat4 Camera::GetViewMatrix()
{
	return _viewMatrix;
}

glm::mat4 Camera::GetProjMatrix()
{
	return _projMatrix;
}

void Camera::SetPosition(glm::vec3 pos)
{
	_position = pos;
	updateMatrices();
}

void Camera::SetAngles(glm::vec3 newAngles)
{
	CBaseObject::SetAngles(newAngles);
	updateMatrices();
}

void Camera::updateMatrices()
{
	_viewMatrix = glm::lookAt(_position, _position + GetForward(), GetUp());
	_projMatrix = glm::perspective(_fov, _aspectRatio, 0.01f, 1000.0f);
}
