#include "CSkybox.h"

#include <gtc\type_ptr.hpp>

#include "CTexture.h"
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

CSkybox::CSkybox(std::string path)
{
	_flags |= Drawable_Skybox;
	for (int i = 0; i < (sizeof(skyboxVertices) / sizeof(skyboxVertices[0])); i += 3)// ? ok
	{
		VertexData data = { glm::vec3(skyboxVertices[i], skyboxVertices[i + 1], skyboxVertices[i + 2]),	//Pos
			glm::vec3(0.0f, 0.0f, 0.0f),	//Norm
			glm::vec2(0.0f, 0.0f) };		//TexCoord
		_vertices.push_back(data);
	}
	_texture = new CTexture(path, Texture_Cubemap);
}

CSkybox::~CSkybox()
{
}

void CSkybox::Draw()
{
	glDepthFunc(GL_LEQUAL);
	_shader->Use();

	glBindVertexArray(_vao);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(_shader->GetUniformLocation("skybox"), 0);
	_texture->Bind();
	glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDepthFunc(GL_LESS);
}
