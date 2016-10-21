#include "Texture.h"

#include <SOIL.h>

#include "Logging.h"

std::vector<Texture *> Texture::_textures_loaded;

Texture::Texture(std::string path, uint32_t flags) :
	_path(path)
{
	for (uint32_t i = 0; i < _textures_loaded.size(); i++)
	{
		if (_textures_loaded[i]->_path == path)
		{
			_id = _textures_loaded[i]->_id;
			return;
		}
	}

	int texWidth, texHeight;
	unsigned char * image = SOIL_load_image(path.c_str(), &texWidth, &texHeight, 0, 
		flags & Texture_Translucent ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);

	if (image == nullptr) 
		Logging::LogMessage(LogLevel_Error, "Failed loading image %s!\n", path.c_str());

	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, 
		flags & Texture_Translucent ? GL_RGBA :GL_RGB,
		texWidth, texHeight, 0, 
		flags & Texture_Translucent ? GL_RGBA : GL_RGB, 
		GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	_textures_loaded.push_back(this);
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

void Texture::Bind() { glBindTexture(GL_TEXTURE_2D, _id); }
