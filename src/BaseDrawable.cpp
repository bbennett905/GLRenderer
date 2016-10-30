#include "BaseDrawable.h"

#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Camera.h"
#include "Material.h"
#include "Shader.h"
#include "Lights.h"
#include "Texture.h"

BaseDrawable::BaseDrawable() { }

BaseDrawable::BaseDrawable(const GLfloat vertices[], int verticesSize, Material * mat)
{
	Materials.push_back(mat);
	for (int i = 0; i < verticesSize; i += 8)// ? ok
	{
		VertexData data = { glm::vec3(vertices[i], vertices[i+1], vertices[i+2]),	//Pos
							glm::vec3(vertices[i+3], vertices[i+4], vertices[i+5]),	//Norm
							glm::vec2(vertices[i+6], vertices[i+7]) };				//TexCoord
		Vertices.push_back(data);
	}
}

BaseDrawable::BaseDrawable(const GLfloat vertices[], int verticesSize, 
	std::vector<Material *> & mat) :
	Materials(mat)
{
	for (int i = 0; i < verticesSize; i += 8)// ? ok
	{
		VertexData data = { glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]),	//Pos
			glm::vec3(vertices[i + 3], vertices[i + 4], vertices[i + 5]),	//Norm
			glm::vec2(vertices[i + 6], vertices[i + 7]) };				//TexCoord
		Vertices.push_back(data);
	}
}

BaseDrawable::BaseDrawable(std::vector<VertexData> vert) :
	Vertices(vert)
{ }

BaseDrawable::BaseDrawable(std::vector<VertexData> & vert, std::vector<GLuint> & ind,
	std::vector<Material *> & mat) :
	Vertices(vert), Indices(ind), Materials(mat)
{ 
	assert(mat.size() < MAX_MATERIALS && "An object exceeded MAX_MATERIALS!");
}

BaseDrawable::~BaseDrawable()
{
	//We DON'T destroy the shader, because they may be shared with other objects
	//Same with materials/textures
	glDeleteVertexArrays(1, &VertexArrayObj);
	glDeleteBuffers(1, &VertexBufferObj);
	glDeleteBuffers(1, &ElementBufferObj);
}

glm::mat4 BaseDrawable::GetModelMatrix()
{
	return glm::mat4(); //Return an identity matrix, no transform
}

void BaseDrawable::Draw(Camera * camera, 
	std::vector<LightPoint *> & point_light_list,
	std::vector<LightSpot *> & spot_light_list, 
	LightDirectional * directional_light)
{
	ShaderObj->Use();

	if (Materials.size())
		glUniform1i(glGetUniformLocation(ShaderObj->GetProgram(), "hasMaterials"), 1);
	else
		glUniform1i(glGetUniformLocation(ShaderObj->GetProgram(), "hasMaterials"), 0);
	for (uint32_t i = 0; i < Materials.size(); i++)
	{
		//The reason the shader works even when one of these uniforms isn't set is because
		//Sampler2Ds in GLSL are guaranteed to return black if there's no texture unit bound.
		if (Materials[i]->DiffuseMap != nullptr)
		{
			glUniform1i(glGetUniformLocation(ShaderObj->GetProgram(),
				("materials[" + std::to_string(i) + "].HasDiffMap").c_str()), 1);
			glActiveTexture(GL_TEXTURE0 + ShaderObj->TextureCount);
			Materials[i]->DiffuseMap->Bind();
			glUniform1i(glGetUniformLocation(ShaderObj->GetProgram(),
				("materials[" + std::to_string(i) + "].DiffMap").c_str()), 
				ShaderObj->TextureCount);
			ShaderObj->TextureCount++;
		}
		else
		{
			glUniform1i(glGetUniformLocation(ShaderObj->GetProgram(),
				("materials[" + std::to_string(i) + "].HasDiffMap").c_str()), 0);
		}

		if (Materials[i]->SpecularMap != nullptr)
		{
			glUniform1i(glGetUniformLocation(ShaderObj->GetProgram(),
				("materials[" + std::to_string(i) + "].HasSpecMap").c_str()), 1);
			glActiveTexture(GL_TEXTURE0 + ShaderObj->TextureCount);
			Materials[i]->SpecularMap->Bind();
			glUniform1i(glGetUniformLocation(ShaderObj->GetProgram(),
				("materials[" + std::to_string(i) + "].SpecMap").c_str()),
				ShaderObj->TextureCount);
			ShaderObj->TextureCount++;
		}
		else
		{
			glUniform1i(glGetUniformLocation(ShaderObj->GetProgram(),
				("materials[" + std::to_string(i) + "].HasSpecMap").c_str()), 0);
		}

		glUniform1f(glGetUniformLocation(ShaderObj->GetProgram(),
			("materials[" + std::to_string(i) + "].AmbientStrength").c_str()), 
			Materials[i]->AmbientStrength);
		glUniform1f(glGetUniformLocation(ShaderObj->GetProgram(),
			("materials[" + std::to_string(i) + "].DiffuseStrength").c_str()), 
			Materials[i]->DiffuseStrength);
		glUniform1f(glGetUniformLocation(ShaderObj->GetProgram(),
			("materials[" + std::to_string(i) + "].SpecularStrength").c_str()),
			Materials[i]->SpecularStrength);
		glUniform1f(glGetUniformLocation(ShaderObj->GetProgram(),
			("materials[" + std::to_string(i) + "].Shininess").c_str()), 
			Materials[i]->Shininess);
	}

	glUniform1i(glGetUniformLocation(ShaderObj->GetProgram(), "numMaterials"), 
		ShaderObj->TextureCount);

	//model matrix transforms model space to world space - rotation and translation
	glUniformMatrix4fv(glGetUniformLocation(ShaderObj->GetProgram(), "model"), 1, GL_FALSE, 
		glm::value_ptr(GetModelMatrix()));

	//view matrix moves the world relative to the camera - rotation + translation
	glUniformMatrix4fv(glGetUniformLocation(ShaderObj->GetProgram(), "view"), 1, GL_FALSE, 
		glm::value_ptr(camera->GetViewMatrix()));
	//projection matrix is the projection of the camera, perspective or orthogonal
	glUniformMatrix4fv(glGetUniformLocation(ShaderObj->GetProgram(), "projection"), 1, GL_FALSE, 
		glm::value_ptr(camera->GetProjMatrix()));

	glUniform3f(glGetUniformLocation(ShaderObj->GetProgram(), "viewPos"),
		camera->GetPos().x, camera->GetPos().y, camera->GetPos().z);

	//TODO all this could be moved to a private SceneRenderer::setLightUniforms method
	for (uint32_t i = 0; i < point_light_list.size(); i++)
	{
		glUniform3f(glGetUniformLocation(ShaderObj->GetProgram(), 
			("pointLights[" + std::to_string(i) + "].Position").c_str()),
			point_light_list[i]->Position.x, point_light_list[i]->Position.y, 
			point_light_list[i]->Position.z);
		glUniform3f(glGetUniformLocation(ShaderObj->GetProgram(), 
			("pointLights[" + std::to_string(i) + "].Color").c_str()),
			point_light_list[i]->Color.x, point_light_list[i]->Color.y, 
			point_light_list[i]->Color.z);
		glUniform1f(glGetUniformLocation(ShaderObj->GetProgram(), 
			("pointLights[" + std::to_string(i) + "].Intensity").c_str()),
			point_light_list[i]->Intensity);
		glUniform1f(glGetUniformLocation(ShaderObj->GetProgram(), 
			("pointLights[" + std::to_string(i) + "].Constant").c_str()),
			point_light_list[i]->Constant);
		glUniform1f(glGetUniformLocation(ShaderObj->GetProgram(), 
			("pointLights[" + std::to_string(i) + "].Linear").c_str()),
			point_light_list[i]->Linear);
		glUniform1f(glGetUniformLocation(ShaderObj->GetProgram(), 
			("pointLights[" + std::to_string(i) + "].Quadratic").c_str()),
			point_light_list[i]->Quadratic);
	}

	for (uint32_t i = 0; i < spot_light_list.size(); i++)
	{
		glUniform3f(glGetUniformLocation(ShaderObj->GetProgram(), 
			("spotLights[" + std::to_string(i) + "].Position").c_str()),
			spot_light_list[i]->Position.x, spot_light_list[i]->Position.y, 
			spot_light_list[i]->Position.z);
		glUniform3f(glGetUniformLocation(ShaderObj->GetProgram(), 
			("spotLights[" + std::to_string(i) + "].Direction").c_str()),
			spot_light_list[i]->GetForward().x, spot_light_list[i]->GetForward().y, 
			spot_light_list[i]->GetForward().z);
		glUniform3f(glGetUniformLocation(ShaderObj->GetProgram(), 
			("spotLights[" + std::to_string(i) + "].Color").c_str()),
			spot_light_list[i]->Color.x, spot_light_list[i]->Color.y, 
			spot_light_list[i]->Color.z);
		glUniform1f(glGetUniformLocation(ShaderObj->GetProgram(), 
			("spotLights[" + std::to_string(i) + "].InnerCutOff").c_str()),
			glm::cos(glm::radians(spot_light_list[i]->InnerCutOff)));
		glUniform1f(glGetUniformLocation(ShaderObj->GetProgram(), 
			("spotLights[" + std::to_string(i) + "].OuterCutOff").c_str()),
			glm::cos(glm::radians(spot_light_list[i]->OuterCutOff)));
		glUniform1f(glGetUniformLocation(ShaderObj->GetProgram(), 
			("spotLights[" + std::to_string(i) + "].Intensity").c_str()),
			spot_light_list[i]->Intensity);
		glUniform1f(glGetUniformLocation(ShaderObj->GetProgram(), 
			("spotLights[" + std::to_string(i) + "].Constant").c_str()),
			spot_light_list[i]->Constant);
		glUniform1f(glGetUniformLocation(ShaderObj->GetProgram(), 
			("spotLights[" + std::to_string(i) + "].Linear").c_str()),
			spot_light_list[i]->Linear);
		glUniform1f(glGetUniformLocation(ShaderObj->GetProgram(), 
			("spotLights[" + std::to_string(i) + "].Quadratic").c_str()),
			spot_light_list[i]->Quadratic);
	}
	glUniform3f(glGetUniformLocation(ShaderObj->GetProgram(), "directionalLight.Direction"),
		directional_light->GetForward().x, directional_light->GetForward().y, 
		directional_light->GetForward().z);
	glUniform3f(glGetUniformLocation(ShaderObj->GetProgram(), "directionalLight.Color"), 
		directional_light->Color.x, directional_light->Color.y, 
		directional_light->Color.z);
	glUniform1f(glGetUniformLocation(ShaderObj->GetProgram(), "directionalLight.Intensity"), 
		directional_light->Intensity);
	glUniform3f(glGetUniformLocation(ShaderObj->GetProgram(), "directionalLight.AmbientColor"), 
		directional_light->AmbientColor.x, directional_light->AmbientColor.y,
		directional_light->AmbientColor.z);
	glUniform1f(glGetUniformLocation(ShaderObj->GetProgram(), "directionalLight.AmbientIntensity"), 
		directional_light->AmbientIntensity);

	//Bind our VAO so we have the correct vertex attribute configuration
	glBindVertexArray(VertexArrayObj);
	//Draw! - type of primitive, starting index of vertex array, number of vertices
	if (Indices.size() > 0) glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
	else glDrawArrays(GL_TRIANGLES, 0, Vertices.size());
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}
