#include "Texture.h"
#include <SOIL.h>

Texture::Texture(const char * path)
{
	int texWidth, texHeight;
	unsigned char * image = SOIL_load_image(path, &texWidth, &texHeight, 0, SOIL_LOAD_RGB);

	if (image == nullptr) printf("ERROR: Failed loading image %s!\n", path);

	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 
		0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	Path = path;
}

Texture::~Texture()
{
	glDeleteTextures(1, &_id);
}

void Texture::Bind() { glBindTexture(GL_TEXTURE_2D, _id); }
