#pragma once
#include "BaseDrawable.h"

#include <string>
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

struct TextureData
{
	GLuint id;
	std::string type;
	aiString path;
};

class Mesh :
	public BaseDrawable
{
public:
	Mesh(std::vector<VertexData> vert, std::vector<GLuint> ind, std::vector<TextureData> texts);
	std::vector<TextureData> TextData;
};

class Model :
	public BaseObject
{
public:
	std::vector<Mesh> Meshes;
};