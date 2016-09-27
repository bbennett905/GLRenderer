#pragma once

#include <string>
#include <vector>
#include <scene.h>

#include "BaseObject.h"
#include "Mesh.h"


class Model :
	public BaseObject
{
public:
	Model(const char * path, ShaderCreateInfo shaderInfo);
	std::vector<Mesh> * GetMeshes();

	void SetPosition(glm::vec3 pos);
	glm::vec3 GetPosition();
	void SetAngles(glm::vec3 ang);
private:
	void loadModel(std::string path);
	void processNode(aiNode * node, const aiScene * scene);
	Mesh processMesh(aiMesh * mesh, const aiScene * scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName);
	std::vector<Material> loadMaterials(aiMaterial * mat);

	std::vector<Mesh> _meshes;
	std::string _directory;
	std::vector<Texture> _textures_loaded;
	Shader * _shader;
	ShaderCreateInfo _shader_create_info;

	glm::vec3 Position;
};