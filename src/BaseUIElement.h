#pragma once
#include "BaseDrawable.h"
#include "BaseObject.h"

class CBaseUIElement :
	public CBaseDrawable, public CBaseObject
{
public:
	CBaseUIElement(glm::vec2 pos = glm::vec2(0.0f, 0.0f),
		glm::vec2 scale = glm::vec2(1.0f, 1.0f));
	virtual ~CBaseUIElement();

	virtual glm::mat4 GetModelMatrix();

protected:
	//Automatically scales and positions an element, given its width, height, 
	//and the x, y coords of the top left - in pixels, counting from top left
	void autoPosition(int width, int height, int x, int y);

private:
	static bool _is_ttf_init;
};

