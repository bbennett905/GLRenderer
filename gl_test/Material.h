#pragma once

#include <glm.hpp>

#include "Shader.h"
#include "Texture.h"

//TODO we could load these from a file (like vmt)
//and this could also bind textures + other uniforms for the shader,
//making this more suited to a class?	probably not 
//or we could pass a Material to a renderer method, setting as the one to use <---
//also, more textures for normal maps, reflection maps, etc would eventually be necessary
struct Material
{
	//Shader used for this material
	Shader * ShaderObj;

	//Primary texture
	Texture * TextureObj1;
	//Secondary (blended) texture, optional
	Texture * TextureObj2;
	//Mix Ratio between the textures
	float MixRatio;
	//Texture for specular highlights
	Texture * SpecularMap;

	//Reflection properties
	float AmbientStrength;
	float DiffuseStrength;
	float SpecularStrength;
	float Shininess;

	Material(Shader * shad, Texture * tex1 = nullptr, Texture * tex2 = nullptr, float mix = 0.5f, 
		Texture * specMap = nullptr,
		float ambStr = 0.5f, float difStr = 1.0f, float specStr = 1.0f, float shiny = 32.0f) : 
		ShaderObj(shad), TextureObj1(tex1), TextureObj2(tex2), MixRatio(mix),
		SpecularMap(specMap),
		AmbientStrength(ambStr), DiffuseStrength(difStr), SpecularStrength(specStr),
		Shininess(shiny) { }
	Material(Shader * shad, Texture * tex1 = nullptr,
		float ambStr = 0.5f, float difStr = 1.0f, float specStr = 1.0f, float shiny = 32.0f) :
		ShaderObj(shad), TextureObj1(tex1),
		AmbientStrength(ambStr), DiffuseStrength(difStr), SpecularStrength(specStr),
		Shininess(shiny) { }
	Material() { }
	Material(const Material&) = default;
	Material& operator=(const Material&) = default;
};