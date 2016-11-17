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
	char * vertexPath;
	char * fragPath;
	vertexPath = "../shaders/default.vert";
	fragPath = "../shaders/cooktorrance.frag";

	if (info.Flags & Shader_Unlit)
	{
		vertexPath = "../shaders/default_unlit.vert";
		fragPath = "../shaders/default_unlit.frag";
	}
	if (info.Flags & Shader_Skybox)
	{
		vertexPath = "../shaders/skybox.vert";
		fragPath = "../shaders/skybox.frag";
	}

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
		Logging::LogMessage(LogLevel_Error, "Failed reading shader file \"%s\"", vertexPath);
	}

	preprocessShader(vertexSource, fragSource, info);
	createShaders(vertexSource.c_str(), fragSource.c_str());

	_shaders_loaded.push_back(this);
	Logging::LogMessage(LogLevel_Debug, "Created shader (%d)", _program);
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
	glDeleteProgram(_program);
}

Shader * Shader::ShaderExists(ShaderCreateInfo info)
{
	for (uint32_t i = 0; i < _shaders_loaded.size(); i++)
	{
		if (_shaders_loaded[i]->CreateInfo == info)
		{
			Logging::LogMessage(LogLevel_Debug,
				"A suitable shader (%d) has already been created", _shaders_loaded[i]->_program);
			return _shaders_loaded[i];
		}
	}
	return nullptr;
}

void Shader::Use()
{
	glUseProgram(_program);
	TextureCount = 0;
}

void Shader::UseNull()
{
	glUseProgram(0);
}

GLuint Shader::GetProgram()
{
	return _program;
}

//This little optimization has a much greater effect on nvidia rather than intel - but regardless, 
//it slows it down drastically on Debug builds so it's disabled - important to remember for profiling
GLuint Shader::GetUniformLocation(std::string name)
{
#ifdef _DEBUG
	return glGetUniformLocation(_program, name.c_str());
#else
	//If it doesn't exist in the map yet, load it in
	if (_uniform_locations.find(name) == _uniform_locations.end())
	{
		_uniform_locations[name] = glGetUniformLocation(_program, name.c_str());
	}
	return _uniform_locations[name];
#endif //_DEBUG
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
		Logging::LogMessage(LogLevel_Error, "Failed compiling vertex shader:\n%s", infoLog);
	};

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragSource, NULL);
	glCompileShader(frag);
	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(frag, 512, NULL, infoLog);
		Logging::LogMessage(LogLevel_Error, "Failed compiling fragment shader:\n%s", infoLog);
	};

	_program = glCreateProgram();
	glAttachShader(_program, vertex);
	glAttachShader(_program, frag);
	glLinkProgram(_program);
	glGetProgramiv(_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(_program, 512, NULL, infoLog);
		Logging::LogMessage(LogLevel_Error, "Failed linking shaders:\n%s", infoLog);
	}

	glDeleteShader(vertex);
	glDeleteShader(frag);
}