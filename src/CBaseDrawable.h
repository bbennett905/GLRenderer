#pragma once

#include <vector>
#include <glew.h>
#include <glm.hpp>

#include "IDrawable.h"

/*
 * Class that any 3D drawable object inherits from
 */

class SceneRenderer;
class Camera;
class Shader;
class Material;
class LightPoint;
class LightSpot;
class LightDirectional;

class CBaseDrawable : 
	public IDrawable
{
public:
	CBaseDrawable();
	CBaseDrawable(const GLfloat vertices[], int verticesSize, Material * mat);
	CBaseDrawable(const GLfloat vertices[], int verticesSize, std::vector<Material *> & mat);
	CBaseDrawable(std::vector<VertexData> vert);
	CBaseDrawable(std::vector<VertexData> & vert, std::vector<GLuint> & ind, std::vector<Material *> & mat);
	virtual ~CBaseDrawable();

	//Returns the model transformation matrix this object should use
	virtual glm::mat4 GetModelMatrix();
	//Draw the object
	virtual void Draw();

	//Vertex data (pos, normal, texture coords)
	virtual std::vector<VertexData>& GetVertices();
	//Vertex indices
	virtual std::vector<GLuint>& GetIndices();
	//Materials used to draw this
	virtual std::vector<Material *>& GetMaterials();

	//Returns a pointer to the shader object to be used
	virtual Shader*& GetShader();

	//Vertex array object
	virtual GLuint& VAO();
	//Vertex buffer object
	virtual GLuint& VBO();
	//Element buffer object
	virtual GLuint& EBO();

	//Bit flags indicating special draw considerations
	virtual uint32_t& DrawFlags();

protected:
	//Draw flags for this drawable
	uint32_t _flags;

	//List of Vertices of the object
	std::vector<VertexData> _vertices;
	//List of Indices, if used by the object
	std::vector<GLuint> _indices;
	//List of Materials this obj uses
	std::vector<Material *> _materials;

	//The shader that this object uses
	Shader * _shader;

	//OpenGL Vertex Array Object
	GLuint _vao;
	//OpenGL Vertex Buffer Object
	GLuint _vbo;
	//OpenGL Element Buffer Object
	GLuint _ebo;
};
