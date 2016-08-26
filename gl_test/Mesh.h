#pragma once
#include "BaseDrawable.h"

#include <string>

//TODO move this to basedrawable, and update renderer to use it
struct VertexData
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct TextureData
{
	GLuint id;
	std::string type;
};

class Mesh :
	public BaseDrawable
{
public:
	Mesh(std::vector<VertexData> vert, std::vector<GLuint> ind, std::vector<TextureData> texts);
	std::vector<TextureData> TextData;
	std::vector<VertexData> VertData;
};

class Model :
	public BaseObject
{
public:
	//TODO needs pos, ang, etc.
	std::vector<Mesh> Meshes;
};