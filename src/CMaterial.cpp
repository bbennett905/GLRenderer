#include "CMaterial.h"

#include <fstream>
#include <iostream>
#include <glm.hpp>

#include "Logging.h"
#include "CTexture.h"
#include "Utils.h"

CMaterial::CMaterial(CTexture * diffuse, CTexture * mr, glm::vec3 color, float roughness, float metal) :
	DiffuseMap(diffuse), MetalAndRoughMap(mr), BaseColor(color),
	Roughness(roughness), Metallicity(metal) { }

CMaterial::CMaterial(std::string path)
{
	loadMaterial(path);
}

void CMaterial::loadMaterial(std::string path)
{
	std::ifstream materialFile(path.c_str(), std::fstream::in);
	if (materialFile.fail())
	{
		Logging::LogMessage(LogLevel_Error, "Failed loading material file %s", path);
		return;
	}
	char buffer[512];

	std::string diffusePath = "";
	uint32_t texFlags = 0;
	std::string mrPath = "";
	glm::vec3 baseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	float rough = 1.0f;
	float metal = 1.0f;

	while (materialFile.good())
	{
		materialFile.getline(buffer, 512);
		std::string line = buffer;

		uint32_t loc = line.find("[DiffuseMap]");
		if (loc != std::string::npos)
		{
			diffusePath = line.substr(loc + strlen("[DiffuseMap]"), std::string::npos);
		}
		loc = line.find("[MRMap]");
		if (loc != std::string::npos)
		{
			mrPath = line.substr(loc + strlen("[MRMap]"), std::string::npos);
		}
		loc = line.find("[BaseColor]");
		if (loc != std::string::npos)
		{
			std::vector<std::string> splitBuffer = 
				Split(line.substr(loc + strlen("[BaseColor]"), std::string::npos).c_str(), ' ');
			baseColor.x = float(atof(splitBuffer[0].c_str()));
			baseColor.y = float(atof(splitBuffer[1].c_str()));
			baseColor.z = float(atof(splitBuffer[2].c_str()));
		}
		loc = line.find("[Roughness]");
		if (loc != std::string::npos)
		{
			rough = float(atof(line.substr(loc + strlen("[Roughness]"), std::string::npos).c_str()));
		}
		loc = line.find("[Metallicity]");
		if (loc != std::string::npos)
		{
			metal = float(atof(line.substr(loc + strlen("[Metallicity]"), std::string::npos).c_str()));
		}
		loc = line.find("[Translucent]");
		if (loc != std::string::npos)
		{
			texFlags |= Texture_Translucent;
		}
	}

	if (diffusePath == "")
	{
		DiffuseMap = nullptr;
	}
	else
	{
		DiffuseMap = CTexture::TextureExists(diffusePath);
		if (!DiffuseMap) DiffuseMap = new CTexture(diffusePath, texFlags);
	}
	if (mrPath == "")
	{
		MetalAndRoughMap = nullptr;
	}
	else
	{
		MetalAndRoughMap = CTexture::TextureExists(mrPath);
		if (!MetalAndRoughMap) MetalAndRoughMap = new CTexture(mrPath);
	}
	BaseColor = baseColor;
	Roughness = rough;
	Metallicity = metal;

	Logging::LogMessage(LogLevel_Debug, "Loaded material file %s", path);
}
