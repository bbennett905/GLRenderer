#pragma once

#include <glew.h>

class Texture
{
public:
	Texture(const char * path);
	void Bind();

	GLuint ID;
};

