#pragma once

#include <glew.h>
#include <string>

/*
 * Abstraction over SOIL texture loading and OpenGL texture creation and binding
 */

 //Any flags indicating different behaviour needed from the shader, ie translucency
enum TextureFlags
{
	Texture_Translucent = 1 << 0
};

class Texture
{
public:
	//Path to the texture
	Texture(const char * path, uint32_t flags = 0);
	~Texture();

	//Bind this texture object to the active GL texture
	void Bind();

	//Path of this texture - used to avoid loading textures that have already been loaded
	std::string Path;

private:
	//OpenGL texture object
	GLuint _id;
};
