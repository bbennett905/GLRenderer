#include "SceneRenderer.h"

#include <iostream>
#include "Window.h"

SceneRenderer::SceneRenderer(Window * window)
{
	glewExperimental = GL_TRUE;
	GLenum f = glewInit();
	if (f != GLEW_OK)
	{
		std::cout << "GLEW initialization failed!" << std::endl;
	}

	glViewport(0, 0, window->GetWidth(), window->GetHeight());
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//TODO remove this once cubemaps/skybox 
	_clear_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}


SceneRenderer::~SceneRenderer()
{
	//TODO we probably need to destroy things here (lights)?
}
