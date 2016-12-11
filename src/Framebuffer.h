#pragma once

typedef unsigned int GLuint;

class Texture;

class CFramebuffer
{
public:
	//If read is true, then the fbo must be read in a shader, and uses textures rather than RBO
	CFramebuffer(int multisample = 0, bool has_depth = true, bool has_stencil = false);
	~CFramebuffer();

	void Bind(bool read = true, bool draw = true);
	static void BindDefault();

	void BindFramebufferTexture();
protected:
	//The openGL framebuffer object
	GLuint _fbo;
	//if used, the openGL renderbuffer object attached to this framebuffer
	GLuint _rbo;
	//the openGL texture attached to this framebuffer - used for color
	GLuint _texture;
};

