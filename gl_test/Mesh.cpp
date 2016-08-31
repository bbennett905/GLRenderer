#include "Mesh.h"

#include <gtc\matrix_transform.hpp>

Mesh::Mesh(std::vector<VertexData> vert, std::vector<GLuint> ind, std::vector<Texture> texts) :
	BaseDrawable(vert, ind), Textures(texts)
{
	MatObj = Material(new Shader("../shaders/default.vert", "../shaders/default.frag"));
}

glm::mat4 Mesh::GetModelMatrix()
{
	glm::mat4 model;
	model = glm::translate(model, Position);

	glm::vec3 ang = GetAngles();
	model = glm::rotate(model, glm::radians(ang.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(ang.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(ang.z), glm::vec3(0.0f, 0.0f, 1.0f));

	return model;
}


Model::Model(const char * path)
{
	loadModel(path);
}

std::vector<Mesh *> Model::GetMeshes()
{
	return _meshes;
}

void Model::SetPosition(glm::vec3 pos)
{
	Position = pos;
	for (uint32_t i = 0; i < _meshes.size(); i++)
		_meshes[i]->Position = pos; //i THINK this should work
}

glm::vec3 Model::GetPosition()
{
	return Position;
}

void Model::SetAngles(glm::vec3 ang)
{
	BaseObject::SetAngles(ang);
	for (uint32_t i = 0; i < _meshes.size(); i++)
		_meshes[i]->SetAngles(ang); //i THINK this should work
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
		_meshes.push_back(&processMesh(mesh, scene));
	}
	for (uint32_t i = 0; i < node->mNumChildren; i++)
		processNode(node->mChildren[i], scene);
}

Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<VertexData> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	
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
		//TODO here we want to load materials instead, or all texts into 1 mat
		//TODO how many materials / mesh? probably can be more than 1
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (uint32_t i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (uint32_t j = 0; j < _textures_loaded.size(); j++)
		{
			if (aiString(_textures_loaded[j].Path) == str)
			{
				textures.push_back(_textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture texture(str.C_Str());
			texture.Type = typeName;
			textures.push_back(texture);
			_textures_loaded.push_back(texture);
		}
	}
	return textures;
}
