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
	SDL_Surface* surface = TTF_RenderText_Blended(_font, buffer.c_str(), { 255, 255, 255, 255 });
	if (_materials[0]->DiffuseMap)
		delete _materials[0]->DiffuseMap;
	_materials[0]->DiffuseMap = new CTexture(surface, Texture_Translucent);

	autoPosition(surface->w, surface->h, 0, 0);
	SDL_FreeSurface(surface);
}

CFPSCounter::~CFPSCounter()
{
	TTF_CloseFont(_font);
}

void CFPSCounter::Update(double delta_time)
{
	std::string buffer;
	buffer = "FPS: " + std::to_string(1.0 / delta_time)
		+ " (" + std::to_string(delta_time) + " ms)";
	SDL_Surface* surface = TTF_RenderText_Blended(_font, buffer.c_str(), { 255, 255, 255, 255 });

	autoPosition(surface->w, surface->h, 0, 0);
	
	_materials[0]->DiffuseMap->Update(surface);
	SDL_FreeSurface(surface);
}
