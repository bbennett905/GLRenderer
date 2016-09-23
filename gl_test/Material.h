#pragma once

#include <glm.hpp>
#include <vector>

#include "Shader.h"
#include "Texture.h"

//TODO we could load these from a file (like vmt)
//and this could also bind textures + other uniforms for the shader,

struct Material
{
	//shader needs to be for each MESH not material
	//Shader used for this material
	//Shader * ShaderObj;

	//TODO what if THESE were vectors of each map?
	//Primary texture
	Texture * DiffuseMap;
	//Texture for specular highlights
	Texture * SpecularMap;

	//Reflection properties
	float AmbientStrength;
	float DiffuseStrength;
	float SpecularStrength;
	float Shininess;

	//Material(Shader * shad) :
	//	ShaderObj(shad) { }
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