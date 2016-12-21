#include "Framebuffer.h"

#include <map>
#include <glew.h>

#include "Window.h"
#include "Logging.h"

//for screen framebuffer - color, multisample, depth w/ screen res
//shadow map fb - depth w/ ~1024sq res

//params:
//size (width, height)
//multisampling
//has map (color, depth, stencil)
//is readable (color, depth, stencil)

/*CFramebuffer::CFramebuffer(int multisample, bool has_depth, bool has_stencil) :
	_rbo(0)
{
	//So assume color is always readable, RGBA (texture)
	//Depth and stencil are assumed to be not readable (rbo)- doesn't seem like we need that (yet)

	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	//Prepare color attachment - texture
	glGenTextures(1, &_texture);
	if (multisample)
	{
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _texture);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, multisample, GL_RGBA8,
			Window::GetWidth(), Window::GetHeight(), GL_TRUE);
		//glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D_MULTISAMPLE, _texture, 0);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, _texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
			Window::GetWidth(), Window::GetHeight(), 0,
			GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, _texture, 0);
	}
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
			if (multisample)
			{
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, multisample, GL_DEPTH_COMPONENT24,
					Window::GetWidth(), Window::GetHeight());
			}
			else
			{
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
					Window::GetWidth(), Window::GetHeight());
			}
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
				GL_RENDERBUFFER, _rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}
		if (has_stencil)
		{
			//Use a renderbuffer
			glGenRenderbuffers(1, &_rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
			if (multisample)
			{
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, multisample, GL_STENCIL_INDEX8,
					Window::GetWidth(), Window::GetHeight());
			}
			else
			{
				glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8,
					Window::GetWidth(), Window::GetHeight());
			}
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
				GL_RENDERBUFFER, _rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
*/
CFramebuffer::CFramebuffer(FramebufferCreateInfo* info) :
	_color_rbo(0), _color_texture(0), _alt_texture(0), _alt_rbo(0)
{
	if (!validateCreateInfo(info))
	{
		Logging::LogMessage(LogLevel_Error, "Invalid FramebufferCreateInfo!");
		return;
	}

	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	if (info->HasColor)
	{
		if (info->IsColorReadable)
		{
			glGenTextures(1, &_color_texture);
			if (info->Multisampling)
			{
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _color_texture);
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, info->Multisampling, GL_RGBA8,
					info->Width, info->Height, GL_TRUE);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
					GL_TEXTURE_2D_MULTISAMPLE, _color_texture, 0);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, _color_texture);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
					info->Width, info->Height, 0,
					GL_RGBA, GL_UNSIGNED_BYTE, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
					GL_TEXTURE_2D, _color_texture, 0);
			}
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else //IsColorReadable
		{
			glGenRenderbuffers(1, &_color_rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, _color_rbo);
			if (info->Multisampling)
			{
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, info->Multisampling,
					GL_COLOR_ATTACHMENT0, info->Width, info->Height);
			}
			else
			{
				glRenderbufferStorage(GL_RENDERBUFFER, GL_COLOR_ATTACHMENT0, 
					info->Width, info->Height);
			}
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
				GL_RENDERBUFFER, _color_rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}
	} //HasColor

	if (info->HasDepth && info->HasStencil)
	{
		if (info->IsDepthReadable)
		{
			glGenTextures(1, &_alt_texture);
			if (info->Multisampling)
			{
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _alt_texture);
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, info->Multisampling,
					GL_DEPTH24_STENCIL8, info->Width, info->Height, GL_TRUE);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
					GL_TEXTURE_2D_MULTISAMPLE, _alt_texture, 0);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, _alt_texture);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, info->Width, info->Height, 0,
					GL_DEPTH_STENCIL, GL_UNSIGNED_BYTE, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
					GL_TEXTURE_2D, _alt_texture, 0);
			}
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else //IsDepthReadable
		{
			glGenRenderbuffers(1, &_alt_rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, _alt_rbo);
			if (info->Multisampling)
			{
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, info->Multisampling,
					GL_DEPTH24_STENCIL8, info->Width, info->Height);
			}
			else
			{
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
					info->Width, info->Height);
			}
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
				GL_RENDERBUFFER, _alt_rbo);
		}
	} 
	else //HasDepth && HasStencil
	{
		if (info->HasDepth)
		{
			if (info->IsDepthReadable)
			{
				glGenTextures(1, &_alt_texture);
				if (info->Multisampling)
				{
					glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _alt_texture);
					glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, info->Multisampling,
						GL_DEPTH_COMPONENT24, info->Width, info->Height, GL_TRUE);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
						GL_TEXTURE_2D_MULTISAMPLE, _alt_texture, 0);
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D, _alt_texture);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, info->Width, info->Height, 0,
						GL_DEPTH, GL_UNSIGNED_BYTE, NULL);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
						GL_TEXTURE_2D, _alt_texture, 0);
				}
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			else //IsDepthReadable
			{
				glGenRenderbuffers(1, &_alt_rbo);
				glBindRenderbuffer(GL_RENDERBUFFER, _alt_rbo);
				if (info->Multisampling)
				{
					glRenderbufferStorageMultisample(GL_RENDERBUFFER, info->Multisampling,
						GL_DEPTH_COMPONENT24, info->Width, info->Height);
				}
				else
				{
					glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
						info->Width, info->Height);
				}
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
					GL_RENDERBUFFER, _alt_rbo);
			}

		} //HasDepth
		if (info->HasStencil)
		{
			if (info->IsStencilReadable)
			{
				glGenTextures(1, &_alt_texture);
				if (info->Multisampling)
				{
					glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _alt_texture);
					glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, info->Multisampling,
						GL_STENCIL_INDEX8, info->Width, info->Height, GL_TRUE);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
						GL_TEXTURE_2D_MULTISAMPLE, _alt_texture, 0);
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D, _alt_texture);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX8, info->Width, info->Height, 0,
						GL_STENCIL, GL_UNSIGNED_BYTE, NULL);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
						GL_TEXTURE_2D, _alt_texture, 0);
				}
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			else
			{
				glGenRenderbuffers(1, &_alt_rbo);
				glBindRenderbuffer(GL_RENDERBUFFER, _alt_rbo);
				if (info->Multisampling)
				{
					glRenderbufferStorageMultisample(GL_RENDERBUFFER, info->Multisampling,
						GL_STENCIL_INDEX8, info->Width, info->Height);
				}
				else
				{
					glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8,
						info->Width, info->Height);
				}
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
					GL_RENDERBUFFER, _alt_rbo);
			}
		} //HasStencil
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

CFramebuffer::~CFramebuffer()
{
	if (_color_rbo)
		glDeleteRenderbuffers(1, &_color_rbo);
	if (_color_texture)
		glDeleteTextures(1, &_color_texture);
	if (_alt_rbo)
		glDeleteRenderbuffers(1, &_alt_rbo);
	if (_alt_texture)
		glDeleteTextures(1, &_alt_texture);
}

void CFramebuffer::Bind(bool read, bool draw)
{
	if (read && draw)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		return;
	}
	if (read)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo);
		return;
	}
	if (draw)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
		return;
	}
}

void CFramebuffer::BindDefault()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
/*
void CFramebuffer::BindFramebufferTexture()
{
	glBindTexture(GL_TEXTURE_2D, _texture);
}*/

void CFramebuffer::BindFramebufferColorTexture()
{
	if (_color_texture) glBindTexture(GL_TEXTURE_2D, _color_texture);
}

void CFramebuffer::BindFramebufferAltTexture()
{
	if (_alt_texture) glBindTexture(GL_TEXTURE_2D, _alt_texture);

}

bool CFramebuffer::validateCreateInfo(FramebufferCreateInfo* info)
{
	if ((info->HasDepth && info->HasStencil) && info->IsDepthReadable != info->IsStencilReadable)
	{
		return false;
	}
	return true;
}
