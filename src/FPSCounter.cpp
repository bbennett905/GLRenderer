#include "FPSCounter.h"

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

#include "Material.h"
#include "Texture.h"
#include "Window.h"

CFPSCounter::CFPSCounter(CWindow * window) :
	CBaseUIElement(window)
{
	_font = TTF_OpenFont("C:/Windows/Fonts/Arial.ttf", 18);

	std::string buffer;
	buffer = "FPS: N/A (N/A ms)";
	_surface = TTF_RenderText_Blended(_font, buffer.c_str(), { 255, 255, 255, 255 });
	delete _texture;
	_texture = new CTexture(_surface, Texture_Translucent);

	autoPosition(_surface->w, _surface->h, 0, 0);
}

CFPSCounter::~CFPSCounter()
{
	SDL_FreeSurface(_surface);
	_surface = NULL;
}

void CFPSCounter::Update(double delta_time)
{
	SDL_FreeSurface(_surface);
	_surface = NULL;

	std::string buffer;
	buffer = "FPS: " + std::to_string(1.0 / delta_time)
		+ " (" + std::to_string(delta_time) + " ms)";
	_surface = TTF_RenderText_Blended(_font, buffer.c_str(), { 255, 255, 255, 255 });

	autoPosition(_surface->w, _surface->h, 0, 0);
	
	_texture->Update(_surface);
}
