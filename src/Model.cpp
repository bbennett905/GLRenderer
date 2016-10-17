#include "Model.h"

#include <Importer.hpp>
#include <postprocess.h>

#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"

Model::Model(const char * path)
{
	loadModel(path);
}

Model::~Model()
{
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
		glm::vec3 vec;
		vec.x = mesh->mVertices[i].x;
		vec.y = mesh->mVertices[i].y;
		vec.z = mesh->mVertices[i].z;
		vertex.Position = vec;

		vec.x = mesh->mNormals[i].x;
		vec.y = mesh->mNormals[i].y;
		vec.z = mesh->mNormals[i].z;
		vertex.Normal = vec;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vector;
			vector.x = mesh->mTextureCoords[0][i].x;
			vector.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vector;
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
		bool skip = false;
		for (uint32_t j = 0; j < _textures_loaded.size(); j++)
		{
			if (aiString(_textures_loaded[j]->Path) == str)
			{
				diffuse = _textures_loaded[j];
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			std::string path = _directory + "/" + std::string(str.C_Str());
			diffuse = new Texture(path.c_str());
			_textures_loaded.push_back(diffuse);
		}

		if (i < mat->GetTextureCount(aiTextureType_SPECULAR))
		{
			//Check if the specular map has already been loaded
			mat->GetTexture(aiTextureType_SPECULAR, i, &str);
			skip = false;
			for (uint32_t j = 0; j < _textures_loaded.size(); j++)
			{
				if (aiString(_textures_loaded[j]->Path) == str)
				{
					specular = _textures_loaded[j];
					skip = true;
					break;
				}
			}
			if (!skip)
			{
				std::string path = _directory + "/" + std::string(str.C_Str());
				specular = new Texture(path.c_str());
				_textures_loaded.push_back(specular);
			}
		}
		//Create a material
		material = new Material(diffuse, specular);
		mats.push_back(material);
	}

	return mats;
}
