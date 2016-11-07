#include "Skybox.h"

#include <gtc\type_ptr.hpp>

#include "Texture.h"
#include "Shader.h"
#include "Camera.h"

GLfloat skyboxVertices[] = {
	// Positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};

Skybox::Skybox(std::string path)
{
	Flags |= Drawable_Skybox;
	for (int i = 0; i < (sizeof(skyboxVertices) / sizeof(skyboxVertices[0])); i += 3)// ? ok
	{
		VertexData data = { glm::vec3(skyboxVertices[i], skyboxVertices[i + 1], skyboxVertices[i + 2]),	//Pos
			glm::vec3(0.0f, 0.0f, 0.0f),	//Norm
			glm::vec2(0.0f, 0.0f) };		//TexCoord
		Vertices.push_back(data);
	}
	_texture = new Texture(path, Texture_Cubemap);
}

Skybox::~Skybox()
{
}

void Skybox::Draw(Camera * camera)
{
	glDepthFunc(GL_LEQUAL);
	ShaderObj->Use();

	glBindVertexArray(VertexArrayObj);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(ShaderObj->GetUniformLocation("skybox"), 0);
	_texture->Bind();
	glDrawArrays(GL_TRIANGLES, 0, Vertices.size());
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDepthFunc(GL_LESS);
}
