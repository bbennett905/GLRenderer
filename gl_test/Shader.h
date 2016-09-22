#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glew.h>

//We can add more versions here later on, but for now leave it as this
enum GLShaderVersion {
	ShaderVersion330Core
};

class Shader
{
public:
	//Create a shader from file
	Shader(const char * vertexPath, const char * fragPath);
	//version, (numDiffuse, numSpec)->(numMaterials?), numLights, numSpotLights,
	//doing numMaterials, set 1 diff and 1 spec per material, and vector of mix values between each mat?
	//TODO edit materials to 1 diff 1 spec
	//Create a typical shader dynamically, given certain parameters
	Shader(GLShaderVersion version,
		uint32_t numPointLights, uint32_t numSpotLights,
		uint32_t numMaterials);
	void Use();

	GLuint Program;

private:
	void preprocessShader(std::string vertexSource, std::string fragSource, GLShaderVersion version, int numPointLights, int numSpotLights, int numMaterials);
	void createShaders(const char * vertexSource, const char * fragSource);
};
