#include "Framebuffer.h"

#include <map>
#include <glew.h>

#include "Window.h"
#include "Logging.h"

CFramebuffer::CFramebuffer(bool has_depth, bool has_stencil) :
	_rbo(0)
{
	//So assume color is always readable, RGBA (texture)
	//Depth and stencil are assumed to be not readable (rbo)- doesn't seem like we need that (yet)

	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	//Prepare color attachment - texture
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
		Window::GetWidth(), Window::GetHeight(), 0,
		GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, _texture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//If has depth and stencil prepare depth24stencil8 rbo
	if (has_depth && has_stencil)
	{
		//Use a renderbuffer
		glGenRenderbuffers(1, &_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
			Window::GetWidth(), Window::GetHeight());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
			GL_RENDERBUFFER, _rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
	else
	{
		if (has_depth)
		{
			//Use a renderbuffer
			glGenRenderbuffers(1, &_rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
				Window::GetWidth(), Window::GetHeight());
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
				GL_RENDERBUFFER, _rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}
		if (has_stencil)
		{
			//Use a renderbuffer
			glGenRenderbuffers(1, &_rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8,
				Window::GetWidth(), Window::GetHeight());
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
				GL_RENDERBUFFER, _rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}
	}
	
	//TODO remove this when done testing
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Logging::LogMessage(LogLevel_Error, "Framebuffer (%d) is not complete!", _fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

CFramebuffer::~CFramebuffer()
{
	if (_rbo)
		glDeleteRenderbuffers(0, &_rbo);
	if (_texture)
		glDeleteTextures(1, &_texture);
}

void CFramebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
}

void CFramebuffer::BindDefault()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
