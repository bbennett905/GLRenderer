#pragma once
#include "CBaseUIElement.h"

struct SDL_Surface;
typedef struct _TTF_Font TTF_Font;

class CFPSCounter :
	public CBaseUIElement
{
public:
	CFPSCounter(Window * window);
	~CFPSCounter();

	void Update(double delta_time);

private:
	TTF_Font * _font;
	SDL_Surface * _surface;
};

