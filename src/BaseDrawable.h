#pragma once
#include "BaseObject.h"

#include <vector>
#include <glew.h>

#include "Material.h"
#include "Shader.h"
#include "Lights.h"

/*
 * Class that any 3D drawable object inherits from
 */

struct VertexData
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

enum DrawableFlags
{
	Drawable_Translucent = 1 << 0
};

//TODO add scale member, see Cube class for implementation

class SceneRenderer;
class Camera;

class BaseDrawable
{
public:
	BaseDrawable();
	BaseDrawable(const GLfloat vertices[], int verticesSize, Material * mat);
	BaseDrawable(const GLfloat vertices[], int verticesSize, std::vector<Material *> & mat);
	BaseDrawable(std::vector<VertexData> vert);
	BaseDrawable(std::vector<VertexData> & vert, std::vector<GLuint> & ind, std::vector<Material *> & mat);
	virtual ~BaseDrawable();

	//Returns the model transformation matrix this object should use
	virtual glm::mat4 GetModelMatrix();

	//TODO this will be called from a Scene object or somthing, that is where drawlist will be?
	virtual void Draw(Camera * camera, std::vector<LightPoint *> & point_light_list,
		std::vector<LightSpot *> & spot_light_list, LightDirectional * directional_light);

	//List of Vertices of the object
	std::vector<VertexData> Vertices;
	//List of Indices, if used by the object
	std::vector<GLuint> Indices;
	//List of Materials this obj uses
	std::vector<Material *> Materials;

	//The shader that this object uses
	Shader * ShaderObj;

	//Draw flags for this drawable
	uint32_t Flags;

	//OpenGL Vertex Array Object
	GLuint VertexArrayObj;
	//OpenGL Vertex Buffer Object
	GLuint VertexBufferObj;
	//OpenGL Element Buffer Object
	GLuint ElementBufferObj;
};
