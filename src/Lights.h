#pragma once
#include "BaseObject.h"

/*
 * Classes for various types of light entities
 */

class BaseLight :
	public BaseObject
{
public:
	glm::vec3 Color;
	float Intensity;

	BaseLight(glm::vec3 col, float intens) :
		Color(col), Intensity(intens)
	{ }
};

class LightDirectional :
	public BaseLight
{
public:
	glm::vec3 AmbientColor;
	float AmbientIntensity;

	LightDirectional(glm::vec3 ang, glm::vec3 col, float intens, glm::vec3 ambCol, float ambIntens) :
		BaseLight(col, intens), AmbientColor(ambCol), AmbientIntensity(ambIntens)
	{ 
		SetAngles(ang);
	}
};

class LightPoint :
	public BaseLight
{
public:
	//Attenuation values
	float Constant;
	float Linear;
	float Quadratic;

	LightPoint(glm::vec3 pos, glm::vec3 col, float intens, 
		float constant = 0.3f, float linear = 0.2f, float quad = 1.0f) :
		BaseLight(col, intens), Constant(constant), Linear(linear), Quadratic(quad)
	{ 
		Position = pos;
	}
};

class LightSpot :
	public BaseLight
{
public:
	//Attenuation values
	float Constant;
	float Linear;
	float Quadratic;

	//Angles
	float InnerCutOff;
	float OuterCutOff;

	LightSpot(glm::vec3 pos, glm::vec3 ang, glm::vec3 col, float intens, float cutoffIn, float cutoffOut,
		float constant = 0.3f, float linear = 0.2f, float quad = 1.0f) :
		BaseLight(col, intens), InnerCutOff(cutoffIn), OuterCutOff(cutoffOut),
		Constant(constant), Linear(linear), Quadratic(quad)
	{ 
		Position = pos;
		SetAngles(ang);
	}
};

//Other possible light types - spherical: point light with radius; linear: evenly along a line