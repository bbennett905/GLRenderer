#include "Shader.h"

#include "Utils.h"

#include <fstream>
#include <sstream>
#include <iostream>

//TODO Rather than passing a path to constructor, pass name of shader - eg "default"
//Then, look in ../shaders/ for default.vert & default.frag, or if no materials look
//for one with the _nomat suffix (_unlit for no lights)

//TODO this will break if given 0 point lights or 0 spot lights
//possible workaround could be pass 1 light always, with 0 brightness

Shader::Shader(const char * vertexPath, const char * fragPath, ShaderCreateInfo & info) :
	TextureCount(0)
{
	std::string vertexSource;
	std::string fragSource;
	std::ifstream vertexFile;
	std::ifstream fragFile;

	vertexFile.exceptions(std::ifstream::badbit);
	fragFile.exceptions(std::ifstream::badbit);
	try
	{
		vertexFile.open(vertexPath);
		fragFile.open(fragPath);
		std::stringstream vertexStream, fragStream;

		vertexStream << vertexFile.rdbuf();
		fragStream << fragFile.rdbuf();

		vertexFile.close();
		fragFile.close();

		vertexSource = vertexStream.str();
		fragSource = fragStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	preprocessShader(vertexSource, fragSource, info);
	createShaders(vertexSource.c_str(), fragSource.c_str());
}

Shader::Shader(ShaderCreateInfo & info) :
	TextureCount(0)
{
	char * vertexPath = "../shaders/default.vert";
	char * fragPath = "../shaders/default.frag";

	std::string vertexSource;
	std::string fragSource;
	std::ifstream vertexFile;
	std::ifstream fragFile;

	vertexFile.exceptions(std::ifstream::badbit);
	fragFile.exceptions(std::ifstream::badbit);
	try
	{
		vertexFile.open(vertexPath);
		fragFile.open(fragPath);
		std::stringstream vertexStream, fragStream;

		vertexStream << vertexFile.rdbuf();
		fragStream << fragFile.rdbuf();

		vertexFile.close();
		fragFile.close();

		vertexSource = vertexStream.str();
		fragSource = fragStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	preprocessShader(vertexSource, fragSource, info);
	createShaders(vertexSource.c_str(), fragSource.c_str());
}

void Shader::Use()
{
	glUseProgram(Program);
	TextureCount = 0;
}

void Shader::preprocessShader(std::string & vertexSource, std::string & fragSource,
	ShaderCreateInfo info)
{
	switch (info.Version)
	{
	case ShaderVersion330Core:
		FindAndReplaceAll(vertexSource, "#insert version", "#version 330 core");
		FindAndReplaceAll(fragSource, "#insert version", "#version 330 core");
		break;
	default:
		FindAndReplaceAll(vertexSource, "#insert version", "#version 330 core");
		FindAndReplaceAll(fragSource, "#insert version", "#version 330 core");
		break;
	}

	FindAndReplaceAll(vertexSource, "#insert num_point_lights", std::to_string(info.NumPointLights));
	FindAndReplaceAll(fragSource, "#insert num_point_lights", std::to_string(info.NumPointLights));
	FindAndReplaceAll(vertexSource, "#insert num_spot_lights", std::to_string(info.NumSpotLights));
	FindAndReplaceAll(fragSource, "#insert num_spot_lights", std::to_string(info.NumSpotLights));
	FindAndReplaceAll(vertexSource, "#insert num_materials", std::to_string(info.NumMaterials));
	FindAndReplaceAll(fragSource, "#insert num_materials", std::to_string(info.NumMaterials));
}

void Shader::createShaders(const char * vertexSource, const char * fragSource)
{
	GLuint vertex, frag;
	GLint success;
	GLchar infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexSource, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragSource, NULL);
	glCompileShader(frag);
	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(frag, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	Program = glCreateProgram();
	glAttachShader(Program, vertex);
	glAttachShader(Program, frag);
	glLinkProgram(Program);
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(frag);
}