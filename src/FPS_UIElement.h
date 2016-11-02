#pragma once
#include "BaseUIElement.h"

struct SDL_Surface;
typedef struct _TTF_Font TTF_Font;

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

