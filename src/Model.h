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
struct Material;
class Shader;
class Mesh;

class Model :
	public BaseObject
{
public:
	Model(const char * path);
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

	std::string _directory;
	std::vector<Texture *> _textures_loaded;
	Shader * _shader;

	glm::vec3 Position;
	glm::vec3 Scale;
};
