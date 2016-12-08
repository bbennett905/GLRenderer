#pragma once

#include <string>
#include <vector>

/*
 * Abstraction over SOIL texture loading and OpenGL texture creation and binding
 */

typedef unsigned int GLuint;
struct SDL_Surface;

 //Any flags indicating different behaviour needed from the shader, ie translucency
enum TextureFlags
{
	Texture_Translucent = 1 << 0,
	Texture_Cubemap = 1 << 1
};

class CTexture
{
public:
	//Path to the texture
	CTexture(std::string path, uint32_t flags = 0);
	CTexture(SDL_Surface * surface, uint32_t flags = 0);
	CTexture();
	~CTexture();

	static CTexture * TextureExists(std::string path);

	//Bind this texture object to the active GL texture
	void Bind();

	void Update(SDL_Surface * surface);

private:
	void loadTextureFromPath(std::string path);
	void loadCubemapFromPath(std::string path);

	static std::vector<CTexture *> _textures_loaded;
	//OpenGL texture object
	GLuint _id;
	//Path of this texture - used to avoid loading textures that have already been loaded
	std::string _path;

	uint32_t _flags;
};
