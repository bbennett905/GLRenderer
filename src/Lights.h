#pragma once
#include "CBaseObject.h"

/*
 * Classes for various types of light entities
 */

class CBaseLight :
	public CBaseObject
{
public:
	glm::vec3 Color;
	float Intensity;

	CBaseLight(glm::vec3 col, float intens) :
		Color(col), Intensity(intens)
	{ }
};

class CLightDirectional :
	public CBaseLight
{
public:
	glm::vec3 AmbientColor;
	float AmbientIntensity;

	CLightDirectional(glm::vec3 ang, glm::vec3 col, float intens, glm::vec3 ambCol, float ambIntens) :
		CBaseLight(col, intens), AmbientColor(ambCol), AmbientIntensity(ambIntens)
	{ 
		SetAngles(ang);
	}
};

class CLightPoint :
	public CBaseLight
{
public:
	//Attenuation values
	float Constant;
	float Linear;
	float Quadratic;

	CLightPoint(glm::vec3 pos, glm::vec3 col, float intens, 
		float constant = 0.3f, float linear = 0.2f, float quad = 1.0f) :
		CBaseLight(col, intens), Constant(constant), Linear(linear), Quadratic(quad)
	{ 
		_position = pos;
	}
};

class CLightSpot :
	public CBaseLight
{
public:
	//Attenuation values
	float Constant;
	float Linear;
	float Quadratic;

	//Angles
	float InnerCutOff;
	float OuterCutOff;

	CLightSpot(glm::vec3 pos, glm::vec3 ang, glm::vec3 col, float intens, float cutoffIn, float cutoffOut,
		float constant = 0.3f, float linear = 0.2f, float quad = 1.0f) :
		CBaseLight(col, intens), InnerCutOff(cutoffIn), OuterCutOff(cutoffOut),
		Constant(constant), Linear(linear), Quadratic(quad)
	{ 
		_position = pos;
		SetAngles(ang);
	}
};

//Other possible light types - spherical: point light with radius; linear: evenly along a line