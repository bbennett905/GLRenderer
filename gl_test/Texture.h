#pragma once

#include <glew.h>

#include <string>

class Texture
{
public:
	Texture(const char * path);
	void Bind();

	GLuint ID;
	std::string Type;
	std::string Path;
};

