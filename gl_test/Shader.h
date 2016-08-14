#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glew.h>

class Shader
{
public:
	Shader(const GLchar * vertexPath, const GLchar * fragPath);
	void Use();

	GLuint Program;
};

