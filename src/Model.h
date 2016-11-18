#pragma once

#include <string>
#include <vector>
#include <scene.h>

#include "BaseObject.h"

/*
 * Class representing a 3D model, imported using Assimp
 * Composed of several meshes
 */

class Texture;
class Material;
class Shader;
class Mesh;

class Model :
	public BaseObject
{
public:
	Model(std::string path);
	~Model();

	//Sets world position
	void SetPosition(glm::vec3 pos);
	//Returns world position
	glm::vec3 GetPosition();
	//Sets angles
	void SetAngles(glm::vec3 ang);
	//Gets object scale vector
	glm::vec3 GetScale();
	//Sets object scale vector
	void SetScale(glm::vec3 scale);

	std::vector<Mesh *> Meshes;

private:
	void loadModel(std::string path);
	void processNode(aiNode * node, const aiScene * scene);
	Mesh * processMesh(aiMesh * mesh, const aiScene * scene);
	std::vector<Material *> loadMaterials(aiMaterial * mat);

	static std::vector<Model *> _models_loaded;

	std::string _path;
	std::string _directory;
	Shader * _shader;

	glm::vec3 Position;
	glm::vec3 Scale;
};
