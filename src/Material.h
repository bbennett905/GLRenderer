#pragma once

#include <glm.hpp>
#include <vector>

/*
 * Struct with Textures and material properties
 */

class Texture;

//TODO we could load these from a file (like vmt)

struct Material //cook torrance
{
	Texture * DiffuseMap;
	Texture * MetalAndRoughMap; //metal is R, rough is G
	glm::vec3 BaseColor;
	float Roughness;
	float Metallicity;

	Material(Texture * diffuse = nullptr, Texture * mr = nullptr,
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f),
		float roughness = 1.0f, float metal = 1.0f) :
		DiffuseMap(diffuse), MetalAndRoughMap(mr), BaseColor(color),
		Roughness(roughness), Metallicity(metal) { }
};