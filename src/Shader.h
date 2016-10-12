#pragma once

#include <string>

#include <glew.h>

/*
 * Shader object class which can dynamically create shaders for the specific scene 
 * being rendered - params for shader creation in ShaderCreateInfo struct
 */

//I really have no idea if this is a reasonable number
#define MAX_MATERIALS 8

//We can add more versions here later on, but for now leave it as this
enum GLShaderVersion {
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
};

class Shader
{
public:
	//Create a shader from file
	Shader(const char * vertexPath, const char * fragPath, ShaderCreateInfo & info);

	//Create a typical shader dynamically, given certain parameters
	Shader(ShaderCreateInfo & info);

	//Tells OpenGL to use this shader
	void Use();

	//OpenGL shader program object of this Shader
	GLuint Program;

	//USED BY RENDERER, DO NOT MODIFY
	int TextureCount;

private:
	void preprocessShader(std::string & vertexSource, std::string & fragSource, ShaderCreateInfo info);
	void createShaders(const char * vertexSource, const char * fragSource);
};
