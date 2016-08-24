#pragma once
#include "BaseObject.h"

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

	LightDirectional(glm::vec3 col, float intens, glm::vec3 ambCol, float ambIntens) :
		BaseLight(col, intens), AmbientColor(ambCol), AmbientIntensity(ambIntens)
	{ }
};

class LightPoint :
	public BaseLight
{
public:
	//Attenuation values
	float Constant;
	float Linear;
	float Quadratic;

	LightPoint(glm::vec3 col, float intens, 
		float constant = 1.0f, float linear = 0.14f, float quad = 0.07f) :
		BaseLight(col, intens), Constant(constant), Linear(linear), Quadratic(quad)
	{ }
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

	LightSpot(glm::vec3 col, float intens, float cutoffIn, float cutoffOut,
		float constant = 1.0f, float linear = 0.14f, float quad = 0.07f) :
		BaseLight(col, intens), InnerCutOff(cutoffIn), OuterCutOff(cutoffOut),
		Constant(constant), Linear(linear), Quadratic(quad)
	{ }
};