#pragma once
#include "BaseDrawable.h"

class Texture;
class Window;

class BaseUIElement :
	public BaseDrawable
{
public:
	BaseUIElement(Window * window, glm::vec2 pos = glm::vec2(0.0f, 0.0f),
		glm::vec2 scale = glm::vec2(1.0f, 1.0f));
	virtual ~BaseUIElement();

	void Draw();
	glm::mat4 GetModelMatrix();

	glm::vec2 Scale;
	glm::vec2 Position;

protected:
	//Automatically scales and positions an element, given its width, height, 
	//and the x, y coords of the top left - in pixels, counting from top left
	void autoPosition(int width, int height, int x, int y);
	Texture * _texture;
	Window * _window;

private:
	static bool _is_ttf_init;
};

