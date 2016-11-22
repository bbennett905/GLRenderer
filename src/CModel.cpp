#include "CModel.h"

#include <Importer.hpp>
#include <postprocess.h>

#include "CMesh.h"
#include "CShader.h"
#include "CMaterial.h"
#include "CTexture.h"
#include "Logging.h"

std::vector<CModel *> CModel::_models_loaded;

CModel::CModel(std::string path) : 
	_path(path)
{
	for (uint32_t i = 0; i < _models_loaded.size(); i++)
	{
		if (path == _models_loaded[i]->_path)
		{
			for (auto mesh : _models_loaded[i]->Meshes)
			{
				CMesh * new_mesh = new CMesh();
				*new_mesh = *mesh;
				Meshes.push_back(new_mesh);
			}
			_shader = _models_loaded[i]->_shader;
			Logging::LogMessage(LogLevel_Debug, 
				"Model \"%s\" has already been loaded, skipping", _path.c_str());
			return;
		}
	}
	loadModel(path);
	_models_loaded.push_back(this);
	Logging::LogMessage(LogLevel_Debug, "Loaded model \"%s\"", _path.c_str());
}

CModel::~CModel()
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

void CModel::SetPosition(glm::vec3 pos)
{
	Position = pos;
	for (uint32_t i = 0; i < Meshes.size(); i++)
		Meshes[i]->SetPosition(pos);
}

glm::vec3 CModel::GetPosition()
{
	return Position;
}

void CModel::SetAngles(glm::vec3 ang)
{
	CBaseObject::SetAngles(ang);
	for (uint32_t i = 0; i < Meshes.size(); i++)
		Meshes[i]->SetAngles(ang);
}

glm::vec3 CModel::GetScale()
{
	return Scale;
}

void CModel::SetScale(glm::vec3 scale)
{
	Scale = scale;
	for (uint32_t i = 0; i < Meshes.size(); i++)
		Meshes[i]->SetScale(scale);
}

void CModel::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene * scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Logging::LogMessage(LogLevel_Error, 
			"Failed loading model \"%s\": %s\n", importer.GetErrorString());
		return;
	}
	_directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void CModel::processNode(aiNode * node, const aiScene * scene)
{
	for (uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
		Meshes.push_back(processMesh(mesh, scene));
	}
	for (uint32_t i = 0; i < node->mNumChildren; i++)
		processNode(node->mChildren[i], scene);
}

CMesh * CModel::processMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<VertexData> vertices;
	std::vector<GLuint> indices;
	std::vector<CMaterial *> materials;

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

	return new CMesh(vertices, indices, materials);
}

std::vector<CMaterial *> CModel::loadMaterials(aiMaterial * mat)
{
	std::vector<CMaterial *> mats;

	for (uint32_t i = 0; i < mat->GetTextureCount(aiTextureType_DIFFUSE); i++)
	{
		CTexture * diffuse = nullptr;
		CTexture * mrmap = nullptr;
		CMaterial * material;

		//Check if the texture has already been loaded
		aiString str;
		mat->GetTexture(aiTextureType_DIFFUSE, i, &str);

		std::string path = _directory + "/" + std::string(str.C_Str());
		diffuse = CTexture::TextureExists(path);
		if (!diffuse) diffuse = new CTexture(path);

		//Specular is actually MR map
		if (i < mat->GetTextureCount(aiTextureType_SPECULAR))
		{
			//Check if the specular map has already been loaded
			mat->GetTexture(aiTextureType_SPECULAR, i, &str);

			std::string path = _directory + "/" + std::string(str.C_Str());

			mrmap = CTexture::TextureExists(path);
			if (!mrmap) mrmap = new CTexture(path);
		}
		//Create a material
		material = new CMaterial(diffuse, mrmap);
		mats.push_back(material);
	}

	return mats;
}
