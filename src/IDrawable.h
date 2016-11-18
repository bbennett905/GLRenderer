#pragma once

#include <glm.hpp>

class IDrawable
{
public:
	virtual ~IDrawable() {}
	virtual void Draw() = 0;
	virtual glm::mat4 GetModelMatrix() = 0;
};