#pragma once
#include "CBaseDrawable.h"
#include "CBaseObject.h"

#include <string>

class Texture;

class Skybox :
	public CBaseObject, public CBaseDrawable
{
public:
	Skybox(std::string path);
	~Skybox();

	void Draw();

protected:
	Texture * _texture;
};

