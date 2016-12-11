#pragma once
#include "Framebuffer.h"

class CShader;

class CScreenFramebuffer :
	public CFramebuffer
{
public:
	CScreenFramebuffer();
	~CScreenFramebuffer();

	void Draw();

protected:
	CShader* _shader;
	GLuint _vao;
	GLuint _vbo;

	CFramebuffer* _intermediate_framebuffer;
};

