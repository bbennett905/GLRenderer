#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

Camera camera(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 90.0f, 0.0f));

void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		printf("angle %f\n", camera.GetAngles().y);
}

int main()
{
	//most glfw functions are self-explanatory
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	//this sets the opengl version to use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow * window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Testing", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Window creation failed!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	GLenum f = glewInit();
	if (f != GLEW_OK)
	{
		std::cout << "GLEW initialization failed!" << std::endl;
		glfwTerminate();
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	
	//Set where we handle input
	glfwSetKeyCallback(window, KeyCallback);

	//the vertices that we will render, in normalized device coordinates:
	//coordinates between -1.0f and 1.0f are visible on screen, anything outside of this is not visible
	/*GLfloat vertices[] = {
		// Positions			// Colors           // Texture Coords
		0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,   // Top Right
		0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,   // Bottom Right
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,   // Bottom Left
		-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f    // Top Left 
	}; */

	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	//indices used for the EBO, so we require fewer vertices
	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3,  // First Triangle
		1, 2, 3   // Second Triangle
	};

	Texture texture1("../images/container.jpg");
	Texture texture2("../images/awesomeface.png");

	Shader newShader("../shaders/test.vert", "../shaders/test.frag");

	glEnable(GL_DEPTH_TEST);

	//VBO - vertex buffer object: can store large # of verices in the gpu memory
	//allows for sending large amounts of data at the same time, rather than individually
	//VAO - vertex array object: stores vertex attribute calls
	//instead of configuring vertex attribute pointers every time, you can simply bind a VAO
	//EBO - element buffer object: stores indices of vertices - not currently used in this sample
	//used so that instead of having multiple overlapping vertices, each vertex is assigned an index
	//	which is used to specify which vertex is part of which triangle
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);

	//generate the vertex buffer object's ID
	glGenBuffers(1, &VBO);
	//same
	glGenBuffers(1, &EBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	//bind the VBO to array_buffer - it is now actually what we want it to be
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//copies vertex data into buffer's memory - last arg means data is not likely to change, or only rarely
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//bind the EBO to the element array buffer, and give it the index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//tell GL how to interpret vertex data - pass data to this vertex attribute (see vertex shader), size of (3 values, vec), type is float,
	//	normalize it to 0-1, space between consecutive vertex sets, offset of where the data begins in the buffer
	//should look up more about this
	//these are called after we bind the VAO - if we bind a different VAO and rebind this one, these calls are stored
	//the 0 here corresponds to the 0 for the vec3 position in vertex shader code
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//unbind VBO, we've given it the data it needs
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Unbind VAO, so we don't mistakenly call it - not necessary for this simple code, but good practice
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		//Go to the event callbacks specified before
		glfwPollEvents();

		glClearColor(0.7f, 1.0f, 0.7f, 1.0f);	//sets state
		//glClear(GL_COLOR_BUFFER_BIT);			//uses set state
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Time varying color
		GLfloat time = GLfloat(glfwGetTime());
		GLfloat greenValue = (sin(time * 1.3f) / 2) + 0.5f;
		GLfloat redValue = (sin(time * 1.5f) / 5) + 0.5f;
		GLfloat blueValue = (sin(time) * 1.0f) + 0.5f;
		
		//Sets which shaderprogram we should use for rendering this
		newShader.Use();
		//glUniform4f(glGetUniformLocation(newShader.Program, "ourColor"), redValue, greenValue, blueValue, 1.0f);
		
		glActiveTexture(GL_TEXTURE0);
		texture1.Bind();
		glUniform1i(glGetUniformLocation(newShader.Program, "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		texture2.Bind();
		glUniform1i(glGetUniformLocation(newShader.Program, "ourTexture2"), 1);

		//model matrix is the matrix that transforms model space to world space - rotation and translation
		glm::mat4 model;
		model = glm::rotate(model, glm::radians(GLfloat(glfwGetTime() * 25.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(GLfloat(glfwGetTime() * -40.0f)), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(newShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		//view matrix moves the world relative to the camera - rotation + translation
		glUniformMatrix4fv(glGetUniformLocation(newShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

		//projection matrix is the projection of the camera, perspective or orthogonal
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), float(SCREEN_WIDTH / SCREEN_HEIGHT), 0.1f, 1000.0f);
		glUniformMatrix4fv(glGetUniformLocation(newShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));		

		//Bind our VAO so we have the correct vertex attribute configuration
		glBindVertexArray(VAO);
		//Draw! - type of primitive, starting index of vertex array, number of vertices
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//Unbind VAO at end of call
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, 0);

		glfwSwapBuffers(window);
	}

	//Cleanup our things
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}