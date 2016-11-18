#pragma once

#include <glm.hpp>
#include <vector>

/*
 * Class with Textures and material properties
 */

class Texture;

class Material //cook torrance
{
public:
	Material(Texture * diffuse = nullptr, Texture * mr = nullptr,
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f),
		float roughness = 1.0f, float metal = 1.0f);

	Material(std::string path);

	Texture * DiffuseMap;
	Texture * MetalAndRoughMap; //metal is R, rough is G
	glm::vec3 BaseColor;
	float Roughness;
	float Metallicity;

protected:
	void loadMaterial(std::string path);
};