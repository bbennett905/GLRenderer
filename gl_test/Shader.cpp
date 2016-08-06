#include "Shader.h"

Shader::Shader(const GLchar * vertexPath, const GLchar * fragPath)
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

	const GLchar * vShaderCode = vertexSource.c_str();
	const GLchar * fShaderCode = fragSource.c_str();

	GLuint vertex, frag;
	GLint success;
	GLchar infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fShaderCode, NULL);
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

void Shader::Use() { glUseProgram(Program); }
