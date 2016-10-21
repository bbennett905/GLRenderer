#include "Shader.h"

#include "Utils.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "Logging.h"

std::vector<Shader *> Shader::_shaders_loaded;

Shader::Shader(ShaderCreateInfo info) :
	TextureCount(0), CreateInfo(info)
{
	for (uint32_t i = 0; i < _shaders_loaded.size(); i++)
	{
		if (_shaders_loaded[i]->CreateInfo == info)
		{
			Program = _shaders_loaded[i]->Program;
			return;
		}
	}
	//TODO do something with ShaderFlags 
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
		Logging::LogMessage(LogLevel_Error, "Error reading shader file %s", vertexPath);
	}

	preprocessShader(vertexSource, fragSource, info);
	createShaders(vertexSource.c_str(), fragSource.c_str());

	_shaders_loaded.push_back(this);
}

Shader::~Shader()
{
	for (uint32_t i = 0; i < _shaders_loaded.size(); i++)
	{
		if (this == _shaders_loaded[i])
		{
			_shaders_loaded.erase(_shaders_loaded.begin() + i);
			break;
		}
	}
	glDeleteProgram(Program);
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

	//rather crude way to pass a #define through, but it works for now
	FindAndReplaceAll(vertexSource, "MAX_MATERIALS", std::to_string(MAX_MATERIALS));
	FindAndReplaceAll(fragSource, "MAX_MATERIALS", std::to_string(MAX_MATERIALS));
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
		Logging::LogMessage(LogLevel_Error, "Error compiling vertex shader:\n%s", infoLog);
	};

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragSource, NULL);
	glCompileShader(frag);
	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(frag, 512, NULL, infoLog);
		Logging::LogMessage(LogLevel_Error, "Error compiling fragment shader:\n%s", infoLog);
	};

	Program = glCreateProgram();
	glAttachShader(Program, vertex);
	glAttachShader(Program, frag);
	glLinkProgram(Program);
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		Logging::LogMessage(LogLevel_Error, "Error linking shaders:\n%s", infoLog);
	}

	glDeleteShader(vertex);
	glDeleteShader(frag);
}