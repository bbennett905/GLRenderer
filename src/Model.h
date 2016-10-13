#pragma once

#include <string>
#include <vector>
#include <scene.h>

#include "BaseObject.h"
#include "Mesh.h"

/*
 * Class representing a 3D model, imported using Assimp
 * Composed of several meshes
 */

class Model :
	public BaseObject
{
public:
	Model(const char * path);

	//Returns the list of Meshes this model is composed of (why do i do this?)
	std::vector<Mesh> * GetMeshes();

	//Sets world position
	void SetPosition(glm::vec3 pos);
	//Returns world position
	glm::vec3 GetPosition();
	//Sets angles
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
	//ShaderCreateInfo _shader_create_info;

	glm::vec3 Position;
};