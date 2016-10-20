#include "Model.h"

#include <Importer.hpp>
#include <postprocess.h>

#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"

std::vector<Model *> Model::_models_loaded;

Model::Model(std::string path) : 
	_path(path)
{
	for (uint32_t i = 0; i < _models_loaded.size(); i++)
	{
		if (path == _models_loaded[i]->_path)
		{
			for (auto mesh : _models_loaded[i]->Meshes)
			{
				Mesh * new_mesh = new Mesh();
				*new_mesh = *mesh;
				Meshes.push_back(new_mesh);
			}
			_shader = _models_loaded[i]->_shader;
			return;
		}
	}
	loadModel(path);
	_models_loaded.push_back(this);
}

Model::~Model()
{
	for (uint32_t i = 0; i < _models_loaded.size(); i++)
	{
		if (this == _models_loaded[i])
		{
			_models_loaded.erase(_models_loaded.begin() + i);
			break;
		}
	}
	for (auto mesh : Meshes)
		delete mesh;
}

void Model::SetPosition(glm::vec3 pos)
{
	Position = pos;
	for (uint32_t i = 0; i < Meshes.size(); i++)
		Meshes[i]->Position = pos;
}

glm::vec3 Model::GetPosition()
{
	return Position;
}

void Model::SetAngles(glm::vec3 ang)
{
	BaseObject::SetAngles(ang);
	for (uint32_t i = 0; i < Meshes.size(); i++)
		Meshes[i]->SetAngles(ang);
}

glm::vec3 Model::GetScale()
{
	return Scale;
}

void Model::SetScale(glm::vec3 scale)
{
	Scale = scale;
	for (uint32_t i = 0; i < Meshes.size(); i++)
		Meshes[i]->Scale = scale;
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene * scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		printf("ERROR::ASSIMP::%s\n", importer.GetErrorString());
		return;
	}
	_directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode * node, const aiScene * scene)
{
	for (uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
		Meshes.push_back(processMesh(mesh, scene));
	}
	for (uint32_t i = 0; i < node->mNumChildren; i++)
		processNode(node->mChildren[i], scene);
}

Mesh * Model::processMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<VertexData> vertices;
	std::vector<GLuint> indices;
	std::vector<Material *> materials;

	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		VertexData vertex;
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}

	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (uint32_t j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];
		materials = loadMaterials(material);
	}

	return new Mesh(vertices, indices, materials);
}

std::vector<Material *> Model::loadMaterials(aiMaterial * mat)
{
	std::vector<Material *> mats;

	for (uint32_t i = 0; i < mat->GetTextureCount(aiTextureType_DIFFUSE); i++)
	{
		Texture * diffuse = nullptr;
		Texture * specular = nullptr;
		Material * material;

		//Check if the texture has already been loaded for this model
		aiString str;
		mat->GetTexture(aiTextureType_DIFFUSE, i, &str);

		std::string path = _directory + "/" + std::string(str.C_Str());
		diffuse = new Texture(path.c_str());

		if (i < mat->GetTextureCount(aiTextureType_SPECULAR))
		{
			//Check if the specular map has already been loaded
			mat->GetTexture(aiTextureType_SPECULAR, i, &str);

			std::string path = _directory + "/" + std::string(str.C_Str());
			specular = new Texture(path.c_str());
		}
		//Create a material
		material = new Material(diffuse, specular);
		mats.push_back(material);
	}

	return mats;
}
