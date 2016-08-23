#include "Renderer.h"

Renderer::Renderer(Window & window, Camera * cam, LightSimple & light) :
	_window(window), _camera(cam), _light(light)
{
	glewExperimental = GL_TRUE;
	GLenum f = glewInit();
	if (f != GLEW_OK)
	{
		std::cout << "GLEW initialization failed!" << std::endl;
	}

	glViewport(0, 0, _window.GetWidth(), _window.GetHeight());
	glEnable(GL_DEPTH_TEST);
	ClearColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

Renderer::~Renderer()
{
	for (BaseDrawable * obj : _drawList)
	{
		glDeleteVertexArrays(1, &(obj->VertexArrayObj));
		glDeleteBuffers(1, &(obj->VertexBufferObj));
		delete obj;
	}
}

void Renderer::Draw()
{
	glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (auto obj : _drawList)
	{
		//Sets which shaderprogram we should use for rendering this
		obj->MatObj.ShaderObj->Use();
		//Set all the material uniforms in the shader
		if (obj->MatObj.TextureObj1 != nullptr)
		{
			glActiveTexture(GL_TEXTURE0);
			obj->MatObj.TextureObj1->Bind();
			glUniform1i(glGetUniformLocation(obj->MatObj.ShaderObj->Program, "material.Texture1"), 0);
		}
		if (obj->MatObj.TextureObj2 != nullptr)
		{
			glActiveTexture(GL_TEXTURE1);
			obj->MatObj.TextureObj2->Bind();
			glUniform1i(glGetUniformLocation(obj->MatObj.ShaderObj->Program, "material.Texture2"), 1);
			glUniform1f(glGetUniformLocation(obj->MatObj.ShaderObj->Program, "material.MixRatio"), obj->MatObj.MixRatio);
		}
		if (obj->MatObj.SpecularMap != nullptr)
		{
			glActiveTexture(GL_TEXTURE2);
			obj->MatObj.SpecularMap->Bind();
			glUniform1i(glGetUniformLocation(obj->MatObj.ShaderObj->Program, "material.SpecMap"), 2);
		}
		glUniform1f(glGetUniformLocation(obj->MatObj.ShaderObj->Program, "material.AmbientStrength"), obj->MatObj.AmbientStrength);
		glUniform1f(glGetUniformLocation(obj->MatObj.ShaderObj->Program, "material.DiffuseStrength"), obj->MatObj.DiffuseStrength);
		glUniform1f(glGetUniformLocation(obj->MatObj.ShaderObj->Program, "material.SpecularStrength"), obj->MatObj.SpecularStrength);
		glUniform1f(glGetUniformLocation(obj->MatObj.ShaderObj->Program, "material.Shininess"), obj->MatObj.Shininess);

		//model matrix is the matrix that transforms model space to world space - rotation and translation
		glUniformMatrix4fv(glGetUniformLocation(obj->MatObj.ShaderObj->Program, "model"), 1, GL_FALSE, glm::value_ptr(obj->GetModelMatrix()));

		//view matrix moves the world relative to the camera - rotation + translation
		glUniformMatrix4fv(glGetUniformLocation(obj->MatObj.ShaderObj->Program, "view"), 1, GL_FALSE, glm::value_ptr(_camera->GetViewMatrix()));
		//projection matrix is the projection of the camera, perspective or orthogonal
		glUniformMatrix4fv(glGetUniformLocation(obj->MatObj.ShaderObj->Program, "projection"), 1, GL_FALSE, glm::value_ptr(_camera->GetProjMatrix()));

		glUniform3f(glGetUniformLocation(obj->MatObj.ShaderObj->Program, "viewPos"), _camera->GetPos().x, _camera->GetPos().y, _camera->GetPos().z);

		glUniform3f(glGetUniformLocation(obj->MatObj.ShaderObj->Program, "lightPos"), _light.Position.x, _light.Position.y, _light.Position.z);
		glUniform3f(glGetUniformLocation(obj->MatObj.ShaderObj->Program, "lightColor"), _light.LightColor.x, _light.LightColor.y, _light.LightColor.z);
		glUniform1f(glGetUniformLocation(obj->MatObj.ShaderObj->Program, "lightIntensity"), _light.LightIntensity);
		glUniform3f(glGetUniformLocation(obj->MatObj.ShaderObj->Program, "ambientColor"), _light.AmbientColor.x, _light.AmbientColor.y, _light.AmbientColor.z);
		glUniform1f(glGetUniformLocation(obj->MatObj.ShaderObj->Program, "ambientIntensity"), _light.AmbientIntensity);
		//Bind our VAO so we have the correct vertex attribute configuration
		glBindVertexArray(obj->VertexArrayObj);
		//Draw! - type of primitive, starting index of vertex array, number of vertices
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Renderer::AddToDrawList(BaseDrawable * obj)
{
	_drawList.push_back(obj);

	glGenVertexArrays(1, &(obj->VertexArrayObj));
	glGenBuffers(1, &(obj->VertexBufferObj));

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(obj->VertexArrayObj);

	//bind the VBO to array_buffer - it is now actually what we want it to be
	glBindBuffer(GL_ARRAY_BUFFER, obj->VertexBufferObj);
	//copies vertex data into buffer's memory - last arg means data is not likely to change, or only rarely
	glBufferData(GL_ARRAY_BUFFER, obj->Vertices.size() * sizeof(GLfloat), &(obj->Vertices)[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
