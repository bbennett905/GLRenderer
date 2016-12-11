#include "ScreenFramebuffer.h"

#include <glew.h>

#include "Shader.h"
#include "Window.h"

GLfloat fbVertices[] = { 
	// Positions   // TexCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
};

CScreenFramebuffer::CScreenFramebuffer() :
	CFramebuffer(4), 
	_shader(new CShader("../shaders/screen_default.vert", "../shaders/screen_default.frag")),
	_intermediate_framebuffer(new CFramebuffer(0, false, false))
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fbVertices), &fbVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);
}

CScreenFramebuffer::~CScreenFramebuffer()
{
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
}

void CScreenFramebuffer::Draw()
{
	Bind(true, false);
	_intermediate_framebuffer->Bind(false, true);
	glBlitFramebuffer(0, 0, Window::GetWidth(), Window::GetHeight(),
		0, 0, Window::GetWidth(), Window::GetHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);

	BindDefault();
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	
	_shader->Use();
	glBindVertexArray(_vao);
	_intermediate_framebuffer->BindFramebufferTexture();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
