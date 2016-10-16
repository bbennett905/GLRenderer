#pragma once

#include <string>

#include <glew.h>

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
};

//Any flags indicating different behaviour needed from the shader, ie translucency
enum ShaderFlags
{
	Shader_Translucent = 1 << 0
};

class Shader
{
public:
	//Create a shader from file
	Shader(const char * vertexPath, const char * fragPath, ShaderCreateInfo info);
	//Create a typical shader dynamically, given certain parameters
	Shader(ShaderCreateInfo info);
	~Shader();

	//Tells OpenGL to use this shader
	void Use();

	//TODO this should probably be read-only 
	//OpenGL shader program object of this Shader
	GLuint Program;

	//TODO could make IncrementTextureCount and GetTextureCount 
	//USED BY RENDERER, DO NOT MODIFY
	int TextureCount;

	//Holds the info used to create this shader
	ShaderCreateInfo CreateInfo;

private:
	void preprocessShader(std::string & vertexSource, std::string & fragSource, ShaderCreateInfo info);
	void createShaders(const char * vertexSource, const char * fragSource);
};
