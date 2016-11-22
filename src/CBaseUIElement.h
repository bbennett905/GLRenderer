#pragma once
#include "CBaseDrawable.h"
#include "CBaseObject.h"

class CTexture;
class Window;

class CBaseUIElement :
	public CBaseDrawable, public CBaseObject
{
public:
	CBaseUIElement(Window * window, glm::vec2 pos = glm::vec2(0.0f, 0.0f),
		glm::vec2 scale = glm::vec2(1.0f, 1.0f));
	virtual ~CBaseUIElement();

	virtual void Draw();
	virtual glm::mat4 GetModelMatrix();

protected:
	//Automatically scales and positions an element, given its width, height, 
	//and the x, y coords of the top left - in pixels, counting from top left
	void autoPosition(int width, int height, int x, int y);
	CTexture * _texture;
	Window * _window;

private:
	static bool _is_ttf_init;
};

