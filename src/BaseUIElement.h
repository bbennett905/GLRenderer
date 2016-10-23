#pragma once
#include "BaseDrawable.h"

class BaseUIElement :
	public BaseDrawable
{
public:
	BaseUIElement(glm::vec2 pos = glm::vec2(0.0f, 0.0f), 
		glm::vec2 scale = glm::vec2(1.0f, 1.0f));
	virtual ~BaseUIElement();

	void Draw(Camera * camera, std::vector<LightPoint *> & point_light_list,
		std::vector<LightSpot *> & spot_light_list, LightDirectional * directional_light);
	glm::mat4 GetModelMatrix();

	glm::vec2 Scale;
	glm::vec2 Position;
};

