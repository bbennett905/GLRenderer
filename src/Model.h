#pragma once

#include <string>
#include <vector>
#include <scene.h>

#include "BaseObject.h"

/*
 * Class representing a 3D model, imported using Assimp
 * Composed of several meshes
 */

class CTexture;
class CMaterial;
class CShader;
class CMesh;

class CModel :
	public CBaseObject
{
public:
	CModel(std::string path);
	~CModel();

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

	std::vector<CMesh *> Meshes;

private:
	void loadModel(std::string path);
	void processNode(aiNode * node, const aiScene * scene);
	CMesh * processMesh(aiMesh * mesh, const aiScene * scene);
	std::vector<CMaterial *> loadMaterials(aiMaterial * mat);

	static std::vector<CModel *> _models_loaded;

	std::string _path;
	std::string _directory;
	CShader * _shader;

	glm::vec3 Position;
	glm::vec3 Scale;
};
