#pragma once

#include <glm.hpp>
#include <vector>

/*
 * Class with Textures and material properties
 */

class CTexture;

class CMaterial //cook torrance
{
public:
	CMaterial(CTexture * diffuse = nullptr, CTexture * mr = nullptr,
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f),
		float roughness = 1.0f, float metal = 1.0f);

	CMaterial(std::string path);

	CTexture * DiffuseMap;
	CTexture * MetalAndRoughMap; //metal is R, rough is G
	CTexture * NormalMap;
	glm::vec3 BaseColor;
	float Roughness;
	float Metallicity;

protected:
	void loadMaterial(std::string path);
};