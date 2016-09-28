#pragma once

#include <glm.hpp>
#include <vector>

#include "Texture.h"

/*
 * Struct with Textures and material properties
 */

//TODO we could load these from a file (like vmt)

struct Material
{
	//Primary texture
	Texture * DiffuseMap;
	//Texture for specular highlights
	Texture * SpecularMap;

	//Ambient reflection strength
	float AmbientStrength;
	//Diffuse reflection strength
	float DiffuseStrength;
	//Specular reflection strength
	float SpecularStrength;
	//Shininess exponent
	float Shininess;

	Material(Texture * tex1,
		Texture * specMap = nullptr,
		float ambStr = 0.5f, float difStr = 1.0f, float specStr = 1.0f, float shiny = 32.0f) : 
		DiffuseMap(tex1),
		SpecularMap(specMap),
		AmbientStrength(ambStr), DiffuseStrength(difStr), SpecularStrength(specStr),
		Shininess(shiny) { }
	Material() { }
	Material(const Material&) = default;
	Material& operator=(const Material&) = default;
};