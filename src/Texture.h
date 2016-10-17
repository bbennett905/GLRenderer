#pragma once

#include <glew.h>
#include <string>

/*
 * Abstraction over SOIL texture loading and OpenGL texture creation and binding
 */

class Texture
{
public:
	//Path to the texture
	Texture(const char * path);
	~Texture();

	//Bind this texture object to the active GL texture
	void Bind();

	//Path of this texture - used to avoid loading textures that have already been loaded
	std::string Path;

private:
	//OpenGL texture object
	GLuint _id;
};
