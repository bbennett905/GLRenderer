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

	//Sets world position
	void SetPosition(glm::vec3 pos);
	//Returns world position
	glm::vec3 GetPosition();
	//Sets angles
	void SetAngles(glm::vec3 ang);

	std::vector<Mesh *> Meshes;

private:
	void loadModel(std::string path);
	void processNode(aiNode * node, const aiScene * scene);
	Mesh * processMesh(aiMesh * mesh, const aiScene * scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName);
	std::vector<Material> loadMaterials(aiMaterial * mat);

	std::string _directory;
	std::vector<Texture> _textures_loaded;
	Shader * _shader;
	//ShaderCreateInfo _shader_create_info;

	glm::vec3 Position;
};