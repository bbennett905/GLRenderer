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

	void Draw(Camera * camera);

protected:
	Texture * _texture;
};

