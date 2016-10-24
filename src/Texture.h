#pragma once

#include <glew.h>
#include <string>
#include <vector>

/*
 * Abstraction over SOIL texture loading and OpenGL texture creation and binding
 */

struct SDL_Surface;

 //Any flags indicating different behaviour needed from the shader, ie translucency
enum TextureFlags
{
	Texture_Translucent = 1 << 0
};

class Texture
{
public:
	//Path to the texture
	Texture(std::string path, uint32_t flags = 0);
	Texture(SDL_Surface * surface, uint32_t flags);
	~Texture();

	static Texture * TextureExists(std::string path);

	//Bind this texture object to the active GL texture
	void Bind();

	void Update(SDL_Surface * surface);

private:
	static std::vector<Texture *> _textures_loaded;
	//OpenGL texture object
	GLuint _id;
	//Path of this texture - used to avoid loading textures that have already been loaded
	std::string _path;

	uint32_t _flags;
};
