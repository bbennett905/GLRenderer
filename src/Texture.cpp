#include "Texture.h"

#include <SOIL.h>
#include <SDL.h>

#include "Logging.h"

std::vector<Texture *> Texture::_textures_loaded;

Texture::Texture(std::string path, uint32_t flags) :
	_path(path), _flags(flags)
{
	if (flags & Texture_Cubemap)
		loadCubemapFromPath(path);
	else
		loadTextureFromPath(path);
}

Texture::Texture(SDL_Surface * surface, uint32_t flags) :
	_flags(flags)
{
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0,
		flags & Texture_Translucent ? GL_RGBA : GL_RGB,
		surface->w, surface->h, 0,
		flags & Texture_Translucent ? GL_RGBA : GL_RGB,
		GL_UNSIGNED_BYTE, surface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	_textures_loaded.push_back(this);
	Logging::LogMessage(LogLevel_Debug, "Loaded texture from SDL surface");
}

Texture::Texture()
{
	glGenTextures(1, &_id);
}

Texture::~Texture()
{
	for (uint32_t i = 0; i < _textures_loaded.size(); i++)
	{
		if (this == _textures_loaded[i])
		{
			_textures_loaded.erase(_textures_loaded.begin() + i);
			break;
		}
	}
	glDeleteTextures(1, &_id);
}

Texture * Texture::TextureExists(std::string path)
{
	for (uint32_t i = 0; i < _textures_loaded.size(); i++)
	{
		if (_textures_loaded[i]->_path == path)
		{
			Logging::LogMessage(LogLevel_Debug, 
				"Texture \"%s\" has already been loaded", path.c_str());
			return _textures_loaded[i];
		}
	}
	return nullptr;
}

void Texture::Bind() 
{ 
	if (_flags & Texture_Cubemap)
		glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
	else
		glBindTexture(GL_TEXTURE_2D, _id); 
}

void Texture::Update(SDL_Surface * surface)
{
	glBindTexture(GL_TEXTURE_2D, _id);

	glTexImage2D(GL_TEXTURE_2D, 0,
		_flags & Texture_Translucent ? GL_RGBA : GL_RGB,
		surface->w, surface->h, 0,
		_flags & Texture_Translucent ? GL_RGBA : GL_RGB,
		GL_UNSIGNED_BYTE, surface->pixels);
	//glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::loadTextureFromPath(std::string path)
{
	int texWidth, texHeight;
	unsigned char * image = SOIL_load_image(path.c_str(), &texWidth, &texHeight, 0,
		_flags & Texture_Translucent ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);

	if (image == nullptr)
		Logging::LogMessage(LogLevel_Error, "Failed loading texture \"%s\"\n", path.c_str());

	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0,
		_flags & Texture_Translucent ? GL_RGBA : GL_RGB,
		texWidth, texHeight, 0,
		_flags & Texture_Translucent ? GL_RGBA : GL_RGB,
		GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	_textures_loaded.push_back(this);
	Logging::LogMessage(LogLevel_Debug, "Loaded texture \"%s\"", _path.c_str());
}

void Texture::loadCubemapFromPath(std::string path)
{
	glGenTextures(1, &_id);

	int tex_width, tex_height;
	unsigned char * image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, _id);

	for (uint32_t i = 0; i < 6; i++)
	{
		std::string name = path + std::to_string(i) + ".jpg";
		image = SOIL_load_image(name.c_str(), &tex_width, &tex_height, 0, SOIL_LOAD_RGB);
		if (image == nullptr)
			Logging::LogMessage(LogLevel_Error, "Failed loading texture \"%s\"\n", path.c_str());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
			tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	_textures_loaded.push_back(this);
	Logging::LogMessage(LogLevel_Debug, "Loaded cubemap texture \"%s\"", _path.c_str());
}
