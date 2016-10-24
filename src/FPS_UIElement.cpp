#include "FPS_UIElement.h"

#include <string>

#include "Material.h"
#include "Texture.h"
#include "Window.h"

FPS_UIElement::FPS_UIElement(Window * window) :
	BaseUIElement(window)
{
	TTF_Init();
	_font = TTF_OpenFont("C:/Windows/Fonts/Arial.ttf", 18);

	std::string buffer;
	buffer = "FPS: N/A (N/A ms)";
	_surface = TTF_RenderText_Solid(_font, buffer.c_str(), { 255, 255, 255, 255 });
	_surface = SDL_ConvertSurfaceFormat(_surface, SDL_PIXELFORMAT_RGBA8888, 0);
	delete _texture;
	_texture = new Texture(_surface, Texture_Translucent);

	autoPosition(_surface->w, _surface->h, 0, 0);
}

FPS_UIElement::~FPS_UIElement()
{
	SDL_FreeSurface(_surface);
	delete _texture;
}

void FPS_UIElement::Update(double delta_time)
{
	SDL_FreeSurface(_surface);

	std::string buffer;
	buffer = "FPS: " + std::to_string(1.0 / delta_time)
		+ " (" + std::to_string(delta_time) + " ms)";
	_surface = TTF_RenderText_Solid(_font, buffer.c_str(), { 255, 255, 255, 255 });
	_surface = SDL_ConvertSurfaceFormat(_surface, SDL_PIXELFORMAT_RGBA8888, 0);

	autoPosition(_surface->w, _surface->h, 0, 0);
	
	_texture->Update(_surface);
}
