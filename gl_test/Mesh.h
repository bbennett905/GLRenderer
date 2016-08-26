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

//TODO separate these two!
class Mesh :
	public BaseDrawable
{
public:
	Mesh(std::vector<VertexData> vert, std::vector<GLuint> ind, std::vector<TextureData> texts);
	glm::mat4 GetModelMatrix();
	std::vector<TextureData> TextData;
};

class Model :
	public BaseObject
{
public:
	std::vector<Mesh> Meshes;
	Model(const char * path);
private:
	void loadModel(std::string path);
	void processNode(aiNode * node, const aiScene * scene);
	Mesh processMesh(aiMesh * mesh, const aiScene * scene);
	std::string _directory;
};