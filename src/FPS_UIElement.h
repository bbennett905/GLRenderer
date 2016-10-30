#pragma once
#include "BaseUIElement.h"

#include <SDL.h>
#include <SDL_ttf.h>

class FPS_UIElement :
	public BaseUIElement
{
public:
	FPS_UIElement(Window * window);
	~FPS_UIElement();

	void Update(double delta_time);

private:
	TTF_Font * _font;
	SDL_Surface * _surface;
};

