#pragma once

#include <glew.h>

#include <string>

class Texture
{
public:
	Texture(const char * path);
	void Bind();

	GLuint ID;
	//"texture_diffuse" or "texture_specular"
	std::string Type;
	std::string Path;
};
