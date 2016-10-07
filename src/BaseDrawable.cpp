#include "BaseDrawable.h"

#include <gtc\matrix_transform.hpp>

BaseDrawable::BaseDrawable() { }

BaseDrawable::BaseDrawable(const GLfloat vertices[], int verticesSize,
	Shader * shad, Material & mat) :
	ShaderObj(shad)
{
	Materials.push_back(mat);
	for (int i = 0; i < verticesSize; i+=8)// ? ok
	{
		VertexData data = { glm::vec3(vertices[i], vertices[i+1], vertices[i+2]),	//Pos
							glm::vec3(vertices[i+3], vertices[i+4], vertices[i+5]),	//Norm
							glm::vec2(vertices[i+6], vertices[i+7]) };				//TexCoord
		Vertices.push_back(data);
	}
}

BaseDrawable::BaseDrawable(const GLfloat vertices[], int verticesSize, 
	Shader * shad, std::vector<Material> & mat) :
	ShaderObj(shad), Materials(mat)
{
	for (int i = 0; i < verticesSize; i += 8)// ? ok
	{
		VertexData data = { glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]),	//Pos
			glm::vec3(vertices[i + 3], vertices[i + 4], vertices[i + 5]),	//Norm
			glm::vec2(vertices[i + 6], vertices[i + 7]) };				//TexCoord
		Vertices.push_back(data);
	}
}

BaseDrawable::BaseDrawable(std::vector<VertexData> vert, Shader * shad) :
	Vertices(vert), ShaderObj(shad)
{ }

BaseDrawable::BaseDrawable(std::vector<VertexData> & vert, std::vector<GLuint> & ind, Shader * shad,
	std::vector<Material> & mat) :
	Vertices(vert), Indices(ind), ShaderObj(shad), Materials(mat)
{ }

glm::mat4 BaseDrawable::GetModelMatrix()
{
	return glm::mat4(); //Return an identity matrix, no transform
}

//TODO make this all work
//will need to take a SceneRenderer or something, which holds Light data
//Scene calls this - basedrawable should also have flags - one for transparency
//so that transparent objects can actually WORK
//scene will draw objects in a specific order
void BaseDrawable::Draw(SceneRenderer * scene_renderer)
{
	//new method of handling materials
	ShaderObj->Use();

	for (uint32_t i = 0; i < Materials.size(); i++)
	{
		//TODO this will actually give it multiple copies of the same texture (will it)?
		if (Materials[i].DiffuseMap != nullptr)
		{
			glActiveTexture(GL_TEXTURE0 + ShaderObj->TextureCount);
			Materials[i].DiffuseMap->Bind();
			GLuint loc = glGetUniformLocation(ShaderObj->Program,
				("materials[" + std::to_string(i) + "].DiffMap").c_str());
			glUniform1i(loc, ShaderObj->TextureCount);
			ShaderObj->TextureCount++;
		}

		if (Materials[i].SpecularMap != nullptr)
		{
			glActiveTexture(GL_TEXTURE0 + ShaderObj->TextureCount);
			Materials[i].SpecularMap->Bind();
			glUniform1i(glGetUniformLocation(ShaderObj->Program,
				("materials[" + std::to_string(i) + "].SpecMap").c_str()),
				ShaderObj->TextureCount);
			ShaderObj->TextureCount++;
		}

		//It'd be nice if there was some way to check what params a shader has, before providing it
		glUniform1f(glGetUniformLocation(ShaderObj->Program,
			("materials[" + std::to_string(i) + "].AmbientStrength").c_str()), Materials[i].AmbientStrength);
		glUniform1f(glGetUniformLocation(ShaderObj->Program,
			("materials[" + std::to_string(i) + "].DiffuseStrength").c_str()), Materials[i].DiffuseStrength);
		glUniform1f(glGetUniformLocation(ShaderObj->Program,
			("materials[" + std::to_string(i) + "].SpecularStrength").c_str()), Materials[i].SpecularStrength);
		glUniform1f(glGetUniformLocation(ShaderObj->Program,
			("materials[" + std::to_string(i) + "].Shininess").c_str()), Materials[i].Shininess);
	}

	//model matrix is the matrix that transforms model space to world space - rotation and translation
	glUniformMatrix4fv(glGetUniformLocation(ShaderObj->Program, "model"), 1, GL_FALSE, glm::value_ptr(obj->GetModelMatrix()));

	//view matrix moves the world relative to the camera - rotation + translation
	glUniformMatrix4fv(glGetUniformLocation(ShaderObj->Program, "view"), 1, GL_FALSE, glm::value_ptr(_camera->GetViewMatrix()));
	//projection matrix is the projection of the camera, perspective or orthogonal
	glUniformMatrix4fv(glGetUniformLocation(ShaderObj->Program, "projection"), 1, GL_FALSE, glm::value_ptr(_camera->GetProjMatrix()));

	glUniform3f(glGetUniformLocation(ShaderObj->Program, "viewPos"), _camera->GetPos().x, _camera->GetPos().y, _camera->GetPos().z);

	glUniform1i(glGetUniformLocation(ShaderObj->Program, "numPointLights"), _lightPointList.size());
	for (uint32_t i = 0; i < _lightPointList.size(); i++)
	{
		glUniform3f(glGetUniformLocation(obj->ShaderObj->Program, ("pointLights[" + std::to_string(i) + "].Position").c_str()),
			_lightPointList[i]->Position.x, _lightPointList[i]->Position.y, _lightPointList[i]->Position.z);
		glUniform3f(glGetUniformLocation(obj->ShaderObj->Program, ("pointLights[" + std::to_string(i) + "].Color").c_str()),
			_lightPointList[i]->Color.x, _lightPointList[i]->Color.y, _lightPointList[i]->Color.z);
		glUniform1f(glGetUniformLocation(obj->ShaderObj->Program, ("pointLights[" + std::to_string(i) + "].Intensity").c_str()),
			_lightPointList[i]->Intensity);
		glUniform1f(glGetUniformLocation(obj->ShaderObj->Program, ("pointLights[" + std::to_string(i) + "].Constant").c_str()),
			_lightPointList[i]->Constant);
		glUniform1f(glGetUniformLocation(obj->ShaderObj->Program, ("pointLights[" + std::to_string(i) + "].Linear").c_str()),
			_lightPointList[i]->Linear);
		glUniform1f(glGetUniformLocation(obj->ShaderObj->Program, ("pointLights[" + std::to_string(i) + "].Quadratic").c_str()),
			_lightPointList[i]->Quadratic);
	}

	glUniform1i(glGetUniformLocation(obj->ShaderObj->Program, "numSpotLights"), _lightSpotList.size());
	for (uint32_t i = 0; i < _lightSpotList.size(); i++)
	{
		glUniform3f(glGetUniformLocation(obj->ShaderObj->Program, ("spotLights[" + std::to_string(i) + "].Position").c_str()),
			_lightSpotList[i]->Position.x, _lightSpotList[i]->Position.y, _lightSpotList[i]->Position.z);
		glUniform3f(glGetUniformLocation(obj->ShaderObj->Program, ("spotLights[" + std::to_string(i) + "].Direction").c_str()),
			_lightSpotList[i]->GetForward().x, _lightSpotList[i]->GetForward().y, _lightSpotList[i]->GetForward().z);
		glUniform3f(glGetUniformLocation(obj->ShaderObj->Program, ("spotLights[" + std::to_string(i) + "].Color").c_str()),
			_lightSpotList[i]->Color.x, _lightSpotList[i]->Color.y, _lightSpotList[i]->Color.z);
		glUniform1f(glGetUniformLocation(obj->ShaderObj->Program, ("spotLights[" + std::to_string(i) + "].InnerCutOff").c_str()),
			glm::cos(glm::radians(_lightSpotList[i]->InnerCutOff)));
		glUniform1f(glGetUniformLocation(obj->ShaderObj->Program, ("spotLights[" + std::to_string(i) + "].OuterCutOff").c_str()),
			glm::cos(glm::radians(_lightSpotList[i]->OuterCutOff)));
		glUniform1f(glGetUniformLocation(obj->ShaderObj->Program, ("spotLights[" + std::to_string(i) + "].Intensity").c_str()),
			_lightSpotList[i]->Intensity);
		glUniform1f(glGetUniformLocation(obj->ShaderObj->Program, ("spotLights[" + std::to_string(i) + "].Constant").c_str()),
			_lightSpotList[i]->Constant);
		glUniform1f(glGetUniformLocation(obj->ShaderObj->Program, ("spotLights[" + std::to_string(i) + "].Linear").c_str()),
			_lightSpotList[i]->Linear);
		glUniform1f(glGetUniformLocation(obj->ShaderObj->Program, ("spotLights[" + std::to_string(i) + "].Quadratic").c_str()),
			_lightSpotList[i]->Quadratic);
	}
	//Handle directional light
	glUniform3f(glGetUniformLocation(obj->ShaderObj->Program, "directionalLight.Direction"), _lightDir->GetForward().x, _lightDir->GetForward().y, _lightDir->GetForward().z);
	glUniform3f(glGetUniformLocation(obj->ShaderObj->Program, "directionalLight.Color"), _lightDir->Color.x, _lightDir->Color.y, _lightDir->Color.z);
	glUniform1f(glGetUniformLocation(obj->ShaderObj->Program, "directionalLight.Intensity"), _lightDir->Intensity);
	glUniform3f(glGetUniformLocation(obj->ShaderObj->Program, "directionalLight.AmbientColor"), _lightDir->AmbientColor.x, _lightDir->AmbientColor.y, _lightDir->AmbientColor.z);
	glUniform1f(glGetUniformLocation(obj->ShaderObj->Program, "directionalLight.AmbientIntensity"), _lightDir->AmbientIntensity);

	//Bind our VAO so we have the correct vertex attribute configuration
	glBindVertexArray(obj->VertexArrayObj);
	//Draw! - type of primitive, starting index of vertex array, number of vertices

	if (obj->Indices.size() > 0) glDrawElements(GL_TRIANGLES, obj->Indices.size(), GL_UNSIGNED_INT, 0);
	else glDrawArrays(GL_TRIANGLES, 0, obj->Vertices.size());
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}
