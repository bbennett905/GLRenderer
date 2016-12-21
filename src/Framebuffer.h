#pragma once

#include <stdint.h>

typedef unsigned int GLuint;

class Texture;

struct FramebufferCreateInfo
{
	uint16_t Width;
	uint16_t Height;

	uint8_t Multisampling = 0;

	bool HasColor = true;
	bool IsColorReadable = true;
	bool HasDepth = true;
	bool IsDepthReadable = false;
	bool HasStencil = false;
	bool IsStencilReadable = false;

	FramebufferCreateInfo(uint16_t width, uint16_t height, uint8_t msaa = 0,
		bool color = true, bool color_read = true,
		bool depth = true, bool depth_read = false,
		bool stencil = false, bool stencil_read = false) :
		Width(width), Height(height), Multisampling(msaa), 
		HasColor(color), IsColorReadable(color_read),
		HasDepth(depth), IsDepthReadable(depth_read),
		HasStencil(stencil), IsStencilReadable(stencil_read) { }
};

class CFramebuffer
{
public:
	//If read is true, then the fbo must be read in a shader, and uses textures rather than RBO
	//CFramebuffer(int multisample = 0, bool has_depth = true, bool has_stencil = false);
	CFramebuffer(FramebufferCreateInfo* info);
	~CFramebuffer();

	void Bind(bool read = true, bool draw = true);
	static void BindDefault();

	//void BindFramebufferTexture();
	//Binds the color texture, if it exists (used for color
	void BindFramebufferColorTexture();
	//Binds the alt texture, if it exists (used for depth/stencil)
	void BindFramebufferAltTexture();

protected:
	//Check the FramebufferCreateInfo for any inconsistencies - returns true if 
	static bool validateCreateInfo(FramebufferCreateInfo* info);
	//The openGL framebuffer object
	GLuint _fbo;
	//if used, the openGL renderbuffer object attached to this framebuffer
	//GLuint _rbo;
	//the openGL texture attached to this framebuffer - used for color
	//GLuint _texture;

	GLuint _color_texture;
	GLuint _color_rbo;
	GLuint _alt_texture;
	GLuint _alt_rbo;
};

