#pragma once
#include "BaseDrawable.h"

#include <string>
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

//TODO separate these two!
class Mesh :
	public BaseDrawable, public BaseObject
{
public:
	Mesh(std::vector<VertexData> vert, std::vector<GLuint> ind, std::vector<Texture> texts);
	glm::mat4 GetModelMatrix();
	std::vector<Texture> Textures;
};

class Model :
	public BaseObject
{
public:
	Model(const char * path);
	std::vector<Mesh> * GetMeshes();

	void SetPosition(glm::vec3 pos);
	glm::vec3 GetPosition();
	void SetAngles(glm::vec3 ang);
private:
	void loadModel(std::string path);
	void processNode(aiNode * node, const aiScene * scene);
	Mesh processMesh(aiMesh * mesh, const aiScene * scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typename);
	
	std::vector<Mesh> _meshes;
	std::string _directory;
	std::vector<Texture> _textures_loaded;

	glm::vec3 Position;
};