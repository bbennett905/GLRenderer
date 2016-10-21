#pragma once

#include <string>
#include <glew.h>
#include <vector>

/*
 * Shader object class which can dynamically create shaders for the specific scene 
 * being rendered - params for shader creation in ShaderCreateInfo struct
 */

//I really have no idea if this is a reasonable number
#define MAX_MATERIALS 8

//We can add more versions here later on, but for now this is good enough
enum GLShaderVersion 
{
	ShaderVersion330Core
};

//Information to pass in the Shader constructor giving requirements for the shader
struct ShaderCreateInfo
{
	//Shader API version to use - currently limited to 330 core
	GLShaderVersion Version;
	//Number of point lights this shader will take
	uint32_t NumPointLights;
	//Number of spot lights this shader will take
	uint32_t NumSpotLights;
	//Flags to use for this shader
	uint32_t Flags;

	inline bool operator==(const ShaderCreateInfo& a) const
	{
		if (Version == a.Version &&
			NumPointLights == a.NumPointLights &&
			NumSpotLights == a.NumSpotLights &&
			Flags == a.Flags)
			return true;
		else return false;
	}
};

//Any flags indicating different behaviour needed from the shader, ie translucency
enum ShaderFlags
{
	Shader_Translucent = 1 << 0
};

class Shader
{
public:
	//Create a typical shader dynamically, given certain parameters
	Shader(ShaderCreateInfo info);
	~Shader();

	//Returns nullptr if no suitable shader exists, else returns pointer to shader
	static Shader * ShaderExists(ShaderCreateInfo info);

	//Tells OpenGL to use this shader
	void Use();

	//TODO this should probably be read-only (or friend renderer)
	//OpenGL shader program object of this Shader
	GLuint Program;

	//TODO could make IncrementTextureCount and GetTextureCount (or use friend Renderer and make private)
	//USED BY RENDERER, DO NOT MODIFY
	int TextureCount;

	//Holds the info used to create this shader
	ShaderCreateInfo CreateInfo;

private:
	void preprocessShader(std::string & vertexSource, std::string & fragSource, ShaderCreateInfo info);
	void createShaders(const char * vertexSource, const char * fragSource);

	static std::vector<Shader *> _shaders_loaded;
};
