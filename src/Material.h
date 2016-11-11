#pragma once

#include <glm.hpp>
#include <vector>

/*
 * Struct with Textures and material properties
 */

class Texture;

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
		float ambStr = 0.5f, float difStr = 1.0f, float specStr = 1.0f, float shiny = 64.0f) : 
		DiffuseMap(tex1),
		SpecularMap(specMap),
		AmbientStrength(ambStr), DiffuseStrength(difStr), SpecularStrength(specStr),
		Shininess(shiny) { }
	Material() { }
	Material(const Material&) = default;
	~Material() { }	//We cant destroy textures here, as there is no guarantee they are not shared
	Material& operator=(const Material&) = default;
};

struct MaterialCT //cook torrance
{
	glm::vec3 BaseColor;
	float Roughness;
	float Metallicity;
};