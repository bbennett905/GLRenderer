#include "Camera.h"

#include <gtc\matrix_transform.hpp>

Camera::Camera(glm::vec3 pos, glm::vec3 ang, float FOV, float ratio) :
	BaseObject(pos, ang), Position(pos), _fov(glm::radians(FOV)), _aspectRatio(ratio)
{
	updateMatrices();
}

Camera::Camera(GLfloat xpos, GLfloat ypos, GLfloat zpos, GLfloat pitch, GLfloat yaw, GLfloat roll) :
	BaseObject(glm::vec3(xpos, ypos, zpos), glm::vec3(pitch, yaw, roll))
{ }

glm::mat4 Camera::GetViewMatrix()
{
	return _viewMatrix;
}

glm::mat4 Camera::GetProjMatrix()
{
	return _projMatrix;
}

glm::vec3 Camera::GetPos()
{
	return Position;
}

void Camera::SetPos(glm::vec3 pos)
{
	Position = pos;
	updateMatrices();
}

void Camera::SetAngles(glm::vec3 newAngles)
{
	BaseObject::SetAngles(newAngles);
	updateMatrices();
}

void Camera::updateMatrices()
{
	_viewMatrix = glm::lookAt(Position, Position + GetForward(), GetUp());
	_projMatrix = glm::perspective(_fov, _aspectRatio, 0.1f, 1000.0f);
}


