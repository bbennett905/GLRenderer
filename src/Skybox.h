#pragma once
#include "BaseDrawable.h"
#include "BaseObject.h"

#include <string>

class Texture;

class Skybox :
	public BaseObject, public BaseDrawable
{
public:
	Skybox(std::string path);
	~Skybox();

	void Draw();

protected:
	Texture * _texture;
};

