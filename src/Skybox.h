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

	void Draw(Camera * camera,
		std::vector<LightPoint *> & point_light_list,
		std::vector<LightSpot *> & spot_light_list,
		LightDirectional * directional_light);

protected:
	Texture * _texture;
};

