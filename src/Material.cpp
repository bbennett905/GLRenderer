#include "Material.h"

#include <fstream>
#include <iostream>
#include <glm.hpp>
#include <document.h>

#include "Logging.h"
#include "Texture.h"
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
		Logging::LogMessage(LogLevel_Error, "Failed opening material file %s", path.c_str());
		return;
	}
	
	char json[4096];
	materialFile.read(json, sizeof(json));
	
	if (!materialFile.eof())
	{
		//Either file was too large, or some other error occured
		Logging::LogMessage(LogLevel_Error, "Failed loading material file %s", path.c_str());
		materialFile.close();
		return;
	}
	json[materialFile.gcount()] = '\0'; //Add a null terminator for rapidjson..
	materialFile.close();

	rapidjson::Document doc;
	if (doc.Parse(json).HasParseError())
	{
		int err = doc.GetParseError();
		int offs = doc.GetErrorOffset();
		Logging::LogMessage(LogLevel_Error, "Failed parsing material file %s", path.c_str());
		return;
	}

	//Defaults
	std::string diffusePath = "";
	uint32_t texFlags = 0;
	std::string mrPath = "";
	std::string normPath = "";
	glm::vec3 baseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	float rough = 1.0f;
	float metal = 1.0f;

	for (rapidjson::Value::ConstMemberIterator i = doc.MemberBegin(); i != doc.MemberEnd(); i++)
	{
		std::string s = i->name.GetString();
		rapidjson::Value &v = doc[s.c_str()];
		if (s == "DiffuseMap")
		{
			if (v.IsString()) diffusePath = v.GetString();
			continue;
		} 
		if (s == "MRMap")
		{
			if (v.IsString()) mrPath = v.GetString();
			continue;
		}
		if (s == "NormalMap")
		{
			if (v.IsString()) normPath = v.GetString();
			continue;
		}
		if (s == "BaseColor")
		{
			if (v.IsArray() && v.Size() == 3)
			{
				baseColor.x = v[0].GetFloat();
				baseColor.y = v[1].GetFloat();
				baseColor.z = v[2].GetFloat();
			}
			continue;
		}
		if (s == "Metallicity")
		{
			if (v.IsFloat()) metal = v.GetFloat();
			continue;
		}
		if (s == "Roughness")
		{
			if (v.IsFloat()) rough = v.GetFloat();
			continue;
		}
		if (s == "Translucent")
		{
			if (v.IsBool() && v.GetBool()) texFlags |= Texture_Translucent;
			continue;
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
	if (normPath == "")
	{
		NormalMap = nullptr;
	}
	else
	{
		NormalMap = CTexture::TextureExists(normPath);
		if (!NormalMap) NormalMap = new CTexture(normPath);
	}
	BaseColor = baseColor;
	Roughness = rough;
	Metallicity = metal;

	Logging::LogMessage(LogLevel_Debug, "Loaded material file %s", path.c_str());
}
