#include "Camera.h"

#include <gtc\matrix_transform.hpp>

Camera::Camera(glm::vec3 pos, glm::vec3 ang) :
	BaseObject(pos, ang)
{ }

Camera::Camera(GLfloat xpos, GLfloat ypos, GLfloat zpos, GLfloat pitch, GLfloat yaw, GLfloat roll) :
	BaseObject(glm::vec3(xpos, ypos, zpos), glm::vec3(pitch, yaw, roll))
{ }

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + GetForward(), GetUp());
}
